#include "all.h"
#include "GameFramework.h"
#include <Windows.h>

static C_command_line cmd_line;

S_application_data init_data = {
   "Void Demo",
   "Software\\V4 Games\\Void Demo\\",
#ifdef _DEBUG
   "C:\\!Void_Crashes\\",
#else
   "contact@madaraszd.net",
#endif
   "_tmp\\dbase.bin",
   200 * 1024 * 1024,
   &cmd_line,
   AppInit,
   AppClose,
#ifdef EDITOR
   EdCreate,
#else
   NULL,
#endif
   AppCrashInfo,
};

//required
void GameRegisterSymbols(){
   //call RegisterSymbol(VM_LOAD_SYMBOL) to hook up custom methods
}

//required
PC_actor GameCreateActor(PI3D_frame in_frm, E_ACTOR_TYPE in_type, const void* data){
   //allocate custom actors here
   return NULL;
}

class C_tick_demo_class : public C_class_to_be_ticked {
public:
   C_tick_demo_class()
   {
   }

   //----------------------------

   virtual dword GetID() const { return 'GMIS'; }
   virtual void Render() {
   }

   virtual void Tick(const S_tick_context& tc) {
   }
};

//required
C_class_to_be_ticked* GameGetStartupClass(){
   return new C_tick_demo_class;
}

static void RegisterGameInternals(){
   //register script methods, new actors, editor plugins and change any game settings in here
}

//----------------------------

int GameRun(const S_application_data& app_data, const char* cp_cmd_line) {

    try {
        SetupExceptionHandling();

        C_command_line& cmd_line = *app_data.cmd_line;
        //process command-line, exit if some errors encountered
        if (!cmd_line.Scan(cp_cmd_line, app_data.app_name))
            return 1;

        C_str reg_base = C_str(app_data.reg_base) + app_data.app_name;
        char language[64];
        *language = 0;

        if (cmd_line.run_config) {
            E_CONFIG_STATUS st = RunGameConfiguration(reg_base, true, language);
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

            RegisterGameInternals();
            
            AppRun(true);
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
