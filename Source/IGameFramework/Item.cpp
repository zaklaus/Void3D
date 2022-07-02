#include "all.h"
#include "Item.h"

#ifdef _DEBUG

//#define DEBUG_NO_IDLE         //never detect idle state

#endif

bool C_actor_item::CanGrab() const{
   return (owner == 0);
}

void C_actor_item::GameBegin(){
   const auto grip = GetFrame()->FindChildFrame("grip");
   if (grip){
      pos = grip->GetPos();
      rot = grip->GetRot();
   }

   C_actor_physics::GameBegin();
}

void C_actor_item::Fire(){
   #ifdef EDITOR
   DEBUG("Mock fire!");
   #endif

   PI3D_animation_set as{};
   const auto mod = GetAnimModel();
   
   if (!mod->IsPlaying() && GetAnimation("fire", &as)) {
       mod->SetAnimation(0, as, I3DANIMOP_BLEND, 1.0f, 1.0f, 1.0f);
       as->Release();
   }
}

void C_actor_item::Tick(const S_tick_context &tc){
   C_actor_physics::Tick(tc);

   struct S_hlp{
      static I3DENUMRET I3DAPI cbEnum(PI3D_frame frm, dword c){
         I3DCAST_VISUAL(frm)->EnableOverlay((bool)c);
         return I3DENUMRET_OK;
      }
   };
   frame->EnumFrames(S_hlp::cbEnum, (owner != nullptr), ENUMF_VISUAL);
}

bool C_actor_item::GetAnimation(C_str name, PI3D_animation_set* as)
{
	PI3D_model mod = GetAnimModel();
	const char* fileName = (const char*)mod->GetFileName();
	const char* p = fileName;
	while (!*p) {
		if (*p == '\\') {
			fileName = p;
		}
		++p;
	}
    const auto animFileName = C_fstr("\\%s_%s", fileName, (const char*)name);
	I3D_RESULT ir = anim_cache.Create((const char*)animFileName, as, mission.GetScene());
	if (I3D_FAIL(ir)) {
		return false;
	}
    return true;
}

PI3D_model C_actor_item::GetAnimModel()
{
	PI3D_frame childMesh = frame->FindChildFrame(0, ENUMF_MODEL);
	if (!childMesh) {
		childMesh = frame;
	}

	return I3DCAST_MODEL(childMesh);
}


void C_actor_item::GameEnd(){
   owner = nullptr;
}

bool C_actor_item::Use(E_USE_TYPE use_type, C_actor* instigator, PI3D_frame hit_frm){
   if (owner) return false;

   C_player *plr = CastPlayerActor(mission, instigator);
   if (plr){
      if (use_type == USE_PEEK){
         return true;
      }

      if (IsSingleUse()){
         Fire();
         return true;
      }

      plr->EquipItem(this);
      return true;
   }

   return false;
}

PC_actor_item CastItemActor(PC_actor act){
   assert(act);
   if (act->HasPhysics()){
      return reinterpret_cast<PC_actor_item>(act);
   } else{
      return NULL;
   }
}
//----------------------------
//----------------------------

PC_actor CreateActorItem(C_game_mission &gm, PI3D_frame in_frm){
   return new C_actor_item(gm, in_frm, ACTOR_ITEM);
}

//----------------------------
//----------------------------
