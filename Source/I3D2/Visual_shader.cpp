#include "all.h"
#include "object.h"
#include "mesh.h"

//----------------------------

//----------------------------
//----------------------------

class I3D_object_shader : public I3D_object {
private:
    C_str shader_name;
public:
    I3D_object_shader(PI3D_driver d);

public:
    I3DMETHOD(Duplicate)(CPI3D_frame);
    I3DMETHOD(SetProperty)(dword index, dword value);
    I3DMETHOD_(dword, GetProperty)(dword index) const;
    I3DMETHOD_(PI3D_shader, GetShader)() const { return NULL; }
public:
    I3DMETHOD_(dword, Release)() { if (--ref) return ref; delete this; return 0; }
};

//----------------------------

I3D_object_shader::I3D_object_shader(PI3D_driver d) :
    I3D_object(d),
    shader_name("")
{
    visual_type = I3D_VISUAL_SHADER;
}

//----------------------------

I3D_RESULT I3D_object_shader::Duplicate(CPI3D_frame frm) {

    if (frm == this) return I3D_OK;
    if (frm->GetType1() != FRAME_VISUAL)
        return I3D_frame::Duplicate(frm);

    CPI3D_visual vis = I3DCAST_CVISUAL(frm);
    switch (vis->GetVisualType1()) {
    case I3D_VISUAL_SHADER:
    {
        I3D_object_shader* obj = (I3D_object_shader*)vis;
        shader_name = obj->shader_name;
    }
    break;
    }
    return I3D_object::Duplicate(vis);
}

//----------------------------

I3D_RESULT I3D_object_shader::SetProperty(dword index, dword value) {

    switch (index) {
    case 0: shader_name = (const char*)value;
        break;
    default: return I3DERR_INVALIDPARAMS;
    }
    return I3D_OK;
}

//----------------------------

dword I3D_object_shader::GetProperty(dword index) const {

    switch (index) {
    case 0: return (dword)(const char*)shader_name;
    }
    return 0xcdcdcdcd;
}

//----------------------------

extern const S_visual_property props_Shader[] = {
{I3DPROP_STRING, "Shader", "Shader file name"},
{I3DPROP_NULL}
};

I3D_visual* CreateShader(PI3D_driver drv) {
    return new I3D_object_shader(drv);
}

//----------------------------


