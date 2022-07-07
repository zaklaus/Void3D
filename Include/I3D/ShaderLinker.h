#pragma once

#include "C_unknwn.hpp"

enum class E_SHADER_LINKER_FLAGS{
   OPTIMIZERLEVEL_NONE,
   OPTIMIZERLEVEL_BASIC
};

class C_shader_linker: public C_unknown{
   dword ref;
public:
   virtual void ReserveConstantRange(dword start, dword end) = 0;
   virtual void ClearConstantRange() = 0;
   virtual HRESULT AddFragments(const char* buf, int sz) = 0;

   virtual HRESULT GetFragmentID(const char* name, dword* id) = 0;
   virtual HRESULT GetConstantID(const char* name, dword* id) = 0;
   virtual HRESULT GetConstantSlot(dword id, dword offset, dword* loc) = 0;
   virtual HRESULT CreateBinaryShader(dword* ids, LPD3DXBUFFER* buf) = 0;

   virtual HRESULT GetShaderSource(void** ret) = 0;
   virtual void AddHeaderCode(C_str code) = 0;
   virtual void ClearHeaderCode() = 0;
   virtual void SetVersion(C_str ver) = 0;

   dword AddRef() { return ++ref; }
   dword Release() { if (--ref) return ref; delete this; return 0; }
};

C_shader_linker *CreateShaderLinker(E_SHADER_LINKER_FLAGS flags);
