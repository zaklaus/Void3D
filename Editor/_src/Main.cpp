#include "all.h"
#include <float.h>
#include <iexcpt.h>
#include "commandline.h"
#include "main.h"
#include "winstuff.h"
#include "Game_Cam.h"
#include "GameMission.h"
#include "SysTables.h"
#include <Tabler2.h>
#include "script.h"
#include <Insanity\AppInit.h>
#include <Windows.h>
#include <WinReg.h>

//----------------------------

#if defined _DEBUG && defined EDITOR
#define MIN_FPS 4
#define MAX_FPS 144
#else
#define MIN_FPS 4             //we don't accept less than this
#define MAX_FPS 144           //we don't accept more than this (the rest of time we're Sleeping)
#endif

extern S_application_data init_data;
//----------------------------
                              //globals
PIGraph igraph;
PISND_driver isound;
PI3D_driver driver;
LPC_controller controller;

//pointer to class which is updated each frame
C_smart_ptr<C_class_to_be_ticked> tick_class;

C_all_texts all_txt;
//I3D caches
C_I3D_model_cache_special model_cache;
C_I3D_sound_cache sound_cache;
C_I3D_special_anim_cache anim_cache;

S_game_configuration game_configuration;

#ifdef EDITOR
PC_editor editor;
#endif

static C_smart_ptr<C_dta_read> game_dta;
static C_command_line cmd_line;

//main data file - containing config files, texts, tables, etc
// it is initialized before any other data file is initialized
// and must always be copied at executable's path
#define GAME_DTA_FILENAME "data\\data.dta"


//----------------------------
#ifdef EDITOR
static void LogFunc(const char* cp) {

    static FILE* f_os_log;
    C_fstr log_name("%s.log", init_data.app_name);
    if (!f_os_log)
        f_os_log = fopen(log_name, "wt");
    if (f_os_log) {
        fwrite(cp, 1, strlen(cp), f_os_log);
        fwrite("\n", 1, 1, f_os_log);
        //re-open, so that we have immediate feedback
        fclose(f_os_log);
        f_os_log = fopen(log_name, "at");
    }
};
#endif
//----------------------------

#if defined EDITOR
void I3DAPI cbDriver(I3D_CALLBACK_MESSAGE msg, dword prm1, dword prm2, void* context) {

    switch (msg) {
    case I3DCB_MESSAGE:
        if (editor) {
            PC_editor_item_Log ei = (PC_editor_item_Log)editor->FindPlugin("Log");
            if (ei)
                ei->AddText((const char*)prm1);
        }
        break;

    case I3DCB_DEBUG_MESSAGE:
        if (editor) {
            switch (prm2) {
            case 0:
                DEBUG((const char*)prm1);
                break;
            case 1:
                PRINT((const char*)prm1);
                break;
            case 2:
            {
                PC_editor_item_Log ei_log = (PC_editor_item_Log)editor->FindPlugin("Log");
                if (ei_log)
                    ei_log->AddText((const char*)prm1, 0xffff0000);
            }
            break;
            }
        }
        break;

    case I3DCB_DEBUG_CODE:
        switch (prm1) {
        case 0:                 //debug-line
        {
            struct S_dl {
                S_vector from, to;
                int type;
                dword color;
            } *l = (S_dl*)prm2;
            PC_editor_item_DebugLine ei = (PC_editor_item_DebugLine)editor->FindPlugin("DebugLine");
            if (ei)
                ei->AddLine(l->from, l->to, (E_DEBUGLINE_TYPE)l->type, l->color);
        }
        break;
        case 1:                 //debug-point
        {
            struct S_dp {
                S_vector pos;
                float radius;
                int type;
                dword color;
            } *p = (S_dp*)prm2;
            PC_editor_item_DebugLine ei = (PC_editor_item_DebugLine)editor->FindPlugin("DebugLine");
            if (ei)
                ei->AddPoint(p->pos, p->radius, (E_DEBUGLINE_TYPE)p->type, p->color);
        }
        break;

        default: assert(0);
        }
        break;
    }
}
#endif //EDITOR

//----------------------------
// System initialization function.
bool InitSystem(const C_command_line& cmd_line) {

    I3D_RESULT ir;
#ifdef _DEBUG
    _set_printf_count_output(1);
#endif

    int res_x = game_configuration.mode_x;
    int res_y = game_configuration.mode_y;

    IG_INIT ig;
    memset(&ig, 0, sizeof(ig));
    ig.size_x = res_x;
    ig.size_y = res_y;
    ig.bits_per_pixel = game_configuration.bits_per_pixel;
    ig.flags =
#ifdef EDITOR
        IG_NOHIDEMOUSE |
        IG_WARN_OUT |
#endif
        (game_configuration.tripple_buffering ? IG_TRIPPLEBUF : 0) |
        (game_configuration.fullscreen ? IG_FULLSCREEN : 0);

#if defined EDITOR || defined _DEBUG
    ig.log_func = LogFunc;
#endif
    ig.adapter_id = game_configuration.adapter_identifier;

    ig.flags |= IG_VERBOSE;
    ig.antialias_mode = (IG_INIT::E_ANTIALIAS_MODE)game_configuration.antialias_mode;

    //initialize graphics
    igraph = IGraphCreate(&ig);
    if (!igraph) {
        OsMessageBox(NULL,
            "Unable to initialize graphics. This program requires DirectX 9 or greater. Please make sure you have latest DirectX installed, you can obtain it from www.microsoft.com. Also install latest drivers for your graphics card. Finally, you may need to run IConfig.exe and select appropriate video mode.",
            init_data.app_name, MBOX_OK);
        return false;
    }
    //let disable suspend in editor on user request(cp testing etc.)
#ifdef EDITOR
    if (cmd_line.disable_suspend)
        igraph->EnableSuspend(false);
#endif

    igraph->SetAppName(init_data.app_name);
    //clear all buffers
    igraph->ClearViewport();
    igraph->UpdateScreen();
    if (game_configuration.fullscreen) {
        igraph->ClearViewport();
        igraph->UpdateScreen();
        if (game_configuration.tripple_buffering) {
            igraph->ClearViewport();
            igraph->UpdateScreen();
        }
    }
    //initialize sounds
    if (game_configuration.use_sounds) {
        ISND_INIT is;
        memset(&is, 0, sizeof(is));
        is.mix_frequency = 22050;
        is.num_channels = 2;
        is.bits_per_second = 16;
        is.hwnd = igraph->GetHWND();
        is.guid = NULL;
#ifdef EDITOR
        is.log_func = LogFunc;
        is.flags |= ISC_WARN_OUT;
#endif

        if (game_configuration.sounds_use_3D_hw) {
            is.flags = ISC_3DENABLE;
            if (game_configuration.sounds_use_EAX)
                is.flags |= ISC_USE_EAX;
        }

        isound = ISoundCreate(&is,
#if defined EDITOR || defined _DEBUG
            cmd_line.debug_use_debug_dlls);
#else
            false);
#endif
        if (!isound) {
            OsMessageBox(NULL,
                "Unable to initialize sound system. The application will now continue to run with sounds disabled.",
                init_data.app_name, MBOX_OK);
        }
    }

    //initialize 3D
    dword i3d_init_flags = 0;
#ifdef EDITOR
    i3d_init_flags |= I3DINIT_WARN_OUT;
#endif

    I3DINIT is = {
       igraph,
       i3d_init_flags,
       isound,
 #ifdef EDITOR
       LogFunc
 #else
       NULL
 #endif
    };
    ir = I3DCreate(&driver, &is);
    if (I3D_FAIL(ir)) {
        igraph->Release(); igraph = NULL;
        const char* cp = "(unknown error)";
        switch (ir) {
        case I3DERR_OUTOFMEM:
            cp = "Probable cause: not enough video memory for selected video mode";
            break;
        }
        OsMessageBox(NULL, C_fstr("Unable to initialize 3D system: %s", cp), init_data.app_name, MBOX_OK);
        return false;
    }
    if (game_configuration.use_dbase) {
        if (I3D_FAIL(driver->SetDataBase(init_data.dbase_name, init_data.dbase_size, 15))) {
            driver->SetDataBase(init_data.dbase_name, init_data.dbase_size / 4, 15);
        }
    }
    driver->SetState(RS_LOADMIPMAP, true);
    driver->SetState(RS_TEXTUREDITHER, true);
    driver->SetState(RS_LMTRUECOLOR, true);
    driver->SetState(RS_LMDITHER, true);
    driver->SetState(RS_TEXTURECOMPRESS, game_configuration.compress_textures);

    driver->AddDir(I3DDIR_SOUNDS, "");  //we use cache, so fake driver to load from given dir
    driver->AddDir(I3DDIR_PROCEDURALS, "Tables\\Procedurals");

#ifdef EDITOR
    driver->SetCallback(cbDriver, 0);
#endif
    return true;
}

//----------------------------
// System shutdown function.
void CloseSystem() {

    if (editor) {
#ifdef EDITOR
        editor->Release();
        editor = NULL;
#endif
    }

    if (isound) {
        isound->Release();
        isound = NULL;
    }

    if (igraph) {
        //store current setting to config
        game_configuration.mode_x = igraph->Scrn_sx();
        game_configuration.mode_y = igraph->Scrn_sy();
        game_configuration.bits_per_pixel = igraph->GetPixelFormat()->bytes_per_pixel * 8;
        game_configuration.fullscreen = (igraph->GetFlags() & IG_FULLSCREEN);
        game_configuration.WriteSysConfig(C_str(init_data.reg_base) + init_data.app_name);

        igraph->MouseClose();
        igraph->Release();
        igraph = NULL;
    }
    if (driver) {
        driver->Release();
        driver = NULL;
    }
}

//----------------------------

#ifdef EDITOR

class C_tick_class_begin : public C_class_to_be_ticked {
    C_smart_ptr<C_game_mission> mission;
    const C_command_line& cmd_line;
public:
    C_tick_class_begin(const C_command_line& cmd_line1) :
        cmd_line(cmd_line1)
    {
        mission = ((PC_editor_item_Mission_spec)editor->FindPlugin("Mission"))->GetMission();
    }

    //----------------------------

    virtual dword GetID() const { return 'TBEG'; }
    virtual void Render() {}

    virtual void Tick(const S_tick_context& tc) {

        //load default mission and switch to game mode
#ifdef EDITOR
        PC_editor_item_Mission_spec e_miss = (PC_editor_item_Mission_spec)editor->FindPlugin("Mission");
        e_miss->Reload(false);
#else //EDITOR
        {
            mission->Open("gametest", OPEN_NO_EDITOR);
        }
#endif   //!EDITOR

        //since we process this in Tick, update the loaded mission
        mission->Tick(tc);
        SetTickClass(mission);
    }
};

#endif   //EDITOR

//----------------------------
// Return smoothed relative time - cut off too big changes in time (due resource loading, etc).
static dword SmoothTime(dword time) {

    const int NUM_SAMPLES = 128;
    static dword time_samples[NUM_SAMPLES] = {};
    static dword curr_index = 0;

    time_samples[curr_index] = time;
    if (++curr_index == NUM_SAMPLES)
        curr_index = 0;

    dword average = 0;
    for (dword i = NUM_SAMPLES; i--; )
        average += time_samples[i];
    average /= NUM_SAMPLES;

    time = Min(time, average * 2);
    return time;
}

//----------------------------
//----------------------------
// Main game loop.
void GameLoop(const C_command_line& cmd_line) {

    assert(GetTickClass());

    igraph->NullTimer();

#ifdef EDITOR
    PC_editor_item_Exit e_exit = editor ? (PC_editor_item_Exit)editor->FindPlugin("Exit") : NULL;
#endif
    //driver->SetNightVision(true);

    S_tick_context tc;
    tc.p_ctrl = controller;

    tc.time = 0;
    while (true) {
        tc.key = igraph->ReadKey(true);
        tc.modify_keys = (byte)igraph->GetShiftKeys();

        igraph->MouseUpdate();
        tc.mouse_rel[0] = igraph->Mouse_rx();
        tc.mouse_rel[1] = igraph->Mouse_ry();
        tc.mouse_rel[2] = igraph->Mouse_rz();
        tc.mouse_buttons = (byte)igraph->GetMouseButtons();
        //DEBUG(tc.mouse_buttons);

        bool want_close = igraph->GetWindowCloseStatus();
#if defined MIN_FPS
        tc.time = igraph->GetTimer(1000 / MAX_FPS, 1000 / MIN_FPS);
#else
        tc.time = igraph->GetTimer(1, 0);
#endif
        tc.time = SmoothTime(tc.time);

#ifdef _DEBUG
        //fast quit during development
#ifndef EDITOR
        if (controller->Get(CS_FAST_QUIT)) {
            break;
        }
#endif//EDITOR
#endif//_DEBUG

#ifdef EDITOR
        if (editor) {
            PC_editor_item_Mission_spec e_miss = (PC_editor_item_Mission_spec)editor->FindPlugin("Mission");
            PC_editor_item_MouseEdit e_medit = (PC_editor_item_MouseEdit)editor->FindPlugin("MouseEdit");
            //update Insanity editor
            PC_game_mission pm = e_miss->GetMission();
            bool gcam_acquired = (pm->IsInGame() && pm->GetScene()->GetActiveCamera() == pm->GetGameCamera()->GetCamera());
            //let mouse input go in tick class unless we are in editor or we are free fly in game
            if (editor->IsActive() || !gcam_acquired) {
                editor->Tick(tc.modify_keys, tc.time,
                    tc.mouse_rel[0], tc.mouse_rel[1], tc.mouse_rel[2], tc.mouse_buttons);
                //clear mouse imput in tc - not to be passed further
                memset(tc.mouse_rel, 0, sizeof(tc.mouse_rel));
                tc.mouse_buttons = 0;
            }
            if (editor->IsActive()) {
                if (tc.key != K_NOKEY)
                    editor->MenuHit(0, tc.key, tc.modify_keys);
            }
            else {
                //check few keys also in game mode
                switch (tc.key) {
                case K_F5:        //edit mode
                case K_F7:        //set camera focus
                case K_F8:        //cam acq
                case K_SPACE:     //toggle edit mode
                case K_ESC:
                case K_NUM0: case K_NUM1: case K_NUM2: case K_NUM3: case K_NUM4: case K_NUM5: case K_NUM6: case K_NUM7: case K_NUM8: case K_NUM9:
                    //default:
                    editor->MenuHit(0, tc.key, 0);
                    break;
                }
                if (gcam_acquired)
                    editor->Tick(0, tc.time, 0, 0, 0, 0);
            }
            PC_editor_item_TimeScale e_tscale = (PC_editor_item_TimeScale)editor->FindPlugin("TimeScale");
            if (e_tscale) {
                //scale time
                float f = e_tscale->GetScale();
                if (f != 1.0f) {
                    static float time_rest;
                    float time = (float)tc.time * f + time_rest;
                    time_rest = (float)fmod(time, 1.0f);
                    tc.time = FloatToInt((float)floor(time));
                }
            }
        }
        //check exit request
        if (want_close || (e_exit && e_exit->GetState()) ||
            (tc.key == (char)K_ESC && tc.modify_keys == SKEY_SHIFT)) {

            PC_editor_item_Mission e_miss = editor ? (PC_editor_item_Mission)editor->FindPlugin("Mission") : NULL;
            if (!e_miss)
                break;
            if (e_miss->Save(true))
                break;
            if (e_exit)
                e_exit->SetState(false);
        }

#else //EDITOR
        //non-sync exit allowed only by window's close button
        if (want_close)
            break;

        //screenshots may be captured from any tick class
        // - check it here, so that we don't need to check it on multiple places
        if (controller->Get(CS_SCREENSHOT, true))
            SaveScreenShot();
#endif//!EDITOR
        controller->Update(tc);

        //tick all
        GetTickClass()->Tick(tc);
        if (!GetTickClass())     //tick class destroyed => end of game
            break;

        //render and update screen
        GetTickClass()->Render();
        igraph->UpdateScreen();
    }
    driver->SetViewport(I3D_rectangle(0, 0, igraph->Scrn_sx(), igraph->Scrn_sy()));
    igraph->ClearViewport();
    igraph->UpdateScreen();
}

//----------------------------
//----------------------------

void TablesCloseAll();
void SetBreakAlloc(dword);

//----------------------------
// Function called when crash encountered. It flushes database and lets user
// save current work.
static void __stdcall CrashCallback() {

    //reset FPU so that we may do some tasks
    _fpreset();

    //hide window, so that user sees the crash also in fullscreen
    if (igraph) {
        igraph->EnableSuspend(true);
        igraph->FlipToGDI();
        OsShowWindow(igraph->GetHWND(), false);
        igraph->ProcessWinMessages();
    }

    if (driver) {
        //save what's possible
#ifdef EDITOR
        PC_editor_item_Mission e_mission = editor ? (PC_editor_item_Mission)editor->FindPlugin("Mission") : NULL;
        if (e_mission) {
            if (editor->IsModified()) {
                int i = OsMessageBox(NULL, "The program crashed.\nDo you want to save mission before quitting?",
                    "Program crash", MBOX_YESNO);
                if (i == MBOX_RETURN_YES)
                    e_mission->Save(false);
            }
        }
        //close editor
        if (editor) {
            C_smart_ptr<C_editor> ed = editor;  //keep ref until global 'editor' gets NULL
            editor->Release();
            editor = NULL;
        }
#endif
        driver->FlushDataBase();
    }
    //disable leak warning
#ifdef _DEBUG
    SetBreakAlloc((dword)-2);
#endif

    TablesCloseAll();
}

//----------------------------
// List of DTA files.
                              //flags specifying under which install type particular DTA was installed to dest dir
#define DTA_INSTALL_MIN    0x1
#define DTA_INSTALL_MED    0x2
#define DTA_INSTALL_MAX    0x4

static const struct {
    char* filename;
    dword flags;
} dta_files[] = {
   { "maps",      DTA_INSTALL_MAX},
   { "sounds",    DTA_INSTALL_MAX},
   { "missions",  DTA_INSTALL_MAX | DTA_INSTALL_MED },
   { "models",    DTA_INSTALL_MAX | DTA_INSTALL_MED },
   { "anims",     DTA_INSTALL_MAX | DTA_INSTALL_MED },
   { NULL, 0}
};

//----------------------------
// list of opened data files, this is intialized by DtaOpenAll.
static C_vector<C_smart_ptr<C_dta_read> > dta_list;

//----------------------------

void DtaCloseAll() {
    dta_list.clear();
}

//----------------------------

bool DtaOpenAll() {
#if !defined EDITOR && 1
    //close all previously open DTA files
    DtaCloseAll();

    //flags indexed by S_game_cfg_1::E_INSTALL_TYPE
    static dword inst_flags[] = {
       DTA_INSTALL_MIN, DTA_INSTALL_MED, DTA_INSTALL_MAX,
    };
    //open data files
    for (int i = 0; dta_files[i].filename; i++) {
        dword file_flags = dta_files[i].flags;
        //search for the file on installed location
        C_str fullname = C_fstr("data\\%s.dta", dta_files[i].filename);

        PC_dta_read dta = dtaCreate(fullname);
        if (!dta) {
#if 0
            OsMessageBox(NULL, C_fstr(all_txt["err_NoFile"], (const char*)fullname), init_data.app_name, MBOX_OK);
            return false;
#else
            continue;
#endif
        }
        else {
            dta_list.push_back(dta);
            dta->Release();
        }
    }
#endif
    return true;
}

//----------------------------

//list of text files
static const char* texts_file_names[] = {
   NULL,
};

//----------------------------
// Open all text files, use current language. If some text couldn't be opened,
// the function displays an error message and returns false.
bool TextOpenAll(const char* language_name) {

    for (int i = 0; texts_file_names[i]; i++) {
        C_fstr file_name("text\\%s\\%s", (const char*)language_name, texts_file_names[i]);
        if (!all_txt.AddFile(file_name)) {
            return false;
        }
    }
    return true;
}

//----------------------------
                              //default controller settings
static const struct S_controller_item {
    int slot;
    C_controller::E_VALUE_MODE value_mode;
    struct {
        C_controller::E_INPUT_DEVICE input_device;
        dword data;
        float conversion;
    } input[2];
} controller_init[] = {
    //basic movement
CS_TURN_LEFT,  C_controller::VM_FLOAT, {C_controller::ID_KEYBOARD, K_CURSORLEFT, 1, C_controller::ID_JOY_AXIS, 0x80000000},
CS_MOVE_FORWARD, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_W, 0, C_controller::ID_JOY_AXIS, 0x80000001, .4f},
CS_TURN_RIGHT, C_controller::VM_FLOAT, {C_controller::ID_KEYBOARD, K_CURSORRIGHT, 1, C_controller::ID_JOY_AXIS, 0},
CS_MOVE_BACK,  C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_S, 0, C_controller::ID_JOY_AXIS, 0x00000001, .4f},
CS_MOVE_LEFT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_A, 0, C_controller::ID_JOY_AXIS, 0x80000002, .4f},
CS_MOVE_RIGHT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_D, 0, C_controller::ID_JOY_AXIS, 2, .4f},
CS_JUMP, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_E, 0, C_controller::ID_MOUSE_BUTTON, 1},
//CS_RESET_AIM, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_C},
//CS_SLANT_RIGHT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_PAGEDOWN},
//CS_SLANT_LEFT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_DEL},

                           //mode change
//CS_STAY_UP,    C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_W},//, 0, C_controller::ID_MOUSE_AXIS, 2},
CS_STAY_DOWN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_Q, 0, C_controller::ID_MOUSE_BUTTON, 2},
CS_RUN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_SHIFT, 0, C_controller::ID_JOY_AXIS, 1, .7f},
//CS_STRAFE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_ALT, 0, C_controller::ID_JOY_BUTTON, 2},

                           //weapons
CS_WEAPON_1, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_1},
CS_WEAPON_2, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_2},
CS_WEAPON_3, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_3},
CS_WEAPON_4, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_4},
CS_WEAPON_5, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_5},
CS_WEAPON_6, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_6},
CS_WEAPON_7, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_7},
CS_WEAPON_NO, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_BACKAPOSTROPH},

//action
CS_FIRE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_ENTER, 0, C_controller::ID_MOUSE_BUTTON, 0},
CS_ALT_FIRE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_INS},
CS_RELOAD, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_R},
CS_DROP,  C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_T},
CS_FREE_LOOK, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_Z},
CS_INV_SCROLL_LEFT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_LBRACKET, 0, C_controller::ID_MOUSE_AXIS, 2},
CS_INV_SCROLL_RIGHT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_RBRACKET, 0, C_controller::ID_MOUSE_AXIS, 0x80000002},
CS_INV_HIDE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_BACKSPACE},
//CS_INV_USE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_ENTER, 0},
CS_USE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F, 0, C_controller::ID_MOUSE_BUTTON, 2},
//CS_USE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_SPACE, 0, C_controller::ID_JOY_BUTTON, 4},
//CS_SHIELD, C_controller::VM_BOOL,{C_controller::ID_KEYBOARD, K_LALT},

                           //camera management
CS_CAM_ZOOM_IN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_EQUALS},
CS_CAM_ZOOM_OUT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_MINUS},
//CS_CAM_ZOOM_UP, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_GREYSLASH},
//CS_CAM_ZOOM_DOWN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_GREYDOT},

CS_DEBUG, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F11},
//CS_FAST_QUIT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F10},

                           //game management
//CS_HELP, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F1},
CS_SCREENSHOT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F12},
//   CS_PANEL_IN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD,  K_GREYPLUS},
//   CS_PANEL_OUT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_GREYMINUS},
   CS_GAME_ESCAPE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_ESC},

   CS_OBJECTIVES, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F1},

   //CS_SAVEGAME, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F3},
   //CS_LOADGAME, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F4},

   CS_QUICKSAVE, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F9},
   CS_QUICKLOAD, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_F10},

   //pop up menu
CS_POP_UP_SELECT, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_HOME},
CS_POP_UP_UP, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_INS},
CS_POP_UP_DOWN, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_PAGEUP},

//#if defined DEBUG_PANEL || 1
   //CS_SWITCH_PANEL, C_controller::VM_BOOL, {C_controller::ID_KEYBOARD, K_U},
//#endif
   -1
};

//----------------------------

void SetupControllerDefaults(int slot) {

    struct S_hlp {
        static void SetupSingleSlot(int slot_index) {
            const S_controller_item& ci = controller_init[slot_index];
            for (int j = 0; j < 2; j++) {
                controller->SetupSlotInput(ci.slot, j, ci.value_mode, ci.input[j].input_device, ci.input[j].data);
                controller->SetConversionValue(ci.slot, j, *(dword*)&ci.input[j].conversion);
            }
        }
    };
    if (slot == -1) {
        //setup default controller values - all slots
        for (int i = 0; controller_init[i].slot != -1; i++) {
            S_hlp::SetupSingleSlot(i);
        }
    }
    else {
        S_hlp::SetupSingleSlot(slot);
    }
}

//----------------------------
//----------------------------

bool AppInit(PI3D_driver drv) {

    driver = drv;
    igraph = driver->GetGraphInterface();
    isound = driver->GetSoundInterface();

    //#ifndef EDITOR
                                  //open DATA.DTA - needed for texts initialization
    game_dta = dtaCreate(GAME_DTA_FILENAME);
    if (game_dta)
        game_dta->Release();
    //#endif
    if (!all_txt.AddFile("text\\english\\system.txt")) {
        //we don't have localized texts yet, use direct english.
        OsMessageBox(NULL, "Failed to open text files. Please reinstall the game.", init_data.app_name, MBOX_OK);
        return false;
    }
    //open DTA files
    if (!DtaOpenAll()) {
        all_txt.Close();
        return false;
    }
    if (!TextOpenAll("English")) {
#ifndef EDITOR
        return false;
#endif
    }
    TablesOpenAll();

    //init caches
    model_cache.SetCacheSize(64);
    sound_cache.SetCacheSize(64);
    anim_cache.SetCacheSize(64);
    model_cache.SetDir("Models");
    sound_cache.SetDir("Sounds");
    anim_cache.SetDir("");

    //register directories
    {
        driver->AddDir(I3DDIR_MAPS, "Maps");
        C_cache ck;
        if (ck.open("Maps\\Paths.txt", CACHE_READ)) {
            while (!ck.eof()) {
                char line[256];
                ck.getline(line, sizeof(line));
                if (line[0] == ';' || !line[0])
                    continue;
                driver->AddDir(I3DDIR_MAPS, C_fstr("Maps\\%s", line));
            }
        }
    }

    //initialize controller
    controller = CreateController();
    controller->Init(igraph, NULL, CS_LAST);
    SetupControllerDefaults();

#ifndef EDITOR
    igraph->MouseInit();
#endif

#if (defined EDITOR || defined _DEBUG) && 0
    script_man->SetDebugMode(true);
#endif

    return true;
}

//----------------------------

#ifndef EDITOR
static FILE* f_os_log;

void LogFunc(const char* cp) {
    C_fstr log_name("%s.log", init_data.app_name);
    if (!f_os_log)
        f_os_log = fopen(log_name, "wt");
    if (f_os_log) {
        fwrite(cp, 1, strlen(cp), f_os_log);
        fwrite("\n", 1, 1, f_os_log);
        //re-open, so that we have immediate feedback
        fclose(f_os_log);
        f_os_log = fopen(log_name, "at");
    }
    //write into window title
    if (igraph)
        igraph->SetAppName(C_xstr("% - Last error: '%'") % init_data.app_name % cp);
};
#endif

//----------------------------

#ifdef EDITOR
                              //editor plugins initialization
void InitMissionPlugin(PC_editor, C_game_mission&);
//void InitDebugPlugin(PC_editor, C_game_mission&);
void InitCheckpointPlugin(PC_editor, C_game_mission&);
void InitPlugin_Script(PC_editor, C_game_mission&);
void InitTestPlugin(PC_editor, C_game_mission&);
void InitSysConfigPlugin(PC_editor, C_game_mission&);

static C_editor_item_Selection::t_GetFrameScript GetFrameScriptName;
C_editor_item_Selection::t_GetFrameActor GetFrameActorName;

const char* GetFrameScriptName(PI3D_frame frm) {

    S_frame_info* fi = GetFrameInfo(frm);
    if (fi && fi->vm)
        return fi->vm->GetName();
    return NULL;
}

//----------------------------

static PC_editor EdCreate() {

    C_game_mission* CreateGameMission();
    C_game_mission* mission = CreateGameMission();

    editor = CreateEditor(driver, ((C_mission*)mission)->GetScene());

    editor->SetModelCache(&model_cache);
    editor->SetSoundCache(&sound_cache);

    InitSysConfigPlugin(editor, *mission);
    InitMissionPlugin(editor, *mission);
    //InitDebugPlugin(editor, *mission);
    InitCheckpointPlugin(editor, *mission);
    InitPlugin_Script(editor, *mission);

    editor->LoadState("editor\\editor");
    PC_editor_item_Selection e_slct = (PC_editor_item_Selection)editor->FindPlugin("Selection");
    if (e_slct) {
        e_slct->SetGetScriptNameFunc(GetFrameScriptName);
        e_slct->SetGetActorNameFunc(GetFrameActorName);
    }
    model_cache.PushWorkMission((C_mission*)mission);
    mission->Release();

    return editor;
}
#endif

//----------------------------

static void AppRun() {

    //apply tables
    ApplySoundEnvTable();
    ApplyMaterialTable();
    //driver->SetState(RS_PROFILER_MODE, 1);
    {
        C_class_to_be_ticked* init_tc = NULL;
#if defined EDITOR
        {
            init_tc = new C_tick_class_begin(cmd_line);
        }
#else
        if (cmd_line.cmdline_mission.Size()) {
            PC_game_mission mission = CreateGameMission();
            E_MISSION_IO mio = mission->Open(cmd_line.cmdline_mission, 0);
            if (mio == MIO_OK) {
                //mission->SetMode(MM_SINGLE);
                mission->GameBegin();
                init_tc = mission;
                init_tc->AddRef();
            }
            mission->Release();
        }
        if (!init_tc) {
            //@todo -- main menu
        }
#endif//!EDITOR
        if (init_tc) {
            SetTickClass(init_tc);
            init_tc->Release();
        }
    }
    if (GetTickClass()) {
        GameLoop(cmd_line);
        SetTickClass(NULL);
    }
}

//----------------------------

static void AppClose() {

#ifdef EDITOR
    if (editor) {
        editor->SaveState("editor\\editor");
        //C_smart_ptr<C_editor> ed = editor;  //keep ref until global 'editor' gets NULL
        editor->Release();
        editor = NULL;
    }
#endif                        //EDITOR

    script_man->ShutDown();
    model_cache.Clear();
    sound_cache.Clear();
    anim_cache.Clear();

#ifndef EDITOR
    //WriteControllerConfig();
#endif
    if (controller) {
        controller->Release();
        controller = NULL;
    }

    TablesCloseAll();
    all_txt.Close();
    DtaCloseAll();
    //game_configuration.Close();
}

//----------------------------

static C_str AppCrashInfo() {

    C_str ret;
    if (GetTickClass()) {
        dword id = GetTickClass()->GetID();
        char ids[5];
        *(dword*)ids = id;
        reverse(ids, ids + 4);
        ids[4] = 0;
        ret = C_fstr("Tick class: %s", ids);
        switch (id) {
        case 'MISS':
        case 'GMIS':
        {
            C_mission* miss = (C_mission*)GetTickClass();
            const S_vector& cpos = miss->GetScene()->GetActiveCamera()->GetWorldPos();
            ret += C_fstr("  Mission name: '%s', Camera: %.1f, %.1f, %.1f",
                (const char*)miss->GetName(), cpos.x, cpos.y, cpos.z);
        }
        break;
        }
    }
    return ret;
}

//----------------------------

S_application_data init_data = {
   "Void Demo",
   "Software\\V4 Games\\Void Demo\\",
#ifdef _DEBUG
   "W:\\!Void_Crashes\\",
#else
   "contact@madaraszd.net",
#endif
   "_tmp\\dbase.bin",
   200 * 1024 * 1024,
   &cmd_line,
   AppInit,
   //AppRun,
   AppClose,
#ifdef EDITOR
   EdCreate,
#else
   NULL,
#endif
   AppCrashInfo,
};

//----------------------------

static void SetupExceptionHandling() {

    if (init_data.crash_send_addr)
        SetExceptionSendFunc(WinSendCrashReport);
    SetExceptionCallback(CrashCallback);
#if defined _MSC_VER && defined _DEBUG && 1
    //catch hardware exceptions
    dword flags = (dword)~(_EM_INVALID | _EM_OVERFLOW | _EM_ZERODIVIDE);
    InitializeExceptions(flags);
#else
    InitializeExceptions();
#endif
}

//----------------------------

//----------------------------
//Read configuration. If fail, invoke config dialog until settings are readed correctly.
//Return value:
//    true ... config was read successfully
//    false ... config read failed, and user has canceled the config dialog
static bool ReadConfigUntilOkCancel(const char* language) {

    while (true) {
        //read info from Windows registry
        bool cfg_read =
            game_configuration.ReadSysConfig(C_str(init_data.reg_base) + init_data.app_name);
        if (cfg_read)
            break;
        //failed to read initialization config...
        // run hardware config
        E_CONFIG_STATUS st = RunGameConfiguration(C_str(init_data.reg_base) + init_data.app_name, language);
        switch (st) {
        case CONFIG_OK:
            break;
        default:                //failed to run program
            OsMessageBox(NULL, "Failed to invoke configuration dialog.", init_data.app_name, MBOX_OK);
            //flow...
        case CONFIG_CANCELED:
            return false;
        }
    }
    return true;
}

//----------------------------

#define GAMEINFO_FILE "gameinfo.bin"

static PC_table game_data_table;

/*
* S_application_data init_data = {
   "Void Demo",
   "Software\\V4 Games\\Void Demo\\",
#ifdef _DEBUG
   "W:\\!Void_Crashes\\",
#else
   "contact@madaraszd.net",
#endif
   "_tmp\\dbase.bin",
   200 * 1024 * 1024,
   &cmd_line,
   AppInit,
   //AppRun,
   AppClose,
#ifdef EDITOR
   EdCreate,
#else
   NULL,
#endif
   AppCrashInfo,
};

 */

enum game_props_data_index
{
    GAME_F_APP_NAME,
    GAME_F_REGISTRY_PATH,
    GAME_F_ERR_REPORT_PATH,
    GAME_F_DB_NAME,
    GAME_F_DB_SIZE,
};

void OpenGameDataEditor(){
    const C_table_element te_game_props[] = {
        {TE_STRING, GAME_F_APP_NAME, "Application Name", 240, 0, 0, "Application name"},
        {TE_STRING, GAME_F_REGISTRY_PATH, "Registry Path", 255, 0, 0, "Application registry path (used for storing game settings, ...)"},
        {TE_STRING, GAME_F_ERR_REPORT_PATH, "Error Report Email", 240, 0, 0, "Error reporting e-mail destination"},
        {TE_STRING, GAME_F_DB_NAME, "Cache Database Path", 255, 0, 0, "Cache database file path"},
        {TE_INT, GAME_F_DB_SIZE, "Cache Database Size", 0,  1024, 200, "Cache database reserved file size in megabytes"},
        {TE_NULL}
    };

    const C_table_template templ_game_props = {"Game Data", te_game_props};
    game_data_table->Load(&templ_game_props, TABOPEN_TEMPLATE);

    if (OsIsDirExist(GAMEINFO_FILE)){
        game_data_table->Load(GAMEINFO_FILE, TABOPEN_UPDATE | TABOPEN_FILENAME);
    }

    HWND editor_window = (HWND)game_data_table->Edit(&templ_game_props, NULL, NULL, 0);
    OsCenterWindow(editor_window, NULL);
    WaitForSingleObject(editor_window, INFINITE);

    game_data_table->Save(GAMEINFO_FILE, TABOPEN_FILENAME);
    MessageBox(NULL, "Manifest gameinfo.bin has been edited!", "Void application", MB_OK);
}

bool SetupGameData(S_application_data &app_data, bool first_try=true){
    if (game_data_table){
        game_data_table->Release();
        game_data_table = NULL;
    }
    game_data_table = CreateTable();
    
    if (!OsIsDirExist(GAMEINFO_FILE)){
#ifndef EDITOR
        MessageBox(NULL, "Manifest gameinfo.bin was not found! Please re-install the game!", "Void application error", MB_OK);
        return false;
#else
        if (first_try)
            OpenGameDataEditor();
        else{
            MessageBox(NULL, "Manifest gameinfo.bin was not found! Make sure you configure it!", "Void application error", MB_OK);
            return false;
        }
#endif
    }

    game_data_table->Load(GAMEINFO_FILE, TABOPEN_FILENAME);

    app_data.app_name = game_data_table->ItemS(GAME_F_APP_NAME);
    app_data.reg_base = game_data_table->ItemS(GAME_F_REGISTRY_PATH);
    app_data.crash_send_addr = game_data_table->ItemS(GAME_F_ERR_REPORT_PATH);
    app_data.dbase_name = game_data_table->ItemS(GAME_F_DB_NAME);
    app_data.dbase_size = game_data_table->ItemI(GAME_F_DB_SIZE) * 1024 * 1024;

    return true;
}

int GameRun(const S_application_data& app_data, const char* cp_cmd_line) {

    try {
        SetupExceptionHandling();
        game_data_table = NULL;

        if (!SetupGameData(const_cast<S_application_data&>(app_data))){
            return 0;
        }

        C_command_line& cmd_line = *app_data.cmd_line;
        //process command-line, exit if some errors encountered
        if (!cmd_line.Scan(cp_cmd_line, app_data.app_name))
            return 1;

        C_str reg_base = C_str(app_data.reg_base) + app_data.app_name;
        char language[64];
        *language = 0;

        if (cmd_line.run_config) {
            return !RunGameConfiguration(C_str(app_data.reg_base) + app_data.app_name, true, language);
        }
        if (cmd_line.edit_game_data){
            OpenGameDataEditor();
            SetupGameData(const_cast<S_application_data&>(app_data), false);
            return 0;
        }
        if (cmd_line.run_config) {
            E_CONFIG_STATUS st = RunGameConfiguration(C_str(app_data.reg_base) + app_data.app_name, true, language);
            if (st != CONFIG_OK)
                return 0;
        }

        if (!ReadConfigUntilOkCancel(language))
            return 3;

        if (InitSystem(cmd_line)) {
            if (app_data.Init && !(app_data.Init(driver))) {
                CloseSystem();
                return 2;
            }

#ifdef EDITOR
            if (app_data.EdCreate) {
                editor = (*app_data.EdCreate)();
                void InitPhysicsStudio(PC_editor editor);
                InitPhysicsStudio(editor);
            }
#endif
            AppRun();
        }
        if (app_data.Close)
            app_data.Close();
        CloseSystem();
    }
    catch (const C_except& e) {
        //e.ShowCallStackDialog();
        MessageBox(NULL, e.what(), "Void application error", MB_OK);
    }
    return 0;
}


//----------------------------

int Main(void* hi, const char* cp_cmd_line) {

    //executables reside in .\BIN directory, so set working path one level down
    _chdir("..");
    return GameRun(init_data, cp_cmd_line);
}

//----------------------------
