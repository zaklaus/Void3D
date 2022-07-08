
#include "all.h"
#include <i3d\i3d_format.h>
#include "loader.h"

#include "dummy.h"
#include "visual.h"
#include "joint.h"
#include "volume.h"
#include "mesh.h"
#include "procedural.h"
#include "anim.h"
#include <integer.h>

#define ANIM_FPS 100
#define AUTO_SMOOTH_ANGLE (50.0f*PI/180.0f)  //angle of autosmoothing - used when smoothgroups are not present
#define FORCE_MIPMAPPING      //force using mipmapping, unless it is explicitly disabled
#define REPORT_EMPTY_MESH_ERROR  //report meshes with no vertices or faces
//#define REPORT_EMPTY_SMOOTHGROUPS //report warning on meshes with faces with no smoothgroup set
//#define REPORT_INVALID_FACES  //report meshes with invalid (on line) faces
//#define FORCE_DETAILMAP_COMPRESSION
//#define REPORT_UNUSED_VERTS  //report meshes with unused vertices
//#define REPORT_REDUNDANT_VERTS
//#define DETECT_DUPLICATED_MATERIALS //detect duplicated materials in one loaded file
#define USE_DATABASE          //try to cache computation-expensive info in database
#define MAX_VERTICES 0x7fff   //max vertices we may duplicate due to UV mapping and normal generation


                                 //animation set loader

I3D_RESULT I3D_animation_set::Open(const char* fname, dword flags, PI3D_LOAD_CB_PROC cb_proc, void* cb_context) {

   anims.clear();

   C_chunk ck;
   C_smart_ptr<C_loader> loader(CreateLoaderI3D(drv, ck));
   loader->Release();
   I3D_RESULT ir = I3DERR_FILECORRUPTED;
   try {
      if (!ck.ROpen(fname))
         return I3DERR_NOFILE;
      if (!cb_proc)
         flags &= ~I3DLOAD_LOG;
      switch (++ck) {
      case CT_BASE:
         ir = loader->Open(fname, flags, cb_proc, cb_context, NULL, NULL, this, NULL);
         break;
      }
   }
   catch (const C_except&) {
   }
   /*
   ck.Close();

   if(I3D_SUCCESS(ir)){
                              //sync all times
      dword max_time = 0;
      dword i;
      for(i=0; i<anims.size(); i++)
         max_time = Max(max_time, ((PI3D_keyframe_anim)(PI3D_animation_base)anims[i].anim)->GetEndTime());
      for(i=0; i<anims.size(); i++)
         ((PI3D_keyframe_anim)(PI3D_animation_base)anims[i].anim)->SetEndTime(max_time);
   }
   */
   return ir;
}

//----------------------------
//----------------------------

I3D_RESULT I3D_container::OpenFromChunk(C_chunk& ck, dword load_flags, PI3D_LOAD_CB_PROC load_cb_proc,
   void* load_cb_context, PI3D_scene scene, PI3D_frame root, PI3D_animation_set anim_set, PI3D_driver drv) {

   Close();

   if (load_flags & I3DLOAD_PROGRESS)
      load_cb_proc(CBM_PROGRESS, I3DFloatAsInt(0.0f), 0, load_cb_context);

   I3D_RESULT ir = I3DERR_FILECORRUPTED;

   try {
      if (!load_cb_proc) {
         load_flags &= ~(I3DLOAD_LOG | I3DLOAD_PROGRESS);
      }
      switch (++ck) {

      case CT_BASE:
      {
         C_smart_ptr<C_loader> loader(CreateLoaderI3D(drv, ck));
         loader->Release();
         ir = loader->Open(filename, load_flags, load_cb_proc, load_cb_context, root, scene, anim_set, this);
         if (I3D_SUCCESS(ir))
            SetUserData(loader->user_comments);
      }
      break;
      }
      --ck;
   }
   catch (const C_except&) {
   }

   if (load_flags & I3DLOAD_PROGRESS)
      load_cb_proc(CBM_PROGRESS, I3DFloatAsInt(1.0f), 2, load_cb_context);

   if (I3D_FAIL(ir))
      Close();

   return ir;
}

//----------------------------

I3D_RESULT I3D_container::Open(const char* filename1, dword load_flags, PI3D_LOAD_CB_PROC load_cb_proc,
   void* load_cb_context, PI3D_scene scene, PI3D_frame root, PI3D_animation_set anim_set, PI3D_driver drv) {

   filename = filename1;
   C_chunk ck;

   //open file
   if (!ck.ROpen(filename)) {
      return I3DERR_NOFILE;
   }

   return OpenFromChunk(ck, load_flags, load_cb_proc, load_cb_context, scene, root, anim_set, drv);
}
//----------------------------
                              //scene loader
I3D_RESULT I3D_scene::Open(const char* fname, dword flags, PI3D_LOAD_CB_PROC cb_proc, void* cb_context) {

   return cont.Open(fname, flags, cb_proc, cb_context, this, GetPrimarySector1(), NULL, drv);
}

//----------------------------

