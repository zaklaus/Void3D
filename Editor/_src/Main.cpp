#include "all.h"
#include "GameFramework.h"
#include <Windows.h>

static C_command_line cmd_line;

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

//required
PC_actor GameCreateActor(PI3D_frame in_frm, E_ACTOR_TYPE in_type, const void* data){
   //allocate custom actors here
   return NULL;
}

static void RegisterGameInternals(){
   //register script methods, new actors and change any game settings in here
}

//----------------------------

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
            }
#endif

            RegisterGameInternals();
            
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
