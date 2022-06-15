#pragma once

#include "GameMission.h"

class C_actor_item;

class C_player: public C_actor{
public:
   C_player(C_game_mission& gm, PI3D_frame in_frm) :
        C_actor(gm, in_frm, ACTOR_PLAYER)
   {}

   virtual bool EquipItem(C_actor_item *item = NULL) = 0;
};

C_player *CastPlayerActor(C_game_mission &gm, PC_actor act);

