#include "all.h"
#include "object.h"
#include "mesh.h"

//----------------------------

//----------------------------
//----------------------------

class I3D_object_shader : public I3D_object {
public:
    I3D_object_shader(PI3D_driver d);

public:
    I3DMETHOD(Duplicate)(CPI3D_frame);
    I3DMETHOD(SetProperty)(dword index, dword value);
    I3DMETHOD_(dword, GetProperty)(dword index) const;
public:
    I3DMETHOD_(dword, Release)() { if (--ref) return ref; delete this; return 0; }
};

//----------------------------

I3D_object_shader::I3D_object_shader(PI3D_driver d) :
    I3D_object(d)
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
        I3D_object_shader* bobj = (I3D_object_shader*)vis;
        //@todo
    }
    break;
    }
    return I3D_object::Duplicate(vis);
}

//----------------------------

I3D_RESULT I3D_object_shader::SetProperty(dword index, dword value) {

    switch (index) {
    default: return I3DERR_INVALIDPARAMS;
    }
    return I3D_OK;
}

//----------------------------

dword I3D_object_shader::GetProperty(dword index) const {

    switch (index) {
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


