#pragma once

#include "GameMission.h"
#include "PhysicsActor.h"

class C_ragdoll: public C_actor{
public:
   C_ragdoll(C_game_mission& gm, PI3D_frame frm):
   C_actor(gm, frm, ACTOR_RAGDOLL)
   {}

   virtual C_smart_ptr<IPH_body> GetBodyPart(C_str name) = 0; 
};
