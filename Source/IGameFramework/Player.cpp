#include "all.h"

#include "GameFramework.h"
#include "actors.h"
#include "Player.h"
#include "GameMission.h"
#include "Game_cam.h"
#include "Item.h"
#include "PhysicsActor.h"


//----------------------------
//----------------------------

#define MOVE_SPEED_FORWARD 4.0f
#define MOVE_SPEED_BACK 1.5f
#define MOVE_SPEED_SIDE 2.5f

#define MOVE_RUN_MULTIPLIER 3.0f //when running, speed is multiplied by this
#define MOVE_BENCH_MULTIPLIER .5f   //when in bench, speed is multiplied by this

#define MAX_SPEED_CHANGE 40.0f //speed change per second

#define INIT_FALL_SPEED .01f  //initial
#define FALL_SPEED 20.0f      //fall down acceleration
#define JUMP_SPEED 10.0f      //speed of jump up

#define LOOK_DIR_SPEED .005f  //mapping of mouse units to rotation angle
#define MAX_LOOK_ANGLE PI*.45f//maximal vertical angle how we can look

#define BENCH_ANIM_TIME .3f   //time how long bench is animated, in seconds

#define SMALL_STEP_HEIGHT .8f //height of climbable small step
#define SMALL_STEP_TEST_DEPTH (vol_radius[0]*2.0f) //depth if collision testing for small testing (from body center line)
#define SMALL_STEP_CLIMB_Y 1.0f//speed of climbing small steps (per second)

#define DEFAULT_MAX_USE_DIST 1.5f   //default maximal use distance
//#define DEFAULT_MAX_USE_ANGLE (PI*.4f) //default maximal use angle
#define DEFAULT_MAX_GRAB_DIST DEFAULT_MAX_USE_DIST // 3.0f   //default maximal grab distance
#define DEFAULT_MAX_GRAB_POWER 400.0f   //default grab power
#define DEFAULT_MAX_GRAB_LOOSE_DIST 2.5f   //default grab max distance

//----------------------------

static const S_vector AXIS_Y(0.0f, 1.0f, 0.0f);

//----------------------------

class C_player_imp : public C_player {
    S_vector curr_move_dir;
    S_vector want_move_dir;
    float look_angle;          //0=straight, positive = up, negative = down
    float fall_speed;
    PI3D_frame floor_link;     //frame we're currently linked to (on which we stay)
    S_matrix m_floor;          //matrix of 'floor_link' - used to move with this frame (valid when floor_link!=NULL)
    PI3D_frame vols[3];
    float vol_y[3];
    float vol_radius[3];
    float bench_ratio;         //0.0 = staying, 1.0 = full bench
    bool jump_released;
    float shake_anim_pos;

    // player use highlight temp
    C_str last_hit_frm_name;
    float last_hit_frm_lit;

    // grab mode vars
    C_smart_ptr<C_actor_physics> grab_act;
    C_smart_ptr<C_actor_item> equip_act;

    void SetFocus(bool b) {
        PI3D_camera cam = mission.GetGameCamera()->GetCamera();
        if (b) {
            cam->LinkTo(frame);
            cam->SetDir(S_vector(0, 0, 1));
            cam->SetPos(S_vector(0, 1.7f, 0));
            cam->LinkTo(vols[2], I3DLINK_UPDMATRIX);
            mission.SetInputActor(this);
        }
        else {
            mission.GetGameCamera()->ReattachOwner();
        }
    }

    virtual bool  Use(E_USE_TYPE use_type, C_actor* instigator, PI3D_frame hit_frm) {
        if (!instigator) {
            return 0;
        }

        if (use_type == USE_PEEK) {
            return 1;
        }

        if (instigator->GetActorType() == ACTOR_VEHICLE) {
            SetFocus(true);
        }

        return 1;
    }

    //----------------------------                               
    // Setup floor frame (the one, on which we stand and moving with).
    void SetupFloorLink(PI3D_frame new_link) {

        if (floor_link != new_link) {
            if (!new_link) {
                floor_link = NULL;
            }
            else {
                floor_link = new_link;
                m_floor = floor_link->GetMatrix();
            }
        }
    }

    //----------------------------
    // Perform re-link to floor - update position and rotation since last frame.
    bool DoFloorLink() {

        if (floor_link) {
            if (!floor_link->IsOn()) {
                floor_link = NULL;
            }
            else                   //check if matrix of linked frame changed
                if (memcmp(&m_floor, &floor_link->GetMatrix(), sizeof(S_matrix))) {
                    //compute displacement
                    const S_matrix& m_floor_new = floor_link->GetMatrix();

                    const S_matrix& m_curr = frame->GetMatrix();
                    //put to local coords
                    S_matrix m_loc = m_curr * ~m_floor;
                    //put back to world coords
                    S_matrix m_new = m_loc * m_floor_new;
                    //save new floor matrix
                    m_floor = m_floor_new;
                    //re-position frame
                    frame->SetPos(m_new(3));
                    frame->SetDir1(m_new(2), S_vector(0,1,0));
                    return true;
                }
        }
        return false;
    }

    //----------------------------
    // Collision response function for above-head test.
    bool HeadColResp(I3D_cresp_data& rd) {

        const S_vector& limit_normal = rd.GetHitNormal();
        float angle_cos = limit_normal.y;
        if (angle_cos > -.5f)
            return false;
        return true;
    }

    static bool I3DAPI HeadColResp_thunk(I3D_cresp_data& rd) {
        return ((C_player_imp*)rd.cresp_context)->HeadColResp(rd);
    }

    //----------------------------

    PI3D_frame DetectVolumeAboveHead() {
        //detect "something" above the head
        const S_matrix& m0 = vols[0]->GetMatrix();
        const S_matrix& m = frame->GetMatrix();
        S_vector from = m0(3);
        S_vector dir = (m(3) + m(1) * (vol_y[2] + .06f)) - from;

        //DebugLine(from, from+dir, 1, 0x800000ff);
        I3D_collision_data cd;
        cd.from = from;
        cd.dir = dir;
        cd.flags = I3DCOL_MOVING_SPHERE;
        cd.radius = vol_radius[2] - .05f;
        cd.frm_ignore = frame;
        cd.callback = HeadColResp_thunk;
        cd.cresp_context = this;

        if (!mission.GetScene()->TestCollision(cd))
            return NULL;
        //DebugLine(from, from+S_normal(dir) * cd.GetHitDistance(), 1, 0xffff0000);
        //DEBUG(cd.GetHitFrm()->GetName());
        float ratio = cd.GetHitDistance() / dir.Magnitude();
        //DEBUG(ratio);
        ratio = 1.0f - ratio;
        //add some value to prevent hitting head during possible movement
        ratio = Min(1.0f, ratio * 1.2f);
        bench_ratio = Max(bench_ratio, ratio);
        //forget jumping if our head hit something
        fall_speed = Max(INIT_FALL_SPEED, fall_speed);
        //jump_released = true;
        //DEBUG(animate_bench_time);
        return cd.GetHitFrm();
    }

    //----------------------------
                                  //context structure passed to collision testing, used in response callback
    class C_collision_context {
    public:
        C_player_imp* _this;
        C_smart_ptr<I3D_frame> col_floor;   //current floor frame we're colliding with
        //int floor_mat_id;       //set to mat_id of floor, if detected
        bool is_idle;
        bool wall_hit;
        S_vector wall_normal;   //valid only when wall_hit==true

        C_collision_context(C_player_imp* t) :
            _this(t),
            //floor_mat_id(-1),
            wall_hit(false),
            is_idle(false)
        {}
    };

    //----------------------------

    bool ColResp(I3D_cresp_data& rd, C_collision_context& cc) {

        //const int mat_id = GetMaterialID(rd.GetHitFrm());
        //assert((mat_id >= 0) && (mat_id < MAX_MATERIALS));
        //E_MAT_STYLE mat_style = (E_MAT_STYLE)mat_table->ItemE(TAB_E_MAT_STYLE, mat_id);

        const S_vector& limit_normal = rd.GetHitNormal();
        float angle_cos = limit_normal.y;
#define FLOOR_ANGLE .95f
#define STAIRS_ANGLE .75f

        DEBUG(angle_cos);
        if (angle_cos >= STAIRS_ANGLE) {
            cc.col_floor = rd.GetHitFrm();
            if (cc.is_idle)
                rd.ModifyNormal(AXIS_Y);
        }
        else if (!cc.wall_hit && rd.vol_source == vols[0] && angle_cos <= STAIRS_ANGLE) {
            cc.wall_normal = limit_normal;
            cc.wall_hit = true;
        }
        return true;
    }

    //----------------------------
    // Thunk into collision response function.
    static bool I3DAPI ColResp_thunk(I3D_cresp_data& rd) {
        C_collision_context* cc = (C_collision_context*)rd.cresp_context;
        return cc->_this->ColResp(rd, *cc);
    }

    //----------------------------

    void UpdateMovement(float tsec) {

        DoFloorLink();
        const S_matrix& m = frame->GetMatrix();
        const S_vector& from = m(3);
        //update by wanted move direction
        S_vector move_delta = want_move_dir - curr_move_dir;
        float delta = move_delta.Magnitude();
        if (!IsAbsMrgZero(delta)) {
            if (delta > (MAX_SPEED_CHANGE * tsec)) {
                move_delta *= (MAX_SPEED_CHANGE * tsec) / delta;
            }
        }
        curr_move_dir += move_delta;
        move_delta = (curr_move_dir * tsec) % m;

        //update falling
        move_delta.y -= fall_speed * tsec;
        fall_speed += tsec * FALL_SPEED;

        //detect collisions
        DebugLine(from, from + move_delta, 1);
        PI3D_scene scene = mission.GetScene();

        C_collision_context cc(this);
        cc.is_idle = IsAbsMrgZero(move_delta.x * move_delta.z);

        I3D_collision_data cd;
        cd.from = from;
        cd.dir = move_delta;
        cd.flags = I3DCOL_MOVING_GROUP | I3DCOL_SOLVE_SLIDE;
        cd.frm_ignore = frame;
        cd.frm_root = frame;
        cd.callback = ColResp_thunk;
        cd.cresp_context = &cc;
        if (scene->TestCollision(cd)) {
            move_delta = cd.GetDestination() - from;

            if (cc.col_floor && !cc.is_idle) {
                fall_speed = INIT_FALL_SPEED;
            }
        }
        else {
#if 0
            I3D_collision_data cd;
            cd.from = from + move_delta;
            cd.dir = AXIS_Y * -1.0f;
            cd.flags = I3DCOL_MOVING_GROUP | I3DCOL_SOLVE_SLIDE;
            cd.frm_ignore = frame;
            cd.frm_root = frame;
            cd.callback = ColResp_thunk;
            cd.cresp_context = &cc;

            if (scene->TestCollision(cd)) {
                move_delta = cd.GetDestination() - from - move_delta;
            }
#endif
        }
        SetupFloorLink(cc.col_floor);

        DEBUG(floor_link ? floor_link->GetName() : "-");
        float curr_speed = curr_move_dir.Magnitude();
        S_vector pos = from + move_delta;

        DEBUG(curr_speed);

        if (cc.wall_hit && !IsMrgZeroLess(curr_speed)) {
            //detect small steps
            I3D_collision_data cd;
            cd.from = pos;
            cd.dir = -cc.wall_normal;
            cd.dir.y = 0.0f;
            cd.dir *= SMALL_STEP_TEST_DEPTH / cd.dir.Magnitude();
            cd.from.y += SMALL_STEP_HEIGHT;
            cd.flags = I3DCOL_LINE;
            //DebugLine(cd.from, cd.from + cd.dir, 1);

            if (!scene->TestCollision(cd)) {
                //allow climbing step - elevate and forget fall speed
                pos.y += tsec * SMALL_STEP_CLIMB_Y;
                fall_speed = INIT_FALL_SPEED;
            }
        }

        frame->SetPos(pos);
        mission.GetScene()->SetFrameSectorPos(frame, vols[1]->GetWorldPos());

        DetectVolumeAboveHead();
        ApplyBench();

        if (mission.IsInputActor(this))
        {
            S_vector pos;
            S_quat rot;
            float t = tsec * (.5f + curr_speed / 6.0f);
            GetAnimatedShaking(t, curr_speed / 15.0f, pos, rot);
            PI3D_camera cam = mission.GetGameCamera()->GetCamera();

            //add camera horizontal rotation
            S_quat rot_h(S_vector(1, 0, 0), look_angle);
            rot *= rot_h;

            cam->SetPos(pos);
            cam->SetRot(rot);
        }
    }

    //----------------------------
    // Animate camera shaking and return delta C_vector to be applied to camera.
    void GetAnimatedShaking(float tsec, float ratio, S_vector& pos, S_quat& rot) {

        shake_anim_pos += tsec * 3.0f;
        if (shake_anim_pos >= 4.0f)
            shake_anim_pos -= 4.0f;
        //DEBUG(shake_anim_pos);

                                //make position
        float f = shake_anim_pos;

        pos.y = (1.0f - ((float)cos(f * PI)));
        pos.y *= .1f * ratio;
        bool reverse = (shake_anim_pos >= 2.0f);
        if (reverse)
            f -= 2.0f;
        if (f >= 1.0f)
            f = 2.0f - f;
        pos.x = (float)sin(f * PI * .5f);
        pos.x *= .2f * ratio;
        if (reverse)
            pos.x = -pos.x;
        //DEBUG(pos.y);
        pos.z = 0.0f;

        //make rotation
        float angle = pos.x * PI * .05f;
        rot.Make(S_vector(0, 0, -1), angle);
    }

    //----------------------------
    // Reset value assiciated with movement.
    void ResetMoveValues() {

        floor_link = NULL;
        curr_move_dir.Zero();
        want_move_dir.Zero();
        look_angle = 0.0f;
        fall_speed = INIT_FALL_SPEED;
        bench_ratio = 0.0f;
        jump_released = true;
        shake_anim_pos = 0.0f;
    }

    //----------------------------
    // Apply current bench ratio to volumes' position.
    void ApplyBench() {
        //float t = ease_in_out[bench_ratio];
        float t = bench_ratio;
        for (int i = 2; i--; ) {
            float dir = vol_y[0] - vol_y[i + 1];
            vols[i + 1]->SetPos(S_vector(0, vol_y[i + 1] + dir * t, 0));
        }
    }

    //----------------------------
    // Animate bench - to be updated by player's input (bench key down).
    void AnimateBench(bool down, float tsec) {

        if (down) {
            if (bench_ratio == 1.0f)
                return;
            bench_ratio = Min(1.0f, bench_ratio + tsec / (float)BENCH_ANIM_TIME);
        }
        else {
            if (!bench_ratio)
                return;
            bench_ratio = Max(0.0f, bench_ratio - tsec / (float)BENCH_ANIM_TIME);
        }
    }

    //----------------------------
    // When jumping up, this function slows down jumping becuse player no more wants to raise.
    void SlowdownJump(float tsec) {

        jump_released = true;
        if (fall_speed < 0.0f)
            fall_speed = Min(0.0f, fall_speed + tsec * FALL_SPEED * 2.0f);
    }

    //----------------------------

    void SetupVolumes() {

        struct S_hlp {
            PI3D_frame owner;
            C_player_imp* act;
            bool in_hierarchy;

            static I3DENUMRET I3DAPI cbVol(PI3D_frame frm, dword c) {
                PI3D_volume vol = I3DCAST_VOLUME(frm);
                S_hlp* hp = (S_hlp*)c;

                //vol->SetHierarchical(hp->in_hierarchy);
                vol->SetOn(true);
                vol->SetOwner(hp->owner);

                return I3DENUMRET_OK;
            }
        } hlp;
        hlp.owner = frame;
        hlp.act = this;

        frame->EnumFrames(S_hlp::cbVol, (dword)&hlp, ENUMF_VOLUME);
    }
    //----------------------------
public:
    C_player_imp(C_game_mission& gm, PI3D_frame in_frm) :
        C_player(gm, gm.GetScene()->CreateFrame(FRAME_MODEL))
    {
        frame->Release();
        {
            PI3D_model mod = I3DCAST_MODEL(frame);
            model_cache.Open(mod, "test\\player", mission.GetScene());
            frame->SetName("player");
            frame->SetPos(in_frm->GetWorldPos());
            frame->SetRot(in_frm->GetWorldRot());
            frame->LinkTo(mission.GetScene()->GetPrimarySector());
        }
        {
            for (int i = 3; i--; ) {
                PI3D_frame frm = frame->FindChildFrame(C_fstr("vol%i", i), ENUMF_VOLUME);
                vols[i] = frm;
                if (frm) {
                    const S_matrix& m = frm->GetLocalMatrix();
                    vol_y[i] = m(3).y;
                    vol_radius[i] = m(0).Magnitude();
                }
                else {
                    vol_y[i] = 0.0f;
                    vol_radius[i] = 0.0f;
                }
            }
        }
        SetupVolumes();
        ResetMoveValues();

        grab_act = NULL;
    }


    //----------------------------

    void GameBegin() {

        SetFocus(true);

        //add shadow circle to humans which has no rendered shadow
        if (!(frame->GetFlags() & I3D_FRMF_SHADOW_CAST)) {
            PI3D_model mshd = I3DCAST_MODEL(mission.GetScene()->CreateFrame(FRAME_MODEL));
            model_cache.Open(mshd, "shadow", mission.GetScene(), 0);
            I3DCAST_MODEL(frame)->AddFrame(mshd);
            mshd->LinkTo(frame);
            mshd->SetName("auto-shadow");
            mshd->SetPos(S_vector(0, .1f, 0));
            mshd->SetScale(.75f);
            mshd->Release();
        }
    }

    //----------------------------

    void GameEnd() {

        mission.DestroyActor(this);
    }

    //----------------------------

    //@todo: Move to common.cpp
    void SetModelBrightness(PI3D_model mod, float brightness, I3D_VISUAL_BRIGHTNESS reg){
        const PI3D_frame *frms = mod->GetFrames();
        for(dword i=mod->NumFrames(); i--; ){
            PI3D_frame frm = *frms++;
            if(frm->GetType()==FRAME_VISUAL)
                I3DCAST_VISUAL(frm)->SetBrightness(reg, brightness);
        }
    }

    void RestoreHitBrightness(){
        if (last_hit_frm_name.Size()){
            PI3D_frame vis_or_mod = mission.GetScene()->FindFrame(last_hit_frm_name, ENUMF_VISUAL|ENUMF_MODEL);
            if (vis_or_mod){
                if (vis_or_mod->GetType() == FRAME_VISUAL){
                    I3DCAST_VISUAL(vis_or_mod)->SetBrightness(I3D_VIS_BRIGHTNESS_EMISSIVE, last_hit_frm_lit);
                    last_hit_frm_lit = 0.0f;
                }else{
                    //@todo how to restore brightness in models?
                    SetModelBrightness(I3DCAST_MODEL(vis_or_mod), 0.0f, I3D_VIS_BRIGHTNESS_EMISSIVE);
                }
                last_hit_frm_name = "";
            }
        }
    }

    bool CheckPlayerActorInteractive(const struct S_tick_context& tc, I3D_collision_data& cd){
        C_game_camera* cam = mission.GetGameCamera();
        if (!cam) return false;

        const S_matrix& m0 = cam->GetCamera()->GetMatrix();
        S_vector from = m0(3);
        S_vector dir = m0(2);

        cd.from = from;
        cd.dir = dir * DEFAULT_MAX_USE_DIST;
        cd.flags = I3DCOL_MOVING_SPHERE;
        cd.radius = 0.5f;
        cd.frm_ignore = frame;

        return mission.GetScene()->TestCollision(cd);
    }

    bool EquipItem(PC_actor_item item = NULL) override{
        C_game_camera* cam = mission.GetGameCamera();
        if (!cam) return false;

        if (item && !equip_act){
            equip_act = item;
            equip_act->owner = this;
            auto frm = equip_act->GetFrame();

            frm->LinkTo(cam->GetCamera());
            frm->SetPos(equip_act->pos);
            frm->SetRot(equip_act->rot);
            equip_act->EnableVolumes(false);
            equip_act->GetBody()->Enable(false);
            return true;
        }else if (!item && equip_act){
            I3D_collision_data cd;
            const S_matrix& m0 = cam->GetCamera()->GetMatrix();
            S_vector from = m0(3);
            S_vector dir = m0(2);

            cd.from = from;
            cd.dir = dir * DEFAULT_MAX_USE_DIST;
            cd.flags = I3DCOL_MOVING_GROUP;
            cd.frm_root = equip_act->GetFrame();
            cd.frm_ignore = frame;

            if (mission.GetScene()->TestCollision(cd)){
                from = cd.ComputeHitPos();
            }else{
                from += cd.dir;
            }
            equip_act->GetFrame()->LinkTo(nullptr);
            equip_act->GetFrame()->SetPos(from);
            mission.GetScene()->SetFrameSectorPos(equip_act->GetFrame(), from);
            equip_act->EnableVolumes(true);
            equip_act->GetBody()->SetPosRot(from, equip_act->GetFrame()->GetWorldRot());
            equip_act->GetBody()->SetLinearVelocity(S_vector(0,0,0));
            equip_act->GetBody()->Enable(true);
            equip_act->owner = nullptr;
            equip_act = nullptr;
            return true;
        }

        return false;
    }

    void HighlightUsableActor(PI3D_frame frm){
        RestoreHitBrightness();
        if (frm->GetType() == FRAME_VISUAL){
            PI3D_visual vis = I3DCAST_VISUAL(frm);
            last_hit_frm_name = frm->GetName();
            last_hit_frm_lit = vis->GetBrightness(I3D_VIS_BRIGHTNESS_EMISSIVE);
            vis->SetBrightness(I3D_VIS_BRIGHTNESS_EMISSIVE, 0.5f);
        }else if (frm->GetType() == FRAME_MODEL){
            PI3D_model mod = I3DCAST_MODEL(frm);
            last_hit_frm_name = frm->GetName();
            SetModelBrightness(mod, 0.5f, I3D_VIS_BRIGHTNESS_EMISSIVE);
        }
    }

    PI3D_frame GetRootFrameOfHitVolume(PI3D_frame hit_frm, PI3D_frame& old_frm) const{
        old_frm = hit_frm;
        while (GetFrameActor(hit_frm) == nullptr) {
            if (hit_frm->GetParent() == mission.GetScene()->GetPrimarySector())
                break;
            hit_frm = hit_frm->GetParent();
        }

        if (old_frm->GetParent() != mission.GetScene()->GetPrimarySector())
            old_frm = old_frm->GetParent();

        return hit_frm;
    }

    void CheckPlayerUse(const struct S_tick_context& tc, I3D_collision_data& cd) {
        PI3D_frame hit_frm = cd.GetHitFrm();
        PI3D_frame old_frm;
        hit_frm = GetRootFrameOfHitVolume(hit_frm, old_frm);

        PC_actor act = GetFrameActor(hit_frm);
        if (act) {
            if (act->Use(USE_PEEK, this, old_frm)) {
                HighlightUsableActor(old_frm);
                
                DEBUG("Press F to use");
                if (tc.p_ctrl->Get(CS_USE, true)) {
                    if (act) {
                        RestoreHitBrightness();
                        act->Use(USE_TOGGLE, this, old_frm);
                    }
                }
            }
        }
    }

    void CheckPlayerGrab(const S_tick_context& tc, I3D_collision_data& cd){
        PI3D_frame hit_frm = cd.GetHitFrm();
        PI3D_frame old_frm;
        hit_frm = GetRootFrameOfHitVolume(hit_frm, old_frm);

        PC_actor act = GetFrameActor(hit_frm);
        if (act) {
            PC_actor_physics phys_act = CastPhysicsActor(act);
            if (phys_act && phys_act->CanGrab()){
                HighlightUsableActor(old_frm);
                
                DEBUG("Press G to grab");
                if (tc.p_ctrl->Get(CS_GRAB, true)) {
                    grab_act = phys_act;
                    grab_act->SetGrabbed(true);
                }
            }
        }
    }

    void ManipulateGrabbedActor(const S_tick_context& tc){
        if (!grab_act) return;
        C_game_camera* cam = mission.GetGameCamera();
        if (!cam) return;

        const S_matrix& m0 = cam->GetCamera()->GetMatrix();
        S_vector from = m0(3);
        S_vector dir = m0(2);
        PI3D_frame frm = grab_act->GetFrame();

        I3D_collision_data cd;
        cd.from = from;
        cd.dir = dir * DEFAULT_MAX_GRAB_DIST;
        cd.flags = I3DCOL_MOVING_GROUP;
        cd.frm_root = frm;
        cd.frm_ignore = frame;

        S_vector dest = cd.from + cd.dir;

        frm->SetOn(false);
        if (mission.GetScene()->TestCollision(cd)){
            dest = cd.GetDestination();
        }
        frm->SetOn(true);

        S_vector obj_dir = dest - grab_act->GetCenterPos();
        IPH_body *body = grab_act->GetBody(0);
        assert(body);
        body->SetLinearVelocity(obj_dir*(DEFAULT_MAX_GRAB_POWER/body->GetWeight()));
        body->SetAngularVelocity(S_vector(0,0,0));

        RestoreHitBrightness();
        HighlightUsableActor(frm);
        DEBUG("Grab mode activated!");

        if (obj_dir.Magnitude() > DEFAULT_MAX_GRAB_LOOSE_DIST){
            grab_act->SetGrabbed(false);
            grab_act = NULL;
        }
    }

    //----------------------------
    void Input(const S_tick_context& tc) {
        C_game_camera* cam = mission.GetGameCamera();

        if (cam->GetCamera()->GetParent() != vols[2]) {
            return;
        }

        LPC_controller ctrl = tc.p_ctrl;

        float rx = (float)tc.mouse_rel[0], ry = (float)tc.mouse_rel[1];
        //mouse - apply sensitivity and speed
        if (ctrl) {
            if (ctrl->GetConfigValue(C_controller::CFG_INVERT_MOUSE_Y))
                ry = -ry;

            float f = (float)ctrl->GetConfigValue(C_controller::CFG_MOUSE_SENSITIVITY) * .01f;
            rx *= f;
            ry *= f;
        }

        want_move_dir.Zero();
        float tsec = (float)tc.time * .001f;
        if (ctrl) {
            bool is_running = ctrl->Get(CS_RUN);
            if (ctrl && ctrl->GetConfigValue(C_controller::CFG_ALWAYS_RUN))
                is_running = !is_running;

            RestoreHitBrightness();

            if (grab_act){
                if (tc.p_ctrl->Get(CS_GRAB, true)) {
                    grab_act->SetGrabbed(false);
                    grab_act = NULL;
                }
            }

            if (!grab_act){
                I3D_collision_data cd;
                if (!equip_act && CheckPlayerActorInteractive(tc, cd)){
                    // DEBUG(cd.GetHitFrm()->GetName());
                    CheckPlayerUse(tc, cd);
                    CheckPlayerGrab(tc, cd);
                }else{
                    if (tc.p_ctrl->Get(CS_USE, true) && equip_act){
                        EquipItem();
                    }
                }
            }

            if (equip_act){
                DEBUG("Press LMB to use item");
                if (ctrl->Get(CS_FIRE)) {
                    equip_act->Fire();
                }
            }

            ManipulateGrabbedActor(tc);

            if (ctrl->Get(CS_JUMP)) {
                if (!bench_ratio && floor_link && jump_released) {
                    jump_released = false;
                    fall_speed = -JUMP_SPEED;
                    if (is_running)
                        fall_speed *= 1.25f;
                }
            }
            else {
                SlowdownJump(tsec);
            }

            //update movement
            if (ctrl->Get(CS_MOVE_FORWARD))
                want_move_dir.z += MOVE_SPEED_FORWARD;

            if (ctrl->Get(CS_MOVE_BACK))
                want_move_dir.z -= MOVE_SPEED_BACK;

            if (ctrl->Get(CS_MOVE_LEFT))
                want_move_dir.x -= MOVE_SPEED_SIDE;

            if (ctrl->Get(CS_MOVE_RIGHT))
                want_move_dir.x += MOVE_SPEED_SIDE;

            if (ctrl->Get(CS_TURN_LEFT))
                rx = -tsec * PI * 300.0f;
            if (ctrl->Get(CS_TURN_RIGHT))
                rx = tsec * PI * 300.0f;

            if (is_running)
                want_move_dir *= MOVE_RUN_MULTIPLIER;

            AnimateBench(ctrl->Get(CS_STAY_DOWN), tsec);
            if (bench_ratio)
                want_move_dir -= want_move_dir * ((1.0f - MOVE_BENCH_MULTIPLIER) * bench_ratio);

            //update rotation
            if (rx) {
                float angle = rx * LOOK_DIR_SPEED;
                S_quat rot(S_vector(0, -1, 0), angle);
                frame->SetRot(frame->GetRot() * rot);
            }
            if (ry) {
                look_angle += -ry * LOOK_DIR_SPEED;
                look_angle = Max(-MAX_LOOK_ANGLE, Min(MAX_LOOK_ANGLE, look_angle));
            }
        }
        else {
            AnimateBench(false, tsec);
            SlowdownJump(tsec);
        }
    }
    //----------------------------

    virtual void Tick(const struct S_tick_context& tc) {
        float tsec = (float)tc.time * .001f;
        UpdateMovement(tsec);

        if (equip_act){
            bool on = mission.IsInputActor(this);
            equip_act->GetFrame()->SetOn(on);
            equip_act->Enable(on);
        }
    }
};

//----------------------------
//----------------------------

PC_actor CreatePlayerActor(C_game_mission& gm, PI3D_frame frm, const void*) {
    return new C_player_imp(gm, frm);
}

//----------------------------

C_player *CastPlayerActor(C_game_mission &gm, PC_actor act){
    if (gm.IsInputActor(act)){
        return reinterpret_cast<C_player*>(act);
    }
    return nullptr;
}