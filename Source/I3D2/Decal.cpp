#include "all.h"
#include "visual.h"
#include "mesh.h"

//----------------------------

//----------------------------
//----------------------------

class I3D_decal : public I3D_visual {
public:
	C_smart_ptr<I3D_material> mat;
	
	I3D_decal(PI3D_driver d): I3D_visual(d) {
		visual_type = I3D_VISUAL_DECAL;
	}

	// Inherited via I3D_visual
	virtual bool ComputeBounds() override
	{
		bound.bound_local.bbox.min = S_vector(-1.0f * .5f, -1.0f * .5f, -1.0f * .5f);
		bound.bound_local.bbox.max = S_vector(1.0f * .5f, 1.0f * .5f, 1.0f * .5f);
		bound.bound_local.bsphere.pos.Zero();
		bound.bound_local.bsphere.radius = .5f;

		vis_flags |= VISF_BOUNDS_VALID;
		frm_flags &= ~FRMFLAGS_BSPHERE_TRANS_VALID;
		return true;
	}
	virtual void AddPrimitives(S_preprocess_context& pc) override
	{
		if (pc.mode != RV_NORMAL)
			return;

		if (!mat)
			return;
		if (!(drv->GetFlags2() & (DRVF2_DRAWTEXTURES|DRVF2_USEDECALS)))
			return;

		pc.decal_casters.push_back(this);
	}

	void SetMeshInternal(PI3D_mesh_base mesh) override {
		if (mesh) {
			SetMaterial(mesh->GetFGroups1()->mat);
		}
		else {
			PI3D_material mat = drv->CreateMaterial();
			SetMaterial(mat);
			mat->Release();
		}
	}

	I3DMETHOD(Duplicate)(CPI3D_frame frm) {
		if (frm == this)
			return I3D_OK;
		if (frm->GetType1() != FRAME_VISUAL)
			return I3D_frame::Duplicate(frm);
		CPI3D_visual vis = I3DCAST_CVISUAL(frm);
		switch (vis->GetVisualType1()) {
		case I3D_VISUAL_DECAL:
		{
			I3D_decal* dcl = (I3D_decal*)vis;
			SetMaterial(dcl->GetMaterial());
		}
		break;
		default:
			SetMeshInternal(const_cast<PI3D_visual>(vis)->GetMesh());
		}
		return I3D_visual::Duplicate(vis);
	}

	I3DMETHOD(DebugDraw)(PI3D_scene scene) const {
        byte alpha(byte(0xff));

        const S_matrix& m = GetMatrix();
        //draw cone
        {
            scene->SetRenderMatrix(m);

            S_vector v1;
            float f = 0.15f * .5f;
            v1.x = (float)sin(f);
            v1.z = (float)cos(f);
            v1.y = v1.x * .75f;
            v1 *= 2.0f / v1.Magnitude();
            S_vector v[5];
            v[0].Zero();
            v[1] = v1;
            v[2] = v1, v[2].y = -v[2].y;
            v[3] = v1, v[3].x = -v[3].x;
            v[4] = v[3], v[4].y = -v[4].y;
            static const word indx[] = { 0, 1, 0, 2, 0, 3, 0, 4, 1, 2, 2, 4, 4, 3, 3, 1 };

            scene->DrawLines(v, 5, indx, sizeof(indx) / sizeof(word), (alpha << 24) | 0xcccccc);
        }

		return I3D_OK;
	}
	
public:
	I3DMETHOD_(dword, Release)() { if (--ref) return ref; delete this; return 0; }
	
	I3DMETHOD_(I3D_RESULT, SetMaterial)(PI3D_material mat1) {
		mat = mat1;
		return I3D_OK;
	}
	I3DMETHOD_(PI3D_material, GetMaterial()) { return mat; }
	I3DMETHOD_(CPI3D_material, GetMaterial()) const { return mat; }
};

I3D_visual* CreateDecal(PI3D_driver drv) {
	return new I3D_decal(drv);
}

//----------------------------


