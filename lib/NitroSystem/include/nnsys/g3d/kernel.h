/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     kernel.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.36 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_KERNEL_H_
#define NNSG3D_KERNEL_H_

#include <nnsys/g3d/config.h>
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/binres/res_struct_accessor_anm.h>

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
//
// �\���̒�`�y��typedef
//

#define NNS_G3D_SIZE_MAT_MAX_MAX 256
#define NNS_G3D_SIZE_JNT_MAX_MAX 256
#define NNS_G3D_SIZE_SHP_MAX_MAX 256

#if (NNS_G3D_SIZE_MAT_MAX <= 0 || NNS_G3D_SIZE_MAT_MAX > NNS_G3D_SIZE_MAT_MAX_MAX)
#error NNS_G3D_SIZE_MAT_MAX range error.
#endif

#if (NNS_G3D_SIZE_JNT_MAX <= 0 || NNS_G3D_SIZE_JNT_MAX > NNS_G3D_SIZE_JNT_MAX_MAX)
#error NNS_G3D_SIZE_JNT_MAX range error.
#endif

#if (NNS_G3D_SIZE_SHP_MAX <= 0 || NNS_G3D_SIZE_SHP_MAX > NNS_G3D_SIZE_SHP_MAX_MAX)
#error NNS_G3D_SIZE_SHP_MAX range error.
#endif

#if (NNS_G3D_SIZE_MAT_MAX % 32 != 0)
#error NNS_G3D_SIZE_MAT_MAX must be a multiple of 32.
#endif

#if (NNS_G3D_SIZE_JNT_MAX % 32 != 0)
#error NNS_G3D_SIZE_JNT_MAX must be a multiple of 32.
#endif

#if (NNS_G3D_SIZE_SHP_MAX % 32 != 0)
#error NNS_G3D_SIZE_SHP_MAX must be a multiple of 32.
#endif

typedef u32 NNSG3dTexKey;    // compatible with NNSGfdTexKey
typedef u32 NNSG3dPlttKey;   // compatible with NNSGfdPlttKey


/*---------------------------------------------------------------------------*
    NNSG3dAnmObj

    NNSG3dRenderObj�ɎQ�Ƃ����\���́B�������̊m�ہE����̓��[�U�[������s��
    �K�v������܂��B��������NNS_G3dAnmObjInit�֐��ōs���܂��B
    ���̍\���̖̂����́A
    �E�A�j���[�V�������\�[�X�Ƃ������������֐��̃y�A���w�肵�A
    �E�A�j���[�V�������\�[�X�ƃ��f�����\�[�X�̊֘A�t�����s���A
    �E�A�j���[�V�����̌��݂̃t���[����ێ����邱�Ƃł��B

    frame:      �ǂ̃t���[�����Đ����邩�w�肷��B
    ratio:      �A�j���[�V�����u�����h�֐����g�p����B
    resAnm:     �X�̃A�j���[�V�������\�[�X�ւ̃|�C���^
    funcAnm:    resAnm��frame�Ԗڂ̃t���[�����Đ�����֐��ւ̃|�C���^
    next:       NNS_G3dRenderObjBindAnmObj, NNS_G3dRenderObjReleaseAnmObj��
                �ݒ肳���B
    resTex:     �e�N�X�`�����\�[�X�ւ̃|�C���^�B�A�j���[�V�������K�v�Ƃ���ꍇ�̂�
                �ݒ肳���B
    priority:   NNSG3dRenderObj�ɓo�^�����ۂ̗D�揇��
    numMapData: ���f�����\�[�X�ƃA�j�����\�[�X�̃C���f�b�N�X�̑Ή����Ƃ�e�[�u��
                �̃G���g���̐�
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dAnmObj_
{
    fx32                  frame;
    fx32                  ratio;
    void*                 resAnm;     // ���\�[�X�t�@�C�����A�j���[�V�����f�[�^�u���b�N�ւ̃|�C���^
    void*                 funcAnm;    // �e�A�j���[�V�����̊֐��|�C���^�ɃL���X�g�����B�f�t�H���g�����邪�ύX�ł���
    struct NNSG3dAnmObj_* next;
    const NNSG3dResTex*   resTex;     // �e�N�X�`���u���b�N�̏�񂪕K�v�ȏꍇ(�e�N�X�`���p�^�[���A�j���[�V�����̂�)
    u8                    priority;
    u8                    numMapData;
    u16                   mapData[1]; // numMapData�̔z��ɂȂ�(NNSG3dAnmObjMapData)
}
NNSG3dAnmObj;

//
// NNSG3dAnmObj�ɕK�v�ȃ������̃T�C�Y�̓��f�����\�[�X�ƃA�j���[�V�����̃J�e�S�����猈�肳���B
// �}�e���A���A�j���[�V�����̏ꍇ�́A�ȉ��̃}�N�����g�p����B
// �{����sizeof(NNSG3dAnmObj) + sizeof(u16) * (pMdl->info.numMat - 1)
// ����4�o�C�g���E�ɂ���
//
#define NNS_G3D_ANMOBJ_SIZE_MATANM(pMdl) ((sizeof(NNSG3dAnmObj) + sizeof(u16) * pMdl->info.numMat) & ~3)

//
// NNSG3dAnmObj�ɕK�v�ȃ������̃T�C�Y�̓��f�����\�[�X�ƃA�j���[�V�����̃J�e�S�����猈�肳���B
// �W���C���g�A�j���[�V�����ƃr�W�r���e�B�A�j���[�V�����̏ꍇ�́A�ȉ��̃}�N�����g�p����B
// �{����sizeof(NNSG3dAnmObj) + sizeof(u16) * (pMdl->info.numNode - 1)
// ����4�o�C�g���E�ɂ���
//
#define NNS_G3D_ANMOBJ_SIZE_JNTANM(pMdl) ((sizeof(NNSG3dAnmObj) + sizeof(u16) * pMdl->info.numNode) & ~3)
#define NNS_G3D_ANMOBJ_SIZE_VISANM(pMdl) ((sizeof(NNSG3dAnmObj) + sizeof(u16) * pMdl->info.numNode) & ~3)

/*---------------------------------------------------------------------------*
    NNSG3dAnmObjMapData

    NNSG3dAnmObj��mapData�z��Ɋi�[�����f�[�^�̂��߂̃��[�e�B���e�B�I��
    �񋓌^�B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_ANMOBJ_MAPDATA_EXIST     = 0x0100,
    NNS_G3D_ANMOBJ_MAPDATA_DISABLED  = 0x0200,
    NNS_G3D_ANMOBJ_MAPDATA_DATAFIELD = 0x00ff
}
NNSG3dAnmObjMapData;


/*---------------------------------------------------------------------------*
    �A�j���[�V�����u�����h�֐���typedef
 *---------------------------------------------------------------------------*/
// anm.h�Œ�`
struct NNSG3dMatAnmResult_;
struct NNSG3dJntAnmResult_;
struct NNSG3dVisAnmResult_;


// �}�e���A���A�j���[�V�����u�����h�֐�
typedef BOOL (*NNSG3dFuncAnmBlendMat)(struct NNSG3dMatAnmResult_*,
                                      const NNSG3dAnmObj*,
                                      u32);

// �W���C���g�A�j���[�V�����u�����h�֐�
typedef BOOL (*NNSG3dFuncAnmBlendJnt)(struct NNSG3dJntAnmResult_*,
                                      const NNSG3dAnmObj*,
                                      u32);

// �r�W�r���e�B�A�j���[�V�����u�����h�֐�
typedef BOOL (*NNSG3dFuncAnmBlendVis)(struct NNSG3dVisAnmResult_*,
                                      const NNSG3dAnmObj*,
                                      u32);


/*---------------------------------------------------------------------------*
    NNSG3dRenderObjFlag

    NNSG3dRenderObj�����t���O�ŁANNS_G3dDraw�̓�����J�X�^�}�C�Y�ł���B
    
    NNS_G3D_RENDEROBJ_FLAG_RECORD
        NNS_G3dDraw�̎��s���ɁArecJntAnm, recMatAnm�ɃW���C���g��}�e���A����
        �v�Z���ʂ��X�g�A���܂��B���s�I�����ɂ��̃t���O�̓��Z�b�g����܂��B
        ���̃t���O�����Z�b�g����Ă��āArecJntAnm, recMatAnm��NULL�Ŗ����ꍇ��
        recJntAnm, recMatAnm���̌v�Z���ʂ����̂܂܎g�p����܂��B
    NNS_G3D_RENDEROBJ_FLAG_NOGECMD
        NNS_G3dDraw�̎��s���ɁA�W�I���g���R�}���h�𑗐M���܂���B
    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_DRAW
        NNS_G3dDraw�̎��s���ɁASBC�̕`��n�R�}���h�̎��s���X�L�b�v���܂��B
    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_MTXCALC
        NNS_G3dDraw�̎��s���ɁASBC�̍s��v�Z�n�R�}���h�̎��s���X�L�b�v���܂��B

    NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE
        G3D�����ŃZ�b�g�^���Z�b�g�����t���O��hintXXXAnmExist���������Ȃ����
        �̏ꍇ�Z�b�g����܂�(NNS_G3dRenderObjRemoveAnmObj���ꂽ�Ƃ��Z�b�g)

    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_DRAW��NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_MTXCALC��
    g3dcvtr��-s�I�v�V���������ăR���o�[�g�������f���ɑ΂��ėL���ł��B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_RENDEROBJ_FLAG_RECORD             = 0x00000001,
    NNS_G3D_RENDEROBJ_FLAG_NOGECMD            = 0x00000002,
    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_DRAW      = 0x00000004,
    NNS_G3D_RENDEROBJ_FLAG_SKIP_SBC_MTXCALC   = 0x00000008,
    NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE      = 0x00000010
}
NNSG3dRenderObjFlag;


/*---------------------------------------------------------------------------*
    NNSG3dSbcCallBackFunc

    NNSG3dRS���Ɋi�[�����R�[���o�b�N�֐��ւ̃|�C���^
 *---------------------------------------------------------------------------*/
struct NNSG3dRS_;
typedef void (*NNSG3dSbcCallBackFunc)(struct NNSG3dRS_*);


/*---------------------------------------------------------------------------*
    NNSG3dSbcCallBackTiming

    SBC���ߓ��ŋN������R�[���o�b�N�̃^�C�~���O��3��ނ���w��ł���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_SBC_CALLBACK_TIMING_NONE = 0x00,
    NNS_G3D_SBC_CALLBACK_TIMING_A    = 0x01,
    NNS_G3D_SBC_CALLBACK_TIMING_B    = 0x02,
    NNS_G3D_SBC_CALLBACK_TIMING_C    = 0x03
}
NNSG3dSbcCallBackTiming;

#define NNS_G3D_SBC_CALLBACK_TIMING_ASSERT(x)               \
    NNS_G3D_ASSERT(x == NNS_G3D_SBC_CALLBACK_TIMING_NONE || \
                   x == NNS_G3D_SBC_CALLBACK_TIMING_A    || \
                   x == NNS_G3D_SBC_CALLBACK_TIMING_B    || \
                   x == NNS_G3D_SBC_CALLBACK_TIMING_C)

/*---------------------------------------------------------------------------*
    NNSG3dRenderObj

    �\���̂̃����o���͏�ɕω�����\��������
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dRenderObj_
{
    u32 flag; // NNSG3dRenderObjFlag

    // NOTICE:
    // NNS_G3dDraw���ł�NNSG3dResMdl�̒��������������邱�Ƃ͂���܂���B
    // (�R�[���o�b�N���g�����ꍇ���͏���)
    NNSG3dResMdl*         resMdl;
    NNSG3dAnmObj*         anmMat;
    NNSG3dFuncAnmBlendMat funcBlendMat;
    NNSG3dAnmObj*         anmJnt;
    NNSG3dFuncAnmBlendJnt funcBlendJnt;
    NNSG3dAnmObj*         anmVis;
    NNSG3dFuncAnmBlendVis funcBlendVis;

    // �R�[���o�b�N�p���
    NNSG3dSbcCallBackFunc cbFunc;              // NULL�Ȃ�΃R�[���o�b�N�͂Ȃ��B
    u8                    cbCmd;               // ��~�ʒu���R�}���h�Ŏw��BNNS_G3D_SBC_*****(res_struct.h�Œ�`)
    u8                    cbTiming;            // NNSG3dSbcCallBackTiming(sbc.h�Œ�`)
    u16                   dummy_;

    // �����_�����O�J�n���O�ɌĂ΂��B��ʓI��NNS_G3dRS��
    // �R�[���o�b�N�x�N�^��ݒ肷�邽�߂Ɏg����B
    NNSG3dSbcCallBackFunc cbInitFunc;

    // ���[�U�[���Ǘ�����̈�ւ̃|�C���^
    // ���炩���߃|�C���^��ݒ肵�Ă����΃R�[���o�b�N�ł̎g�p���\.
    void*                 ptrUser;

    //
    // �f�t�H���g�ł�resMdl���Ɋi�[����Ă���SBC���g�p���邪�A
    // ptrUserSbc�Ƀ|�C���^���i�[���邱�Ƃɂ���āA���[�U�[��`��SBC��
    // �g�p�����悤�ɂȂ�B
    //
    // Example:
    // �p�[�e�B�N���V�X�e�����ŁA�P���ȃ��f��(1�}�e���A��1�V�F�C�v)�����낢���
    // �ꏊ�ɏo�������悤�ȏꍇ�A���̂悤��SBC�R�[�h���쐬���ăR�[�h�ւ̃|�C���^��
    // ptrUserSbc�ɑ�����Ă����ƌ������悢�B
    // 
    // MAT[000] 0
    // MTX 0
    // SHP 0
    // MTX 1
    // SHP 0
    // ....
    // MTX n
    // SHP 0
    // RET
    //
    // ���炩���ߍs��͍s��X�^�b�N�ɃZ�b�g���Ă����K�v������B
    // �܂��A�}�e���A�����V�F�C�v���ɏ������ς������ꍇ�́A
    // �R�[���o�b�N�ŕύX����̂��悢���낤�B
    //
    u8*                   ptrUserSbc;

    //
    // �v�Z���ʂ̃o�b�t�@�����O�̈�ւ̃|�C���^
    // �W���C���g�E�}�e���A���̌v�Z���ʂ𕡐��t���[���^�����̂̃��f��
    // �Ŏg���܂킵�����ꍇ�́ArecJntAnm/recMatAnm�Ƀo�b�t�@���Z�b�g����B
    //
    // flag��NNS_G3D_RENDEROBJ_FLAG_RECORD��ON�̂Ƃ��A
    // recJntAnm, recMatAnm��NULL�łȂ����̂ɂ��āA
    // ���ꂼ��}�e���A���E�W���C���g�̌v�Z���ʂ��L�^����Ă����B
    //
    // flag��NNS_G3D_RENDEROBJ_FLAG_RECORD��OFF�̂Ƃ��A
    // recJntAnm, recMatAnm��NULL�łȂ����̂ɂ��āA
    // ���ꂼ��}�e���A���E�W���C���g�̌v�Z���ʂƂ��ė��p���Ă����B
    //
    // ���[�U�[��
    // recJntAnm�ɂ��ẮA
    // sizeof(NNSG3dJntAnmResult) * resMdl->info.numNode�o�C�g
    // recMatAnm�ɂ��ẮA
    // sizeof(NNSG3dMatAnmResult) * resMdl->info.numMat�o�C�g
    // �̗̈���m�ۂ��Ă����Ȃ��Ă͂Ȃ�Ȃ��B
    //
    struct NNSG3dJntAnmResult_*   recJntAnm;
    struct NNSG3dMatAnmResult_*   recMatAnm;

    //
    // �A�j���[�V�������ǉ����ꂽ�Ƃ��AmatID/nodeID�ɑ΂��Ē�`����Ă����
    // bit��1�ɂȂ�B�eID�͍ő�ł�256�܂łȂ̂ŁA8���[�h���ŊǗ��ł���B
    // �������A�A�j���[�V�������폜���ꂽ�ꍇ�ɂ�bit��1�̂܂܂ł���B
    // SBC�̃C���^�v���^�͂��̃t�B�[���h���`�F�b�N���āA
    // NNSG3dFuncBlendMatXXX���R�[�����邩�ǂ��������肷��B
    // ���ӂ��ׂ��Ȃ̂́Abit��0�̂Ƃ��́A����matID/nodeID�Ɋ֌W����
    // �A�j���[�V�������Ȃ��A�Ƃ������Ƃ������m���ł��邱�Ƃł���B
    //
    u32 hintMatAnmExist[NNS_G3D_SIZE_MAT_MAX / 32];
    u32 hintJntAnmExist[NNS_G3D_SIZE_JNT_MAX / 32];
    u32 hintVisAnmExist[NNS_G3D_SIZE_JNT_MAX / 32];
}
NNSG3dRenderObj;

// NNSG3dJntAnmResult�v�Z���ʂ��o�b�t�@�����O����ۂ�
// �g�p����o�b�t�@�̃T�C�Y�v�Z�}�N��
#define NNS_G3D_RENDEROBJ_JNTBUFFER_SIZE(numJnt) \
    ((u32)(sizeof(NNSG3dJntAnmResult) * (numJnt)))

// NNSG3dMatAnmResult�v�Z���ʂ��o�b�t�@�����O����ۂ�
// �g�p����o�b�t�@�̃T�C�Y�v�Z�}�N��
#define NNS_G3D_RENDEROBJ_MATBUFFER_SIZE(numMat) \
    ((u32)(sizeof(NNSG3dMatAnmResult) * (numMat)))





////////////////////////////////////////////////////////////////////////////////
//
// �֐��̐錾
//

//
// inline functions for NNSG3dAnmObj
//
NNS_G3D_INLINE void NNS_G3dAnmObjSetFrame(NNSG3dAnmObj* pAnmObj, fx32 frame);
NNS_G3D_INLINE void NNS_G3dAnmObjSetBlendRatio(NNSG3dAnmObj* pAnmObj, fx32 ratio);
NNS_G3D_INLINE fx32 NNS_G3dAnmObjGetNumFrame(const NNSG3dAnmObj* pAnmObj);


//
// non-inline functions for NNSG3dAnmObj
//
u32 NNS_G3dAnmObjCalcSizeRequired(const void* pAnm, const NNSG3dResMdl* pMdl);
void NNS_G3dAnmObjInit(NNSG3dAnmObj* pAnmObj,
                       void* pResAnm,
                       const NNSG3dResMdl* pResMdl,
                       const NNSG3dResTex* pResTex);
void NNS_G3dAnmObjEnableID(NNSG3dAnmObj* pAnmObj, int id);
void NNS_G3dAnmObjDisableID(NNSG3dAnmObj* pAnmObj, int id);


//
// inline functions for NNSG3dRenderObj
//
NNS_G3D_INLINE void NNS_G3dRenderObjSetFlag(NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag);
NNS_G3D_INLINE void NNS_G3dRenderObjResetFlag(NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag);
NNS_G3D_INLINE BOOL NNS_G3dRenderObjTestFlag(const NNSG3dRenderObj* pRenderObj, NNSG3dRenderObjFlag flag);
NNS_G3D_INLINE u8* NNS_G3dRenderObjSetUserSbc(NNSG3dRenderObj* pRenderObj, u8* sbc);
NNS_G3D_INLINE void NNS_G3dRenderObjSetJntAnmBuffer(NNSG3dRenderObj* pRenderObj, struct NNSG3dJntAnmResult_* buf);
NNS_G3D_INLINE void NNS_G3dRenderObjSetMatAnmBuffer(NNSG3dRenderObj* pRenderObj, struct NNSG3dMatAnmResult_* buf);

NNS_G3D_INLINE struct NNSG3dJntAnmResult_* NNS_G3dRenderObjReleaseJntAnmBuffer(NNSG3dRenderObj* pRenderObj);
NNS_G3D_INLINE struct NNSG3dMatAnmResult_* NNS_G3dRenderObjReleaseMatAnmBuffer(NNSG3dRenderObj* pRenderObj);
NNS_G3D_INLINE void* NNS_G3dRenderObjSetUserPtr(NNSG3dRenderObj* pRenderObj, void* ptr);

NNS_G3D_INLINE NNSG3dResMdl* NNS_G3dRenderObjGetResMdl(NNSG3dRenderObj* pRenderObj);
NNS_G3D_INLINE void NNS_G3dRenderObjSetBlendFuncMat(NNSG3dRenderObj* pRenderObj, NNSG3dFuncAnmBlendMat func);
NNS_G3D_INLINE void NNS_G3dRenderObjSetBlendFuncJnt(NNSG3dRenderObj* pRenderObj, NNSG3dFuncAnmBlendJnt func);
NNS_G3D_INLINE void NNS_G3dRenderObjSetBlendFuncVis(NNSG3dRenderObj* pRenderObj, NNSG3dFuncAnmBlendVis func);


//
// non-inline functions for NNSG3dRenderObj
//
void NNS_G3dRenderObjInit(NNSG3dRenderObj* pRenderObj, NNSG3dResMdl* pResMdl);
void NNS_G3dRenderObjAddAnmObj(NNSG3dRenderObj* pRenderObj, NNSG3dAnmObj* pAnmObj);
void NNS_G3dRenderObjRemoveAnmObj(NNSG3dRenderObj* pRenderObj, NNSG3dAnmObj* pAnmObj);
void NNS_G3dRenderObjSetCallBack(NNSG3dRenderObj* pRenderObj,
                                 NNSG3dSbcCallBackFunc func,
                                 u8*,
                                 u8 cmd,
                                 NNSG3dSbcCallBackTiming timing);
void NNS_G3dRenderObjResetCallBack(NNSG3dRenderObj* pRenderObj);
void NNS_G3dRenderObjSetInitFunc(NNSG3dRenderObj* pRenderObj, NNSG3dSbcCallBackFunc func);


//
// non-inline functions for Model/Texture binding
//

// Texture Key
u32 NNS_G3dTexGetRequiredSize(const NNSG3dResTex* pTex);
u32 NNS_G3dTex4x4GetRequiredSize(const NNSG3dResTex* pTex);
void NNS_G3dTexSetTexKey(NNSG3dResTex* pTex,
                         NNSG3dTexKey texKey,
                         NNSG3dTexKey tex4x4Key);
void NNS_G3dTexLoad(NNSG3dResTex* pTex, BOOL exec_begin_end);
void NNS_G3dTexReleaseTexKey(NNSG3dResTex* pTex,
                             NNSG3dTexKey* texKey,
                             NNSG3dTexKey* tex4x4Key);

// Palette Key
u32 NNS_G3dPlttGetRequiredSize(const NNSG3dResTex* pTex);
void NNS_G3dPlttSetPlttKey(NNSG3dResTex* pTex, NNSG3dPlttKey plttKey);
void NNS_G3dPlttLoad(NNSG3dResTex* pTex, BOOL exec_begin_end);
NNSG3dPlttKey NNS_G3dPlttReleasePlttKey(NNSG3dResTex* pTex);

// Model -> Texture binding
BOOL NNS_G3dBindMdlTex(NNSG3dResMdl* pMdl, const NNSG3dResTex* pTex);
BOOL NNS_G3dBindMdlTexEx(NNSG3dResMdl* pMdl,
                         const NNSG3dResTex* pTex,
                         const NNSG3dResName* pResName);
BOOL NNS_G3dForceBindMdlTex(NNSG3dResMdl* pMdl,
                            const NNSG3dResTex* pTex,
                            u32 texToMatListIdx,
                            u32 texIdx);

void NNS_G3dReleaseMdlTex(NNSG3dResMdl* pMdl);
BOOL NNS_G3dReleaseMdlTexEx(NNSG3dResMdl* pMdl, const NNSG3dResName* pResName);

// Model -> Palette binding
BOOL NNS_G3dBindMdlPltt(NNSG3dResMdl* pMdl, const NNSG3dResTex* pTex);
BOOL NNS_G3dBindMdlPlttEx(NNSG3dResMdl* pMdl,
                          const NNSG3dResTex* pTex,
                          const NNSG3dResName* pResName);
BOOL NNS_G3dForceBindMdlPltt(NNSG3dResMdl* pMdl,
                             const NNSG3dResTex* pTex,
                             u32 plttToMatListIdx,
                             u32 plttIdx);
void NNS_G3dReleaseMdlPltt(NNSG3dResMdl* pMdl);
BOOL NNS_G3dReleaseMdlPlttEx(NNSG3dResMdl* pMdl, const NNSG3dResName* pResName);

BOOL NNS_G3dBindMdlSet(NNSG3dResMdlSet* pMdlSet, const NNSG3dResTex* pTex);
void NNS_G3dReleaseMdlSet(NNSG3dResMdlSet* pMdlSet);


//
// Misc inline functions
//

#ifdef __cplusplus
}
#endif

#include <nnsys/g3d/kernel_inline.h>

#endif
