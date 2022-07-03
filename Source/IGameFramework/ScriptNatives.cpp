//----------------------------
typedef const char* t_string;

//----------------------------
                              //script functions - implementation
static float __stdcall RandomF() {
   return S_float_random();
}

//----------------------------

static int __stdcall RandomI(int max) {
   if (!max)
      return 0;
   return S_int_random(max);
}

//----------------------------// debugging //----------------------------

static void __stdcall LogS(t_string cp) {

#ifdef EDITOR
   if (editor) {
      PC_editor_item_Log ei = (PC_editor_item_Log)editor->FindPlugin("Log");
      if (ei)
         ei->AddText(cp, 0);
      //script_man_imp.debug_logs.push_back(cp);
   }
#endif
}

static void __stdcall LogI(int i) {
   LogS(C_fstr("%i", i));
}

static void __stdcall LogF(float f) {
   LogS(C_fstr("%.3f", f));
}

//----------------------------// timer //----------------------------

static void __stdcall Pause(int time) {

   //if zero time, we've nothing to do
   if (!time)
      return;
   C_script_manager_imp::C_thread_context* tc = script_man_imp.CreateThreadContext(C_script_manager_imp::SUSPEND_PAUSE);
   tc->counter = time;
}

//----------------------------

static void __stdcall SendSignal(t_string frm_receiver, int signal_id) {

   script_man_imp.SaveSignalData(frm_receiver, signal_id);
}

//----------------------------

static void __stdcall SetObjectOn(bool on_off, t_string what) {

   PI3D_frame frm;
   if (*what) {
      frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(what);
      if (!frm) {
         SCRIPT_ERR("SetObjectOn", C_fstr("can't find frame: '%s'", what));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   frm->SetOn(on_off);
}

//----------------------------

static void __stdcall SetObjectsParent(t_string what, t_string on) {

   PI3D_frame frm;
   if (*what) {
      frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(what);
      if (!frm) {
         SCRIPT_ERR("SetObjectOn", C_fstr("can't find frame: %s", what));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   PI3D_frame f_on = script_man_imp.GetCurrMission()->GetScene()->FindFrame(on);
   frm->LinkTo(f_on);
}

//----------------------------

static bool __stdcall IsObjectOn(t_string what) {

   PI3D_frame frm;
   if (*what) {
      frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(what);
      if (!frm) {
         SCRIPT_ERR("IsObjectOn", C_fstr("can't find frame: %s", what));
         return false;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   return frm->IsOn();
}

//----------------------------

static void __stdcall PlaySoundAmbient(t_string filename, float volume) {

   script_man_imp.GetCurrMission()->PlayAmbientSound(filename, volume);
}

//----------------------------

static void __stdcall PlaySound(t_string filename, float min_dist, float max_dist, t_string where, float volume) {

   PI3D_frame frm;
   if (*where) {
      if (where[0] == '+')
         frm = script_man_imp.GetCurrFrame()->FindChildFrame(where + 1);
      else
         frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(where);
      if (!frm) {
         SCRIPT_ERR("PlaySound", C_fstr("can't find frame: %s", where));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   assert(filename);
   script_man_imp.GetCurrMission()->PlaySound(filename, min_dist, max_dist, frm, S_vector(0, 0, 0), volume);
}

//----------------------------

static void __stdcall FadeVolume(int time, float volume, t_string what) {

   PI3D_frame frm;
   if (*what) {
      if (what[0] == '+')
         frm = script_man_imp.GetCurrFrame()->FindChildFrame(what + 1);
      else
         frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(what);
      if (!frm) {
         SCRIPT_ERR("FadeVolume", C_fstr("can't find frame: %s", what));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   if (frm->GetType() != FRAME_SOUND) {
      SCRIPT_ERR("FadeVolume", "function requires frame of sound type");
      return;
   }
   if (!time) {
      I3DCAST_SOUND(frm)->SetVolume(volume);
   }
   else {
      C_script_manager_imp::C_thread_context* tc = script_man_imp.CreateThreadContext(C_script_manager_imp::SUSPEND_FADE_VOL);
      tc->counter = time;
      tc->data1.fade_vol.r_init_time = 1.0f / (float)time;
      tc->data1.fade_vol.dest_volume = volume;
      tc->data1.fade_vol.volume_delta = I3DCAST_SOUND(frm)->GetVolume() - volume;
   }
}

//----------------------------

static void __stdcall SetVolume(float volume, t_string what) {

   FadeVolume(0, volume, what);
}

//----------------------------

static void __stdcall MissionOver(E_MISSION_RESULT mr, t_string msg_id) {
   script_man_imp.GetCurrMission()->MissionOver(mr, *msg_id ? msg_id : NULL);
}

//----------------------------

static void __stdcall EnableActor(bool on_off, t_string actor_name) {

   if (*actor_name) {
      int num_done = 0;
      const C_vector<C_smart_ptr<C_actor> >& actors = script_man_imp.GetCurrMission()->GetActorsVector();
      for (int i = actors.size(); i--; ) {
         PC_actor act = (PC_actor)(CPC_actor)actors[i];
         if (act->GetName().Match(actor_name)) {
            act->Enable(on_off);
            ++num_done;
         }
      }
      if (!num_done) {
         SCRIPT_ERR("EnableActor", C_fstr("no actor matching to string: '%s'", actor_name));
      }
   }
   else {
      INIT_ACTOR("EnableActor");
      act->Enable(on_off);
   }
}

//----------------------------

static void __stdcall FlashObject(dword attack, dword stay, dword decay, float flash_value, t_string name) {

   PI3D_frame frm;
   if (*name) {
      frm = script_man_imp.GetCurrMission()->GetScene()->FindFrame(name);
      if (!frm) {
         SCRIPT_ERR("FlashObject", C_fstr("can't find frame: %s", name));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   if (frm->GetType() != FRAME_MODEL) {
      SCRIPT_ERR("FlashObject", "only models may be flashed");
      return;
   }
   C_script_manager_imp::C_thread_context* tc = script_man_imp.CreateThreadContext(C_script_manager_imp::SUSPEND_FLASH);
   tc->data1.flash.attack_count = attack;
   tc->data1.flash.stay_count = stay;
   tc->data1.flash.decay_count = decay;
   tc->counter = attack;
   tc->data1.flash.flash_value = flash_value;
   tc->data1.flash.mode = C_script_manager_imp::C_thread_context::U_data::S_flash::FLASH_ATTACK;
}

//----------------------------

static void __stdcall SetBrightness(float value, t_string name) {

   PC_game_mission gmis = script_man_imp.GetCurrMission();
   PI3D_frame frm;
   if (*name) {
      frm = gmis->GetScene()->FindFrame(name);
      if (!frm) {
         SCRIPT_ERR("SetBrightness", C_fstr("can't find frame: %s", name));
         return;
      }
   }
   else {
      frm = script_man_imp.GetCurrFrame();
   }
   if (frm->GetType() != FRAME_MODEL) {
      SCRIPT_ERR("SetBrightness", "only models may be changed");
      return;
   }
   SetModelBrightness(I3DCAST_MODEL(frm), value, I3D_VIS_BRIGHTNESS_NOAMBIENT);
}

//----------------------------

static void __stdcall SetAnimation(t_string filename, int stage, int blend_op, float scale, float blend, float speed) {

   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PC_game_mission gmis = script_man_imp.GetCurrMission();
   PI3D_animation_set as;
   I3D_RESULT ir = anim_cache.Create(filename, &as, gmis->GetScene());
   if (I3D_FAIL(ir)) {
      ErrReport(C_fstr("SetAnimation: failed to load anim '%s'", filename), editor);
      return;
   }
   if (frm->GetType() != FRAME_MODEL) {
      ErrReport("SetAnimation: can set anims only on models", editor);
      as->Release();
      return;
   }
   PI3D_model mod = I3DCAST_MODEL(frm);
   mod->SetAnimation(stage, as, blend_op | I3DANIMOP_LOOP, scale, blend, speed);
   as->Release();
}

static void __stdcall StopAnimation(int stage) {
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PC_game_mission gmis = script_man_imp.GetCurrMission();
   if (frm->GetType() != FRAME_MODEL) {
      ErrReport("SetAnimation: can stop anims only on models", editor);
      return;
   }
   PI3D_model mod = I3DCAST_MODEL(frm);
   mod->StopAnimation(stage);
}

//----------------------------

static int __stdcall NewSpriteImage(t_string prj_name1, t_string diff_name1 = NULL, t_string op_name1 = NULL) {
   C_str prj_name = C_fstr("tables\\%s", prj_name1);

   PC_game_mission gm = script_man_imp.GetCurrMission();
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PS_frame_info fi = GetFrameInfo(frm);
   assert(frm && fi);

   C_smart_ptr<C_sprite_image> img = CreateSpriteImage();
   auto ir = img->Init(gm->GetScene()->GetDriver(), prj_name, strlen(diff_name1) ? diff_name1 : NULL, strlen(op_name1) ? op_name1 : NULL, TEXTMAP_NOMIPMAP, true);
   switch (ir) {
   case SPRINIT_CANT_OPEN_PROJECT: ErrReport(C_fstr("NewSpriteImage: can't open sprite project '%s'", prj_name1), editor); return -1;
   case SPRINIT_CANT_OPEN_DIFFUSE: ErrReport(C_fstr("NewSpriteImage: can't open diffuse texture '%s'", diff_name1), editor); return -1;
   case SPRINIT_CANT_OPEN_OPACITY: ErrReport(C_fstr("NewSpriteImage: can't open opacity texture '%s'", op_name1), editor); return -1;
   case SPRINIT_CANT_CREATE_TEXTURE: ErrReport("NewSpriteImage: can't create textures", editor); return -1;
   default: break;
   }

   fi->img.push_back(img);
   img->Release();
   return fi->img.size() - 1;
}

static int __stdcall NewSprite(dword img_idx, dword rect_idx, float x, float y, int z, bool set_on) {
   PC_game_mission gm = script_man_imp.GetCurrMission();
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PS_frame_info fi = GetFrameInfo(frm);
   assert(frm && fi);
   assert(img_idx < fi->img.size());

   PC_sprite img = CreateSprite(x, y, 1.0f, (CPC_sprite_image)fi->img[img_idx], rect_idx, z, 1.0f);
   img->SetOn(set_on);
   fi->sprites->AddSprite(img);
   img->Release();
   return fi->sprites->NumSprites() - 1;
}

static PC_sprite GetSprite(dword spr_idx) {
   PC_game_mission gm = script_man_imp.GetCurrMission();
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PS_frame_info fi = GetFrameInfo(frm);
   assert(frm && fi);
   if (spr_idx >= fi->sprites->NumSprites()) {
      ErrReport(C_fstr("GetSprite: cannot find sprite by ID: '%d'", spr_idx), editor);
      return NULL;
   }

   auto sprites = fi->sprites->GetSprites();

   return (PC_sprite)(sprites[spr_idx]);
}

static void __stdcall SpriteSetOn(dword spr_idx, bool on) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetOn(on);
   }
}

static bool __stdcall SpriteIsOn(dword spr_idx) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      return spr->IsOn();
   }
   return false;
}

static void __stdcall SpriteSetPos(dword spr_idx, float x, float y) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetPos(S_vector2(x, y));
   }
}

static void __stdcall SpriteSetScreenPos(dword spr_idx, float x, float y) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetScreenPos(S_vector2(x, y));
   }
}

static void __stdcall SpriteSetScale(dword spr_idx, float sx, float sy) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetScale(S_vector2(sx, sy));
   }
}

static void __stdcall SpriteSetColor(dword spr_idx, dword color) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetColor(color);
   }
}

static void __stdcall SpriteRotate(dword spr_idx, float angle, float px, float py) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->Rotate(angle, S_vector2(px, py));
   }
}

static void __stdcall SpriteShear(dword spr_idx, float angle, float y) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->Shear(angle, y);
   }
}

static void __stdcall SpriteSetUV(dword spr_idx, float l, float t, float r, float b) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetUV(S_vector2(l, t), S_vector2(r, b));
   }
}

static void __stdcall SpriteSetCoords(dword spr_idx, float l, float t, float r, float b) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetCoords(S_vector2(l, t), S_vector2(r, b));
   }
}

static void __stdcall SpriteSetSize(dword spr_idx, float r, float b) {
   auto spr = GetSprite(spr_idx);
   if (spr) {
      spr->SetSize(S_vector2(r, b));
   }
}

static dword __stdcall ScreenSx() {
   auto igraph = script_man_imp.GetCurrMission()->GetScene()->GetDriver()->GetGraphInterface();
   return igraph->Scrn_sx();
}
static dword __stdcall ScreenSy() {
   auto igraph = script_man_imp.GetCurrMission()->GetScene()->GetDriver()->GetGraphInterface();
   return igraph->Scrn_sy();
}
static float __stdcall ScreenAspect() {
   auto igraph = script_man_imp.GetCurrMission()->GetScene()->GetDriver()->GetGraphInterface();
   return igraph->Scrn_sx() / (float)igraph->Scrn_sy();
}

static dword __stdcall ScreenAspectMode() {
   auto aspect = ScreenAspect();
   aspect *= 100;
   aspect = (int)aspect;
   aspect /= 100;

   if (aspect == 1.60f)
      return 5; /* 16:10 */
   if (aspect == 1.25f)
      return 4; /* 5:4 */
   if (aspect == 3.77f)
      return 3; /* 34:9 */
   if (aspect >= 2.33f && aspect <= 2.38f)
      return 2; /* 21:9 */
   if (aspect == 1.77f)
      return 1; /* 16:9 */

   return 0; /* 4:3 */
}

//----------------------------
//Array support

static inline C_DataVector* GetArrayPointer(dword idx) {
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PS_frame_info fi = GetFrameInfo(frm);
   assert(frm && fi);

   if (idx < 0 || idx >= fi->arrays.size()) {
      ErrReport(C_fstr("GetArrayPointer: cannot find array by ID: '%d'", idx), editor);
      return NULL;
   }

   return &fi->arrays[idx];
}

dword __stdcall CreateArray(int reserved_size = -1) {
   PI3D_frame frm = script_man_imp.GetCurrFrame();
   PS_frame_info fi = GetFrameInfo(frm);
   assert(frm && fi);
   fi->arrays.push_back(C_DataVector());

   if (reserved_size != -1) {
      fi->arrays.back().reserve(reserved_size);
   }
   return fi->arrays.size() - 1;
}
void __stdcall ArrayClear(dword arr) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data) {
      data->clear();
   }
}
int __stdcall ArrayPush(dword arr, dword val) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data) {
      data->push_back(val);
      return data->size() - 1;
   }
   return -1;
}
dword __stdcall ArrayPop(dword arr) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data && data->size() > 0) {
      dword val = data->back();
      data->pop_back();
      return val;
   }
   else {
      ErrReport("ArrayPop: Array is empty!", editor);
      return 0xcececece;
   }
}
int __stdcall ArrayLen(dword arr) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data) {
      return data->size();
   }
   return 0;
}
void __stdcall ArraySet(dword arr, int idx, dword val) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data && data->size() > 0) {
      if (idx < 0 || idx >= data->size()) {
         ErrReport(C_fstr("ArraySet: index out of range: '%d' !", idx), editor);
      }
      else {
         (*data)[idx] = val;
      }
   }
   else {
      ErrReport("ArraySet: Array is empty!", editor);
   }
}
dword __stdcall ArrayGet(dword arr, int idx) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data && data->size() > 0) {
      if (idx < 0 || idx >= data->size()) {
         ErrReport(C_fstr("ArrayGet: index out of range: '%d' !", idx), editor);
      }
      else {
         return (*data)[idx];
      }
   }
   else {
      ErrReport("ArrayGet: Array is empty!", editor);
   }

   return 0xcececece;
}
void __stdcall ArrayDelete(dword arr, int idx) {
   C_DataVector* data = GetArrayPointer(arr);
   if (data && data->size() > 0) {
      if (idx < 0 || idx >= data->size()) {
         ErrReport(C_fstr("ArrayDelete: index out of range: '%d' !", idx), editor);
      }
      else {
         data->remove_index(idx);
      }
   }
   else {
      ErrReport("ArrayDelete: Array is empty!", editor);
   }
}


//----------------------------

//todo: 
VM_LOAD_SYMBOL script_symbols[512] = { NULL };

#define DEFINE_SYMBOL(name) {&name, #name}

static void RegisterNativeSymbols() {
   RegisterSymbol(DEFINE_SYMBOL(LogS));
   RegisterSymbol(DEFINE_SYMBOL(LogI));
   RegisterSymbol(DEFINE_SYMBOL(LogF));
   RegisterSymbol(DEFINE_SYMBOL(Pause));

   RegisterSymbol(DEFINE_SYMBOL(RandomF));
   RegisterSymbol(DEFINE_SYMBOL(RandomI));
   RegisterSymbol(DEFINE_SYMBOL(PlaySoundAmbient));
   RegisterSymbol(DEFINE_SYMBOL(PlaySound));
   RegisterSymbol(DEFINE_SYMBOL(FadeVolume));
   RegisterSymbol(DEFINE_SYMBOL(SetVolume));
   RegisterSymbol(DEFINE_SYMBOL(SetObjectOn));
   RegisterSymbol(DEFINE_SYMBOL(SetObjectsParent));
   RegisterSymbol(DEFINE_SYMBOL(IsObjectOn));
   RegisterSymbol(DEFINE_SYMBOL(SendSignal));
   RegisterSymbol(DEFINE_SYMBOL(SetAnimation));
   RegisterSymbol(DEFINE_SYMBOL(StopAnimation));

   RegisterSymbol({ &NewSpriteImage, "CreateSpriteImage" }),
   RegisterSymbol({ &NewSprite, "CreateSprite" }),
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetOn));
   RegisterSymbol(DEFINE_SYMBOL(SpriteIsOn));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetPos));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetScreenPos));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetScale));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetColor));
   RegisterSymbol(DEFINE_SYMBOL(SpriteRotate));
   RegisterSymbol(DEFINE_SYMBOL(SpriteShear));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetUV));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetCoords));
   RegisterSymbol(DEFINE_SYMBOL(SpriteSetSize));

   RegisterSymbol(DEFINE_SYMBOL(ScreenSx));
   RegisterSymbol(DEFINE_SYMBOL(ScreenSy));
   RegisterSymbol(DEFINE_SYMBOL(ScreenAspect));
   RegisterSymbol(DEFINE_SYMBOL(ScreenAspectMode));

   RegisterSymbol(DEFINE_SYMBOL(MissionOver));

   RegisterSymbol(DEFINE_SYMBOL(FlashObject));
   RegisterSymbol(DEFINE_SYMBOL(SetBrightness));

   RegisterSymbol(DEFINE_SYMBOL(EnableActor));

   RegisterSymbol(DEFINE_SYMBOL(CreateArray));
   RegisterSymbol(DEFINE_SYMBOL(ArrayClear));
   RegisterSymbol(DEFINE_SYMBOL(ArrayPush));
   RegisterSymbol(DEFINE_SYMBOL(ArrayPop));
   RegisterSymbol(DEFINE_SYMBOL(ArrayLen));
   RegisterSymbol(DEFINE_SYMBOL(ArraySet));
   RegisterSymbol(DEFINE_SYMBOL(ArrayGet));
   RegisterSymbol(DEFINE_SYMBOL(ArrayDelete));

   GameRegisterSymbols();
}
