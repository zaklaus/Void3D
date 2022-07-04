#include "all.h"
#include "iscrpt_i.h"

//----------------------------
//----------------------------
                              //functions
PC_script ISLAPI CreateScript(){
   return new C_script;
}

//----------------------------
//----------------------------

C_script::C_script():
   ref(1),
   reloc_symbols(NULL),
   reloc_data(NULL),
   comp_ok(false)
{
}

//----------------------------

C_script::~C_script(){
   Free();
}

//----------------------------

void C_script::Free(){

   if(comp_ok){
      code.clear();
      data.clear();
      delete[] reloc_symbols; reloc_symbols = NULL;
      delete[] reloc_data; reloc_data = NULL;
      table_templ.clear();
      dependency.clear();
      debug_info.clear();
      comp_ok = false;
   }
}

//----------------------------

ISL_RESULT C_script::Compile(const char *fname, dword flags,
   const char *cmd_line,
   T_report_err *err_fnc1, void *cb_context){

   Free();
                              //load pre-compiled script
#ifdef SCRIPT_COMPILE
   /*ISL_RESULT ret = CompileInternal(fname, flags, cmd_line, err_fnc1, cb_context);
   if(ISL_SUCCESS(ret))
      RelocateTableTemplate();*/
   return ISLERR_GENERIC;
#else
   return ISLERR_NOFILE;
#endif 
}

//----------------------------

const void *C_script::GetAddress(const char *var_name) const{

   if(reloc_symbols){
      for(S_symbol_info *si = reloc_symbols; !si->IsLast(); ++si)
         if(!strcmp(si->GetName(), var_name)) break;
      if(!si->IsLast()){
                              //can only return pointer to data
         if(si->flags&SI_DATA){
            return data.begin() + si->GetAddress();
         }
      }
   }
   return NULL;
}

//----------------------------

static C_str ConvertString(const char *cp, int *scanned = NULL){

   C_vector<char> s;

   int i = 0;
   while(true){
      ++i;
      char c = *cp++;
      switch(c){
      case '\n':
         s.push_back('~');
         s.push_back('n');
         break;
      default:
         s.push_back(c);
      }
      if(!c)
         break;
   }
   if(scanned)
      *scanned = i;
   return C_fstr(" \"%s\"", s.begin());
}
