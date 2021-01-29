//==============================================================================
/**
 * @file	footprint_stamp.c
 * @brief	���Ճ{�[�h�̃X�^���v����
 * @author	matsuda
 * @date	2008.01.20(��)
 */
//==============================================================================
#include "common.h"
#include "system/snd_tool.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/d3dobj.h"
#include "application/wifi_lobby/wflby_system_def.h"
#include "footprint_common.h"
#include "graphic/footprint_board.naix"
#include "footprint_stamp.h"
#include "poketool/monsno.h"
#include "poketool/pokefoot.h"	//POKEFOOT_ARC_CHAR_DMMY��`�̈�
#include "system/procsys.h"
#include "application/footprint_main.h"
#include "system/wipe.h"
#include "system/wipe_def.h"

#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "footprint_tool.h"
#include "footprint_snd_def.h"


//==============================================================================
//	�R���g���[���f�[�^
//==============================================================================
#include "footprint_stamp_control.dat"

//==============================================================================
//	�萔��`
//==============================================================================
///�A���Ǘ��̃��[�N�ԍ����擾���Ă��Ȃ����
#define CHAIN_WORK_NULL			(0xff)

///�X�^���v��Z���W
#define STAMP_POS_Z		(FX32_ONE * 16)	//(FX32_CONST(100))
///�X�^���v�̃X�P�[��
#define STAMP_SCALE		(FX32_CONST(1.00f))

///�X�^���v�̃T�C�Y(fx32)
#define STAMP_SIZE			(FX32_ONE * 16 * 1)	//1grid(1gird=16unit)
///�X�^���v�̔����̃T�C�Y
#define STAMP_SIZE_HALF		(STAMP_SIZE / 2)

///�X�^���v���삪�V�X�e���֕Ԃ��Ԏ�
typedef enum{
	RET_CONTINUE,		///<����ێ�
	RET_DELETE,			///<�폜�v��
}STAMP_RET;

///���Ղ̃e�N�X�`���[�T�C�Y(byte)
#define FOOTMARK_TEXTURE_SIZE	(16*16/4)

///�X�^���v�Ŏg�p�o����|���S��ID�̊J�n�l
#define STAMP_POLYGON_ID_START		(1)
///�X�^���v�Ŏg�p�o����|���S��ID�̏I���l
#define STAMP_POLYGON_ID_END		(63)
///�X�^���v�Ń|���S��ID�̊m�ۂɎ��s
#define STAMP_POLYGON_ID_ERROR		(0xff)


///�X�^���v���t���[���A�E�g�ƌ��Ȃ����W�͈�
///�{�[�h����5�O���b�h�A�c3�O���b�h(1grid=16unit)�ׁ̈A�����菭���傫�߂ɂ��Ă���
static const struct{
	fx32 left;
	fx32 right;
	fx32 top;
	fx32 bottom;
}StampFrameOutRect = {
	-FX32_ONE*16*(5+0),
	FX32_ONE*16*(5+0),
	FX32_ONE*16*(3+0),
	-FX32_ONE*16*(3+0),
};

//==============================================================================
//	�^��`
//==============================================================================
///�X�^���v�̓���֐��̌^
typedef STAMP_RET (* STAMP_MOVE_FUNC)(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
///�`����s�O�ɍs���֐��̌^
typedef void ( *STAMP_DRAW_FUNC )(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
///�X�^���v�̂����蔻��֐��̌^
typedef BOOL (* STAMP_HITCHECK)(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);

///�X�y�V�����G�t�F�N�g�̓���֐��̌^
typedef BOOL (* SPECIAL_EFF_FUNC)(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);

//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
//	����e�[�u���̌^
//--------------------------------------------------------------
///����e�[�u���̌^
typedef struct{
	STAMP_MOVE_FUNC move_func;			///<����֐�
	STAMP_DRAW_FUNC draw_func;			///<�Ǝ��ɕ`�揈�����s���ꍇ�w��
	STAMP_HITCHECK hitcheck_func;		///<�q�b�g�`�F�b�N
}STAMP_MOVE_DATA_TBL;

///�X�^���v�̓����蔻��`�F�b�N�p��RECT�^
typedef struct{
	fx32 left;
	fx32 right;
	fx32 top;
	fx32 bottom;
}STAMP_HIT_RECT;

//--------------------------------------------------------------
//	���ŃG�t�F�N�g
//--------------------------------------------------------------
///���ŃG�t�F�N�g�F���ʏ�������
typedef struct{
	s32 wait;
	STAMP_HIT_RECT rect;
}ERASE_EFF_INIT_MOVE;

///���ŃG�t�F�N�g�F�ɂ���
typedef struct{
	int alpha;			///<�A���t�@�l
	fx32 affine_xyz;	///<�g�k��
	u8 seq;			///<�V�[�P���X�ԍ�
	u8 polygon_id;		///<�|���S��ID
	STAMP_HIT_RECT rect;
}ERASE_EFF_NIJIMI;

///���ŃG�t�F�N�g�F�͂���
typedef struct{
	D3DOBJ		child_obj[HAJIKE_OBJ_CHILD_NUM];	///<�`��OBJ
	u16 frame;
	u8 seq;
	STAMP_HIT_RECT rect[HAJIKE_OBJ_CHILD_NUM + 1];	// +1 = �e�̕�
}ERASE_EFF_HAJIKE;

///���ŃG�t�F�N�g�F�W�O�U�O
typedef struct{
	int seq;
	int wait;
	int turn_frame;
	STAMP_HIT_RECT rect;
}ERASE_EFF_ZIGZAG;

///���ŃG�t�F�N�g�F�֍s
typedef struct{
	int seq;
	fx32 theta;
	int wait;
	fx32 center_y;
	STAMP_HIT_RECT rect;
}ERASE_EFF_DAKOU;

///���ŃG�t�F�N�g�F�O��
typedef struct{
	D3DOBJ		child_obj[KISEKI_OBJ_CHILD_NUM];	///<�`��OBJ
	u16 frame;
	u8 drop_no;
	u8 obj_hit;			///<TRUE:��OBJ�ƏՓ˂���
	u8 polygon_id;
	u8 seq;
	STAMP_HIT_RECT rect;
}ERASE_EFF_KISEKI;

///���ŃG�t�F�N�g�F�h��
typedef struct{
	int seq;
	fx32 theta;
	fx32 center_x;
	STAMP_HIT_RECT rect;
}ERASE_EFF_YURE;

///���ŃG�t�F�N�g�F�g��
typedef struct{
	fx32 affine_xyz;	///<�g�k��
	u16 frame;
	u8 seq;			///<�V�[�P���X�ԍ�
	STAMP_HIT_RECT rect;
}ERASE_EFF_KAKUDAI;

///���ŃG�t�F�N�g�F�u���[X
typedef struct{
	D3DOBJ		child_obj[BRAR_X_OBJ_CHILD_NUM];	///<�`��OBJ
	fx32 theta;
	u16 alpha;
	u8 polygon_id;
	u8 seq;
	STAMP_HIT_RECT rect[BRAR_X_OBJ_CHILD_NUM + 1];	// +1 = �e�̕�
}ERASE_EFF_BRAR_X;

///���ŃG�t�F�N�g�F�u���[Y
typedef struct{
	D3DOBJ		child_obj[BRAR_Y_OBJ_CHILD_NUM];	///<�`��OBJ
	fx32 theta;
	u16 alpha;
	u8 polygon_id;
	u8 seq;
	STAMP_HIT_RECT rect[BRAR_Y_OBJ_CHILD_NUM + 1];	// +1 = �e�̕�
}ERASE_EFF_BRAR_Y;

///���ŃG�t�F�N�g�F����
typedef struct{
	fx32 affine_xyz;	///<�g�k��
	u16 frame;
	u8 seq;
	STAMP_HIT_RECT rect;
}ERASE_EFF_TARE;

//--------------------------------------------------------------
/**
 * @brief	���ŃG�t�F�N�g���샏�[�N
 */
//--------------------------------------------------------------
typedef union{
	ERASE_EFF_INIT_MOVE	init_move;
	ERASE_EFF_NIJIMI	nijimi;
	ERASE_EFF_HAJIKE	hajike;
	ERASE_EFF_ZIGZAG	zigzag;
	ERASE_EFF_DAKOU		dakou;
	ERASE_EFF_KISEKI	kiseki;
	ERASE_EFF_YURE		yure;
	ERASE_EFF_KAKUDAI	kakudai;
	ERASE_EFF_BRAR_X	brar_x;
	ERASE_EFF_BRAR_Y	brar_y;
	ERASE_EFF_TARE		tare;
}ERASE_EFF_WORK;


//--------------------------------------------------------------
//	�V�X�e��
//--------------------------------------------------------------
///�X�^���v���쐧�䃏�[�N
typedef struct _STAMP_MOVE_WORK{
	STAMP_PARAM param;		///<�X�^���v�p�����[�^

	D3DOBJ_MDL	mdl;
	D3DOBJ		obj;
	
	ERASE_EFF_WORK erase_eff;	///<���ŃG�t�F�N�g���샏�[�N
	
	u32 initialize:4;			///<TRUE:����������
	u32 tex_load_req:4;			///<TRUE:�e�N�X�`��VRAM�]�����N�G�X�g�𔭍s���Ă���
	u32 move_type:8;			///<����^�C�v(MOVE_???)
	u32 next_move_type:8;		///<����^�C�v�\��(�A���������Ɉ����p������^�C�v���Z�b�g�����)
	u32 chain_work_no:8;		///<�g�p���Ă���A�����[�N�̔ԍ�
}STAMP_MOVE_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static STAMP_MOVE_PTR Stamp_Create(FOOTPRINT_SYS_PTR fps, STAMP_SYSTEM_WORK *ssw, const STAMP_PARAM *param, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, BOOL arceus_flg);
static void Stamp_Free(STAMP_MOVE_PTR move);
static void Stamp_DeletePack(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move, int stamp_no);
static void Stamp_PosConvert(FOOTPRINT_SYS_PTR fps, int x, int y, fx32 *ret_x, fx32 *ret_y);
static BOOL Stamp_MdlLoadH(STAMP_SYSTEM_WORK *ssw, D3DOBJ_MDL *p_mdl, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, const STAMP_PARAM *param, BOOL arceus_flg);
static void Stamp_TexRewrite(NNSG3dResTex *pTex, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, const STAMP_PARAM *param, BOOL arceus_flg);
static void Stamp_TexDotErase(NNSG3dResTex *pTex, int move_type);
static void Stamp_TexDotFlip(NNSG3dResTex *pTex, int move_type);
static void VWaitTCB_MdlLoad(TCB_PTR tcb, void *work);
static BOOL Stamp_MdlTexKeyAreaSecure(NNSG3dResTex* tex);
static BOOL Stamp_LoadTexDataSet(STAMP_SYSTEM_WORK *ssw, NNSG3dResTex *tex);
static void Stamp_LoadTexDataFree(STAMP_SYSTEM_WORK *ssw, NNSG3dResTex *tex);
static BOOL StampMoveTool_MoveTypeSet(STAMP_MOVE_PTR move, int move_type);
static u8 StampMoveTool_MoveTypeGet(const STAMP_PARAM *param);
static int Stamp_PolygonIDGet(STAMP_SYSTEM_WORK *ssw);
static void Stamp_PolygonIDFree(STAMP_SYSTEM_WORK *ssw, int polygon_id);
static void Stamp_HitRectCreate(D3DOBJ *obj, const STAMP_PARAM *param, STAMP_HIT_RECT *rect, int affine_flag);
static BOOL Stamp_RectHitCheck(const STAMP_HIT_RECT *rect0, const STAMP_HIT_RECT *rect1);
static void Stamp_ChainWorkGet(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static int Stamp_ChainAdd(STAMP_SYSTEM_WORK *ssw, int chain_work_no, int move_type);
static void Stamp_ChainSub(STAMP_SYSTEM_WORK *ssw, int chain_work_no);
static void Special_EffectSet(STAMP_SYSTEM_WORK *ssw);
static BOOL SpecialFlashEff(STAMP_SYSTEM_WORK *ssw, SPECIAL_FLASH_WORK *flash_work, int game_status, int board_type);

static STAMP_RET StampMove_FirstWait(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Zigzag(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Dakou(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Yure(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Kakudai(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static STAMP_RET StampMove_Tare(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);

static void StampDraw_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static void StampDraw_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static void StampDraw_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static void StampDraw_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);
static void StampDraw_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move);

static BOOL StampHitcheck_FirstWait(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Nijimi(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Hajike(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Zigzag(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Dakou(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Kiseki(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Yure(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Kakudai(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move, STAMP_MOVE_PTR target);
static BOOL StampHitcheck_BrarX(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_BrarY(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);
static BOOL StampHitcheck_Tare(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target);

static BOOL SpecialMove_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Zigzag(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Dakou(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Yure(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Kakudai(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);
static BOOL SpecialMove_Tare(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera);

//==============================================================================
//	�V�[�P���X�e�[�u��
//==============================================================================
///�X�^���v����֐��e�[�u��		��MOVE_TYPE_???�ƕ��т𓯂��ɂ��Ă������ƁI
static const STAMP_MOVE_DATA_TBL StampMoveDataTbl[] = {
	{StampMove_FirstWait,	NULL,					StampHitcheck_FirstWait},
	{StampMove_Nijimi,		StampDraw_Nijimi,		StampHitcheck_Nijimi},
	{StampMove_Hajike,		StampDraw_Hajike,		StampHitcheck_Hajike},
	{StampMove_Zigzag,		NULL,					StampHitcheck_Zigzag},
	{StampMove_Dakou,		NULL,					StampHitcheck_Dakou},
	{StampMove_Kiseki,		StampDraw_Kiseki,		StampHitcheck_Kiseki},
	{StampMove_Yure,		NULL,					StampHitcheck_Yure},
	{StampMove_Kakudai,		NULL,					StampHitcheck_Kakudai},
	{StampMove_BrarX,		StampDraw_BrarX,		StampHitcheck_BrarX},
	{StampMove_BrarY,		StampDraw_BrarY,		StampHitcheck_BrarY},
	{StampMove_Tare,		NULL,					StampHitcheck_Tare},
};

///�X�^���v�̈ړ��^�C�v  ��StampMoveDataTbl�ASpecialEffectDataTbl�ƕ��т𓯂��ɂ��Ă������ƁI
enum{
	MOVE_TYPE_FIRST_WAIT,	///<�����E�F�C�g
	MOVE_TYPE_NIJIMI,
	MOVE_TYPE_HAJIKE,
	MOVE_TYPE_ZIGZAG,
	MOVE_TYPE_DAKOU,
	MOVE_TYPE_KISEKI,
	MOVE_TYPE_YURE,
	MOVE_TYPE_KAKUDAI,
	MOVE_TYPE_BRAR_X,
	MOVE_TYPE_BRAR_Y,
	MOVE_TYPE_TARE,
};
///���i���瓮��^�C�v�����o���e�[�u��
ALIGN4 static const u8 Seikaku_to_MoveType_Tbl[] = {
	MOVE_TYPE_KISEKI,	//����΂��
	MOVE_TYPE_NIJIMI,	//���݂�����
	MOVE_TYPE_KISEKI,	//�䂤����
	MOVE_TYPE_YURE,		//�������ς�
	MOVE_TYPE_YURE,		//��񂿂�
	MOVE_TYPE_DAKOU,	//���ԂƂ�
	MOVE_TYPE_ZIGZAG,	//���Ȃ�
	MOVE_TYPE_BRAR_Y,	//�̂�
	MOVE_TYPE_KAKUDAI,	//���ς�
	MOVE_TYPE_BRAR_X,	//�̂��Ă�
	MOVE_TYPE_NIJIMI,	//�����т傤
	MOVE_TYPE_KISEKI,	//��������
	MOVE_TYPE_ZIGZAG,	//�܂���
	MOVE_TYPE_BRAR_X,	//�悤��
	MOVE_TYPE_HAJIKE,	//�ނ��Ⴋ
	MOVE_TYPE_TARE,		//�Ђ�����
	MOVE_TYPE_DAKOU,	//�����Ƃ�
	MOVE_TYPE_ZIGZAG,	//�ꂢ����
	MOVE_TYPE_BRAR_Y,	//�Ă��
	MOVE_TYPE_KAKUDAI,	//���������
	MOVE_TYPE_TARE,		//�����₩
	MOVE_TYPE_DAKOU,	//���ƂȂ���
	MOVE_TYPE_YURE,		//�Ȃ܂���
	MOVE_TYPE_NIJIMI,	//���񂿂傤
	MOVE_TYPE_HAJIKE,	//���܂���
};

///�X�^���v�̃q�b�g�T�C�Y
static const STAMP_HIT_RECT StampDefaultHitRect[] = {
	{-(STAMP_SIZE_HALF / 3), STAMP_SIZE_HALF / 3, STAMP_SIZE_HALF / 3, -(STAMP_SIZE_HALF / 3)},
	{-(STAMP_SIZE_HALF / 2), STAMP_SIZE_HALF / 2, STAMP_SIZE_HALF / 2, -(STAMP_SIZE_HALF / 2)},
	{-(STAMP_SIZE_HALF/3*2), STAMP_SIZE_HALF/3*2, STAMP_SIZE_HALF / 3*2, -(STAMP_SIZE_HALF / 3*2)},
};


//--------------------------------------------------------------
//	�X�y�V�����G�t�F�N�g�̓���V�[�P���X�e�[�u��
//--------------------------------------------------------------
///�X�y�V�����G�t�F�N�g�̓���V�[�P���X�e�[�u��	��MOVE_TYPE_???�ƕ��т𓯂��ɂ��Ă������ƁI�I
static const SPECIAL_EFF_FUNC SpecialEffectDataTbl[] = {
	NULL,						//MOVE_TYPE_FIRST_WAIT
	SpecialMove_Nijimi,         //MOVE_TYPE_NIJIMI
	SpecialMove_Hajike,         //MOVE_TYPE_HAJIKE
	SpecialMove_Zigzag,         //MOVE_TYPE_ZIGZAG
	SpecialMove_Dakou,          //MOVE_TYPE_DAKOU
	SpecialMove_Kiseki,         //MOVE_TYPE_KISEKI
	SpecialMove_Yure,           //MOVE_TYPE_YURE
	SpecialMove_Kakudai,        //MOVE_TYPE_KAKUDAI
	SpecialMove_BrarX,          //MOVE_TYPE_BRAR_X
	SpecialMove_BrarY,          //MOVE_TYPE_BRAR_Y
	SpecialMove_Tare,           //MOVE_TYPE_TARE
};


//--------------------------------------------------------------
//	�X�^���v�z�u���W�f�[�^
//--------------------------------------------------------------
#define STAMP_BASE_POS_OFFSET	(16)	//16�h�b�g�Ԋu
//16�h�b�g�Ԋu
static const s32 StampPosBaseX[] = {
	-238076,
	-209404,
	-180732,
	-150012,
	-121340,
	-90620,
	-61948,
	-31228,
	-1020,
	28676,
	57348,
	88068,
	116740,
	147460,
	176132,
	206852,
	235524,
};

//16�h�b�g�Ԋu
static const s32 StampPosBaseY[] = {
	146191,
	117519,
	87311,
	57615,
	27919,
	-2289,
	-31985,
	-61681,
	-91377,
	-121073,
	-149745,
};

//--------------------------------------------------------------
//	�f�o�b�O�p�ϐ�
//--------------------------------------------------------------
#ifdef PM_DEBUG
static struct{
	D3DOBJ_MDL	mdl;
	D3DOBJ		obj[4];
}DebugStamp;

static u8 debug_sp_eff = 0;
#endif


//==============================================================================
//
//	�X�^���v�V�X�e��
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�^���v�V�X�e��������
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   arceus_flg	�A���Z�E�X���J�t���O(TRUE:���JOK)
 */
//--------------------------------------------------------------
void StampSys_Init(STAMP_SYSTEM_WORK *ssw, BOOL arceus_flg)
{
	MI_CpuClear8(ssw, sizeof(STAMP_SYSTEM_WORK));

#ifdef PM_DEBUG
	{
		ARCHANDLE *hdl_main, *hdl_mark;
		STAMP_PARAM debug_stamp = {
			0x001f, 493, 0x0000, 0, 0, 0,
		};
		int i;
		
		hdl_main = ArchiveDataHandleOpen( ARC_FOOTPRINT_GRA, HEAPID_FOOTPRINT );
		hdl_mark = ArchiveDataHandleOpen(ARC_POKEFOOT_GRA, HEAPID_FOOTPRINT);

		Stamp_MdlLoadH(ssw, &DebugStamp.mdl, hdl_main, hdl_mark, &debug_stamp, arceus_flg);
		for(i = 0; i < 4; i++){
			D3DOBJ_Init( &DebugStamp.obj[i], &DebugStamp.mdl );
			D3DOBJ_SetDraw( &DebugStamp.obj[i], FALSE );
		}

		ArchiveDataHandleClose( hdl_main );
		ArchiveDataHandleClose( hdl_mark );
	}
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�V�X�e����j��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void StampSys_Exit(STAMP_SYSTEM_WORK *ssw)
{
	int i;
	
	for(i = 0; i < STAMP_MAX; i++){
		if(ssw->move[i] != NULL){
			Stamp_Free(ssw->move[i]);
		}
	}
#ifdef PM_DEBUG
	D3DOBJ_MdlDelete(&DebugStamp.mdl);
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v��V�K�쐬����
 *
 * @param   ssw				�X�^���v�V�X�e���ւ̃|�C���^
 * @param   param			�X�^���v�p�����[�^
 * @param   hdl_main		���C���O���t�B�b�N�ւ̃n���h��
 * @param   hdl_mark		���ՃO���t�B�b�N�ւ̃n���h��
 * @param   arceus_flg		�A���Z�E�X���J�t���O(TRUE:���JOK)
 *
 * @retval	TRUE:�쐬�����B�@FALSE:�쐬���s
 */
//--------------------------------------------------------------
BOOL StampSys_Add(FOOTPRINT_SYS_PTR fps, STAMP_SYSTEM_WORK *ssw, const STAMP_PARAM *param, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, BOOL arceus_flg)
{
	int i;
	
	if(ssw->special_effect_type != 0){
		return FALSE;
	}
	
	for(i = 0; i < STAMP_MAX; i++){
		if(ssw->move[i] == NULL){
			ssw->move[i] = Stamp_Create(fps, ssw, param, hdl_main, hdl_mark, arceus_flg);
			if(ssw->move[i] == NULL){
				OS_TPrintf("�X�^���v�̐V�K�o�^���s�F�e�N�X�`�����������������ς�\n");
				return FALSE;
			}
			return TRUE;
		}
	}
	
	OS_TPrintf("�X�^���v�̐V�K�o�^���s�F�̈悢���ς�������\n");
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�X�V����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void StampSys_Update(STAMP_SYSTEM_WORK *ssw, GF_CAMERA_PTR camera_ptr, int game_status, int board_type)
{
	int i;
	STAMP_MOVE_PTR move;
	STAMP_RET ret;
	BOOL sp_ret;
	
#ifdef PM_DEBUG		//�T�E���h�`�F�b�N�p
	if((sys.trg & PAD_BUTTON_A) && ssw->special_effect_type == 0){
		MI_CpuClear8(&ssw->special_work, sizeof(STAMP_SPECIAL_WORK));
		debug_sp_eff++;
		debug_sp_eff %= NELEMS(SpecialEffectDataTbl);
		ssw->special_effect_type = debug_sp_eff;
	}
#endif

	//�X�y�V�����G�t�F�N�g�����O�̓���
	if(ssw->special_effect_req != 0){
		SpecialFlashEff(ssw, &ssw->flash_work, game_status, board_type);
	}
	
	//�X�y�V�����G�t�F�N�g����
	if(ssw->special_effect_type != 0){
		if(SpecialEffectDataTbl[ssw->special_effect_type](ssw, &ssw->special_work, camera_ptr) == TRUE){
			ssw->special_effect_type = 0;
		}
	}
	
	//�X�^���v����
	for(i = 0; i < STAMP_MAX; i++){
		move = ssw->move[i];
		if(move != NULL){
			if(move->move_type == 0 && move->next_move_type != 0){
				move->move_type = move->next_move_type;
			}
			
			ret = StampMoveDataTbl[move->move_type].move_func(ssw, move);
			switch(ret){
			case RET_DELETE:
				Stamp_DeletePack(ssw, move, i);
				break;
			}
		}
		move++;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void StampSys_ObjDraw(STAMP_SYSTEM_WORK *ssw)
{
	int i;
	STAMP_MOVE_PTR move;
	
	for(i = 0; i < STAMP_MAX; i++){
		move = ssw->move[i];
		if(move != NULL){
			if(StampMoveDataTbl[move->move_type].draw_func != NULL){
				StampMoveDataTbl[move->move_type].draw_func(ssw, move);
			}
			else{
				D3DOBJ_Draw(&move->obj);
			}
		}
	}

#ifdef PM_DEBUG
	for(i = 0; i < 4; i++){
		D3DOBJ_Draw(&DebugStamp.obj[i]);
	}
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�̃q�b�g�`�F�b�N
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void StampSys_HitCheck(STAMP_SYSTEM_WORK *ssw)
{
	int i, move_max, normal_max, move_count, normal_count;
	STAMP_MOVE_PTR move, normal;
	BOOL ret;
	int sp_eff = 0;
	
	//�q�b�g�`�F�b�N�Ώۂ�move�|�C���^�݂̂𔲂��o��
	move_max = 0;
	normal_max = 0;
	for(i = 0; i < STAMP_MAX; i++){
		move = ssw->move[i];
		if(move != NULL){
			if(move->move_type != 0){	//���ɓ��삵�Ă���X�^���v
				ssw->hitcheck_move[move_max] = move;
				move_max++;
			}
			else{	//�܂�����O�̃X�^���v
				ssw->hitcheck_move[STAMP_MAX - 1 - normal_max] = move;
				normal_max++;
			}
		}
	}
	
	//�q�b�g�`�F�b�N
	for(normal_count = 0; normal_count < normal_max; normal_count++){
		normal = ssw->hitcheck_move[STAMP_MAX - 1 - normal_count];
		for(move_count = 0; move_count < move_max; move_count++){
			move = ssw->hitcheck_move[move_count];
			if(StampMoveDataTbl[move->move_type].hitcheck_func(ssw, move, normal) == TRUE){
				ret = StampMoveTool_MoveTypeSet(normal, move->move_type);
				if(ret == TRUE && normal->chain_work_no == CHAIN_WORK_NULL){
					normal->chain_work_no = move->chain_work_no;
					if(Stamp_ChainAdd(ssw, move->chain_work_no, move->move_type) == TRUE){
						sp_eff++;
					}
					break;	//�����蔻�肪���������̂Ŏ��̃m�[�}���X�^���v�̔���ցB
				}
			}
		}
	}
	
	//�X�y�V�����G�t�F�N�g���������𖞂����Ă���΃X�y�V��������
	if(ssw->special_effect_req != 0 && ssw->special_effect_start_effect_end == TRUE){
		for(i = 0; i < STAMP_MAX; i++){
			move = ssw->move[i];
			if(move != NULL && move->move_type != ssw->special_effect_req 
					&& move->next_move_type != ssw->special_effect_req){
				if(StampMoveTool_MoveTypeSet(move, ssw->special_effect_req) == FALSE){
					//���ɓ��삵�Ă���X�^���v�͍폜����
					Stamp_DeletePack(ssw, move, i);
				}
			}
		}
		Special_EffectSet(ssw);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���vVwait�X�V����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   game_status	�Q�[���X�e�[�^�X
 */
//--------------------------------------------------------------
void StampSys_VWaitUpdate(STAMP_SYSTEM_WORK *ssw, int game_status)
{
	int i;
	NNSG3dResTex **tex;
	
	tex = ssw->load_tex;
	for(i = 0; i < LOAD_TEX_BUFFER_MAX; i++){
		if(*tex != NULL){
			//DMA�]������̂Ńt���b�V������
			DC_FlushRange( *tex, (*tex)->header.size );
			// VRAM�ւ̃��[�h
			NNS_G3dTexLoad(*tex, TRUE);
			NNS_G3dPlttLoad(*tex, TRUE);
			
			*tex = NULL;
			OS_TPrintf("�]������ %d\n", i);
		}
		tex++;
	}
	
	if(ssw->v_wipe_req && game_status != FOOTPRINT_GAME_STATUS_FINISH){
		WIPE_SetMstBrightness(WIPE_DISP_MAIN, ssw->v_wipe_no);
		ssw->v_wipe_req = 0;
	}
}


//==============================================================================
//
//	�X�^���v
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief	�X�^���v���쐬����
 *
 * @param	param			�X�^���v�p�����[�^�ւ̃|�C���^
 * @param	ssw				�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param	hdl_main		���Ճ{�[�h�̃��C��narc�t�@�C���̃n���h��
 * @param	hdl_mark		���ՃO���t�B�b�N��narc�t�@�C���̃n���h��
 * @param   arceus_flg		�A���Z�E�X���J�t���O(TRUE:���JOK)
 *
 * @retval	���������X�^���v���샏�[�N�ւ̃|�C���^
 * @retval	�����o���Ȃ������ꍇ��NULL
 */
//--------------------------------------------------------------
static STAMP_MOVE_PTR Stamp_Create(FOOTPRINT_SYS_PTR fps, STAMP_SYSTEM_WORK *ssw, const STAMP_PARAM *param, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, BOOL arceus_flg)
{
	STAMP_MOVE_PTR move;
	fx32 x, y;
	BOOL mdl_ret;
	
	move = sys_AllocMemory(HEAPID_FOOTPRINT, sizeof(STAMP_MOVE_WORK));
	MI_CpuClear8(move, sizeof(STAMP_MOVE_WORK));
	move->chain_work_no = CHAIN_WORK_NULL;
	
	move->param = *param;
	if(move->param.monsno > MONSNO_END){
		GF_ASSERT(0);
		move->param.monsno = MONSNO_METAMON;
	}
	
	//���f���f�[�^�ǂݍ���
	//D3DOBJ_MdlLoadH(&move->mdl, hdl_main, NARC_footprint_board_ashiato_nsbmd, HEAPID_FOOTPRINT);
	mdl_ret = Stamp_MdlLoadH(ssw, &move->mdl, hdl_main, hdl_mark, &move->param, arceus_flg);
	if(mdl_ret == FALSE){
		return NULL;
	}
	
	//�����_�[�I�u�W�F�N�g�ɓo�^
	D3DOBJ_Init( &move->obj, &move->mdl );
	
	//���W�ݒ�
	Stamp_PosConvert(fps, param->x, param->y, &x, &y);
	D3DOBJ_SetMatrix( &move->obj, x, y, STAMP_POS_Z);
	D3DOBJ_SetScale(&move->obj, STAMP_SCALE, STAMP_SCALE, STAMP_SCALE);
	D3DOBJ_SetDraw( &move->obj, FALSE );
	
	return move;
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`���f�[�^�𑫐Ղɏ���������VRAM�]��(���f���f�[�^�̓ǂݍ��݂Ȃǂ��s��)
 *
 * @param   p_mdl			���f���f�[�^�����
 * @param   hdl_main		���C���O���t�B�b�N�ւ̃n���h��
 * @param   hdl_mark		���ՃO���t�B�b�N�ւ̃n���h��
 * @param   param			�X�^���v�p�����[�^�ւ̃|�C���^
 * @param   arceus_flg		�A���Z�E�X���J�t���O(TRUE:���JOK)
 * 
 * @retval	TRUE:�����B�@FALSE:���s
 *
 *	D3DOBJ_MdlLoadH�֐��̒��g�𔲂��o���ĕK�v�ȕ������J�X�^�}�C�Y��������
 */
//--------------------------------------------------------------
static BOOL Stamp_MdlLoadH(STAMP_SYSTEM_WORK *ssw, D3DOBJ_MDL *p_mdl, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, const STAMP_PARAM *param, BOOL arceus_flg)
{
	// ���f���ް��ǂݍ���
	p_mdl->pResMdl = ArcUtil_HDL_Load(hdl_main, NARC_footprint_board_ashiato_nsbmd, 
		FALSE, HEAPID_FOOTPRINT, ALLOC_TOP );

	// ���f���f�[�^�ݒ聕�e�N�X�`���]��
	{
		BOOL vram_ret;

		// ���\�[�X�ǂݍ��ݍς݂ł���K�v������
		GF_ASSERT( p_mdl->pResMdl );
		
		// ���f���ް��擾
		p_mdl->pModelSet	= NNS_G3dGetMdlSet( p_mdl->pResMdl );
		p_mdl->pModel		= NNS_G3dGetMdlByIdx( p_mdl->pModelSet, 0 );
		p_mdl->pMdlTex		= NNS_G3dGetTex( p_mdl->pResMdl );
		
		if( p_mdl->pMdlTex ){
			//�e�N�X�`����������
			Stamp_TexRewrite(p_mdl->pMdlTex, hdl_main, hdl_mark, param, arceus_flg);
			
			// �e�N�X�`���f�[�^�]��
			//LoadVRAMTexture( p_mdl->pMdlTex );
			vram_ret = Stamp_MdlTexKeyAreaSecure(p_mdl->pMdlTex);
			if(vram_ret == FALSE || Stamp_LoadTexDataSet(ssw, p_mdl->pMdlTex) == FALSE){
				//VRAM or TCB���m�ۏo���Ȃ��ꍇ�́A�S�ĉ������
				NNSG3dTexKey texKey;
				NNSG3dTexKey tex4x4Key;
				NNSG3dPlttKey plttKey;

				//VramKey�j��
				if(vram_ret == TRUE){
					NNS_G3dTexReleaseTexKey(p_mdl->pMdlTex, &texKey, &tex4x4Key );
					NNS_GfdFreeTexVram( texKey );
					NNS_GfdFreeTexVram( tex4x4Key );

					plttKey = NNS_G3dPlttReleasePlttKey( p_mdl->pMdlTex );
					NNS_GfdFreePlttVram( plttKey );
				}
				
				// �S���\�[�X�j��
				if(p_mdl->pResMdl){
					sys_FreeMemoryEz( p_mdl->pResMdl );
				}
				memset( p_mdl, 0, sizeof(D3DOBJ_MDL) );

				OS_TPrintf("!!!!!VRAM or TCB�̊m�ێ��s!!!!!! vram_ret = %d\n", vram_ret);
				return FALSE;
			}
			
			BindTexture( p_mdl->pResMdl, p_mdl->pMdlTex );
		}
	}
	
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`��Vwait�]�����N�G�X�g��ݒ肷��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   tex		�]������e�N�X�`�����\�[�X�ւ̃|�C���^
 *
 * @retval  TRUE:���N�G�X�g��t����
 * @retval  TRUE:���N�G�X�g��t���s
 */
//--------------------------------------------------------------
static BOOL Stamp_LoadTexDataSet(STAMP_SYSTEM_WORK *ssw, NNSG3dResTex *tex)
{
	int i;
	
	for(i = 0; i < LOAD_TEX_BUFFER_MAX; i++){
		if(ssw->load_tex[i] == NULL){
			ssw->load_tex[i] = tex;
			return TRUE;
		}
	}
//	GF_ASSERT(0);	//�e�N�X�`��VWait�]���o�b�t�@�������ς�������
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`��Vwait�]�����N�G�X�g����������
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   tex		�]������e�N�X�`�����\�[�X�ւ̃|�C���^
 *
 * @retval  TRUE:���N�G�X�g��t����
 * @retval  TRUE:���N�G�X�g��t���s
 */
//--------------------------------------------------------------
static void Stamp_LoadTexDataFree(STAMP_SYSTEM_WORK *ssw, NNSG3dResTex *tex)
{
	int i;
	
	for(i = 0; i < LOAD_TEX_BUFFER_MAX; i++){
		if(ssw->load_tex[i] == tex){
			ssw->load_tex[i] = NULL;
			return;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`���̈�m�ہ����\�[�X�ɃL�[���Z�b�g����
 *
 * @param   tex		�e�N�X�`�����\�[�X�ւ̃|�C���^
 *
 * @retval  TRUE:�m�ې���
 * @retval  FALSE:�m�ێ��s
 */
//--------------------------------------------------------------
static BOOL Stamp_MdlTexKeyAreaSecure(NNSG3dResTex* tex)
{
    u32 szTex, szPltt;
    NNSG3dTexKey keyTex;
    NNSG3dPlttKey keyPltt;

	tex->texInfo.vramKey = 0;

	// �K�v�ȃT�C�Y���擾
    szTex    = NNS_G3dTexGetRequiredSize(tex);
    szPltt   = NNS_G3dPlttGetRequiredSize(tex);

	// �e�N�X�`���C���[�W�X���b�g�Ɋm��
	keyTex = NNS_GfdAllocTexVram(szTex, FALSE, 0);
    if (keyTex == NNS_GFD_ALLOC_ERROR_TEXKEY){
		return FALSE;
	}

	// ���݂���΃e�N�X�`���p���b�g�X���b�g�Ɋm��
	keyPltt = 
		NNS_GfdAllocPlttVram(szPltt,
							tex->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4,
							0);
	if (keyPltt == NNS_GFD_ALLOC_ERROR_PLTTKEY){
		NNS_GfdFreeTexVram(keyTex);	//��Ɋm�ۂ��Ă����e�N�X�`��VRAM���������
		return FALSE;
	}

	// �L�[�̃A�T�C��
	NNS_G3dTexSetTexKey(tex, keyTex, 0);
	NNS_G3dPlttSetPlttKey(tex, keyPltt);

#if 0
	//DMA�]������̂Ńt���b�V������
	DC_FlushRange( tex, tex->header.size );
	// VRAM�ւ̃��[�h
	NNS_G3dTexLoad(tex, TRUE);
	NNS_G3dPlttLoad(tex, TRUE);
#endif

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`���C���[�W���w��|�P�����̑��Ղɏ���������
 *
 * @param   pTex			�e�N�X�`�����\�[�X�ւ̃|�C���^
 * @param   hdl_mark		���ՃO���t�B�b�N�̃A�[�J�C�u�n���h���ւ̃|�C���^
 * @param   param			�X�^���v�p�����[�^�ւ̃|�C���^
 * @param   arceus_flg		�A���Z�E�X���J���t���O(TRUE:���JOK)
 */
//--------------------------------------------------------------
static void Stamp_TexRewrite(NNSG3dResTex *pTex, ARCHANDLE *hdl_main, ARCHANDLE *hdl_mark, const STAMP_PARAM *param, BOOL arceus_flg)
{
	u16 *pPalDest;
	u8 *pDest, *pSrc;
	enum{
		AFTER_COLOR_NO = 1,	//�F���̏ꏊ�͑S�Ă��̃J���[�ԍ��ɕϊ�����
	};
	NNSG2dCharacterData *pChar;
	int foot_disp;
	
	foot_disp = FootprintTool_FootDispCheck(param->monsno, param->form_no, arceus_flg);

	pDest = (u8*)((u8*)pTex + pTex->texInfo.ofsTex);	//�e�N�X�`���̈�
	if(foot_disp == TRUE){
		pSrc = ArcUtil_HDL_Load(hdl_mark, POKEFOOT_ARC_CHAR_DMMY + param->monsno, //����
			TRUE, HEAPID_FOOTPRINT, ALLOC_BOTTOM);
		NNS_G2dGetUnpackedCharacterData(pSrc, &pChar); 
	}
	else{
		pSrc = ArcUtil_HDL_Load(hdl_main, NARC_footprint_board_wifi_mark_NCGR, //WIFI�}�[�N
			FALSE, HEAPID_FOOTPRINT, ALLOC_BOTTOM);
		NNS_G2dGetUnpackedCharacterData(pSrc, &pChar);
	}
	
	//�e�N�X�`���̈��S�ăN���A(���炩���ߓ����Ă���_�~�[�̑��Ճf�[�^���N���A���Ă���)
	MI_CpuClear16(pDest, FOOTMARK_TEXTURE_SIZE);

	OS_TPrintf("�X�^���v��monsno = %d\n", param->monsno);
	//2D�O���t�B�b�N��ϊ����Ȃ���e�N�X�`���̈�Ƀf�[�^������
	{
		int x, y, i, s, w, dest_shift;
		u8 *read_src, *read_data;
		u16 *write_data;
		
		read_src = pChar->pRawData;
		read_data = read_src;
		write_data = (u16*)pDest;
		for(w = 0; w < 4; w++){
			switch(w){
			case 0:
				if(foot_disp == TRUE){
					read_data = &read_src[0x20 * 4];	//128k�}�b�s���O�Ȃ̂ŋ󔒂������Ă���
				}
				else{
					read_data = &read_src[0x20 * 0];
				}
				write_data = (u16*)pDest;
				break;
			case 1:
				if(foot_disp == TRUE){
					read_data = &read_src[0x20 * 5];	//128k�}�b�s���O�Ȃ̂ŋ󔒂������Ă���
				}
				else{
					read_data = &read_src[0x20 * 1];
				}
				write_data = (u16*)pDest;
				write_data++;
				break;
			case 2:
				if(foot_disp == TRUE){
					read_data = read_src;
				}
				else{
					read_data = &read_src[0x20 * 2];
				}
				write_data = (u16*)pDest;
				write_data = &write_data[2*8];
				break;
			case 3:
				if(foot_disp == TRUE){
					read_data = &read_src[0x20 * 1];	//128k�}�b�s���O�Ȃ̂ŋ󔒂������Ă���
				}
				else{
					read_data = &read_src[0x20 * 3];
				}
				write_data = (u16*)pDest;
				write_data = &write_data[2*8 + 1];
				break;
			}
			
			i = 0;
			for(y = 0; y < 8; y++){
				dest_shift = 0;
				for(s = 0; s < 4; s++){
					//OS_TPrintf("read_data %d = %x\n", i, read_data[i]);
					if(read_data[i] & 0x0f){
						*write_data |= AFTER_COLOR_NO << dest_shift;
					}
					dest_shift += 2;
					if(read_data[i] & 0xf0){
						*write_data |= AFTER_COLOR_NO << dest_shift;
					}
					dest_shift += 2;
					i++;
				}
				//OS_TPrintf("texbit = %x\n", *write_data);
				write_data++;
				write_data++;
			}
		}
	}

	//���Ղ̃p���b�g�ύX
	pPalDest = (u16*)((u8*)pTex + pTex->plttInfo.ofsPlttData);
	pPalDest[1] = param->color;

	sys_FreeMemoryEz(pSrc);
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`���C���[�W���ꕔ���
 *
 * @param   pTex			�e�N�X�`�����\�[�X�ւ̃|�C���^
 * @param   hdl_mark		���ՃO���t�B�b�N�̃A�[�J�C�u�n���h���ւ̃|�C���^
 * @param   param			�X�^���v�p�����[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Stamp_TexDotErase(NNSG3dResTex *pTex, int move_type)
{
	u16 *pDest;
	int i;
	
	pDest = (u16*)((u8*)pTex + pTex->texInfo.ofsTex);	//�e�N�X�`���̈�
	
	switch(move_type){
	case MOVE_TYPE_ZIGZAG:
		pDest[0] = 0;
		pDest[1] = 0;
		break;
	case MOVE_TYPE_DAKOU:
		for(i = 0; i < 32; i += 2){
			pDest[i] &= 0xfffc;
		}
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`���C���[�W���h�b�g�P�ʂő��삵�A�t���b�v����
 *
 * @param   pTex			�e�N�X�`�����\�[�X�ւ̃|�C���^
 * @param   hdl_mark		���ՃO���t�B�b�N�̃A�[�J�C�u�n���h���ւ̃|�C���^
 * @param   param			�X�^���v�p�����[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Stamp_TexDotFlip(NNSG3dResTex *pTex, int move_type)
{
	u32 *pDest;
	u32 *pSrc;
	int x, y, dest_x, dest_y, data;
	
	pDest = (u32*)((u8*)pTex + pTex->texInfo.ofsTex);	//�e�N�X�`���̈�
	pSrc = sys_AllocMemory(HEAPID_FOOTPRINT, FOOTMARK_TEXTURE_SIZE);
	MI_CpuCopy16(pDest, pSrc, FOOTMARK_TEXTURE_SIZE);
	MI_CpuClear16(pDest, FOOTMARK_TEXTURE_SIZE);
	
	switch(move_type){
	case MOVE_TYPE_ZIGZAG:
		dest_x = 30;
		for(y = 0; y < 16; y++){
			dest_y = 0;
			for(x = 0; x < 32; x += 2){
				data = (pSrc[y] >> x) & 3;
				pDest[dest_y] |= data << dest_x;
				dest_y++;
			}
			dest_x -= 2;
		}
		break;
	case MOVE_TYPE_DAKOU:
		dest_x = 0;
		for(y = 0; y < 16; y++){
			dest_y = 15;
			for(x = 0; x < 32; x += 2){
				data = (pSrc[y] >> x) & 3;
				pDest[dest_y] |= data << dest_x;
				dest_y--;
			}
			dest_x += 2;
		}
		break;
	}
	
	sys_FreeMemoryEz(pSrc);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v���������
 *
 * @param   move		�X�^���v���샏�[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Stamp_Free(STAMP_MOVE_PTR move)
{
	D3DOBJ_MdlDelete( &move->mdl );
	sys_FreeMemoryEz(move);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�̍폜�ɕK�v�ȏ������܂Ƃ߂�����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   stamp_no	�X�^���v�ԍ�
 */
//--------------------------------------------------------------
static void Stamp_DeletePack(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move, int stamp_no)
{
	if(move->initialize == 0 || move->tex_load_req == TRUE){
		Stamp_LoadTexDataFree(ssw, move->mdl.pMdlTex);
	}
	Stamp_ChainSub(ssw, move->chain_work_no);
	Stamp_Free(move);
	ssw->move[stamp_no] = NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�X�^���v��2D���W��3D���W�ɕϊ�����
 *
 * @param	x			2D���WX
 * @param	y			2D���WY
 * @param	ret_x		X�̕ϊ����W�����
 * @param	ret_y		Y�̕ϊ����W�����
 */
//--------------------------------------------------------------
static void Stamp_PosConvert(FOOTPRINT_SYS_PTR fps, int x, int y, fx32 *ret_x, fx32 *ret_y)
{
#if 0
	*ret_x = FX32_CONST(x - 256/2);
	*ret_y = FX32_CONST(-(y - 196/2));
	
	{
		VecFx32 near, far;
		NNS_G3dScrPosToWorldLine(x, y, &near, &far);
		OS_TPrintf("tp_x = %d, tp_y = %d, \nnear.x = %d, near.y = %d, near.z = %d, \nfar.x = %d, far.y = %d, far.z = %d\n\n", x, y, near.x, near.y,near.z, far.x,far.y,far.z);
	}
#elif 0
	fx32 width, height;
	fx32 width_half, height_half;
	
	Footprint_WorldWidthHeightGet(fps, &width, &height);
	*ret_x = width * x / 256 - width / 2;
	*ret_y = -(height * y / 196 - height / 2);
#else
	int tbl_no;
	
	tbl_no = x / STAMP_BASE_POS_OFFSET;
	if(tbl_no >= NELEMS(StampPosBaseX)){	//�ꉞ
		tbl_no = NELEMS(StampPosBaseX) - 1;
	}
	*ret_x = StampPosBaseX[tbl_no];
	if(tbl_no < NELEMS(StampPosBaseX) - 1){
		*ret_x += (StampPosBaseX[tbl_no + 1] - StampPosBaseX[tbl_no]) 
			/ STAMP_BASE_POS_OFFSET * (x % STAMP_BASE_POS_OFFSET);
	}

	tbl_no = y / STAMP_BASE_POS_OFFSET;
	if(tbl_no >= NELEMS(StampPosBaseY)){	//�ꉞ
		tbl_no = NELEMS(StampPosBaseY) - 1;
	}
	*ret_y = StampPosBaseY[tbl_no];
	if(tbl_no < NELEMS(StampPosBaseY) - 1){
		*ret_y += (StampPosBaseY[tbl_no + 1] - StampPosBaseY[tbl_no]) 
			/ STAMP_BASE_POS_OFFSET * (y % STAMP_BASE_POS_OFFSET);
	}
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �|���S��ID���擾����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 *
 * @retval  �|���S��ID
 */
//--------------------------------------------------------------
static int Stamp_PolygonIDGet(STAMP_SYSTEM_WORK *ssw)
{
	int i, k;
	
	for(i = STAMP_POLYGON_ID_START; i < 32; i++){
		if((ssw->polygon_id_manage[0] & (1 << i)) == 0){
			ssw->polygon_id_manage[0] |= 1 << i;
			return i;
		}
	}
	k = 0;
	for( ; i < STAMP_POLYGON_ID_END; i++){
		if((ssw->polygon_id_manage[1] & (1 << k)) == 0){
			ssw->polygon_id_manage[1] |= 1 << k;
			return i;
		}
		k++;
	}
	OS_TPrintf("�|���S��ID�̊m�ۂɎ��s\n");
	return STAMP_POLYGON_ID_ERROR;
}

//--------------------------------------------------------------
/**
 * @brief   �|���S��ID�̉�����s��
 *
 * @param   ssw				�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   polygon_id		�|���S��ID
 */
//--------------------------------------------------------------
static void Stamp_PolygonIDFree(STAMP_SYSTEM_WORK *ssw, int polygon_id)
{
	if(polygon_id == STAMP_POLYGON_ID_ERROR){
		return;
	}
	
	if(polygon_id < 32){
		ssw->polygon_id_manage[0] &= 0xffffffff ^ (1 << polygon_id);
	}
	else{
		ssw->polygon_id_manage[1] &= 0xffffffff ^ (1 << (polygon_id - 32));
	}
}

//--------------------------------------------------------------
/**
 * @brief   �q�b�g�͈͂��쐬���܂��B
 *
 * @param   move			�X�^���v�ւ̃|�C���^
 * @param   rect			�q�b�g�͈͑����
 * @param   affine_flag		TRUE:�g�k�����Ă���BFALSE�ɂ���Ɗg�k���̌v�Z���Ȃ��ׁA�y���Ȃ�܂�
 */
//--------------------------------------------------------------
static void Stamp_HitRectCreate(D3DOBJ *obj, const STAMP_PARAM *param, STAMP_HIT_RECT *rect, int affine_flag)
{
	fx32 fx_x, fx_y, fx_z;
	fx32 scale_x, scale_y, scale_z;
	int hit_size;
	fx64 bairitu_x, bairitu_y, add_x, add_y;
	
	hit_size = FootprintTool_FootHitSizeGet(param->monsno, param->form_no);
	GF_ASSERT(hit_size < NELEMS(StampDefaultHitRect));
	
	D3DOBJ_GetMatrix(obj, &fx_x, &fx_y, &fx_z);

	rect->left = fx_x + StampDefaultHitRect[hit_size].left;
	rect->right = fx_x + StampDefaultHitRect[hit_size].right;
	rect->top = fx_y + StampDefaultHitRect[hit_size].top;
	rect->bottom = fx_y + StampDefaultHitRect[hit_size].bottom;
//	OS_TPrintf("hit_rect fx_x = %d, fx_y = %d\n", fx_x, fx_y);
//	OS_TPrintf("hit_rect normal left=%d, right=%d, top=%d, bottom=%d\n", rect->left, rect->right, rect->top, rect->bottom);
	if(affine_flag == TRUE){
		D3DOBJ_GetScale(obj, &scale_x, &scale_y, &scale_z);
		
		bairitu_x = ((fx64)scale_x) * 100 / FX32_ONE;
		add_x = (rect->right - rect->left) * bairitu_x / 100 - (rect->right - rect->left);
		rect->right += add_x / 2;
		rect->left += -(add_x / 2);
		
		bairitu_y = ((fx64)scale_y) * 100 / FX32_ONE;
		add_y = (rect->top - rect->bottom) * bairitu_y / 100 - (rect->top - rect->bottom);
		rect->top += add_y / 2;
		rect->bottom += -(add_y / 2);
//		OS_TPrintf("hit_rect affine left=%d, right=%d, top=%d, bottom=%d\n", rect->left, rect->right, rect->top, rect->bottom);
	}

#ifdef PM_DEBUG
	{
		int i;
		
		if(sys.cont & PAD_BUTTON_X){
			D3DOBJ_SetMatrix( &DebugStamp.obj[0], rect->left, rect->top, STAMP_POS_Z);
			D3DOBJ_SetMatrix( &DebugStamp.obj[1], rect->left, rect->bottom, STAMP_POS_Z);
			D3DOBJ_SetMatrix( &DebugStamp.obj[2], rect->right, rect->top, STAMP_POS_Z);
			D3DOBJ_SetMatrix( &DebugStamp.obj[3], rect->right, rect->bottom, STAMP_POS_Z);
			for(i = 0; i < 4; i++){
				D3DOBJ_SetDraw( &DebugStamp.obj[i], TRUE );
			}
		}
		else{
			for(i = 0; i < 4; i++){
				D3DOBJ_SetDraw( &DebugStamp.obj[i], FALSE );
			}
		}
	}
#endif
}

//--------------------------------------------------------------
/**
 * @brief   ��`���m�̓����蔻��`�F�b�N
 *
 * @param   rect0		��`�P
 * @param   rect1		��`�Q
 *
 * @retval  TRUE:�q�b�g�L��B�@FALSE:�q�b�g����
 */
//--------------------------------------------------------------
static BOOL Stamp_RectHitCheck(const STAMP_HIT_RECT *rect0, const STAMP_HIT_RECT *rect1)
{
	if( (rect0->left <= rect1->right) && (rect1->left <= rect0->right) &&
			(rect0->top >= rect1->bottom) && (rect1->top >= rect0->bottom)) {
		return TRUE;
	}else{
		return FALSE;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �A���Ǘ����[�N�ԍ����擾����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Stamp_ChainWorkGet(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	int i;
	STAMP_CHAIN_WORK *chain_work;
	
	chain_work = ssw->chain_work;
	for(i = 0; i < STAMP_MAX; i++){
		if(chain_work->stamp_num == 0){
			move->chain_work_no = i;
			chain_work->chain = 0;
			chain_work->stamp_num++;
			return;
		}
		chain_work++;
	}
	GF_ASSERT(0);	//�`�F�C�����[�N�̊m�ۂ��o���Ȃ�����
}

//--------------------------------------------------------------
/**
 * @brief   �A���Ǘ����[�N���Q�Ƃ��Ă���X�^���v�𑝂₷
 *
 * @param   ssw					�X�^���v�V�X�e���ւ̃|�C���^
 * @param   chain_work_no		�A�����[�N�Ǘ��ԍ�
 * @param   move_type			�A�����̓���^�C�v
 *
 * @retval  TRUE:�X�y�V�����G�t�F�N�g����
 * @retval  FALSE:�X�y�V�����G�t�F�N�g�͔������Ă��Ȃ�
 */
//--------------------------------------------------------------
static int Stamp_ChainAdd(STAMP_SYSTEM_WORK *ssw, int chain_work_no, int move_type)
{
	if(chain_work_no == CHAIN_WORK_NULL){
		return FALSE;
	}
	
	ssw->chain_work[chain_work_no].chain++;
	ssw->chain_work[chain_work_no].stamp_num++;
	OS_TPrintf("%d�A��! work_no = %d\n", ssw->chain_work[chain_work_no].chain, chain_work_no);
	
	if(ssw->special_effect_type == 0 && ssw->special_effect_req == 0 
			&& ssw->chain_work[chain_work_no].chain >= SpecialChainNum[ssw->player_max]){
		ssw->special_effect_req = move_type;
		MI_CpuClear8(&ssw->flash_work, sizeof(SPECIAL_FLASH_WORK));
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �A���Ǘ����[�N���Q�Ƃ��Ă���X�^���v�����炷
 *
 * @param   ssw					�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   chain_work_no		�A�����[�N�Ǘ��ԍ�
 */
//--------------------------------------------------------------
static void Stamp_ChainSub(STAMP_SYSTEM_WORK *ssw, int chain_work_no)
{
	int temp;
	
	if(chain_work_no == CHAIN_WORK_NULL){
		return;
	}
	
	//�X�^���v�͏����Ă��A���񐔂͎�������̂ŁA�Q�Ƃ��Ă���X�^���v������������
	temp = ssw->chain_work[chain_work_no].stamp_num;
	temp--;
	if(temp < 0){		//�O�̂���
		GF_ASSERT(0);
		temp = 0;
	}
	ssw->chain_work[chain_work_no].stamp_num = temp;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�̃p�����[�^�Z�b�g
 *
 * @param   ssw		�X�^���v�V�X�e���ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Special_EffectSet(STAMP_SYSTEM_WORK *ssw)
{
	MI_CpuClear8(&ssw->special_work, sizeof(STAMP_SPECIAL_WORK));
	ssw->special_effect_type = ssw->special_effect_req;
	ssw->special_effect_req = 0;
	ssw->special_effect_start_effect_end = 0;
}

//==============================================================================
//
//	����
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�^���v����^�C�v��ݒ肷��
 *
 * @param   move			�X�^���v�ւ̃|�C���^
 * @param   move_type		����^�C�v
 *
 * @retval  TRUE:�����B�@FALSE:���s
 */
//--------------------------------------------------------------
static BOOL StampMoveTool_MoveTypeSet(STAMP_MOVE_PTR move, int move_type)
{
	if(move->move_type != 0 || move->next_move_type != 0 || move->initialize == 0){
		return FALSE;	//���ɑ��̃G�t�F�N�g��������Ă���
	}
	move->next_move_type = move_type;
	MI_CpuClear8(&move->erase_eff, sizeof(ERASE_EFF_WORK));
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�p�����[�^���瓮��^�C�v�����o��
 * @param   param		�X�^���v�p�����[�^�ւ̃|�C���^
 * @retval  ����^�C�v
 */
//--------------------------------------------------------------
static u8 StampMoveTool_MoveTypeGet(const STAMP_PARAM *param)
{
	u8 seikaku;
	
	seikaku = PokeSeikakuGetRnd(param->personal_rnd);
	return Seikaku_to_MoveType_Tbl[seikaku];
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v��������F��莞�Ԍo������e�G�t�F�N�g����Ɉڍs
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_FirstWait(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_INIT_MOVE *init_move = &move->erase_eff.init_move;
	
	init_move->wait++;
	if(init_move->wait == 2){
		Stamp_HitRectCreate(&move->obj, &move->param, &init_move->rect, FALSE);
		D3DOBJ_SetDraw( &move->obj, TRUE );	//�e�N�X�`����VBlank�œ]������Ă���`��ON����
		move->initialize = TRUE;
	}

	if(init_move->wait > STAMP_FIRST_WAIT){
		Stamp_ChainWorkGet(ssw, move);
		StampMoveTool_MoveTypeSet(move, StampMoveTool_MoveTypeGet(&move->param));
	}
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F��莞�Ԍo������e�G�t�F�N�g����Ɉڍs
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_FirstWait(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F3�{���x�Ɋg�債�Ȃ���w�i�ɏ��X�ɓ��߂��������
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_NIJIMI *nijimi = &move->erase_eff.nijimi;
	
	switch(nijimi->seq){
	case 0:
		nijimi->alpha = 31 << 8;
		nijimi->polygon_id = Stamp_PolygonIDGet(ssw);
		nijimi->affine_xyz = FX32_ONE;
		nijimi->seq++;
		//break;
	case 1:
		nijimi->affine_xyz += NIJIMI_ADD_AFFINE;
		if(nijimi->alpha - NIJIMI_ADD_ALPHA < 0x100){
			D3DOBJ_SetDraw( &move->obj, FALSE);
			nijimi->seq++;
			break;
		}
		nijimi->alpha -= NIJIMI_ADD_ALPHA;
		D3DOBJ_SetScale(&move->obj, nijimi->affine_xyz, nijimi->affine_xyz, FX32_ONE);
		break;
	default:
		Stamp_PolygonIDFree(ssw, nijimi->polygon_id);
		return RET_DELETE;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &nijimi->rect, TRUE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��O�����F3�{���x�Ɋg�債�Ȃ���w�i�ɏ��X�ɓ��߂��������
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void StampDraw_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_NIJIMI *nijimi = &move->erase_eff.nijimi;

	if(nijimi->polygon_id != STAMP_POLYGON_ID_ERROR){
		NNS_G3dGlbPolygonAttr(0, 0, 0, nijimi->polygon_id, nijimi->alpha >> 8, 0);
		NNS_G3dMdlUseGlbPolygonID(move->mdl.pModel);
		NNS_G3dMdlUseGlbAlpha(move->mdl.pModel);
	}
	D3DOBJ_Draw(&move->obj);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F3�{���x�Ɋg�債�Ȃ���w�i�ɏ��X�ɓ��߂��������
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Nijimi(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_NIJIMI *nijimi = &move->erase_eff.nijimi;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&nijimi->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F���Ղ�4�������A4�̃I�u�W�F�N�g�ɂȂĎl���ɔ�юU���ď�����
 * 						�I�u�W�F�N�g�̈ړ��͈͂́A�������_����32�O���b�h���x
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_HAJIKE *hajike = &move->erase_eff.hajike;
	int i;
	fx32 fx_x, fx_y, fx_z;
	
	switch(hajike->seq){
	case 0:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < HAJIKE_OBJ_CHILD_NUM; i++){
			D3DOBJ_Init( &hajike->child_obj[i], &move->mdl );
			D3DOBJ_SetMatrix(&hajike->child_obj[i], fx_x, fx_y, fx_z);
		}
		hajike->seq++;
		//break;
	case 1:
		//�{�͍̂���
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		D3DOBJ_SetMatrix(&move->obj, fx_x - HAJIKE_ADD_X, fx_y - HAJIKE_ADD_Y, fx_z);
		//�E��
		D3DOBJ_GetMatrix(&hajike->child_obj[0], &fx_x, &fx_y, &fx_z);
		D3DOBJ_SetMatrix(&hajike->child_obj[0], fx_x + HAJIKE_ADD_X, fx_y - HAJIKE_ADD_Y, fx_z);
		//����
		D3DOBJ_GetMatrix(&hajike->child_obj[1], &fx_x, &fx_y, &fx_z);
		D3DOBJ_SetMatrix(&hajike->child_obj[1], fx_x - HAJIKE_ADD_X, fx_y + HAJIKE_ADD_Y, fx_z);
		//�E��
		D3DOBJ_GetMatrix(&hajike->child_obj[2], &fx_x, &fx_y, &fx_z);
		D3DOBJ_SetMatrix(&hajike->child_obj[2], fx_x + HAJIKE_ADD_X, fx_y + HAJIKE_ADD_Y, fx_z);
		
		hajike->frame++;
		if(hajike->frame > HAJIKE_DELETE_FRAME){
			return RET_DELETE;
		}
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &hajike->rect[0], FALSE);
	for(i = 0; i < HAJIKE_OBJ_CHILD_NUM; i++){
		Stamp_HitRectCreate(&hajike->child_obj[i], &move->param, &hajike->rect[1 + i], FALSE);
	}
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��O�����F���Ղ�4�������A4�̃I�u�W�F�N�g�ɂȂĎl���ɔ�юU���ď�����
 * 						�I�u�W�F�N�g�̈ړ��͈͂́A�������_����32�O���b�h���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void StampDraw_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_HAJIKE *hajike = &move->erase_eff.hajike;
	int i;
	
	for(i = 0; i < HAJIKE_OBJ_CHILD_NUM; i++){
		D3DOBJ_Draw(&hajike->child_obj[i]);
	}
	D3DOBJ_Draw(&move->obj);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F���Ղ�4�������A4�̃I�u�W�F�N�g�ɂȂĎl���ɔ�юU���ď�����
 * 						�I�u�W�F�N�g�̈ړ��͈͂́A�������_����32�O���b�h���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Hajike(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_HAJIKE *hajike = &move->erase_eff.hajike;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;
	int i;
	
	for(i = 0; i < HAJIKE_OBJ_CHILD_NUM + 1; i++){
		if(Stamp_RectHitCheck(&hajike->rect[i], &init_move->rect) == TRUE){
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_���琅��-90�x��]���A
 *						�����E�����Ɏ֍s�ړ����A�t���[���A�E�g����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Zigzag(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_ZIGZAG *zig = &move->erase_eff.zigzag;
	u16 rot, before_rot;
	fx32 fx_x, fx_y, fx_z;

	switch(zig->seq){
	case 0:
		rot = D3DOBJ_GetRota(&move->obj, D3DOBJ_ROTA_WAY_Z);
		rot -= 65536/4/ZIGZAG_THETA_FRAME;
		if(rot <= 65536/8*7){
			//Stamp_TexDotErase(move->mdl.pMdlTex, move->move_type);
			Stamp_TexDotFlip(move->mdl.pMdlTex, move->move_type);
			Stamp_LoadTexDataSet(ssw, move->mdl.pMdlTex);
			move->tex_load_req = TRUE;
			rot += 65536/4;
			zig->seq++;
		}
		D3DOBJ_SetRota(&move->obj, rot, D3DOBJ_ROTA_WAY_Z);
		break;
	case 1:
		move->tex_load_req = 0;
		rot = D3DOBJ_GetRota(&move->obj, D3DOBJ_ROTA_WAY_Z);
		rot -= 65536/4/ZIGZAG_THETA_FRAME;
		if(rot >= 65536/4*3){
			rot = 0;
			zig->seq++;
		}
		
		D3DOBJ_SetRota(&move->obj, rot, D3DOBJ_ROTA_WAY_Z);
		break;
	case 2:
		zig->wait++;
		if(zig->wait < ZIGZAG_THETA_AFTER_WAIT){
			break;
		}
		zig->wait = 0;
		zig->seq++;
		//break;
	case 3:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		if(fx_x < StampFrameOutRect.left || fx_x > StampFrameOutRect.right
				|| fx_y > StampFrameOutRect.top || fx_y < StampFrameOutRect.bottom){
			//OS_TPrintf("�W�O�U�O�F�t���[���A�E�g x=%d, y=%d\n", fx_x, fx_y);
			return RET_DELETE;
		}
		
		if(zig->turn_frame < ZIGZAG_TURN_FRAME){
			D3DOBJ_SetMatrix(&move->obj, fx_x + ZIGZAG_ADD_X, fx_y - ZIGZAG_ADD_Y, fx_z);
		}
		else{
			D3DOBJ_SetMatrix(&move->obj, fx_x + ZIGZAG_ADD_X, fx_y + ZIGZAG_ADD_Y, fx_z);
		}
		zig->turn_frame++;
		if(zig->turn_frame >= ZIGZAG_TURN_FRAME*2){
			zig->turn_frame = 0;
		}
		break;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &zig->rect, FALSE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_���琅��-90�x��]���A
 *						�����E�����Ɏ֍s�ړ����A�t���[���A�E�g����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Zigzag(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_ZIGZAG *zig = &move->erase_eff.zigzag;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&zig->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_���琅��90�x��]���A
 * 						�����������Ɏ֍s�ړ����A�t���[���A�E�g����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Dakou(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_DAKOU *dakou = &move->erase_eff.dakou;
	u16 rot;
	fx32 fx_x, fx_y, fx_z;
	fx32 offset_y;
	
	switch(dakou->seq){
	case 0:
		rot = D3DOBJ_GetRota(&move->obj, D3DOBJ_ROTA_WAY_Z);
		rot += 65536/4/DAKOU_THETA_FRAME;
		if(rot >= 65536/4/2){
			Stamp_TexDotFlip(move->mdl.pMdlTex, move->move_type);
			rot -= 65536/4;
			Stamp_LoadTexDataSet(ssw, move->mdl.pMdlTex);
			move->tex_load_req = TRUE;
			dakou->seq++;
		}
		D3DOBJ_SetRota(&move->obj, rot, D3DOBJ_ROTA_WAY_Z);
		break;
	case 1:
		move->tex_load_req = 0;
		rot = D3DOBJ_GetRota(&move->obj, D3DOBJ_ROTA_WAY_Z);
		rot += 65536/4/DAKOU_THETA_FRAME;
		if(rot >= 0 && rot < 65536/4*1){
			rot = 0;
			dakou->seq++;
		}
		D3DOBJ_SetRota(&move->obj, rot, D3DOBJ_ROTA_WAY_Z);
		break;
	case 2:
		dakou->wait++;
		if(dakou->wait < DAKOU_THETA_AFTER_WAIT){
			break;
		}
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		dakou->center_y = fx_y;
		dakou->wait = 0;
		dakou->seq++;
		//break;
	case 3:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		if(fx_x < StampFrameOutRect.left || fx_x > StampFrameOutRect.right
				|| fx_y > StampFrameOutRect.top || fx_y < StampFrameOutRect.bottom){
			//OS_TPrintf("�֍s�F�t���[���A�E�g x=%d, y=%d\n", fx_x, fx_y);
			return RET_DELETE;
		}
		
		dakou->theta += DAKOU_ADD_THETA;
		if(dakou->theta >= (360 << FX32_SHIFT)){
			dakou->theta -= 360 << FX32_SHIFT;
		}
		offset_y = FX_Mul(Sin360FX(dakou->theta), DAKOU_FURIHABA_Y);// - DAKOU_FURIHABA_Y/2;
		D3DOBJ_SetMatrix(&move->obj, fx_x - DAKOU_ADD_X, dakou->center_y + offset_y, fx_z);
		break;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &dakou->rect, FALSE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_���琅��90�x��]���A
 * 						�����������Ɏ֍s�ړ����A�t���[���A�E�g����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Dakou(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_DAKOU *dakou = &move->erase_eff.dakou;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&dakou->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_����A����������ɋO�Ղ��c���Ȃ���ړ��A�t���[���A�E�g����
 * 						�ړ����A���̃I�u�W�F�N�g�ƐڐG����Ƃ����ő���(+�O��)�͏�����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_KISEKI *kiseki = &move->erase_eff.kiseki;
	fx32 fx_x, fx_y, fx_z;
	int i;
	
	switch(kiseki->seq){
	case 0:
		kiseki->polygon_id = Stamp_PolygonIDGet(ssw);

		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < KISEKI_OBJ_CHILD_NUM; i++){
			D3DOBJ_Init( &kiseki->child_obj[i], &move->mdl );
			D3DOBJ_SetMatrix(&kiseki->child_obj[i], fx_x, fx_y, fx_z);
			D3DOBJ_SetDraw( &kiseki->child_obj[i], FALSE );
		}
		kiseki->seq++;
		//break;
	case 1:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		if(fx_x < StampFrameOutRect.left || fx_x > StampFrameOutRect.right
				|| fx_y > StampFrameOutRect.top || fx_y < StampFrameOutRect.bottom
				|| kiseki->obj_hit == TRUE){
			//OS_TPrintf("�O�ՁF�t���[���A�E�g x=%d, y=%d\n", fx_x, fx_y);
			for(i = 0; i < KISEKI_OBJ_CHILD_NUM; i++){
				D3DOBJ_SetDraw( &kiseki->child_obj[i], FALSE );
			}
			D3DOBJ_SetDraw( &move->obj, FALSE );
			kiseki->seq++;
			break;
		}

		D3DOBJ_SetMatrix(&move->obj, fx_x, fx_y + KISEKI_ADD_Y, fx_z);
		
		if(kiseki->frame % KISEKI_DROP_FRAME == 0){
			D3DOBJ_SetMatrix(&kiseki->child_obj[kiseki->drop_no], fx_x, fx_y + KISEKI_ADD_Y, fx_z);
			D3DOBJ_SetDraw( &kiseki->child_obj[kiseki->drop_no], TRUE );
			kiseki->drop_no++;
			if(kiseki->drop_no >= KISEKI_OBJ_CHILD_NUM){
				kiseki->drop_no = 0;
			}
		}
		kiseki->frame++;
		break;
	default:
		Stamp_PolygonIDFree(ssw, kiseki->polygon_id);
		return RET_DELETE;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &kiseki->rect, FALSE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��O�����F�^�b�`�����_����A����������ɋO�Ղ��c���Ȃ���ړ��A
 * 						�t���[���A�E�g����ړ����A���̃I�u�W�F�N�g�ƐڐG�����
 *						�����ő���(+�O��)�͏�����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void StampDraw_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_KISEKI *kiseki = &move->erase_eff.kiseki;
	int i;
	
	//�q
	if(kiseki->polygon_id != STAMP_POLYGON_ID_ERROR){
		NNS_G3dGlbPolygonAttr(0, 0, 0, kiseki->polygon_id, KISEKI_ALPHA, 0);
		NNS_G3dMdlUseGlbPolygonID(move->mdl.pModel);
		NNS_G3dMdlUseGlbAlpha(move->mdl.pModel);
	}
	for(i = 0; i < KISEKI_OBJ_CHILD_NUM; i++){
		D3DOBJ_Draw(&kiseki->child_obj[i]);
	}
	
	//�e	�������̔Z�x�����ɖ߂��Ă���`��
	if(kiseki->polygon_id != STAMP_POLYGON_ID_ERROR){
		NNS_G3dGlbPolygonAttr(0, 0, 0, kiseki->polygon_id, 31, 0);
		NNS_G3dMdlUseGlbPolygonID(move->mdl.pModel);
		NNS_G3dMdlUseGlbAlpha(move->mdl.pModel);
	}
	D3DOBJ_Draw(&move->obj);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_����A����������ɋO�Ղ��c���Ȃ���ړ��A
 * 						�t���[���A�E�g����ړ����A���̃I�u�W�F�N�g�ƐڐG�����
 *						�����ő���(+�O��)�͏�����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Kiseki(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_KISEKI *kiseki = &move->erase_eff.kiseki;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&kiseki->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_���琂���������ɑ��Ղ��A
 * 						���E�ɐU��Ȃ���ړ����t���[���A�E�g����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Yure(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_YURE *yure = &move->erase_eff.yure;
	u16 rot;
	fx32 fx_x, fx_y, fx_z;
	fx32 offset_x;
	
	switch(yure->seq){
	case 0:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		yure->center_x = fx_x;
		yure->seq++;
		//break;
	case 1:
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		if(fx_x < StampFrameOutRect.left || fx_x > StampFrameOutRect.right
				|| fx_y > StampFrameOutRect.top || fx_y < StampFrameOutRect.bottom){
			//OS_TPrintf("�h��F�t���[���A�E�g x=%d, y=%d\n", fx_x, fx_y);
			return RET_DELETE;
		}
		
		yure->theta += YURE_ADD_THETA;
		if(yure->theta >= (360 << FX32_SHIFT)){
			yure->theta -= 360 << FX32_SHIFT;
		}
		offset_x = FX_Mul(Sin360FX(yure->theta), YURE_FURIHABA_X);
		D3DOBJ_SetMatrix(&move->obj, yure->center_x + offset_x, fx_y - YURE_ADD_Y, fx_z);
		break;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &yure->rect, FALSE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_���琂���������ɑ��Ղ��A
 * 						���E�ɐU��Ȃ���ړ����t���[���A�E�g����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Yure(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_YURE *yure = &move->erase_eff.yure;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&yure->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F��C��4�{���x(64x64)�܂Ŋg�債�Ă����u�ŏ�����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Kakudai(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_KAKUDAI *kakudai = &move->erase_eff.kakudai;
	
	switch(kakudai->seq){
	case 0:
		kakudai->affine_xyz = FX32_ONE;
		kakudai->seq++;
		//break;
	case 1:
		kakudai->frame++;
		if(kakudai->frame > KAKUDAI_DELETE_FRAME){
			return RET_DELETE;
		}
		kakudai->affine_xyz += KAKUDAI_ADD_AFFINE;
		D3DOBJ_SetScale(&move->obj, kakudai->affine_xyz, kakudai->affine_xyz, FX32_ONE);
		break;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &kakudai->rect, TRUE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F��C��4�{���x(64x64)�܂Ŋg�債�Ă����u�ŏ�����
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Kakudai(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target)
{
	ERASE_EFF_KAKUDAI *kakudai = &move->erase_eff.kakudai;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&kakudai->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_�𒆐S�ɉ����Ƀu���[���Ȃ��珙�X�ɏ�����B�͈͂�3�{���x
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_BRAR_X *brar_x = &move->erase_eff.brar_x;
	fx32 fx_x, fx_y, fx_z;
	fx32 offset;
	int i;
	
	switch(brar_x->seq){
	case 0:
		brar_x->polygon_id = Stamp_PolygonIDGet(ssw);

		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < BRAR_X_OBJ_CHILD_NUM; i++){
			D3DOBJ_Init( &brar_x->child_obj[i], &move->mdl );
			D3DOBJ_SetMatrix(&brar_x->child_obj[i], fx_x, fx_y, fx_z);
			D3DOBJ_SetDraw( &brar_x->child_obj[i], TRUE );
		}
		brar_x->alpha = 31 << 8;
		brar_x->seq++;
		//break;
	case 1:
		//ALPHA
		if(brar_x->alpha - BRAR_X_ADD_ALPHA < 0x100){
			for(i = 0; i < BRAR_X_OBJ_CHILD_NUM; i++){
				D3DOBJ_SetDraw( &brar_x->child_obj[i], FALSE );
			}
			D3DOBJ_SetDraw( &move->obj, FALSE );
			brar_x->seq++;
			break;
		}
		brar_x->alpha -= BRAR_X_ADD_ALPHA;
		
		//SIN, THETA
		brar_x->theta += BRAR_X_ADD_THETA;
		if(brar_x->theta >= (360 << FX32_SHIFT)){
			brar_x->theta -= 360 << FX32_SHIFT;
		}
		offset = FX_Mul(Sin360FX(brar_x->theta), BRAR_X_FURIHABA);

		//POS SET
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < BRAR_X_OBJ_CHILD_NUM; i++){
			if(i & 1){
				D3DOBJ_SetMatrix(&brar_x->child_obj[i], fx_x + offset, fx_y, fx_z);
			}
			else{
				D3DOBJ_SetMatrix(&brar_x->child_obj[i], fx_x - offset, fx_y, fx_z);
			}
		}
		break;
	default:
		Stamp_PolygonIDFree(ssw, brar_x->polygon_id);
		return RET_DELETE;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &brar_x->rect[0], FALSE);
	for(i = 0; i < BRAR_X_OBJ_CHILD_NUM; i++){
		Stamp_HitRectCreate(&brar_x->child_obj[i], &move->param, &brar_x->rect[1 + i], FALSE);
	}
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��O�����F�^�b�`�����_�𒆐S�ɉ����Ƀu���[���Ȃ��珙�X�ɏ�����B�͈͂�3�{���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void StampDraw_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_BRAR_X *brar_x = &move->erase_eff.brar_x;
	int i;
	
	if(brar_x->polygon_id != STAMP_POLYGON_ID_ERROR){
		NNS_G3dGlbPolygonAttr(0, 0, 0, brar_x->polygon_id, brar_x->alpha >> 8, 0);
		NNS_G3dMdlUseGlbPolygonID(move->mdl.pModel);
		NNS_G3dMdlUseGlbAlpha(move->mdl.pModel);
	}
	
	for(i = 0; i < BRAR_X_OBJ_CHILD_NUM; i++){
		D3DOBJ_Draw(&brar_x->child_obj[i]);
	}
	D3DOBJ_Draw(&move->obj);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_�𒆐S�ɉ����Ƀu���[���Ȃ��珙�X�ɏ�����B
 *									�͈͂�3�{���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_BrarX(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_BRAR_X *brar_x = &move->erase_eff.brar_x;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;
	int i;
	
	for(i = 0; i < BRAR_X_OBJ_CHILD_NUM + 1; i++){
		if(Stamp_RectHitCheck(&brar_x->rect[i], &init_move->rect) == TRUE){
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_�𒆐S�ɏc���Ƀu���[���Ȃ��珙�X�ɏ�����B�͈͂�3�{���x
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_BRAR_Y *brar_y = &move->erase_eff.brar_y;
	fx32 fx_x, fx_y, fx_z;
	fx32 offset;
	int i;
	
	switch(brar_y->seq){
	case 0:
		brar_y->polygon_id = Stamp_PolygonIDGet(ssw);

		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM; i++){
			D3DOBJ_Init( &brar_y->child_obj[i], &move->mdl );
			D3DOBJ_SetMatrix(&brar_y->child_obj[i], fx_x, fx_y, fx_z);
			D3DOBJ_SetDraw( &brar_y->child_obj[i], TRUE );
		}
		brar_y->alpha = 31 << 8;
		brar_y->seq++;
		//break;
	case 1:
		//ALPHA
		if(brar_y->alpha - BRAR_Y_ADD_ALPHA < 0x100){
			for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM; i++){
				D3DOBJ_SetDraw( &brar_y->child_obj[i], FALSE );
			}
			D3DOBJ_SetDraw( &move->obj, FALSE );
			brar_y->seq++;
			break;
		}
		brar_y->alpha -= BRAR_Y_ADD_ALPHA;
		
		//SIN, THETA
		brar_y->theta += BRAR_Y_ADD_THETA;
		if(brar_y->theta >= (360 << FX32_SHIFT)){
			brar_y->theta -= 360 << FX32_SHIFT;
		}
		offset = FX_Mul(Sin360FX(brar_y->theta), BRAR_Y_FURIHABA);

		//POS SET
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM; i++){
			if(i & 1){
				D3DOBJ_SetMatrix(&brar_y->child_obj[i], fx_x, fx_y + offset, fx_z);
			}
			else{
				D3DOBJ_SetMatrix(&brar_y->child_obj[i], fx_x, fx_y - offset, fx_z);
			}
		}
		break;
	default:
		Stamp_PolygonIDFree(ssw, brar_y->polygon_id);
		return RET_DELETE;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &brar_y->rect[0], FALSE);
	for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM; i++){
		Stamp_HitRectCreate(&brar_y->child_obj[i], &move->param, &brar_y->rect[1 + i], FALSE);
	}
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�`��O�����F�^�b�`�����_�𒆐S�ɏc���Ƀu���[���Ȃ��珙�X�ɏ�����B�͈͂�3�{���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void StampDraw_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_BRAR_Y *brar_y = &move->erase_eff.brar_y;
	int i;
	
	if(brar_y->polygon_id != STAMP_POLYGON_ID_ERROR){
		NNS_G3dGlbPolygonAttr(0, 0, 0, brar_y->polygon_id, brar_y->alpha >> 8, 0);
		NNS_G3dMdlUseGlbPolygonID(move->mdl.pModel);
		NNS_G3dMdlUseGlbAlpha(move->mdl.pModel);
	}
	
	for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM; i++){
		D3DOBJ_Draw(&brar_y->child_obj[i]);
	}
	D3DOBJ_Draw(&move->obj);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_�𒆐S�ɏc���Ƀu���[���Ȃ��珙�X�ɏ�����B
 *									�͈͂�3�{���x
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_BrarY(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move, STAMP_MOVE_PTR target)
{
	ERASE_EFF_BRAR_Y *brar_y = &move->erase_eff.brar_y;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;
	int i;
	
	for(i = 0; i < BRAR_Y_OBJ_CHILD_NUM + 1; i++){
		if(Stamp_RectHitCheck(&brar_y->rect[i], &init_move->rect) == TRUE){
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v����F�^�b�`�����_���牺������64dot���x�g�債�ď�����B
 * 							�y���L�������悤�ȃC���[�W
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 *
 * @retval	�V�X�e���֕Ԃ��Ԏ�(RET_???)
 */
//--------------------------------------------------------------
static STAMP_RET StampMove_Tare(STAMP_SYSTEM_WORK *ssw, STAMP_MOVE_PTR move)
{
	ERASE_EFF_TARE *tare = &move->erase_eff.tare;
	fx32 fx_x, fx_y, fx_z;
	fx32 offset;
	
	switch(tare->seq){
	case 0:
		tare->affine_xyz = FX32_ONE;
		tare->seq++;
		//break;
	case 1:
		tare->frame++;
		if(tare->frame > TARE_DELETE_FRAME){
			return RET_DELETE;
		}
		tare->affine_xyz += TARE_ADD_AFFINE;
		D3DOBJ_SetScale(&move->obj, FX32_ONE, tare->affine_xyz, FX32_ONE);
		
	//	offset = 128 - (128 * scale_x / FX32_ONE);
		offset = -(FX32_ONE*2/2 * (tare->affine_xyz - FX32_ONE) / FX32_ONE);
		D3DOBJ_GetMatrix(&move->obj, &fx_x, &fx_y, &fx_z);
		D3DOBJ_SetMatrix(&move->obj, fx_x, fx_y + offset, fx_z);
		break;
	}

	Stamp_HitRectCreate(&move->obj, &move->param, &tare->rect, TRUE);
	return RET_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�^���v�q�b�g�`�F�b�N�F�^�b�`�����_���牺������64dot���x�g�債�ď�����B
 * 							�y���L�������悤�ȃC���[�W
 *
 * @param   ssw			�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   move		�X�^���v�ւ̃|�C���^
 * @param   target		�܂��m�[�}����Ԃ̃X�^���v�ւ̃|�C���^
 *
 * @retval  TRUE:�����蔻�蔭��
 * @retval  FALSE:�����蔻�薳��
 */
//--------------------------------------------------------------
static BOOL StampHitcheck_Tare(STAMP_SYSTEM_WORK *ssw,STAMP_MOVE_PTR move,STAMP_MOVE_PTR target)
{
	ERASE_EFF_TARE *tare = &move->erase_eff.tare;
	ERASE_EFF_INIT_MOVE *init_move = &target->erase_eff.init_move;

	if(Stamp_RectHitCheck(&tare->rect, &init_move->rect) == TRUE){
		return TRUE;
	}
	return FALSE;
}


//==============================================================================
//	�X�y�V�����G�t�F�N�g�����O�̃t���b�V��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�����O�̃t���b�V���G�t�F�N�g
 *
 * @param   ssw				�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   flash_work		�t���b�V�����[�N
 * @param   game_status		�Q�[���X�e�[�^�X
 * @param   board_type		�{�[�h�^�C�v
 *
 * @retval  TRUE:�G�t�F�N�g�I���B
 * @retval  FALSE:�G�t�F�N�g�p����
 */
//--------------------------------------------------------------
static BOOL SpecialFlashEff(STAMP_SYSTEM_WORK *ssw, SPECIAL_FLASH_WORK *flash_work, int game_status, int board_type)
{
	int white_black;
	
	if(game_status == FOOTPRINT_GAME_STATUS_FINISH){
		return FALSE;
	}
	
	if(board_type == FOOTPRINT_BOARD_TYPE_WHITE){
		white_black = -1;
	}
	else{
		white_black = 1;
	}
	
	switch(flash_work->seq){
	case 0:
		flash_work->evy += SPECIAL_FLASH_ADD_EVY;
		if(flash_work->evy >= SPECIAL_FLASH_EVY_MAX){
			flash_work->evy = SPECIAL_FLASH_EVY_MAX;
			flash_work->seq++;
		}
		ssw->v_wipe_req = TRUE;
		ssw->v_wipe_no = (flash_work->evy >> 8) * white_black;
		break;
	case 1:
		flash_work->evy -= SPECIAL_FLASH_ADD_EVY;
		if(flash_work->evy <= 0){
			flash_work->evy = 0;
			flash_work->loop++;
			if(flash_work->loop < SPECIAL_FLASH_LOOP){
				flash_work->seq = 0;
			}
			else{
				flash_work->seq++;
			}
		}
		ssw->v_wipe_req = TRUE;
		ssw->v_wipe_no = (flash_work->evy >> 8) * white_black;
		break;
	default:
		ssw->special_effect_start_effect_end = TRUE;
		return TRUE;
	}
	
	return FALSE;
}

//==============================================================================
//	�X�y�V�����G�t�F�N�g
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F�J�������O�[�b�Ɗ���Ė߂�
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Nijimi(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_NIJIMI *sp_nijimi = &sp->sp_nijimi;
	
	switch(sp_nijimi->seq){
	case 0:
		sp_nijimi->default_len = GFC_GetCameraDistance(camera_ptr);
		sp_nijimi->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_NIJIMI);
		//break;
	case 1:
		GFC_AddCameraDistance(SP_NIJIMI_FORWARD_ADD_LEN, camera_ptr);
		sp_nijimi->wait++;
		if(sp_nijimi->wait >= SP_NIJIMI_FORWARD_SYNC){
			sp_nijimi->wait = 0;
			sp_nijimi->seq++;
		}
		break;
	case 2:
		sp_nijimi->wait++;
		if(sp_nijimi->wait > SP_NIJIMI_WAIT_SYNC){
			sp_nijimi->wait = 0;
			sp_nijimi->seq++;
		}
		break;
	case 3:
		GFC_AddCameraDistance(SP_NIJIMI_BACK_ADD_LEN, camera_ptr);
		sp_nijimi->wait++;
		if(sp_nijimi->wait >= SP_NIJIMI_BACK_SYNC 
				|| GFC_GetCameraDistance(camera_ptr) >= sp_nijimi->default_len){
			GFC_SetCameraDistance(sp_nijimi->default_len, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F�J������]
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Hajike(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_HAJIKE *sp_hajike = &sp->sp_hajike;
	fx32 value;
	
	switch(sp_hajike->seq){
	case 0:
		Snd_SePlay(FOOTPRINT_SE_SP_HAJIKE);
		sp_hajike->seq++;
		//break;
	case 1:
		sp_hajike->count++;
		value = FX32_ONE*4 / SP_HAJIKE_ROTATION_SYNC * sp_hajike->count;
		
		// Y
		if(value < FX32_ONE*2){
			sp_hajike->up_vec.y = FX32_ONE - value;
		}
		else{
			sp_hajike->up_vec.y = -FX32_ONE + (value - FX32_ONE*2);
		}
		
		// X
		if(value < FX32_ONE*1){
			sp_hajike->up_vec.x = value;
		}
		else if(value < FX32_ONE*2){
			sp_hajike->up_vec.x = FX32_ONE - (value - FX32_ONE);
		}
		else if(value < FX32_ONE*3){
			sp_hajike->up_vec.x = -(value - FX32_ONE*2);
		}
		else{
			sp_hajike->up_vec.x = -FX32_ONE + (value - FX32_ONE*3);
		}
		
		if(sp_hajike->count >= SP_HAJIKE_ROTATION_SYNC){
			sp_hajike->up_vec.y = FX32_ONE;
			sp_hajike->up_vec.x = 0;
			sp_hajike->count = 0;
			sp_hajike->loop++;
			if(sp_hajike->loop >= SP_HAJIKE_ROTATION_NUM){
				sp_hajike->seq++;
			}
		}
		GFC_SetCamUp(&sp_hajike->up_vec, camera_ptr);
		break;
	default:
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F��ʑS�̂��E�ɌX��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Zigzag(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_ZIGZAG *sp_zigzag = &sp->sp_zigzag;
	CAMERA_ANGLE angle = {0,0,0,0};
	
	switch(sp_zigzag->seq){
	case 0:
		sp_zigzag->default_angle = GFC_GetCameraAngle(camera_ptr);
		sp_zigzag->calc_angle_y = sp_zigzag->default_angle.y;
		sp_zigzag->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_ZIGZAG);
		//break;
	case 1:
		angle.y += SP_ZIGZAG_ADD_ANGLE;
		sp_zigzag->calc_angle_y += SP_ZIGZAG_ADD_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_zigzag->calc_angle_y <= sp_zigzag->default_angle.y - SP_ZIGZAG_ANGLE_MAX){
			sp_zigzag->seq++;
		}
		break;
	case 2:	
		sp_zigzag->wait++;
		if(sp_zigzag->wait >= SP_ZIGZAG_WAIT){
			sp_zigzag->seq++;
		}
		break;
	case 3:
		angle.y += SP_ZIGZAG_SUB_ANGLE;
		sp_zigzag->calc_angle_y += SP_ZIGZAG_SUB_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_zigzag->calc_angle_y >= sp_zigzag->default_angle.y){
			GFC_SetCameraAngleRev(&sp_zigzag->default_angle, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F��ʑS�̂����ɌX��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Dakou(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_DAKOU *sp_dakou = &sp->sp_dakou;
	CAMERA_ANGLE angle = {0,0,0,0};
	
	switch(sp_dakou->seq){
	case 0:
		sp_dakou->default_angle = GFC_GetCameraAngle(camera_ptr);
		sp_dakou->calc_angle_y = sp_dakou->default_angle.y;
		sp_dakou->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_DAKOU);
		//break;
	case 1:
		angle.y += SP_DAKOU_ADD_ANGLE;
		sp_dakou->calc_angle_y += SP_DAKOU_ADD_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_dakou->calc_angle_y >= sp_dakou->default_angle.y + SP_DAKOU_ANGLE_MAX){
			sp_dakou->seq++;
		}
		break;
	case 2:	
		sp_dakou->wait++;
		if(sp_dakou->wait >= SP_DAKOU_WAIT){
			sp_dakou->seq++;
		}
		break;
	case 3:
		angle.y += SP_DAKOU_SUB_ANGLE;
		sp_dakou->calc_angle_y += SP_DAKOU_SUB_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_dakou->calc_angle_y <= sp_dakou->default_angle.y){
			GFC_SetCameraAngleRev(&sp_dakou->default_angle, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F��ʑS�̂���ɌX��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Kiseki(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_KISEKI *sp_kiseki = &sp->sp_kiseki;
	CAMERA_ANGLE angle = {0,0,0,0};
	
	switch(sp_kiseki->seq){
	case 0:
		sp_kiseki->default_angle = GFC_GetCameraAngle(camera_ptr);
		sp_kiseki->calc_angle_x = sp_kiseki->default_angle.x;
		Snd_SePlay(FOOTPRINT_SE_SP_KISEKI);
		sp_kiseki->seq++;
		//break;
	case 1:
		angle.x += SP_KISEKI_ADD_ANGLE;
		sp_kiseki->calc_angle_x += SP_KISEKI_ADD_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_kiseki->calc_angle_x >= sp_kiseki->default_angle.x + SP_KISEKI_ANGLE_MAX){
			sp_kiseki->seq++;
		}
		break;
	case 2:	
		sp_kiseki->wait++;
		if(sp_kiseki->wait >= SP_KISEKI_WAIT){
			sp_kiseki->seq++;
		}
		break;
	case 3:
		angle.x += SP_KISEKI_SUB_ANGLE;
		sp_kiseki->calc_angle_x += SP_KISEKI_SUB_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_kiseki->calc_angle_x <= sp_kiseki->default_angle.x){
			GFC_SetCameraAngleRev(&sp_kiseki->default_angle, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F���E�̓��������h��X�^���v�̓���������
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Yure(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_YURE *sp_yure = &sp->sp_yure;
	fx32 offset;
	CAMERA_ANGLE angle = {0,0,0,0};
	
	switch(sp_yure->seq){
	case 0:
		sp_yure->default_angle = GFC_GetCameraAngle(camera_ptr);
		sp_yure->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_YURE);
		//break;
	case 1:
		sp_yure->theta += SP_YURE_ADD_THETA;
		if(sp_yure->theta >= (360 << FX32_SHIFT)){
			sp_yure->theta -= 360 << FX32_SHIFT;
			sp_yure->loop++;
			if(sp_yure->loop >= SP_YURE_LOOP_NUM){
				sp_yure->seq++;
				sp_yure->theta = 0;
			}
		}
		offset = FX_Mul(Sin360FX(sp_yure->theta), SP_YURE_FURIHABA_X);

		angle = sp_yure->default_angle;
		angle.y = offset;
		GFC_SetCameraAngleRot(&angle, camera_ptr);
		break;
	default:
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F�J���������[���ƈ����Ė߂�
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Kakudai(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_KAKUDAI *sp_kakudai = &sp->sp_kakudai;
	
	switch(sp_kakudai->seq){
	case 0:
		sp_kakudai->default_len = GFC_GetCameraDistance(camera_ptr);
		sp_kakudai->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_KAKUDAI);
		//break;
	case 1:
		GFC_AddCameraDistance(SP_KAKUDAI_FORWARD_ADD_LEN, camera_ptr);
		sp_kakudai->wait++;
		if(sp_kakudai->wait >= SP_KAKUDAI_FORWARD_SYNC){
			sp_kakudai->wait = 0;
			sp_kakudai->seq++;
		}
		break;
	case 2:
		sp_kakudai->wait++;
		if(sp_kakudai->wait > SP_KAKUDAI_WAIT_SYNC){
			sp_kakudai->wait = 0;
			sp_kakudai->seq++;
		}
		break;
	case 3:
		GFC_AddCameraDistance(SP_KAKUDAI_BACK_ADD_LEN, camera_ptr);
		sp_kakudai->wait++;
		if(sp_kakudai->wait >= SP_KAKUDAI_BACK_SYNC 
				|| GFC_GetCameraDistance(camera_ptr) <= sp_kakudai->default_len){
			GFC_SetCameraDistance(sp_kakudai->default_len, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F�J�������X�^���v�Ɠ����悤�ɓ�����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_BrarX(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_BRAR_X *sp_brar_x = &sp->sp_brar_x;
	VecFx32 move = {0,0,0};
	fx32 offset_x;
	
	switch(sp_brar_x->seq){
	case 0:
		sp_brar_x->default_pos = GFC_GetCameraPos(camera_ptr);
		sp_brar_x->default_target = GFC_GetLookTarget(camera_ptr);
		sp_brar_x->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_BRAR_X);
		//break;
	case 1:
		sp_brar_x->theta += SP_BRAR_X_ADD_THETA;
		if(sp_brar_x->theta >= (360 << FX32_SHIFT)){
			sp_brar_x->theta -= 360 << FX32_SHIFT;
			sp_brar_x->loop++;
			if(sp_brar_x->loop >= SP_BRAR_X_LOOP_NUM){
				sp_brar_x->seq++;
				sp_brar_x->theta = 0;
			}
		}
		offset_x = FX_Mul(Sin360FX(sp_brar_x->theta), SP_BRAR_X_FURIHABA_X);

		move.x = offset_x;
		GFC_SetLookTarget(&sp_brar_x->default_target, camera_ptr);
		GFC_SetCameraPos(&sp_brar_x->default_pos, camera_ptr);
		GFC_ShiftCamera(&move, camera_ptr);
		break;
	default:
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F�J�������X�^���v�Ɠ����悤�ɓ�����
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_BrarY(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_BRAR_Y *sp_brar_y = &sp->sp_brar_y;
	VecFx32 move = {0,0,0};
	fx32 offset_y;
	
	switch(sp_brar_y->seq){
	case 0:
		sp_brar_y->default_pos = GFC_GetCameraPos(camera_ptr);
		sp_brar_y->default_target = GFC_GetLookTarget(camera_ptr);
		sp_brar_y->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_BRAR_Y);
		//break;
	case 1:
		sp_brar_y->theta += SP_BRAR_Y_ADD_THETA;
		if(sp_brar_y->theta >= (360 << FX32_SHIFT)){
			sp_brar_y->theta -= 360 << FX32_SHIFT;
			sp_brar_y->loop++;
			if(sp_brar_y->loop >= SP_BRAR_Y_LOOP_NUM){
				sp_brar_y->seq++;
				sp_brar_y->theta = 0;
			}
		}
		offset_y = FX_Mul(Sin360FX(sp_brar_y->theta), SP_BRAR_Y_FURIHABA_Y);

		move.y = offset_y;
		GFC_SetLookTarget(&sp_brar_y->default_target, camera_ptr);
		GFC_SetCameraPos(&sp_brar_y->default_pos, camera_ptr);
		GFC_ShiftCamera(&move, camera_ptr);
		break;
	default:
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�y�V�����G�t�F�N�g�F��ʑS�̂����ɌX��
 *
 * @param   ssw		�X�^���v�V�X�e�����[�N�ւ̃|�C���^
 * @param   sp		�X�y�V�������[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
static BOOL SpecialMove_Tare(STAMP_SYSTEM_WORK *ssw, STAMP_SPECIAL_WORK *sp, GF_CAMERA_PTR camera_ptr)
{
	SPECIAL_EFF_TARE *sp_tare = &sp->sp_tare;
	CAMERA_ANGLE angle = {0,0,0,0};
	
	switch(sp_tare->seq){
	case 0:
		sp_tare->default_angle = GFC_GetCameraAngle(camera_ptr);
		sp_tare->calc_angle_x = sp_tare->default_angle.x;
		sp_tare->seq++;
		Snd_SePlay(FOOTPRINT_SE_SP_TARE);
		//break;
	case 1:
		angle.x += SP_TARE_ADD_ANGLE;
		sp_tare->calc_angle_x += SP_TARE_ADD_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_tare->calc_angle_x <= sp_tare->default_angle.x - SP_TARE_ANGLE_MAX){
			sp_tare->seq++;
		}
		break;
	case 2:	
		sp_tare->wait++;
		if(sp_tare->wait >= SP_TARE_WAIT){
			sp_tare->seq++;
		}
		break;
	case 3:
		angle.x += SP_TARE_SUB_ANGLE;
		sp_tare->calc_angle_x += SP_TARE_SUB_ANGLE;
		GFC_AddCameraAngleRev(&angle, camera_ptr);
		
		if(sp_tare->calc_angle_x >= sp_tare->default_angle.x){
			GFC_SetCameraAngleRev(&sp_tare->default_angle, camera_ptr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

