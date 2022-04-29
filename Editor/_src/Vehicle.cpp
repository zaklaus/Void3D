#include "all.h"                              
#include "gamemission.h"
#include "PhysicsActor.h"

//----------------------------

#define TURN_SPEED 800

#ifdef EDITOR

#define DEBUG_SHOW_INFO       //show info about vehicle (speed, etc)

#define DEBUG_NO_IDLE

#endif

#define DEBUG_DIRECT_KEYS

//----------------------------
//----------------------------

#include "game_cam.h"

                           //speed per AUTO_TRANSLATE_UP t/sec
static const float wanted_speed[] = {
   -3.5,
   0,
   2, 8, 16, 32, 50, 72
};

enum C_vehicle_props_data_index {
    CVEH_F_STEER_FORCE,
    CVEH_F_GEAR_S_NAME,
    CVEH_F_GEAR_VEL = 2,
    CVEH_F_GEAR_FORCE,
    CVEH_F_GEAR_TRAN_SPEED,
    CVEH_F_CAMERA_DISTANCE,
};

static const C_table_element te_veh_props[] = {
       {TE_FLOAT, CVEH_F_STEER_FORCE, "Steer Force", 0, 10, 1, ""},
       {TE_ARRAY, CVEH_F_GEAR_S_NAME, "Gears", 7, 0, 0, "Vehicle gears"},
         {TE_BRANCH, 0, "Gear", 3, (dword)"%[0]"},
          {TE_FLOAT, CVEH_F_GEAR_VEL,"Velocity", -1000, 1000, 0, "Gear velocity"},
          {TE_FLOAT, CVEH_F_GEAR_FORCE, "Force", -1000, 1000, 0, "Gear force"},
          {TE_FLOAT, CVEH_F_GEAR_TRAN_SPEED, "Tran. Speed", -2000, 2000, 0, "Transition speed to gear up at"},
       {TE_FLOAT, CVEH_F_CAMERA_DISTANCE, "Camera Dist.", 0, 100, 10, "Camera max distance"},
       {TE_NULL}
};

static const C_table_template templ_veh_props = { "Vehicle", te_veh_props };

class C_vehicle_imp : public C_actor_physics {

    struct S_wheel {
        C_smart_ptr<IPH_joint_hinge2> jnt;
        bool powered;
    };

    C_smart_ptr<I3D_sound> snd_engine;
    C_smart_ptr<I3D_visual> brake_light;
    //wheel joints, first are front, last are back
    C_vector<S_wheel> jnt_wheels;
    C_str ctrl;

    C_smart_ptr<I3D_frame> exits[3];
    C_smart_ptr<I3D_frame> doors;

    bool enabled;
    bool can_control;

    S_vector last_pos;
    int last_gear;
    float last_frequency;
    bool brake;
    bool engine_on;

    // transient data
    int gear = 0;
    float curr_speed = 0.0f;
    bool moving_backward = false;
    int want_dir = 0;       //0=stay, 1=forward, -1=backward
    float steer = 0.0f;

    float mx, my;

    //----------------------------

    void Enable(bool b) {

        if (enabled == b)
            return;
        for (int i = bodies.size(); i--; )
            bodies[i]->Enable(b);
        enabled = b;
    }

    //----------------------------

    virtual void OnPhysIdle() {

#ifndef DEBUG_NO_IDLE
        Enable(false);
#endif
        idle_count = 0;
    }

    //----------------------------

    virtual bool ContactReport(PI3D_frame dst_frm, const S_vector& pos, const S_vector& normal, float depth,
        bool play_sounds = true) {

        return C_actor_physics::ContactReport(dst_frm, pos, normal, depth, false);
    }

    //----------------------------

public:
    C_vehicle_imp(C_game_mission& gm1, PI3D_frame in_frm) :
        C_actor_physics(gm1, in_frm, ACTOR_VEHICLE),
        enabled(false),
        can_control(false),
        engine_on(false),
        doors(0),
        last_pos(0, 0, 0),
        last_gear(0),
        mx(0),
        my(0),
        ctrl("")
    {
        AssignTableTemplate();
    }

    virtual const C_table_template* GetTableTemplate() const { return &templ_veh_props; }

    //----------------------------

    virtual void GameBegin() {
        SetupVolumes(0x1);
        PI3D_model mod = GetModel();
        if (mod) {
            const C_str& mname = mod->GetFileName();
            S_phys_template templ;
            if (!mission.LoadPhysicsTemplate(C_xstr("Models\\%\\scene.bin") % &mname[1], templ))
                ReportActorError("Model is not listed in active objects");
            else {
                C_actor_physics::Init(&templ);
                enabled = true;
                Enable(false);
                //find all wheels
                for (dword i = joints.size(); i--; ) {
                    PIPH_joint jnt = joints[i];
                    if (jnt->GetType() == IPHJOINTTYPE_HINGE2) {
                        //apply steering on the front joint
                        PIPH_joint_hinge2 jh = (PIPH_joint_hinge2)jnt;

                        S_wheel wheel = { jh, false };

                        if (jh->GetName().Matchi("*_power*")) {
                            wheel.powered = true;
                        }

                        jnt_wheels.push_back(wheel);
                    }
                }

                const float force_steer = 50.0f;
                const float force_steer_mul = tab->GetItemF(CVEH_F_STEER_FORCE, 0);

                for (auto& wheel : jnt_wheels) {
                    if (wheel.powered)
                        wheel.jnt->SetMaxForce(force_steer*force_steer_mul);
                }
            }

            {
                PI3D_frame frm = 0;

                if (frm = mod->FindChildFrame("exit_l"))
                    exits[0] = frm;
                if (frm = mod->FindChildFrame("exit_r"))
                    exits[1] = frm;
                if (frm = mod->FindChildFrame("exit_t"))
                    exits[2] = frm;
            }
        }

        {
            if (!snd_engine) {
                snd_engine = I3DCAST_SOUND(mission.GetScene()->CreateFrame(FRAME_SOUND));
                snd_engine->Release();
            }
            snd_engine->SetSoundType(I3DSOUND_POINT);
            snd_engine->SetOn(false);

            const char* cp = "Test\\Motor";

            sound_cache.Open(snd_engine, cp, mission.GetScene(), I3DLOAD_SOUND_FREQUENCY, ErrReport, editor);
            snd_engine->SetRange(2.0f, 40.0f);
            snd_engine->SetLoop(true);
            PI3D_frame dum = frame->FindChildFrame("engine");
            if (!dum) dum = frame;
            snd_engine->LinkTo(dum);
        }

        PI3D_frame lit_dum = frame->FindChildFrame("light_brake", ENUMF_VISUAL);
        if (lit_dum)
            brake_light = I3DCAST_VISUAL(lit_dum);

        doors = frame->FindChildFrame("doors", ENUMF_DUMMY);

        last_pos = frame->GetWorldPos();
        last_gear = 0;
        last_frequency = 1.0f;
    }

    //----------------------------

    virtual void GameEnd(bool) {

        bodies.clear();
        joints.clear();
        enabled = false;
        snd_engine = NULL;
    }


    bool EjectOnSide(PI3D_frame frm, int side) {
        PI3D_frame exit_frm = exits[side == -1 ? 0 : 1];
        S_vector pos;
        S_vector dir;

        if (exit_frm == NULL) {
            pos = frame->GetWorldPos();
            dir = S_vector(0, 1, 0).Cross(frame->GetWorldDir()) * 2.0f * side;
        }
        else {
            pos = exit_frm->GetWorldPos();
            dir = S_vector(0, 1, 0);
        }

        I3D_collision_data cd;
        cd.from = pos;
        cd.dir = dir;
        cd.frm_ignore = frame;
        cd.flags = I3DCOL_MOVING_SPHERE;
        cd.radius = 1;

        frm->SetOn(false);
        bool collided = mission.TestCollision(cd);
        frm->SetOn(true);

        if (!collided) {
            frm->SetPos(cd.GetDestination());
        }

        return !collided;
    }


    virtual bool Use(E_USE_TYPE use_type /*= USE_TOGGLE*/, C_actor* instigator /*= NULL*/, PI3D_frame hit_frm /*= NULL*/) {
        C_game_camera* cam = mission.GetGameCamera();

        if (!cam) {
            return 0;
        }

        //@todo support more features
        enum {
            USE_NONE = 0x1,
            USE_DOORS = 0x2,
            USE_FUEL = 0x4,
            USE_ENGINE = 0x8,

            USE_ANY = 0xffffffff
        };

        //check for door entrance
        dword use = USE_NONE;
        if (hit_frm) {
            if (doors) {
                if (hit_frm->GetParent() == doors) {
                    use = USE_DOORS;
                }
            } else {
                use = USE_DOORS;
            }
        }

        if (use == USE_NONE)
            return 0;

        if (use_type == USE_PEEK){
            return 1;
        }

        if (use & USE_DOORS){
            if (!can_control) {
                cam->ReattachOwner();
                cam->GetCamera()->SetPos(S_vector(0, 0, 0));
                cam->SetFocus(frame->FindChildFrame("engine"), frame);
                cam->SetAngleMode(1);
                cam->SetDistance(tab->GetItemF(CVEH_F_CAMERA_DISTANCE));
                can_control = true;
                ctrl = instigator->GetName();
                instigator->GetFrame()->SetOn(false);
                mission.SetInputActor(this);
            }
        }

        return 1;
    }

    void ExitCar(){
        PI3D_frame frm = mission.GetScene()->FindFrame(ctrl);
        PC_actor act = GetFrameActor(frm);
        if (frm && act) {
            can_control = false;

            act->GetFrame()->SetOn(true);
            {
                if (!EjectOnSide(frm, -1)) {
                    if (!EjectOnSide(frm, 1)) {
                        S_vector pos = (exits[2] == NULL ? frame->GetWorldPos() + S_vector(0, 2, 0) : exits[2]->GetWorldPos());
                        frm->SetPos(pos);
                    }
                }
            }
            act->Use(USE_ON, this, NULL);
            brake = false;
            ctrl = "";
        }
    }

    //----------------------------

    void Input(const S_tick_context& tc){
        float tsec = (float)tc.time * .001f;
        
        if (tc.time) {
            S_vector move_dir = last_pos - frame->GetWorldPos();
            float dist = move_dir.Magnitude();
            float t = tsec / 3.6f;
            curr_speed = dist / t;

            moving_backward = (move_dir.Dot(frame->GetWorldDir()) > 0.0f);
        }

#ifdef DEBUG_DIRECT_KEYS
        if (can_control) {
            // orbit camera
            {
                float rx = (float)tc.mouse_rel[0], ry = (float)tc.mouse_rel[1];
                //mouse - apply sensitivity and speed
                if (tc.p_ctrl) {
                    if (tc.p_ctrl->GetConfigValue(C_controller::CFG_INVERT_MOUSE_Y))
                        ry = -ry;

                    float f = (float)tc.p_ctrl->GetConfigValue(C_controller::CFG_MOUSE_SENSITIVITY) * .01f;
                    rx *= f;
                    ry *= f;

                    mx += rx * 0.1f;
                    my += ry;

                    C_game_camera* cam = mission.GetGameCamera();

                    if (cam) {
                        S_quat rot = S_quat(S_vector(0, 1, 0), (mx) * (PI / 180.0f));
                        cam->SetDeltaRot(rot);
                    }
                }
            }

            brake = false;

            if (tc.p_ctrl) {
                if (tc.p_ctrl->Get(CS_MOVE_FORWARD)) {
                    want_dir = 1;
                }
                else
                    if (tc.p_ctrl->Get(CS_MOVE_BACK)) {
                        want_dir = -1;
                    }
                if (tc.p_ctrl->Get(CS_MOVE_LEFT)) {
                    steer = 1;
                }
                else
                    if (tc.p_ctrl->Get(CS_MOVE_RIGHT)) {
                        steer = -1;
                    }
                if (tc.p_ctrl->Get(CS_JUMP)) {
                    brake = true;
                }
            }

            if (tc.p_ctrl && tc.p_ctrl->Get(CS_USE, true)) {
                if (engine_on && curr_speed < 2.0f) {
                    engine_on = false;
                    brake = false;
                    if (snd_engine->IsOn()) {
                        snd_engine->SetOn(false);
                    }
                    Enable(false);
                }
                else if (ctrl.Size()) {
                    ExitCar();
                }
            }
        }
#endif   //DEBUG_DIRECT_KEYS
    }

    //----------------------------

    virtual void Tick(const S_tick_context& tc) {
        float tsec = (float)tc.time * .001f;
        const int max_gear = 5;

        //determine gear
        if (want_dir) {
            if (want_dir > 0) {
                if (moving_backward && curr_speed > 1.0f) {
                    want_dir = 0;
                    brake = true;
                }
                else {
                    for (gear = 1; gear < max_gear; gear++) {
                        if (curr_speed < tab->GetItemF(CVEH_F_GEAR_TRAN_SPEED, gear + 1))
                            break;
                    }
                    //don't gear down too early
                    if (last_gear > gear && curr_speed > (tab->GetItemF(CVEH_F_GEAR_TRAN_SPEED, gear + 1) * .7f))
                        ++gear;
                    else
                        if (gear > last_gear + 1)
                            gear = last_gear + 1;
                }
            }
            else {
                if (!moving_backward && curr_speed > 1.0f) {
                    want_dir = 0;
                    brake = true;
                }
                else {
                    gear = -1;
                }
            }
        }
        last_gear = gear;

        {
            if ((want_dir || steer) && !enabled) {
                Enable(true);
            }

            if (want_dir || steer) {
                engine_on = true;
            }
            //forces applied to joint motor under various circumstances
            const float brake_force = 100.0f;
            const float neutral_force = 3.0f;

            float vel = 0.0f, force;
            if (!want_dir) {
                force = brake ? brake_force : neutral_force;
            }
            else {
                force = tab->GetItemF(CVEH_F_GEAR_FORCE, gear + 1);
                float rel_speed = Min(curr_speed, tab->GetItemF(CVEH_F_GEAR_TRAN_SPEED, gear + 1));
                vel = .5f + rel_speed * .3f;
                vel *= (float)want_dir;
            }
            //process all wheels
            for (int i = jnt_wheels.size(); i--; ) {
                S_wheel* wheel = &jnt_wheels[i];
                PIPH_joint_hinge2 jnt = wheel->jnt;
                if (wheel->powered) {
                    float curr_angle = jnt->GetAngle();
                    float v = steer - curr_angle;
                    float k = tsec * 2.0f;
                    if (curr_angle * steer < 0.0f)
                        k *= 2.0f;
                    v = Max(-k, Min(k, v));
                    v *= 10.0f;
                    jnt->SetDesiredVelocity(v);
                }
                //apply motor onto joint
                jnt->SetDesiredVelocity(-vel, 1);
                jnt->SetMaxForce(force, 1);
            }
            if (snd_engine) {
                float m = 1.0f;
                if (gear) {
                    m = curr_speed * .4f / I3DFabs(tab->GetItemF(CVEH_F_GEAR_VEL, gear + 1));
                    m = Max(.7f, m);
                }
                float delta = m - last_frequency;
                const float max_change = tsec * 3.0f;
                if (I3DFabs(delta) > max_change) {
                    delta = (delta < 0.0f) ? -max_change : max_change;
                }
                last_frequency += delta;

                if (!snd_engine->IsOn() && engine_on)
                    snd_engine->SetOn(true);
                snd_engine->SetFrequency(last_frequency);
            }
            if (brake_light)
                brake_light->SetBrightness(I3D_VIS_BRIGHTNESS_EMISSIVE, brake ? 1.0f : 0.0f);
        }
#ifdef DEBUG_SHOW_INFO
        if (can_control) {
            DEBUG(C_xstr("Speed: #.1% km/h") % curr_speed);
            DEBUG(C_xstr("Gear: %") % gear);
        }
#endif

        last_pos = frame->GetWorldPos();

        C_actor_physics::Tick(tc);
        if (bodies.size())
            mission.GetScene()->SetFrameSectorPos(frame, bodies[0]->GetFrame()->GetWorldPos());

        // reset transient data
        gear = 0;
        curr_speed = 0.0f;
        moving_backward = false;
        want_dir = 0;       //0=stay, 1=forward, -1=backward
        steer = 0.0f;
    }
};

//----------------------------

PC_actor CreateVehicleActor(C_game_mission& gm1, PI3D_frame in_frm) {

    C_vehicle_imp* vp = new C_vehicle_imp(gm1, in_frm);
    return vp;
}

//----------------------------
