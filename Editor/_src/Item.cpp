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
