#pragma once

#include "GameMission.h"

#include "Player.h"
#include "PhysicsActor.h"

//----------------------------

class C_actor_item: public C_actor_physics{
public:
   C_smart_ptr<C_player> owner;
   virtual void Fire();
   virtual bool IsSingleUse() const { return false; }
public:
   S_vector pos;
   S_quat rot;

   C_actor_item(C_game_mission &gm, PI3D_frame in_frm, E_ACTOR_TYPE at):
      C_actor_physics(gm, in_frm ? in_frm : gm.GetScene()->CreateFrame(FRAME_MODEL), at)
   {
      pos = {0,0,0};
      rot = {1, 0,0,0};
   }
   //--------------------------
   virtual bool CanGrab() const override;

   void GameBegin() override;
   void GameEnd() override;

   bool Use(E_USE_TYPE use_type, C_actor* instigator, PI3D_frame hit_frm) override;

protected:
      void Tick(const S_tick_context &tc) override;

      bool GetAnimation(C_str name, PI3D_animation_set*);
      PI3D_model GetAnimModel();
};

typedef C_actor_item *PC_actor_item;

PC_actor_item CastItemActor(PC_actor act);
