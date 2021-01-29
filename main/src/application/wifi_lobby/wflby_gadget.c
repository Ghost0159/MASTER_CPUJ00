//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_gadget.c
 *	@brief		�K�W�F�b�g�A�j���[�V�����V�X�e��
 *	@author		tomoya takahashi
 *	@data		2008.01.21
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/arc_util.h"
#include "system/d3dobj.h"

#include "graphic/wifi_lobby_other.naix"

#include "wflby_gadget.h"
#include "wflby_3dmapobj.h"
#include "wflby_snd.h"
#include "wflby_3dmatrix.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
#ifdef PM_DEBUG
//#define WFLBY_DEBUG_GADGET_ALL_PUT	// R�������ƑS�����^�b�`�g�C��炷
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�K�W�F�b�g�I�u�W�F�f�[�^
//=====================================
enum {
	WFLBY_GADGET_OBJ_RIPPLE00,	// ���b�v����
	WFLBY_GADGET_OBJ_RIPPLE01,	// ���b�v����
	WFLBY_GADGET_OBJ_RIPPLE02,	// ���b�v����
	WFLBY_GADGET_OBJ_SIGNAL00,	// �V�O�i��
	WFLBY_GADGET_OBJ_SIGNAL01,	// �V�O�i��
	WFLBY_GADGET_OBJ_SIGNAL02,	// �V�O�i��
	WFLBY_GADGET_OBJ_SWING00,	// �X�E�B���O
	WFLBY_GADGET_OBJ_SWING01,	// �X�E�B���O
	WFLBY_GADGET_OBJ_SWING02,	// �X�E�B���O
	WFLBY_GADGET_OBJ_CRACLCER00,// �N���b�J�[
	WFLBY_GADGET_OBJ_CRACLCER01,// �N���b�J�[
	WFLBY_GADGET_OBJ_CRACLCER02,// �N���b�J�[
	WFLBY_GADGET_OBJ_FLASH00,	// �t���b�V��
	WFLBY_GADGET_OBJ_SPARKLE00,	// �X�p�[�N����
	WFLBY_GADGET_OBJ_SPARKLE01,	// �X�p�[�N����
	WFLBY_GADGET_OBJ_SPARKLE02,	// �X�p�[�N����
	WFLBY_GADGET_OBJ_BALLOON00,	// ���D��
	WFLBY_GADGET_OBJ_BALLOON01,	// ���D��
	WFLBY_GADGET_OBJ_BALLOON02,	// ���D��
	WFLBY_GADGET_OBJ_ONPU00_R,	// ����A
	WFLBY_GADGET_OBJ_ONPU01_R,	// ����B
	WFLBY_GADGET_OBJ_ONPU02_R,	// ����C
	WFLBY_GADGET_OBJ_ONPU03_R,	// ����D
	WFLBY_GADGET_OBJ_ONPU04_R,	// ����E
	WFLBY_GADGET_OBJ_ONPU05_R,	// ����F
	WFLBY_GADGET_OBJ_ONPU00_Y,	// ����A
	WFLBY_GADGET_OBJ_ONPU01_Y,	// ����B
	WFLBY_GADGET_OBJ_ONPU02_Y,	// ����C
	WFLBY_GADGET_OBJ_ONPU03_Y,	// ����D
	WFLBY_GADGET_OBJ_ONPU04_Y,	// ����E
	WFLBY_GADGET_OBJ_ONPU05_Y,	// ����F
	WFLBY_GADGET_OBJ_ONPU00_B,	// ����A
	WFLBY_GADGET_OBJ_ONPU01_B,	// ����B
	WFLBY_GADGET_OBJ_ONPU02_B,	// ����C
	WFLBY_GADGET_OBJ_ONPU03_B,	// ����D
	WFLBY_GADGET_OBJ_ONPU04_B,	// ����E
	WFLBY_GADGET_OBJ_ONPU05_B,	// ����F
	WFLBY_GADGET_OBJ_SPARK00,	// ���Ȃ��܍�
	WFLBY_GADGET_OBJ_SPARK01,	// ���Ȃ��܉E
	WFLBY_GADGET_OBJ_NUM,
} ;


//-------------------------------------
///	�K�W�F�b�g���f����
//=====================================
enum {
	WFLBY_GADGET_MDL_RIPPLER64x64,
	WFLBY_GADGET_MDL_SWING64x64,
	WFLBY_GADGET_MDL_16x16,
	WFLBY_GADGET_MDL_CRACKER32x32,
	WFLBY_GADGET_MDL_CRACKER0132x32,
	WFLBY_GADGET_MDL_CRACKER0232x32,
	WFLBY_GADGET_MDL_FLASH32x32,
	WFLBY_GADGET_MDL_SPARKLE_R32x32,
	WFLBY_GADGET_MDL_SPARKLE_Y32x32,
	WFLBY_GADGET_MDL_SPARKLE_B32x32,
	WFLBY_GADGET_MDL_BALLOON_R32x32,
	WFLBY_GADGET_MDL_BALLOON_Y32x32,
	WFLBY_GADGET_MDL_BALLOON_B32x32,
	WFLBY_GADGET_MDL_SPARKA16x16,
	WFLBY_GADGET_MDL_SPARKB16x16,
	WFLBY_GADGET_MDL_NUM,
} ;
#define WFLBY_GADGET_MDL_FILE_START		(NARC_wifi_lobby_other_ripple_r_nsbmd)

//-------------------------------------
///	�K�W�F�b�g�e�N�X�`����
//=====================================
enum {
	WFLBY_GADGET_TEX_RIPPLE0,
	WFLBY_GADGET_TEX_RIPPLE1,
	WFLBY_GADGET_TEX_RIPPLE2,
	WFLBY_GADGET_TEX_SIGNAL_R,
	WFLBY_GADGET_TEX_SIGNAL_Y,
	WFLBY_GADGET_TEX_SIGNAL_B,
	WFLBY_GADGET_TEX_SWING_R,
	WFLBY_GADGET_TEX_SWING_Y,
	WFLBY_GADGET_TEX_SWING_B,
	WFLBY_GADGET_TEX_CRACLCER_X,
	WFLBY_GADGET_TEX_CRACLCER_B,
	WFLBY_GADGET_TEX_CRACLCER_R,
	WFLBY_GADGET_TEX_FLASH,
	WFLBY_GADGET_TEX_SPARKLE_R,
	WFLBY_GADGET_TEX_SPARKLE_Y,
	WFLBY_GADGET_TEX_SPARKLE_B,
	WFLBY_GADGET_TEX_BALLOON_R,
	WFLBY_GADGET_TEX_BALLOON_Y,
	WFLBY_GADGET_TEX_BALLOON_B,
	WFLBY_GADGET_TEX_ONPU_A_R,
	WFLBY_GADGET_TEX_ONPU_B_R,
	WFLBY_GADGET_TEX_ONPU_C_R,
	WFLBY_GADGET_TEX_ONPU_D_R,
	WFLBY_GADGET_TEX_ONPU_E_R,
	WFLBY_GADGET_TEX_ONPU_F_R,
	WFLBY_GADGET_TEX_ONPU_A_Y,
	WFLBY_GADGET_TEX_ONPU_B_Y,
	WFLBY_GADGET_TEX_ONPU_C_Y,
	WFLBY_GADGET_TEX_ONPU_D_Y,
	WFLBY_GADGET_TEX_ONPU_E_Y,
	WFLBY_GADGET_TEX_ONPU_F_Y,
	WFLBY_GADGET_TEX_ONPU_A_B,
	WFLBY_GADGET_TEX_ONPU_B_B,
	WFLBY_GADGET_TEX_ONPU_C_B,
	WFLBY_GADGET_TEX_ONPU_D_B,
	WFLBY_GADGET_TEX_ONPU_E_B,
	WFLBY_GADGET_TEX_ONPU_F_B,
	WFLBY_GADGET_TEX_SPARK_A,
	WFLBY_GADGET_TEX_SPARK_B,
	WFLBY_GADGET_TEX_NUM,
} ;
#define WFLBY_GADGET_TEX_FILE_START		(NARC_wifi_lobby_other_ripple_r_nsbtx)

//-------------------------------------
///	�K�W�F�b�g�A�j����
//=====================================
enum {
	WFLBY_GADGET_ANM_RIPPLE1_ICA,
	WFLBY_GADGET_ANM_SIGNAL_ICA,
	WFLBY_GADGET_ANM_SIGNAL_IMA,
	WFLBY_GADGET_ANM_SWING_ICA,
	WFLBY_GADGET_ANM_CRACLCER_X,
	WFLBY_GADGET_ANM_CRACLCER_B,
	WFLBY_GADGET_ANM_CRACLCER_R,
	WFLBY_GADGET_ANM_FLASH,
	WFLBY_GADGET_ANM_SPARKLE_R,
	WFLBY_GADGET_ANM_SPARKLE_Y,
	WFLBY_GADGET_ANM_SPARKLE_B,
	WFLBY_GADGET_ANM_BALLOON_R,
	WFLBY_GADGET_ANM_BALLOON_Y,
	WFLBY_GADGET_ANM_BALLOON_B,
	WFLBY_GADGET_ANM_SPARK_A,
	WFLBY_GADGET_ANM_SPARK_B,
	WFLBY_GADGET_ANM_NUM,
	WFLBY_GADGET_ANM_NONE,
} ;
#define WFLBY_GADGET_ANM_FILE_START		(NARC_wifi_lobby_other_ripple1_nsbca)



//-------------------------------------
///	�RDMDL�I�u�W�FID�J�n�ԍ�
//=====================================
#define WFLBY_GADGET_3DMDL_OBJID	( 40 )






//-------------------------------------
///	�K�W�F�b�g�A�j���ő吔
//=====================================
#define WFLBY_GADGET_ANM_MAX	( 2 )

#define WFLBY_GADGET_ANM_SPEED	( FX32_ONE*2 )

//-------------------------------------
///	�P�K�W�F�b�g�œ����ɕ\������I�u�W�F�N�g�̍ő�l
//=====================================
#define WFLBY_GADGET_OBJ_MAX	( 18 )

//-------------------------------------
///	�K�W�F�b�g��Y����
//=====================================
#define WFLBY_GADGET_FLOOR_Y	( FX32_CONST(8) )	// ��
#define WFLBY_GADGET_AIR_Y		( FX32_CONST(16) )	// ��



//-------------------------------------
///	�N���b�J�[
//=====================================
enum{
	WFLBY_GADGET_CRACKEROBJ_FLASH,
	WFLBY_GADGET_CRACKEROBJ_CRACKER,
	WFLBY_GADGET_CRACKEROBJ_CRACKER01,
	WFLBY_GADGET_CRACKEROBJ_CRACKER02,

	WFLBY_GADGET_CRACKEROBJ_CRACKER_MAX = 3,
};
#define WFLBY_GADGET_CRACKER_DISX	( FX32_CONST(23) )
#define WFLBY_GADGET_CRACKER_DISZ	( FX32_CONST(28) )
#define WFLBY_GADGET_CRACKER_DISZUN	( FX32_CONST(24) )
#define WFLBY_GADGET_FLASH_DIS		( FX32_CONST(14) )
#define WFLBY_GADGET_FLASH_DISUN	( FX32_CONST(11) )
#define WFLBY_GADGET_FLASH_DISX		( FX32_CONST(7) )
#define WFLBY_GADGET_FLASH_Z		( FX32_CONST(8) )
#define WFLBY_GADGET_FLASH_Y		( FX32_CONST(16) )
#define WFLBY_GADGET_CRACKER_CENTER_X	( FX32_CONST(8) )
#define WFLBY_GADGET_CRACKER_ROT_ADD	( 40 )
#define WFLBY_GADGET_CRACKER_ADD_MOV	( FX32_CONST(8) )
#define WFLBY_GADGET_CRACKER_START_WAIT	( 3 )
#define WFLBY_GADGET_CRACKER_ALPHA_OUT_S	( 20 )
#define WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC	( 8 )



//-------------------------------------
///	���D
//=====================================
enum{
	WFLBY_GADGET_BALLOON_INIT,
	WFLBY_GADGET_BALLOON_UPMOVE,
	WFLBY_GADGET_BALLOON_DOWNMOVE,
};
enum{
	WFLBY_GADGET_BALLOON_OBJ_BL00,
	WFLBY_GADGET_BALLOON_OBJ_BL01,
	WFLBY_GADGET_BALLOON_OBJ_BL02,
	WFLBY_GADGET_BALLOON_OBJ_NUM,
};
enum{
	WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_WAIT,		// �o�^�o�^�J�n�܂�
	WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_START,	// �o�^�o�^�J�n
	WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_MAIN,		// �o�^�o�^���C��
	WFLBY_GADGET_BALLOON_DOWNSEQ_DOWN,			// ������
	WFLBY_GADGET_BALLOON_DOWNSEQ_DON_MAIN,		// �h�����C��
};
enum{
	WFLBY_GADGET_BALLOON_SEQ_UP,		// ���
	WFLBY_GADGET_BALLOON_SEQ_DOWN,		// ������
};

// �����̈ړ�����
static const u8	sc_WFLBY_GADGET_BALLOON_OBJ_UPDIS[ WFLBY_GADGET_BALLOON_OBJ_NUM ] = {
	48, 84,	128, 
};
#define WFLBY_GADGET_BALLOON_OBJ_UPSIDEDIS		( 8 )	// �h�ꕝ
#define WFLBY_GADGET_BALLOON_OBJ_UPROTSPEED		( 10*182 )// ��]�X�s�[�h
#define WFLBY_GADGET_BALLOON_OBJ_UP_SYNC		( 108 )	// �ړ��V���N��
#define WFLBY_GADGET_BALLOON_OBJ_DOWN_SYNC		( 10 )	// �ړ��V���N��
#define WFLBY_GADGET_BALLOON_OBJ_BATA_SWAIT		( 8 )	// �o�^�o�^����̂��n�܂�܂ł̃V���N��
#define WFLBY_GADGET_BALLOON_OBJ_BATA_WAIT		( 16 )	// �o�^�o�^����V���N��
#define WFLBY_GADGET_BALLOON_OBJ_DON_DIS		( FX32_CONST(8) )	//�@DON�ƈړ����鋗�� 
#define WFLBY_GADGET_BALLOON_OBJ_DON_SYNC		( 10 )	// �o�^�o�^����V���N��
static const WF2DMAP_POS sc_WFLBY_GADGET_BALLOON_OBJ_POS[ WFLBY_GADGET_BALLOON_OBJ_NUM ] = {
	{  2, 56 },		// ��
	{  14, 56 },	// �E
	{   8, 60 },	// �^��
};
#define WFLBY_GADGET_BALLOON_OBJ_POS_Z		( FX32_CONST(1) )
// �����̈ړ�����	��������̏ꍇ
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_UPDIS			( 2 )	// ��鋗��
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_UPSIDEDIS		( 0 )	// �h�ꕝ
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_UPROTSPEED	( 10*182 )// ��]�X�s�[�h
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_UP_SYNC		( 2 )	// �ړ��V���N��
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_DOWN_SYNC		( 1 )	// �ړ��V���N��
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_BATA_WAIT		( 8 )	// �o�^�o�^����V���N��

static const WF2DMAP_POS sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[ WFLBY_GADGET_BALLOON_OBJ_NUM ] = {
	{  2, 44 },		// ��
	{  14, 44 },	// �E
	{   8, 48 },	// �^��
};
#define WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z		( -FX32_CONST(8) )
// �K�W�F�b�g�A�j���萔
enum {
	WFLBY_GADGET_BALLOON_ANM_CENTER,
	WFLBY_GADGET_BALLOON_ANM_RIGHT,
	WFLBY_GADGET_BALLOON_ANM_LEFT,
	WFLBY_GADGET_BALLOON_ANM_OTHER,
	WFLBY_GADGET_BALLOON_ANM_CRASH,
	WFLBY_GADGET_BALLOON_ANM_NUM,
} ;
#define WFLBY_GADGET_BALLOON_ANM_WAIT	( 8 )
#define WFLBY_GADGET_BALLOON_ANM_ONEFR	( FX32_ONE*2 )

// ���D���ł����Ƃ��̒萔
#define WFLBY_GADGET_BALLOON_OBJ_WAIT	( WFLBY_GADGET_BALLOON_OBJ_DOWN_SYNC + WFLBY_GADGET_BALLOON_OBJ_BATA_WAIT )
#define WFLBY_GADGET_BALLOON_OBJ_MOVE_Y	( FX32_CONST( 64 ) )
#define WFLBY_GADGET_BALLOON_OBJ_MOVE_X	( FX32_CONST( 64 ) )
#define WFLBY_GADGET_BALLOON_OBJ_ROT_S	( 15*182 )
#define WFLBY_GADGET_BALLOON_OBJ_ROT_DIS ( FX32_CONST( 6 ) )



//-------------------------------------
///	�X�p�[�N��
//=====================================
#define WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM	(4)	// �P��̉�]�ŏo���I�u�W�F�̐�
#define WFLBY_GADGET_SPARKLE_1ROOP_MAX		(4)	// ��]�̍ő吔
#define WFLBY_GADGET_SPARKLE_OBJ_MAX		( WFLBY_GADGET_SPARKLE_1ROOP_MAX*WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM )
#define WFLBY_GADGET_SPARKLE_1ROOP_SYNC		(8)
#define WFLBY_GADGET_SPARKLE_OFFS			( 10*FX32_ONE )	// ���ӂԂ�����Ԉʒu
#define WFLBY_GADGET_SPARKLE_CENTER_X		( 8*FX32_ONE )	// ���ӂԂ�����Ԉʒu
#define WFLBY_GADGET_SPARKLE_CENTER_Z		( -8*FX32_ONE )	// ���ӂԂ�����Ԉʒu
#define WFLBY_GADGET_SPARKLE_ANM_SYNC		( 14 )			// �P���ӂԂ��A�j���V���N��
//  ��l������̏����ʒu
static const VecFx32 sc_WFLBY_GADGET_SPARKLE_OFFS[ WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM ] = {
	{ WFLBY_GADGET_SPARKLE_CENTER_X, WFLBY_GADGET_AIR_Y, WFLBY_GADGET_SPARKLE_CENTER_Z+WFLBY_GADGET_SPARKLE_OFFS },
	{ WFLBY_GADGET_SPARKLE_CENTER_X+WFLBY_GADGET_SPARKLE_OFFS, WFLBY_GADGET_AIR_Y, WFLBY_GADGET_SPARKLE_CENTER_Z },
	{ WFLBY_GADGET_SPARKLE_CENTER_X, WFLBY_GADGET_AIR_Y, WFLBY_GADGET_SPARKLE_CENTER_Z-WFLBY_GADGET_SPARKLE_OFFS },
	{ WFLBY_GADGET_SPARKLE_CENTER_X-WFLBY_GADGET_SPARKLE_OFFS, WFLBY_GADGET_AIR_Y, WFLBY_GADGET_SPARKLE_CENTER_Z },
};
//  ��l������̈ړ��l
#define WFLBY_GADGET_SPARKLE_MOVE_Y		( -FX32_CONST( 8 ) )	// Y�����ړ��l
#define WFLBY_GADGET_SPARKLE_MOVE_DIS	( FX32_CONST( 8 ) )	// ���ʈړ��l
static const VecFx32 sc_WFLBY_GADGET_SPARKLE_MOVE[ WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM ] = {
	{ 0, WFLBY_GADGET_SPARKLE_MOVE_Y, WFLBY_GADGET_SPARKLE_MOVE_DIS },
	{ WFLBY_GADGET_SPARKLE_MOVE_DIS, WFLBY_GADGET_SPARKLE_MOVE_Y, 0 },
	{ 0, WFLBY_GADGET_SPARKLE_MOVE_Y, -WFLBY_GADGET_SPARKLE_MOVE_DIS },
	{ -WFLBY_GADGET_SPARKLE_MOVE_DIS, WFLBY_GADGET_SPARKLE_MOVE_Y, 0 },
};



//-------------------------------------
///	���b�v��
//=====================================
#define WFLBY_GADGET_RIPPLE_NUM				( 3 )
#define WFLBY_GADGET_RIPPLE_ROOP_NUM		( 2 )
#define WFLBY_GADGET_RIPPLE_ROOP_START		( 6 )	// �������ː�
#define WFLBY_GADGET_RIPPLE_ROOP_SYNC		( 54 )	// ���ˊ����V���N��
#define WFLBY_GADGET_RIPPLE_MAT_Z			( FX32_CONST(6) )	// Z�I�t�Z�b�g���W



//-------------------------------------
///	�V�O�i��
//=====================================
//1�̃A�j���̒萔
#define WFLBY_GADGET_SIGNAL_ANM_MOVE_X			(FX32_CONST(0))
#define WFLBY_GADGET_SIGNAL_ANM_MOVE_Y			(FX32_CONST(0))
#define WFLBY_GADGET_SIGNAL_ANM_MOVE_Z			(FX32_CONST(0))
#define WFLBY_GADGET_SIGNAL_ANM_FRAME_NUM		(12)
#define WFLBY_GADGET_SIGNAL_ANM_SYNC			(48)
#define WFLBY_GADGET_SIGNAL_OBJ_NUM				(3)
enum{
	WFLBY_GADGET_SINGNAL_ANM_OFF,
	WFLBY_GADGET_SINGNAL_ANM_DRAW00,
	WFLBY_GADGET_SINGNAL_ANM_DRAW01,
};
typedef struct{
	u16 objidx;
	u16 anm_data;
} WFLBY_GADGET_SINGNAL_ANMDATA;
static const WFLBY_GADGET_SINGNAL_ANMDATA sc_WFLBY_GADGET_SIGNAL_ANMSEQ[3][WFLBY_GADGET_SIGNAL_ANM_FRAME_NUM] = {
	{
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
	},
	{
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
	},
	{
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 2, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 2, WFLBY_GADGET_SINGNAL_ANM_OFF,    },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 2, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 0, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
		{ 1, WFLBY_GADGET_SINGNAL_ANM_DRAW01, },
		{ 2, WFLBY_GADGET_SINGNAL_ANM_DRAW00, },
	},
};



//-------------------------------------
///	�X�E�B���O
//=====================================
#define WFLBY_GADGET_SWING_ANM_SYNC		( 32 )
#define	WFLBY_GADGET_SWING_OBJ_NUM		( 3 )



//-------------------------------------
///	��������
//=====================================
typedef enum{
	WFLBY_GADGET_ONPU_MAIN_PLAY,		// �Đ���
	WFLBY_GADGET_ONPU_MAIN_START,		// �Đ����ŉ����𔭎˂���
	WFLBY_GADGET_ONPU_MAIN_ALLEND,		// �S���I�����
} WFLBY_GADGET_ONPU_MAIN_RET;
enum{
	WFLBY_GADGET_ONPU_MOVE_BELL,	// �����x������
	WFLBY_GADGET_ONPU_MOVE_DRUM,	// �����h��������
	WFLBY_GADGET_ONPU_MOVE_CYMBALS,	// �����V���o������
	WFLBY_GADGET_ONPU_MOVE_NUM,		//	��������^�C�v��
};
#define WFLBY_GADGET_ONPU_OBJWK_NUM	( 16 )
// �����\���f�[�^
static const u8 sc_WFLBY_LEVEL_00ADDOBJ[ WFLBY_GADGET_ONPU_OBJWK_NUM ] = {
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
};
static const u8 sc_WFLBY_LEVEL_01ADDOBJ[ WFLBY_GADGET_ONPU_OBJWK_NUM ] = {
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU02_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU03_Y,
	WFLBY_GADGET_OBJ_ONPU04_Y,
	WFLBY_GADGET_OBJ_ONPU02_Y,
	WFLBY_GADGET_OBJ_ONPU00_Y,
	WFLBY_GADGET_OBJ_ONPU00_Y,
	WFLBY_GADGET_OBJ_ONPU04_Y,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU04_Y,
	WFLBY_GADGET_OBJ_ONPU02_Y,
	WFLBY_GADGET_OBJ_ONPU04_R,
	WFLBY_GADGET_OBJ_ONPU00_R,
};
static const u8 sc_WFLBY_LEVEL_02ADDOBJ[ WFLBY_GADGET_ONPU_OBJWK_NUM ] = {
	WFLBY_GADGET_OBJ_ONPU00_R,
	WFLBY_GADGET_OBJ_ONPU01_R,
	WFLBY_GADGET_OBJ_ONPU00_B,
	WFLBY_GADGET_OBJ_ONPU04_B,
	WFLBY_GADGET_OBJ_ONPU01_B,
	WFLBY_GADGET_OBJ_ONPU03_B,
	WFLBY_GADGET_OBJ_ONPU02_Y,
	WFLBY_GADGET_OBJ_ONPU05_Y,
	WFLBY_GADGET_OBJ_ONPU05_B,
	WFLBY_GADGET_OBJ_ONPU04_Y,
	WFLBY_GADGET_OBJ_ONPU05_R,
	WFLBY_GADGET_OBJ_ONPU02_B,
	WFLBY_GADGET_OBJ_ONPU05_Y,
	WFLBY_GADGET_OBJ_ONPU02_R,
	WFLBY_GADGET_OBJ_ONPU03_B,
	WFLBY_GADGET_OBJ_ONPU00_B,
};
// ����f�[�^
#define WFLBY_GADGET_ONPU_OBJ_NUM		( 4 )
#define WFLBY_GADGET_ONPU_X_OFS			( FX32_CONST(8) )
#define WFLBY_GADGET_ONPU_Z_OFS			( -FX32_CONST(8) )
//  �x������
#define WFLBY_GADGET_ONPU_MOVE_BELL_SYNC	( 18 )
#define WFLBY_GADGET_ONPU_MOVE_BELL_Y		( FX32_CONST(16) )
#define WFLBY_GADGET_ONPU_MOVE_BELL_X0		( FX32_CONST(8) )
#define WFLBY_GADGET_ONPU_MOVE_BELL_X1		( FX32_CONST(12) )
#define WFLBY_GADGET_ONPU_MOVE_BELL_Z		( FX32_CONST(6) )
#define WFLBY_GADGET_ONPU_MOVE_BELL_CVX		( FX32_CONST(6) )
#define WFLBY_GADGET_ONPU_MOVE_BELL_CVSP	( FX_GET_ROTA_NUM(18) )
// �h��������
#define WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SYNC	( 14 )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_UP_SYNC		( 8 )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_XZ_SYNC		( 19 )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_SYNC		( WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SYNC+WFLBY_GADGET_ONPU_MOVE_DRUM_UP_SYNC )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_Y		( FX32_CONST(32) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_UP_Y		( FX32_CONST(12) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_ROT	( FX_GET_ROTA_NUM(30) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SP		( FX_GET_ROTA_NUM(10) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_UP_ROT		( FX_GET_ROTA_NUM(0) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_UP_SP		( FX_GET_ROTA_NUM(20) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_X			( FX32_CONST(16) )
#define WFLBY_GADGET_ONPU_MOVE_DRUM_Z			( FX32_CONST(16) )
// �V���o��
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ_NUM			(3)
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_START_Y			( FX32_CONST(4) )

#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_SYNC		( 8 )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_Y00		( FX32_CONST(5) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_Y01		( FX32_CONST(14) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_X00		( FX32_CONST(13) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_X01		( FX32_CONST(7) )

#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_SYNC		( 3 )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_Y00		( FX32_CONST(4) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_Y01		( FX32_CONST(3) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_X00		( -FX32_CONST(3) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_X01		( -FX32_CONST(3) )

#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_SYNC		( 4 )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_Y00		( FX32_CONST(5) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_Y01		( FX32_CONST(10) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_X00		( FX32_CONST(10) )
#define WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_X01		( FX32_CONST(6) )



//-------------------------------------
///	��������
//=====================================
#define WFLBY_GADGET_MOVE_ONPU_WK_NUM		(4)
typedef struct {
	u16 count;
	u8	anmidx;
	u8	mvnum;
} WFLBY_GADGET_MOVE_ONPU_DATA;
// �x��
#define WFLBY_GADGET_MOVE_ONPU_BELL00_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL00[]={
	{ 0, 0, 2 },
	{ 24, 2, 2 },
};
#define WFLBY_GADGET_MOVE_ONPU_BELL01_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL01[]={
	{ 0, 0, 3 },
//	{ 11, 1, 3 },
	{ 24, 2, 3 },
//	{ 32, 3, 3 },
};
#define WFLBY_GADGET_MOVE_ONPU_BELL02_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL02[]={
	{ 0, 0, 4 },
//	{ 8, 1, 4 },
	{ 16, 2, 4 },
//	{ 24, 3, 4 },
	{ 32, 0, 4 },
};
// �h����
#define WFLBY_GADGET_MOVE_ONPU_DRUM00_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM00[]={
	{ 0, 0, 2 },
//	{ 8, 1, 2 },
	{ 24, 2, 2 },
//	{ 32, 3, 2 },
};
#define WFLBY_GADGET_MOVE_ONPU_DRUM01_SYSNC		(41)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM01[]={
	{ 0, 0, 3 },
//	{ 8, 1, 3 },
	{ 16, 2, 3 },
//	{ 24, 3, 3 },
	{ 40, 0, 3 },
};
#define WFLBY_GADGET_MOVE_ONPU_DRUM02_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM02[]={
	{ 0, 0, 4 },
//	{ 8, 1, 4 },
	{ 16, 2, 4 },
//	{ 26, 3, 4 },
	{ 35, 0, 4 },
};

// �V���o��
#define WFLBY_GADGET_MOVE_ONPU_CYMBALS00_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS00[]={
	{ 0, 0, 2 },
	{ 24, 1, 2 },
//	{ 32, 2, 2 },
};
#define WFLBY_GADGET_MOVE_ONPU_CYMBALS01_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS01[]={
	{ 0, 0, 2 },
//	{ 8, 1, 2 },
	{ 24, 2, 2 },
//	{ 32, 3, 2 },
};
#define WFLBY_GADGET_MOVE_ONPU_CYMBALS02_SYSNC		(33)
static const WFLBY_GADGET_MOVE_ONPU_DATA sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS02[]={
	{ 0, 0, 4 },
//	{ 4, 1, 4 },
	{ 20, 2, 4 },
//	{ 30, 3, 4 },
};


//-------------------------------------
///	�V���o����ȓ���
//=====================================
enum{
	WFLBY_GADGET_CYMBALS_INAZUMA_00 = 16,
	WFLBY_GADGET_CYMBALS_INAZUMA_01,
	WFLBY_GADGET_CYMBALS_INAZUMA_NUM = 2,
};
#define WFLBY_GADGET_CYMBALS_INAZUMA_OFS_X		(-FX32_CONST(4))
#define WFLBY_GADGET_CYMBALS_INAZUMA_DIS_X		(FX32_CONST(24))
#define WFLBY_GADGET_CYMBALS_INAZUMA_OFS_Z		(-FX32_CONST(10))
#define WFLBY_GADGET_CYMBALS_INAZUMA_OFS_Y		(-FX32_CONST(0))
#define WFLBY_GADGET_CYMBALS_INAZUMA_ANM_SYNC	(13)


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�ėp����V�X�e��
//	�����ړ�
//=====================================
typedef struct {
	s32	count_max;
	fx32 x;
	fx32 y;
	fx32 z;
	fx32 dis_x;
	fx32 start_x;
	fx32 dis_y;
	fx32 start_y;
	fx32 dis_z;
	fx32 start_z;
} WFLBY_GADGET_MV_STRAIGHT;


//-------------------------------------
///	�ėp����V�X�e��
//	Sin�J�[�u�ړ�
//=====================================
typedef struct {
	u16		rota_num;
	u16		speed;
	fx32	dis;
	fx32	num;
} WFLBY_GADGET_MV_SINCURVE;



//-------------------------------------
///	�K�W�F�b�g���\�[�X�\���f�[�^
//	4byte
//=====================================
typedef struct {
	u8 mdl;
	u8 tex;
	u8 anm[ WFLBY_GADGET_ANM_MAX ];
} WFLBY_GADGET_RES;


//-------------------------------------
///	�������샏�[�N
//	4�������O���[�v������œ��삷��
//=====================================
typedef struct {
	s16 count;
	u16	type;
	u16 mvnum;
	u8	move;
	u8	seq;
	WFLBY_GADGET_MV_STRAIGHT	st[WFLBY_GADGET_ONPU_OBJ_NUM];
	WFLBY_GADGET_MV_SINCURVE	cv[WFLBY_GADGET_ONPU_OBJ_NUM];
	D3DOBJ*	p_obj[WFLBY_GADGET_ONPU_OBJ_NUM];
	const WFLBY_3DPERSON* cp_person;
} WFLBY_GADGET_ONPU;




//-------------------------------------
///	�`��I�u�W�F���샏�[�N
//=====================================
typedef union {
	struct{
		s8 bl_seq;
		// ���D�̂���
		s8	balloon_num;
		s16 count;
		// DON�A�j���p
		fx32 don_start_y;
		// �A�j���E�G�C�g
		u8 anm_wait[ WFLBY_GADGET_BALLOON_OBJ_NUM ];
		u8 pad0;
		u8 anmidx[ WFLBY_GADGET_BALLOON_OBJ_NUM ];
		u8 pad1;
		// ���D��s�J�E���^
		u8 balloon_fly_count;
		// ��ɉ����̂悤�Ȃ��̂�����̂��t���O
		u8 roof_flag;
		u8 pad2[2];
		// ���D����
		WFLBY_GADGET_MV_SINCURVE	sincurve[WFLBY_GADGET_BALLOON_OBJ_NUM];
		WFLBY_GADGET_MV_STRAIGHT	straight[WFLBY_GADGET_BALLOON_OBJ_NUM];
		// ��l������
		WFLBY_GADGET_MV_STRAIGHT	straightobj;
		WFLBY_GADGET_MV_SINCURVE	sincurveobj;
	} balloon;

	struct{
		s32 wait;	
		s32 alpha_count[ WFLBY_GADGET_CRACKEROBJ_CRACKER_MAX ];
	} cracker;

	struct {
		s8 count;			// ����J�E���^
		s8 roop_count;		// ���[�v���J�E���^
		u8 roop_num;		// �S�̃��[�v��
		u8 buff_num;		// �I�u�W�F���쐔

		// ��������
		s8							objcount[WFLBY_GADGET_SPARKLE_OBJ_MAX];
		WFLBY_GADGET_MV_STRAIGHT	objstraight[WFLBY_GADGET_SPARKLE_OBJ_MAX];
	} sparkle;

	struct {
		s8 ripple_num;
		s8 move_num;
		s8 pad[2];
	} ripple;

	struct{
		s32	lastnum;
	} signal;

	struct{
		s16	lastnum;
		s16	objnum;
	} swing;

	struct{
		WFLBY_GADGET_ONPU move[WFLBY_GADGET_MOVE_ONPU_WK_NUM];
		const WFLBY_GADGET_MOVE_ONPU_DATA*	cp_data;
		u16									data_num;
		u16									count_max;

		// �Ž�ϗp
		u16 inazuma_move;
		u16	inazuma_count;
	} onpu;
} WFLBY_GADGET_OBJWK;


//-------------------------------------
///	�`��I�u�W�F
//=====================================
typedef struct {
	u8			gadget_type;				// �K�W�F�b�g�^�C�v
	u8			seq;						// �V�[�P���X
	s16			count;						// �J�E���^
	u32			mdl_objid;					// ���f���ɐݒ肷��I�u�W�FID

	// �I�u�W�F�f�[�^
	WFLBY_3DPERSON* p_person;

	
	//-------------------------------------
	///	���ꂼ��̏������֐��Őݒ肷��f�[�^
	//=====================================
	// �`��I�u�W�F
	D3DOBJ		obj[ WFLBY_GADGET_OBJ_MAX ];
	// ���쒆�̃I�u�W�F�f�[�^
	const WFLBY_GADGET_RES*	cp_objres[ WFLBY_GADGET_OBJ_MAX ];
	// �A�j���[�V�����t���[��
	fx32		anm_frame[ WFLBY_GADGET_OBJ_MAX ][ WFLBY_GADGET_ANM_MAX ];	
	// ���ꂼ��̓���p�̃��[�N
	WFLBY_GADGET_OBJWK	mvwk;
} WFLBY_GADGET_OBJ;


//-------------------------------------
///	�K�W�F�b�g�V�X�e��
//=====================================
typedef struct _WFLBY_GADGET{
	WFLBY_SYSTEM*		p_system;	// �V�X�e��
	WFLBY_3DOBJCONT*	p_objcont;	// �I�u�W�F
	WFLBY_CAMERA*		p_camera;	// �J�����I�u�W�F
	WFLBY_MAPCONT*		p_mapcont;	// �}�b�v�Ǘ��V�X�e��
	
	// �K�W�F�b�g����I�u�W�F
	WFLBY_GADGET_OBJ	obj[ WFLBY_PLAYER_MAX ];	


	// �e���\�[�X
	D3DOBJ_MDL	mdl[ WFLBY_GADGET_MDL_NUM ];		// ���f��
	void*		p_texres[ WFLBY_GADGET_TEX_NUM ];	// �e�N�X�`��
	D3DOBJ_ANM	anm[ WFLBY_GADGET_ANM_NUM ];		// �A�j��

	// �A���P�[�^
	NNSFndAllocator		allocator;
	
} WFLBY_GADGET;



//-----------------------------------------------------------------------------
/**
 *			�K�W�F�b�g�\���f�[�^
 */
//-----------------------------------------------------------------------------
static const WFLBY_GADGET_RES sc_WFLBY_GADGET_RES[ WFLBY_GADGET_OBJ_NUM ] = {
	// ���b�v��
	{
		WFLBY_GADGET_MDL_RIPPLER64x64,
		WFLBY_GADGET_TEX_RIPPLE0,
		{
			WFLBY_GADGET_ANM_RIPPLE1_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// ���b�v��
	{
		WFLBY_GADGET_MDL_RIPPLER64x64,
		WFLBY_GADGET_TEX_RIPPLE1,
		{
			WFLBY_GADGET_ANM_RIPPLE1_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// ���b�v��
	{
		WFLBY_GADGET_MDL_RIPPLER64x64,
		WFLBY_GADGET_TEX_RIPPLE2,
		{
			WFLBY_GADGET_ANM_RIPPLE1_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// �V�O�i��
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SIGNAL_R,
		{
			WFLBY_GADGET_ANM_SIGNAL_ICA,
			WFLBY_GADGET_ANM_SIGNAL_IMA,
		},
	},

	// �V�O�i��
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SIGNAL_Y,
		{
			WFLBY_GADGET_ANM_SIGNAL_ICA,
			WFLBY_GADGET_ANM_SIGNAL_IMA,
		},
	},

	// �V�O�i��
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SIGNAL_B,
		{
			WFLBY_GADGET_ANM_SIGNAL_ICA,
			WFLBY_GADGET_ANM_SIGNAL_IMA,
		},
	},

	// �X�C���O
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SWING_R,
		{
			WFLBY_GADGET_ANM_SWING_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// �X�C���O
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SWING_Y,
		{
			WFLBY_GADGET_ANM_SWING_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// �X�C���O
	{
		WFLBY_GADGET_MDL_SWING64x64,
		WFLBY_GADGET_TEX_SWING_B,
		{
			WFLBY_GADGET_ANM_SWING_ICA,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �N���b�J�[00
	{
		WFLBY_GADGET_MDL_CRACKER0232x32,
		WFLBY_GADGET_TEX_CRACLCER_R,
		{
			WFLBY_GADGET_ANM_CRACLCER_R,
			WFLBY_GADGET_ANM_NONE,
		},
	},


	//  �N���b�J�[01
	{
		WFLBY_GADGET_MDL_CRACKER32x32,
		WFLBY_GADGET_TEX_CRACLCER_X,
		{
			WFLBY_GADGET_ANM_CRACLCER_X,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �N���b�J�[02
	{
		WFLBY_GADGET_MDL_CRACKER0132x32,
		WFLBY_GADGET_TEX_CRACLCER_B,
		{
			WFLBY_GADGET_ANM_CRACLCER_B,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �t���b�V��
	{
		WFLBY_GADGET_MDL_FLASH32x32,
		WFLBY_GADGET_TEX_FLASH,
		{
			WFLBY_GADGET_ANM_FLASH,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �X�p�[�N��WFLBY_GADGET_OBJ_SPARKLE00
	{
		WFLBY_GADGET_MDL_SPARKLE_R32x32,
		WFLBY_GADGET_TEX_SPARKLE_R,
		{
			WFLBY_GADGET_ANM_SPARKLE_R,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �X�p�[�N��WFLBY_GADGET_OBJ_SPARKLE01
	{
		WFLBY_GADGET_MDL_SPARKLE_Y32x32,
		WFLBY_GADGET_TEX_SPARKLE_Y,
		{
			WFLBY_GADGET_ANM_SPARKLE_Y,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �X�p�[�N��WFLBY_GADGET_OBJ_SPARKLE02
	{
		WFLBY_GADGET_MDL_SPARKLE_B32x32,
		WFLBY_GADGET_TEX_SPARKLE_B,
		{
			WFLBY_GADGET_ANM_SPARKLE_B,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �o���[����
	{
		WFLBY_GADGET_MDL_BALLOON_R32x32,
		WFLBY_GADGET_TEX_BALLOON_R,
		{
			WFLBY_GADGET_ANM_BALLOON_R,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �o���[�����F
	{
		WFLBY_GADGET_MDL_BALLOON_Y32x32,
		WFLBY_GADGET_TEX_BALLOON_Y,
		{
			WFLBY_GADGET_ANM_BALLOON_Y,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  �o���[����
	{
		WFLBY_GADGET_MDL_BALLOON_B32x32,
		WFLBY_GADGET_TEX_BALLOON_B,
		{
			WFLBY_GADGET_ANM_BALLOON_B,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����A	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_A_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����B	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_B_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����C	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_C_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����D	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_D_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����E	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_E_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����F	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_F_R,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����A	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_A_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����B	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_B_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����C	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_C_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����D	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_D_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����E	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_E_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����F	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_F_Y,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����A	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_A_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����B	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_B_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����C	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_C_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����D	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_D_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����E	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_E_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	//  ����F	��
	{
		WFLBY_GADGET_MDL_16x16,
		WFLBY_GADGET_TEX_ONPU_F_B,
		{
			WFLBY_GADGET_ANM_NONE,
			WFLBY_GADGET_ANM_NONE,
		},
	},


	// ���Ȃ��܍�
	{
		WFLBY_GADGET_MDL_SPARKA16x16,
		WFLBY_GADGET_TEX_SPARK_A,
		{
			WFLBY_GADGET_ANM_SPARK_A,
			WFLBY_GADGET_ANM_NONE,
		},
	},

	// ���Ȃ��܉E
	{
		WFLBY_GADGET_MDL_SPARKB16x16,
		WFLBY_GADGET_TEX_SPARK_B,
		{
			WFLBY_GADGET_ANM_SPARK_B,
			WFLBY_GADGET_ANM_NONE,
		},
	},
	
};


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �ėp����֐�
// �����ړ�
static void WFLBY_GADGET_MV_Straight_Init( WFLBY_GADGET_MV_STRAIGHT* p_wk, fx32 s_x, fx32 e_x, fx32 s_y, fx32 e_y, fx32 s_z, fx32 e_z, s32 count_max );
static BOOL WFLBY_GADGET_MV_Straight_Main( WFLBY_GADGET_MV_STRAIGHT* p_wk, s32 count );
static void WFLBY_GADGET_MV_Straight_GetNum( const WFLBY_GADGET_MV_STRAIGHT* cp_wk, fx32* p_x, fx32* p_y, fx32* p_z );
// Sin�J�[�u�ړ�
static void WFLBY_GADGET_MV_SinCurve_Init( WFLBY_GADGET_MV_SINCURVE* p_wk, u16 s_rota, u16 speed, fx32 dis );
static void WFLBY_GADGET_MV_SinCurve_Main( WFLBY_GADGET_MV_SINCURVE* p_wk );
static void WFLBY_GADGET_MV_SinCurve_GetNum( const WFLBY_GADGET_MV_SINCURVE* cp_wk, fx32* p_num );

// SE�Đ�
static void WFLBY_GADGET_SePlay( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 seno );


// ���f���f�[�^
static void WFLBY_GADGET_LoadMdl( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID );
static void WFLBY_GADGET_DeleteMdl( WFLBY_GADGET* p_sys );
// �e�N�X�`���f�[�^
static void WFLBY_GADGET_LoadTex( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID );
static void WFLBY_GADGET_DeleteTex( WFLBY_GADGET* p_sys );
// �A�j���f�[�^
static void WFLBY_GADGET_LoadAnm( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID );
static void WFLBY_GADGET_DeleteAnm( WFLBY_GADGET* p_sys );


// �K�W�F�b�g�I�u�W�F
static void WFLBY_GADGET_OBJ_Start( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, WFLBY_3DPERSON* p_person, WFLBY_ITEMTYPE gadget );
static void WFLBY_GADGET_OBJ_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_OBJ_Draw( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_OBJ_End( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_OBJ_CheckMove( const WFLBY_GADGET_OBJ* cp_wk );

static BOOL WFLBY_GADGET_OBJ_CheckRes( const WFLBY_GADGET_OBJ* cp_wk, u32 idx );
static void WFLBY_GADGET_OBJ_SetRes( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 idx, const WFLBY_GADGET_RES* cp_resdata );
static void WFLBY_GADGET_OBJ_DrawRes( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 idx );
static void WFLBY_GADGET_OBJ_LoopAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx );
static BOOL WFLBY_GADGET_OBJ_NoLoopAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx );
static void WFLBY_GADGET_OBJ_LoopAnm_Sp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 speed );
static BOOL WFLBY_GADGET_OBJ_NoLoopAnm_Sp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 speed );
static void WFLBY_GADGET_OBJ_SetFrame( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 frame );
static fx32 WFLBY_GADGET_OBJ_GetFrame( const WFLBY_GADGET* cp_sys, const WFLBY_GADGET_OBJ* cp_wk, u32 objidx, u32 anmidx );


// �e�K�W�F�b�g�^�C�v�̃A�j���f�[�^
static void WFLBY_GADGET_ANM_Init_Rippru00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Rippru01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Rippru02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Swing00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Swing01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Swing02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Signal00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cracker00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cracker01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cracker02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Balloon00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Balloon01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Balloon02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Sparkle00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Sparkle01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Sparkle02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Bell00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Bell01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Bell02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Drum00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Drum01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Drum02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cymbals00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cymbals01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_ANM_Init_Cymbals02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );

static BOOL WFLBY_GADGET_ANM_Main_AnmTwo( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_AnmOne( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Rippru00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Signal00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Signal01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Signal02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Swing00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Cracker00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Balloon00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Sparkle00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Onpu00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static BOOL WFLBY_GADGET_ANM_Main_Cymbals( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );


static BOOL WFLBY_GADGET_ANM_Draw_Obj( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );




// ���D����
static void WFLBY_GADGET_Balloon_InitUp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_num );
static void WFLBY_GADGET_Balloon_InitUp_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_InitUp_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainUp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainUp_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainUp_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_InitDown( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_InitDown_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_InitDown_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainDown( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainDown_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_MainDown_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_SetObjPos( WFLBY_GADGET_OBJ*  p_wk );
static void WFLBY_GADGET_Balloon_SetObjPos_Roof( WFLBY_GADGET_OBJ*  p_wk );
static BOOL WFLBY_GADGET_Balloon_SetAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_idx, u32 anmidx );
static void WFLBY_GADGET_Balloon_Anm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_idx );

// ���ρ[����
static void WFLBY_GADGET_Sparkle_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 roop_num );
static BOOL WFLBY_GADGET_Sparkle_OneObj_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx );


// ���b�v��
static void WFLBY_GADGET_Ripple_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 ripple_num );


// �V�O�i��
static BOOL WFLBY_GADGET_Signal_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 num );

// �X�E�B���O
static void WFLBY_GADGET_Swing_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 num );

// �Ž��
static void WFLBY_GADGET_Inazuma_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_Inazuma_Start( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );
static void WFLBY_GADGET_Inazuma_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk );

// �������˃V�X�e��
static void WFLBY_GADGET_ONPU_Cont_Init( WFLBY_GADGET_OBJWK* p_wk, const WFLBY_GADGET_MOVE_ONPU_DATA* cp_data, u32 data_num, u32 count_max, u32 anm_type, WFLBY_GADGET_OBJ* p_gadget );
static WFLBY_GADGET_ONPU_MAIN_RET WFLBY_GADGET_ONPU_Cont_Main( WFLBY_GADGET_OBJWK* p_wk, u32 count );

// ���������̓���
static void WFLBY_GADGET_OnpuMove_Init( WFLBY_GADGET_ONPU* p_wk, D3DOBJ* p_obj0, D3DOBJ* p_obj1, D3DOBJ* p_obj2, D3DOBJ* p_obj3, const WFLBY_3DPERSON* cp_person, u32 anm_type );
static void WFLBY_GADGET_OnpuMove_Start( WFLBY_GADGET_ONPU* p_wk, u32 num );
static BOOL WFLBY_GADGET_OnpuMove_Main( WFLBY_GADGET_ONPU* p_wk );
//�x��
static void WFLBY_GADGET_OnpuMove_InitBell( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person );
static BOOL WFLBY_GADGET_OnpuMove_MainBell( WFLBY_GADGET_ONPU* p_wk );
//�h����
static void WFLBY_GADGET_OnpuMove_InitDram( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person );
static BOOL WFLBY_GADGET_OnpuMove_MainDram( WFLBY_GADGET_ONPU* p_wk );
//�V���o��
static void WFLBY_GADGET_OnpuMove_InitCymbals( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person );
static BOOL WFLBY_GADGET_OnpuMove_MainCymbals( WFLBY_GADGET_ONPU* p_wk );
static void WFLBY_GADGET_OnpuMove_SetCymbalsStParam( WFLBY_GADGET_ONPU* p_wk, u32 idx, const VecFx32* cp_def_matrix, fx32 ofs_x0, fx32 ofs_y0, fx32 ofs_x1, fx32 ofs_y1, u32 sync );

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�V�X�e��������
 *
 *	@param	p_system		���r�[�V�X�e��
 *	@param	p_mapcont		�}�b�v�Ǘ��V�X�e��
 *	@param	p_camera		�J�����Ǘ��V�X�e��
 *	@param	p_objcont		�I�u�W�F�Ǘ��V�X�e��	
 *	@param	heapID			�q�[�vID
 *	@param	gheapID			�O���t�B�b�N�q�[�vID
 */
//-----------------------------------------------------------------------------
WFLBY_GADGET* WFLBY_GADGET_Init( WFLBY_SYSTEM* p_system, WFLBY_MAPCONT* p_mapcont, WFLBY_CAMERA* p_camera, WFLBY_3DOBJCONT* p_objcont, u32 heapID, u32 gheapID )
{
	WFLBY_GADGET* p_sys;

	// �V�X�e�����[�N�쐬
	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_GADGET) );
	memset( p_sys, 0, sizeof(WFLBY_GADGET) );

	// �I�u�W�F�Ǘ��V�X�e���ۑ�
	p_sys->p_system		= p_system;
	p_sys->p_objcont	= p_objcont;
	p_sys->p_camera		= p_camera;
	p_sys->p_mapcont	= p_mapcont;

	// ���\�[�X�ǂݍ���
	{
		ARCHANDLE* p_handle;
		p_handle = ArchiveDataHandleOpen( ARC_WIFILOBBY_OTHER_GRA, heapID );

		// �A���P�[�^�쐬
		sys_InitAllocator( &p_sys->allocator, gheapID, 4 );

		// ���f���ǂݍ���
		WFLBY_GADGET_LoadMdl( p_sys, p_handle, gheapID );

		// �e�N�X�`���ǂݍ���
		WFLBY_GADGET_LoadTex( p_sys, p_handle, gheapID );

		// �A�j���ǂݍ���
		WFLBY_GADGET_LoadAnm( p_sys, p_handle, gheapID );

		ArchiveDataHandleClose( p_handle );
	}

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�V�X�e���j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_GADGET_Exit( WFLBY_GADGET* p_sys )
{
	// �A�j���f�[�^�j��
	{
		// ���f���ǂݍ���
		WFLBY_GADGET_DeleteMdl( p_sys );

		// �e�N�X�`���ǂݍ���
		WFLBY_GADGET_DeleteTex( p_sys );

		// �A�j���ǂݍ���
		WFLBY_GADGET_DeleteAnm( p_sys );
	}

	// ���Ԃ̔j��
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���C��
 *
 *	@param	p_sys	�K�W�F�b�g�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_GADGET_Main( WFLBY_GADGET* p_sys )
{
	int i;

#ifdef WFLBY_DEBUG_GADGET_ALL_PUT
	if( sys.cont & PAD_BUTTON_R ){
		const WFLBY_USER_PROFILE* cp_profile;
		cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_sys->p_system );
		for( i=0; i<WFLBY_PLAYER_MAX; i++ ){

			// ���̐l�����邩�`�F�b�N
			if( WFLBY_3DOBJCONT_GetPlIDWk( p_sys->p_objcont, i ) != NULL ){
				if( WFLBY_GADGET_EndWait( p_sys, i ) == TRUE ){
//					WFLBY_GADGET_Start( p_sys, i, (gf_mtRand() % WFLBY_ITEM_NUM) );
					WFLBY_GADGET_Start( p_sys, i, WFLBY_SYSTEM_GetProfileItem( cp_profile ) );
				}
			}
		}
	}
#endif
	
	
	// ���쒆�̃I�u�W�F�𓮂���
	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		WFLBY_GADGET_OBJ_Main( p_sys, &p_sys->obj[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g	�`��
 *
 *	@param	p_sys	�K�W�F�b�g�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_GADGET_Draw( WFLBY_GADGET* p_sys )
{
	int i;
	BOOL result;
	WFLBY_3DPERSON* p_player;

	p_player = WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont );

	// ���쒆�̃I�u�W�F��`��
	for( i=0; i<WFLBY_PLAYER_MAX; i++ ){
		WFLBY_GADGET_OBJ_Draw( p_sys, &p_sys->obj[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g	�J�n
 *
 *	@param	p_sys	�K�W�F�b�g�V�X�e��
 *	@param	idx		���[�UID
 *	@param	gadget	�K�W�F�b�g�^�C�v
 */
//-----------------------------------------------------------------------------
void WFLBY_GADGET_Start( WFLBY_GADGET* p_sys, u32 idx, WFLBY_ITEMTYPE gadget )
{	
	WFLBY_3DPERSON* p_person;
	
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );
	
	// �J�n
	p_person = WFLBY_3DOBJCONT_GetPlIDWk( p_sys->p_objcont, idx );
	if( p_person == NULL ){
		OS_TPrintf( "gadget start err person none\n" );
		return ; 
	}
	WFLBY_GADGET_OBJ_Start( p_sys, &p_sys->obj[idx], p_person, gadget );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g	�I���҂�
 *
 *	@param	cp_sys	�K�W�F�b�g�V�X�e��
 *	@param	idx		���[�UID
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_GADGET_EndWait( const WFLBY_GADGET* cp_sys, u32 idx )
{
	if( WFLBY_GADGET_OBJ_CheckMove( &cp_sys->obj[idx] ) == TRUE ){
		return FALSE;
	}
	return TRUE;
}





//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���i�ړ�
 *
 *	@param	p_wk		���[�N
 *	@param	s_x			�J�n�����W	
 *	@param	e_x			�I�������W
 *	@param	s_y			�J�n�����W
 *	@param	e_y			�I�������W
 *	@param	s_z			�J�n�����W
 *	@param	e_z			�I�������W
 *	@param	count_max	�ő�J�E���g��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_MV_Straight_Init( WFLBY_GADGET_MV_STRAIGHT* p_wk, fx32 s_x, fx32 e_x, fx32 s_y, fx32 e_y, fx32 s_z, fx32 e_z, s32 count_max )
{
	p_wk->count_max		= count_max;
	p_wk->x				= s_x;
	p_wk->dis_x			= e_x - s_x;
	p_wk->start_x		= s_x;
	p_wk->y				= s_y;
	p_wk->dis_y			= e_y - s_y;
	p_wk->start_y		= s_y;
	p_wk->z				= s_z;
	p_wk->dis_z			= e_z - s_z;
	p_wk->start_z		= s_z;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���i�ړ�	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	count		�J�E���g�l
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_MV_Straight_Main( WFLBY_GADGET_MV_STRAIGHT* p_wk, s32 count )
{
	s32 count_num;	
	BOOL ret = FALSE;

	// �J�E���g�l�̍ő�l�I�[�o�[�`�F�b�N
	if( count > p_wk->count_max ){
		count_num = p_wk->count_max;
		ret = TRUE;
	}else{
		count_num = count;
	}
	if( p_wk->dis_x != 0 ){
		p_wk->x = p_wk->start_x + FX_Div(FX_Mul( FX32_CONST(count_num), p_wk->dis_x ), FX32_CONST(p_wk->count_max));
	}
	if( p_wk->dis_y != 0 ){
		p_wk->y = p_wk->start_y + FX_Div(FX_Mul( FX32_CONST(count_num), p_wk->dis_y ), FX32_CONST(p_wk->count_max));
	}
	if( p_wk->dis_z != 0 ){
		p_wk->z = p_wk->start_z + FX_Div(FX_Mul( FX32_CONST(count_num), p_wk->dis_z ), FX32_CONST(p_wk->count_max));
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���i�ړ�	�l���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_x			�����W
 *	@param	p_y			�����W
 *	@param	p_z			�����W
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_MV_Straight_GetNum( const WFLBY_GADGET_MV_STRAIGHT* cp_wk, fx32* p_x, fx32* p_y, fx32* p_z )
{
	*p_x = cp_wk->x;
	*p_y = cp_wk->y;
	*p_z = cp_wk->z;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�T�C���J�[�u����	������
 *
 *	@param	p_wk		���[�N
 *	@param	s_rota		�J�n��]�p�x
 *	@param	speed		�X�s�[�h
 *	@param	dis			����
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_MV_SinCurve_Init( WFLBY_GADGET_MV_SINCURVE* p_wk, u16 s_rota, u16 speed, fx32 dis )
{
	p_wk->rota_num	= s_rota;
	p_wk->speed		= speed;
	p_wk->dis		= dis;
	p_wk->num		= FX_Mul( FX_SinIdx( s_rota ), p_wk->dis );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�C���J�[�u����	���C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_MV_SinCurve_Main( WFLBY_GADGET_MV_SINCURVE* p_wk )
{
	p_wk->rota_num	+= p_wk->speed;
	p_wk->num		= FX_Mul( FX_SinIdx( p_wk->rota_num ), p_wk->dis );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�C���J�[�u����	�l�擾
 *
 *	@param	cp_wk	���[�N
 *	@param	p_num	�ړ��l
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_MV_SinCurve_GetNum( const WFLBY_GADGET_MV_SINCURVE* cp_wk, fx32* p_num )
{
	*p_num = cp_wk->num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�gSE�Đ�
 *
 *	@param	p_sys		�K�W�F�b�g�V�X�e��
 *	@param	p_wk		���[�N
 *	@param	seno		SE�i���o�[
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_SePlay( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 seno )
{
	// �����Ƒ��l�ōĐ����@��ύX
	if( p_wk->p_person == WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont ) ){
		Snd_SePlayEx( seno, SND_PLAYER_NO_WIFI_HIROBA );	//������p�̃v���C���[�i���o�[�ōĐ�
	}else{
		if( WFLBY_3DOBJCONT_GetCullingFlag( p_wk->p_person ) == FALSE ){
			Snd_SePlay( seno );
		}
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	���f���f�[�^�ǂݍ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	gheapID		�O���t�B�b�N�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_LoadMdl( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID )
{
	int i;

	for( i=0; i<WFLBY_GADGET_MDL_NUM; i++ ){
		p_sys->mdl[i].pResMdl	= ArcUtil_HDL_Load( p_handle, 
					WFLBY_GADGET_MDL_FILE_START + i,
					FALSE, gheapID, ALLOC_TOP );
		p_sys->mdl[i].pModelSet	= NNS_G3dGetMdlSet( p_sys->mdl[i].pResMdl );
		p_sys->mdl[i].pModel	= NNS_G3dGetMdlByIdx( p_sys->mdl[i].pModelSet, 0 );

		// �e�N�X�`���͌�Ńo�C���h����
		p_sys->mdl[i].pMdlTex = NULL;

		// �G�~�b�V�����𖾂邭����
		NNS_G3dMdlSetMdlEmiAll( p_sys->mdl[i].pModel, GX_RGB( 31,31,31 ) );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���f���f�[�^�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_DeleteMdl( WFLBY_GADGET* p_sys )
{
	int i;

	for( i=0; i<WFLBY_GADGET_MDL_NUM; i++ ){
		sys_FreeMemoryEz( p_sys->mdl[i].pResMdl );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�N�X�`���ǂݍ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_handle	�n���h��
 *	@param	gheapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_LoadTex( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID )
{
	int i;

	for( i=0; i<WFLBY_GADGET_TEX_NUM; i++ ){
		WFLBY_3DMAPOBJ_TEX_LoatCutTex( &p_sys->p_texres[i], p_handle,
							WFLBY_GADGET_TEX_FILE_START+i, gheapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�N�X�`���j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_DeleteTex( WFLBY_GADGET* p_sys )
{
	int i;
	NNSG3dTexKey texKey;
	NNSG3dTexKey tex4x4Key;
	NNSG3dPlttKey plttKey;
	NNSG3dResTex* p_tex;

	for( i=0; i<WFLBY_GADGET_TEX_NUM; i++ ){

		p_tex = NNS_G3dGetTex( p_sys->p_texres[i] );
		//VramKey�j��
		NNS_G3dTexReleaseTexKey( p_tex, &texKey, &tex4x4Key );
		NNS_GfdFreeTexVram( texKey );	
		NNS_GfdFreeTexVram( tex4x4Key );	

		plttKey = NNS_G3dPlttReleasePlttKey( p_tex );
		NNS_GfdFreePlttVram( plttKey );

		sys_FreeMemoryEz( p_sys->p_texres[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���f�[�^�ǂݍ���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_handle	�n���h��
 *	@param	gheapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_LoadAnm( WFLBY_GADGET* p_sys, ARCHANDLE* p_handle, u32 gheapID )
{
	int i, j;

	// �A�j���̓��f���f�[�^���K�v�Ȃ���sc_WFLBY_GADGET_RES�����悤���ď���������B
	// sc_WFLBY_GADGET_RES�ɏd�����ăA�j���萔���w�肵���Ƃ��́A
	// �ŏ��̂P�񂾂����������m�ۂ���
	for( i=0; i<WFLBY_GADGET_OBJ_NUM; i++ ){
		for( j=0; j<WFLBY_GADGET_ANM_MAX; j++ ){
			if( sc_WFLBY_GADGET_RES[i].anm[j] != WFLBY_GADGET_ANM_NONE ){
				if( p_sys->anm[ sc_WFLBY_GADGET_RES[i].anm[j] ].pResAnm == NULL ){

					// �e�N�X�`���A�j���p�ɑΉ�����e�N�X�`�������f���ɐݒ肷��
					p_sys->mdl[ sc_WFLBY_GADGET_RES[i].mdl ].pMdlTex = NNS_G3dGetTex( p_sys->p_texres[sc_WFLBY_GADGET_RES[i].tex] );
					
					D3DOBJ_AnmLoadH( &p_sys->anm[ sc_WFLBY_GADGET_RES[i].anm[j] ], 
							&p_sys->mdl[ sc_WFLBY_GADGET_RES[i].mdl ], p_handle, 
							WFLBY_GADGET_ANM_FILE_START + sc_WFLBY_GADGET_RES[i].anm[j], 
							gheapID, &p_sys->allocator );
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j�����\�[�X�̔j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_DeleteAnm( WFLBY_GADGET* p_sys )
{
	int i;

	for( i=0; i<WFLBY_GADGET_ANM_NUM; i++ ){
		if( p_sys->anm[i].pResAnm != NULL ){
			D3DOBJ_AnmDelete( &p_sys->anm[i], &p_sys->allocator );
			p_sys->anm[i].pResAnm = NULL;
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�A�j���J�n
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	p_person	�l�����[�N
 *	@param	gadget		�K�W�F�b�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_Start( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, WFLBY_3DPERSON* p_person, WFLBY_ITEMTYPE gadget )
{
	static void (* const pFunc[ WFLBY_ITEM_NUM ])( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk ) = {
		WFLBY_GADGET_ANM_Init_Bell00,		// �x����
		WFLBY_GADGET_ANM_Init_Bell01,		// �x����
		WFLBY_GADGET_ANM_Init_Bell02,		// �x����
		WFLBY_GADGET_ANM_Init_Drum00,       // �h������
		WFLBY_GADGET_ANM_Init_Drum01,       // �h������
		WFLBY_GADGET_ANM_Init_Drum02,       // �h������
		WFLBY_GADGET_ANM_Init_Cymbals00,	// �V���o����
		WFLBY_GADGET_ANM_Init_Cymbals01,	// �V���o����
		WFLBY_GADGET_ANM_Init_Cymbals02,	// �V���o����
		WFLBY_GADGET_ANM_Init_Rippru00,     // ���b�v����
		WFLBY_GADGET_ANM_Init_Rippru01,     // ���b�v����
		WFLBY_GADGET_ANM_Init_Rippru02,     // ���b�v����
		WFLBY_GADGET_ANM_Init_Signal00,     // �V�O�i����
		WFLBY_GADGET_ANM_Init_Signal00,     // �V�O�i����
		WFLBY_GADGET_ANM_Init_Signal00,     // �V�O�i����
		WFLBY_GADGET_ANM_Init_Swing00,   	// �X�E�B���O��
		WFLBY_GADGET_ANM_Init_Swing01,       // �X�E�B���O��
		WFLBY_GADGET_ANM_Init_Swing02,       // �X�E�B���O��
		WFLBY_GADGET_ANM_Init_Cracker00,     // �N���b�J�[��
		WFLBY_GADGET_ANM_Init_Cracker01,     // �N���b�J�[��
		WFLBY_GADGET_ANM_Init_Cracker02,     // �N���b�J�[��
		WFLBY_GADGET_ANM_Init_Sparkle00,     // �X�p�[�N����
		WFLBY_GADGET_ANM_Init_Sparkle01,     // �X�p�[�N����
		WFLBY_GADGET_ANM_Init_Sparkle02,     // �X�p�[�N����
		WFLBY_GADGET_ANM_Init_Balloon00,   // �o���[����
		WFLBY_GADGET_ANM_Init_Balloon01,   // �o���[����
		WFLBY_GADGET_ANM_Init_Balloon02,   // �o���[����
	};
	static const u32 sc_SndData[ WFLBY_ITEM_NUM ] = {
		WFLBY_SND_TOUCH_TOY01_1,
		WFLBY_SND_TOUCH_TOY01_2,
		WFLBY_SND_TOUCH_TOY01_3,
		WFLBY_SND_TOUCH_TOY02_1,
		WFLBY_SND_TOUCH_TOY02_2,
		WFLBY_SND_TOUCH_TOY02_3,
		WFLBY_SND_TOUCH_TOY03_1,
		WFLBY_SND_TOUCH_TOY03_2,
		WFLBY_SND_TOUCH_TOY03_3,
		WFLBY_SND_TOUCH_TOY04_1,
		WFLBY_SND_TOUCH_TOY04_2,
		WFLBY_SND_TOUCH_TOY04_3,
		WFLBY_SND_TOUCH_TOY05_1,
		WFLBY_SND_TOUCH_TOY05_2,
		WFLBY_SND_TOUCH_TOY05_3,
		WFLBY_SND_TOUCH_TOY06_1,
		WFLBY_SND_TOUCH_TOY06_2,
		WFLBY_SND_TOUCH_TOY06_3,
		WFLBY_SND_TOUCH_TOY07_1,
		WFLBY_SND_TOUCH_TOY07_2,
		WFLBY_SND_TOUCH_TOY07_3,
		WFLBY_SND_TOUCH_TOY08_1,
		WFLBY_SND_TOUCH_TOY08_2,
		WFLBY_SND_TOUCH_TOY08_3,
		WFLBY_SND_TOUCH_TOY09_1,
		WFLBY_SND_TOUCH_TOY09_2,
		WFLBY_SND_TOUCH_TOY09_3,
	};
	GF_ASSERT( gadget < WFLBY_ITEM_NUM );

	// ���쒆�Ȃ炻�̃f�[�^�j��
	WFLBY_GADGET_OBJ_End( p_sys, p_wk );
	
	// �l�����[�N�ݒ�
	p_wk->p_person		= p_person;

	// �K�W�F�b�g�^�C�v�ݒ�
	p_wk->gadget_type	= gadget;

	// ���f���ɐݒ肷��I�u�W�FID
	p_wk->mdl_objid		= WFLBY_GADGET_3DMDL_OBJID + WFLBY_3DOBJCONT_GetWkObjData( p_person, WF2DMAP_OBJPM_ST );

	// �`��f�[�^�ݒ�
	pFunc[ p_wk->gadget_type ]( p_sys, p_wk );

	// ��ʓ��ɐl�������特��炷
	WFLBY_GADGET_SePlay( p_sys, p_wk, sc_SndData[ p_wk->gadget_type ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I�u�W�F���상�C��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	static BOOL (* const pFunc[ WFLBY_ITEM_NUM ])( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk ) = {
		WFLBY_GADGET_ANM_Main_Onpu00,		// �x����
		WFLBY_GADGET_ANM_Main_Onpu00,		// �x����
		WFLBY_GADGET_ANM_Main_Onpu00,		// �x����
		WFLBY_GADGET_ANM_Main_Onpu00,		// �h������
		WFLBY_GADGET_ANM_Main_Onpu00,		// �h������
		WFLBY_GADGET_ANM_Main_Onpu00,		// �h������
		WFLBY_GADGET_ANM_Main_Cymbals,		// �V���o����
		WFLBY_GADGET_ANM_Main_Cymbals,		// �V���o����
		WFLBY_GADGET_ANM_Main_Cymbals,		// �V���o����
		WFLBY_GADGET_ANM_Main_Rippru00,		// ���b�v����
		WFLBY_GADGET_ANM_Main_Rippru00,		// ���b�v����
		WFLBY_GADGET_ANM_Main_Rippru00,		// ���b�v����
		WFLBY_GADGET_ANM_Main_Signal00,		// �V�O�i����
		WFLBY_GADGET_ANM_Main_Signal01,	    // �V�O�i����
		WFLBY_GADGET_ANM_Main_Signal02,	    // �V�O�i����
		WFLBY_GADGET_ANM_Main_Swing00,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Main_Swing00,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Main_Swing00,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Main_Cracker00,	// �N���b�J�[��
		WFLBY_GADGET_ANM_Main_Cracker00,	// �N���b�J�[��
		WFLBY_GADGET_ANM_Main_Cracker00,	// �N���b�J�[��
		WFLBY_GADGET_ANM_Main_Sparkle00,	// �X�p�[�N����
		WFLBY_GADGET_ANM_Main_Sparkle00,	// �X�p�[�N����
		WFLBY_GADGET_ANM_Main_Sparkle00,	// �X�p�[�N����
		WFLBY_GADGET_ANM_Main_Balloon00,	// �o���[����
		WFLBY_GADGET_ANM_Main_Balloon00,	// �o���[����
		WFLBY_GADGET_ANM_Main_Balloon00,	// �o���[����
	};

	// �f�[�^�����邩�`�F�b�N
	if( WFLBY_GADGET_OBJ_CheckMove( p_wk ) == FALSE ){
		return ;
	}
	
	GF_ASSERT( p_wk->gadget_type < WFLBY_ITEM_NUM );

	if( pFunc[ p_wk->gadget_type ]( p_sys, p_wk ) == TRUE ){
		WFLBY_GADGET_OBJ_End( p_sys, p_wk );	// �I��
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I�u�W�F�`��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�`�悵��
 *	@retval	FALSE	�`�悵�ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OBJ_Draw( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	/*
	static void (* const pFunc[ WFLBY_ITEM_NUM ])( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk ) = {
		WFLBY_GADGET_ANM_Draw_Obj,		// �x����
		WFLBY_GADGET_ANM_Draw_Obj,		// �x����
		WFLBY_GADGET_ANM_Draw_Obj,		// �x����
		WFLBY_GADGET_ANM_Draw_Obj,		// �h������
		WFLBY_GADGET_ANM_Draw_Obj,		// �h������
		WFLBY_GADGET_ANM_Draw_Obj,		// �h������
		WFLBY_GADGET_ANM_Draw_Obj,		// �V���o����
		WFLBY_GADGET_ANM_Draw_Obj,		// �V���o����
		WFLBY_GADGET_ANM_Draw_Obj,		// �V���o����
		WFLBY_GADGET_ANM_Draw_Obj,		// ���b�v����
		WFLBY_GADGET_ANM_Draw_Obj,		// ���b�v����
		WFLBY_GADGET_ANM_Draw_Obj,		// ���b�v����
		WFLBY_GADGET_ANM_Draw_Obj,		// �V�O�i����
		WFLBY_GADGET_ANM_Draw_Obj,	    // �V�O�i����
		WFLBY_GADGET_ANM_Draw_Obj,	    // �V�O�i����
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�E�B���O��
		WFLBY_GADGET_ANM_Draw_Obj,		// �N���b�J�[��
		WFLBY_GADGET_ANM_Draw_Obj,		// �N���b�J�[��
		WFLBY_GADGET_ANM_Draw_Obj,		// �N���b�J�[��
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�p�[�N����
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�p�[�N����
		WFLBY_GADGET_ANM_Draw_Obj,		// �X�p�[�N����
		WFLBY_GADGET_ANM_Draw_Obj,		// �o���[����
		WFLBY_GADGET_ANM_Draw_Obj,		// �o���[����
		WFLBY_GADGET_ANM_Draw_Obj,		// �o���[����
	};
	*/

	// �f�[�^�����邩�`�F�b�N
	if( WFLBY_GADGET_OBJ_CheckMove( p_wk ) == FALSE ){
		return FALSE;
	}

	GF_ASSERT( p_wk->gadget_type < WFLBY_ITEM_NUM );

//	pFunc[ p_wk->gadget_type ]( p_sys, p_wk );
	return WFLBY_GADGET_ANM_Draw_Obj( p_sys, p_wk );		// ��{����őS��������
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I�u�W�F����I��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_End( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	// �S���[�N�̏�����
	memset( p_wk, 0, sizeof(WFLBY_GADGET_OBJ) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I�u�W�F�����쒆���`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���쒆
 *	@retval	FALSE	��~��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OBJ_CheckMove( const WFLBY_GADGET_OBJ* cp_wk )
{
	if( cp_wk->p_person == NULL ){
		return FALSE;
	}
	return TRUE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	IDX�̕`��I�u�W�F�������Ă邩�`�F�b�N����
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE	����IDX�̕`��I�u�W�F�������Ă���
 *	@retval	FALSE	�������ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OBJ_CheckRes( const WFLBY_GADGET_OBJ* cp_wk, u32 idx )
{
	if( cp_wk->cp_objres[idx] == NULL ){
		return FALSE;
	}
	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�I�u�W�F	���\�[�X�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	idx			D3DOBJ�@�C���f�b�N�X
 *	@param	cp_resdata	���\�[�X�\���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_SetRes( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 idx, const WFLBY_GADGET_RES* cp_resdata )
{
	int i;
	
	GF_ASSERT( idx < WFLBY_GADGET_OBJ_MAX );
	// �I�u�W�F�ݒ肸�݂łȂ����`�F�b�N
	GF_ASSERT( p_wk->cp_objres[idx] == NULL );


	// ���������o��
	{
//		OS_TPrintf( "init mdl %d  tex %d anm [%d] [%d]\n", cp_resdata->mdl, cp_resdata->tex, cp_resdata->anm[0], cp_resdata->anm[1] );
	}
	
	
	// �I�u�W�F�\���f�[�^��ۑ�
	p_wk->cp_objres[idx] = cp_resdata;
	
	// ���f���̐ݒ�
	D3DOBJ_Init( &p_wk->obj[idx], &p_sys->mdl[cp_resdata->mdl] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�PD3DOBJ�̕`�揈��
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_DrawRes( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 idx )
{
	BOOL result;
	void* p_tex;
	D3DOBJ_MDL* p_mdl;
	D3DOBJ_ANM* p_anm;
	int i;

	GF_ASSERT( p_wk->cp_objres[idx] != NULL );

	if( D3DOBJ_GetDraw( &p_wk->obj[idx] ) == FALSE ){
		return ;
	}

	// �e�N�X�`���ƃ��f���擾
	p_tex = p_sys->p_texres[ p_wk->cp_objres[idx]->tex ];
	p_mdl = &p_sys->mdl[ p_wk->cp_objres[idx]->mdl ];
	
	// �e�N�X�`���o�C���h
	p_mdl->pMdlTex = NNS_G3dGetTex( p_tex );
	// �������\��t���Ă݂�
	result = NNS_G3dForceBindMdlTex( p_mdl->pModel, p_mdl->pMdlTex, 0, 0 );
	GF_ASSERT( result );
	// �������\��t���Ă݂�
	result = NNS_G3dForceBindMdlPltt( p_mdl->pModel, p_mdl->pMdlTex, 0, 0 );
	GF_ASSERT( result );

	// �A�j���̓K�p
	for( i=0; i<WFLBY_GADGET_ANM_MAX; i++ ){
		if( p_wk->cp_objres[idx]->anm[i] != WFLBY_GADGET_ANM_NONE ){
			p_anm = &p_sys->anm[ p_wk->cp_objres[idx]->anm[i] ];
			D3DOBJ_AddAnm( &p_wk->obj[idx], p_anm );
			// �t���[�������킹��
			D3DOBJ_AnmSet( p_anm, p_wk->anm_frame[idx][i] );
		}else{
			break;
		}
	}

	// OBJID�̐ݒ�
	NNS_G3dMdlSetMdlPolygonIDAll( p_mdl->pModel, p_wk->mdl_objid );

	// �`��
	D3DOBJ_Draw( &p_wk->obj[idx] );	

	// �A�j����j��
	for( i=0; i<WFLBY_GADGET_ANM_MAX; i++ ){
		if( p_wk->cp_objres[idx]->anm[i] != WFLBY_GADGET_ANM_NONE ){
			p_anm = &p_sys->anm[ p_wk->cp_objres[idx]->anm[i] ];
			D3DOBJ_DelAnm( &p_wk->obj[idx], p_anm );
		}else{
			break;
		}
	}

	// �o�C���h����
	NNS_G3dReleaseMdlTex( p_mdl->pModel );
	NNS_G3dReleaseMdlPltt( p_mdl->pModel );
//	NNS_G3dReleaseMdlSet(p_mdl->pModelSet );
	p_mdl->pMdlTex = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�v�A�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	objidx		�I�u�W�FIDX
 *	@param	anmidx		�A�j��IDX
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_LoopAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx )
{
	WFLBY_GADGET_OBJ_LoopAnm_Sp( p_sys, p_wk, objidx, anmidx, WFLBY_GADGET_ANM_SPEED );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�Ղ��Ȃ��A�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	objidx		�I�u�W�FIDX
 *	@param	anmidx		�A�j��IDX
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OBJ_NoLoopAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx )
{
	return WFLBY_GADGET_OBJ_NoLoopAnm_Sp( p_sys, p_wk, objidx, anmidx, WFLBY_GADGET_ANM_SPEED );
}
//----------------------------------------------------------------------------
/**
 *	@brief	���[�v�Đ�	�X�s�[�h�w���
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OBJ_LoopAnm_Sp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 speed )
{
	fx32 end_frame;
	D3DOBJ_ANM* p_anm;

	// �A�j���I�u�W�F�擾
	p_anm = &p_sys->anm[ p_wk->cp_objres[ objidx ]->anm[ anmidx ] ];

	end_frame = D3DOBJ_AnmGetFrameNum( p_anm );

	// �A�j����i�߂�
	if( (p_wk->anm_frame[objidx][anmidx] + speed) < end_frame ){
		p_wk->anm_frame[objidx][anmidx] += speed;
	}else{
		p_wk->anm_frame[objidx][anmidx] = (p_wk->anm_frame[objidx][anmidx] + WFLBY_GADGET_ANM_SPEED) % end_frame;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	NO���[�v�Đ�	�X�s�[�h�w���
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OBJ_NoLoopAnm_Sp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 speed )
{
	fx32 end_frame;
	BOOL ret;
	D3DOBJ_ANM* p_anm;

	// �A�j���I�u�W�F�擾
	p_anm = &p_sys->anm[ p_wk->cp_objres[ objidx ]->anm[ anmidx ] ];

	end_frame = D3DOBJ_AnmGetFrameNum( p_anm );

	// �A�j����i�߂�
	if( (p_wk->anm_frame[objidx][anmidx] + speed) < end_frame ){
		p_wk->anm_frame[objidx][anmidx] += speed;
		ret = FALSE;
	}else{
		p_wk->anm_frame[objidx][anmidx] =  end_frame - FX32_HALF;
		ret = TRUE;
	}

	return ret;
}

// �t���[�����̐ݒ�
static void WFLBY_GADGET_OBJ_SetFrame( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx, u32 anmidx, fx32 frame )
{
	fx32 end_frame;
	D3DOBJ_ANM* p_anm;

	// �A�j���I�u�W�F�擾
	p_anm = &p_sys->anm[ p_wk->cp_objres[ objidx ]->anm[ anmidx ] ];

	end_frame = D3DOBJ_AnmGetFrameNum( p_anm );

	if( end_frame < frame ){
		p_wk->anm_frame[objidx][anmidx] =  end_frame - FX32_HALF;
	}else{
		p_wk->anm_frame[objidx][anmidx] =  frame;
	}
}
//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���t���[�����擾����
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	cp_wk		���[�N
 *	@param	objidx		�I�u�W�FIDX
 *	@param	anmidx		�A�j��IDX
 *
 *	@return	�t���[��
 */
//-----------------------------------------------------------------------------
static fx32 WFLBY_GADGET_OBJ_GetFrame( const WFLBY_GADGET* cp_sys, const WFLBY_GADGET_OBJ* cp_wk, u32 objidx, u32 anmidx )
{
	fx32 end_frame;
	const D3DOBJ_ANM* cp_anm;

	// �A�j���I�u�W�F�擾
	cp_anm = &cp_sys->anm[ cp_wk->cp_objres[ objidx ]->anm[ anmidx ] ];

	return D3DOBJ_AnmGet( cp_anm );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���̏�����
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
// ���b�v��
static void WFLBY_GADGET_ANM_Init_Rippru00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Ripple_Init( p_sys, p_wk, 1 );
}
static void WFLBY_GADGET_ANM_Init_Rippru01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Ripple_Init( p_sys, p_wk, 2 );
}
static void WFLBY_GADGET_ANM_Init_Rippru02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Ripple_Init( p_sys, p_wk, 3 );
}
// �E�C���N
static void WFLBY_GADGET_ANM_Init_Swing00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Swing_Init( p_sys, p_wk, 1 );
}
static void WFLBY_GADGET_ANM_Init_Swing01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Swing_Init( p_sys, p_wk, 2 );
}
static void WFLBY_GADGET_ANM_Init_Swing02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Swing_Init( p_sys, p_wk, 3 );
}
// �V�O�i��
static void WFLBY_GADGET_ANM_Init_Signal00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	VecFx32 matrix;


	for( i=0; i<WFLBY_GADGET_SIGNAL_OBJ_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_SIGNAL00 + i] );

		// ���W�����킹��
		WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
		// �ݒ�
		matrix.x += WFLBY_GADGET_SIGNAL_ANM_MOVE_X;
		matrix.z += WFLBY_GADGET_SIGNAL_ANM_MOVE_Z;
		matrix.y += WFLBY_GADGET_FLOOR_Y + (FX_Mul( FX32_ONE*i, WFLBY_GADGET_SIGNAL_ANM_MOVE_Y ));
		D3DOBJ_SetMatrix( &p_wk->obj[i], matrix.x, matrix.y, matrix.z );

		// ��\��
		D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );
	}

	p_wk->mvwk.signal.lastnum = 0xff;
}

// �N���b�J�[
static void WFLBY_GADGET_ANM_Init_Cracker00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	VecFx32 matrix, flash_matrix;
	u32	way;
	
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER00] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_FLASH, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_FLASH00] );

	// �N���b�J�[�̕`��OFF
	D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FALSE );

	// ���W�����킹��
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	matrix.y		+= WFLBY_GADGET_AIR_Y;
	matrix.x		+= WFLBY_GADGET_CRACKER_CENTER_X;
	flash_matrix	= matrix;
	flash_matrix.y	+= WFLBY_GADGET_FLASH_Y;


	// �A���t�@�J�E���^������
	p_wk->mvwk.cracker.alpha_count[0] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;

	// ��]�p�x
	way = WFLBY_3DOBJCONT_GetWkObjData( p_wk->p_person, WF2DMAP_OBJPM_WAY );
	switch( way ){
	case WF2DMAP_WAY_UP:
		matrix.z		-= 	WFLBY_GADGET_CRACKER_DISZ;
		flash_matrix.z	-=	WFLBY_GADGET_FLASH_DIS;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FX_GET_ROTA_NUM(270), D3DOBJ_ROTA_WAY_Y );
		break;
		
	case WF2DMAP_WAY_DOWN:
		matrix.z		+= 	WFLBY_GADGET_CRACKER_DISZUN;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_DISUN;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FX_GET_ROTA_NUM(90), D3DOBJ_ROTA_WAY_Y );
		break;
		
	case WF2DMAP_WAY_LEFT:
		matrix.x		-= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	-=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FX_GET_ROTA_NUM(0), D3DOBJ_ROTA_WAY_Y );
		break;
		
	case WF2DMAP_WAY_RIGHT:
		matrix.x		+= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	+=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FX_GET_ROTA_NUM(180), D3DOBJ_ROTA_WAY_Y );
		break;

	default:
		break;
	}

	// �ݒ�
	D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x, matrix.y, matrix.z );
	D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], flash_matrix.x, flash_matrix.y, flash_matrix.z );
}
// �N���b�J�[�Q��
static void WFLBY_GADGET_ANM_Init_Cracker01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	VecFx32 matrix, flash_matrix;
	u32	way;
	
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER00] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_FLASH, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_FLASH00] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER01, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER01] );

	// �N���b�J�[�̕`��OFF
	D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FALSE );

	// ���W�����킹��
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	matrix.y		+= WFLBY_GADGET_AIR_Y;
	matrix.x		+= WFLBY_GADGET_CRACKER_CENTER_X;
	flash_matrix	= matrix;
	flash_matrix.y	+= WFLBY_GADGET_FLASH_Y;

	// �A���t�@�J�E���^������
	p_wk->mvwk.cracker.alpha_count[0] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;
	p_wk->mvwk.cracker.alpha_count[1] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;

	// ��]�p�x
	way = WFLBY_3DOBJCONT_GetWkObjData( p_wk->p_person, WF2DMAP_OBJPM_WAY );
	switch( way ){
	case WF2DMAP_WAY_UP:
		matrix.z		-= 	WFLBY_GADGET_CRACKER_DISZ;
		flash_matrix.z	-=	WFLBY_GADGET_FLASH_DIS;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((270-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((270+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x + WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x - WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		break;
		
	case WF2DMAP_WAY_DOWN:
		matrix.z		+= 	WFLBY_GADGET_CRACKER_DISZUN;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_DISUN;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((90-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((90+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x - WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x + WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		break;
		
	case WF2DMAP_WAY_LEFT:
		matrix.x		-= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	-=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((0-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((0+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x, 
				matrix.y, matrix.z - WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x, 
				matrix.y, matrix.z + WFLBY_GADGET_CRACKER_ADD_MOV );
		break;
		
	case WF2DMAP_WAY_RIGHT:
		matrix.x		+= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	+=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((180-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((180+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x, 
				matrix.y, matrix.z + WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x, 
				matrix.y, matrix.z - WFLBY_GADGET_CRACKER_ADD_MOV );
		break;

	default:
		break;
	}

	// �ݒ�
	D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], flash_matrix.x, flash_matrix.y, flash_matrix.z );
}
// �N���b�J�[�R��
static void WFLBY_GADGET_ANM_Init_Cracker02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	VecFx32 matrix, flash_matrix;
	u32	way;
	
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER00] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_FLASH, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_FLASH00] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER01, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER01] );
	WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_CRACKER02, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_CRACLCER02] );

	// �N���b�J�[�̕`��OFF
	D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], FALSE );

	// �A���t�@�J�E���^������
	p_wk->mvwk.cracker.alpha_count[0] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;
	p_wk->mvwk.cracker.alpha_count[1] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;
	p_wk->mvwk.cracker.alpha_count[2] = WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;

	// ���W�����킹��
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	matrix.y		+= WFLBY_GADGET_AIR_Y;
	matrix.x		+= WFLBY_GADGET_CRACKER_CENTER_X;
	flash_matrix	= matrix;
	flash_matrix.y	+= WFLBY_GADGET_FLASH_Y;

	// ��]�p�x
	way = WFLBY_3DOBJCONT_GetWkObjData( p_wk->p_person, WF2DMAP_OBJPM_WAY );
	switch( way ){
	case WF2DMAP_WAY_UP:
		matrix.z		-= 	WFLBY_GADGET_CRACKER_DISZ;
		flash_matrix.z	-=	WFLBY_GADGET_FLASH_DIS;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((270-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((270+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], 
				FX_GET_ROTA_NUM(270), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x + WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x - WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], matrix.x, matrix.y, matrix.z );
		break;
		
	case WF2DMAP_WAY_DOWN:
		matrix.z		+= 	WFLBY_GADGET_CRACKER_DISZUN;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_DISUN;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((90-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((90+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], 
				FX_GET_ROTA_NUM(90), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x - WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x + WFLBY_GADGET_CRACKER_ADD_MOV, 
				matrix.y, matrix.z );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], matrix.x, matrix.y, matrix.z );
		break;
		
	case WF2DMAP_WAY_LEFT:
		matrix.x		-= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	-=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((0-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((0+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], 
				FX_GET_ROTA_NUM(0), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x, 
				matrix.y, matrix.z - WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x, 
				matrix.y, matrix.z + WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], matrix.x, matrix.y, matrix.z );
		break;
		
	case WF2DMAP_WAY_RIGHT:
		matrix.x		+= 	WFLBY_GADGET_CRACKER_DISX;
		flash_matrix.x	+=	WFLBY_GADGET_FLASH_DISX;
		flash_matrix.z	+=	WFLBY_GADGET_FLASH_Z;
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], 
				FX_GET_ROTA_NUM((180-WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], 
				FX_GET_ROTA_NUM((180+WFLBY_GADGET_CRACKER_ROT_ADD)), D3DOBJ_ROTA_WAY_Y );
		D3DOBJ_SetRota( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], 
				FX_GET_ROTA_NUM(180), D3DOBJ_ROTA_WAY_Y );

		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER], matrix.x, 
				matrix.y, matrix.z + WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER01], matrix.x, 
				matrix.y, matrix.z - WFLBY_GADGET_CRACKER_ADD_MOV );
		D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_CRACKER02], matrix.x, matrix.y, matrix.z );
		break;

	default:
		break;
	}

	// �ݒ�
	D3DOBJ_SetMatrix( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], flash_matrix.x, flash_matrix.y, flash_matrix.z );
}


// ���D�P��
static void WFLBY_GADGET_ANM_Init_Balloon00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Balloon_InitUp( p_sys, p_wk, 1 );
}

// ���D�Q��
static void WFLBY_GADGET_ANM_Init_Balloon01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Balloon_InitUp( p_sys, p_wk, 2 );
}

// ���D�R��
static void WFLBY_GADGET_ANM_Init_Balloon02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Balloon_InitUp( p_sys, p_wk, 3 );
}

// ���ӂԂ�1��
static void WFLBY_GADGET_ANM_Init_Sparkle00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Sparkle_Init( p_sys, p_wk, 1 );
}
// ���ӂԂ�2��
static void WFLBY_GADGET_ANM_Init_Sparkle01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Sparkle_Init( p_sys, p_wk, 2 );
}
// ���ӂԂ�4��
static void WFLBY_GADGET_ANM_Init_Sparkle02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	WFLBY_GADGET_Sparkle_Init( p_sys, p_wk, 4 );
}
// �x��
static void WFLBY_GADGET_ANM_Init_Bell00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_00ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL00, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL00), 
			WFLBY_GADGET_MOVE_ONPU_BELL00_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_BELL,
			p_wk );
}
static void WFLBY_GADGET_ANM_Init_Bell01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_01ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL01, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL01), 
			WFLBY_GADGET_MOVE_ONPU_BELL01_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_BELL,
			p_wk );
}
static void WFLBY_GADGET_ANM_Init_Bell02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_02ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL02, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_BELL02), 
			WFLBY_GADGET_MOVE_ONPU_BELL02_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_BELL,
			p_wk );
}

// �h����
static void WFLBY_GADGET_ANM_Init_Drum00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_00ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM00, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM00), 
			WFLBY_GADGET_MOVE_ONPU_DRUM00_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_DRUM,
			p_wk );
}
static void WFLBY_GADGET_ANM_Init_Drum01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_01ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM01, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM01), 
			WFLBY_GADGET_MOVE_ONPU_DRUM01_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_DRUM,
			p_wk );
}
static void WFLBY_GADGET_ANM_Init_Drum02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_02ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM02, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_DRUM02), 
			WFLBY_GADGET_MOVE_ONPU_DRUM02_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_DRUM,
			p_wk );
}
// �V���o��
static void WFLBY_GADGET_ANM_Init_Cymbals00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	VecFx32 matrix;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_00ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS00, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS00), 
			WFLBY_GADGET_MOVE_ONPU_CYMBALS00_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_CYMBALS,
			p_wk );

	// �Ž�Ϗ�����
	WFLBY_GADGET_Inazuma_Init( p_sys, p_wk );
}
static void WFLBY_GADGET_ANM_Init_Cymbals01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	VecFx32 matrix;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_01ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS01, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS01), 
			WFLBY_GADGET_MOVE_ONPU_CYMBALS01_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_CYMBALS,
			p_wk );


	// �Ž�Ϗ�����
	WFLBY_GADGET_Inazuma_Init( p_sys, p_wk );
}
static void WFLBY_GADGET_ANM_Init_Cymbals02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	// �I�u�W�F�쐬
	for( i=0; i<WFLBY_GADGET_ONPU_OBJWK_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[ sc_WFLBY_LEVEL_02ADDOBJ[i] ] );
	}

	// ����p�����[�^�쐬
	WFLBY_GADGET_ONPU_Cont_Init( &p_wk->mvwk, sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS02, 
			NELEMS(sc_WFLBY_GADGET_MOVE_ONPU_DATA_CYMBALS02), 
			WFLBY_GADGET_MOVE_ONPU_CYMBALS02_SYSNC, 
			WFLBY_GADGET_ONPU_MOVE_CYMBALS,
			p_wk );

	// �Ž�Ϗ�����
	WFLBY_GADGET_Inazuma_Init( p_sys, p_wk );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���̃��C��
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
// 2�̃A�j���𓮂���
static BOOL WFLBY_GADGET_ANM_Main_AnmTwo( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	BOOL ret = TRUE;
	BOOL result;
	
	// 2�̃A�j���𓮂���
	for( i=0; i<2; i++ ){
		// �A�j�������邩�`�F�b�N
		if( p_wk->cp_objres[0]->anm[i] != WFLBY_GADGET_ANM_NONE ){
			result = WFLBY_GADGET_OBJ_NoLoopAnm( p_sys, p_wk, 0, i );
			if( result == FALSE ){
				ret = FALSE;
			}
		}
	}

	return ret;
}
// 1�̃A�j���𓮂���
static BOOL WFLBY_GADGET_ANM_Main_AnmOne( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	GF_ASSERT( p_wk->cp_objres[0]->anm[0] != WFLBY_GADGET_ANM_NONE );
	return WFLBY_GADGET_OBJ_NoLoopAnm( p_sys, p_wk, 0, 0 );
}
// ���b�v��
static BOOL WFLBY_GADGET_ANM_Main_Rippru00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	BOOL result;
	u32 move_num;
	u32 objidx;

	if( (p_wk->count+1) <= WFLBY_GADGET_RIPPLE_ROOP_SYNC ){
		p_wk->count ++;
	}
	
	// �I�u�W�F���ˊǗ�����
	// ���v�U�������b�v���̍Đ����J�n����
	move_num = (p_wk->count * WFLBY_GADGET_RIPPLE_ROOP_START) / WFLBY_GADGET_RIPPLE_ROOP_SYNC;
	for( i=p_wk->mvwk.ripple.move_num; i<move_num; i++ ){
		objidx = i % WFLBY_GADGET_RIPPLE_NUM;
		if( objidx < p_wk->mvwk.ripple.ripple_num ){	// ���̃I�u�W�F�͐�������Ă��邩�`�F�b�N
			D3DOBJ_SetDraw( &p_wk->obj[objidx], TRUE );
		}
	}
	p_wk->mvwk.ripple.move_num = move_num;

	// �A�j���Đ�����
	result = TRUE;
	for( i=0; i<p_wk->mvwk.ripple.ripple_num; i++ ){
		if( D3DOBJ_GetDraw( &p_wk->obj[i] ) == TRUE ){
			result = WFLBY_GADGET_OBJ_NoLoopAnm( p_sys, p_wk, i, 0 );
			if( result ){
				D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );
				WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, i, 0, 0 );
			}
		}
	}


	// �I���`�F�b�N
	// �S���b�v���𔭎˂��Ă��āA�S�A�j�����I����Ă�����I��
	if( (p_wk->mvwk.ripple.move_num >= WFLBY_GADGET_RIPPLE_ROOP_START) &&
		(result == TRUE) ){
		return TRUE;
	}
	return FALSE;
}
// �����Ȃ�
static BOOL WFLBY_GADGET_ANM_Main_Signal00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	return WFLBY_GADGET_Signal_Main( p_sys, p_wk, 0 );
}
static BOOL WFLBY_GADGET_ANM_Main_Signal01( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	return WFLBY_GADGET_Signal_Main( p_sys, p_wk, 1 );
}
static BOOL WFLBY_GADGET_ANM_Main_Signal02( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	return WFLBY_GADGET_Signal_Main( p_sys, p_wk, 2 );
}
// �X�E�B���O
static BOOL WFLBY_GADGET_ANM_Main_Swing00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	u32 move_num;
	int	i;
	BOOL result;

	move_num = (p_wk->count * WFLBY_GADGET_SWING_OBJ_NUM) / WFLBY_GADGET_SWING_ANM_SYNC;

	if( move_num != p_wk->mvwk.swing.lastnum ){
		p_wk->mvwk.swing.lastnum = move_num;

		if( p_wk->mvwk.swing.objnum > move_num ){

			// ���܂œ����Ă�����\��
			if( move_num > 0 ){
				D3DOBJ_SetDraw( &p_wk->obj[move_num-1], FALSE );
			}
			// ����J�n
			D3DOBJ_SetDraw( &p_wk->obj[move_num], TRUE );
		}
	}

	if( p_wk->count < WFLBY_GADGET_SWING_ANM_SYNC ){
		p_wk->count ++;
	}

	// �\�����̃I�u�W�F�̃A�j����i�߂�
	result = FALSE;
	for( i=0; i<p_wk->mvwk.swing.objnum; i++ ){
		if( D3DOBJ_GetDraw( &p_wk->obj[i] ) ){
			if( i==p_wk->mvwk.swing.objnum-1 ){
				result = WFLBY_GADGET_OBJ_NoLoopAnm( p_sys, p_wk, i, 0 );
			}else{
				WFLBY_GADGET_OBJ_LoopAnm( p_sys, p_wk, i, 0 );
			}
		}
	}

	return result;
}

// �N���b�J�[
static BOOL WFLBY_GADGET_ANM_Main_Cracker00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	BOOL result;
	BOOL ret;
	int i;
	GF_ASSERT( p_wk->cp_objres[0]->anm[0] != WFLBY_GADGET_ANM_NONE );

	// �����o��
	// �����o��������N���b�J�[���Ȃ�
	if( p_wk->mvwk.cracker.wait > 0 ){
		p_wk->mvwk.cracker.wait --;
		D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], FALSE );			
	}else{
		if( p_wk->count < WFLBY_GADGET_CRACKEROBJ_CRACKER_MAX ){
			if( WFLBY_GADGET_OBJ_CheckRes( p_wk, p_wk->count+WFLBY_GADGET_CRACKEROBJ_CRACKER ) == TRUE ){
				D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], TRUE );
				result = WFLBY_GADGET_OBJ_NoLoopAnm( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_FLASH, 0 );
				if( result == TRUE ){
					p_wk->count ++;
					p_wk->mvwk.cracker.wait = WFLBY_GADGET_CRACKER_START_WAIT;
					if( p_wk->count < WFLBY_GADGET_CRACKEROBJ_CRACKER_MAX ){
						// �A�j���t���[�������ɖ߂�
						WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, WFLBY_GADGET_CRACKEROBJ_FLASH, 0, 0 );
					}else{
						D3DOBJ_SetDraw( &p_wk->obj[WFLBY_GADGET_CRACKEROBJ_FLASH], FALSE );			
					}
				}
			}
		}
	}
	// �S��������
	ret = TRUE;
	for( i=WFLBY_GADGET_CRACKEROBJ_CRACKER; i<=WFLBY_GADGET_CRACKEROBJ_CRACKER02; i++ ){

		// ���̃I�u�W�F�����邩�`�F�b�N
		if( WFLBY_GADGET_OBJ_CheckRes( p_wk, i ) == TRUE ){
			//  ���˃^�C�~���O�`�F�b�N
			if( p_wk->count > (i-WFLBY_GADGET_CRACKEROBJ_CRACKER) ){
				D3DOBJ_SetDraw( &p_wk->obj[i], TRUE );
				result = WFLBY_GADGET_OBJ_NoLoopAnm_Sp( p_sys, p_wk, i, 0, FX32_CONST( 2.50f ) );
				if( result == FALSE ){

					// ���쒆
					ret = FALSE;
				}else{
					s32 alpha;
					s32 alpha_idx;

					alpha_idx = i-WFLBY_GADGET_CRACKEROBJ_CRACKER;

					// �A�j�����I��������A���t�@��OFF�ɂ���

					// �A���t�@�ύX
					if( p_wk->mvwk.cracker.alpha_count[alpha_idx] == 0 ){

						// �I��
						D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );

						// �A���t�@�����ɖ߂�
						NNS_G3dMdlSetMdlAlphaAll( p_sys->mdl[ p_wk->cp_objres[i]->mdl ].pModel, 31 );

					}else{
						// �A�j�����I�������A���t�@�t�F�[�h������
						p_wk->mvwk.cracker.alpha_count[alpha_idx]	--;

						alpha = (p_wk->mvwk.cracker.alpha_count[alpha_idx] * WFLBY_GADGET_CRACKER_ALPHA_OUT_S) / WFLBY_GADGET_CRACKER_ALPHA_OUT_SYNC;

						// �A���t�@��ݒ�
						NNS_G3dMdlSetMdlAlphaAll( p_sys->mdl[p_wk->cp_objres[i]->mdl].pModel, alpha );

						// ���쒆
						ret = FALSE;
					}
				}
			}else{
				ret = FALSE;
			}
		}
	}

	return ret;
}
// ���D
static BOOL WFLBY_GADGET_ANM_Main_Balloon00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	BOOL result;
	
	switch( p_wk->seq ){
	case WFLBY_GADGET_BALLOON_SEQ_UP:		// ���
		result = WFLBY_GADGET_Balloon_MainUp( p_sys, p_wk );
		if( result ){
			p_wk->seq = WFLBY_GADGET_BALLOON_SEQ_DOWN;
			WFLBY_GADGET_Balloon_InitDown( p_sys, p_wk );
		}
		break;
		
	case WFLBY_GADGET_BALLOON_SEQ_DOWN:		// ������
		result = WFLBY_GADGET_Balloon_MainDown( p_sys, p_wk );
		if( result ){
			return TRUE;
		}
		break;
	}

	return FALSE;
}

static BOOL WFLBY_GADGET_ANM_Main_Sparkle00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	BOOL result;
	u32 move_num;
	int i;

	
	// �J�E���g�Ɠ���J�n�����́A���[�v�����ő�l�ȉ��̂Ƃ��ɍs��
	if( (p_wk->mvwk.sparkle.roop_count < p_wk->mvwk.sparkle.roop_num) ){
		
		// �J�E���g����
		p_wk->mvwk.sparkle.count ++;
		if( p_wk->mvwk.sparkle.count >= WFLBY_GADGET_SPARKLE_1ROOP_SYNC ){
			p_wk->mvwk.sparkle.count = 0;
			p_wk->mvwk.sparkle.roop_count++;
		}

		
		// ����J�n�ݒ�
		// ���݂̃J�E���g�l�ł̓��쐔�ɂȂ�܂ŁA�I�u�W�F�̃A�j�����J�n������
		move_num = (p_wk->mvwk.sparkle.count * WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM) / WFLBY_GADGET_SPARKLE_1ROOP_SYNC;
		move_num += p_wk->mvwk.sparkle.roop_count * WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM;
		for( i=p_wk->mvwk.sparkle.buff_num; i<move_num; i++ ){
			D3DOBJ_SetDraw( &p_wk->obj[i], TRUE );	// �`��J�n
		}
		p_wk->mvwk.sparkle.buff_num	= move_num; 
	}


	// ����
	for( i=0; i<p_wk->mvwk.sparkle.buff_num; i++ ){
		if( D3DOBJ_GetDraw( &p_wk->obj[i] ) == TRUE ){
			result = WFLBY_GADGET_Sparkle_OneObj_Main( p_sys, p_wk, i );
			// �A�j���I�����\��
			if( result == TRUE ){
				D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );
			}
		}
	}

	// �I���`�F�b�N
	// �I�u�W�F���Ō�܂ŏo�āA�A�j�����I�������I��
	if( (p_wk->mvwk.sparkle.roop_count == p_wk->mvwk.sparkle.roop_num) &&
		(result == TRUE) ){
		return TRUE;
	}
	return FALSE;
}

// �����S��
static BOOL WFLBY_GADGET_ANM_Main_Onpu00( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	u32 result;
	result =  WFLBY_GADGET_ONPU_Cont_Main( &p_wk->mvwk, p_wk->count );
	p_wk->count ++;
	if( result == WFLBY_GADGET_ONPU_MAIN_ALLEND ){
		return TRUE;
	}
	return FALSE;
}
// �V���o�����C��
static BOOL WFLBY_GADGET_ANM_Main_Cymbals( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	u32 result;
	int i;
	
	result =  WFLBY_GADGET_ONPU_Cont_Main( &p_wk->mvwk, p_wk->count );
	p_wk->count ++;

	// �A�j���J�n�`�F�b�N
	if( result == WFLBY_GADGET_ONPU_MAIN_START ){
		WFLBY_GADGET_Inazuma_Start( p_sys, p_wk );
	}

	// �Ž�σA�j�����C��
	WFLBY_GADGET_Inazuma_Main( p_sys, p_wk );

	if( result == WFLBY_GADGET_ONPU_MAIN_ALLEND ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���̕`��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	���s
 */	
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_ANM_Draw_Obj( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;

	// ���\�[�X�̓C���f�b�N�X�O����߂ē�����Ă���
	if( WFLBY_3DOBJCONT_GetCullingFlag( p_wk->p_person ) == TRUE ){
		// ��`���ɂ��Ȃ��Ȃ�A�o���Ȃ�
		return FALSE;
	}

	for( i=0; i<WFLBY_GADGET_OBJ_MAX; i++ ){
		if( WFLBY_GADGET_OBJ_CheckRes( p_wk, i ) == TRUE ){

			WFLBY_GADGET_OBJ_DrawRes( p_sys, p_wk, i );
		}
	}

	return TRUE;
}






//----------------------------------------------------------------------------
/**
 *	@brief	���D����	�㏸
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	p_wk			���[�N
 *	@param	balloon_num		���D�̐�
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Balloon_InitUp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_num )
{
	VecFx32		matrix;

	// ��l�����`�F�b�N
	{
		if( p_wk->p_person == WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont ) ){
			// �J�������͂���
			WFLBY_CAMERA_ResetTargetPerson( p_sys->p_camera );
		}
	}

	// ��ɉ�����̂��̂��Ȃ����`�F�b�N
	{
		WF2DMAP_POS pos;
		u32 ev_data;
		pos		= WFLBY_3DOBJCONT_GetWkPos( p_wk->p_person );
		ev_data	= WFLBY_MAPCONT_EventGet( p_sys->p_mapcont, WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );
		if( ev_data == WFLBY_MAPEVID_EV_ROOF_MAT ){
			p_wk->mvwk.balloon.roof_flag = TRUE;
		}else{
			p_wk->mvwk.balloon.roof_flag = FALSE;
		}
	}

	// ���f����ǂݍ���
	switch(balloon_num){
	case 1:
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 0, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON00] );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 0, WFLBY_GADGET_BALLOON_ANM_CENTER );
		break;
	case 2:
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 0, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON00] );
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 1, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON01] );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 0, WFLBY_GADGET_BALLOON_ANM_LEFT );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 1, WFLBY_GADGET_BALLOON_ANM_RIGHT );
		break;

	default:
	case 3:
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 0, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON00] );
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 1, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON01] );
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 2, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_BALLOON02] );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 0, WFLBY_GADGET_BALLOON_ANM_CENTER );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 1, WFLBY_GADGET_BALLOON_ANM_LEFT );
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, 2, WFLBY_GADGET_BALLOON_ANM_RIGHT );
		break;

	}

	//
	p_wk->mvwk.balloon.balloon_num	= balloon_num;

	// ��l������X�V�Ȃ�
	WFLBY_3DOBJCONT_DRAW_SetUpdata( p_wk->p_person, FALSE );	
	
	// ��������Ȃ��œ����ς���
	// �Ȃ��[���ʏ퓮��
	// ����[�������Ɋ����
	if( p_wk->mvwk.balloon.roof_flag == FALSE ){
		WFLBY_GADGET_Balloon_InitUp_Normal( p_sys, p_wk );
	}else{
		WFLBY_GADGET_Balloon_InitUp_Roof( p_sys, p_wk );
	}
	
}
// �ʏ�̏�����
static void WFLBY_GADGET_Balloon_InitUp_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32 matrix;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	// ��l�����쏉����
	WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.balloon.straightobj,
			matrix.x, matrix.x, 
			matrix.y, 
			matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_UPDIS[ p_wk->mvwk.balloon.balloon_num-1 ]),
			matrix.z, matrix.z, 
			WFLBY_GADGET_BALLOON_OBJ_UP_SYNC );

	WFLBY_GADGET_MV_SinCurve_Init( &p_wk->mvwk.balloon.sincurveobj, 0, 
			WFLBY_GADGET_BALLOON_OBJ_UPROTSPEED, 
			FX32_CONST( WFLBY_GADGET_BALLOON_OBJ_UPSIDEDIS ) );


	// ���W��ݒ�
	WFLBY_GADGET_Balloon_SetObjPos( p_wk );

	p_wk->mvwk.balloon.count = 0;
}
// ��ɉ���������Ƃ��̏�����
static void WFLBY_GADGET_Balloon_InitUp_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32		matrix;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	// ��l�����쏉����
	WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.balloon.straightobj,
			matrix.x, matrix.x, 
			matrix.y, 
			matrix.y + FX32_CONST(WFLBY_GADGET_BALLOON_OBJ_ROOF_UPDIS),
			matrix.z, matrix.z, 
			WFLBY_GADGET_BALLOON_OBJ_ROOF_UP_SYNC );

	WFLBY_GADGET_MV_SinCurve_Init( &p_wk->mvwk.balloon.sincurveobj, 0, 
			WFLBY_GADGET_BALLOON_OBJ_ROOF_UPROTSPEED, 
			FX32_CONST( WFLBY_GADGET_BALLOON_OBJ_ROOF_UPSIDEDIS ) );


	// ���W��ݒ�
	WFLBY_GADGET_Balloon_SetObjPos_Roof( p_wk );

	p_wk->mvwk.balloon.count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���D����	���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_Balloon_MainUp( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	BOOL ret;
	
	if( p_wk->mvwk.balloon.roof_flag == FALSE ){
		ret = WFLBY_GADGET_Balloon_MainUp_Normal( p_sys, p_wk );
	}else{
		ret = WFLBY_GADGET_Balloon_MainUp_Roof( p_sys, p_wk );
	}
	return ret;
}
// �ʏ퓮��
static BOOL WFLBY_GADGET_Balloon_MainUp_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	BOOL		ret;
	VecFx32		matrix;
	fx32		carve_x;
	int			i;
	
	ret = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.balloon.straightobj, 
			p_wk->mvwk.balloon.count );
	WFLBY_GADGET_MV_SinCurve_Main( &p_wk->mvwk.balloon.sincurveobj );
	p_wk->mvwk.balloon.count ++;
	
	//  �l��ݒ�
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.balloon.straightobj, &matrix.x, &matrix.y, &matrix.z );
	WFLBY_GADGET_MV_SinCurve_GetNum( &p_wk->mvwk.balloon.sincurveobj, &carve_x );

	if( ret == FALSE ){
		matrix.x += carve_x;
	}

	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_person, &matrix );


	// ���W��ݒ�
	WFLBY_GADGET_Balloon_SetObjPos( p_wk );

	// �o���[���A�j��
	for( i=0;  i<p_wk->mvwk.balloon.balloon_num; i++ ){

		WFLBY_GADGET_Balloon_Anm( p_sys, p_wk, i );
	}

	// �L�[�ŕ�����ς���
	if( p_wk->p_person == WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont ) ){
		u32 way = WF2DMAP_WAY_NUM;
		if( sys.trg & PAD_KEY_UP ){
			way = WF2DMAP_WAY_UP;
		}
		if( sys.trg & PAD_KEY_DOWN ){
			way = WF2DMAP_WAY_DOWN;
		}
		if( sys.trg & PAD_KEY_RIGHT ){
			way = WF2DMAP_WAY_RIGHT;
		}
		if( sys.trg & PAD_KEY_LEFT ){
			way = WF2DMAP_WAY_LEFT;
		}
		if( way != WF2DMAP_WAY_NUM ){
			WFLBY_3DOBJCONT_DRAW_SetWay( p_wk->p_person, way );
			WFLBY_3DOBJCONT_SetWkPosAndWay( p_wk->p_person, 
					WFLBY_3DOBJCONT_GetWkPos( p_wk->p_person ), way );
		}
	}

	return ret;
}
// ����������Ƃ��̓���
static BOOL WFLBY_GADGET_Balloon_MainUp_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	BOOL		ret;
	VecFx32		matrix;
	int			i;
	
	ret = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.balloon.straightobj, 
			p_wk->mvwk.balloon.count );
	WFLBY_GADGET_MV_SinCurve_Main( &p_wk->mvwk.balloon.sincurveobj );
	p_wk->mvwk.balloon.count ++;
	
	//  �l��ݒ�
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.balloon.straightobj, &matrix.x, &matrix.y, &matrix.z );

	WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_person, &matrix );


	// ���W��ݒ�
	WFLBY_GADGET_Balloon_SetObjPos_Roof( p_wk );

	// �o���[���A�j��
	for( i=0;  i<p_wk->mvwk.balloon.balloon_num; i++ ){

		WFLBY_GADGET_Balloon_Anm( p_sys, p_wk, i );
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���~����	������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Balloon_InitDown( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	if( p_wk->mvwk.balloon.roof_flag == FALSE ){
		WFLBY_GADGET_Balloon_InitDown_Normal( p_sys, p_wk );
	}else{
		WFLBY_GADGET_Balloon_InitDown_Roof( p_sys, p_wk );
	}
}
// �ʏ퓮��
static void WFLBY_GADGET_Balloon_InitDown_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32		matrix;
	VecFx32		move_matrix;
	int i;
	u32 anmidx;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	// ��l�����쏉����
	WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.balloon.straightobj,
			matrix.x, matrix.x, 
			matrix.y, 
			matrix.y - FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_UPDIS[ p_wk->mvwk.balloon.balloon_num-1 ]),
			matrix.z, matrix.z, 
			WFLBY_GADGET_BALLOON_OBJ_DOWN_SYNC );
	p_wk->mvwk.balloon.count = 0;
	p_wk->mvwk.balloon.bl_seq	= 0;

	// ���D�̓����ݒ�
	for( i=0; i<p_wk->mvwk.balloon.balloon_num; i++ ){
		D3DOBJ_GetMatrix( &p_wk->obj[i], &matrix.x, &matrix.y, &matrix.z );

		anmidx = p_wk->mvwk.balloon.anmidx[i];
		switch( anmidx ){
		case WFLBY_GADGET_BALLOON_ANM_CENTER:
			move_matrix.x = matrix.x;
			move_matrix.y = matrix.y + WFLBY_GADGET_BALLOON_OBJ_MOVE_Y;
			move_matrix.z = matrix.z - WFLBY_GADGET_BALLOON_OBJ_MOVE_Y;
			break;
			
		case WFLBY_GADGET_BALLOON_ANM_RIGHT:
			move_matrix.x = matrix.x + WFLBY_GADGET_BALLOON_OBJ_MOVE_X;
			move_matrix.y = matrix.y + WFLBY_GADGET_BALLOON_OBJ_MOVE_Y;
			move_matrix.z = matrix.z;
			break;
			
		case WFLBY_GADGET_BALLOON_ANM_LEFT:
			move_matrix.x = matrix.x - WFLBY_GADGET_BALLOON_OBJ_MOVE_X;
			move_matrix.y = matrix.y + WFLBY_GADGET_BALLOON_OBJ_MOVE_Y;
			move_matrix.z = matrix.z;
			break;
		}
		
		WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.balloon.straight[i],
				matrix.x, move_matrix.x, 
				matrix.y, move_matrix.y,
				matrix.z, move_matrix.z, 
				WFLBY_GADGET_BALLOON_OBJ_WAIT );

		// ���A�j��
		WFLBY_GADGET_MV_SinCurve_Init( &p_wk->mvwk.balloon.sincurve[i], 0, 
				WFLBY_GADGET_BALLOON_OBJ_ROT_S, 
				WFLBY_GADGET_BALLOON_OBJ_ROT_DIS );

		// �A�j���ύX
		WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, i, WFLBY_GADGET_BALLOON_ANM_OTHER );
	}

	p_wk->mvwk.balloon.balloon_fly_count = 0;
	
}
// �������蓮��
static void WFLBY_GADGET_Balloon_InitDown_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32		matrix;
	VecFx32		move_matrix;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	// ��l�����쏉����
	WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.balloon.straightobj,
			matrix.x, matrix.x, 
			matrix.y, 
			matrix.y - FX32_CONST(WFLBY_GADGET_BALLOON_OBJ_ROOF_UPDIS),
			matrix.z, matrix.z, 
			WFLBY_GADGET_BALLOON_OBJ_ROOF_DOWN_SYNC );
	p_wk->mvwk.balloon.count = 0;
	p_wk->mvwk.balloon.bl_seq	= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���~����	���C��
 *
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_Balloon_MainDown( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	BOOL ret;
	
	if( p_wk->mvwk.balloon.roof_flag == FALSE ){
		ret = WFLBY_GADGET_Balloon_MainDown_Normal( p_sys, p_wk );
	}else{
		ret = WFLBY_GADGET_Balloon_MainDown_Roof( p_sys, p_wk );
	}
	return ret;
}
// �ʏ퓮��
static BOOL WFLBY_GADGET_Balloon_MainDown_Normal( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	int i;

	switch( p_wk->mvwk.balloon.bl_seq ){
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_WAIT:		// �o�^�o�^�J�n�܂�
		p_wk->mvwk.balloon.count ++;
		if( p_wk->mvwk.balloon.count >= WFLBY_GADGET_BALLOON_OBJ_BATA_SWAIT ){
			p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_START;
		}
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_START:
		WFLBY_3DOBJCONT_DRAW_SetAnmBata( p_wk->p_person, TRUE );
		p_wk->mvwk.balloon.count = WFLBY_GADGET_BALLOON_OBJ_BATA_WAIT;

		p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_MAIN;
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_MAIN:
		p_wk->mvwk.balloon.count--;
		if( p_wk->mvwk.balloon.count <= 0 ){
			WFLBY_3DOBJCONT_DRAW_SetAnmBata( p_wk->p_person, FALSE );
			p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_DOWN;
			p_wk->mvwk.balloon.count = 0;

			// �����Ă����������������
			{
				u32 way;

				way = WFLBY_3DOBJCONT_GetWkObjData( p_wk->p_person, WF2DMAP_OBJPM_WAY );
				WFLBY_3DOBJCONT_DRAW_SetWay( p_wk->p_person, way );
			}
		}
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_DOWN:
		{
			BOOL		result;
			VecFx32		matrix;
			
			result = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.balloon.straightobj, 
					p_wk->mvwk.balloon.count );
			p_wk->mvwk.balloon.count ++;
			
			//  �l��ݒ�
			WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
			WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.balloon.straightobj, &matrix.x, &matrix.y, &matrix.z );
			WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_person, &matrix );

			if( result == TRUE ){
				p_wk->mvwk.balloon.bl_seq		= WFLBY_GADGET_BALLOON_DOWNSEQ_DON_MAIN;
				p_wk->mvwk.balloon.count		= 0;
				p_wk->mvwk.balloon.don_start_y	= matrix.y;

				// ���n��
				WFLBY_GADGET_SePlay( p_sys, p_wk, WFLBY_SND_BALLOON_02 );
			}
		}
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_DON_MAIN:
		{
			u16 rot;
			fx32 dis;
			VecFx32	matrix;
			
			rot = ( 0x7fff * p_wk->mvwk.balloon.count ) / WFLBY_GADGET_BALLOON_OBJ_DON_SYNC;
			dis = FX_Mul( FX_SinIdx( rot ), WFLBY_GADGET_BALLOON_OBJ_DON_DIS );

			WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
			matrix.y = p_wk->mvwk.balloon.don_start_y+dis;
			WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_person, &matrix );

			if( (p_wk->mvwk.balloon.count + 1) > WFLBY_GADGET_BALLOON_OBJ_DON_SYNC ){

				// ��l������X�V����
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_wk->p_person, TRUE );	


				// ��l�����`�F�b�N
				{
					if( p_wk->p_person == WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont ) ){
						// �J������������
						WFLBY_CAMERA_SetTargetPerson( p_sys->p_camera, p_wk->p_person );
					}
				}
				
				return TRUE;
			}else{
				p_wk->mvwk.balloon.count ++;
			}
		}
		break;
	}

	// �o���[���A�j��
	p_wk->mvwk.balloon.balloon_fly_count++;
	for( i=0;  i<p_wk->mvwk.balloon.balloon_num; i++ ){

		// ���W�ݒ�
		{
			BOOL		ret;
			VecFx32		matrix;
			fx32		carve_x;
			BOOL		result;
			
			ret = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.balloon.straight[i], 
					p_wk->mvwk.balloon.balloon_fly_count );
			if( ret == FALSE ){
				WFLBY_GADGET_MV_SinCurve_Main( &p_wk->mvwk.balloon.sincurve[i] );

				//  �l��ݒ�
				D3DOBJ_GetMatrix( &p_wk->obj[i], &matrix.x, &matrix.y, &matrix.z );
				WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.balloon.straight[i], &matrix.x, &matrix.y, &matrix.z );
				WFLBY_GADGET_MV_SinCurve_GetNum( &p_wk->mvwk.balloon.sincurve[i], &carve_x );

				matrix.x += carve_x;

				D3DOBJ_SetMatrix( &p_wk->obj[i], matrix.x, matrix.y,  matrix.z );
			}else{
				// �j��
				result = WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, i, WFLBY_GADGET_BALLOON_ANM_CRASH );

				// �j��
				if( result == TRUE ){
					WFLBY_GADGET_SePlay( p_sys, p_wk, WFLBY_SND_BALLOON_01 );
				}
			}
		}
		
		WFLBY_GADGET_Balloon_Anm( p_sys, p_wk, i );
	}

	return FALSE;
}
// ����������Ƃ��̓���
static BOOL WFLBY_GADGET_Balloon_MainDown_Roof( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk )
{
	int i;
	
	// ���D�A�j���͏�ɓ�����
	for( i=0;  i<p_wk->mvwk.balloon.balloon_num; i++ ){
		WFLBY_GADGET_Balloon_Anm( p_sys, p_wk, i );
	}
	
	switch( p_wk->mvwk.balloon.bl_seq ){
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_WAIT:		// �o�^�o�^�J�n�܂��͂Ȃ�
		WFLBY_GADGET_SePlay( p_sys, p_wk, WFLBY_SND_BALLOON_01 );	// ���D�����ꂽ���ƕ��D������
		for( i=0; i<p_wk->mvwk.balloon.balloon_num; i++ ){
			WFLBY_GADGET_Balloon_SetAnm( p_sys, p_wk, i, WFLBY_GADGET_BALLOON_ANM_CRASH );
		}
		p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_START;
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_START:
		WFLBY_3DOBJCONT_DRAW_SetAnmBata( p_wk->p_person, TRUE );
		p_wk->mvwk.balloon.count = WFLBY_GADGET_BALLOON_OBJ_ROOF_BATA_WAIT;

		p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_MAIN;
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_BATA_MAIN:
		p_wk->mvwk.balloon.count--;
		if( p_wk->mvwk.balloon.count <= 0 ){
			WFLBY_3DOBJCONT_DRAW_SetAnmBata( p_wk->p_person, FALSE );
			p_wk->mvwk.balloon.bl_seq = WFLBY_GADGET_BALLOON_DOWNSEQ_DOWN;
			p_wk->mvwk.balloon.count = 0;

			// �����Ă����������������
			{
				u32 way;

				way = WFLBY_3DOBJCONT_GetWkObjData( p_wk->p_person, WF2DMAP_OBJPM_WAY );
				WFLBY_3DOBJCONT_DRAW_SetWay( p_wk->p_person, way );
			}
		}
		break;
		
	case WFLBY_GADGET_BALLOON_DOWNSEQ_DOWN:
		{
			BOOL		result;
			VecFx32		matrix;
			
			result = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.balloon.straightobj, 
					p_wk->mvwk.balloon.count );
			p_wk->mvwk.balloon.count ++;
			
			//  �l��ݒ�
			WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
			WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.balloon.straightobj, &matrix.x, &matrix.y, &matrix.z );
			WFLBY_3DOBJCONT_DRAW_Set3DMatrix( p_wk->p_person, &matrix );

			if( result == TRUE ){
				p_wk->mvwk.balloon.count		= 0;
				p_wk->mvwk.balloon.don_start_y	= matrix.y;

				// ���n��
				WFLBY_GADGET_SePlay( p_sys, p_wk, WFLBY_SND_BALLOON_02 );

				// ��l������X�V����
				WFLBY_3DOBJCONT_DRAW_SetUpdata( p_wk->p_person, TRUE );	


				// ��l�����`�F�b�N
				{
					if( p_wk->p_person == WFLBY_3DOBJCONT_GetPlayer( p_sys->p_objcont ) ){
						// �J������������
						WFLBY_CAMERA_SetTargetPerson( p_sys->p_camera, p_wk->p_person );
					}
				}
				
				return TRUE;
			}
		}
		break;
		
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̈ʒu�ɕ��D��`��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Balloon_SetObjPos( WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32 matrix;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	switch( p_wk->mvwk.balloon.balloon_num ){
	case 1:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[2].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[2].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );
		break;
		
	case 2:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[0].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[0].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[1], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[1].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[1].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );
		break;
		
	default:
	case 3:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[2].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[2].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[1], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[0].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[0].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[2], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[1].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_POS[1].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_POS_Z );


		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l���̈ʒu�ɕ��D��`��	��������o�[�W����
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Balloon_SetObjPos_Roof( WFLBY_GADGET_OBJ*  p_wk )
{
	VecFx32 matrix;

	// ���W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	
	switch( p_wk->mvwk.balloon.balloon_num ){
	case 1:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[2].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[2].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );
		break;
		
	case 2:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[0].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[0].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[1], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[1].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[1].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );
		break;
		
	default:
	case 3:
		D3DOBJ_SetMatrix( &p_wk->obj[0], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[2].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[2].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[1], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[0].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[0].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );

		D3DOBJ_SetMatrix( &p_wk->obj[2], 
				matrix.x + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[1].x),
				matrix.y + FX32_CONST(sc_WFLBY_GADGET_BALLOON_OBJ_ROOF_POS[1].y),
				matrix.z + WFLBY_GADGET_BALLOON_OBJ_ROOF_POS_Z );
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���ݒ�
 *
 *	@param	p_sys			�K�W�F�b�g�V�X�e��
 *	@param	p_wk			���[�N
 *	@param	balloon_idx		���D�C���f�b�N�X
 *	@param	anmidx			�A�j���C���f�b�N�X
 *
 *	@retval	TRUE	�ݒ肵��
 *	@retval	FALSE	�ݒ肵�Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_Balloon_SetAnm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_idx, u32 anmidx )
{
	if( p_wk->mvwk.balloon.anmidx[ balloon_idx ] != anmidx ){
		p_wk->mvwk.balloon.anmidx[ balloon_idx ] = anmidx;
		p_wk->mvwk.balloon.anm_wait[ balloon_idx ] = 0;
		WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, balloon_idx, 0, (anmidx*WFLBY_GADGET_BALLOON_ANM_ONEFR) );
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���D�A�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	balloon_idx	���D�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Balloon_Anm( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ*  p_wk, u32 balloon_idx )
{
	fx32 frame;
	u32 anmidx;

	anmidx = p_wk->mvwk.balloon.anmidx[balloon_idx];
	
	if( p_wk->mvwk.balloon.anm_wait[balloon_idx] <= 0 ){
		frame = WFLBY_GADGET_OBJ_GetFrame( p_sys, p_wk, balloon_idx, 0 );
		if( frame == (anmidx*WFLBY_GADGET_BALLOON_ANM_ONEFR) ){
			frame = (anmidx*WFLBY_GADGET_BALLOON_ANM_ONEFR) + FX32_ONE;
		}else{

			// �j��A�j���́A�I����������
			if( anmidx != WFLBY_GADGET_BALLOON_ANM_CRASH ){
			
				frame = (anmidx*WFLBY_GADGET_BALLOON_ANM_ONEFR);
			}else{
				D3DOBJ_SetDraw( &p_wk->obj[balloon_idx], FALSE );
			}
		}

		WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, balloon_idx, 0, frame );
		 
		p_wk->mvwk.balloon.anm_wait[balloon_idx] = WFLBY_GADGET_BALLOON_ANM_WAIT;
	}

	p_wk->mvwk.balloon.anm_wait[balloon_idx]--;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ρ[����	������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 *	@param	roop_num	���[�v��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Sparkle_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 roop_num )
{
	int i, j;
	VecFx32 matrix;
	VecFx32 set_matrix;
	VecFx32 end_matrix;
	u32 idx;
	u32 objidx;
	
	// ��l�����W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );


	for( i=0; i<roop_num; i++ ){
		objidx = WFLBY_GADGET_OBJ_SPARKLE00+i;
		if( objidx > WFLBY_GADGET_OBJ_SPARKLE02 ){
			objidx = WFLBY_GADGET_OBJ_SPARKLE02;
		}
		
		for( j=0; j<WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM; j++ ){
			
			idx = (i*WFLBY_GADGET_SPARKLE_1ROOP_OBJNUM)+j;
			WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 
					idx,
					&sc_WFLBY_GADGET_RES[objidx] );
			VEC_Add( &matrix, 
					&sc_WFLBY_GADGET_SPARKLE_OFFS[ j ], &set_matrix );
			VEC_Add( &set_matrix, 
					&sc_WFLBY_GADGET_SPARKLE_MOVE[ j ], &end_matrix );
			D3DOBJ_SetMatrix( &p_wk->obj[idx], 
					set_matrix.x, set_matrix.y, set_matrix.z );

			D3DOBJ_SetDraw( &p_wk->obj[idx], FALSE );

			// �ړ��p�����[�^�ݒ�
			p_wk->mvwk.sparkle.objcount[idx] = 0;
			WFLBY_GADGET_MV_Straight_Init( &p_wk->mvwk.sparkle.objstraight[idx],
					set_matrix.x, end_matrix.x, 
					set_matrix.y, end_matrix.y, 
					set_matrix.z, end_matrix.z, 
					WFLBY_GADGET_SPARKLE_ANM_SYNC );
		}	
	}
	
	p_wk->mvwk.sparkle.roop_num		= roop_num;
	p_wk->mvwk.sparkle.roop_count	= 0;
	p_wk->mvwk.sparkle.count		= 0;
	p_wk->mvwk.sparkle.buff_num		= 0;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�X�p�[�N��	�P�I�u�W�F����
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 *	@param	objidx		�I�u�W�F�C���f�b�N�X
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_Sparkle_OneObj_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 objidx )
{
	BOOL result;
	VecFx32 move;

	p_wk->mvwk.sparkle.objcount[objidx]++;
		
	// �ړ�����
	result = WFLBY_GADGET_MV_Straight_Main( &p_wk->mvwk.sparkle.objstraight[objidx],
			p_wk->mvwk.sparkle.objcount[objidx]	);

	// �ړ��l�ݒ�
	WFLBY_GADGET_MV_Straight_GetNum( &p_wk->mvwk.sparkle.objstraight[objidx], 
			&move.x, &move.y, &move.z );

	D3DOBJ_SetMatrix( &p_wk->obj[objidx], move.x, move.y, move.z );

	//  �A�j�����[�v
	WFLBY_GADGET_OBJ_LoopAnm( p_sys, p_wk, objidx, 0 );

	return result;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���b�v������̏�����
 *
 *	@param	p_sys			�V�X�e��
 *	@param	p_wk			���[�N
 *	@param	ripple_num		���b�v����
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Ripple_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 ripple_num )
{
	int i;
	VecFx32 matrix;


	for( i=0; i<ripple_num; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_RIPPLE00 + i] );

		// ���W�����킹��
		WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
		// �ݒ�
		matrix.y += WFLBY_GADGET_FLOOR_Y;
		matrix.z += WFLBY_GADGET_RIPPLE_MAT_Z;
		D3DOBJ_SetMatrix( &p_wk->obj[i], matrix.x, matrix.y, matrix.z );

		// ��\��
		D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );
	}

	p_wk->mvwk.ripple.ripple_num	= ripple_num;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�V�O�i���I�u�W�F	���C������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 *	@param	num			�\������I�u�W�F�̎��
 *
 *	@retval	TRUE	�I��
 *	@retval	FALS	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_Signal_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 num )
{
	BOOL result;
	u32	move_num;
	WFLBY_GADGET_SINGNAL_ANMDATA	data;

	// ����
	move_num = (p_wk->count * WFLBY_GADGET_SIGNAL_ANM_FRAME_NUM) / WFLBY_GADGET_SIGNAL_ANM_SYNC;
	if( move_num != p_wk->mvwk.signal.lastnum ){
		p_wk->mvwk.signal.lastnum = move_num;

		// ���܂ŕ\�����Ă��̂��\���ɂ���
		if( move_num > 0 ){
			data = sc_WFLBY_GADGET_SIGNAL_ANMSEQ[ num ][ move_num-1 ];
			D3DOBJ_SetDraw( &p_wk->obj[data.objidx], FALSE );
		}

		data = sc_WFLBY_GADGET_SIGNAL_ANMSEQ[ num ][ move_num ];

		// �A�j���Đ�
		if( WFLBY_GADGET_SINGNAL_ANM_OFF != data.anm_data ){
			WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, data.objidx, 0, 
					FX32_CONST( data.anm_data ) );
			WFLBY_GADGET_OBJ_SetFrame( p_sys, p_wk, data.objidx, 1, 
					FX32_CONST( data.anm_data ) );

			D3DOBJ_SetDraw( &p_wk->obj[data.objidx], TRUE );
		}else{
			D3DOBJ_SetDraw( &p_wk->obj[data.objidx], FALSE );
		}
	}
	
	// �J�E���g
	if( p_wk->count < WFLBY_GADGET_SIGNAL_ANM_SYNC ){
		p_wk->count ++;
	}

	// �I���`�F�b�N
	if( (p_wk->count >= WFLBY_GADGET_SIGNAL_ANM_SYNC) ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�X�E�B���O	������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 *	@param	num			�I�u�W�F��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Swing_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk, u32 num )
{
	int i;
	VecFx32 matrix;


	for( i=0; i<num; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, i, &sc_WFLBY_GADGET_RES[WFLBY_GADGET_OBJ_SWING00 + i] );

		// ���W�����킹��
		WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
		// �ݒ�
		matrix.y += WFLBY_GADGET_FLOOR_Y;
		D3DOBJ_SetMatrix( &p_wk->obj[i], matrix.x, matrix.y, matrix.z );

		// ��\��
		D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );
	}

	p_wk->mvwk.swing.lastnum	= 0xff;
	p_wk->mvwk.swing.objnum		= num;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�V���o���Ž��	������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Inazuma_Init( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	VecFx32 matrix;
	int i;

	// ��ȍ쐬
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( p_wk->p_person, &matrix );
	matrix.z += WFLBY_GADGET_CYMBALS_INAZUMA_OFS_Z;
	matrix.x += WFLBY_GADGET_CYMBALS_INAZUMA_OFS_X;
	matrix.y += WFLBY_GADGET_AIR_Y + WFLBY_GADGET_CYMBALS_INAZUMA_OFS_Y;
	for( i=0; i<WFLBY_GADGET_CYMBALS_INAZUMA_NUM; i++ ){
		WFLBY_GADGET_OBJ_SetRes( p_sys, p_wk, 
				WFLBY_GADGET_CYMBALS_INAZUMA_00+i, &sc_WFLBY_GADGET_RES[ WFLBY_GADGET_OBJ_SPARK00+i ] );

		// ���W�ݒ�
		D3DOBJ_SetMatrix( &p_wk->obj[i+WFLBY_GADGET_CYMBALS_INAZUMA_00], 
				matrix.x + (i*WFLBY_GADGET_CYMBALS_INAZUMA_DIS_X),
				matrix.y, matrix.z );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ž�σA�j��	�J�n
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Inazuma_Start( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;
	
	p_wk->mvwk.onpu.inazuma_move	= TRUE;
	p_wk->mvwk.onpu.inazuma_count	= 0;

	for( i=0; i<WFLBY_GADGET_CYMBALS_INAZUMA_NUM; i++ ){
		D3DOBJ_SetDraw( &p_wk->obj[i+WFLBY_GADGET_CYMBALS_INAZUMA_00], TRUE );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ž�σA�j��	���C��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_Inazuma_Main( WFLBY_GADGET* p_sys, WFLBY_GADGET_OBJ* p_wk )
{
	int i;

	if( p_wk->mvwk.onpu.inazuma_move == FALSE ){
		return ;
	}

	// �J�E���g����
	if( p_wk->mvwk.onpu.inazuma_count < WFLBY_GADGET_CYMBALS_INAZUMA_ANM_SYNC ){
		p_wk->mvwk.onpu.inazuma_count++;
	}else{
		// �����~
		p_wk->mvwk.onpu.inazuma_move = FALSE;
	}

	// ���[�v�A�j��
	for( i=0; i<WFLBY_GADGET_CYMBALS_INAZUMA_NUM; i++ ){
		if( p_wk->mvwk.onpu.inazuma_count < WFLBY_GADGET_CYMBALS_INAZUMA_ANM_SYNC ){
			WFLBY_GADGET_OBJ_LoopAnm( p_sys, p_wk, i+WFLBY_GADGET_CYMBALS_INAZUMA_00, 0 );
		}else{
			D3DOBJ_SetDraw( &p_wk->obj[i+WFLBY_GADGET_CYMBALS_INAZUMA_00], FALSE );
		}
	}

	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������˃V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	data_num	�f�[�^��
 *	@param	count_max	�J�E���g�ő�l
 *	@param	anm_type	�A�j���^�C�v
 *	@param	p_gadget	�K�W�F�b�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_ONPU_Cont_Init( WFLBY_GADGET_OBJWK* p_wk, const WFLBY_GADGET_MOVE_ONPU_DATA* cp_data, u32 data_num, u32 count_max, u32 anm_type, WFLBY_GADGET_OBJ* p_gadget )
{
	int i;
	
	p_wk->onpu.cp_data		= cp_data;
	p_wk->onpu.data_num		= data_num;
	p_wk->onpu.count_max	= count_max;

	// ����p�����[�^�̏�����
	for( i=0; i<WFLBY_GADGET_MOVE_ONPU_WK_NUM; i++ ){
		WFLBY_GADGET_OnpuMove_Init( &p_wk->onpu.move[i], 
				&p_gadget->obj[(i*WFLBY_GADGET_ONPU_OBJ_NUM)+0],
				&p_gadget->obj[(i*WFLBY_GADGET_ONPU_OBJ_NUM)+1],
				&p_gadget->obj[(i*WFLBY_GADGET_ONPU_OBJ_NUM)+2],
				&p_gadget->obj[(i*WFLBY_GADGET_ONPU_OBJ_NUM)+3],
				p_gadget->p_person, anm_type );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������˃V�X�e��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	count		�J�E���g
 *
 *	@retval	WFLBY_GADGET_ONPU_MAIN_PLAY,		// �Đ���
 *	@retval	WFLBY_GADGET_ONPU_MAIN_START,		// �Đ����ŉ����𔭎˂���
 *	@retval	WFLBY_GADGET_ONPU_MAIN_ALLEND,		// �S���I�����
 */
//-----------------------------------------------------------------------------
static WFLBY_GADGET_ONPU_MAIN_RET WFLBY_GADGET_ONPU_Cont_Main( WFLBY_GADGET_OBJWK* p_wk, u32 count )
{
	int i;
	WFLBY_GADGET_MOVE_ONPU_DATA data;
	BOOL result;
	BOOL all_end;
	BOOL ret = WFLBY_GADGET_ONPU_MAIN_PLAY;
	
	// count�̃J�E���g�l�œ�����J�n����f�[�^���������瓮����
	for( i=0; i<p_wk->onpu.data_num; i++ ){
		data = p_wk->onpu.cp_data[i];
		if( data.count == count ){
			GF_ASSERT( data.anmidx < WFLBY_GADGET_MOVE_ONPU_WK_NUM );
			GF_ASSERT( data.mvnum <= WFLBY_GADGET_ONPU_OBJ_NUM );
			WFLBY_GADGET_OnpuMove_Start( &p_wk->onpu.move[ data.anmidx ], data.mvnum );
			ret = WFLBY_GADGET_ONPU_MAIN_START;
		}
	}

	// ���상�C��
	all_end = TRUE;
	for( i=0; i<WFLBY_GADGET_MOVE_ONPU_WK_NUM; i++ ){
		result = WFLBY_GADGET_OnpuMove_Main( &p_wk->onpu.move[ i ] );
		if( result == FALSE ){	// �����Ă���̂�����̂ł܂��I������Ⴂ�����Ȃ�
			all_end = FALSE;
			
		}
	}


	// �I���`�F�b�N
	if( (p_wk->onpu.count_max <= count) && (all_end == TRUE) ){
		ret = WFLBY_GADGET_ONPU_MAIN_ALLEND;
	}
	return ret;
}



//----------------------------------------------------------------------------
/**
 *	@brief	����̊J�n�֐�
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj0		�`��I�u�W�F�O
 *	@param	p_obj1		�`��I�u�W�F�P
 *	@param	p_obj2		�`��I�u�W�F�Q
 *	@param	p_obj3		�`��I�u�W�F�R
 *	@param	cp_person	��l��
 *	@param	anm_type	�A�j���^�C�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_Init( WFLBY_GADGET_ONPU* p_wk, D3DOBJ* p_obj0, D3DOBJ* p_obj1, D3DOBJ* p_obj2, D3DOBJ* p_obj3, const WFLBY_3DPERSON* cp_person, u32 anm_type )
{
	GF_ASSERT( anm_type < WFLBY_GADGET_ONPU_MOVE_NUM );

	// �\������I�u�W�F�̕\���t���O��ݒ�
	p_wk->p_obj[0] = p_obj0;
	p_wk->p_obj[1] = p_obj1;
	p_wk->p_obj[2] = p_obj2;
	p_wk->p_obj[3] = p_obj3;

	p_wk->cp_person = cp_person;

	
	// �x�����쏉����
	p_wk->type	= anm_type;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��������	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	num			��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_Start( WFLBY_GADGET_ONPU* p_wk, u32 num )
{
	int i;
	static void (* const p_Func[WFLBY_GADGET_ONPU_MOVE_NUM])( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person ) = {
		WFLBY_GADGET_OnpuMove_InitBell,
		WFLBY_GADGET_OnpuMove_InitDram,
		WFLBY_GADGET_OnpuMove_InitCymbals,
	};

	// �\���J�n
	for( i=0; i<num; i++ ){
		D3DOBJ_SetDraw( p_wk->p_obj[i], TRUE );
	}

	p_wk->count	= 0;
	p_wk->move	= TRUE;
	p_wk->mvnum	= num;

	p_Func[ p_wk->type ]( p_wk, p_wk->cp_person );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������상�C���֐�
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OnpuMove_Main( WFLBY_GADGET_ONPU* p_wk )
{
	int i;
	BOOL result;
	static BOOL (* const p_Func[WFLBY_GADGET_ONPU_MOVE_NUM])( WFLBY_GADGET_ONPU* p_wk ) = {
		WFLBY_GADGET_OnpuMove_MainBell,
		WFLBY_GADGET_OnpuMove_MainDram,
		WFLBY_GADGET_OnpuMove_MainCymbals,
	};

	if( p_wk->move == FALSE ){
		return TRUE;
	}

	// ���C������
	result = p_Func[ p_wk->type ]( p_wk );

	// �J�E���g
	p_wk->count ++;

	// �I���ɂȂ����珈�����I��
	if( result == TRUE ){
		// �\��OFF
		for( i=0; i<p_wk->mvnum; i++ ){
			D3DOBJ_SetDraw( p_wk->p_obj[i], FALSE );
		}
		p_wk->move = FALSE;
	}

	return result;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�x������̊J�n
 *	
 *	@param	p_wk		���[�N
 *	@param	cp_person	��l��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_InitBell( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person )
{
	int i;
	VecFx32 matrix;
	VecFx32 end_matrix;
	u16	start_rot;

	// ��l���̍��W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( cp_person, &matrix );
	matrix.x += WFLBY_GADGET_ONPU_X_OFS;
	matrix.y += WFLBY_GADGET_AIR_Y;
	matrix.z += WFLBY_GADGET_ONPU_Z_OFS;
		
	for( i=0; i<p_wk->mvnum; i++ ){
		switch( i ){
		case 0:
			end_matrix.x = matrix.x - WFLBY_GADGET_ONPU_MOVE_BELL_X1;
			end_matrix.y = matrix.y + WFLBY_GADGET_ONPU_MOVE_BELL_Y;
			end_matrix.z = matrix.z + 0;
			start_rot	 = FX_GET_ROTA_NUM(0);
			break;
		case 1:
			end_matrix.x = matrix.x + WFLBY_GADGET_ONPU_MOVE_BELL_X1;
			end_matrix.y = matrix.y + WFLBY_GADGET_ONPU_MOVE_BELL_Y;
			end_matrix.z = matrix.z + 0;
			start_rot	 = FX_GET_ROTA_NUM(180);
			break;
		case 2:
			end_matrix.x = matrix.x - WFLBY_GADGET_ONPU_MOVE_BELL_X0;
			end_matrix.y = matrix.y + WFLBY_GADGET_ONPU_MOVE_BELL_Y;
			end_matrix.z = matrix.z - WFLBY_GADGET_ONPU_MOVE_BELL_Z;
			start_rot	 = FX_GET_ROTA_NUM(180);
			break;
		case 3:
			end_matrix.x = matrix.x + WFLBY_GADGET_ONPU_MOVE_BELL_X0;
			end_matrix.y = matrix.y + WFLBY_GADGET_ONPU_MOVE_BELL_Y;
			end_matrix.z = matrix.z - WFLBY_GADGET_ONPU_MOVE_BELL_Z;
			start_rot	 = FX_GET_ROTA_NUM(0);
			break;
		}
		WFLBY_GADGET_MV_Straight_Init( &p_wk->st[i], 
				matrix.x, end_matrix.x,
				matrix.y, end_matrix.y,
				matrix.z, end_matrix.z,
				WFLBY_GADGET_ONPU_MOVE_BELL_SYNC );
		WFLBY_GADGET_MV_SinCurve_Init( &p_wk->cv[i], 
				start_rot, WFLBY_GADGET_ONPU_MOVE_BELL_CVSP,
				WFLBY_GADGET_ONPU_MOVE_BELL_CVX );
		D3DOBJ_SetMatrix( p_wk->p_obj[i], matrix.x, matrix.y, matrix.z );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�x�����상�C���֐�
 *
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OnpuMove_MainBell( WFLBY_GADGET_ONPU* p_wk )
{
	int		i;
	BOOL	result;
	VecFx32	matrix;
	fx32	cv_num;

	for( i=0; i<p_wk->mvnum; i++ ){
		// �J�[�u�ړ�
		WFLBY_GADGET_MV_SinCurve_Main( &p_wk->cv[i] );
		
		// �����ړ�
		result = WFLBY_GADGET_MV_Straight_Main( &p_wk->st[i], p_wk->count );

		// ���W�̐ݒ�
		WFLBY_GADGET_MV_SinCurve_GetNum( &p_wk->cv[i], &cv_num );
		WFLBY_GADGET_MV_Straight_GetNum( &p_wk->st[i], &matrix.x, &matrix.y, &matrix.z );
		matrix.x += cv_num;
		D3DOBJ_SetMatrix( p_wk->p_obj[i], matrix.x, matrix.y, matrix.z );
	}

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�h��������J�n
 *
 *	@param	p_wk		���[�N
 *	@param	cp_person	��l��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_InitDram( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person )
{
	int i;
	VecFx32 matrix;
	VecFx32 end_matrix;

	// ��l���̍��W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( cp_person, &matrix );
	matrix.x += WFLBY_GADGET_ONPU_X_OFS;
	matrix.y += WFLBY_GADGET_FLOOR_Y;
	matrix.z += WFLBY_GADGET_ONPU_Z_OFS;
		
	for( i=0; i<p_wk->mvnum; i++ ){
		switch( i ){
		case 0:
			end_matrix.x = matrix.x - WFLBY_GADGET_ONPU_MOVE_DRUM_X;
			end_matrix.y = matrix.y;
			end_matrix.z = matrix.z + WFLBY_GADGET_ONPU_MOVE_DRUM_Z;
			break;
		case 1:
			end_matrix.x = matrix.x + WFLBY_GADGET_ONPU_MOVE_DRUM_X;
			end_matrix.y = matrix.y;
			end_matrix.z = matrix.z + WFLBY_GADGET_ONPU_MOVE_DRUM_Z;
			break;
		case 2:
			if( p_wk->mvnum == 3 ){
				end_matrix.x = matrix.x;
				end_matrix.y = matrix.y;
				end_matrix.z = matrix.z - (WFLBY_GADGET_ONPU_MOVE_DRUM_Z + WFLBY_GADGET_ONPU_MOVE_DRUM_X);
			}else{
				end_matrix.x = matrix.x - WFLBY_GADGET_ONPU_MOVE_DRUM_X;
				end_matrix.y = matrix.y;
				end_matrix.z = matrix.z - WFLBY_GADGET_ONPU_MOVE_DRUM_Z;
			}
			break;
		case 3:
			end_matrix.x = matrix.x + WFLBY_GADGET_ONPU_MOVE_DRUM_X;
			end_matrix.y = matrix.y;
			end_matrix.z = matrix.z - WFLBY_GADGET_ONPU_MOVE_DRUM_Z;
			break;
		}
		WFLBY_GADGET_MV_Straight_Init( &p_wk->st[i], 
				matrix.x, end_matrix.x,
				matrix.y, end_matrix.y,
				matrix.z, end_matrix.z,
				WFLBY_GADGET_ONPU_MOVE_DRUM_XZ_SYNC );
		WFLBY_GADGET_MV_SinCurve_Init( &p_wk->cv[i], 
				WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_ROT, 
				WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SP,
				WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_Y );
	}

	// ���W�ݒ�	
	WFLBY_GADGET_OnpuMove_MainDram( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�h�������상�C���֐�
 *
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OnpuMove_MainDram( WFLBY_GADGET_ONPU* p_wk )
{
	int		i;
	BOOL	result;
	VecFx32	matrix;
	fx32	cv_num;

	for( i=0; i<p_wk->mvnum; i++ ){

		// �J�E���^��WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SYNC�ɂȂ������]�ړ��̃p�����[�^�ύX
		if( p_wk->count == WFLBY_GADGET_ONPU_MOVE_DRUM_DOWN_SYNC ){
			WFLBY_GADGET_MV_SinCurve_Init( &p_wk->cv[i], 
					WFLBY_GADGET_ONPU_MOVE_DRUM_UP_ROT, 
					WFLBY_GADGET_ONPU_MOVE_DRUM_UP_SP,
					WFLBY_GADGET_ONPU_MOVE_DRUM_UP_Y );
		}else{

			// �J�[�u�ړ�
			WFLBY_GADGET_MV_SinCurve_Main( &p_wk->cv[i] );
		}
		
		// �����ړ�
		WFLBY_GADGET_MV_Straight_Main( &p_wk->st[i], p_wk->count );

		// ���W�̐ݒ�
		WFLBY_GADGET_MV_SinCurve_GetNum( &p_wk->cv[i], &cv_num );
		WFLBY_GADGET_MV_Straight_GetNum( &p_wk->st[i], &matrix.x, &matrix.y, &matrix.z );
		matrix.y += cv_num;
		D3DOBJ_SetMatrix( p_wk->p_obj[i], matrix.x, matrix.y, matrix.z );

	}

	if( p_wk->count >= WFLBY_GADGET_ONPU_MOVE_DRUM_SYNC ){
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V���o������J�n
 *
 *	@param	p_wk		���[�N
 *	@param	cp_person	��l��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_InitCymbals( WFLBY_GADGET_ONPU* p_wk, const WFLBY_3DPERSON* cp_person )
{
	int i;
	VecFx32 matrix;
	VecFx32 end_matrix;

	// ��l���̍��W���擾
	WFLBY_3DOBJCONT_DRAW_Get3DMatrix( cp_person, &matrix );
	matrix.x += WFLBY_GADGET_ONPU_X_OFS;
	matrix.y += WFLBY_GADGET_AIR_Y + WFLBY_GADGET_ONPU_MOVE_CYMBALS_START_Y;
	matrix.z += WFLBY_GADGET_ONPU_Z_OFS;
		
	for( i=0; i<p_wk->mvnum; i++ ){
		WFLBY_GADGET_OnpuMove_SetCymbalsStParam( p_wk, i, &matrix,
				WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_X00,
				WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_Y00,
				WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_X01,
				WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_Y01,
				WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ00_SYNC );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V���o�����상�C���֐�
 *
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_GADGET_OnpuMove_MainCymbals( WFLBY_GADGET_ONPU* p_wk )
{
	int		i;
	BOOL	result;
	VecFx32	matrix;

	for( i=0; i<p_wk->mvnum; i++ ){
		
		// �����ړ�
		result = WFLBY_GADGET_MV_Straight_Main( &p_wk->st[i], p_wk->count );

		// ���W�̐ݒ�
		WFLBY_GADGET_MV_Straight_GetNum( &p_wk->st[i], &matrix.x, &matrix.y, &matrix.z );
		D3DOBJ_SetMatrix( p_wk->p_obj[i], matrix.x, matrix.y, matrix.z );

		// �I�������玟�̏���
		if( result == TRUE ){

			if( p_wk->seq == 0 ){
				WFLBY_GADGET_OnpuMove_SetCymbalsStParam( p_wk, i, &matrix,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_X00,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_Y00,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_X01,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_Y01,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ01_SYNC );
			}else{
				WFLBY_GADGET_OnpuMove_SetCymbalsStParam( p_wk, i, &matrix,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_X00,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_Y00,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_X01,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_Y01,
						WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ02_SYNC );
			}
		}
	}

	if( result == TRUE ){
		if( (p_wk->seq + 1) < WFLBY_GADGET_ONPU_MOVE_CYMBALS_SEQ_NUM ){
			p_wk->seq ++;
			p_wk->count = 0;	// �J�E���^������
		}else{
			// �I��
			return TRUE;
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	CYMBALS����l�ݒ�
 *
 *	@param	p_wk			���[�N
 *	@param	idx				�C���f�b�N�X
 *	@param	cp_def_matrix	��{���W
 *	@param	ofs_x0			�I�t�Z�b�gX
 *	@param	ofs_y0			�I�t�Z�b�gY
 *	@param	ofs_x1			�I�t�Z�b�gX
 *	@param	ofs_y1			�I�t�Z�b�gY
 *	@param	sync			�V���N��
 */
//-----------------------------------------------------------------------------
static void WFLBY_GADGET_OnpuMove_SetCymbalsStParam( WFLBY_GADGET_ONPU* p_wk, u32 idx, const VecFx32* cp_def_matrix, fx32 ofs_x0, fx32 ofs_y0, fx32 ofs_x1, fx32 ofs_y1, u32 sync )
{
	VecFx32 end_matrix;
	
	switch( idx ){
	case 0:
		end_matrix.x = cp_def_matrix->x - ofs_x0;
		end_matrix.y = cp_def_matrix->y + ofs_y0;
		end_matrix.z = cp_def_matrix->z;
		break;
	case 1:
		end_matrix.x = cp_def_matrix->x + ofs_x0;
		end_matrix.y = cp_def_matrix->y + ofs_y0;
		end_matrix.z = cp_def_matrix->z;
		break;
	case 2:
		end_matrix.x = cp_def_matrix->x - ofs_x1;
		end_matrix.y = cp_def_matrix->y + ofs_y1;
		end_matrix.z = cp_def_matrix->z;
		break;
	case 3:
		end_matrix.x = cp_def_matrix->x + ofs_x1;
		end_matrix.y = cp_def_matrix->y + ofs_y1;
		end_matrix.z = cp_def_matrix->z;
		break;
	}
	WFLBY_GADGET_MV_Straight_Init( &p_wk->st[idx], 
			cp_def_matrix->x, end_matrix.x,
			cp_def_matrix->y, end_matrix.y,
			cp_def_matrix->z, end_matrix.z,
			sync );

	D3DOBJ_SetMatrix( p_wk->p_obj[idx], cp_def_matrix->x, cp_def_matrix->y, cp_def_matrix->z );
}
