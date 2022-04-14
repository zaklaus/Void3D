#ifndef _4DS_
#define _4DS_

#include <I3D\I3D_math.h>

namespace DataFormat4DS {

    typedef enum {
        MATERIALFLAG_TEXTUREDIFFUSE = 0x00040000,  // whether diffuse texture is present
        MATERIALFLAG_COLORED = 0x08000000,         // whether to use diffuse color (only applies with diffuse texture)
        MATERIALFLAG_MIPMAPPING = 0x00800000,
        MATERIALFLAG_ANIMATEDTEXTUREDIFFUSE = 0x04000000,
        MATERIALFLAG_ANIMATEXTEXTUREALPHA = 0x02000000,
        MATERIALFLAG_DOUBLESIDEDMATERIAL = 0x10000000,   // whether backface culling should be off
        MATERIALFLAG_ENVIRONMENTMAP = 0x00080000,        // simulates glossy material with environment texture
        MATERIALFLAG_NORMALTEXTUREBLEND = 0x00000100,    // blend between diffuse and environment texture normally
        MATERIALFLAG_MULTIPLYTEXTUREBLEND = 0x00000200,  // blend between diffuse and environment texture by multiplying
        MATERIALFLAG_ADDITIVETEXTUREBLEND = 0x00000400,  // blend between diffuse and environment texture by addition
        MATERIALFLAG_CALCREFLECTTEXTUREY = 0x00001000,
        MATERIALFLAG_PROJECTREFLECTTEXTUREY = 0x00002000,
        MATERIALFLAG_PROJECTREFLECTTEXTUREZ = 0x00004000,
        MATERIALFLAG_ADDITIONALEFFECT = 0x00008000,  // should be ALPHATEXTURE | COLORKEY | ADDITIVEMIXING
        MATERIALFLAG_ALPHATEXTURE = 0x40000000,
        MATERIALFLAG_COLORKEY = 0x20000000,
        MATERIALFLAG_ADDITIVEMIXING = 0x80000000  // the object is blended against the world by
                                                  // adding RGB (see street lamps etc.)
    } MaterialFlag;

    typedef enum {
        MESHTYPE_STANDARD = 0x01,   // visual mesh
        MESHTYPE_COLLISION = 0x02,  // NOTE(zaklaus): Imaginary type based on mesh name "wcol*"
        MESHTYPE_SECTOR = 0x05,     // part of space, used for culling, effective lighting etc.
        MESHTYPE_DUMMY = 0x06,      // invisible bounding box
        MESHTYPE_TARGET = 0x07,     // used in human models (as a shooting target?)
        MESHTYPE_JOINT = 0x0a        // for skeletal animation
    } MeshType;

    typedef enum {
        VISUALMESHTYPE_STANDARD = 0x0,      // normal mesh
        VISUALMESHTYPE_SINGLEMESH = 0x02,   // mesh with bones
        VISUALMESHTYPE_SINGLEMORPH = 0x03,  // combination of morph (for face) and skeletal (for body) animation
        VISUALMESHTYPE_BILLBOARD = 0x04,    // billboarding mesh (rotates towards camera
        VISUALMESHTYPE_MORPH = 0x05,        // mesh with morphing (non-skeletal) animation, e.g. curtains in wind
        VISUALMESHTYPE_GLOW = 0x06,         // has no geometry, only shows glow texture
        VISUALMESHTYPE_MIRROR = 0x08        // reflects the scene
    } VisualMeshType;                       // subtype of mesh, when MeshType == MESHTYPE_STANDARD

    typedef enum {
        MESHRENDERFLAG_USEDEPTHBIAS = 0x0001,  // whether to receive shadows
        MESHRENDERFLAG_USESHADOWS = 0x0002,
        MESHRENDERFLAG_UNKNOWN = 0x0008,        // always 1
        MESHRENDERFLAG_USEPROJECTION = 0x0020,  // used for projecting textures, such as blood
        MESHRENDERFLAG_FORBIDFOG = 0x0080
    } MeshRenderFlag;

    typedef enum {
        MESHOCCLUDINGFLAG_NORMAL = 0x09,
        MESHOCCLUDINGFLAG_SECTOR = 0x7D,
        MESHOCCLUDINGFLAG_WALL = 0x3D,    // mesh in sector (walls)
        MESHOCCLUDINGFLAG_PORTAL = 0x1D,  // mesh in portal
        MESHOCCLUDINGFLAG_INACTIVE = 0x11
    } MeshOccludingFlag;

    typedef struct {
        dword mFlags;
        S_vector mAmbient;
        S_vector mDiffuse;   // only used if there is no diffuse texture, or if COLORED flag is set
        S_vector mEmission;  // always used
        float mTransparency;  // 0.0 - invisible; 1.0 - opaque

        // environment map
        float mEnvRatio;  // parameter for interpolating between env. and diffuse map (only for
                          // NORMAL blending flag)
        char mEnvMapNameLength;
        char mEnvMapName[255];

        char mDiffuseMapNameLength;
        char mDiffuseMapName[255];

        // alpha map
        char mAlphaMapNameLength;
        char mAlphaMapName[255];

        // anim map
        dword mAnimSequenceLength;  // how many frames animated texture has
        word mUnk0;
        dword mFramePeriod;
        dword mUnk1;
        dword mUnk2;
    } Material;

    typedef struct {
        S_vector mPos;
        S_vector mNormal;
        S_vector2 mUV;
    } Vertex;

    typedef struct {
        word mA;
        word mB;
        word mC;
    } Face;

    typedef struct {
        word mFaceCount;
        C_vector<Face> mFaces;
        word mMaterialID;  // 1-based, 0 = default material
    } FaceGroup;

    typedef struct {
        float mRelativeDistance;
        word mVertexCount;
        C_vector<Vertex> mVertices;
        byte mFaceGroupCount;
        C_vector<FaceGroup> mFaceGroups;
    } Lod;

    typedef struct {
        word mInstanced;
        // for non-instanced mesh
        byte mLODLevel;
        C_vector<Lod> mLODs;
    } Standard;

    typedef struct {
        word mUnk0;
        byte mTargetCount;
        C_vector<word> mTargets;
    } Target;

    typedef struct {
        float mTransform[16];
        dword mJointID;
    } Joint;

    typedef struct {
        byte mVertexCount;
        dword mUnk0;  // always 4.
        dword mUnk1[6];
        C_vector<S_vector> mVertices;
    } Portal;

    typedef struct {
        dword mUnk0;  // always 2049.
        dword mUnk1;  // always 0.
        dword mVertexCount;
        dword mFaceCount;
        C_vector<S_vector> mVertices;
        C_vector<Face> mFaces;
        S_vector mMinBox;
        S_vector mMaxBox;
        byte mPortalCount;
        C_vector<Portal> mPortals;
    } Sector;

    typedef struct {
        Standard mStandard;
        dword mRotationAxis;  // 0 - X, 1 - Y, 2 - Z
        byte mIgnoreCamera;   // 0 - rotate around center point, 1 - rotate around mRotationAxis
    } Billboard;

    typedef struct {
        // bounding box
        S_vector mMinBox;
        S_vector mMaxBox;
    } Dummy;

    typedef struct {
        float mPosition;
        word mMaterialID;
    } GlowData;

    typedef struct {
        byte mGlowCount;
        C_vector<GlowData> mGlowData;
    } Glow;

    typedef struct {
        S_vector mMinBox;
        S_vector mMaxBox;
        S_vector mCenter;
        float mRadius;
        S_matrix mReflectionMatrix;
        S_vector mBackgroundColor;
        float mViewDistance;
        dword mVertexCount;
        dword mFaceCount;
        C_vector<S_vector> mVertices;
        C_vector<Face> mFaces;
    } Mirror;

    typedef struct {
        S_vector mPosition;
        S_vector mNormals;
    } MorphLodVertex;

    typedef struct {
        word mVertexCount;
        C_vector<MorphLodVertex> mVertices;
        byte mUnk0;
        C_vector<word> mVertexLinks;  // addresses vertices from Standard's LOD mesh
    } MorphLod;

    typedef struct {
        Standard mStandard;
        byte mFrameCount;
        byte mLODLevel;  // should be equal to Standard.LODLevel
        byte mUnk0;
        C_vector<MorphLod> mLODs;
        S_vector mMinBox;
        S_vector mMaxBox;
        float mUnk1[4];
    } Morph;

    typedef struct {
        float mInverseTransform[16];
        dword mNoneWeightedVertCount;  // amount of vertices that should have a weight of 1.0f
        dword mWeightedVertCount;           // amount of vertices whose weights are stored in mWeights
        dword mBoneID;                // this is likely a reference to a paired bone, which takes the remainder
                                         // (1.0f - w) of weight
        S_vector mMinBox;
        S_vector mMaxBox;
        C_vector<float> mWeights;
    } SingleMeshLodBone;

    typedef struct {
        byte mBoneCount;
        dword mNonWeightedVertCount;
        S_vector mMinBox;
        S_vector mMaxBox;
        C_vector<SingleMeshLodBone> mBones;
    } SingleMeshLod;

    typedef struct {
        Standard mStandard;
        C_vector<SingleMeshLod> mLODs;  // LODLevel == Standard.LODLevel.
    } SingleMesh;

    typedef struct {
        SingleMesh mSingleMesh;
        Morph mMorph;  // Morph without Standard Mesh!
    } SingleMorph;

    typedef struct {
        byte mMeshType;
        // standard mesh type
        byte mVisualMeshType;
        word mMeshRenderFlags;
        word mParentID;  // 0 - not connected
        S_vector mPos;
        S_vector mScale;
        S_quat mRot;
        byte mCullingFlags;
        byte mMeshNameLength;
        char mMeshName[255];
        byte mMeshParamsLength;
        char mMeshParams[255];
        Standard mStandard;
        Dummy mDummy;
        Mirror mMirror;
        Glow mGlow;
        Billboard mBillboard;
        Sector mSector;
        Target mTarget;
        Joint mJoint;
        Morph mMorph;
        SingleMesh mSingleMesh;
        SingleMorph mSingleMorph;
    } Mesh;

    typedef struct sModel {
        byte mSignature[4];
        word mFormatVersion;  // PC : 0x1D (29)
        __int64 mTimestamp;
        word mMaterialCount;
        C_vector<Material> mMaterials;
        word mMeshCount;
        C_vector<Mesh> mMeshes;
        byte mUse5DS;
    } Model;
}

#endif