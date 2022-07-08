#ifndef __LOAD_3DS_H
#define __LOAD_3DS_H


//----------------------------

#define MAX_UV_CHANNELS    2  //number of uv channels we may load

//----------------------------

#pragma pack(push,1)
class C_rgb{
   byte color[3];
public:
   C_rgb(dword val = 0){
      color[0] = (byte)(val>>16);
      color[1] = (byte)(val>>8);
      color[2] = (byte)val;
   }
   C_rgb(byte r, byte g, byte b){
      color[0]=r;
      color[1]=g;
      color[2]=b;
   }
   byte operator[](int i){
      return color[i];
   }

   S_vector ToVector() const{
      return S_vector((float)color[2] * R_255, (float)color[1] * R_255, (float)color[0] * R_255);
   }
};
#pragma pack(pop)

//----------------------------

#define LOADF_TXT_TRANSP         1     //colorkeyed
#define LOADF_TXT_TRUECOLOR      2
#define LOADF_TXT_DIFF_ANIM      4
#define LOADF_TXT_REFL_ANIM      8
#define LOADF_TXT_EMBM_ANIM      0x10
#define LOADF_TXT_DIFFUSE_KEEP   0x20
#define LOADF_TXT_TRANSP_ALPHA   0x80  //colorkeyed with alpharef at zero
#define LOADF_TXT_CUBEMAP        0x100 //cube map

//----------------------------
                              //internal stock lists for object keeping
                              // between creation and hierarchy build
struct S_mat_info{
   C_smart_ptr<I3D_material> mat;
   enum{
      MAP_DIFFUSE,
      MAP_OPACITY,
      MAP_ENVIRONMENT,
      MAP_EMBM,
      MAP_PROCEDURAL,
      MAP_NORMAL,
      MAP_LAST
   };
   C_str map_filename[MAP_LAST];
   dword ct_flags;
   int diffuse_anim_speed;
   float environment_power, embm_power;
   bool txt_created;
   dword load_flags;
   int embm_anim_speed;
   S_mat_info():
      load_flags(0),
      txt_created(false)
   {}
   S_mat_info(PI3D_material m1, dword ct_flags1, const C_str &sd, const C_str &so,
      const C_str &ss, const C_str &sb):
      diffuse_anim_speed(0),
      mat(m1), ct_flags(ct_flags1)
   {
      map_filename[0] = sd;
      map_filename[1] = so;
      map_filename[2] = ss;
      map_filename[3] = sb;
   }
};

//----------------------------

class C_loader: public C_unknown{
                              //id-based array of frames
protected:
   C_vector<PI3D_frame> frame_id_list;

public:
   PI3D_driver driver;

   PI3D_scene scene;          //valid during scene loading
   PI3D_frame root_frame;     //valid during scene or model loading
   PI3D_container container;  //valid during scene or model loading
   PI3D_animation_set anim_set; //valid during animation loading

   C_chunk &ck;
   dword load_flags;
   dword file_length;
                              //loader callback function
   I3D_LOAD_CB_PROC *load_cb_proc;
   void *load_cb_context;

   C_str file_name;
   __int64 file_time_;

   C_str user_comments;       //read from the loaded file

   int tab;                   //tab for log
   typedef map<PI3D_frame, C_str> t_frm_properties;
   t_frm_properties frm_properties;

public:
                              //regions of joints
   struct S_region_data{
      S_matrix bbox_matrix;   //bounding box' matrix relative to this joint (i.e. local matrix)
      I3D_bbox bbox;          //bounding-box used for SM vertex assignment
      int index;
   };
   typedef map<PI3D_frame, C_vector<S_region_data> > t_regions_info;
   t_regions_info regions_info;

   /*
   struct S_skin_data{
      C_str joint_name;
      struct S_vertex_info{
         word vertex_index;
         float weight;
      };
      C_buffer<S_vertex_info> vertex_info;
   };
   typedef C_vector<S_skin_data> t_skin_data_list;
   typedef map<CPI3D_frame, t_skin_data_list> t_skin_data;
   t_skin_data skin_data;
   */

protected:

//----------------------------
// Add single region to list of regions associated with particular frame.
   void AddRegion(PI3D_frame frm, const I3D_bbox &bb, const S_matrix &tm, int index){

      t_regions_info::iterator it = regions_info.find(frm);
      if(it==regions_info.end())
         it = regions_info.insert(pair<PI3D_frame, C_vector<S_region_data> >(frm, C_vector<S_region_data>())).first;
      C_vector<S_region_data> &rdv = (*it).second;
      rdv.push_back(S_region_data());
      S_region_data &rd = rdv.back();
      rd.bbox_matrix = tm;
      rd.bbox = bb;
      rd.index = index;
   }

public:
   C_loader(PI3D_driver d, C_chunk &ck_in):
      driver(d),
      ck(ck_in),
      load_flags(0),
      file_length(0),
      file_time_(0),
      tab(0),
      scene(NULL),
      root_frame(NULL),
      container(NULL),
      anim_set(NULL),
      load_cb_proc(NULL), load_cb_context(NULL)
   {}

   void REPORT_ERR(const char *cp) const{
      if((load_flags&I3DLOAD_ERRORS) && load_cb_proc){
         load_cb_proc(CBM_ERROR,
            (dword)(const char*)C_fstr("%s (file '%s')", cp, (const char*)file_name),
            0, load_cb_context);
      }
   }

   void LOG(const char *cp){
      char buf[512];
      memset(buf, ' ', Min(256, tab*2));
      strcpy(&buf[tab*2], cp);
      load_cb_proc(CBM_LOG, (dword)buf, 0, load_cb_context);
   }

   inline PI3D_frame GetFrame(dword id) const{
      return id<frame_id_list.size() ? frame_id_list[id] : NULL;
   }

   bool AddFrameID(dword id, PI3D_frame frm){
      if(frame_id_list.size() < (id+1)) frame_id_list.resize(id+256, NULL);
      frame_id_list[id] = frm;
      return true;
   }

   struct S_auto_lod_data{
      bool use;
      float min_dist;
      float max_dist;
      int num_parts;
      int min_num_faces;
      bool preserve_edges;
      S_auto_lod_data():
         use(false),
         preserve_edges(false),
         min_dist(-1),
         max_dist(-1),
         num_parts(-1)
      {}
   };

public:
   virtual I3D_RESULT Open(const char* fname, dword flags, PI3D_LOAD_CB_PROC cb_proc,
      void *cb_context, PI3D_frame root, PI3D_scene scene, PI3D_animation_set, PI3D_container) = 0;
  
   //virtual void Close() = 0;
   
   virtual I3DENUMRET BuildSectors(PI3D_frame frm, PI3D_scene scene, PI3D_model model) = 0;
};

C_loader* CreateLoaderI3D(PI3D_driver d, C_chunk& ck);

//----------------------------
#endif
