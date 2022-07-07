#include "all.h"
#include "I3D/ShaderLinker.h"

#include <string> // for std::string

class C_shader_linker_imp : public C_shader_linker{
public:
   C_shader_linker_imp(E_SHADER_LINKER_FLAGS flags1){
      ClearConstantRange();
      flags = flags1;
      current_constant_id = temp_var_id = 0;
      version = "vs_1_1";
      shd_count = 0;
   }

   // Inherited via C_shader_linker
   virtual void ReserveConstantRange(dword start, dword end) override
   {
      range_start = start;
      range_end = end;
   }

   // Inherited via C_shader_linker
   virtual void ClearConstantRange() override
   {
      range_start = 0;
      range_end = 96;
   }

   bool ReadStringLocal(const char *p, char* res){
      const char* beg = p;
      char c[2] = { *p, NULL };
      bool in_brackets = false;
      while (*p) {
         c[0] = *p;
         if (*p == '\n')
            return NULL;
         if (isalnum(*p) || strstr("_%", c)){
            break;
         }
         ++beg, ++p;
      }

      while (*p){
         c[0] = *p;
         if (*p == '[') {
            in_brackets = true;
         }
         if (*p == '\n')
            break;
         if (!in_brackets && !isalnum(*p) && !strstr("_%", c)) {
            break;
         }
         if (*p == ']'){
            in_brackets = false;
         }

         ++p;
      }
      
      strncpy(res, beg, p - beg);
      return (beg != p);
   }

   HRESULT AddFragments(const char* buf, int sz) override {
      (void)sz;

      const char* p = buf;
      while (*p){
         p = strstr(p, "#beginfragment ");
         if (!p) break;
         if (*(p - 1) == ';') {
            // skip commented out fragments
            p += strlen("#beginfragment ")+1;
            continue;
         }
         
         p += strlen("#beginfragment ");
         
         char fragName[64] = {};
         if (!ReadStringLocal(p, fragName))
            return E_FAIL;
         p += strlen(fragName);
         
         const char* fragCode = p;
         
         const char* fragEnd = strstr(p, "#endfragment");
         if (!fragEnd) return E_FAIL;
         
         int size = fragEnd - fragCode;
         char* code = new char[size+1];
         assert(code);
         code[size] = 0;
         strncpy(code, fragCode, size);

         char* cp = code;
         while (*cp) {
            if (*cp == ';') {
               do {
                  *cp++ = ' ';
               } while (*cp && *cp != '\n');
            }
            ++cp;
         }

         fragments.push_back(S_fragment{ fragName, code });
         delete[] code;
      }
      
      p = buf;
      while (*p) {
         p = strstr(p, "#macro ");
         if (!p) break;
         if (*(p - 1) == ';') continue; // skip commented out macros

         p += strlen("#macro ");

         char macroName[64] = {};
         if (!ReadStringLocal(p, macroName))
            return E_FAIL;
         p += strlen(macroName);

         C_vector<C_str> args;

         while(*p && *p != '\n'){
            char argName[64] = {};
            if (!ReadStringLocal(p, argName))
               break;
            p += strlen(argName);
            if (*p == ',') ++p;
            args.push_back(argName);
         }

         const char* macroCode = p;
         const char* macroEnd = strstr(p, "#endm");
         if (!macroEnd) return E_FAIL;

         int size = macroEnd - macroCode;
         char* code = new char[size+1];
         assert(code);
         code[size] = 0;
         strncpy(code, macroCode, size);

         char* cp = code;
         while (*cp) {
            if (*cp == ';') {
               do {
                  *cp++ = ' ';
               } while (*cp && *cp != '\n');
            }
            ++cp;
         }

         macros.push_back(S_macro{ macroName, code, args });
         delete[] code;
      }
      
      return S_OK;
   }

   C_str ProcessMacros(C_str code){
      C_str buf;

      const char* p = (const char*)code;
      bool found_macro = false;
      bool found_fill = false;
      int macro_cnt = 0;

      while (*p){
         for (auto& mac : macros){ // SLOW!
            found_macro = false;

            if (!strncmp(p, (const char*)mac.name, mac.name.Size())){
               found_macro = true;
               ++macro_cnt;
               p += mac.name.Size();

               C_vector<C_str> macro_fills;

               for (int j = 0; j < mac.args.size(); ++j){
                  char fill[64] = {};
                  if (!ReadStringLocal(p, fill))
                     return buf;

                  p += strlen(fill);
                  if (*p == ',') ++p;
                  
                  macro_fills.push_back(fill);
               }

               C_str macro_snippet;

               const char* k = (const char*)mac.code;

               while (*k){
                  int j = 0;
                  found_fill = false;
                  
                  if (*k == '%') {
                     ++k;
                     for (auto& arg : mac.args) {
                        if (!strncmp(k, (const char*)arg, arg.Size())) {
                           macro_snippet += macro_fills[j];
                           k += arg.Size();
                           found_fill = true;
                           break;
                        }
                        ++j;
                     }
                  }

                  if (!found_fill){
                     macro_snippet.AddChar(*k++);
                  }
               }

               const char* gb = (const char*)macro_snippet;
               buf += macro_snippet;
            }
         }

         if (!found_macro){
            buf.AddChar(*p++);
         }
      }

      if (macro_cnt > 0)
         return ProcessMacros(buf);
      
      return buf;
   }

   C_str ExpandAndAllocateConstants(C_str code) {
      C_str buf;

      const char* p = (const char*)code;
      bool found_constant = false;

      while (*p) {
         for (auto& cst : constants) { // SLOW!
            found_constant = false;

            if (!strncmp(p, (const char*)cst.name, cst.name.Size())) {
               found_constant = true;
               p += cst.name.Size();

               assert(current_constant_id + 1 <= range_end);

               S_used_constant *used_cst = NULL;
               
               for (int s = used_constants.size(); s--;) {
                  auto& csts = used_constants[s];
                  if (csts.id == cst.id){
                     used_cst = &csts;
                     break;
                  }
               }

               if (!used_cst || used_cst->needs_offset) {
                  if (used_cst){
                     used_cst->needs_offset = false;
                  }
                  used_constants.push_back(S_used_constant{ cst.name, cst.id, current_constant_id++, (used_cst) ? used_cst->offset + 1 : 0, false });
                  used_cst = &used_constants.back();
               }

               buf += C_fstr("c%d", used_cst->regid);

               while (*p){                  
                  if (*p == '+' && *(p + 1) == '+') {
                     p += 2;
                     used_cst->needs_offset = true;
                     break;
                  }

                  if (!isalnum(*p) && *p != '.')
                     break;
                  
                  buf.AddChar(*p++);
               }
            }
         }

         if (!found_constant) {
            buf.AddChar(*p++);
         }
      }

      return buf;
   }

   C_str ReplaceTempVars(C_str code) {
      C_str buf;

      const char* p = (const char*)code;
      bool found_var = false;

      while (*p) {
         found_var = false;
         if (!strncmp(p, "r_", 2)){
            found_var = true;

            char varName[64] = {};
            if (!ReadStringLocal(p, varName)){
               return buf;
            }

            size_t varsz = strlen(varName);

            p += varsz;

            S_tempvar* tmpvar = NULL;
            
            for (auto& var : tempvars){
               if (var.name == varName){
                  tmpvar = &var;
                  break;
               }
            }

            if (!tmpvar){
               tempvars.push_back(S_tempvar{ varName, temp_var_id++ });
               tmpvar = &tempvars.back();
            }


            buf += C_fstr("r%d", tmpvar->regid);
         }

         if (!found_var){
            buf.AddChar(*p++);
         }
      }

      return buf;
   }

   HRESULT CreateBinaryShader(dword* ids, LPD3DXBUFFER* buf) override {
      HRESULT hr = S_OK;

      C_vector<S_fragment> final_fragments;
      current_constant_id = range_start;
      used_constants.clear();
      tempvars.clear();
      temp_var_id = 0;

      //push used fragments
      for (int i = 0; ids[i]; ++i){
         const auto& frag = fragments[ids[i]];
         final_fragments.push_back(frag);
         //MessageBoxA(NULL, (const char*)frag.code, "dump", MB_OK);
      }

      //expand macros
      for (auto& frag : final_fragments){
         frag.code = ProcessMacros(frag.code);
         //MessageBoxA(NULL, (const char*)frag.code, "dump", MB_OK);
      }

      //expand and allocate constants
      for (auto& frag : final_fragments) {
         frag.code = ExpandAndAllocateConstants(frag.code);
         //MessageBoxA(NULL, (const char*)frag.code, "dump", MB_OK);
      }

      //replace temp variables
      for (auto& frag : final_fragments) {
         frag.code = ReplaceTempVars(frag.code);
         //MessageBoxA(NULL, (const char*)frag.code, "dump", MB_OK);
      }

      //concatenate shader code
      C_str shd_code = version + "\n" + hdr_code + "\n" + fragments[0].code;
      
      int j = 0;
      for (auto& frag : final_fragments) {
         shd_code += C_fstr("// Fragment: %s (%d)\n", (const char*)frag.name, j++);
         shd_code += frag.code;
      }
      
      //MessageBoxA(NULL, (const char*)shd_code, "dump", MB_OK);

      LPD3DXBUFFER errorBuf;
      hr = D3DXAssembleShader(shd_code, shd_code.Size(), NULL, NULL, 0, buf, &errorBuf);
      if (FAILED(hr)){
         char* cp = (char*)errorBuf->GetBufferPointer();
         int len = errorBuf->GetBufferSize();
         cp[len] = 0;
         MessageBoxA(NULL, cp, "Shader error", MB_OK);
         
         FILE* dumpHandle = fopen("shd_dump.txt", "wb");
         fwrite((const char*)shd_code, 1, shd_code.Size(), dumpHandle);
         fclose(dumpHandle);
         exit(1);
      }

      const auto fp = C_fstr("ShaderDump\\shd_%d.txt", shd_count++);
      FILE* dumpHandle = fopen((const char*)fp, "wb");
      if (dumpHandle) {
         fwrite((const char*)shd_code, 1, shd_code.Size(), dumpHandle);
         fclose(dumpHandle);
      }
      
      return hr;
   }

   void AddHeaderCode(C_str code) override{
      hdr_code += code + "\n";
   }
   void ClearHeaderCode() override{
      hdr_code = "\n";
   }
   
   void SetVersion(C_str ver) override{
      version = ver;
   }

   // Inherited via C_shader_linker
   virtual HRESULT GetFragmentID(const char* name, dword* id) override
   {
      int i = 0;
      for (auto& f : fragments){
         if (f.name == name){
            *id = i;
            return S_OK;
         }
         ++i;
      }
      
      return E_FAIL;
   }
   virtual HRESULT GetConstantID(const char* name, dword* id) override
   {
      constants.push_back(S_constant{ name, constants.size() });
      *id = constants.size()-1;
      
      return S_OK;
   }

   HRESULT GetConstantSlot(dword id, dword offset, dword* loc) override {
      if (id >= constants.size())
         return E_FAIL;

      for (auto& cst : used_constants) {
         if (cst.id == id && cst.offset == offset) {
            *loc = cst.regid;
            return S_OK;
         }
      }
            
      return E_FAIL;
   }

   HRESULT GetShaderSource(void** ret) override {
      return E_NOTIMPL;
   }

private:
   dword current_constant_id;
   dword temp_var_id;
   dword range_start, range_end;
   E_SHADER_LINKER_FLAGS flags;
   
   C_str hdr_code;
   C_str version;

   struct S_fragment{
      C_str name;
      C_str code;
   };

   struct S_constant{
      C_str name;
      dword id;
   };

   struct S_used_constant{
      C_str name;
      dword id, regid, offset;
      bool needs_offset;
   };

   struct S_macro{
      C_str name;
      C_str code;
      C_vector<C_str> args;
   };

   struct S_tempvar{
      C_str name;
      dword regid;
   };

   C_vector<S_constant> constants;
   C_vector<S_used_constant> used_constants;
   C_vector<S_fragment> fragments;
   C_vector<S_macro> macros;
   C_vector<S_tempvar> tempvars;

   //debug
   dword shd_count;
};

C_shader_linker* CreateShaderLinker(E_SHADER_LINKER_FLAGS flags) {
   return new C_shader_linker_imp(flags);
}