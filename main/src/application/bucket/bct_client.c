//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *  GAME FREAK inc.
 *
 *  @file       bct_cllient.c
 *  @brief		�ʓ���	�N���C�A���g�V�X�e��
 *  @author		tomoya takahashi
 *  @data       2007.06.19
 *
 *
 *  ����̘A���Ȃ̂ł߂�����萔�����ł��B
 *  ���߂�Ȃ����B
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "bct_client.h"
#include "bct_snd.h"

#include "gflib/g3d_system.h"

#include "system/wordset.h"
#include "system/msgdata.h"
#include "system/clact_util.h"
#include "system/render_oam.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/brightness.h"
#include "system/fontoam.h"
#include "system/font_arc.h"

#include "communication/communication.h"

#include "application/wifi_lobby/minigame_tool.h"

#include "src/graphic/bucket.naix"
#include "src/graphic/wlmngm_tool.naix"
#include "system/d3dobj.h"

#include "gflib/calctool.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_debug_tomoya.h"
#include "msgdata/msg_lobby_minigame1.h"
#include "system/snd_tool.h"

//-----------------------------------------------------------------------------
/**
 *                  �R�[�f�B���O�K��
 *      ���֐���
 *              �P�����ڂ͑啶������ȍ~�͏������ɂ���
 *      ���ϐ���
 *              �E�ϐ�����
 *                      const�ɂ� c_ ��t����
 *                      static�ɂ� s_ ��t����
 *                      �|�C���^�ɂ� p_ ��t����
 *                      �S�č��킳��� csp_ �ƂȂ�
 *              �E�O���[�o���ϐ�
 *                      �P�����ڂ͑啶��
 *              �E�֐����ϐ�
 *                      �������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------

#ifdef BCT_DEBUG
static s16  BCT_DEBUG_NutsSetNum = 0;
static s16  BCT_DEBUG_NutsDrawNum = 0;
#endif


//-----------------------------------------------------------------------------
/**
 *                  �萔�錾
*/
//-----------------------------------------------------------------------------
#define BCT_NUTSBUFF_NUM    ( 96 )  // ���M�p�؂̎��f�[�^�o�b�t�@��
#define BCT_NUTSBUFFOAM_NUM ( 96 )  // ���`��o�b�t�@��
#define BCT_MAINOAM_CONTID  ( 200 )
#define BCT_MAINOAM_BGPRI  ( 0 )
#define BCT_STRBUF_NUM      ( 128 ) // STRBUF�����Ƃ��̃o�b�t�@��
#define BCT_FONTOAM_WKNUM	( 8 )	// FONTOAM���[�N��

#define BCT_COL_N_BLACK ( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )      // �t�H���g�J���[�F��
#define BCT_COL_N_WHITE ( GF_PRINTCOLOR_MAKE( 15, 14, 15 ) )        // �t�H���g�J���[�F��
#define BCT_COL_N_RED       ( GF_PRINTCOLOR_MAKE( 3, 4, 15 ) )      // �t�H���g�J���[�F��
#define BCT_COL_N_BLUE      ( GF_PRINTCOLOR_MAKE( 5, 6, 15 ) )      // �t�H���g�J���[�F��
#define BCT_COL_N_GRAY      ( GF_PRINTCOLOR_MAKE( 2, 14, 15 ) )     // �t�H���g�J���[�F�D

#define BCT_COL_OAM_BLACK     ( GF_PRINTCOLOR_MAKE( 15, 14, 0 ) )     // �t�H���g�J���[�F��


//-------------------------------------
/// �؂̎�����V�[�P���X
//=====================================
enum {
    BCT_NUTSSEQ_NONE,
    BCT_NUTSSEQ_MOVE,       // ����
    BCT_NUTSSEQ_MOVEOTHER,  // ���l�̖؂̎��Ƃ��ē���
	BCT_NUTSSEQ_INMOVE,		// �����Ă���̓���
    BCT_NUTSSEQ_END,        // ���M�҂�
    BCT_NUTSSEQ_MOVEAWAY,   // ��O��
    BCT_NUTSSEQ_NUM,
} ;

//-------------------------------------
/// �J�n��
//=====================================
enum{
    BCT_STARTSEQ_TOUCHPEN_START,   // �^�b�`�y���J�n
    BCT_STARTSEQ_TOUCHPEN_MAIN,   // �^�b�`�y�����C��
    BCT_STARTSEQ_MARUNOMU_SND,   // �}���m�[��������Ă���҂�
    BCT_STARTSEQ_MARUNOMU_OPEN, // �}���m�[���̌����J���҂�
    BCT_STARTSEQ_WAIT,          // �J�E���g�_�E���J�n�҂�
    BCT_STARTSEQ_TEXTINIT,      // �e�L�X�g�̏���
    BCT_STARTSEQ_COUNTDOWNWAIT, // �J�E���g�_�E���҂�
    BCT_STARTSEQ_GAMESTARTWAIT, // �Q�[�����o�����ԂɂȂ�܂ő҂�
    BCT_STARTSEQ_END,           // �J�n�I���
};
#define BCT_START_MARU_MOUTH_OPEN_WAIT	( 65 )
  

//-------------------------------------
///	�Q�[�������
//=====================================
typedef enum {
    BCT_MARUNOMU_MOVE_EASY,			// �ȒP����
    BCT_MARUNOMU_MOVE_FEVER_EFF,	// FEVER�O�G�t�F�N�g
    BCT_MARUNOMU_MOVE_FEVER,		// FEVER
    BCT_MARUNOMU_MOVE_NUM,			// �^�C�v��
} BCT_MARUNOMU_MOVE_TYPE;
// ����J�n����
static const u32 sc_BCT_MARUNOMU_MOVE_STARTTIME[ BCT_MARUNOMU_MOVE_NUM ] = {
	0 * 30,
	(22 * 30),
	(25 * 30),
};



//-------------------------------------
/// �}���m�[��
//=====================================
enum{   // �}���m�[�����f���I�u�W�F
	BCT_MARUNOMU_MDL_NORMAL,	// �ʏ�
	BCT_MARUNOMU_MDL_OPEN,		// �I�[�v���A�j���p
	BCT_MARUNOMU_MDL_FEVER,		// �t�B�[�o�[�A�j���p
    BCT_MARUNOMU_MDL_NUM,
};
enum{   // �}���m�[���A�j���I�u�W�F
    BCT_MARUNOMU_ANM_WALK,  // �����A�j��
    BCT_MARUNOMU_ANM_ROTA,  // ��]�A�j��
    BCT_MARUNOMU_ANM_PURU,  // ���v���v���A�j��
    BCT_MARUNOMU_ANM_FEV,	// FEVER�O�A�j��
    BCT_MARUNOMU_ANM_OPEN,  // ���J���A�j��
    BCT_MARUNOMU_ANM_CLOSE, // �����A�j��
    BCT_MARUNOMU_ANM_NUM,
};
enum{
	BCT_MARUNOMU_ANM_COLANM_NORMAL,		//�@�ʏ탂�f���p�J���[�A�j���iBCT_MARUNOMU_MDL_NORMAL�p�j
//	BCT_MARUNOMU_ANM_COLANM_FEVER,		//�@�t�B�[�o�[�G�t�F�N�g���f���p�J���[�A�j���iBCT_MARUNOMU_MDL_FEVER�p�j
	BCT_MARUNOMU_ANM_COLANM_NUM,
};
enum{	// �F���G�A�j���t���[���Ǘ�
	BCT_MARUNOMU_ANM_COL_NORMAL,
	BCT_MARUNOMU_ANM_COL_1P,
	BCT_MARUNOMU_ANM_COL_2P,
	BCT_MARUNOMU_ANM_COL_3P,
	BCT_MARUNOMU_ANM_COL_4P,
	BCT_MARUNOMU_ANM_COL_NUM,

	BCT_MARUNOMU_ANM_COL_DATA_START = 0,	// �J�n�t���[��
	BCT_MARUNOMU_ANM_COL_DATA_END,			// �I���t���[��
	BCT_MARUNOMU_ANM_COL_DATA_NUM,
};
// �J���[�t���[���@�J�n�I���t���[��
static const u8 sc_BCT_MARUNOMU_ANM_COL_DATA[BCT_MARUNOMU_ANM_COL_NUM][BCT_MARUNOMU_ANM_COL_DATA_NUM] = {
	// BCT_MARUNOMU_ANM_COL_NORMAL,
	{  0,  0},
	// BCT_MARUNOMU_ANM_COL_1P,
	{  1, 10 },
	// BCT_MARUNOMU_ANM_COL_2P,
	{ 11, 20 },
	// BCT_MARUNOMU_ANM_COL_3P,
	{ 21, 30 },
	// BCT_MARUNOMU_ANM_COL_4P,
	{ 31, 40 },
};
#define BCT_MARUNOMU_ANM_COL_1PSTART	( sc_BCT_MARUNOMU_ANM_COL_DATA[BCT_MARUNOMU_ANM_COL_1P][BCT_MARUNOMU_ANM_COL_DATA_START]*FX32_ONE )
#define BCT_MARUNOMU_ANM_COL_END		( sc_BCT_MARUNOMU_ANM_COL_DATA[BCT_MARUNOMU_ANM_COL_4P][BCT_MARUNOMU_ANM_COL_DATA_END]*FX32_ONE )

// �A�j���̃m�[�h��
#define BCT_MARUNOMU_ANM_NODE_NUM	( 15 )
static const NNSG3dResName sc_AnmNodeName[ BCT_MARUNOMU_ANM_NUM ][ BCT_MARUNOMU_ANM_NODE_NUM ] = {
	{	// BCT_MARUNOMU_ANM_WALK
		"polySurface2",
		"polySurface3",
		"pTorus2",
		"null1",
	},

	{	// BCT_MARUNOMU_ANM_ROTA
		"polySurface7",
		"polySurface13",
		"pSphere2",
		"pSphere3",
		"pSphere6",
		"pSphere7",
	},

	{	// BCT_MARUNOMU_ANM_PURU
		"polySurface13",
		"pSphere7",
	},

	{	// BCT_MARUNOMU_ANM_FEV
		"null1",
		"pCylinder4",
		"pCylinder5",
		"pSphere2",
		"pSphere3",
		"pSphere6",
		"pSphere7",
		"pTorus1",
		"pTorus2",
		"polySurface13",
		"polySurface8", 
		"polySurface2",
		"polySurface3",
		"polySurface7",
		"polySurface12",
	},

	{	// BCT_MARUNOMU_ANM_OPEN
		"polySurface7",
		"polySurface13",
		"pSphere2",
		"pSphere3",
		"pSphere6",
		"pSphere7",
		"null1",
	},

	{	// BCT_MARUNOMU_ANM_CLOSE
		"polySurface7",
		"polySurface13",
		"pSphere2",
		"pSphere3",
		"pSphere6",
		"pSphere7",
	},
};
// �A�j���ƃ��f���̑Ή��\
static const u8 sc_BCT_MARUNOMU_ANM_MDL[ BCT_MARUNOMU_ANM_NUM ] = {
	BCT_MARUNOMU_MDL_NORMAL,
	BCT_MARUNOMU_MDL_NORMAL,
	BCT_MARUNOMU_MDL_NORMAL,
	BCT_MARUNOMU_MDL_FEVER,
	BCT_MARUNOMU_MDL_OPEN,
	BCT_MARUNOMU_MDL_NORMAL,
};

// �J���[�A�j���ƃ��f���̑Ή��\
static const u8 sc_BCT_MARUNOMU_COLANM_MDL[ BCT_MARUNOMU_ANM_COLANM_NUM ] = {
	BCT_MARUNOMU_MDL_NORMAL,
//	BCT_MARUNOMU_MDL_FEVER,
};
#define BCT_MARUNOMU_ANM_FRAME_MAX		( FX32_CONST(180) )	// �}���m�[���A�j���ő�t���[��
#define BCT_MARUNOMU_ANM_SPEED		( FX32_CONST(2) )	// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_STARTSPEED	( FX32_CONST(1.0) )	// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_FEVERSPEED	( FX32_CONST(1.0) )	// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_SLOWSPEED	( FX32_CONST(1) )	// �A�j���X�s�[�h
// �}���m�[�������蔻��f�[�^
#define BCT_FIELD_MARUNOMU_X    ( FX32_CONST(0) )   // �}���m�[��������ꏊ
#define BCT_FIELD_MARUNOMU_Z    ( FX32_CONST(0) )  // �}���m�[��������ꏊ
#define BCT_FIELD_MARUNOMU_Y    ( FX32_CONST(-90) )   // �}���m�[��������ꏊ
#define BCT_FIELD_MIN			( FX32_CONST(-50) )	// ���̈ʒu
#define BCT_FIELD_Y_DEL			( FX32_CONST(-100) )	// �j�����鏰�̈ʒu

// ���̕`��ʒu
#define BCT_FIELD_YUKA_DRAW_Y	( BCT_FIELD_MARUNOMU_Y - FX32_CONST(8) )

#define BCT_SHADOW_Y_FLOOR		( BCT_FIELD_MIN - FX32_CONST( 1 ) )
#define BCT_SHADOW_Y_DEL		( FX32_CONST( -300 ) )
#define BCT_SHADOW_Y_SIZE_DIF	( FX32_CONST( 300 ) )	// �؂̎��̍����ő�
#define BCT_SHADOW_SIZE_MIN		( FX32_CONST( 1 ) )		// �A�̃T�C�Y
#define BCT_SHADOW_SIZE_DIV		( FX32_CONST( 1 ) )		// �A�̃T�C�Y
#define BCT_SHADOW_ALPHA_MIN	( 8 )	// �ŏ�
#define BCT_SHADOW_ALPHA_DIV	( 16 )	// ��

static const VecFx32 sc_MARUNOMU_MAT = { BCT_FIELD_MARUNOMU_X, BCT_FIELD_MARUNOMU_Y, BCT_FIELD_MARUNOMU_Z };

#define BCT_MARUNOMU_HIT_DIS_DIV    ( FX32_CONST(30) )  // �}���m�[���̌��̑傫���ϓ��l
#define BCT_MARUNOMU_HIT_DIS_MIN    ( FX32_CONST(50) )   // �}���m�[���̌��̑傫���ŏ�
#define BCT_MARUNOMU_HIT_DIS_MAX    ( BCT_MARUNOMU_HIT_DIS_MIN+BCT_MARUNOMU_HIT_DIS_DIV )  // �}���m�[���̌��̑傫���ő�
#define BCT_MARUNOMU_SCALE_INIT		( 2500 )	// �}���m�[���̑傫���J�n
#define BCT_MARUNOMU_SCALE_END		( 4516 )	// �}���m�[���̑傫���I��
#define BCT_MARUNOMU_INRYOKU_DIST   ( FX32_CONST(200) ) // �}���m�[���Ɍ������ē�������
#define BCT_MARUNOMU_INRYOKU_POWER_PAR  ( FX32_CONST(0.03) )    // �����̂��ꕪ���͂𓭂�����
static const VecFx32 BCT_MARUNOMU_HIT_PLANE_N = { 0, FX32_CONST(2500), 0 }; // �}���m�[���̌������蔻��̖@���x�N�g��
#define BCT_MARUNOMU_HIT_X	( 0 )	// �����蔻�肘���W
#define BCT_MARUNOMU_HIT_Y	( FX32_CONST(55) )	// �����蔻�肙���W
#define BCT_MARUNOMU_HITBODY_Y_MIN	( FX32_CONST(150) )	// �}���m�[���̂����蔻��J�n
#define BCT_MARUNOMU_HITBODY_Y_DIF	( FX32_CONST(60) )	// �p�x���O�`�X�O�ɂȂ�Ԃɕς�鍂��
#define BCT_MARUNOMU_HITBODY_R		( FX32_CONST(80) )	// �����蔻�蔼�a
#define BCT_MARUNOMU_HITBODY_R_MAX	( FX32_CONST(120) )	// �����蔻�蔼�a
#define BCT_MARUNOMU_HITBODY_R_DIS	( BCT_MARUNOMU_HITBODY_R_MAX - BCT_MARUNOMU_HITBODY_R )
enum{
    BCT_MARUNOMU_MOUTH_HIT_NONE,    // �܂����܂ł����Ă��Ȃ�
    BCT_MARUNOMU_MOUTH_HIT_OK,  // �������I
    BCT_MARUNOMU_MOUTH_HIT_NG,  // �͂��ꂽ�I
    BCT_MARUNOMU_MOUTH_HIT_NEAR,// ���͔����I
} ;
#define BCT_MARUNOMU_SCALE      (FX32_CONST(1.50f))
#define BCT_MARUNOMU_ROTA_X_MIN			( FX_GET_ROTA_NUM( 20 ) )	// �X�΍ŏ��l
#define BCT_MARUNOMU_ROTA_X_INIT        ( FX_GET_ROTA_NUM( 20 ) )    // �}���m�[���̌��̌X��
#define BCT_MARUNOMU_ROTA_X_ADD         ( FX_GET_ROTA_NUM( 30 ) )   // �}���m�[���̌��̌X��
#define BCT_MARUNOMU_ROTA_X_MAX_360		( 90 )	// �ő�X��
#define BCT_MARUNOMU_ROTA_X_MAX			( FX_GET_ROTA_NUM( BCT_MARUNOMU_ROTA_X_MAX_360 ) )	// �ő�X��
#define BCT_MARUNOMU_ROTA_X_DIF			( BCT_MARUNOMU_ROTA_X_MAX - BCT_MARUNOMU_ROTA_X_MIN )	// MIN��MAX�̍�
#define BCT_MARUNOMU_ROTA_X_ADD_SUB ( FX_GET_ROTA_NUM( 8 ) )    // �␳�n
#define BCT_MARUNOMU_ROTA_INIT			( FX_GET_ROTA_NUM( 180 ) )	// Y����]�����l
#define BCT_MARUNOMU_ROTA_SPEED_INIT    ( 128 )
#define BCT_MARUNOMU_ROTA_SPEED_ADD     ( 360 ) // INIT�ɑ����ő�l
#define BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ( 96 )  // �␳�n
#define BCT_MARUNOMU_DRAWROTA_ADD       ( FX_GET_ROTA_NUM( 0 ) )  // �\���ŕ␳�����]�p
#define BCT_MARUNOMU_ROTACOUNT_ONE		( 1000 )
#define BCT_MARUNOMU_ROTACOUNT_GETNUM(x) ((x)*BCT_MARUNOMU_ROTACOUNT_ONE)

// �}���m�[���o�N�o�N����
#define BCT_MARUNOMU_BAKUBAKU_ONE_SYNC	( 50 )	// �P��J���ĕ���Ԋu

// ���_�f�[�^
#define BCT_MARUNOMU_SCORE_TYPENUM  (3) // �X�R�A�̃^�C�v
static const u32 BCT_SCORE_NUM[ BCT_MARUNOMU_SCORE_TYPENUM ] = {
    100, 200, 300
};
#define BCT_MARUNOMU_SCORE_FEVER_MUL	(3)	// FEVER�^�C���̎��ɂ�����l

// �t�B�[���h�f�[�^
#define BCT_FIELD_ZDIS  ( FX32_CONST(600) )     // �t�B�[���h�̑傫��
#define BCT_FIELD_XDIS  ( FX32_CONST(600) )     // �t�B�[���h�̑傫��
#define BCT_FIELD_ZDISHALF  ( BCT_FIELD_ZDIS/2 )    // �t�B�[���h�̑傫��
#define BCT_FIELD_XDISHALF  ( BCT_FIELD_XDIS/2 )    // �t�B�[���h�̑傫��
#define BCT_FIELD_PLAYER_X  ( FX32_CONST(0) )   // �v���C���[�̂���ꏊ
#define BCT_FIELD_PLAYER_Z  ( FX32_CONST(-300) )    // �v���C���[�̂���ꏊ
#define BCT_FIELD_PLAYER_Y  ( FX32_CONST(0) )   // �v���C���[�̂���ꏊ
#define BCT_FIELD_DOT256TOMAT   ( FX32_CONST(150) ) // DOT���W��256���RD�́`�Ƃ��邩
#define BCT_FIELD_DOT256MATHALF (BCT_FIELD_DOT256TOMAT / 2)
#define BCT_FIELD_DOT2MAT(x)        ( ((x)*(BCT_FIELD_DOT256TOMAT/256))-BCT_FIELD_DOT256MATHALF )   // DS1dot�̂RD���W
#define BCT_FIELD_MAT2DOT(x)        ( ((x)+BCT_FIELD_DOT256MATHALF)/(BCT_FIELD_DOT256TOMAT/256) )   // �RD���W��DS1dot���W
#define BCT_NUTS_COMM_SCALE     ( FX32_CONST(0.16) )    // �ʐM�f�[�^�ɂ���Ƃ��̐��l�̈��k��
static const u16 BCT_NUTS_FIELD_NetIDRota[ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ] = {
    {   // 1�l
        0,
        0,0,0
    },
    {   // 2�l
        0,
        FX_GET_ROTA_NUM( 180 ),
        0,0
    },
    {   // 3�l
        0,
        FX_GET_ROTA_NUM( 120 ),
        FX_GET_ROTA_NUM( 240 ),
        0
    },
    {   // 4�l
        0,
        FX_GET_ROTA_NUM( 180 ),
        FX_GET_ROTA_NUM( 90 ),
        FX_GET_ROTA_NUM( 270 ),
    }
};

// �J�����f�[�^
#define BCT_CAMERA_TARGET_X ( 0 )
#define BCT_CAMERA_TARGET_Y ( 0 )
#define BCT_CAMERA_TARGET_Z ( 0 )
#define BCT_CAMERA_ANGLE_X  ( 0xf112 )
#define BCT_CAMERA_ANGLE_Z  ( FX_GET_ROTA_NUM( 0 ) )
#define BCT_CAMERA_DISTANCE ( 0x1d9000 )
#define BCT_CAMERA_PEARCE   ( FX_GET_ROTA_NUM( 22 ) )
#define BCT_CAMERA_UP       ( FX32_ONE )
#define BCT_CAMERA_NEAR		( FX32_CONST(200) )
#define BCT_CAMERA_FAR		( FX32_CONST(1000) )
static const u16 BCT_CAMERA_ANGLEY_NetID[ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ] = {
    {   // 1�l
        0,0,0,0,
	},
    {   // 2�l
        FX_GET_ROTA_NUM( 180 ),
        0,
        0,0,
    },
    {   // 3�l
        FX_GET_ROTA_NUM( 180 ),
        FX_GET_ROTA_NUM( 300 ),
        FX_GET_ROTA_NUM( 60 ),
        0,
    },
    {   // 4�l
        FX_GET_ROTA_NUM( 180 ),
        0,
        FX_GET_ROTA_NUM( 270 ),
        FX_GET_ROTA_NUM( 90 ),
    },
};

// �؂̎�����f�[�^
#define BCT_NUTS_POWER_MIN  ( 0x8000 )    // �p���[�ŏ�
#define BCT_NUTS_POWER_DIS  ( 0x16200 )    // �p���[�ő�ƍŏ��̍�
#define BCT_NUTS_POWER_MAX  ( BCT_NUTS_POWER_MIN+BCT_NUTS_POWER_DIS )// �p���[�ő�

#define BCT_NUTS_POWER_RES  ( 0x670 )   // ��C��R
#define BCT_NUTS_POWER_G    ( 0xffffeccc )  // �d��
#define BCT_NUTS_POWRT_YPOWER ( 0x26000 )   // Y�����̗͂�XZ�����̉��{��
#define BCT_NUTS_POWER_MUL  ( FX32_CONST(0.6) )     // �p���[��傫������{��
#define BCT_NUTS_AWAYPOWER_MUL	( FX32_CONST( 0.850f ) )	// ������уp���[�ɂ�����l
#define BCT_NUTS_AWAYPOWERFIELD_MUL	( FX32_CONST( 0.80f ) )	// �n�ʐ�����уp���[�ɂ�����l
#define BCT_NUTS_AWAYWAY_MUL	( FX32_CONST( 1.0f ) )	// ������ԕ�����h��ɂ��邽�߂ɂ����邠����
#define BCT_NUTS_AWAYPOWER_DITCHMUL	( FX32_CONST( 0.250f ) )	// ������ԕ�����h��ɂ��邽�߂ɂ����邠����
#define BCT_NUTS_AWAYDEL_POWER	( 0x1000 )	// �{�[����j������p���[
#define BCT_NUTS_AWAYDEL_POWER2	( 0x2000 )	// �{�[����j������p���[

#define BCT_NUTS_AWAY_DITCH_MIN_R	( FX32_CONST( 188 ) )		// �a�̔��a�G���A�f�[�^
#define BCT_NUTS_AWAY_DITCH_MAX_R	( FX32_CONST( 240 ) )
#define BCT_NUTS_AWAY_DITCH_DIV_R	( BCT_NUTS_AWAY_DITCH_MAX_R - BCT_NUTS_AWAY_DITCH_MIN_R )
#define BCT_NUTS_AWAY_BLOCK_R		( FX32_CONST( 240 ) )
#define BCT_NUTS_AWAY_MARUNOMU_R	( FX32_CONST( 120 ) )
#define BCT_NUTS_INMOVE_COUNT	( 8 )	// ��������ɓ��������E���g��

#define BCT_NUTSDRAW_ROTA_MIN	( 512 )	// �؂̎���]�X�s�[�h�l�Œ�
#define BCT_NUTSDRAW_ROTA_MAX	( 1024 )	// �؂̎���]�X�s�[�h�l�ő�
#define BCT_NUTSDRAW_ROTA_DIS	( BCT_NUTSDRAW_ROTA_MAX - BCT_NUTSDRAW_ROTA_MIN )
#define BCT_NUTSDRAW_ROTA_CHG_TIMING	( 8 )	// ��]�X�s�[�h��ς���^�C�~���O
#define BCT_NUTS_BONUS_COUNT_MAX	( 99 )	// �{�[�i�X�J�E���g�̍ő�


// �؂̎����\�[�X
typedef enum{
	BCT_NUTSRES_MDLPLNO00,
	BCT_NUTSRES_MDLPLNO01,
	BCT_NUTSRES_MDLPLNO02,
	BCT_NUTSRES_MDLPLNO03,
	BCT_NUTSRES_MDLSPECIAL,
	BCT_NUTSRES_MDLNUM,
} BCT_NUTSRES_MDLTYPE;

// �X���[
#define BCT_NUTS_SLOW_Q ( 8 )   // �����O���L���[�̃L���[�̐�
#define BCT_NUTS_SLOW_USE_VECNUM    ( 2 )   // �K�v�Ȓ��_�̐�
#define BCT_NUTS_SLOW_TOUCH_AREA_CENTER_X	( 128 )	// �؂̎����E����G���A�̒��S���W
#define BCT_NUTS_SLOW_TOUCH_AREA_CENTER_Y	( -96 )	// ���S
#define BCT_NUTS_SLOW_TOUCH_AREA_R			( 190-BCT_NUTS_SLOW_TOUCH_AREA_CENTER_Y )	// ���a
#define BCT_NUTS_SLOW_RENSYA_TIME	( 2 )	// �ŏ������ˑ҂�����

// OAM�؂̎���ʊO�G�t�F�N�g
#define BCT_OAMAWAYNUTS_BUFFNUM	( 8 )	// OAM�؂̎���ʊO�G�t�F�N�g�o�b�t�@��
#define BCT_OAMAWAYNUTS_DEL_XMIN (-FX32_CONST(32))	// �G�t�F�N�g��j��������W
#define BCT_OAMAWAYNUTS_DEL_XMAX (FX32_CONST(288))	// �G�t�F�N�g��j��������W
#define BCT_OAMAWAYNUTS_DEL_YMIN (-FX32_CONST(32))	// �G�t�F�N�g��j��������W
#define BCT_OAMAWAYNUTS_DEL_YMAX (FX32_CONST(224))	// �G�t�F�N�g��j��������W
#define BCT_OAMAWAYNUTS_MOVE0_COUNT	( 16 )	// 1�o�E���h�ڂ̃J�E���g
#define BCT_OAMAWAYNUTS_MOVE0_SPEED	( 8 )	// 1�o�E���h�ڂ̃X�s�[�h
#define BCT_OAMAWAYNUTS_MOVE0_SROTA	( 190 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_MOVE0_EROTA	( 125 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_MOVE1_COUNT	( 10 )	// 1�o�E���h�ڂ̃J�E���g
#define BCT_OAMAWAYNUTS_MOVE1_SPEED	( 4 )	// 1�o�E���h�ڂ̃X�s�[�h
#define BCT_OAMAWAYNUTS_MOVE1_SROTA	( 182 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_MOVE1_EROTA	( 140 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_MOVE2_COUNT	( 6 )	// 1�o�E���h�ڂ̃J�E���g
#define BCT_OAMAWAYNUTS_MOVE2_SPEED	( 3 )	// 1�o�E���h�ڂ̃X�s�[�h
#define BCT_OAMAWAYNUTS_MOVE2_SROTA	( 180 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_MOVE2_EROTA	( 160 )	// ����̊J�n��]�p�x
#define BCT_OAMAWAYNUTS_SCALE	( 9 )	// OAM������Ə���������
#define BCT_OAMAWAYNUTS_SCALE_DIV ( 3 )	// OAM�ߎ��ω��n
#define BCT_OAMAWATNUTS_MOVE_COUNT	( BCT_OAMAWAYNUTS_MOVE0_COUNT+BCT_OAMAWAYNUTS_MOVE1_COUNT+BCT_OAMAWAYNUTS_MOVE2_COUNT )	// �S���̃J�E���^



// �`��f�[�^
#define BCT_GRA_CHARMAN_NUM ( 16 )  // �L�����N�^�}�l�[�W���Ǘ���
#define BCT_GRA_PLTTMAN_NUM ( 16 )  // �p���b�g�}�l�[�W���Ǘ���
#define BCT_GRA_OAMVRAM_MAIN    ( 128*1024 )    // ���C��OAMVRAM
#define BCT_GRA_OAMVRAM_SUB     ( 16*1024 ) // �T�uOAMVRAM
#define BCT_GRA_CLACTNUM        ( 128 ) // �Z���A�N�^�[���[�N��
#define BCT_GRA_RESMAN_NUM      ( 4 )   // �Ǘ����郊�\�[�X��ސ�
#define BCT_GRA_RESMAN_LOADNUM  ( 16 )  // ���\�[�X�Ǘ���
#define BCT_GRA_OAMSUBSURFACE_Y ( 512<<FX32_SHIFT )

#define BCT_GRA_NUTS_OAM_RESID  ( 100 ) // �؂̎�OAM�Ǘ�ID
#define BCT_GRA_NUTS_OAM_PRI    ( 32 )
#define BCT_GRA_NUTS_OAM_BGPRI  ( 0 )

#define BCT_GRA_SUBBRIGHT_OFFNUM    (-14) // �T�u�ʂ�OFF���Ă���Ƃ��̃u���C�g�l�X�l

enum{   // MAIN�p���b�g
    BCT_GRA_BGMAIN_PAL_NAME_PL00,
    BCT_GRA_BGMAIN_PAL_NAME_PL01,
    BCT_GRA_BGMAIN_PAL_NAME_PL02,
    BCT_GRA_BGMAIN_PAL_NAME_PL03,
    BCT_GRA_BGMAIN_PAL_SYSWND,
    BCT_GRA_BGMAIN_PAL_FONT,
};
enum{
	BCT_GRA_BGSUB_PAL_BACK_NETID0,
	BCT_GRA_BGSUB_PAL_NETID0_TOP,
	BCT_GRA_BGSUB_PAL_NETID0_BACK,
	BCT_GRA_BGSUB_PAL_NETID1_TOP,
	BCT_GRA_BGSUB_PAL_NETID1_BACK,
	BCT_GRA_BGSUB_PAL_NETID2_TOP,
	BCT_GRA_BGSUB_PAL_NETID2_BACK,
	BCT_GRA_BGSUB_PAL_NETID3_TOP,
	BCT_GRA_BGSUB_PAL_NETID3_BACK,
	BCT_GRA_BGSUB_PAL_BACK_NETID1,
	BCT_GRA_BGSUB_PAL_BACK_NETID2,
	BCT_GRA_BGSUB_PAL_BACK_NETID3,
	BCT_GRA_BGSUB_PAL_NUM,
};
enum{   // MAIN��OAM�A�j���V�[�P���X
    BCT_GRA_OAMMAIN_ANM_COUNTDOWN,  // �J�E���g�A�j��
    BCT_GRA_OAMMAIN_ANM_END,        // �I�� �A�j��
    BCT_GRA_OAMMAIN_ANM_100,        // 100 �A�j��
    BCT_GRA_OAMMAIN_ANM_200,        // 200 �A�j��
    BCT_GRA_OAMMAIN_ANM_300,        // 300 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE100,        // FEVER300 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE200,        // FEVER600 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE300,        // FEVER900 �A�j��

    BCT_GRA_OAMMAIN_ANM_100_OTHER,		// ����p100 �A�j��
    BCT_GRA_OAMMAIN_ANM_200_OTHER,		// ����p200 �A�j��
    BCT_GRA_OAMMAIN_ANM_300_OTHER,		// ����p300 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE100_OTHER,	// ����pFEVER300 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE200_OTHER,	// ����pFEVER600 �A�j��
    BCT_GRA_OAMMAIN_ANM_FE300_OTHER,	// ����pFEVER900 �A�j��
};
enum{	// MAIN��OAM�p���b�g
	BCT_GRA_OAMMAIN_PAL_BACK,
	BCT_GRA_OAMMAIN_PAL_BACK00,
	BCT_GRA_OAMMAIN_PAL_BACK01,
	BCT_GRA_OAMMAIN_PAL_SCORE0,
	BCT_GRA_OAMMAIN_PAL_SCORE1,
	BCT_GRA_OAMMAIN_PAL_SCORE2,
	BCT_GRA_OAMMAIN_PAL_SCORE3,
	BCT_GRA_OAMMAIN_PAL_SCOREEX,
	BCT_GRA_OAMMAIN_PAL_NUM,
};
// ���C����ʃ��j���[�E�B���h�E
#define BCT_GRA_SYSWND_CGX      ( 1 )
#define BCT_GRA_SYSWND_CGXEND   ( BCT_GRA_SYSWND_CGX+MENU_WIN_CGX_SIZ )

// �J�n���
// �J�n��ʃE�B���h�E
#define BCT_GRA_STARTWIN_X      ( 2 )
#define BCT_GRA_STARTWIN_Y      ( 19 )
#define BCT_GRA_STARTWIN_SIZX   ( 28 )
#define BCT_GRA_STARTWIN_SIZY   ( 4 )
#define BCT_GRA_STARTWIN_CGX    ( BCT_GRA_SYSWND_CGXEND )
#define BCT_GRA_STARTWIN_MSGX   ( 0 )
#define BCT_GRA_STARTWIN_MSGY   ( 0 )
// �}���m�[��
#define BCT_START_MARUNOMUROTA  ( 120 )
#define BCT_START_MARUNOMU_OPEN_COUNT   ( 24 )  // �J�n������������̂Ɋ|���鎞��
// OAM
#define BCT_START_COUNTDOWN_X   ( 128 ) // �z�uX���W
#define BCT_START_COUNTDOWN_Y   ( 96 )  // �z�uY���W
#define BCT_START_COUNTDOWN_STARTFRAME	( 4 )
// �X�N���[��
#define BCT_START_SCRLL3D_COUNT	( 64 )	// ����J�E���^
#define BCT_START_SCRLL3D_Y_S		( -FX32_CONST( 200 ) )	// �}���m�[���𓮂�������
#define BCT_START_SCRLL3D_Y_E		( FX32_CONST( 0 ) )		// �}���m�[���𓮂�������
#define BCT_START_SCRLL3D_Y_A		( FX32_CONST( 2 ) )	// �}���m�[���𓮂�������
//�u���C�g�l�X
#define BCT_START_BRIGHTNESS_COUNT	( 8 )	// ����J�E���^
// ���O�\��
// ���O�t���[���T�C�Y
#define BCT_START_NAME_FRAMESIZ_X	( 10 )
#define BCT_START_NAME_FRAMESIZ_Y	( 4 )
// ���O�r�b�g�}�b�v�T�C�Y
#define BCT_START_NAME_BMP_WINSIZ_X			( 8 )
#define BCT_START_NAME_BMP_WINSIZ_Y			( 2 )
#define BCT_START_NAME_BMP_WINCGX_START		( 256 )
#define BCT_START_NAME_BMP_WINCGX_ONENUM	( BCT_START_NAME_BMP_WINSIZ_X*BCT_START_NAME_BMP_WINSIZ_Y )
#define BCT_START_NAME_STRBUF_NUM			( 128 )
// �L�����N�^�P�ʁ@�ʒu
//											// plno				commnum			 draw plno
static const Vec2DS32 sc_BCT_START_NAME_TBL[ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ] = {
	// ������PLNO��0
	{	
		// ���������̂Ƃ��͂Ȃ�
		{0},
		// 2�l�ΐ�
		{
			{0,0},
			{12,2},
		},
		// 3�l�ΐ�
		{
			{0,0},
			{23,4},
			{1,4},
		},
		// 4�l�ΐ�
		{
			{0,0},
			{12,2},
			{23,6},
			{1,6},
		},
	},

	// ������PLNO��1
	{	
		// ���������̂Ƃ��͂Ȃ�
		{0},
		// 2�l�ΐ�
		{
			{12,2},
			{0,0},
		},
		// 3�l�ΐ�
		{
			{1,4},
			{0,0},
			{23,4},
		},
		// 4�l�ΐ�
		{
			{12,2},
			{0,0},
			{1,6},
			{23,6},
		},
	},

	// ������PLNO��2
	{	
		// ���������̂Ƃ��͂Ȃ�
		{0},
		// 2�l�Ȃ�
		{
			{0},
		},
		// 3�l�ΐ�
		{
			{23,4},
			{1,4},
			{0,0},
		},
		// 4�l�ΐ�
		{
			{1,6},
			{23,6},
			{0,0},
			{12,2},
		},
	},

	// ������PLNO��3
	{	
		// ���������̂Ƃ��͂Ȃ�
		{0},
		// 2�l�Ȃ�
		{
			{0},
		},
		// 3�l�Ȃ�
		{
			{0,0},
		},
		// 4�l�ΐ�
		{
			{23,6},
			{1,6},
			{12,2},
			{0,0},
		},
	},
};

// ���ʉ��
enum{
    BCT_RESULT_SEQ_ENDINIT,
    BCT_RESULT_SEQ_ENDDRAW,
    BCT_RESULT_SEQ_ENDWAIT,
    BCT_RESULT_SEQ_END,
};
enum{
    BCT_RESULT_OAM_TIMEUP,
    BCT_RESULT_OAM_NUM,
};
#define BCT_RESULT_ENDOAM_X ( FX32_CONST(128) ) // �I����OAM�\�����W
#define BCT_RESULT_ENDOAM_Y ( FX32_CONST(128) )
#define BCT_RESULT_MARUNOMU_MOVE_TIMING ( 60 )
#define BCT_RESULT_MARUNOMU_CLOSE_TIME  ( 24 )
#define BCT_RESULT_ENDBRIGHTNESS_SYNC	( 24 )
#define BCT_RESULT_ENDWAIT	( 64 )

// �w�i
enum{
	BCT_MAINBACK_MDL_WALL_N,
	BCT_MAINBACK_MDL_WALL_F,
	BCT_MAINBACK_MDL_STAGE,
	BCT_MAINBACK_MDL_NUM,
};
#define BCT_MAINBACK_SCALE	( FX32_CONST(1.50f) )
#define BCT_MAINBACK_ROT	( RotKey( 180 ) )
enum{
	BCT_MAINBACK_ANM_WALL_N,
	BCT_MAINBACK_ANM_WALL_F,
	BCT_MAINBACK_ANM_WALL_F_TP,
	BCT_MAINBACK_ANM_NUM,
};
#define BCT_MAINBACK_FEVER_ANM_FRAME_NUM	(FX32_CONST(100))
#define BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS	(FX32_CONST(51))
#define BCT_MARUNOMU_ANM_FEVERWALLSPEED_S	( FX32_CONST(1.0) )		// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_FEVERWALLSPEED_E	( FX32_CONST(2.30) )		// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_FEVERWALLSPEED_ADD	( FX32_CONST(0.10) )	// �A�j���X�s�[�h
#define BCT_MARUNOMU_ANM_FEVERWALLSPEED_SPEED	( FX32_CONST(1.0) )		// �A�j���X�s�[�h


// �X�R�A�G�t�F�N�g
#define BCT_SCORE_EFFECT_BUF		( 3 )	// �X�R�A�G�t�F�N�g�o�b�t�@
#define BCT_SCORE_EFFECT_MOVE_COUTN	( 16 )	// ����J�E���^
#define BCT_SCORE_EFFECT_MOVE_Y		( -FX32_CONST(16) )	// ����l
#define BCT_SCORE_EFFECT_PRI_START	(0)


//-------------------------------------
///	�^�b�`�p�l���f�[�^
//=====================================
#define BCT_TOUCH_SYNC		( 4 )
#define BCT_TOUCH_BUFFNUM	( 8 )
static TPData BCT_TOUCH_BUFF[ BCT_TOUCH_BUFFNUM ];



//-------------------------------------
/// BG�D�揇�ʃX�N���[��
//=====================================
enum {
	BCT_BGPRI_SCRL_SEQ_NORMAL,
	BCT_BGPRI_SCRL_SEQ_START,
	BCT_BGPRI_SCRL_SEQ_TOPFADE,
	BCT_BGPRI_SCRL_SEQ_BOTTOMFADE,
} ;
#define BCT_BGPRI_SCRL_START_MOSTBACK	( 2 )
#define BCT_BGPRI_SCRL_COUNT_MAX		( 16 )
#define BCT_BGPRI_SCRL_BOTTOMCOUNT_MAX	( 8 )
#define BCT_BGPRI_SCRL_MOSTBACK_MAX		( 3 )
#define BCT_BGPRI_SCRL_PRI_MAX			( 2 )
#define BCT_BGPRI_SCRL_REQ_START_NUM	( 10 )




//-------------------------------------
///	�Q�[���f�[�^���f�V�X�e��
//=====================================
#define BCT_CONT_GAMEDATA_COUNT_MAX			( 16 )
#define BCT_CONT_GAMEDATA_RAREGAME_BIGSTART	( 6 )	// �召�t�]�̃e�[�u�����Q�Ƃ���J�n�ʒu



//-------------------------------------
///	�^�b�`�y���V�X�e��
//=====================================
#define BCT_TOUCHPEN_OAM_RESID		( 120 )		// ���\�[�XID[
#define BCT_TOUCHPEN_OAM_BGPRI		( 0 )		// BG�D�揇��
#define BCT_TOUCHPEN_OAM_SOFPRI		( 0 )		// �\�t�g�D�揇��
#define BCT_TOUCHPEN_OAM_X			( 128 )		// �\�����W
#define BCT_TOUCHPEN_OAM_Y			( 120 )		
#define BCT_TOUCHPEN_WAIT			( 16 )
// �^�b�`OAM�A�j���萔
enum{
	BCT_TOUCHPEN_ANM_FRAME_START,
	BCT_TOUCHPEN_ANM_FRAME_MOVE00,	// �ړ���
	BCT_TOUCHPEN_ANM_FRAME_MOVE01,
	BCT_TOUCHPEN_ANM_FRAME_MOVE02,
	BCT_TOUCHPEN_ANM_FRAME_MOVE03,
	BCT_TOUCHPEN_ANM_FRAME_MOVE04,
	BCT_TOUCHPEN_ANM_FRAME_MOVE05,
	BCT_TOUCHPEN_ANM_FRAME_MOVE06,
	BCT_TOUCHPEN_ANM_FRAME_MOVE07,
	BCT_TOUCHPEN_ANM_FRAME_MOVEOK,	// ��ɂ�������
	BCT_TOUCHPEN_ANM_FRAME_FADEOUT,	// �����Ŕ��˂�����
	BCT_TOUCHPEN_ANM_FRAME_END,		// ������
	BCT_TOUCHPEN_ANM_FRAME_NUM,		// ������
};
// ����̃V�[�P���X
enum{
	BCT_TOUCHPEN_ANM_SEQ_MOVE_START,	// �J�n
	BCT_TOUCHPEN_ANM_SEQ_MOVE_MAIN,		// ���상�C��
	BCT_TOUCHPEN_ANM_SEQ_MOVE_WAIT,		// ���̊J�n�܂ł̃E�G�C�g
	BCT_TOUCHPEN_ANM_SEQ_END_WAIT,		// �I���҂��i�؂̎����S������I���̂�҂j
};
#define BCT_TOUCHPEN_ANM_ROOP_NUM	( 2 )
// �^�b�`����p�����[�^
static const s16 sc_BCT_TOUCHPEN_ANM_FRAME_Y[ BCT_TOUCHPEN_ANM_FRAME_NUM ] = {
	BCT_TOUCHPEN_OAM_Y+57, 
	BCT_TOUCHPEN_OAM_Y+47, 
	BCT_TOUCHPEN_OAM_Y+27, 
	BCT_TOUCHPEN_OAM_Y+17, 
	BCT_TOUCHPEN_OAM_Y+7, 
	BCT_TOUCHPEN_OAM_Y-3, 
	BCT_TOUCHPEN_OAM_Y-13, 
	BCT_TOUCHPEN_OAM_Y-23, 
	BCT_TOUCHPEN_OAM_Y-33, 
	BCT_TOUCHPEN_OAM_Y-43, 
	BCT_TOUCHPEN_OAM_Y-43, 
	BCT_TOUCHPEN_OAM_Y-43, 
};



//-------------------------------------
///	�r�����_�Ǘ��V�X�e��
//=====================================
#define BCT_MDLSCR_TOP_DEF	( 0xffff )	// �����Ȉ�ʂ̃C���f�b�N�X



//-------------------------------------
///	�؂̎��J�E���^�[�V�X�e��
//=====================================
// �r�b�g�}�b�v�T�C�Y
#define BCT_NUTS_COUNT_BMP_SIZX		( 3 )
#define BCT_NUTS_COUNT_BMP_SIZY		( 2 )
// �r�b�g�}�b�v�p���b�g�Ǘ�ID
#define BCT_NUTS_COUNT_FONTOAM_PLTT_ID	( 150 )
// FONTOAM�̕\���p�����[�^
#define BCT_NUTS_COUNT_FONTOAM_X		( -7 )
#define BCT_NUTS_COUNT_FONTOAM_Y		( -5 )
#define BCT_NUTS_COUNT_FONTOAM_BG_PRI	( 0 )
#define BCT_NUTS_COUNT_FONTOAM_SF_PRI	( 0 )
// ����萔
enum{
	BCT_NUTS_COUNT_SEQ_WAIT,
	BCT_NUTS_COUNT_SEQ_IN,
	BCT_NUTS_COUNT_SEQ_MAIN,
	BCT_NUTS_COUNT_SEQ_OUT,
};
// �Z���\���p�����[�^
#define BCT_NUTS_COUNT_DRAW_X		( 32 )
#define BCT_NUTS_COUNT_DRAW_Y		( 48 )
#define BCT_NUTS_COUNT_DRAW_RESID	( 140 )
#define BCT_NUTS_COUNT_DRAW_BGPRI	( BCT_NUTS_COUNT_FONTOAM_BG_PRI )
#define BCT_NUTS_COUNT_DRAW_SFPRI	( BCT_NUTS_COUNT_FONTOAM_SF_PRI+1 )
// ���o����p�����[�^
#define BCT_NUTS_COUNT_INOUT_SX			( -FX32_CONST(64) )
#define BCT_NUTS_COUNT_INOUT_EX			( FX32_CONST(BCT_NUTS_COUNT_DRAW_X) )
#define BCT_NUTS_COUNT_INOUT_SS			( FX32_CONST(36) )
#define	BCT_NUTS_COUNT_INOUT_COUNTMAX	( 8 )
#define BCT_NUTS_COUNT_OUT_WAIT			( 12 )
// ��ꓮ��p�����[�^
#define BCT_NUTS_COUNT_YURE_SY			( FX32_CONST(BCT_NUTS_COUNT_DRAW_Y) )
#define BCT_NUTS_COUNT_YURE_EY			( FX32_CONST(BCT_NUTS_COUNT_DRAW_Y) )
#define BCT_NUTS_COUNT_YURE_SS			( FX32_CONST(12) )
#define	BCT_NUTS_COUNT_YURE_COUNTMAX	( 4 )

 
//-------------------------------------
///	FEVER�G�t�F�N�g
//=====================================
enum{
	BCT_FEVER_EFF_SEQ_BGM_WAIT,
	BCT_FEVER_EFF_SEQ_BGM_FLASH_OUT,
	BCT_FEVER_EFF_SEQ_BGM_FLASH_OUTWAIT,
	BCT_FEVER_EFF_SEQ_BGM_FLASH_IN,
	BCT_FEVER_EFF_SEQ_BGM_FLASH_INWAIT,
	BCT_FEVER_EFF_SEQ_BGM_FLASH_END,
	BCT_FEVER_EFF_SEQ_NUM,
};
#define BCT_FEVER_EFF_FLASHOUT_FLASHOUT_WAIT	( 58 )
#define BCT_FEVER_EFF_SE_MARUNOMUMOVE			( 12 )
#define BCT_FEVER_EFF_FLASHOUT_FLASHOUT_SYNC	( 4 )
#define BCT_FEVER_EFF_FLASHOUT_FLASHIN_SYNC		( 14 )
#define BCT_FEVER_EFF_BGM_TEMPO_START			( 256 )
#define BCT_FEVER_EFF_BGM_TEMPO_DIF				( 150 )
#define BCT_FEVER_BACK_ANM_SPEED_START			( FX32_CONST(1) )
#define BCT_FEVER_BACK_ANM_SPEED_DIF			( FX32_CONST(25) )



//-----------------------------------------------------------------------------
/**
 *                  �\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
//	�ėp��������
//=====================================
typedef struct {
	fx32 x;
	fx32 s_x;
	fx32 s_s;	// �����x
	fx32 s_a;	// �����x
	int count;
	int count_max;
} BCT_ADDMOVE_WORK;

//-------------------------------------
/// �J�n�J�E���g�_�E�����[�N
//=====================================
typedef struct {
    u32 seq;
    s32 count;
	s32 wait;
    GF_BGL_BMPWIN   helpwin;    // �Q�[�������E�B���h�E
} BCT_COUNTDOWN_DRAW;


//-------------------------------------
/// �}���m�[�����[�N
//=====================================
typedef struct {
	u8 hit;				// �����蔻�菈���̗L��
	u8 eat_flag;		// �H�ׂ鋖�t���O
    u16 rota;           // y���̉�]�p�x
    u16 rotax;          // x���̉�]�p�x
    u16 speed;          // ��]�X�s�[�h
    fx32 mouth_size;    // ���̃T�C�Y
    MtxFx33 rotax_mtx;  // X��]�̃}�g���N�X
    MtxFx33 rotay_mtx; // Y��]�̃}�g���N�X
    MtxFx33 rotaxy_mtx; // XY��]�̃}�g���N�X
    VecFx32 hit_plane_n;    // ���ʂ̖@��
    VecFx32 center_mat; // ���ʂ̒��S
    fx32    d;          // ���ʂ̕����� D
    fx32    inryoku_area;   // ���͂̓����G���A�̃T�C�Y
    fx32    inryoku_power;  // ���̗͂�

	// ��{�ƂȂ�}���m�[�����W
	VecFx32 marunomu_mat;	

	// �g��k��
	fx32	scale;				// �S�̊g�嗦
	fx32	mouth_div;			// ���̑傫���ϓ��l
	fx32	mouth_min;			// ���̑傫���ŏ�
	fx32	mouth_max;			// ���̑傫���ő�
	fx32	hit_x;				// �����蔻�肘���W
	fx32	hit_y;				// �����蔻�肙���W
	fx32	hitbody_y_min;		// �̂̓����蔻�荂���ŏ�
	fx32	hitbody_y_dif;		// �̂̓����蔻�荂������
	fx32	hitbody_r;			// �̂̓����蔻�蔼�a
	fx32	hitbody_r_max;		// �̂̓����蔻�蔼�a�ő�
	fx32	hitbody_r_dis;		// �̂̓����蔻�蔼�a�ϓ��l
	fx32	draw_scale;			// �`��g�嗦


	// ���̂����蔻��
	VecFx32 matrix;
	fx32 height;
} BCT_MARUNOMU;

//-------------------------------------
/// �}���m�[���`��f�[�^
//=====================================
typedef struct {
    D3DOBJ      obj[BCT_MARUNOMU_MDL_NUM];
    D3DOBJ_MDL  mdl[BCT_MARUNOMU_MDL_NUM];
    D3DOBJ_ANM  anm[BCT_MARUNOMU_ANM_NUM];

	u16 set_mouthanm;	// ���ݐݒ肵�Ă�����̓����̃A�j���f�[�^
	u16	walk_anm_flag;

	// �J���[�A�j��
	D3DOBJ_ANM	colanm[BCT_MARUNOMU_ANM_COLANM_NUM];// �J���[�A�j��
	fx32 colanm_frame;	// �J���[�A�j���t���[��
	u16 col_top;		// �J���[�\������g�b�v��plno	BCT_PLAYER_NUM�Ȃ獕
	u16 col_rand;		// �����_���J���[�\������̂�
} BCT_MARUNOMU_DRAW;


//-------------------------------------
/// �؂̎��f�[�^
//=====================================
typedef struct {
    u16 plno;      // �l�b�gID
    u16 in_flag;    // �؂̎����}���m�[���ɓ������� 
    VecFx32 way;    // ����                         
    fx32    power;      // �p���[                   
    VecFx32 mat;    // ���W
    s32 bonus;		// �{�[�i�X�J�E���^	�i�A���œ������؂̎��̐��j
	BOOL special;	// ����{�[���f�[�^
} BCT_NUT_DATA;

//-------------------------------------
/// �؂̎��f�[�^
//=====================================
typedef struct {
    u8 seq;
    u8 end_flag;
    u16 count;
    BCT_NUT_DATA data;
    VecFx32 mat;
	s16 inmove_count;
	u16 inmove_bonus;
	BOOL blockhit;	//�@�Ǔ����蔻�肪�K�v�Ȃ̂��̃t���O
	fx32 xz_dist;
} BCT_CLIENT_NUTS;

//-------------------------------------
/// �����f�[�^
//=====================================
typedef struct {
    u32 hand;           // �����Ă��邩
    NNSG2dSVec2 tp_q[ BCT_NUTS_SLOW_Q ];// �����f�[�^�L���[
    u16 top;        // �擪
    u16 tail;       // ����
	u32 time;
} BCT_CLIENT_SLOW;

//-------------------------------------
/// �؂̎��`��f�[�^
//=====================================
typedef struct {
    D3DOBJ_MDL  mdl[ BCT_NUTSRES_MDLNUM ];
    D3DOBJ_MDL  shadowmdl;	// �e

    CLACT_U_RES_OBJ_PTR     resobj[4];      // �ǂݍ��񂾂�\�[�X�̃I�u�W�F�N�g
    CLACT_HEADER            header;         // �A�N�^�[�쐬�p�w�b�_�[
} BCT_CLIENT_NUTS_RES;


//-------------------------------------
/// OAM�؂̎��`��
//=====================================
typedef struct {
    CLACT_WORK_PTR p_clwk;
    D3DOBJ      obj;
    D3DOBJ      shadow;	// �����悤
    BOOL draw2d;                    // 2d�`�悳���邩
    const BCT_CLIENT_NUTS* cp_data;
	u16 rota_x;
	u16 rota_z;
	u16 rota_speed_x;
	u16 rota_speed_z;
	s16 rota_chg_count;
} BCT_CLIENT_NUTS_DRAW;

//-------------------------------------
/// �����Ă���؂̎��`��p
//=====================================
typedef struct {
    BOOL draw;
    CLACT_WORK_PTR p_clwk;
} BCT_CLIENT_HANDNUTS_DRAW;

//-------------------------------------
///	�����Ă���؂̎������ڂꗎ���܂����G�t�F�N�g
//=====================================
typedef struct {
    BOOL draw;
    CLACT_WORK_PTR p_clwk;
	VecFx32 mat;
	s32 count;
	BOOL left;
} BCT_CLIENT_OAMAWAYNUTS;
typedef struct {
	BCT_CLIENT_OAMAWAYNUTS nutsbuff[BCT_OAMAWAYNUTS_BUFFNUM];
} BCT_CLIENT_OAMAWAYNUTS_DRAW;

//-------------------------------------
///	SCORE�G�t�F�N�g���[�N
//=====================================
typedef struct {
	CLACT_WORK_PTR  p_clwk[ BCT_SCORE_EFFECT_BUF ];
	u32 count[ BCT_SCORE_EFFECT_BUF ];
	VecFx32 mat[ BCT_SCORE_EFFECT_BUF ];
	BOOL  mydata;
} BCT_CLIENT_SCORE_EFFECT_WK;

//-------------------------------------
/// SCORE�G�t�F�N�g
//=====================================
typedef struct {
    BCT_CLIENT_SCORE_EFFECT_WK wk[ BCT_PLAYER_NUM ];
} BCT_CLIENT_SCORE_EFFECT;

//-------------------------------------
/// ���ʔ��\�\�����[�N
//=====================================
typedef struct {
    u32 seq;
	u32 count;
	BCT_ADDMOVE_WORK mouthmove;
} BCT_CLIENT_RESULT;

//-------------------------------------
///	�w�i�\���O���t�B�b�N
//=====================================
typedef struct {
    D3DOBJ      obj[BCT_MAINBACK_MDL_NUM];
    D3DOBJ_MDL  mdl[BCT_MAINBACK_MDL_NUM];
	D3DOBJ_ANM	anm[BCT_MAINBACK_ANM_NUM];
	fx32 anm_speed;
	u8 fever;
	u8	fever_anm_seq;
	s16	fever_anm_wait;
	fx32 fever_anm_frame;
	fx32 fever_anm_speed;
} BCT_CLIENT_MAINBACK;


//-------------------------------------
/// BG�D�揇�ʃX�N���[��
//=====================================
typedef struct {
    u16 seq;		// �V�[�P���X
	s16 count;		// �G�t�F�N�g�J�E���^
	s16 most_back;	// ��ԉ���BG��
	u8 plno;		// �v���C���[�i���o�[
	u8 req_num;		// ���N�G�X�g��
} BCT_CLIENT_BGPRI_SCRL;

//-------------------------------------
///	�Q�[���f�[�^�ݒ�V�X�e��
//=====================================
typedef struct {
	BCT_GAMEDATA_ONE		set;		// �ݒ�l
	BCT_GAMEDATA_ONE		last;	// �O�̃f�[�^
	BCT_GAMEDATA_ONE		dif;		// �ϓ��l
	s32						count;		// �J�E���^
} BCT_CLIENT_CONT_GAMEDATA;


//-------------------------------------
///	�^�b�`�y�����[�N
//=====================================
typedef struct {
    CLACT_U_RES_OBJ_PTR     resobj[4];      // �ǂݍ��񂾂�\�[�X�̃I�u�W�F�N�g
    CLACT_HEADER            header;         // �A�N�^�[�쐬�p�w�b�_�[
    CLACT_WORK_PTR p_clwk;

	u8					move;			// ����t���O
	u8					roop;			// ���[�v��
	u8					seq;			// �V�[�P���X
	u8					nut_set;		// �؂̎��ݒ肵����
	s32					wait;			// �E�G�C�g
} BCT_CLIENT_TOUCHPEN_MOVE;

//-------------------------------------
///	Middle�X�R�A
//=====================================
typedef struct {
	u32				score[ BCT_PLAYER_NUM ];	// ���_�o�b�t�@
	u32				score_msk;					// �󂯎�����}�X�N
	u32				top_idx;					// ���̂Ƃ���̂P��
} BCT_CLIENT_MIDDLE_SCORE;


//-------------------------------------
///	�؂̎��J�E���^�[
//=====================================
typedef struct {

    CLACT_U_RES_OBJ_PTR     resobj[4];			// �ǂݍ��񂾂�\�[�X�̃I�u�W�F�N�g
    CLACT_HEADER            header;				// �A�N�^�[�쐬�p�w�b�_�[
	CLACT_WORK_PTR			p_tblwk;			// �e�[�u�����[�N
	
	GF_BGL_BMPWIN			objbmp;				// ������r�b�g�}�b�v�f�[�^
	FONTOAM_OBJ_PTR			p_fontoam;			// �t�H���gOAM���[�N
	FONTOAM_OAM_DATA_PTR	p_fontoam_data;		// �t�H���gOAM�\���f�[�^
	CHAR_MANAGER_ALLOCDATA	fontoam_chardata;	// �L�����N�^�m�ۃf�[�^
	STRBUF*					p_str;				// ������f�[�^
	CLACT_U_RES_OBJ_PTR		p_fontoam_pltt;		// �p���b�g���\�[�X

	BCT_ADDMOVE_WORK		inout_data;		// ���o�ړ��f�[�^
	BCT_ADDMOVE_WORK		yure_data;		// ���ړ��f�[�^
	s16						inout_count;	// ����J�E���^
	s16						yure_count;		// ���J�E���^
	u16						seq;			// ����V�[�P���X
	s16						out_wait;		// �o���E�G�C�g


	u32 heapID;
	
} BCT_CLIENT_NUTS_COUNT;


//-------------------------------------
///	FEVER�G�t�F�N�g
//=====================================
typedef struct {
	u16 seq;
	u8 move;
	s8 wait;
} BCT_CLIENT_FEVER_EFF_WK;



//-------------------------------------
/// �`��V�X�e��
//=====================================
typedef struct {
    GF_BGL_INI*         p_bgl;              // GF_BGL_INI
    WORDSET*            p_wordset;          // ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
    MSGDATA_MANAGER*    p_msgman;           // ���b�Z�[�W�f�[�^�}�l�[�W���[
    STRBUF*             p_msgstr;           // ���b�Z�[�W�p������o�b�t�@
    STRBUF*             p_msgtmp;           // ���b�Z�[�W�p������o�b�t�@
    u16                 msg_no;             // ���b�Z�[�W���������i���o�[
    u16                 msg_speed;          // ���b�Z�[�W�X�s�[�h

	GF_G3DMAN* p_3dman;

    CLACT_SET_PTR           clactSet;                       // �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;                       // �ȈՃ����_�[�f�[�^
    CLACT_U_RES_MANAGER_PTR resMan[BCT_GRA_RESMAN_NUM]; // �L�����E�p���b�g���\�[�X�}�l�[�W��
    CLACT_HEADER            mainoamheader;      // �A�N�^�[�쐬�p�w�b�_�[
    CLACT_U_RES_OBJ_PTR     mainoamresobj[BCT_GRA_RESMAN_NUM];

	FONTOAM_SYS_PTR	p_fontoam_sys;	// FONTOAM�̃V�X�e��

    GF_CAMERA_PTR p_camera;
    VecFx32 target;

	// ���C����ʔw�i
	BCT_CLIENT_MAINBACK mainback;

    // �X�R�A�G�t�F�N�g
    BCT_CLIENT_SCORE_EFFECT score_effect;

    // �J�n�A�j��
    BCT_COUNTDOWN_DRAW start;

    // ���ʔ��\
    BCT_CLIENT_RESULT result;

    // �}���m�[��
    BCT_MARUNOMU_DRAW marunomu; // �}���m�[���`��f�[�^

	// START	TIMEUP	�G�t�F�N�g
	MNGM_COUNTWK*  p_countwk;

	// BG�D�揇�ʁ@�G�t�F�N�g
	BCT_CLIENT_BGPRI_SCRL bgpri_scrl;	

    // �؂̎����\�[�X
    BCT_CLIENT_NUTS_RES nutsres;
    BCT_CLIENT_NUTS_DRAW nuts[BCT_NUTSBUFFOAM_NUM];
    BCT_CLIENT_HANDNUTS_DRAW handnuts;  // �����Ă���؂̎��p
	BCT_CLIENT_OAMAWAYNUTS_DRAW oamnutsaway;	//  �����Ă����؂̎��𗎂Ƃ����Ƃ��̃G�t�F�N�g


	// �؂̎��J�E���^�[
	BCT_CLIENT_NUTS_COUNT nutscount;


    // �A���P�[�^
    NNSFndAllocator allocator;


} BCT_CLIENT_GRAPHIC;




//-------------------------------------
/// �N���C�A���g���[�N
//=====================================
typedef struct _BCT_CLIENT{
	u32	move_type;	// ����^�C�v		���ԂőJ�ڂ��铮��̃^�C�v	BCT_MARUNOMU_MOVE_TYPE
	BOOL time_count_flag;
    s32 time;
    s32 time_max;
    u32 comm_num;   // �ʐM�l��
	u32 plno;	// NETID
    s32 score;  // ���_
    s32 bonus;  // �{�[�i�X�J�E���^
    BCT_CLIENT_SLOW slow;                       // �����f�[�^
    BCT_CLIENT_NUTS nuts[BCT_NUTSBUFF_NUM];     // �؂̎��f�[�^
    BCT_MARUNOMU marunomu;                      // �}���m�[��
    BCT_CLIENT_GRAPHIC graphic;                 // �O���t�B�b�N�f�[�^
    BCT_SCORE_COMM  all_score;                      // �݂�Ȃ̃X�R�A
	TP_ONE_DATA tp_one;							// �^�b�`�p�l���P�f�[�^
	BCT_CLIENT_CONT_GAMEDATA gamedata_cont;		// �Q�[���f�[�^�R���g���[��
	const BCT_GAMEDATA* cp_gamedata;			// �Q�[���f�[�^
	u16	gamedata_now;							// ���̃Q�[���f�[�^�C���f�b�N�X
	u16	gamedata_last;							// �O�̃Q�[���f�[�^�C���f�b�N�X
	BCT_GAME_TYPE_WK gametype;					// �Q�[���^�C�v
	BOOL gametype_flag;							// �Q�[���^�C�v��M�`�F�b�N�t���O

	BCT_CLIENT_TOUCHPEN_MOVE touchpen_wk;		// �^�b�`�y���V�X�e��

	BCT_CLIENT_MIDDLE_SCORE	middle_score;		// �r���̓��_�Ǘ��V�X�e��

	
	// FEVER�G�t�F�N�g
	BCT_CLIENT_FEVER_EFF_WK fever_eff;

} ;



//-----------------------------------------------------------------------------
/**
 *                  �p�[�Z���e�[�W�ɑΉ�������]�X�s�[�h
 */
//-----------------------------------------------------------------------------

#ifdef BCT_DEBUG_HITZONE
//-------------------------------------
/// �����蔻��ʒu�\���I�u�W�F�N�g
//=====================================
typedef struct {
    D3DOBJ_MDL  mdl;
    D3DOBJ      obj[4];
} BCT_DEBUG_POSITION;

static BCT_DEBUG_POSITION BCT_DPD;
static void BCT_DEBUG_PositionInit( ARCHANDLE* p_handle, u32 heapID );
static void BCT_DEBUG_PositionDraw( const BCT_CLIENT* cp_client );
static void BCT_DEBUG_PositionExit( void );

#endif



#ifdef BCT_DEBUG_AUTOSLOW
static void BCT_DEBUG_AutoSlow( BCT_CLIENT* p_wk );
#endif

#ifdef PM_DEBUG
static int BCT_DEBUG_in_num = 0;
#endif


//-----------------------------------------------------------------------------
/**
 *                  �v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void BCT_AddMoveReqFx( BCT_ADDMOVE_WORK* p_work, fx32 s_x, fx32 e_x, fx32 s_s, int count_max );
static BOOL	BCT_AddMoveMainFx( BCT_ADDMOVE_WORK* p_work );

// 2D���W�n�̕\���ʒu���擾����
static void BCT_netID2DMatrixGet( u32 comm_num, u32 myplno, u32 targetplno, s32* p_x,  s32* p_y );


// �ȒP�؂̎����ˊ֐�
static void BCT_EasyNutsSet( BCT_CLIENT* p_wk, u16 t0_x, u16 t0_y, u16 t1_x, u16 t1_y, u32 moveseq );

// �{�[�i�X�i���o�[�̃X�R�A�擾
static u32 BCT_AddScoreGet( BCT_CLIENT* cp_wk );

// �X�R�A�G�t�F�N�g
static void BCT_CLIENT_ScoreEffectInit( BCT_CLIENT_SCORE_EFFECT* p_wk, BCT_CLIENT_GRAPHIC* p_drawsys, u32 comm_num, u32 plno, u32 heapID );
static void BCT_CLIENT_ScoreEffectExit( BCT_CLIENT_SCORE_EFFECT* p_wk, BCT_CLIENT_GRAPHIC* p_drawsys);
static void BCT_CLIENT_ScoreEffectStart( BCT_CLIENT_SCORE_EFFECT* p_wk, u32 plno, u32 bonus, BCT_MARUNOMU_MOVE_TYPE movetype );
static void BCT_CLIENT_ScoreEffectMain( BCT_CLIENT_SCORE_EFFECT* p_wk );
static void BCT_CLIENT_ScoreEffectWkInit( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, CLACT_ADD* p_ad, BCT_CLIENT_GRAPHIC* p_drawsys, u32 comm_num, u32 plno, u32 myplno, u32 heapID );
static void BCT_CLIENT_ScoreEffectWkExit( BCT_CLIENT_SCORE_EFFECT_WK* p_wk );
static void BCT_CLIENT_ScoreEffectWkStart( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 plno, u32 bonus, BCT_MARUNOMU_MOVE_TYPE movetype );
static void BCT_CLIENT_ScoreEffectWkMain( BCT_CLIENT_SCORE_EFFECT_WK* p_wk );
static void BCT_CLIENT_ScoreEffectWkSetMatrix( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 idx );
static void BCT_CLIENT_ScoreEffectWkEnd( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 idx );

// �J�n���[�N
static void BCT_CLIENT_StartSysInit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys, const BCT_GAMEDATA* cp_param, u32 commnum, u32 myplno, ARCHANDLE* p_handle, u32 heapID );
static void BCT_CLIENT_StartSysExit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static void BCT_CLIENT_StartSysCountDownInit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static BOOL BCT_CLIENT_StartSysCountDown( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static void BCT_CLIENT_StartSysDrawOff( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static void BCT_CLIENT_StartSysMarunomuChange( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static BOOL BCT_CLIENT_StartSysGameStart( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys );
static s32 BCT_CLIENT_StartSysScrllYGet( BCT_COUNTDOWN_DRAW* p_graphic );
static void BCT_CLIENT_StartSysMarunomuMatrixSet( BCT_CLIENT_GRAPHIC* p_drawsys, const BCT_MARUNOMU* cp_data );

// ���ʔ��\���[�N
static void BCT_CLIENT_EndSysInit( BCT_CLIENT_GRAPHIC* p_graphic, u32 comm_num, u32 plno, u32 heapID );
static void BCT_CLIENT_EndSysExit( BCT_CLIENT_GRAPHIC* p_graphic, u32 comm_num );
static void BCT_CLIENT_EndSysMarunomuAnmChg( BCT_CLIENT_GRAPHIC* p_graphic );
static BOOL BCT_CLIENT_EndSysMarunomuAnm( BCT_CLIENT_GRAPHIC* p_graphic );
static BOOL BCT_CLIENT_EndSysTimeUpAnm( BCT_CLIENT_GRAPHIC* p_graphic );

// �}���m�[��
static void BCT_CLIENT_MarunomuInit( const BCT_CLIENT* cp_wk, BCT_MARUNOMU* p_maru );
static void BCT_CLIENT_MarunomuMain( const BCT_CLIENT* cp_wk, BCT_MARUNOMU* p_maru );
static void BCT_CLIENT_MarunomuSetHitFlag( BCT_MARUNOMU* p_maru, BOOL flag );
static void BCT_CLIENT_MarunomuRotaXSet( BCT_MARUNOMU* p_maru, u16 rota_x );
static void BCT_CLIENT_MarunomuRotaYSet( BCT_MARUNOMU* p_maru, u16 rota_y );
static u32 BCT_CLIENT_MarunomuMouthHitCheck( const BCT_MARUNOMU* cp_maru, const VecFx32* cp_mat, const VecFx32* cp_move, VecFx32* p_inryoku );
static BOOL BCT_CLIENT_MarunomuBodyHitCheck( const BCT_MARUNOMU* cp_maru, const VecFx32* cp_mat );
static fx32 BCT_CLIENT_MarunomuMouthSizeGet( const BCT_MARUNOMU* cp_maru, u16 rota_x );
static fx32 BCT_CLIENT_MarunomuBodyHeightGet( const BCT_MARUNOMU* cp_maru,  u16 rota_x );
static void BCT_CLIENT_MaruScaleSet( BCT_MARUNOMU* p_maru, fx32 scale );
static void BCT_CLIENT_MaruRotSpeedSet( BCT_MARUNOMU* p_maru, u16 speed );
static void BCT_CLIENT_MaruMatZSet( BCT_MARUNOMU* p_maru, fx32 z );
static u16 BCT_CLIENT_MarunomuGetRotaX_Easy( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuGetRotaX_FeverEff( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuGetRotaX_Fever( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time );

//////// ���삳���Ȃ��֐�
#if 0
static u16 BCT_CLIENT_MarunomuParRotaGet( s32 par, s32 max_par );
static u16 BCT_CLIENT_MarunomuParRotaGetEx( s32 par, s32 max_par, u16 start, u16 div );
static u16 BCT_CLIENT_MarunomuRotaSpeedGet( s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuXRotaGet( s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuRotaSpeedGet2( s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuXRotaGet2( s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuRotaSpeedGet3( s32 time, s32 max_time );
static u16 BCT_CLIENT_MarunomuXRotaGet3( s32 time, s32 max_time );
#endif
//////////////////////////


// �ʐM�؂̎��f�[�^�Ɩ؂̎�
static void BCT_CLIENT_NutsDataToComm( const BCT_NUT_DATA* cp_data, BCT_NUT_COMM* p_comm );
static void BCT_CLIENT_NutsCommToData( const BCT_NUT_COMM* cp_comm, BCT_NUT_DATA* p_data );

// �؂̎�
static void BCT_CLIENT_NutsSet( BCT_CLIENT* p_wk, const BCT_NUT_DATA* cp_data, u32 seq );
static void BCT_CLIENT_NutsEnd( BCT_CLIENT* p_wk, BCT_CLIENT_NUTS* p_data, BOOL in_flag, u32 bonus );
static void BCT_CLIENT_NutsDel( BCT_CLIENT_NUTS* p_data );
static void BCT_CLIENT_AllNutsMove( BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsMove( BCT_CLIENT* p_wk, BCT_CLIENT_NUTS* p_nuts );
static void BCT_CLIENT_AllNutsEndCont( BCT_CLIENT* p_wk );
static void BCT_CLIENT_AllNutsEndContAllDel( BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsEndCont( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsSeqOtherChange( BCT_CLIENT* p_wk );
static BOOL BCT_CLIENT_NutsMoveCheck( const BCT_CLIENT* cp_wk );
static fx32 BCT_CLIENT_NutsNowPowerGet( const BCT_CLIENT_NUTS* cp_nuts );
static void BCT_CLIENT_NutsMoveGet( const BCT_CLIENT_NUTS* cp_nuts, VecFx32* p_move );
static void BCT_CLIENT_NutsAwayStart( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk, BOOL maru_hit );
static void BCT_CLIENT_NutsAwayStartPowerReq( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk, BOOL maru_hit, fx32 mul_power, BOOL move_side );
static void BCT_CLIENT_NutsAwayMove( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk );
static BOOL BCT_CLIENT_NutsAwayDitchHitCheck( const BCT_CLIENT_NUTS* cp_nuts );
static BOOL BCT_CLIENT_NutsAwayMarunomuHitCheck( const BCT_CLIENT_NUTS* cp_nuts );
static BOOL BCT_CLIENT_NutsAwayBlockHitCheck( BCT_CLIENT_NUTS* p_nuts );
static void BCT_CLIENT_NutsAwayRetWayMove( BCT_CLIENT_NUTS* p_nuts, fx32 dist );
static void BCT_CLIENT_NutsInMoveStart( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsInMove( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsMatrixMove( BCT_CLIENT_NUTS* p_nuts, const VecFx32* cp_add );


// �^�b�`�ɂ�铮��
static void BCT_CLIENT_NutsSlow( BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsSlowEnd( BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsSlowMain( BCT_CLIENT* p_wk );
static BOOL BCT_CLIENT_NutsSlowStartCheck( BCT_CLIENT* p_wk );
static void BCT_CLIENT_NutsSlowQInit( BCT_CLIENT_SLOW* p_wk );
static void BCT_CLIENT_NutsSlowQPush( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2 data );
static BOOL BCT_CLIENT_NutsSlowQPop( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data );
static BOOL BCT_CLIENT_NutsSlowQGetTop( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data );
static BOOL BCT_CLIENT_NutsSlowQGetTail( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data );
static u32 BCT_CLIENT_NutsSlowQGetInNum( BCT_CLIENT_SLOW* p_wk );
static void BCT_CLIENT_NutsSlowClear( BCT_CLIENT* p_wk );


// �Q�[���f�[�^�Ǘ��V�X�e��
static void BCT_CLIENT_ContGameData_Init( BCT_CLIENT_CONT_GAMEDATA* p_wk, const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype );
static void BCT_CLIENT_ContGameData_Start( BCT_CLIENT_CONT_GAMEDATA* p_wk, const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype, u16 last, u16 next );
static BOOL BCT_CLIENT_ContGameData_Main( BCT_CLIENT_CONT_GAMEDATA* p_wk );
static void BCT_CLIENT_ContGameData_Reflect( BCT_CLIENT* p_wk, const BCT_CLIENT_CONT_GAMEDATA* cp_data );
static void BCT_CLIENT_GameDataOne_Reflect( BCT_CLIENT* p_wk, const BCT_GAMEDATA_ONE* cp_data );
static void BCT_CLIENT_GameData_GetGameTypeOneData( const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype, u32 idx, BCT_GAMEDATA_ONE* p_buff );


// �^�b�`�y�����쏈��
static void BCT_CLIENT_TOUCHPEN_Init( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, u32 heapID );
static void BCT_CLIENT_TOUCHPEN_Exit( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT_GRAPHIC* p_graphic );
static void BCT_CLIENT_TOUCHPEN_Start( BCT_CLIENT_TOUCHPEN_MOVE* p_wk );
static BOOL BCT_CLIENT_TOUCHPEN_Main( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT* p_sys );


// �r���o�ߊǗ��V�X�e��
static void BCT_CLIENT_MDLSCR_Init( BCT_CLIENT_MIDDLE_SCORE* p_wk );
static void BCT_CLIENT_MDLSCR_Set( BCT_CLIENT_MIDDLE_SCORE* p_wk, u32 score, u32 idx );
static void BCT_CLIENT_MDLSCR_Reset( BCT_CLIENT_MIDDLE_SCORE* p_wk );
static u32 BCT_CLIENT_MDLSCR_GetTop( const BCT_CLIENT_MIDDLE_SCORE* cp_wk );
static BOOL BCT_CLIENT_MDLSCR_CheckInNum( const BCT_CLIENT_MIDDLE_SCORE* cp_wk, u32 num );



// �O���t�B�b�N������
static void BCT_CLIENT_GraphicInit( BCT_CLIENT* p_wk, u32 heapID );
static void BCT_CLIENT_GraphicDelete( BCT_CLIENT* p_wk );
static void BCT_CLIENT_GraphicMain( const BCT_CLIENT* cp_wk, BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_GraphicDrawCore( const BCT_CLIENT* cp_wk, BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_BankSet( void );
static void BCT_CLIENT_3DSetUp( void );
static void BCT_CLIENT_3DInit( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num, u32 plno, u32 heapID );
static void BCT_CLIENT_3DExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_BgInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID );
static void BCT_CLIENT_BgExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_OamInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID );
static void BCT_CLIENT_OamExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_MsgInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID );
static void BCT_CLIENT_MsgExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_CameraInit( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num, u32 plno, u32 heapID );
static void BCT_CLIENT_CameraExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_CameraMain( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_BgResLoad( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 plno, u32 heapID );
static void BCT_CLIENT_BgResRelease( BCT_CLIENT_GRAPHIC* p_wk );

static void BCT_CLIENT_MainOamInit( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 heapID );
static void BCT_CLIENT_MainOamExit( BCT_CLIENT_GRAPHIC* p_wk );

static void BCT_CLIENT_MarunomuDrawInit( BCT_MARUNOMU_DRAW* p_wk, ARCHANDLE* p_handle, u32 heapID, NNSFndAllocator* p_allocator );
static void BCT_CLIENT_MarunomuDrawExit( BCT_MARUNOMU_DRAW* p_wk, NNSFndAllocator* p_allocator );
static void BCT_CLIENT_MarunomuDrawMain( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data, BCT_MARUNOMU_MOVE_TYPE movetype );
static void BCT_CLIENT_MarunomuDrawAnmMain( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data, BCT_MARUNOMU_MOVE_TYPE movetype );
static void BCT_CLIENT_MarunomuDrawMatrixSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data );
static void BCT_CLIENT_MarunomuDrawRotaSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data );
static void BCT_CLIENT_MarunomuDrawScaleSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data );
static void BCT_CLIENT_MarunomuDrawDraw( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data );
static void BCT_CLIENT_MarunomuDrawAnmRotaSet( BCT_MARUNOMU_DRAW* p_wk, u32 rotax );
static void BCT_CLIENT_MarunomuDrawSetMouthAnm( BCT_MARUNOMU_DRAW* p_wk, u32 anmno );
static void BCT_CLIENT_MarunomuDrawLoopMouthAnm( BCT_MARUNOMU_DRAW* p_wk );
static BOOL BCT_CLIENT_MarunomuDrawNoLoopMouthAnm( BCT_MARUNOMU_DRAW* p_wk );
static BOOL BCT_CLIENT_MarunomuDrawNoLoopMouthAnm_Speed( BCT_MARUNOMU_DRAW* p_wk, fx32 speed );
static void BCT_CLIENT_MarunomuDrawSetWalkAnm( BCT_MARUNOMU_DRAW* p_wk, BOOL flag );
static void BCT_CLIENT_MarunomuDrawSetColAnmRand( BCT_MARUNOMU_DRAW* p_wk, BOOL flag );
static void BCT_CLIENT_MarunomuDrawSetColAnmTop( BCT_MARUNOMU_DRAW* p_wk, u32 plno );
static void BCT_CLIENT_MarunomuDrawColAnmMain( BCT_MARUNOMU_DRAW* p_wk );

static void BCT_CLIENT_2DMATto3DMAT( s16 x, s16 y, VecFx32* p_mat );
static void BCT_CLIENT_3DMATto2DMAT( const VecFx32* cp_mat, NNSG2dSVec2* p_2dmat );
static void BCT_CLIENT_VecNetIDRot( const VecFx32* cp_vec, VecFx32* p_invec, u32 plno, u32 comm_num );
static void BCT_CLIENT_VecNetIDRetRot( const VecFx32* cp_vec, VecFx32* p_invec, u32 plno, u32 comm_num );

static void BCT_CLIENT_NutsDrawSysInit( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 heapID  );
static void BCT_CLIENT_NutsDrawSysExit( BCT_CLIENT_GRAPHIC* p_wk );
static void BCT_CLIENT_NutsDrawSysStart( BCT_CLIENT_GRAPHIC* p_wk, const BCT_CLIENT_NUTS* cp_data, u32 comm_num, u32 plno );
static void BCT_CLIENT_NutsDrawSysEnd( BCT_CLIENT_GRAPHIC* p_wk, const BCT_CLIENT_NUTS* cp_data );
static void BCT_CLIENT_NutsDrawSysMain( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num );

static void BCT_CLIENT_NutsDrawInit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, u32 heapID );
static void BCT_CLIENT_NutsDrawExit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data );
static void BCT_CLIENT_NutsDrawStart( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data, u32 comm_num );
static void BCT_CLIENT_NutsDrawStartNoOam( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data );
static void BCT_CLIENT_NutsDrawMain( BCT_CLIENT_NUTS_DRAW* p_data, BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num );
static void BCT_CLIENT_NutsDrawShadowMatrixSet( BCT_CLIENT_NUTS_DRAW* p_data );
static void BCT_CLIENT_NutsDrawShadowAlpahReset( BCT_CLIENT_NUTS_DRAW* p_data );
static void BCT_CLIENT_NutsDrawEnd( BCT_CLIENT_NUTS_DRAW* p_data );
static BCT_CLIENT_NUTS_DRAW* BCT_CLIENT_NutsDrawWkGet( BCT_CLIENT_GRAPHIC* p_wk );
static BOOL BCT_CLIENT_NutsDrawMatrixSet( BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data, u32 comm_num );
static void BCT_CLIENT_Nuts3DDrawOn( BCT_CLIENT_NUTS_DRAW* p_data, BCT_CLIENT_GRAPHIC* p_wk );
static D3DOBJ_MDL* BCT_CLIENT_Nuts3DMdlGet( const BCT_CLIENT_NUTS* cp_data, BCT_CLIENT_NUTS_RES* p_nutsres );
static void BCT_CLIENT_NutsDrawRotaSet( BCT_CLIENT_NUTS_DRAW* p_data );

static void BCT_CLIENT_HandNutsDrawInit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_HANDNUTS_DRAW* p_nuts, u32 plno, u32 heapID );
static void BCT_CLIENT_HandNutsDrawExit( BCT_CLIENT_HANDNUTS_DRAW* p_nuts );
static void BCT_CLIENT_HandNutsDrawStart( BCT_CLIENT_HANDNUTS_DRAW* p_nuts );
static void BCT_CLIENT_HandNutsDrawMain( BCT_CLIENT_HANDNUTS_DRAW* p_nuts );
static void BCT_CLIENT_HandNutsDrawEnd( BCT_CLIENT_HANDNUTS_DRAW* p_nuts );
static void BCT_CLIENT_HandNutsDrawSetMatrix( BCT_CLIENT_HANDNUTS_DRAW* p_nuts, s32 x, s32 y );

static void BCT_CLIENT_OamAwayNutsInit( BCT_CLIENT_GRAPHIC* p_drawsys, BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk, u32 plno, u32 heapID );
static void BCT_CLIENT_OamAwayNutsExit( BCT_CLIENT_GRAPHIC* p_drawsys, BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk );
static void BCT_CLIENT_OamAwayNutsMain( BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk );
static void BCT_CLIENT_OamAwayNutsStart( BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk, s32 x, s32 y );
static void BCT_CLIENT_OamAwayNutsMoveXY( int count, int countmax, int speed, int srota, int erota, s32* p_x, s32* p_y );

static void BCT_CLIENT_MainBackInit( BCT_CLIENT_MAINBACK* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, ARCHANDLE* p_handle, int comm_num, u32 plno, u32 heapID, NNSFndAllocator* p_allocator );
static void BCT_CLIENT_MainBackExit( BCT_CLIENT_MAINBACK* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, NNSFndAllocator* p_allocator );
static void BCT_CLIENT_MainBackDraw( BCT_CLIENT_MAINBACK* p_wk );
static void BCT_CLIENT_MainBackSetDrawFever( BCT_CLIENT_MAINBACK* p_wk );
static void BCT_CLIENT_MainBackSetAnmSpeed( BCT_CLIENT_MAINBACK* p_wk, fx32 speed );

static void BCT_CLIENT_CalcPlaneVecHitCheck( const VecFx32* cp_mat, const VecFx32* cp_move, const VecFx32* cp_n, fx32 d, VecFx32* p_hitmat, fx32* p_time );

static void BCT_CLIENT_CameraTargetYSet( BCT_CLIENT_GRAPHIC* p_gra, fx32 y );
static void BCT_CLIENT_CameraAngleXSet( BCT_CLIENT_GRAPHIC* p_gra, u16 angle );


// BG�D�揇�ʃX�N���[��
static void BCT_CLIENT_BGPRISCRL_Init( BCT_CLIENT_BGPRI_SCRL* p_wk, u8 plno );
static void BCT_CLIENT_BGPRISCRL_Main( BCT_CLIENT_BGPRI_SCRL* p_wk, BCT_CLIENT_GRAPHIC* p_gra );
static void BCT_CLIENT_BGPRISCRL_Req( BCT_CLIENT_BGPRI_SCRL* p_wk );
static void BCT_CLIENT_BGPRISCRL_SetPri( BCT_CLIENT_GRAPHIC* p_gra, s16 most_back, u16 plno );


// �؂̎��J�E���^�[
static void BCT_CLIENT_NUTS_COUNT_Init( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, ARCHANDLE* p_handle, u32 heapID );
static void BCT_CLIENT_NUTS_COUNT_Exit( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra );
static void BCT_CLIENT_NUTS_COUNT_Start( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, u32 count );
static void BCT_CLIENT_NUTS_COUNT_End( BCT_CLIENT_NUTS_COUNT* p_wk );
static void BCT_CLIENT_NUTS_COUNT_SetData( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, u32 count );
static void BCT_CLIENT_NUTS_COUNT_Main( BCT_CLIENT_NUTS_COUNT* p_wk );


// FEVER�G�t�F�N�g
static void BCT_CLIENT_FEVER_EFF_Start( BCT_CLIENT_FEVER_EFF_WK* p_wk );
static void BCT_CLIENT_FEVER_EFF_Main( BCT_CLIENT_FEVER_EFF_WK* p_wk, BCT_CLIENT_MAINBACK* p_mainback, BCT_MARUNOMU_DRAW* p_marunomudraw );
static void BCT_CLIENT_FEVER_EFF_Reset( BCT_CLIENT_FEVER_EFF_WK* p_wk );

//----------------------------------------------------------------------------
/**
 *  @brief  �N���C�A���g������
 *
 *  @param  heapID      �q�[�vID
 *  @param  timeover    �^�C�����~�b�g
 *  @param  comm_num    �ʐM�l��
 *  @param	cp_gamedata	�Q�[���f�[�^
 *
 *  @return �쐬�������[�N
 */
//-----------------------------------------------------------------------------
BCT_CLIENT* BCT_CLIENT_Init( u32 heapID, u32 timeover, u32 comm_num, u32 plno, BCT_GAMEDATA* cp_gamedata )
{
    BCT_CLIENT* p_wk;
	u32 check;

    p_wk = sys_AllocMemory( heapID, sizeof(BCT_CLIENT) );
    memset( p_wk, 0, sizeof(BCT_CLIENT) );

	p_wk->time_count_flag	= TRUE;
    p_wk->time				= 0;
    p_wk->time_max			= timeover;
    p_wk->comm_num			= comm_num;
	p_wk->plno				= plno;
	p_wk->cp_gamedata		= cp_gamedata;
    

    // �}���m�[��������
    BCT_CLIENT_MarunomuInit( p_wk, &p_wk->marunomu );

    // �O���t�B�b�N������
    BCT_CLIENT_GraphicInit( p_wk, heapID );

	// �^�b�`�y���V�X�e��������
	BCT_CLIENT_TOUCHPEN_Init( &p_wk->touchpen_wk, &p_wk->graphic, heapID );

	// �r���o�߃f�[�^	������
	BCT_CLIENT_MDLSCR_Init( &p_wk->middle_score );

	// �^�b�`�p�l��
	check = InitTP( BCT_TOUCH_BUFF, BCT_TOUCH_BUFFNUM, 4 );
	GF_ASSERT( check == TP_OK );

    
    return p_wk;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �N���C�A���g�@�j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_Delete( BCT_CLIENT* p_wk )
{
	u32 check;
	// �^�b�`�p�l���T���v�����O�I��
	check = StopTP();
	GF_ASSERT( check == TP_OK );

	// �^�b�`�y���V�X�e���j��
	BCT_CLIENT_TOUCHPEN_Exit( &p_wk->touchpen_wk, &p_wk->graphic );

    // �O���t�B�b�N�j��
    BCT_CLIENT_GraphicDelete( p_wk );
    
    sys_FreeMemoryEz( p_wk );

#ifdef PM_DEBUG
	// �H��������\��
	OS_Printf( "����̐H�ׂ���	%d\n", BCT_DEBUG_in_num );
#endif
}

//----------------------------------------------------------------------------
/**
 *  @brief  �N���C�A���g�@�J�n����
 *
 *  @param  p_wk    ���[�N
 *  @param  event   �g���l����󂯂�w��
 *
 *  @retval TRUE    �r��
 *  @retval FALSE   �I���
 *
 *  event
 *      BCT_CLIENT_STARTEVENT_COUNT_START   �J�E���g���J�n����Ƃ��ɓn���Ă��������E
 *
 */
//-----------------------------------------------------------------------------
BOOL BCT_CLIENT_StartMain( BCT_CLIENT* p_wk, u32 event )
{
    BOOL result;
    BOOL ret = TRUE;
	s32 speed;

    // �}���m�[������ɂ�����蓮����
    if(  p_wk->graphic.start.seq >= BCT_STARTSEQ_WAIT ){
		if( p_wk->gametype.rota_rev ){
			speed = -BCT_START_MARUNOMUROTA;
		}else{
			speed = BCT_START_MARUNOMUROTA;
		}
        BCT_CLIENT_MarunomuRotaYSet( &p_wk->marunomu, p_wk->marunomu.rota + speed );
	}

	// ���W�Ɗp�x��ݒ�
	BCT_CLIENT_StartSysMarunomuMatrixSet( &p_wk->graphic, &p_wk->marunomu );
	BCT_CLIENT_MarunomuDrawRotaSet( &p_wk->graphic.marunomu, &p_wk->marunomu );
	BCT_CLIENT_MarunomuDrawScaleSet( &p_wk->graphic.marunomu, &p_wk->marunomu );

    switch( p_wk->graphic.start.seq ){
    case BCT_STARTSEQ_TOUCHPEN_START:   // �^�b�`�y���J�n
		BCT_CLIENT_TOUCHPEN_Start( &p_wk->touchpen_wk );
		p_wk->graphic.start.seq ++;
		break;
		
    case BCT_STARTSEQ_TOUCHPEN_MAIN:   // �^�b�`�y�����C��
		result = BCT_CLIENT_TOUCHPEN_Main( &p_wk->touchpen_wk, p_wk );
		if( result == TRUE ){
			p_wk->graphic.start.seq ++;

			// �����J�n
			CommTimingSyncStart(BCT_SYNCID_CLIENT_TOUCHPEN_END);
		}
		break;

	case BCT_STARTSEQ_MARUNOMU_SND:   // ����炷

		// ��������������܂ő҂�
		if(!CommIsTimingSync(BCT_SYNCID_CLIENT_TOUCHPEN_END)){
			break;
		}

		p_wk->graphic.start.seq ++;
		p_wk->graphic.start.wait = BCT_START_MARU_MOUTH_OPEN_WAIT;
		break;
		
	case BCT_STARTSEQ_MARUNOMU_OPEN: // �}���m�[���̌����J���҂�

		p_wk->graphic.start.wait--;
		if( p_wk->graphic.start.wait == 50 ){
			Snd_SePlay( BCT_SND_MARUIN3 );	// �͂܂�
		}
		if( p_wk->graphic.start.wait == 20 ){
			Snd_SePlay( BCT_SND_MARUIN2 );	// ���J��
		}
		
		// �P��A�j��
		BCT_CLIENT_MarunomuDrawNoLoopMouthAnm_Speed( &p_wk->graphic.marunomu, BCT_MARUNOMU_ANM_STARTSPEED );
		if( p_wk->graphic.start.wait == 0 ){
			BCT_CLIENT_StartSysMarunomuChange( &p_wk->graphic.start, &p_wk->graphic );
			p_wk->graphic.start.seq = BCT_STARTSEQ_WAIT;
		}
		break;

    case BCT_STARTSEQ_WAIT:         // �J�E���g�_�E���J�n�҂�
        if( event == BCT_CLIENT_STARTEVENT_COUNT_START ){
            p_wk->graphic.start.seq = BCT_STARTSEQ_TEXTINIT;
        }
        break;

    case BCT_STARTSEQ_TEXTINIT:     // �e�L�X�g�̏���
        BCT_CLIENT_StartSysCountDownInit( &p_wk->graphic.start, &p_wk->graphic );
		Snd_SePlay( BCT_SND_COUNT );
        p_wk->graphic.start.seq = BCT_STARTSEQ_COUNTDOWNWAIT;
        break;
        
    case BCT_STARTSEQ_COUNTDOWNWAIT:    // �J�E���g�_�E���҂�
        result = BCT_CLIENT_StartSysCountDown( &p_wk->graphic.start, &p_wk->graphic );
        if( result == FALSE ){
            p_wk->graphic.start.seq = BCT_STARTSEQ_GAMESTARTWAIT;
        }
        break;
        
    case BCT_STARTSEQ_GAMESTARTWAIT:    // �Q�[�����o�����ԂɂȂ�܂ő҂�
        result = BCT_CLIENT_StartSysGameStart( &p_wk->graphic.start, &p_wk->graphic );
        if( result == FALSE ){
            p_wk->graphic.start.seq = BCT_STARTSEQ_END;
        }
        break;
        
    case BCT_STARTSEQ_END:          // �J�n�I���
        BCT_CLIENT_StartSysDrawOff( &p_wk->graphic.start, &p_wk->graphic ); // �����Ƃ�����
		// �}���m�[�������蔻��
		BCT_CLIENT_MarunomuSetHitFlag( &p_wk->marunomu, TRUE );
        ret = FALSE;
        break;
        
    }
	// �\��
	BCT_CLIENT_MarunomuDrawAnmMain( &p_wk->graphic.marunomu, &p_wk->marunomu, p_wk->move_type );
	BCT_CLIENT_GraphicDrawCore( p_wk, &p_wk->graphic );
    return ret;
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���ʔ��\
 *  
 *  @param  p_wk    ���[�N
 *  @param  event   �C�x���g    �O���牽���w�����󂯎��Ƃ�
 *
 *  @retval TRUE    �r��
 *  @retval FALSE   ����
 */
//-----------------------------------------------------------------------------
BOOL BCT_CLIENT_EndMain( BCT_CLIENT* p_wk, u32 event )
{
    BOOL result;
    BOOL ret = TRUE;
	s32 speed;

    switch( p_wk->graphic.result.seq ){
    case BCT_RESULT_SEQ_ENDINIT:
        // �����Ă���r���̖؂̎������ׂĐl�̓��������̂ɂ���
        BCT_CLIENT_NutsSeqOtherChange( p_wk );
        // �����Ă���r���̂��̂��j������
        BCT_CLIENT_NutsSlowClear( p_wk );

		// ������ʃu���C�g�l�X�`�F���W
        ChangeBrightnessRequest( BCT_RESULT_ENDBRIGHTNESS_SYNC, 
                BCT_GRA_SUBBRIGHT_OFFNUM, BRIGHTNESS_NORMAL, PLANEMASK_ALL, MASK_SUB_DISPLAY );

		// �}���m�[���̃A�j����ύX
		BCT_CLIENT_EndSysMarunomuAnmChg( &p_wk->graphic );

		// �^�C���A�b�v
		MNGM_COUNT_StartTimeUp( p_wk->graphic.p_countwk );

		// BGM�����ɖ߂�
		BCT_CLIENT_FEVER_EFF_Reset( &p_wk->fever_eff );

		// �؂̎��J�E���^�[�����
		BCT_CLIENT_NUTS_COUNT_End( &p_wk->graphic.nutscount );

        p_wk->graphic.result.seq ++;
        break;
        
    case BCT_RESULT_SEQ_ENDDRAW:
        result = TRUE;
        // �}���m�[����ROTA�X�O�ɖ߂�
        if( BCT_CLIENT_EndSysMarunomuAnm( &p_wk->graphic ) == FALSE ){
            result = FALSE;
        }

        // �{�[����S���������i�S���͂���j 
        BCT_CLIENT_AllNutsMove( p_wk );
		BCT_CLIENT_AllNutsEndContAllDel( p_wk );
        if( BCT_CLIENT_NutsMoveCheck( p_wk ) == TRUE ){
            result = FALSE;
        }

        // �^�C���A�b�v��\��
        if( BCT_CLIENT_EndSysTimeUpAnm( &p_wk->graphic ) == FALSE ){
            result = FALSE;
        }

		// �T�u��ʃu���C�g�l�X
		if( IsFinishedBrightnessChg( MASK_SUB_DISPLAY ) == FALSE ){
			result = FALSE;
		}
        
        // ���ׂĂ̓������I����āAevent���X�R�A�\���ɂȂ����玟�֐i��
        if( (result == TRUE) && (event == BCT_CLIENT_ENDEVENT_RESULTON) ){
			p_wk->graphic.result.count = 0;
            p_wk->graphic.result.seq++;
        }
        break;

	case BCT_RESULT_SEQ_ENDWAIT:
		// �E�G�C�g
		p_wk->graphic.result.count++;
		if( p_wk->graphic.result.count >= BCT_RESULT_ENDWAIT ){
            p_wk->graphic.result.seq++;
		}
		break;
		
    case BCT_RESULT_SEQ_END:
        ret = FALSE;
        break;
    }

	// ��]
	if( p_wk->gametype.rota_rev ){
		speed = -BCT_START_MARUNOMUROTA;
	}else{
		speed = BCT_START_MARUNOMUROTA;
	}
	BCT_CLIENT_MarunomuRotaYSet( &p_wk->marunomu, p_wk->marunomu.rota + speed );
	// ���W�Ɗp�x��ݒ�
	BCT_CLIENT_MarunomuDrawMatrixSet( &p_wk->graphic.marunomu, &p_wk->marunomu );
	BCT_CLIENT_MarunomuDrawRotaSet( &p_wk->graphic.marunomu, &p_wk->marunomu );

	// �}���m�[���A�j��
	BCT_CLIENT_MarunomuDrawAnmMain( &p_wk->graphic.marunomu, &p_wk->marunomu, p_wk->move_type );

    //�R�c�`��J�n
	BCT_CLIENT_GraphicDrawCore( p_wk, &p_wk->graphic );

	// �؂̎��J�E���^�[
	BCT_CLIENT_NUTS_COUNT_Main( &p_wk->graphic.nutscount );
	
    return ret;
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���C���֐�
 *
 *  @param  p_wk 
 *
 *  @retval TRUE    �r��
 *  @retval FALSE   �I���
 */
//-----------------------------------------------------------------------------
BOOL BCT_CLIENT_Main( BCT_CLIENT* p_wk )
{
	BOOL result;
		
	// �Q�[����ԃ`�F�b�N
	if( p_wk->move_type < BCT_MARUNOMU_MOVE_NUM-1 ){
		if( sc_BCT_MARUNOMU_MOVE_STARTTIME[ p_wk->move_type+1 ] <= p_wk->time ){
			p_wk->move_type ++;	// ���Ԃ������玟�̃Q�[���^�C�v��

			// FEVER�`�F�b�N
			if( p_wk->move_type == BCT_MARUNOMU_MOVE_FEVER_EFF ){

				// FEVER�G�t�F�N�g����
				BCT_CLIENT_FEVER_EFF_Start( &p_wk->fever_eff );
			}
		}
	}
	
	
	// �^�b�`���C��
	MainTP( &p_wk->tp_one, TP_BUFFERING, 0 );
	
    // �^�b�`�p�l������
    BCT_CLIENT_NutsSlow( p_wk );

	// �Q�[���f�[�^�Ǘ�
	{
		result = BCT_CLIENT_ContGameData_Main( &p_wk->gamedata_cont );
		if( result == FALSE ){
			BCT_CLIENT_ContGameData_Reflect( p_wk, &p_wk->gamedata_cont );
		}
	}

	// �r���̓��_�Ǘ�
	{
		u32 top_idx;
		
		result = BCT_CLIENT_MDLSCR_CheckInNum( &p_wk->middle_score, p_wk->comm_num );
		if( result == TRUE ){

			// �}���m�[���̉��̊G��ς���
			top_idx = BCT_CLIENT_MDLSCR_GetTop( &p_wk->middle_score );
//			OS_TPrintf( "now top %d\n", top_idx );
			if( top_idx != BCT_MDLSCR_TOP_DEF ){
				BCT_CLIENT_MarunomuDrawSetColAnmTop( &p_wk->graphic.marunomu, top_idx );
			}

			// �r���o�߂��Ď擾
			BCT_CLIENT_MDLSCR_Reset( &p_wk->middle_score );
		}
	}
    
    // �؂̎�����
    BCT_CLIENT_AllNutsMove( p_wk );

    // �}���m�[���𓮍삳����
    BCT_CLIENT_MarunomuMain( p_wk, &p_wk->marunomu );

    // �`��
    BCT_CLIENT_GraphicMain( p_wk, &p_wk->graphic ); 

    // �؂̎��I���Ǘ�
    BCT_CLIENT_AllNutsEndCont( p_wk );

	// FEVER�G�t�F�N�g
	BCT_CLIENT_FEVER_EFF_Main( &p_wk->fever_eff, &p_wk->graphic.mainback, &p_wk->graphic.marunomu );
	
#ifndef BCT_DEBUG_NOT_TIMECOUNT
	if( p_wk->time_count_flag == TRUE ){
		
	    p_wk->time ++;
	}
#endif
    if( p_wk->time >= p_wk->time_max ){
		p_wk->time = p_wk->time_max;
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  VBlank�֐�
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_VBlank( BCT_CLIENT* p_wk )
{
    
    // BG��������
    GF_BGL_VBlankFunc( p_wk->graphic.p_bgl );

    // Vram�]���}�l�[�W���[���s
    DoVramTransferManager();

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̌o�ߎ��Ԃ��擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
s32	BCT_CLIENT_GetTime( const BCT_CLIENT* cp_wk )
{
	return cp_wk->time;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��f�[�^�̎�M
 *
 *  @param  p_wk        ���[�N
 *  @param  cp_data     ��M�f�[�^
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_NutsDataSet( BCT_CLIENT* p_wk, const BCT_NUT_COMM* cp_data )
{
    BCT_NUT_DATA data;
    
    // �����̃f�[�^�ł͂Ȃ����`�F�b�N
    if( p_wk->plno != cp_data->pl_no ){


        // �N���C�A���g�؂̎��f�[�^�ɕϊ����Đݒ�
        BCT_CLIENT_NutsCommToData( cp_data, &data );
        BCT_CLIENT_NutsSet( p_wk, &data, BCT_NUTSSEQ_MOVEOTHER );

        // �H�ׂ��Ȃ�}���m�[����H�ׂ�����ɂ���
        if( cp_data->in_flag ){
            // �X�R�A�G�t�F�N�g���o��
            BCT_CLIENT_ScoreEffectStart( &p_wk->graphic.score_effect, cp_data->pl_no, cp_data->bonus, p_wk->move_type );
        }
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��f�[�^�擾
 *
 *  @param  p_wk        ���[�N
 *  @param  p_data      �f�[�^
 *
 *  @retval TRUE    �܂�����
 *  @retval FALSE   �����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL BCT_CLIENT_NutsDataGet( BCT_CLIENT* p_wk, BCT_NUT_COMM* p_data )
{
    int i;

    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        if( p_wk->nuts[ i ].seq == BCT_NUTSSEQ_END ){
            // �ʐM�f�[�^�ɕϊ����Đݒ�
            BCT_CLIENT_NutsDataToComm( &p_wk->nuts[i].data, p_data );
            BCT_CLIENT_NutsDel( &p_wk->nuts[ i ] );
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �݂�Ȃ̃X�R�A��ݒ肷��
 *
 *  @param  p_wk        ���[�N
 *  @param  cp_data     �f�[�^
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_AllScoreSet( BCT_CLIENT* p_wk, const BCT_SCORE_COMM* cp_data )
{
    p_wk->all_score = *cp_data;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����̃X�R�A��Ԃ�
 *
 *  @param  cp_wk       ���[�N
 *
 *  @return �X�R�A
 */
//-----------------------------------------------------------------------------
u32 BCT_CLIENT_ScoreGet( const BCT_CLIENT* cp_wk )
{
    return cp_wk->score;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�C���f�b�N�X	�ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_GameDataIdxSet( BCT_CLIENT* p_wk, u32 idx )
{
	p_wk->gamedata_last	= p_wk->gamedata_now;
	p_wk->gamedata_now	= idx;

	// �f�[�^�ύX�����N�G�X�g
	BCT_CLIENT_ContGameData_Start( &p_wk->gamedata_cont, p_wk->cp_gamedata, &p_wk->gametype, p_wk->gamedata_last, p_wk->gamedata_now );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o�߃X�R�A��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	score		�X�R�A
 *	@param	plno		�v���C���[�i���o�[
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_MiddleScoreSet( BCT_CLIENT* p_wk, u32 score, u32 plno )
{
	BCT_CLIENT_MDLSCR_Set( &p_wk->middle_score, score, plno );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���^�C�v���擾����
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_GameTypeSet( BCT_CLIENT* p_wk, const BCT_GAME_TYPE_WK* cp_data )
{
	p_wk->gametype			= *cp_data;
	p_wk->gametype_flag		= TRUE;


	//  �Q�[���f�[�^�ύX�V�X�e��������
	BCT_CLIENT_ContGameData_Init( &p_wk->gamedata_cont, p_wk->cp_gamedata, &p_wk->gametype );
	// �Q�[�������f�[�^�ݒ�
	BCT_CLIENT_ContGameData_Reflect( p_wk, &p_wk->gamedata_cont );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���^�C�v�ݒ�ς݂��`�F�b�N
 *
 *	@param	p_wk		���[�N
 *		
 *	@retval	TRUE	�ݒ�ς�
 *	@retval	FALSE	�ݒ肵�ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL BCT_CLIENT_GameTypeSetCheck( const BCT_CLIENT* cp_wk )
{
	return cp_wk->gametype_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���J�E���g�t���O��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void BCT_CLIENT_TimeCountFlagSet( BCT_CLIENT* p_wk, BOOL flag )
{
	p_wk->time_count_flag = flag;
}






//-----------------------------------------------------------------------------
/**
 *          �v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	��������ėp�@������
 *
 *	@param	p_work	���[�N
 *	@param	s_x		�J�n���W
 *	@param	e_x		�I�����W
 *	@param	s_s		�J�n���x
 *	@param	count_max �����l
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
static void BCT_AddMoveReqFx( BCT_ADDMOVE_WORK* p_work, fx32 s_x, fx32 e_x, fx32 s_s, int count_max )
{
	fx32 t_x_t;	// �^�C���̂Q��
	fx32 vot;	// �����x���^�C��
	fx32 dis;
	fx32 a;

	dis = e_x - s_x;
	
	// �����l�����߂�
	// a = 2(x - vot)/(t*t)
	t_x_t = (count_max * count_max) << FX32_SHIFT;
	vot = FX_Mul( s_s, count_max * FX32_ONE );
	vot = dis - vot;
	vot = FX_Mul( vot, 2*FX32_ONE );
	a = FX_Div( vot, t_x_t );

	p_work->x = s_x;
	p_work->s_x = s_x;
	p_work->s_s = s_s;
	p_work->s_a = a;
	p_work->count = 0;
	p_work->count_max = count_max;
}
//----------------------------------------------------------------------------
/**
 *	@brief	�������상�C��
 *
 *	@param	p_work	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL	BCT_AddMoveMainFx( BCT_ADDMOVE_WORK* p_work )
{
	fx32 dis;
	fx32 t_x_t;
	fx32 calc_work;
	fx32 vot;
	
	// �������x�^��
	// dis = vot + 1/2( a*(t*t) )
	vot = FX_Mul( p_work->s_s, p_work->count << FX32_SHIFT );
	t_x_t = (p_work->count * p_work->count) << FX32_SHIFT;
	calc_work = FX_Mul( p_work->s_a, t_x_t );
	calc_work = FX_Div( calc_work, 2*FX32_ONE );	// 1/2(a*(t*t))
	dis = vot + calc_work;	///<�ړ�����

	p_work->x = p_work->s_x + dis;


	if( (p_work->count + 1) <= p_work->count_max ){
		p_work->count++;
		return FALSE;
	}

	p_work->count = p_work->count_max;
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �l�b�gID�ɑΉ������QD�X�R�A�\���ʒu���擾����
 *
 *  @param  comm_num        �ʐM�l��
 *  @param  myplno         ������NETID
 *  @param  targetplno     ���W���ق����l��NETID
 *  @param  p_x             �����W�i�[��
 *  @param  p_y             �����W�i�[��
 */
//-----------------------------------------------------------------------------
static void BCT_netID2DMatrixGet( u32 comm_num, u32 myplno, u32 targetplno, s32* p_x,  s32* p_y )
{

	//											// plno				commnum			 draw plno
	static const Vec2DS32 sc_BCT_2DMAT_TBL[ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ][ BCT_PLAYER_NUM ] = {
		// ������PLNO��0
		{	
			// ���������̂Ƃ��͂Ȃ�
			{0},
			// 2�l�ΐ�
			{
				{128,144},
				{128,48},
			},
			// 3�l�ΐ�
			{
				{128,144},
				{208,64},
				{48,64},
			},
			// 4�l�ΐ�
			{
				{128,144},
				{128,48},
				{208,96},
				{48,96},
			},
		},

		// ������PLNO��1
		{	
			// ���������̂Ƃ��͂Ȃ�
			{0},
			// 2�l�ΐ�
			{
				{128,48},
				{128,144},
			},
			// 3�l�ΐ�
			{
				{48,64},
				{128,144},
				{208,64},
			},
			// 4�l�ΐ�
			{
				{128,48},
				{128,144},
				{48,96},
				{208,96},
			},
		},

		// ������PLNO��2
		{	
			// ���������̂Ƃ��͂Ȃ�
			{0},
			// 2�l�Ȃ�
			{
				{0},
			},
			// 3�l�ΐ�
			{
				{208,64},
				{48,64},
				{128,144},
			},
			// 4�l�ΐ�
			{
				{48,96},
				{208,96},
				{128,144},
				{128,48},
			},
		},

		// ������PLNO��3
		{	
			// ���������̂Ƃ��͂Ȃ�
			{0},
			// 2�l�Ȃ�
			{
				{0},
			},
			// 3�l�Ȃ�
			{
				{0,0},
			},
			// 4�l�ΐ�
			{
				{208,96},
				{48,96},
				{128,48},
				{128,144},
			},
		},
	};

    *p_x = sc_BCT_2DMAT_TBL[ myplno ][ comm_num-1 ][ targetplno ].x;
    *p_y = sc_BCT_2DMAT_TBL[ myplno ][ comm_num-1 ][ targetplno ].y;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ȒP�؂̎����˃V�X�e��
 *
 *	@param	p_wk		�N���C�A���g���[�N
 *	@param	t0_x		�^�b�`�n�_	���W
 *	@param	t0_y
 *	@param	t1_x		�^�b�`�I�_�@���W
 *	@param	t1_y
 *	@pram	moveseq		����V�[�P���X
 */
//-----------------------------------------------------------------------------
static void BCT_EasyNutsSet( BCT_CLIENT* p_wk, u16 t0_x, u16 t0_y, u16 t1_x, u16 t1_y, u32 moveseq )
{
    BCT_NUT_DATA data;
    NNSG2dSVec2 vec;
    BOOL result;
    s32 vec_num;
	BOOL miss_slow = FALSE;
	fx32 xz_power;

	
	//  �����f�[�^���쐬����
	data.plno = p_wk->plno;
	data.in_flag = FALSE;

	// ���������������߂�
	vec.x = (t0_x - t1_x) / 2;
	vec.y = (t0_y - t1_y) / 2;

        
	data.way.x = vec.x << FX32_SHIFT;
	data.way.z = vec.y << FX32_SHIFT;
	data.way.y = 0;
	xz_power = VEC_Mag( &data.way );	// �������񕽖ʏ�̃p���[���v�Z


	// �͂̒l����
	if( xz_power > BCT_NUTS_POWER_MAX ){
		// �͂𒲐߂��Ė؂̎��ݒ�
		VEC_Normalize( &data.way, &data.way );
		data.way.x = FX_Mul( data.way.x, BCT_NUTS_POWER_MAX );
		data.way.z = FX_Mul( data.way.z, BCT_NUTS_POWER_MAX );
	}

	data.way.y = BCT_NUTS_POWRT_YPOWER;
	data.power = VEC_Mag( &data.way );
	data.power = FX_Mul( data.power, BCT_NUTS_POWER_MUL );  // �p���[����
	VEC_Normalize( &data.way, &data.way );

	// ���W�ʒu�A�����������NETID�Ɗ֘A�Â���
	// ���W�̓^�b�`���Ă���ʒu�ɂ���ĕς��
	BCT_CLIENT_2DMATto3DMAT( t1_x, t1_y, &data.mat );
	data.mat.y = BCT_FIELD_PLAYER_Y;


	// �����ƍ��W��NETID�Ɋ֘A�t����
	BCT_CLIENT_VecNetIDRot( &data.mat, &data.mat, data.plno, p_wk->comm_num );
	BCT_CLIENT_VecNetIDRot( &data.way, &data.way, data.plno, p_wk->comm_num );

	data.special = FALSE;
	
	// �o�^
	BCT_CLIENT_NutsSet( p_wk, &data, moveseq );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �X�R�A�ɑ����l���擾����
 *
 *  @param  cp_wk   ���[�N
 *
 *  @return �X�R�A�ɑ����l
 */
//-----------------------------------------------------------------------------
static u32 BCT_AddScoreGet( BCT_CLIENT* cp_wk )
{
    u8 idx;
	u32 score;

    if( cp_wk->bonus >= BCT_MARUNOMU_SCORE_TYPENUM ){
        idx = BCT_MARUNOMU_SCORE_TYPENUM - 1;
    }else{
        idx = cp_wk->bonus;
    }
//	OS_Printf( "BCT_SCORE_NUM[ idx ] %d\n", BCT_SCORE_NUM[ idx ] );
	score = BCT_SCORE_NUM[ idx ];

	// FEVER�`�F�b�N
	if( cp_wk->move_type == BCT_MARUNOMU_MOVE_FEVER ){
		score *= BCT_MARUNOMU_SCORE_FEVER_MUL;
	}
    return score;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �X�R�A�G�t�F�N�g�V�X�e��    ������
 *
 *  @param  p_wk            ���[�N
 *  @param  p_drawsys       �`��V�X�e��
 *  @param  comm_num        �ʐM�l��
 *  @param	plno			�ʐMID
 *  @param  heapID          �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectInit( BCT_CLIENT_SCORE_EFFECT* p_wk, BCT_CLIENT_GRAPHIC* p_drawsys, u32 comm_num, u32 plno, u32 heapID )
{
    CLACT_ADD add;
    int i, j;
    u32 myplno;

    add.ClActSet    = p_drawsys->clactSet;  // �Z���A�N�^�[�Z�b�g
    add.ClActHeader = &p_drawsys->mainoamheader;// �w�b�_�[�f�[�^
    add.sca.x       = FX32_ONE;     // �g��l
    add.sca.y       = FX32_ONE;     // �g��l
    add.sca.z       = FX32_ONE;     // �g��l
    add.rot         = 0;        // ��]     (0�`65535)
    add.pri         = 0;        // �D�揇��
    add.DrawArea    = NNS_G2D_VRAM_TYPE_2DMAIN; // �`���
    add.heap        = heapID;       // �g�p����q�[�v

    // �ʐM�l�����ǉ�
    myplno = plno;
    for( i=0; i<BCT_PLAYER_NUM; i++ ){
		BCT_CLIENT_ScoreEffectWkInit( &p_wk->wk[i], &add, p_drawsys, comm_num, i, myplno, heapID );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �X�R�A�G�t�F�N�g�V�X�e��    �j��
 *
 *  @param  p_wk        ���[�N
 *  @param  p_drawsys   �`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectExit( BCT_CLIENT_SCORE_EFFECT* p_wk, BCT_CLIENT_GRAPHIC* p_drawsys)
{
    int i, j;

    for( i=0; i<BCT_PLAYER_NUM; i++ ){
		BCT_CLIENT_ScoreEffectWkExit( &p_wk->wk[i] );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �X�R�A�G�t�F�N�g�J�n
 *
 *  @param  p_wk    ���[�N
 *  @param  plno   �l�b�gID
 *  @param  bonus   �{�[�i�X�i���o�[
 *  @param	movetype����^�C�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectStart( BCT_CLIENT_SCORE_EFFECT* p_wk, u32 plno, u32 bonus, BCT_MARUNOMU_MOVE_TYPE movetype )
{
	BCT_CLIENT_ScoreEffectWkStart( &p_wk->wk[plno], plno, bonus, movetype );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A�G�t�F�N�g���C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectMain( BCT_CLIENT_SCORE_EFFECT* p_wk )
{
	int i;

    for( i=0; i<BCT_PLAYER_NUM; i++ ){
		BCT_CLIENT_ScoreEffectWkMain( &p_wk->wk[i] );
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A�G�t�F�N�g���[�N	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_ad		�A�b�h�f�[�^
 *	@param	p_drawsys	�`��V�X�e��
 *	@param	comm_num	�ʐM�l��
 *	@param	plno		�l�b�gID
 *	@param	myplno		������NETID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkInit( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, CLACT_ADD* p_ad, BCT_CLIENT_GRAPHIC* p_drawsys, u32 comm_num, u32 plno, u32 myplno, u32 heapID )
{
	int i;
    s32 x, y;

	for( i=0; i<BCT_SCORE_EFFECT_BUF; i++ ){
		BCT_netID2DMatrixGet( comm_num, myplno, plno, &x, &y );
		p_ad->mat.x = x << FX32_SHIFT; 
		p_ad->mat.y = y << FX32_SHIFT; 
		p_wk->mat[i] = p_ad->mat;
		p_wk->p_clwk[i] = CLACT_Add( p_ad );

		// �A�j���ݒ�
		CLACT_AnmChg( p_wk->p_clwk[i], BCT_GRA_OAMMAIN_ANM_100 );

		// �\��OFF
		CLACT_SetDrawFlag( p_wk->p_clwk[i], FALSE );

		// �I�[�g�A�j���ݒ�
		CLACT_SetAnmFlag( p_wk->p_clwk[i], TRUE );
		CLACT_SetAnmFrame( p_wk->p_clwk[i], FX32_CONST(1.5) );

		// ����J�E���^
		p_wk->count[i] = 0;
	}

	// �����̃G�t�F�N�g���`�F�b�N
	if( myplno == plno ){
		p_wk->mydata = TRUE;
	}else{
		p_wk->mydata = FALSE;
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�j������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkExit( BCT_CLIENT_SCORE_EFFECT_WK* p_wk )
{
	int i;

	for( i=0; i<BCT_SCORE_EFFECT_BUF; i++ ){
		CLACT_Delete( p_wk->p_clwk[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���J�n
 *
 *	@param	p_wk	���[�N
 *	@param	plno	�v���C���[�i���o�[
 *	@param	bonus	�{�[�i�X
 *	@param	movetype����^�C�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkStart( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 plno, u32 bonus, BCT_MARUNOMU_MOVE_TYPE movetype )
{
	int i;
	int idx;
	BOOL clean_in;
	u32 pri;
	u32 max_pri;
	u32 anm_seq;
	u32 palno;

	// �󂢂Ă�Ƃ����T���Ȃ���
	// �݂�Ȃ̃v���C�I���e�B���P������
	// �󂫂������Ȃ�v���C�I���e�B���ő�̃��[�N���g��
	clean_in = FALSE;
	max_pri = 0;
	idx = -1;
	for( i=0; i<BCT_SCORE_EFFECT_BUF; i++ ){
		if( CLACT_GetDrawFlag( p_wk->p_clwk[i] ) == FALSE ){
			idx = i;	// ������g��
			clean_in = TRUE;
		}else{
			// �D�揇�ʂ��P������
			pri = CLACT_DrawPriorityGet( p_wk->p_clwk[i] );
			pri ++;
			CLACT_DrawPriorityChg( p_wk->p_clwk[i], pri );

			// �󂫃��[�N���������ĂȂ��Ȃ�
			// �ő�v���C�I���e�B���[�N��ݒ�
			if( (clean_in == FALSE) && (max_pri <= pri) ){
				max_pri = pri;
				idx = i;
			}
		}
	}
	GF_ASSERT( idx >= 0 );	// �����ł��Ȃ�

	if( bonus >= BCT_MARUNOMU_SCORE_TYPENUM ){
		bonus = BCT_MARUNOMU_SCORE_TYPENUM - 1;
	}

	// �f�[�^�ݒ�
	if( movetype != BCT_MARUNOMU_MOVE_FEVER ){
		if( p_wk->mydata == TRUE ){
			anm_seq = bonus + BCT_GRA_OAMMAIN_ANM_100;
		}else{
			anm_seq = bonus + BCT_GRA_OAMMAIN_ANM_100_OTHER;
		}
		palno	= BCT_GRA_OAMMAIN_PAL_SCORE0+plno;
	}else{
		if( p_wk->mydata == TRUE ){
			anm_seq = bonus + BCT_GRA_OAMMAIN_ANM_FE100;
		}else{
			anm_seq = bonus + BCT_GRA_OAMMAIN_ANM_FE100_OTHER;
		}
		palno	= BCT_GRA_OAMMAIN_PAL_SCOREEX;
	}
	CLACT_AnmChg( p_wk->p_clwk[idx], anm_seq );
	p_wk->count[idx] = 0;
	CLACT_SetDrawFlag( p_wk->p_clwk[idx], TRUE );	// �`��J�n
	BCT_CLIENT_ScoreEffectWkSetMatrix( p_wk, idx );	// ���W
	CLACT_DrawPriorityChg( p_wk->p_clwk[idx], BCT_SCORE_EFFECT_PRI_START );	// �D�揇��
	CLACT_PaletteNoChg( p_wk->p_clwk[idx], palno );	// �p���b�g

}

//----------------------------------------------------------------------------
/**
 *	@brief	���상�C��
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkMain( BCT_CLIENT_SCORE_EFFECT_WK* p_wk )
{
	int i;

	for( i=0; i<BCT_SCORE_EFFECT_BUF; i++ ){
		if( CLACT_GetDrawFlag( p_wk->p_clwk[i] ) == TRUE ){
			p_wk->count[i] ++;
			if( p_wk->count[i] > BCT_SCORE_EFFECT_MOVE_COUTN ){
				// �j��
				BCT_CLIENT_ScoreEffectWkEnd( p_wk, i );
			}else{
				BCT_CLIENT_ScoreEffectWkSetMatrix( p_wk, i );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkSetMatrix( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 idx )
{
	fx32 move_y;
	VecFx32 mat;

	move_y = FX_Div( FX_Mul( FX32_ONE * p_wk->count[idx], BCT_SCORE_EFFECT_MOVE_Y ), FX32_CONST(BCT_SCORE_EFFECT_MOVE_COUTN) );

	mat = p_wk->mat[idx];
	mat.y += move_y;
	CLACT_SetMatrix( p_wk->p_clwk[idx], &mat );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G�t�F�N�g������
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ScoreEffectWkEnd( BCT_CLIENT_SCORE_EFFECT_WK* p_wk, u32 idx )
{
	CLACT_SetDrawFlag( p_wk->p_clwk[idx], FALSE );
}




//----------------------------------------------------------------------------
/**
 *  @brief  �J�n�A�j��  ������
 *
 *  @param  p_graphic       �O���t�B�b�N���[�N
 *  @param  p_drawsys       �`��V�X�e��
 *  @param	p_handle		�n���h��
 *  @param  heapID          �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysInit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys, const BCT_GAMEDATA* cp_param, u32 commnum, u32 myplno, ARCHANDLE* p_handle, u32 heapID )
{
    STRBUF* p_str;
    CLACT_ADD add;

    memset( p_graphic, 0, sizeof(BCT_COUNTDOWN_DRAW) );

    // ���b�Z�[�W�E�B���h�E�쐬
    GF_BGL_BmpWinAdd( p_drawsys->p_bgl , &p_graphic->helpwin, GF_BGL_FRAME1_M,
            BCT_GRA_STARTWIN_X, BCT_GRA_STARTWIN_Y,
            BCT_GRA_STARTWIN_SIZX, BCT_GRA_STARTWIN_SIZY, 
            BCT_GRA_BGMAIN_PAL_FONT, BCT_GRA_STARTWIN_CGX );

    GF_BGL_BmpWinFill( &p_graphic->helpwin, 15, 0, 0, 
            BCT_GRA_STARTWIN_SIZX*8, BCT_GRA_STARTWIN_SIZY*8 );

    // ���b�Z�[�W����������
    p_str = STRBUF_Create( BCT_STRBUF_NUM, heapID );
    MSGMAN_GetString( p_drawsys->p_msgman, msg_a_001, p_str );
    GF_STR_PrintColor( &p_graphic->helpwin, FONT_SYSTEM, p_str, 
            BCT_GRA_STARTWIN_MSGX, BCT_GRA_STARTWIN_MSGY,
            MSG_NO_PUT, BCT_COL_N_BLACK, NULL);
    STRBUF_Delete( p_str );


	// ���O�X�N���[���ǂݍ���
	ArcUtil_HDL_BgCharSet( p_handle, NARC_bucket_ent_win_bg_NCGR, 
			p_drawsys->p_bgl, GF_BGL_FRAME2_M, 0, 0, FALSE, heapID );
	ArcUtil_HDL_ScrnSet( p_handle, NARC_bucket_ent_win_bg02_NSCR+(commnum-2),
			p_drawsys->p_bgl, GF_BGL_FRAME2_M, 0, 0, FALSE, heapID);
	ArcUtil_HDL_PalSet( p_handle, NARC_bucket_ent_win_bg_NCLR,
			PALTYPE_MAIN_BG, BCT_GRA_BGMAIN_PAL_NAME_PL00*32, (BCT_GRA_BGMAIN_PAL_NAME_PL03+1)*32,
			heapID );


	// ���O�������ݗp�r�b�g�}�b�v�쐬
	{
		int i;
		s32 name_x, name_y;
		GF_BGL_BMPWIN namebmpwin;
		u32 namebmp_cgx;
		STRBUF* p_namestr;
		u32 col;
		u32 namestrsize;
		u32 draw_x;

		GF_BGL_BmpWinInit( &namebmpwin );
		GF_BGL_BmpWinAdd( p_drawsys->p_bgl, &namebmpwin, GF_BGL_FRAME2_M,
						0, 0, BCT_START_NAME_BMP_WINSIZ_X, BCT_START_NAME_BMP_WINSIZ_Y, 
						BCT_GRA_BGMAIN_PAL_FONT, BCT_START_NAME_BMP_WINCGX_START );
		namebmp_cgx = BCT_START_NAME_BMP_WINCGX_START;

		p_namestr = STRBUF_Create( BCT_START_NAME_STRBUF_NUM, heapID );

		for( i=0; i<commnum; i++ ){
			// �����̖��O�͂����Ȃ�
			if( i != myplno ){
				name_x = sc_BCT_START_NAME_TBL[ myplno ][ commnum-1 ][ i ].x;
				name_y = sc_BCT_START_NAME_TBL[ myplno ][ commnum-1 ][ i ].y;
//				OS_TPrintf( "my_plno=%d comm_num=%d plno=%d name_x=%d name_y=%d\n", myplno, commnum, i, name_x, name_y );
				// ���O�̏������݂ƃt���[���J���[�ύX
				GF_BGL_ScrPalChange( p_drawsys->p_bgl, GF_BGL_FRAME2_M, name_x-1, name_y-1,
						BCT_START_NAME_FRAMESIZ_X, BCT_START_NAME_FRAMESIZ_Y, BCT_GRA_BGMAIN_PAL_NAME_PL00+i );

				// ���O��������
				GF_BGL_BmpWinDataFill( &namebmpwin, 15 );
				if( cp_param->vip[i] == TRUE ){	// ������J���[����
					col = BCT_COL_N_BLUE;
				}else{
					col = BCT_COL_N_BLACK;
				}
				MyStatus_CopyNameString( cp_param->cp_status[i], p_namestr );	// ���O�擾
				GF_BGL_BmpWinSet_PosX( &namebmpwin, name_x );	// �ʒu�ݒ�
				GF_BGL_BmpWinSet_PosY( &namebmpwin, name_y );
				namebmpwin.chrofs = namebmp_cgx;				// cgx�ݒ�
				namestrsize = FontProc_GetPrintStrWidth( FONT_SYSTEM, p_namestr, 0 );	// �\���ʒu�ݒ�
				draw_x		= ((BCT_START_NAME_BMP_WINSIZ_X*8) - namestrsize) / 2;	// �����\��
				GF_STR_PrintColor( &namebmpwin, FONT_SYSTEM, p_namestr, 
						draw_x, 0,
						MSG_ALLPUT, col, NULL);

				namebmp_cgx += BCT_START_NAME_BMP_WINCGX_ONENUM;
				
			}
		}

		STRBUF_Delete( p_namestr );

		GF_BGL_BmpWinDel( &namebmpwin );
	}


    // ������ʂ𔖈Â����Ă���
//    SetBrightness( BCT_GRA_SUBBRIGHT_OFFNUM, PLANEMASK_ALL, MASK_SUB_DISPLAY );


	// �J�E���^�S
    p_graphic->count = 0;


    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�n��ʃO���t�B�b�N�j��
 *
 *  @param  p_graphic       �O���t�B�b�N���[�N
 *  @param  p_drawsys       �\���V�X�e�����[�N
 */ 
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysExit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{
    // BMP�j��
    GF_BGL_BmpWinDel( &p_graphic->helpwin );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�E���g�_�E������������
 *
 *  @param  p_graphic       ���[�N
 *  @param  p_drawsys       �`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysCountDownInit( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{
    // �O���t�B�b�N�̕\��ON
    BmpMenuWinWrite(&p_graphic->helpwin, WINDOW_TRANS_ON,
        BCT_GRA_SYSWND_CGX, BCT_GRA_BGMAIN_PAL_SYSWND );

    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );

	//  �J�E���g�_�E���J�n
	MNGM_COUNT_StartStart( p_drawsys->p_countwk );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�E���g�_�E��
 *
 *  @param  p_graphic       ���[�N
 *  @param  p_drawsys       �\���V�X�e��
 *
 *  @retval TRUE    �r��
 *  @retval FALSE   ����
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_StartSysCountDown( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{
    BOOL result;
	
    result = MNGM_COUNT_Wait( p_drawsys->p_countwk );
    if( result == TRUE ){
        return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�n��ʕ\����OFF����
 *
 *  @param  p_graphic       ���[�N
 *  @param  p_drawsys       �`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysDrawOff( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{
    BmpMenuWinClear( &p_graphic->helpwin, WINDOW_TRANS_OFF );
    GF_BGL_BmpWinOffVReq( &p_graphic->helpwin );

    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[���ύX����
 *
 *	@param	p_graphic		
 *	@param	p_drawsys 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysMarunomuChange( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{

	// �ʏ�A�j��
	BCT_CLIENT_MarunomuDrawSetMouthAnm( &p_drawsys->marunomu, BCT_MARUNOMU_ANM_ROTA );

	// �p�x�͑S��
	D3DOBJ_AnmSet( &p_drawsys->marunomu.anm[ BCT_MARUNOMU_ANM_ROTA ], BCT_MARUNOMU_ANM_FRAME_MAX );

	// �����A�j���ݒ�
	BCT_CLIENT_MarunomuDrawSetWalkAnm( &p_drawsys->marunomu, TRUE );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�n���    �}���m�[���̌�����������@������ʂ̔�����������
 *
 *  @param  p_graphic   ���[�N
 *  @param  p_drawsys   �`��V�X�e��
 *
 *  @retval TRUE    �r��
 *  @retval FALSE   ����
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_StartSysGameStart( BCT_COUNTDOWN_DRAW* p_graphic, BCT_CLIENT_GRAPHIC* p_drawsys )
{
/*
    // �u���C�g�l�X�t�F�[�h�J�n
    if( p_graphic->count == 0 ){
        ChangeBrightnessRequest( BCT_START_BRIGHTNESS_COUNT, 
                BRIGHTNESS_NORMAL, BCT_GRA_SUBBRIGHT_OFFNUM, PLANEMASK_ALL, MASK_SUB_DISPLAY );
    }
*/
    
    p_graphic->count ++;

	if( (p_graphic->count >= BCT_START_BRIGHTNESS_COUNT) ){
		return FALSE;
	}

    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�n��ʂRD�}���m�[���ʒu�ݒ�
 *
 *	@param	p_drawsys
 *	@param	cp_data 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_StartSysMarunomuMatrixSet( BCT_CLIENT_GRAPHIC* p_drawsys, const BCT_MARUNOMU* cp_data )
{

    // ���W��ݒ�
	BCT_CLIENT_MarunomuDrawMatrixSet( &p_drawsys->marunomu, cp_data );
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���ʔ��\    ������
 *
 *  @param  p_graphic   �O���t�B�b�N
 *  @param  comm_num    �ʐM�l��
 *  @param	plno		�ʐMID
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_EndSysInit( BCT_CLIENT_GRAPHIC* p_graphic, u32 comm_num, u32 plno, u32 heapID )
{
    p_graphic->result.seq = 0;
    p_graphic->result.count = 0;

}

//----------------------------------------------------------------------------
/**
 *  @brief  ���ʔ��\    �j��
 *
 *  @param  p_graphic   �O���t�B�b�N
 *  @param  comm_num    �ʐM�l��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_EndSysExit( BCT_CLIENT_GRAPHIC* p_graphic, u32 comm_num )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[���̃A�j����ύX����
 *
 *	@param	p_graphic	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_EndSysMarunomuAnmChg( BCT_CLIENT_GRAPHIC* p_graphic )
{
	// �}���m�[���̃A�j����ύX
	BCT_CLIENT_MarunomuDrawSetMouthAnm( &p_graphic->marunomu, BCT_MARUNOMU_ANM_CLOSE );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[�����Ƃ��A�j��
 *
 *  @param  p_graphic   ���[�N
 *
 *  @retval TRUE    ����
 *  @retval FALSE   �r��
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_EndSysMarunomuAnm( BCT_CLIENT_GRAPHIC* p_graphic )
{
	BOOL result;
	
    p_graphic->result.count ++;
    // BCT_RESULT_MARUNOMU_MOVE_TIMING�܂őҋ@
    // anmtime�̒l�ŃA�j��������
    if( p_graphic->result.count >= BCT_RESULT_MARUNOMU_MOVE_TIMING ){
		result = D3DOBJ_AnmNoLoop( &p_graphic->marunomu.anm[BCT_MARUNOMU_ANM_CLOSE],
				BCT_MARUNOMU_ANM_SPEED );	
        if( result == TRUE ){
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[��  �^�C���A�b�v�A�j��
 *
 *  @param  p_graphic   ���[�N
 *
 *  @retval TRUE    ����
 *  @retval FALSE   �r��
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_EndSysTimeUpAnm( BCT_CLIENT_GRAPHIC* p_graphic )
{
    BOOL result;

    result = MNGM_COUNT_Wait( p_graphic->p_countwk );
    if( result == TRUE ){
        return TRUE;
    }
    return FALSE;
}


//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[�����[�N��������
 *  
 *  @param  cp_wk   ���[�N
 *  @param  p_maru  �}���m�[�����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuInit( const BCT_CLIENT* cp_wk, BCT_MARUNOMU* p_maru )
{
    // �X�s�[�h�ݒ�
    p_maru->speed = 0;

    // ��]�m�x
    BCT_CLIENT_MarunomuRotaYSet( p_maru, BCT_MARUNOMU_ROTA_INIT );

    // X���̉�]�p�x�ݒ�
    BCT_CLIENT_MarunomuRotaXSet( p_maru, BCT_MARUNOMU_ROTA_X_INIT );

	// �����蔻��֌W�t���O
	p_maru->hit			= FALSE;	// �����蔻��Ȃ�
	p_maru->eat_flag	= TRUE;		// �H�ׂ�t���OON

    // ���̗͂�
    p_maru->inryoku_area = BCT_MARUNOMU_INRYOKU_DIST;
    p_maru->inryoku_power = BCT_MARUNOMU_INRYOKU_POWER_PAR;

	// ���̑傫��
    p_maru->mouth_size = BCT_MARUNOMU_HIT_DIS_MAX;

	// ��{�f�[�^
	p_maru->marunomu_mat	= sc_MARUNOMU_MAT;

	// �g��k��
	BCT_CLIENT_MaruScaleSet( p_maru, BCT_MARUNOMU_SCALE_INIT );

	// ���W
	p_maru->matrix = sc_MARUNOMU_MAT;
	p_maru->height = p_maru->hitbody_y_min;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[�����상�C��
 *
 *  @param  cp_wk       ���[�N
 *  @param  p_maru      �}���m�[�����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuMain( const BCT_CLIENT* cp_wk, BCT_MARUNOMU* p_maru )
{
	static u16 (* const cp_func[ BCT_MARUNOMU_MOVE_NUM ])( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time ) = {
		BCT_CLIENT_MarunomuGetRotaX_Easy,
		BCT_CLIENT_MarunomuGetRotaX_FeverEff,
		BCT_CLIENT_MarunomuGetRotaX_Fever,
	};
    u16 rota_x; // x����]�p�x

	// �H�ׂ�t���O�ݒ�
	switch( cp_wk->move_type ){
	// FEVER�O�G�t�F�N�g
	case BCT_MARUNOMU_MOVE_FEVER_EFF:	
		p_maru->eat_flag = FALSE;
		break;
	
	// ���̑�
	default:
		p_maru->eat_flag = TRUE;
		break;
	}
	
	
    // ��]�X�s�[�h�擾
	rota_x = cp_func[ cp_wk->move_type ]( p_maru, cp_wk->time, cp_wk->time_max );
    BCT_CLIENT_MarunomuRotaXSet( p_maru, rota_x );

	// �}�E�X�T�C�Y�ݒ�
	p_maru->mouth_size = BCT_CLIENT_MarunomuMouthSizeGet( p_maru, p_maru->rotax );


	// �̓����蔻��̍�����ݒ肷��
	p_maru->height = BCT_CLIENT_MarunomuBodyHeightGet( p_maru, p_maru->rotax );

    // ��]
	BCT_CLIENT_MarunomuRotaYSet( p_maru, p_maru->rota + p_maru->speed );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[�������蔻��t���O��ݒ�
 *
 *	@param	p_maru		�}���m�[�����[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuSetHitFlag( BCT_MARUNOMU* p_maru, BOOL flag )
{
	p_maru->hit = flag;
}


//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[����]X�ݒ�
 *
 *  @param  p_maru      �}���m�[�����[�N
 *  @param  rota_x      ��]X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuRotaXSet( BCT_MARUNOMU* p_maru, u16 rota_x )
{
    p_maru->rotax = rota_x;

    // �@���x�N�g������]������
    MTX_Identity33( &p_maru->rotax_mtx );
    MTX_RotX33( &p_maru->rotax_mtx, FX_SinIdx(rota_x), 
            FX_CosIdx(rota_x) );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���̉�]Y�ݒ�
 *
 *  @param  p_maru  �}���m�[��
 *  @param  rota_y  ��]Y
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuRotaYSet( BCT_MARUNOMU* p_maru, u16 rota_y )
{
	VecFx32 center_mat;
	
    p_maru->rota = rota_y;

    // �@���x�N�g������]������
    MTX_Identity33( &p_maru->rotaxy_mtx );
    MTX_Identity33( &p_maru->rotay_mtx );
    MTX_RotY33( &p_maru->rotay_mtx, FX_SinIdx(rota_y), 
            FX_CosIdx(rota_y) );

    // �}�g���N�X���|�����킹��
    MTX_Concat33( &p_maru->rotax_mtx, &p_maru->rotay_mtx, &p_maru->rotaxy_mtx );
    MTX_MultVec33( &BCT_MARUNOMU_HIT_PLANE_N, &p_maru->rotaxy_mtx, &p_maru->hit_plane_n );

	
	// ���S�̌v�Z
	// �����蔻��G���A�̉��𒆐S�ɉ�]����悤�ɁA����G���A�̂������������_�ɂ���
	// ���Ђ傤��XY����]��������
	// ���̌�AY����]�����������蔻��̍��W�𑫂�����
	center_mat.x = 0;
	center_mat.y = 0;
	center_mat.z = -p_maru->mouth_size;
    MTX_MultVec33( &center_mat, &p_maru->rotaxy_mtx, &p_maru->center_mat );
	center_mat.x = p_maru->hit_x + p_maru->marunomu_mat.x;
	center_mat.y = p_maru->hit_y;
	center_mat.z = p_maru->marunomu_mat.z + BCT_MARUNOMU_HIT_DIS_MAX;
    MTX_MultVec33( &center_mat, &p_maru->rotay_mtx, &center_mat );
	VEC_Add( &center_mat, &p_maru->center_mat, &p_maru->center_mat  );

    // ���ʂ̕�����D�̒l�����߂�
    p_maru->d = FX_Mul( p_maru->hit_plane_n.x, p_maru->center_mat.x ) + FX_Mul( p_maru->hit_plane_n.y, p_maru->center_mat.y ) + FX_Mul( p_maru->hit_plane_n.z, p_maru->center_mat.z );

	// �\�����W���ړ�������
    MTX_MultVec33( &p_maru->marunomu_mat, &p_maru->rotay_mtx, &p_maru->matrix );
	

//  OS_Printf( "rotaY %d rotaX %d N x=0x%x y=0x%x z=0x%x\n", p_maru->rota, p_maru->rotax, p_maru->hit_plane_n.x, p_maru->hit_plane_n.y, p_maru->hit_plane_n.z );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���̌��ɓ��������`�F�b�N
 *
 *  @param  cp_maru     �}���m�[�����[�N
 *  @param  cp_mat      �����蔻�肷����̂̌��݈ʒu
 *  @param  cp_move     �����蔻�肷����̂̍��̈ړ��l
 *  @param  p_inryoku   ����
 *
 *  @retval BCT_MARUNOMU_MOUTH_HIT_NONE,    // �܂����܂ł����Ă��Ȃ�
 *  @retval BCT_MARUNOMU_MOUTH_HIT_OK,  // �������I
 *  @retval BCT_MARUNOMU_MOUTH_HIT_NG,  // �͂��ꂽ�I
 *  @retval BCT_MARUNOMU_MOUTH_HIT_NEAR,// ���͔����I
 */
//-----------------------------------------------------------------------------
static u32 BCT_CLIENT_MarunomuMouthHitCheck( const BCT_MARUNOMU* cp_maru, const VecFx32* cp_mat, const VecFx32* cp_move, VecFx32* p_inryoku )
{
    fx32 time;
    VecFx32 hitmat;
    fx32 dist;
    fx32 in_power;
    VecFx32 sub_mat;
    VecFx32 inryoku_way;
    u32 ret = BCT_MARUNOMU_MOUTH_HIT_NONE;

	// ����X�L�b�v
	if( cp_maru->hit == FALSE ){
		return BCT_MARUNOMU_MOUTH_HIT_NONE;
	}

    // ������܂ł̎��Ԃƈʒu���擾
    // ���ʂƕ��s�ɋʂ������Ă���Ƃ����m�Ȓl��
    // �Ԃ��Ȃ��\��������
    BCT_CLIENT_CalcPlaneVecHitCheck( cp_mat, cp_move, 
            &cp_maru->hit_plane_n,
            cp_maru->d, &hitmat, &time );


    // ��_�Ƃ̋��������߂�
//  VEC_Subtract( &cp_maru->center_mat, &hitmat, &sub_mat );

    // ���͍��̈ʒu�ƒ��S�Ƃ̋��������߂�
    VEC_Subtract( &cp_maru->center_mat, cp_mat, &sub_mat );
    
    dist = VEC_Mag( &sub_mat );

    // time�̕������ǂ����ɂȂ�΂悢�̂��킩��Ȃ��̂łƂ肠�����{�����ōl����
    if( (time >= 0) && (time <= FX32_ONE) ){

        // ���Ԃ�\��
//      OS_Printf( "time 0x%x\n", time );
//      OS_Printf( "dist = 0x%x limit = 0x%x \n", dist, cp_maru->mouth_size );

        // ���ʂƂ͓������Ă���̂ŁA���S����̋������擾����
        if( dist <= cp_maru->mouth_size ){
			if( cp_maru->eat_flag == TRUE ){
	            ret = BCT_MARUNOMU_MOUTH_HIT_OK;
			}else{
	            ret = BCT_MARUNOMU_MOUTH_HIT_NG;
			}
        }else{
            // NG�ɂȂ����炱�̌�̏������s���K�v�͂Ȃ�
            return BCT_MARUNOMU_MOUTH_HIT_NG;
        }
    }

    // time ���߂Â��Ă��āA�������߂���������͔���
    if( (time > 0) && (dist < cp_maru->inryoku_area) ){

        // �ŏ��͏����t�����ɔ������A���̌��C�ɋz������
        in_power = FX_Mul( (cp_maru->inryoku_area - dist), cp_maru->inryoku_power );
        VEC_Normalize( &sub_mat, &inryoku_way );
        p_inryoku->x = FX_Mul( inryoku_way.x, in_power );
        p_inryoku->y = FX_Mul( inryoku_way.y, in_power );
        p_inryoku->z = FX_Mul( inryoku_way.z, in_power );

//      OS_Printf( "dist = 0x%x in_power = 0x%x dist - in_power 0x%x \n", dist, in_power, (dist - in_power) );
//      OS_Printf( "in x 0x%x y 0x%x z 0x%x\n", p_inryoku->x, p_inryoku->y, p_inryoku->z );

        // ���͂ɂ���Č��܂ł̋������������Ă��܂��̂ł����
        // ���ɓ��������Ƃɂ���
        if( (dist - in_power) <= cp_maru->mouth_size ){
			if( cp_maru->eat_flag == TRUE ){
	            ret = BCT_MARUNOMU_MOUTH_HIT_OK;
			}else{
	            ret = BCT_MARUNOMU_MOUTH_HIT_NG;
			}
        }else{
            ret = BCT_MARUNOMU_MOUTH_HIT_NEAR;
        }
    }

    return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[���̑̂Ƃ̓����蔻��
 *
 *	@param	cp_maru		�}���m�[���f�[�^
 *	@param	cp_mat		���W
 *
 *	@retval	TRUE	�q�b�g
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_MarunomuBodyHitCheck( const BCT_MARUNOMU* cp_maru, const VecFx32* cp_mat )
{
	VecFx32 sub_vec;
	VecFx32 vec0,  vec1;
	fx32 dist;
	fx32 r;
	s32 y_dis;

	// ����X�L�b�v
	if( cp_maru->hit == FALSE ){
		return FALSE;
	}

	// �~���Ɠ_�̓����蔻��
	
	// �����������Ă��邩�`�F�b�N
	if( (cp_maru->matrix.y < cp_mat->y) && ((cp_maru->matrix.y+cp_maru->height) > cp_mat->y) ){
		
		// XZ���ʂ̔��a�����`�F�b�N
		vec0.x = cp_maru->matrix.x;
		vec0.z = cp_maru->matrix.z;
		vec0.y = 0;
		vec1.x = cp_mat->x;
		vec1.z = cp_mat->z;
		vec1.y = 0;
		VEC_Subtract( &vec0, &vec1, &sub_vec );
		dist = VEC_Mag( &sub_vec );

		// �����蔻�蔼�a�����߂�
		y_dis = cp_mat->y - cp_maru->matrix.y;
		y_dis = y_dis>>FX32_SHIFT;
		r = ( y_dis*cp_maru->hitbody_r_dis ) / (cp_maru->height >> FX32_SHIFT);
		r = cp_maru->hitbody_r_max - r;
		if( dist < r ){
			return TRUE;
		}
	}
	return FALSE;
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃T�C�Y���擾����
 *
 *	@param	cp_maru	�}���m�[�����[�N
 *	@param	rota_x	����]�p
 *
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
static fx32 BCT_CLIENT_MarunomuMouthSizeGet( const BCT_MARUNOMU* cp_maru, u16 rota_x )
{
	s32 rota_num;
	fx32 size;
	rota_num = (rota_x*360) / 0xffff;
	rota_num = BCT_MARUNOMU_ROTA_X_MAX_360 - rota_num;
	size = (rota_num*cp_maru->mouth_div) / BCT_MARUNOMU_ROTA_X_MAX_360;
	size += cp_maru->mouth_min;
	return size;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�̂̍������擾����
 *
 *	@param	cp_maru	�}���m�[�����[�N
 *	@param	rota_x	����]�p
 *
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
static fx32 BCT_CLIENT_MarunomuBodyHeightGet( const BCT_MARUNOMU* cp_maru,  u16 rota_x )
{
	s32 rota_num;
	fx32 height;
	rota_num = (rota_x*360) / 0xffff;
	height = (rota_num*cp_maru->hitbody_y_dif) / BCT_MARUNOMU_ROTA_X_MAX_360;
	height += cp_maru->hitbody_y_min;
	return height;
}

#if 0
//----------------------------------------------------------------------------
/**
 *	@brief	�p�[�Z���e�[�W�ɂ��킹��	BCT_MARUNOMU_ROTA_X_INIT->BCT_MARUNOMU_ROTA_X_ADD->BCT_MARUNOMU_ROTA_X_INIT	�̊p�x��Ԃ�
 *
 *	@param	par			���݃p�[�Z���g
 *	@param	max_par		�}�b�N�X�p�[�Z���g
 *
 *	@return	��]�p�x
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuParRotaGet( s32 par, s32 max_par )
{
	return BCT_CLIENT_MarunomuParRotaGetEx( par, max_par, BCT_MARUNOMU_ROTA_X_INIT, BCT_MARUNOMU_ROTA_X_ADD );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p�[�Z���e�[�W�ɂ��킹��	start->start+div->start	�̊p�x��Ԃ�
 *
 *	@param	par			���݃p�[�Z���g
 *	@param	max_par		�}�b�N�X�p�[�Z���g
 *	@param	start		�J�n��]�p�x
 *	@param	div			�ω��l
 *
 *	@return	��]�p�x
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuParRotaGetEx( s32 par, s32 max_par, u16 start, u16 div )
{
    fx32 add;
    s32 rota;

	//  �␳�l�����߂�
	rota = (par*180) / max_par;
	add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_X_ADD_SUB ) ) >> FX32_SHIFT;
	rota = ((par*div) / max_par) + add;

	return (u16)(rota + start);
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���̎��Ԃ̉�]�X�s�[�h�擾
 *
 *  @param  time        ���̎���
 *  @param  max_time    ��]����
 *
 *  @return ���݂̎��Ԃ̉�]�X�s�[�h
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuRotaSpeedGet( s32 time, s32 max_time )
{
    s32 par;
    fx32 add;
    s32 rota;

    par = (time*BCT_MARUNOMU_ROTACOUNT_GETNUM(2)) / max_time;
    

    // 0�`99�܂ő傫��
    // 100�`199�܂ŏ�����
    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(1) ){
        //  �␳�l�����߂�
        rota = (par*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTA_SPEED_ADD) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1)) + add;
    }else{
        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        //  �␳�l�����߂�
        rota = ((par)*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTA_SPEED_ADD) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1));
        rota = BCT_MARUNOMU_ROTA_SPEED_ADD - rota + add;
    }

    // �Ƃ肠�����A�����ɑ����Ȃ��āA�������Ȃ��Ă���
    return (rota + BCT_MARUNOMU_ROTA_SPEED_INIT);
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[����X����]�p�x�擾
 *  
 *  @param  time        ���̎���
 *  @param  max_time    ��]����
 *
 *  @return ���݂̎��Ԃ�X����]�p
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuXRotaGet( s32 time, s32 max_time )
{
	s32 par;
    s32 rota;

    par = (time*BCT_MARUNOMU_ROTACOUNT_GETNUM(2)) / max_time;

    // 0�`99�܂ő傫��
    // 100�`199�܂ŏ�����
    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(1) ){
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(1) );
    }else{
        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(1) );
		// ���]������
		rota = (BCT_MARUNOMU_ROTA_X_INIT+BCT_MARUNOMU_ROTA_X_ADD) - rota;
		if( rota < BCT_MARUNOMU_ROTA_X_MIN ){
			rota = BCT_MARUNOMU_ROTA_X_MIN;
		}
    }

    // �Ƃ肠�����A�����ɑ����Ȃ��āA�������Ȃ��Ă���
    return rota;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[����]�X�s�[�h�擾	�V�X�e���Q
 *
 *	@param	time
 *	@param	max_time 
 *
 *	@return	��]�X�s�[�h
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuRotaSpeedGet2( s32 time, s32 max_time )
{
    s32 par;
    fx32 add;
    s32 rota;

    par = (time*(BCT_MARUNOMU_ROTACOUNT_GETNUM(3))) / max_time;
    

    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(1) ){
        //  �␳�l�����߂�
        rota = (par*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTA_SPEED_ADD) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1)) + add;
    }else if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(2) ){
		par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(1);

		// ���X�s�[�h�œ�����
		rota = BCT_MARUNOMU_ROTA_SPEED_ADD;
	}else{

        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(2);
        //  �␳�l�����߂�
        rota = ((par)*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTA_SPEED_ADD) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1));
        rota = BCT_MARUNOMU_ROTA_SPEED_ADD - rota + add;
    }

    return (rota + BCT_MARUNOMU_ROTA_SPEED_INIT);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[����X����]�p�擾	�V�X�e���Q
 *
 *	@param	time		
 *	@param	max_time 
 *
 *	@return	X����]�p
 */
//-----------------------------------------------------------------------------
#define BCT_CLIENT_MARUNOMU_ROTX_2_START	( FX_GET_ROTA_NUM( 20 ) )
#define BCT_CLIENT_MARUNOMU_ROTX_2_DIV	( FX_GET_ROTA_NUM( 40 ) )
#define BCT_CLIENT_MARUNOMU_ROTAX_EX_TIME	( 500 )
static u16 BCT_CLIENT_MarunomuXRotaGet2( s32 time, s32 max_time )
{
	s32 par;
    s32 rota;

    par = (time*BCT_MARUNOMU_ROTACOUNT_GETNUM(40)) / max_time;

    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(17) ){
        
		//  �␳�l�����߂�
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(17) );
		
	}else if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(20) ){
		
		rota = BCT_MARUNOMU_ROTA_X_MAX;	// �����Ⴄ
		
    }else if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(30) ){
		
		par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(20);
		
		//  �K�R�K�R��𓮂���	�Ƃ肠����BCT_CLIENT_MARUNOMU_ROTAX_EX_TIME���Ƃɓ������Ă݂�
		rota = BCT_CLIENT_MarunomuParRotaGetEx( par%BCT_CLIENT_MARUNOMU_ROTAX_EX_TIME,
				BCT_CLIENT_MARUNOMU_ROTAX_EX_TIME, 0, BCT_CLIENT_MARUNOMU_ROTX_2_DIV );
		if( ((par/BCT_CLIENT_MARUNOMU_ROTAX_EX_TIME)%2)==0 ){
			rota = BCT_CLIENT_MARUNOMU_ROTX_2_DIV - rota;
		}
		rota += BCT_CLIENT_MARUNOMU_ROTX_2_START;
		
	}else{
		
        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(30);
        //  �␳�l�����߂�
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(10) );
		// ���]������
		rota = (BCT_MARUNOMU_ROTA_X_INIT+BCT_MARUNOMU_ROTA_X_ADD) - rota;
		if( rota < BCT_MARUNOMU_ROTA_X_MIN ){
			rota = BCT_MARUNOMU_ROTA_X_MIN;
		}

    }

    return rota;
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���̎��Ԃ̉�]�X�s�[�h�擾		�^�C�v�R
 *
 *  @param  time        ���̎���
 *  @param  max_time    ��]����
 *
 *  @return ���݂̎��Ԃ̉�]�X�s�[�h
 */
//-----------------------------------------------------------------------------
#define BCT_MARUNOMU_ROTASPEED3_MIN	( 160 )
#define BCT_MARUNOMU_ROTASPEED3_MAX	( 720 )
#define BCT_MARUNOMU_ROTASPEED3_MIN_COUNT	( 500 )
#define BCT_MARUNOMU_ROTASPEED3_MAX_COUNT	( 400 )	// �{���͂T�O�O�����S�O�O�ɂ���
													// ��̂P�O�O��MAX�X�s�[�h�ŉ�
static u16 BCT_CLIENT_MarunomuRotaSpeedGet3( s32 time, s32 max_time )
{
    s32 par;
    fx32 add;
    s32 rota;

	// �O���͏��X�ɑ����Ȃ��Ă����A
	// �㔼�́A�܂��x���Ȃ�A���̌��C�ɉ����A�������J��Ԃ��A
	// �Ō�͂������Ƃ܂�
	
    par = (time*(BCT_MARUNOMU_ROTACOUNT_GETNUM(4))) / max_time;
    

    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(2) ){
        //  �␳�l�����߂�
        rota = (par*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(2);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTA_SPEED_ADD) / BCT_MARUNOMU_ROTACOUNT_GETNUM(2)) + add;
    }else if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(3) ){
		par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(2);

		if( par < BCT_MARUNOMU_ROTASPEED3_MIN_COUNT ){
			rota = ((par*(BCT_MARUNOMU_ROTA_SPEED_ADD-BCT_MARUNOMU_ROTASPEED3_MIN)) / BCT_MARUNOMU_ROTASPEED3_MIN_COUNT);
			rota = BCT_MARUNOMU_ROTA_SPEED_ADD - rota;
		}else{
			par -= BCT_MARUNOMU_ROTASPEED3_MIN_COUNT; 
			if( par > BCT_MARUNOMU_ROTASPEED3_MAX_COUNT ){
				par = BCT_MARUNOMU_ROTASPEED3_MAX_COUNT;
			}
			rota = ((par*(BCT_MARUNOMU_ROTASPEED3_MAX - BCT_MARUNOMU_ROTASPEED3_MIN)) / BCT_MARUNOMU_ROTASPEED3_MAX_COUNT);
			rota += BCT_MARUNOMU_ROTASPEED3_MIN;
		}
	}else{

        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(3);
        //  �␳�l�����߂�
        rota = ((par)*180) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
        add = FX_Mul( Sin360( rota ), FX32_CONST( BCT_MARUNOMU_ROTA_SPEED_ADD_SUB ) ) >> FX32_SHIFT;
        rota = ((par*BCT_MARUNOMU_ROTASPEED3_MAX) / BCT_MARUNOMU_ROTACOUNT_GETNUM(1));
        rota = BCT_MARUNOMU_ROTASPEED3_MAX - rota + add;
    }

    return (rota + BCT_MARUNOMU_ROTA_SPEED_INIT);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[����X����]�p�擾	�^�C�v�R
 *
 *	@param	time		
 *	@param	max_time 
 *
 *	@return	X����]�p
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuXRotaGet3( s32 time, s32 max_time )
{
	s32 par;
    s32 rota;

    par = (time*BCT_MARUNOMU_ROTACOUNT_GETNUM(2)) / max_time;

    // 0�`99�܂ő傫��
    // 100�`199�܂ŏ�����
    if( par < BCT_MARUNOMU_ROTACOUNT_GETNUM(1) ){
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(1) );
    }else{
        par -= BCT_MARUNOMU_ROTACOUNT_GETNUM(1);
		rota = BCT_CLIENT_MarunomuParRotaGet( par, BCT_MARUNOMU_ROTACOUNT_GETNUM(1) );
		// ���]������
		rota = (BCT_MARUNOMU_ROTA_X_INIT+BCT_MARUNOMU_ROTA_X_ADD) - rota;
		if( rota < BCT_MARUNOMU_ROTA_X_MIN ){
			rota = BCT_MARUNOMU_ROTA_X_MIN;
		}
    }

    // �Ƃ肠�����A�����ɑ����Ȃ��āA�������Ȃ��Ă���
    return rota;
}
#endif

//----------------------------------------------------------------------------
/**BCT_MARUNOMU_MOVE_NUM
 *	@brief	�}���m�[���̊g�嗦��ݒ肷��
 *
 *	@param	p_maru		���[�N
 *	@param	scale		�g�嗦
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MaruScaleSet( BCT_MARUNOMU* p_maru, fx32 scale )
{
	p_maru->scale			= scale;
	p_maru->mouth_div		= FX_Mul( BCT_MARUNOMU_HIT_DIS_DIV, scale );  // �}���m�[���̌��̑傫���ω��l
	p_maru->mouth_min		= FX_Mul( BCT_MARUNOMU_HIT_DIS_MIN, scale );   // �}���m�[���̌��̑傫���ŏ�
	p_maru->mouth_max		= FX_Mul( BCT_MARUNOMU_HIT_DIS_MAX, scale );  // �}���m�[���̌��̑傫���ő�
	p_maru->hit_x			= FX_Mul( BCT_MARUNOMU_HIT_X, scale );
	p_maru->hit_y			= FX_Mul( BCT_MARUNOMU_HIT_Y, scale );
	p_maru->hitbody_y_min	= FX_Mul( BCT_MARUNOMU_HITBODY_Y_MIN, scale );
	p_maru->hitbody_y_dif	= FX_Mul( BCT_MARUNOMU_HITBODY_Y_DIF, scale );
	p_maru->hitbody_r		= FX_Mul( BCT_MARUNOMU_HITBODY_R, scale );
	p_maru->hitbody_r_max	= FX_Mul( BCT_MARUNOMU_HITBODY_R_MAX, scale );
	p_maru->hitbody_r_dis	= p_maru->hitbody_r_max - p_maru->hitbody_r;
	p_maru->draw_scale		= FX_Mul( BCT_MARUNOMU_SCALE, scale );

}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�X�s�[�h�ݒ�
 *
 *	@param	p_maru	���[�N	
 *	@param	speed	�X�s�[�h
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MaruRotSpeedSet( BCT_MARUNOMU* p_maru, u16 speed )
{
	p_maru->speed = speed;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʈʒu��ݒ�
 *
 *	@param	p_maru	���[�N
 *	@param	z		���ʈʒu
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MaruMatZSet( BCT_MARUNOMU* p_maru, fx32 z )
{
	p_maru->marunomu_mat.z = z;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[��	�C�[�W�[���쎞��X���W�擾���[�`��
 *
 *	@param	cp_maru		�}���m�[���f�[�^
 *	@param	time		���̎���
 *	@param	max_time	���Ԃ̍ő�l
 *
 *	@return	���̎��Ԃ�X��]�p�x
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuGetRotaX_Easy( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time )
{
	return BCT_MARUNOMU_ROTA_X_MIN;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[��	�t�B�[�o�[�G�t�F�N�g��
 *
 *	@param	cp_maru		�}���m�[���f�[�^
 *	@param	time		���̎���
 *	@param	max_time	���Ԃ̍ő�l
 *
 *	@return	���̎��Ԃ�X��]�p�x
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuGetRotaX_FeverEff( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time )
{
	return BCT_MARUNOMU_ROTA_X_MAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[��	�t�B�[�o�[���쎞��X���W�擾���[�`��
 *
 *	@param	cp_maru		�}���m�[���f�[�^
 *	@param	time		���̎���
 *	@param	max_time	���Ԃ̍ő�l
 *
 *	@return	���̎��Ԃ�X��]�p�x
 */
//-----------------------------------------------------------------------------
static u16 BCT_CLIENT_MarunomuGetRotaX_Fever( const BCT_MARUNOMU* cp_maru, s32 time, s32 max_time )
{
	s32 baku_time;	// �o�N�o�N���Ă鎞��
	s32 rota;
	
	//  ���Ԃ̌o�߂��獡�̊p�x������o��
	baku_time = (time - sc_BCT_MARUNOMU_MOVE_STARTTIME[ BCT_MARUNOMU_MOVE_FEVER ]) % BCT_MARUNOMU_BAKUBAKU_ONE_SYNC;

	// ��]�p�x�͂P��max->min->max�ƂȂ�K�v������̂�
	// �ړ������͂Q�{�Ȃ̂�dif*2����
	rota = (baku_time * (BCT_MARUNOMU_ROTA_X_DIF*2)) / BCT_MARUNOMU_BAKUBAKU_ONE_SYNC;

	// dif���傫���Ƃ���%dif�����l��dif�������
	if( rota > BCT_MARUNOMU_ROTA_X_DIF ){
		rota = BCT_MARUNOMU_ROTA_X_DIF - (rota % BCT_MARUNOMU_ROTA_X_DIF);
	}

	/*
	// ���̂܂܂���min->max->min�Ȃ̂�BCT_MARUNOMU_ROTA_X_DIF����rota���������l��
	// rota�Ƃ���
	rota = BCT_MARUNOMU_ROTA_X_DIF - rota;

	OS_TPrintf( "baku_time=%d  rota=%d\n",  baku_time, rota );
	//*/

	return  BCT_MARUNOMU_ROTA_X_MIN + rota;
}


//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��f�[�^��ʐM�f�[�^�ɕύX
 *
 *  @param  cp_data     ���[�N
 *  @param  p_comm      �c�[�V���f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDataToComm( const BCT_NUT_DATA* cp_data, BCT_NUT_COMM* p_comm )
{
    p_comm->pl_no = cp_data->plno;
    p_comm->in_flag = cp_data->in_flag;
    p_comm->way.x = cp_data->way.x;
    p_comm->way.y = cp_data->way.y;
    p_comm->way.z = cp_data->way.z;
    p_comm->power = FX_Mul( cp_data->power, BCT_NUTS_COMM_SCALE );
    p_comm->mat.x = cp_data->mat.x >> FX32_SHIFT;
    p_comm->mat.y = cp_data->mat.y >> FX32_SHIFT;
    p_comm->mat.z = cp_data->mat.z >> FX32_SHIFT;
    p_comm->bonus = cp_data->bonus;
	p_comm->special = cp_data->special;


    /*
    OS_Printf( "�O�f�[�^ power=%x\n", cp_data->power );
    OS_Printf( "�O�f�[�^ x=%x\n", cp_data->mat.x );
    OS_Printf( "�O�f�[�^ y=%x\n", cp_data->mat.y );
    OS_Printf( "�O�f�[�^ z=%x\n", cp_data->mat.z );
    //*/
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��ʐM�f�[�^���N���C�A���g���̃f�[�^�ɕϊ�
 *
 *  @param  cp_comm     �ʐM�f�[�^
 *  @param  p_data      �N���C�A���g���f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsCommToData( const BCT_NUT_COMM* cp_comm, BCT_NUT_DATA* p_data )
{
    p_data->plno = cp_comm->pl_no;
    p_data->in_flag = cp_comm->in_flag;
    p_data->way.x = cp_comm->way.x;
    p_data->way.y = cp_comm->way.y;
    p_data->way.z = cp_comm->way.z;
    p_data->power = FX_Div( cp_comm->power, BCT_NUTS_COMM_SCALE );
    p_data->mat.x = cp_comm->mat.x << FX32_SHIFT;
    p_data->mat.y = cp_comm->mat.y << FX32_SHIFT;
    p_data->mat.z = cp_comm->mat.z << FX32_SHIFT;
	p_data->special = cp_comm->special;

    /*
    OS_Printf( "��f�[�^ power=%x\n", p_data->power );
    OS_Printf( "��f�[�^ x=%x\n", p_data->mat.x );
    OS_Printf( "��f�[�^ y=%x\n", p_data->mat.y );
    OS_Printf( "��f�[�^ z=%x\n", p_data->mat.z );
    //*/
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��ݒ�
 *
 *  @param  p_wk        ���[�N
 *  @param  cp_data     �ݒ�f�[�^
 *  @param  seq         �ݒ肷��V�[�P���X
 */ 
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSet( BCT_CLIENT* p_wk, const BCT_NUT_DATA* cp_data, u32 seq )
{
    int i;

    // �󂢂Ă���Ƃ���ɓ����
    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        
        if( p_wk->nuts[i].seq == BCT_NUTSSEQ_NONE ){
            p_wk->nuts[i].data = *cp_data;
            p_wk->nuts[i].seq = seq;
            p_wk->nuts[i].mat = cp_data->mat;
            p_wk->nuts[i].count = 0;
			p_wk->nuts[i].blockhit = FALSE;

            BCT_CLIENT_NutsDrawSysStart( &p_wk->graphic, &p_wk->nuts[i], p_wk->comm_num, p_wk->plno );

#ifdef BCT_DEBUG
            BCT_DEBUG_NutsSetNum++;
            OS_Printf( "NutsSetNum %d\n", BCT_DEBUG_NutsSetNum );
#endif
            return ;
        }
    }
    
    // �o�b�t�@�[�I�[�o�[�����ꍇ�����̃f�[�^�Ȃ狭���I�ɓ����
    if( seq == BCT_NUTSSEQ_MOVE ){
        for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
            
            if( ( p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEOTHER ) ||
				( p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEAWAY ) ){

                // �f�[�^������
                BCT_CLIENT_NutsDrawSysEnd( &p_wk->graphic, &p_wk->nuts[i] );
                BCT_CLIENT_NutsDel( &p_wk->nuts[i] );
                
                // �ݒ�
                p_wk->nuts[i].data = *cp_data;
                p_wk->nuts[i].seq = seq;
                p_wk->nuts[i].mat = cp_data->mat;
                p_wk->nuts[i].count = 0;
				p_wk->nuts[i].blockhit = FALSE;

//				OS_Printf( "over write\n" );
                BCT_CLIENT_NutsDrawSysStart( &p_wk->graphic, &p_wk->nuts[i], p_wk->comm_num, p_wk->plno );

#ifdef BCT_DEBUG
                BCT_DEBUG_NutsSetNum++;
                OS_Printf( "NutsSetNum %d\n", BCT_DEBUG_NutsSetNum );
#endif
                return ;
            }
        }

        // ����ł��͂���Ȃ��̂��H
        GF_ASSERT(0);
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎�����I��
 *
 *  @param  p_data      ���[�N
 *  @param  in_flag     ���ɂ͂�������
 *  @param	bonus		�{�[�i�X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsEnd( BCT_CLIENT* p_wk, BCT_CLIENT_NUTS* p_data, BOOL in_flag, u32 bonus )
{
    // �I���t���O�𗧂Ă�
    p_data->end_flag = TRUE;
    
    // �������t���O��ݒ�
    p_data->data.in_flag = in_flag;

    // �������Ƃ��̃{�[�i�X
    p_data->data.bonus = bonus;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��f�[�^�j��
 *  
 *  @param  p_data  ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDel( BCT_CLIENT_NUTS* p_data )
{
#ifdef BCT_DEBUG
    BCT_DEBUG_NutsSetNum--;
    OS_Printf( "NutsSetNum %d\n", BCT_DEBUG_NutsSetNum );
#endif
    memset( p_data, 0, sizeof(BCT_CLIENT_NUTS) );
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���[�N
 *
 *  @param  p_wk    �؂̎�����
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_AllNutsMove( BCT_CLIENT* p_wk )
{
    int i;

    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        // ���삳����
		if( (p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVE) ||
	        (p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEOTHER) ){
	        BCT_CLIENT_NutsMove( p_wk, &p_wk->nuts[i] );
		}
		else if( (p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEAWAY) ){
	        BCT_CLIENT_NutsAwayMove( &p_wk->nuts[i], p_wk );
		}
		else if( (p_wk->nuts[i].seq == BCT_NUTSSEQ_INMOVE) ){
			BCT_CLIENT_NutsInMove( &p_wk->nuts[i], p_wk );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��ЂƂЂƂ̓���
 *
 *  @param  p_wk        ���[�N
 *  @param  p_nuts      �؂̎�
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsMove( BCT_CLIENT* p_wk, BCT_CLIENT_NUTS* p_nuts )
{
    VecFx32 move;
    u32 result;
    VecFx32 inryoku;
	BOOL bounus_del;
	fx32 tmp_y;
	u32 last_seq;
    
    // ���삷��K�v�����邩�H
    if( (p_nuts->seq != BCT_NUTSSEQ_MOVE) &&
        (p_nuts->seq != BCT_NUTSSEQ_MOVEOTHER) ){
        return ;
    }

	// Y���W��ۑ�
	tmp_y = p_nuts->mat.y;

	// �V�[�P���X�ۑ�
	last_seq = p_nuts->seq;

    // ��C��R���������͂��v�Z
	BCT_CLIENT_NutsMoveGet( p_nuts, &move );

    if( p_nuts->seq == BCT_NUTSSEQ_MOVE ){
        // ���Ƃ̓����蔻��
        result = BCT_CLIENT_MarunomuMouthHitCheck( &p_wk->marunomu, 
                &p_nuts->mat, &move, &inryoku );
    }else{
        result = BCT_MARUNOMU_MOUTH_HIT_NONE;
    }

#ifdef BCT_DEBUG
    if( result != 0 ){
        OS_Printf( "mouth hit = %d\n", result );
    }
#endif

    // �ړ���v�Z
	BCT_CLIENT_NutsMatrixMove( p_nuts, &move );

    // ���͕␳
    if( (result == BCT_MARUNOMU_MOUTH_HIT_NEAR) || (result == BCT_MARUNOMU_MOUTH_HIT_OK) ){
		BCT_CLIENT_NutsMatrixMove( p_nuts, &inryoku );
    }

    // �J�E���g
    p_nuts->count ++;

//  OS_Printf( "nuts move   x=0x%x y=0x%x z=0x%x\n", p_nuts->mat.x, p_nuts->mat.y, p_nuts->mat.z );

    // �ړ��悪���ɓ��������`�F�b�N
    if( result == BCT_MARUNOMU_MOUTH_HIT_OK ){
#ifdef PM_DEBUG
		BCT_DEBUG_in_num ++;
#endif

        p_wk->score += BCT_AddScoreGet( p_wk );

		if( p_wk->score > BCT_SCORE_MAX ){
			p_wk->score = BCT_SCORE_MAX;
		}

		switch( p_wk->bonus ){
		case 0:
			Snd_SePlay( BCT_SND_EAT );
			Snd_SePlay( BCT_SND_EAT100 );
			break;
		case 1:
			Snd_SePlay( BCT_SND_EAT );
			Snd_SePlay( BCT_SND_EAT200 );
			break;
		default:
			Snd_SePlay( BCT_SND_EAT );
			Snd_SePlay( BCT_SND_EAT300 );
			break;
		}

		// ���͂��蓮��J�n
		BCT_CLIENT_NutsInMoveStart( p_nuts, p_wk );

        // �X�R�A�G�t�F�N�g���o��
        BCT_CLIENT_ScoreEffectStart( &p_wk->graphic.score_effect, p_wk->plno, p_wk->bonus, p_wk->move_type );

		// �{�[�i�X�J�E���^
		if( p_wk->bonus < BCT_NUTS_BONUS_COUNT_MAX ){
			p_wk->bonus ++; 

		}

		// �؂̎��J�E���^���o�����`�F�b�N
		if( p_wk->bonus == BCT_NUTS_COUNT_START_BONUS_NUM ){
			BCT_CLIENT_NUTS_COUNT_Start( &p_wk->graphic.nutscount, &p_wk->graphic, p_wk->bonus );
		}else{
			// �؂̎��J�E���^�l�ݒ�
			BCT_CLIENT_NUTS_COUNT_SetData( &p_wk->graphic.nutscount, &p_wk->graphic, p_wk->bonus );
		}

    }else{
		bounus_del = FALSE;

		// �}���m�[���Ƃ̓����蔻��
		result = BCT_CLIENT_MarunomuBodyHitCheck( &p_wk->marunomu, &p_nuts->mat );
		if( result == TRUE ){
			// �{�[�i�X�J�E���^��j��
			bounus_del = TRUE;


			if( last_seq == BCT_NUTSSEQ_MOVE ){
				Snd_SePlay( BCT_SND_BOUND );
			}
			
			BCT_CLIENT_NutsAwayStart( p_nuts, p_wk, TRUE );
		}
		//	�n�ʂɂ��������`�F�b�N
		else if( p_nuts->mat.y <= BCT_FIELD_MIN ){
			
			// �������ʒu���a�Ȃ�j��
			result = BCT_CLIENT_NutsAwayDitchHitCheck( p_nuts );
			if( result == FALSE ){
				// �{�[�i�X�J�E���^��j��
				bounus_del = TRUE;

				if( tmp_y <= BCT_FIELD_MIN ){	//�@�a�ȊO�ŁA�ȑO��Y���W��BCT_FIELD_MIN�ȉ��Ȃ�j��
					BCT_CLIENT_NutsEnd( p_wk, p_nuts, FALSE, p_wk->bonus );
				}else{
					BCT_CLIENT_NutsAwayStart( p_nuts, p_wk, FALSE );
				}
			}else if( p_nuts->mat.y <= BCT_NUTS_AWAYPOWER_DITCHMUL ){
				// �{�[�i�X�J�E���^��j��
				bounus_del = TRUE;
				BCT_CLIENT_NutsEnd( p_wk, p_nuts, FALSE, p_wk->bonus );
			}
		}else{
			// �O�ǂƂ̓����蔻��
			result = BCT_CLIENT_NutsAwayBlockHitCheck( p_nuts );
			if( result == TRUE ){

				bounus_del = TRUE;

				// �ǂɓ�����O�ɍ��W�Ɉړ�����
				VEC_Subtract( &p_nuts->mat, &move, &p_nuts->mat );
				
				// �͂��Ȃ����ė����Ă����`
				BCT_CLIENT_NutsAwayStartPowerReq( p_nuts, p_wk, TRUE, BCT_NUTS_AWAYPOWER_DITCHMUL, FALSE );

			}
		}

		// �{�[�i�X�f�[�^�j��
		if( (bounus_del == TRUE) && (last_seq == BCT_NUTSSEQ_MOVE) ){
			p_wk->bonus = 0;

			// �؂̎��J�E���^�N���A
			BCT_CLIENT_NUTS_COUNT_End( &p_wk->graphic.nutscount );
		}
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���ׂĂ̖؂̎��̏I���Ǘ�
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_AllNutsEndCont( BCT_CLIENT* p_wk )
{
    int i;

    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        BCT_CLIENT_NutsEndCont( &p_wk->nuts[i], p_wk );
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ׂĂ̖؂̎��̏I���Ǘ�	���M�f�[�^�͕K���Ȃ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_AllNutsEndContAllDel( BCT_CLIENT* p_wk )
{
    int i;

    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
		if( p_wk->nuts[i].end_flag == TRUE ){
			p_wk->nuts[i].seq = BCT_NUTSSEQ_MOVEOTHER;
		}
        BCT_CLIENT_NutsEndCont( &p_wk->nuts[i], p_wk );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��̏I���Ǘ�
 *  
 *  @param  p_nuts  ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsEndCont( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk )
{
    if( p_nuts->end_flag ){
        if( (p_nuts->seq == BCT_NUTSSEQ_INMOVE) && (p_nuts->data.in_flag == TRUE) ){
            p_nuts->seq = BCT_NUTSSEQ_END;  // ���M�f�[�^�ɂ���
        }else{
            //�f�[�^������
            BCT_CLIENT_NutsDel( p_nuts );
        }
        // �\���I��
        BCT_CLIENT_NutsDrawSysEnd( &p_wk->graphic, p_nuts );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎�������@�𑼐l�p�ɕύX����
 *
 *  @param  p_wk        ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSeqOtherChange( BCT_CLIENT* p_wk )
{
    int i;

    // ����V�[�P���X��ύX����
    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        
        if( (p_wk->nuts[i].seq == BCT_NUTSSEQ_MOVE) ){
            p_wk->nuts[i].seq = BCT_NUTSSEQ_MOVEOTHER;
        }
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎������邩�`�F�b�N
 *
 *  @param  cp_wk   ���[�N
 *
 *  @retval TRUE    �����Ă���؂̎�������
 *  @retval FALSE   �����Ă���؂̎��͂Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsMoveCheck( const BCT_CLIENT* cp_wk )
{
    int i;

    for( i=0; i<BCT_NUTSBUFF_NUM; i++ ){
        
        if( (cp_wk->nuts[i].seq == BCT_NUTSSEQ_MOVE) ||
            (cp_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEOTHER) ||
            (cp_wk->nuts[i].seq == BCT_NUTSSEQ_MOVEAWAY) ||
			(cp_wk->nuts[i].seq == BCT_NUTSSEQ_INMOVE) ){
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃p���[���擾����
 *
 *	@param	cp_nuts		�؂̎�
 *
 *	@return	�p���[
 */
//-----------------------------------------------------------------------------
static fx32 BCT_CLIENT_NutsNowPowerGet( const BCT_CLIENT_NUTS* cp_nuts )
{
    fx32 power_num;
    // ��C��R���������͂��v�Z
    power_num = cp_nuts->data.power - FX_Mul( BCT_NUTS_POWER_RES, cp_nuts->count*FX32_ONE );
    if( power_num < 0 ){
        power_num = 0;
    }
	return power_num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̖؂̎��̈ړ�����l���擾����
 *
 *	@param	cp_nuts		�؂̎����[�N
 *	@param	p_move		�ړ�����l
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsMoveGet( const BCT_CLIENT_NUTS* cp_nuts, VecFx32* p_move )
{
    fx32 power_num;
    VecFx32 move;

    // ��C��R���������͂��v�Z
    power_num = BCT_CLIENT_NutsNowPowerGet( cp_nuts );

    // ���݂̍��W���擾����
    p_move->x = FX_Mul( cp_nuts->data.way.x, power_num );   // ���x
    p_move->y = FX_Mul( cp_nuts->data.way.y, power_num );   // ���x
    p_move->y += FX_Mul( BCT_NUTS_POWER_G, cp_nuts->count*FX32_ONE );   // G
    p_move->z = FX_Mul( cp_nuts->data.way.z, power_num );   // ���x
}


//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎���ʊO�t�b�g�΂�
 *
 *	@param	p_nuts		�؂̎����[�N
 *	@param	p_wk		�N���C�A���g���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsAwayStart( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk, BOOL maru_hit )
{
	fx32 mul;
	
	if( maru_hit == TRUE ){
	
		mul = BCT_NUTS_AWAYPOWER_MUL;
	}else{
		mul = BCT_NUTS_AWAYPOWERFIELD_MUL;
	}
	
	BCT_CLIENT_NutsAwayStartPowerReq( p_nuts, p_wk, maru_hit, mul, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�͔{���w��o�[�W����
 *
 *	@param	p_nuts		�؂̎�
 *	@param	p_wk		���[�N
 *	@param	maru_hit	�}���m�[�����́A��Q���ɓ��������̂�
 *	@param	mul_power	������p���[
 *	@param	move_size	�T�C�h�ɐ؂�Ă������H
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsAwayStartPowerReq( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk, BOOL maru_hit, fx32 mul_power, BOOL move_side )
{
	fx32 tmp;
	fx32 power;
    VecFx32 move;
	u32 rand;
	
	// �}���m�[���ɂ������Ă�����way�𔽓]������
	if( maru_hit ){
		p_nuts->data.way.x = -p_nuts->data.way.x;
		p_nuts->data.way.z = -p_nuts->data.way.z;

		if( move_side ){

			rand  = gf_mtRand();
			
			// ������ƕ������䂪�܂�	�o���邾�����ɐ؂�Ă���	���ʉ��͏������̂ŏ������ق���傫�������Ⴄ
			if( MATH_ABS( p_nuts->data.way.x ) <= MATH_ABS( p_nuts->data.way.z ) ){
				power = FX_Mul( p_nuts->data.way.z, BCT_NUTS_AWAYWAY_MUL );
				if( (rand % 2) == 1 ){	// ����ŕ�����ς����Ⴄ�@���̕ӂ͓K����OK
					p_nuts->data.way.x = -power;
				}else{
					p_nuts->data.way.x = power;
				}
			}else{
				power = FX_Mul( p_nuts->data.way.x, BCT_NUTS_AWAYWAY_MUL );
				if( (rand % 2) == 1 ){	// ����ŕ�����ς����Ⴄ�@���̕ӂ͓K����OK
					p_nuts->data.way.z = -power;
				}else{
					p_nuts->data.way.z = power;
				}
			}
		}
	}

	// �p���[��������
	p_nuts->data.power = FX_Mul( p_nuts->data.power, mul_power );

	// ����J�E���^���P�ɖ߂�	�O���ƈړ������l�����������āA���˕Ԃ肫��Ȃ��ꍇ������̂łP�ɂ���
	p_nuts->count = 0;

	// �V�[�P���X��ύX
	p_nuts->seq = BCT_NUTSSEQ_MOVEAWAY;


	// 1�񓮂���
    // ��C��R���������͂��v�Z
	BCT_CLIENT_NutsMoveGet( p_nuts, &move );

    // �ړ���v�Z
	BCT_CLIENT_NutsMatrixMove( p_nuts, &move );

    p_nuts->count ++;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎���ʂ��t�b�g�΂��@���C��
 *
 *	@param	p_nuts		�؂̎����[�N	
 *	@param	p_wk		�N���C�A���g���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsAwayMove( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk )
{
    VecFx32 move;
	BOOL result;
	BOOL nutsdel = FALSE;
	fx32 tmp_y;

    // ���삷��K�v�����邩�H
    if( (p_nuts->seq != BCT_NUTSSEQ_MOVEAWAY) ){
        return ;
    }

	// y���W
	tmp_y = p_nuts->mat.y;

    // ��C��R���������͂��v�Z
	BCT_CLIENT_NutsMoveGet( p_nuts, &move );

    // �ړ���v�Z
	BCT_CLIENT_NutsMatrixMove( p_nuts, &move );

    p_nuts->count ++;

	// �}���m�[���Ɠ����蔻��
	result = BCT_CLIENT_MarunomuBodyHitCheck( &p_wk->marunomu, &p_nuts->mat );
	if( result == TRUE ){
		// ���˕Ԃ�
		BCT_CLIENT_NutsAwayStart( p_nuts, p_wk, TRUE );
	}else{

		// �n�ʂɗ������炳��ɒ��˕Ԃ�
		if( p_nuts->mat.y <= BCT_FIELD_MIN ){

			// �������`�F�b�N
			result = BCT_CLIENT_NutsAwayDitchHitCheck( p_nuts );

			if( result == FALSE ){	// ��������Ȃ璵�˕Ԃ�
				if( tmp_y <= BCT_FIELD_MIN ){
					// �j��������
					nutsdel = TRUE;
				}else{
					BCT_CLIENT_NutsAwayStart( p_nuts, p_wk, FALSE );
				}
			}else if( p_nuts->mat.y <= BCT_FIELD_Y_DEL ){
				// �j��������
				nutsdel = TRUE;
			}

			// �͂��Ȃ��Ȃ�����I����
			if( p_nuts->data.power <= BCT_NUTS_AWAYDEL_POWER2 ){
				nutsdel = TRUE;
			}

		}else{
			// �O�ǂƂ̓����蔻��
			result = BCT_CLIENT_NutsAwayBlockHitCheck( p_nuts );
			if( result == TRUE ){
				// �ǂɓ�����O�ɍ��W�Ɉړ�����
				VEC_Subtract( &p_nuts->mat, &move, &p_nuts->mat );
				
				// �͂��Ȃ����ė����Ă����`
				BCT_CLIENT_NutsAwayStartPowerReq( p_nuts, p_wk, TRUE, BCT_NUTS_AWAYPOWER_DITCHMUL, FALSE );

			}
		}
	}

	// �͂��Ȃ��Ȃ�����I����
	if( p_nuts->data.power <= BCT_NUTS_AWAYDEL_POWER ){
		nutsdel = TRUE;
	}

	// �����`�F�b�N
	if( nutsdel == TRUE ){
		BCT_CLIENT_NutsEnd( p_wk, p_nuts, FALSE, p_wk->bonus );
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�a�Ƀ{�[�����͂��������`�F�b�N
 *
 *	@param	cp_nuts		�؂̎�
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	����Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsAwayDitchHitCheck( const BCT_CLIENT_NUTS* cp_nuts )
{
	if( (cp_nuts->xz_dist >= BCT_NUTS_AWAY_DITCH_MIN_R) &&
		(cp_nuts->xz_dist <= BCT_NUTS_AWAY_DITCH_MAX_R) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}���m�[���̃G���A�Ƀ{�[�����͂��������`�F�b�N
 *
 *	@param	cp_nuts			�؂̎�
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	����Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsAwayMarunomuHitCheck( const BCT_CLIENT_NUTS* cp_nuts )
{
	if( (cp_nuts->xz_dist <= BCT_NUTS_AWAY_MARUNOMU_R) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O�ǂƂ̓����蔻��
 *
 *	@param	p_nuts		�؂̎�
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	������Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsAwayBlockHitCheck( BCT_CLIENT_NUTS* p_nuts )
{
	if( (p_nuts->xz_dist >= BCT_NUTS_AWAY_BLOCK_R) ){
		if( p_nuts->blockhit == TRUE ){
			return TRUE;
		}
	}else if( p_nuts->blockhit == FALSE ){
		p_nuts->blockhit = TRUE;	// 1��G���A���ɓ������玟����͂����Ɠ����蔻��
		
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�����Ɉړ�������
 *
 *	@param	p_nuts	�؂̎�
 *	@param	dist	����
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsAwayRetWayMove( BCT_CLIENT_NUTS* p_nuts, fx32 dist )
{
	VecFx32 move;

	move.x = FX_Mul( p_nuts->data.way.x, -dist );
	move.y = 0;
	move.z = FX_Mul( p_nuts->data.way.z, -dist );

    // �ړ���v�Z
	BCT_CLIENT_NutsMatrixMove( p_nuts, &move );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��@���ɂ͂������㓮��	�J�n
 *
 *	@param	p_nuts		�؂̎�
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsInMoveStart( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk )
{
	p_nuts->seq = BCT_NUTSSEQ_INMOVE;
	p_nuts->inmove_count = BCT_NUTS_INMOVE_COUNT;
	p_nuts->inmove_bonus = p_wk->bonus;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��@���ɓ������㓮��@���C��
 *
 *	@param	p_nuts		�؂̎�
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsInMove( BCT_CLIENT_NUTS* p_nuts, BCT_CLIENT* p_wk )
{
    VecFx32 move;
    u32 result;
    VecFx32 inryoku;
    
    // ��C��R���������͂��v�Z
	BCT_CLIENT_NutsMoveGet( p_nuts, &move );

	// ���Ƃ̓����蔻��
	result = BCT_CLIENT_MarunomuMouthHitCheck( &p_wk->marunomu, 
			&p_nuts->mat, &move, &inryoku );

    // �ړ���v�Z
	BCT_CLIENT_NutsMatrixMove( p_nuts, &move );

    // ���͕␳
    if( (result == BCT_MARUNOMU_MOUTH_HIT_NEAR) || (result == BCT_MARUNOMU_MOUTH_HIT_OK) ){
		BCT_CLIENT_NutsMatrixMove( p_nuts, &inryoku );
    }

    // �J�E���g
    p_nuts->count ++;

	// �j���J�E���g
	p_nuts->inmove_count --;
	if( p_nuts->inmove_count <= 0 ){
        BCT_CLIENT_NutsEnd( p_wk, p_nuts, TRUE, p_nuts->inmove_bonus );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��̍��W�𓮂���
 *
 *	@param	p_nuts		�؂̎�	
 *	@param	cp_add		�ړ�
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsMatrixMove( BCT_CLIENT_NUTS* p_nuts, const VecFx32* cp_add )
{
	VecFx32 sub_mat;

    VEC_Add( &p_nuts->mat, cp_add, &p_nuts->mat );

	
	// XZ���ʂ̒��S����̋��������߂�
	sub_mat.y = 0;
	sub_mat.x = p_nuts->mat.x;
	sub_mat.z = p_nuts->mat.z;
	p_nuts->xz_dist = VEC_Mag( &sub_mat );
}


//----------------------------------------------------------------------------
/**
 *  @brief  �����鏈��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlow( BCT_CLIENT* p_wk )
{
#ifdef BCT_DEBUG_AUTOSLOW
	BCT_DEBUG_AutoSlow( p_wk );
	return ;
#endif
    // �^�b�`���Ă��邩
    if( sys.tp_cont == FALSE ){
        // �������Ƃ��̏���
        BCT_CLIENT_NutsSlowEnd( p_wk );
    }else{
        // �^�b�`���̏���
        BCT_CLIENT_NutsSlowMain( p_wk );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  ������̊�����
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlowEnd( BCT_CLIENT* p_wk )
{
    BCT_NUT_DATA data;
    NNSG2dSVec2 vec;
    BOOL result;
    NNSG2dSVec2 vec0, vec1;
    s32 vec_num;
	BOOL miss_slow = FALSE;
	fx32 xz_power;


    // �^�b�`�J�n���Ă����瓊����
    if( p_wk->slow.hand == FALSE ){
		// �ʔ��ˊԊu�J�E���g
		p_wk->slow.time ++;
        return ;
    }

    // �ŐV�̍��W�Ɛ̂̍��W���擾
    result = BCT_CLIENT_NutsSlowQGetTop( &p_wk->slow, &vec0 );
    GF_ASSERT( result );
    result = BCT_CLIENT_NutsSlowQGetTail( &p_wk->slow, &vec1 );
    GF_ASSERT( result );
    vec_num = BCT_CLIENT_NutsSlowQGetInNum( &p_wk->slow );

    // ���_��BCT_NUTS_SLOW_USE_VECNUM�ȏ�Ȃ��Ƃ���
    if( vec_num >= BCT_NUTS_SLOW_USE_VECNUM ){

        //  �����f�[�^���쐬����
        data.plno = p_wk->plno;
        data.in_flag = FALSE;

#ifdef BCT_DEBUG
//        OS_Printf( "sx=%d sy=%d  ex=%d ex=%d", vec0.x, vec0.y, vec1.x, vec1.y );
#endif

        // ���������������߂�
        vec.x = (vec0.x - vec1.x) / vec_num;
        vec.y = (vec0.y - vec1.y) / vec_num;

#ifdef BCT_DEBUG
//        OS_Printf( "vec.x %d vec.y %d\n", vec.x, vec.y );
#endif
        
        data.way.x = vec.x << FX32_SHIFT;
        data.way.z = vec.y << FX32_SHIFT;
        data.way.y = 0;
        xz_power = VEC_Mag( &data.way );	// �������񕽖ʏ�̃p���[���v�Z


        // �͂̒l����
        if( xz_power < BCT_NUTS_POWER_MIN ){	// ���ꂾ�Ƃς�[������Ȃ��̂Ń~�X����
			// ������̎��s
/*
			miss_slow = TRUE;
//*/
			if( xz_power != 0 ){
				// �͂𒲐߂��Ė؂̎��ݒ�
				VEC_Normalize( &data.way, &data.way );
				data.way.x = FX_Mul( data.way.x, BCT_NUTS_POWER_MIN );
				data.way.z = FX_Mul( data.way.z, BCT_NUTS_POWER_MIN );
//				OS_Printf( "slow power low\n" );
			}else{
				// ������̎��s
				miss_slow = TRUE;
//				OS_Printf( "slow point none\n" );
			}
        }else if( xz_power > BCT_NUTS_POWER_MAX ){
			// �͂𒲐߂��Ė؂̎��ݒ�
			VEC_Normalize( &data.way, &data.way );
			data.way.x = FX_Mul( data.way.x, BCT_NUTS_POWER_MAX );
			data.way.z = FX_Mul( data.way.z, BCT_NUTS_POWER_MAX );
        }

		if( miss_slow == FALSE ){
			data.way.y = BCT_NUTS_POWRT_YPOWER;
			data.power = VEC_Mag( &data.way );
			data.power = FX_Mul( data.power, BCT_NUTS_POWER_MUL );  // �p���[����
			VEC_Normalize( &data.way, &data.way );

			// ���W�ʒu�A�����������NETID�Ɗ֘A�Â���
			// ���W�̓^�b�`���Ă���ʒu�ɂ���ĕς��
			BCT_CLIENT_2DMATto3DMAT( vec1.x, vec1.y, &data.mat );
			data.mat.y = BCT_FIELD_PLAYER_Y;


			// �؂̎����˃p���[
#ifdef BCT_DEBUG
//			OS_Printf( "---xz_power--- 0x%x\n", xz_power );
//			OS_Printf( "---power--- 0x%x\n", data.power );
#endif

			// �����ƍ��W��NETID�Ɋ֘A�t����
			BCT_CLIENT_VecNetIDRot( &data.mat, &data.mat, data.plno, p_wk->comm_num );
			BCT_CLIENT_VecNetIDRot( &data.way, &data.way, data.plno, p_wk->comm_num );

			// �{�[�i�X�����āA�X�y�V�����{�[���ɂ���
			if( p_wk->bonus >= BCT_SPECIAL_BALL_COUNT ){
				data.special = TRUE;
			}else{
				data.special = FALSE;
			}
			
			// �o�^
			BCT_CLIENT_NutsSet( p_wk, &data, BCT_NUTSSEQ_MOVE );
			Snd_SePlay( BCT_SND_SLOW );

		}
    }else{
        // ������̎��s
		miss_slow = TRUE;
//		OS_Printf( "slow point none\n" );
    }

	// MISS�X���[�G�t�F�N�g�J�n
	if( miss_slow == TRUE ){
		BCT_CLIENT_OamAwayNutsStart( &p_wk->graphic.oamnutsaway, vec1.x, vec1.y );
	}

    // �N���A�ƕ\��OFF
    BCT_CLIENT_NutsSlowClear( p_wk );

	// ���̋����ˊԊu�^�C�}�[���Z�b�g
	p_wk->slow.time = 0;
}


//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��������C��
 *
 *  @param  p_wk 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlowMain( BCT_CLIENT* p_wk )
{
    NNSG2dSVec2 data;
	BOOL result;
    
    if( p_wk->slow.hand == FALSE ){

		// �^�b�`�J�nOK�`�F�b�N
		result = BCT_CLIENT_NutsSlowStartCheck( p_wk );
		if( result == TRUE ){
			
			p_wk->slow.hand = TRUE;
			// �����Ă���؂̎��`��J�n
			BCT_CLIENT_HandNutsDrawStart( &p_wk->graphic.handnuts );

			// ������ʂ�BG�D�揇�ʃX�N���[��
			BCT_CLIENT_BGPRISCRL_Req( &p_wk->graphic.bgpri_scrl );
		}
    }

	if( p_wk->slow.hand == TRUE ){
		// �^�b�`�f�[�^���L���[�Ɋi�[
		if( p_wk->tp_one.Size == 1 ){ // �T���v�����O�ł������_���P�̏ꍇ
			data.x = p_wk->tp_one.TPDataTbl[0].x;
			data.y = p_wk->tp_one.TPDataTbl[0].y;
			BCT_CLIENT_NutsSlowQPush( &p_wk->slow, data );
		}else if( p_wk->tp_one.Size >= 2 ){	// �T���v�����O���Q�ȏ�
			data.x = p_wk->tp_one.TPDataTbl[0].x;
			data.y = p_wk->tp_one.TPDataTbl[0].y;
			BCT_CLIENT_NutsSlowQPush( &p_wk->slow, data );
			
			data.x = p_wk->tp_one.TPDataTbl[p_wk->tp_one.Size-1].x;
			data.y = p_wk->tp_one.TPDataTbl[p_wk->tp_one.Size-1].y;
			BCT_CLIENT_NutsSlowQPush( &p_wk->slow, data );
		}else{
			// �P�������Ȃ�tp_x tp_y�����悤
			data.x = sys.tp_x;
			data.y = sys.tp_y;
			BCT_CLIENT_NutsSlowQPush( &p_wk->slow, data );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��𓊂�����Ԃ��`�F�b�N
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsSlowStartCheck( BCT_CLIENT* p_wk )
{
	s32 x;
	s32 y;
	s32 dist;

	// �{�[���𗣂��Ă����莞�Ԃ����Ȃ��Əo���Ȃ�
	if( p_wk->slow.time < BCT_NUTS_SLOW_RENSYA_TIME ){
		return FALSE;
	}
	
	// ���S����̋��������߂�
	x = BCT_NUTS_SLOW_TOUCH_AREA_CENTER_X - sys.tp_x;
	y = BCT_NUTS_SLOW_TOUCH_AREA_CENTER_Y - sys.tp_y;
	dist = FX_Sqrt( ((x*x)+(y*y))<<FX32_SHIFT ) >> FX32_SHIFT;
	
	// �͈͂̓����蔻��
	if( dist <= BCT_NUTS_SLOW_TOUCH_AREA_R ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎�����Q������
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlowQInit( BCT_CLIENT_SLOW* p_wk )
{
    memset( p_wk->tp_q, 0, sizeof(NNSG2dSVec2)*BCT_NUTS_SLOW_Q);
    p_wk->top = 0;
    p_wk->tail = 0;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����f�[�^�i�[
 *
 *  @param  p_wk    ���[�N
 *  @param  data    �f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlowQPush( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2 data )
{
    // ����+1���擪�̎����t
    if( ((p_wk->tail + 1)%BCT_NUTS_SLOW_Q) == p_wk->top ){
        NNSG2dSVec2 tmp;    // �̂Ă�R�}���h
         
        // ���t
        // 1�f�[�^���|�b�v���ăf�[�^�i�[
        BCT_CLIENT_NutsSlowQPop( p_wk, &tmp );
    }

    // �f�[�^�ݒ�
    p_wk->tp_q[ p_wk->tail ] = data;

    // �����ʒu�𓮂���
    p_wk->tail = (p_wk->tail + 1)%BCT_NUTS_SLOW_Q;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �i�[�f�[�^���擾
 *
 *  @param  p_wk    ���[�N
 *  @param  p_data  �i�[��
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsSlowQPop( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data )
{
    // �擪=����    �f�[�^���Ȃ�
    if( p_wk->tail == p_wk->top ){
        return FALSE;
    }

    *p_data = p_wk->tp_q[ p_wk->top ];

    // �擪�ʒu�𓮂���
    p_wk->top = (p_wk->top + 1)%BCT_NUTS_SLOW_Q;

    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �擪�̃f�[�^���擾����  �i��ԉߋ��̃f�[�^�j
 *  
 *  @param  p_wk    ���[�N
 *  @param  p_data  �f�[�^�i�[��
 *
 *  @retval TRUE    �f�[�^���i�[�ł��܂���
 *  @retval FALSE   �f�[�^���i�[�o���܂���ł���
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsSlowQGetTop( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data )
{
    // �擪=����    �f�[�^���Ȃ�
    if( p_wk->tail == p_wk->top ){
        return FALSE;
    }

    *p_data = p_wk->tp_q[ p_wk->top ];

    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �I�[�̃f�[�^���擾����  �i��ԍŐV�̃f�[�^�j
 *
 *  @param  p_wk        ���[�N
 *  @param  p_data      �f�[�^�i�[��
 *
 *  @retval TRUE    �f�[�^���i�[�ł��܂���
 *  @retval FALSE   �f�[�^���i�[�o���܂���ł���
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsSlowQGetTail( BCT_CLIENT_SLOW* p_wk, NNSG2dSVec2* p_data )
{
    s32 idx;
    
    // �擪=����    �f�[�^���Ȃ�
    if( p_wk->tail == p_wk->top ){
        return FALSE;
    }

    idx = p_wk->tail - 1;
    if( idx < 0 ){
        idx += BCT_NUTS_SLOW_Q;
    }
    
    *p_data = p_wk->tp_q[ idx ];

    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���i�[����Ă���f�[�^�̐����擾����
 *
 *  @param  p_wk    ���[�N
 *
 *  @return �i�[����Ă���f�[�^�̐�
 */
//-----------------------------------------------------------------------------
static u32 BCT_CLIENT_NutsSlowQGetInNum( BCT_CLIENT_SLOW* p_wk )
{
    if( p_wk->top > p_wk->tail ){
        return (BCT_NUTS_SLOW_Q - p_wk->top) + p_wk->tail;
    }
    return p_wk->tail - p_wk->top;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��X���[���[�N�̃N���A  �\����OFF
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsSlowClear( BCT_CLIENT* p_wk )
{
    // ���[�N�̃N���A
    p_wk->slow.hand = FALSE;
    BCT_CLIENT_NutsSlowQInit( &p_wk->slow );

    // �����Ă���؂̎��`��OFF
    BCT_CLIENT_HandNutsDrawEnd( &p_wk->graphic.handnuts );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�ύX�V�X�e��	������
 *
 *	@param	p_wk
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ContGameData_Init( BCT_CLIENT_CONT_GAMEDATA* p_wk, const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype )
{
	p_wk->count = BCT_CONT_GAMEDATA_COUNT_MAX;

	// �����f�[�^��ݒ�
	BCT_CLIENT_GameData_GetGameTypeOneData( cp_gamedata, cp_gametype, 0, &p_wk->set );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�ύX�V�X�e��	�ύX�����J�n
 *
 *	@param	p_wk			���[�N
 *	@param	cp_gamedata		�Q�[���f�[�^
 *	@param	cp_gametype		�Q�[���^�C�v�f�[�^
 *	@param	last			�O�̃f�[�^�C���f�b�N�X
 *	@param	next			���̃f�[�^�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ContGameData_Start( BCT_CLIENT_CONT_GAMEDATA* p_wk, const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype, u16 last, u16 next )
{
	BCT_GAMEDATA_ONE	last_one, next_one;

	
	// �Q�[���^�C�v�̔��f
	BCT_CLIENT_GameData_GetGameTypeOneData( cp_gamedata, cp_gametype, last, &last_one );
	BCT_CLIENT_GameData_GetGameTypeOneData( cp_gamedata, cp_gametype, next, &next_one );
	
	// �J�n�l
	p_wk->last				= last_one;
	p_wk->set				= last_one;

	// �ϓ��l�擾
	p_wk->dif.nuts_num		= next_one.nuts_num - last_one.nuts_num;
	p_wk->dif.camera_ta_y	= next_one.camera_ta_y - last_one.camera_ta_y;
	p_wk->dif.pos_z			= next_one.pos_z - last_one.pos_z;
	p_wk->dif.size			= next_one.size - last_one.size;
	
	// �A���O���A��]�X�s�[�h�͍������������������ق���I��
	if( (next_one.rota_speed - last_one.rota_speed) < ((0xffff + next_one.rota_speed) - last_one.rota_speed) ){	
		p_wk->dif.rota_speed	= next_one.rota_speed - last_one.rota_speed;
	}else{
        p_wk->dif.rota_speed	= ((0xffff + next_one.rota_speed) - last_one.rota_speed);
	}
	if( (next_one.camera_an_x - last_one.camera_an_x) < ((0xffff + next_one.camera_an_x) - last_one.camera_an_x) ){	
		p_wk->dif.camera_an_x	= next_one.camera_an_x - last_one.camera_an_x;
	}else{
        p_wk->dif.camera_an_x	= ((0xffff + next_one.camera_an_x) - last_one.camera_an_x);
	}

	p_wk->count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�ύX�V�X�e��	���C������
 *		
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�ύX�I��
 *	@retval	FALSE	�ύX�r��
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_ContGameData_Main( BCT_CLIENT_CONT_GAMEDATA* p_wk )
{
	if( p_wk->count >= BCT_CONT_GAMEDATA_COUNT_MAX ){
		return TRUE;
	}

	// �v�Z����
	p_wk->count ++;

	p_wk->set.nuts_num		= p_wk->last.nuts_num + ( (p_wk->dif.nuts_num * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	p_wk->set.rota_speed	= p_wk->last.rota_speed + ( (p_wk->dif.rota_speed * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	p_wk->set.camera_ta_y	= p_wk->last.camera_ta_y + ( (p_wk->dif.camera_ta_y * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	p_wk->set.camera_an_x	= p_wk->last.camera_an_x + ( (p_wk->dif.camera_an_x * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	p_wk->set.pos_z			= p_wk->last.pos_z + ( (p_wk->dif.pos_z * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	p_wk->set.size			= p_wk->last.size + ( (p_wk->dif.size * p_wk->count) / BCT_CONT_GAMEDATA_COUNT_MAX );
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�ύX�V�X�e��	�f�[�^���f
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_ContGameData_Reflect( BCT_CLIENT* p_wk, const BCT_CLIENT_CONT_GAMEDATA* cp_data )
{
	BCT_CLIENT_GameDataOne_Reflect( p_wk, &cp_data->set );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�̂P��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GameDataOne_Reflect( BCT_CLIENT* p_wk, const BCT_GAMEDATA_ONE* cp_data )
{
	// �ݒ�f�[�^�\��
#if 0
	OS_TPrintf( "speed %d\n", cp_data->rota_speed );
	OS_TPrintf( "ta_y %d\n", cp_data->camera_ta_y );
	OS_TPrintf( "an_x %d\n", cp_data->camera_an_x );
	OS_TPrintf( "pos_z %d\n", cp_data->pos_z );
	OS_TPrintf( "size %d\n", cp_data->size );
#endif
	
	//�@��]�X�s�[�h
	BCT_CLIENT_MaruRotSpeedSet( &p_wk->marunomu, cp_data->rota_speed );

	// �J�����^�[�Q�b�gY���W
	BCT_CLIENT_CameraTargetYSet( &p_wk->graphic, cp_data->camera_ta_y );

	// �J�����A���O��X
	BCT_CLIENT_CameraAngleXSet( &p_wk->graphic, cp_data->camera_an_x );

	// ���ʈʒu
	BCT_CLIENT_MaruMatZSet( &p_wk->marunomu, cp_data->pos_z );

	// �X�P�[��
	BCT_CLIENT_MaruScaleSet( &p_wk->marunomu, cp_data->size );
}

//----------------------------------------------------------------------------
/**
 *	@brief	gamedata����gametype�ɂ��킹���Q�[���\���P�f�[�^���擾����
 *
 *	@param	cp_gamedata		�Q�[���f�[�^
 *	@param	cp_gametype		�Q�[���^�C�v
 *	@param	idx				�C���f�b�N�X
 *	@param	p_buff			�f�[�^�i�[��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GameData_GetGameTypeOneData( const BCT_GAMEDATA* cp_gamedata, const BCT_GAME_TYPE_WK* cp_gametype, u32 idx, BCT_GAMEDATA_ONE* p_buff )
{
	u16 rota;
	s32 scale_tbl;
	
	GF_ASSERT( idx < cp_gamedata->tblnum );
	
	*p_buff = cp_gamedata->p_tbl[ idx ];

	// ��]�n�p�����[�^��u16���œ����l�ɕύX
	rota				= p_buff->rota_speed;
	p_buff->rota_speed	= rota;

	rota				= p_buff->camera_an_x;
	p_buff->camera_an_x	= rota;


	// �g�嗦�̃e�[�u���t�Q��
	if( cp_gametype->scale_rev ){
		scale_tbl = BCT_CONT_GAMEDATA_RAREGAME_BIGSTART - (idx+1);
		if( scale_tbl < 0 ){
			scale_tbl = 0;
		}
		p_buff->size	= cp_gamedata->p_tbl[ scale_tbl ].size;
//		OS_TPrintf( "cp_gamedata->tblnum=%d  size=%d\n", cp_gamedata->tblnum, p_buff->size );
	}

	// ��]�X�s�[�h�̔��]
	if( cp_gametype->rota_rev ){
		p_buff->rota_speed	= -p_buff->rota_speed;
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�y��	������	
 *
 *	@param	p_wk			���[�N
 *	@param	p_graphic		�O���t�B�b�N
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_TOUCHPEN_Init( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, u32 heapID )
{
	ARCHANDLE* p_handle;
	BOOL result;

	memset( p_wk, 0, sizeof(BCT_CLIENT_TOUCHPEN_MOVE) );

	p_handle = ArchiveDataHandleOpen( ARC_WLMNGM_TOOL_GRA, heapID );
	

	// �O���t�B�b�N�̓ǂݍ���
	{
        // OAM���\�[�X�Ǎ���
        p_wk->resobj[ 0 ] = CLACT_U_ResManagerResAddArcChar_ArcHandle(
                    p_graphic->resMan[ 0 ], p_handle,
                    NARC_wlmngm_tool_touchpen_NCGR,
                    FALSE, BCT_TOUCHPEN_OAM_RESID, NNS_G2D_VRAM_TYPE_2DSUB, heapID );

        p_wk->resobj[ 1 ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle(
                    p_graphic->resMan[ 1 ], p_handle,
                    NARC_wlmngm_tool_touchpen_NCLR,
                    FALSE, BCT_TOUCHPEN_OAM_RESID, NNS_G2D_VRAM_TYPE_2DSUB, 2, heapID );

        p_wk->resobj[ 2 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_graphic->resMan[ 2 ], p_handle,
                NARC_wlmngm_tool_touchpen_NCER, FALSE,
                BCT_TOUCHPEN_OAM_RESID, CLACT_U_CELL_RES, heapID );

        p_wk->resobj[ 3 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_graphic->resMan[ 3 ], p_handle,
                NARC_wlmngm_tool_touchpen_NANR, FALSE,
                BCT_TOUCHPEN_OAM_RESID, CLACT_U_CELLANM_RES, heapID );

        // �]��
        result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_wk->resobj[ 0 ] );
        GF_ASSERT( result );
        result = CLACT_U_PlttManagerSetCleanArea( p_wk->resobj[ 1 ] );
        GF_ASSERT( result );

        // ���\�[�X�����j��
        CLACT_U_ResManagerResOnlyDelete( p_wk->resobj[ 0 ] );
        CLACT_U_ResManagerResOnlyDelete( p_wk->resobj[ 1 ] );

		// �Z���A�N�^�[�w�b�_�[�쐬
		CLACT_U_MakeHeader( &p_wk->header, 
				BCT_TOUCHPEN_OAM_RESID, BCT_TOUCHPEN_OAM_RESID,
				BCT_TOUCHPEN_OAM_RESID, BCT_TOUCHPEN_OAM_RESID,
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,
				BCT_TOUCHPEN_OAM_BGPRI,
				p_graphic->resMan[0], p_graphic->resMan[1], 
				p_graphic->resMan[2], p_graphic->resMan[3],
				NULL, NULL );

	}

	// �^�b�`�y���A�N�^�[�o�^
	{
		CLACT_ADD_SIMPLE add;

		add.ClActSet		= p_graphic->clactSet;
		add.ClActHeader		= &p_wk->header;
		add.mat.x			= FX32_CONST( BCT_TOUCHPEN_OAM_X );
		add.mat.y			= FX32_CONST( BCT_TOUCHPEN_OAM_Y ) + BCT_GRA_OAMSUBSURFACE_Y;
		add.pri				= BCT_TOUCHPEN_OAM_SOFPRI;
		add.DrawArea		= NNS_G2D_VRAM_TYPE_2DSUB;
		add.heap			= heapID;

		p_wk->p_clwk = CLACT_AddSimple( &add );
		CLACT_SetDrawFlag( p_wk->p_clwk, FALSE );
	}

    ArchiveDataHandleClose( p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�y��	�j��
 *
 *	@param	p_wk			���[�N
 *	@param	p_graphic		�O���t�B�b�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_TOUCHPEN_Exit( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT_GRAPHIC* p_graphic )
{
	// �A�N�^�[���[�N�̔j��
	{
		CLACT_Delete( p_wk->p_clwk );
	}

	// ���\�[�X�j��
	{
        // VRAM�Ǘ�����j��
        CLACT_U_CharManagerDelete( p_wk->resobj[0] );
        CLACT_U_PlttManagerDelete( p_wk->resobj[1] );
        
        // ���\�[�X�j��
        CLACT_U_ResManagerResDelete( p_graphic->resMan[0], p_wk->resobj[0] );
        CLACT_U_ResManagerResDelete( p_graphic->resMan[1], p_wk->resobj[1] );
        CLACT_U_ResManagerResDelete( p_graphic->resMan[2], p_wk->resobj[2] );
        CLACT_U_ResManagerResDelete( p_graphic->resMan[3], p_wk->resobj[3] );
	}

	//�N���A
	memset( p_wk, 0, sizeof(BCT_CLIENT_TOUCHPEN_MOVE) );
}
 
//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�y��	�J�n
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_TOUCHPEN_Start( BCT_CLIENT_TOUCHPEN_MOVE* p_wk )
{
	//  ����J�n
	{
		p_wk->move		= TRUE;
		p_wk->seq		= 0;
		p_wk->roop		= 0;
	}

	// �`��J�n
	CLACT_SetDrawFlag( p_wk->p_clwk, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�b�`�y��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�N���C�A���g�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_TOUCHPEN_Main( BCT_CLIENT_TOUCHPEN_MOVE* p_wk, BCT_CLIENT* p_sys )
{
	switch( p_wk->seq ){
	// ����J�n
	case BCT_TOUCHPEN_ANM_SEQ_MOVE_START:
		// �^�b�`�y���̓������Ȃ����Ď����Ă���؂̎���`�悷��
		CLACT_AnmFrameSet( p_wk->p_clwk, BCT_TOUCHPEN_ANM_FRAME_START );

		// �A�j���t���[���ɂ��킹�Ď����Ă���؂̎����o��
		BCT_CLIENT_HandNutsDrawStart( &p_sys->graphic.handnuts );
		BCT_CLIENT_HandNutsDrawSetMatrix( &p_sys->graphic.handnuts, 
				BCT_TOUCHPEN_OAM_X,
				sc_BCT_TOUCHPEN_ANM_FRAME_Y[ BCT_TOUCHPEN_ANM_FRAME_START ] );

		// �؂̎����ˑO
		p_wk->nut_set = FALSE;
		p_wk->seq++;
		break;

	// ���C��
	case BCT_TOUCHPEN_ANM_SEQ_MOVE_MAIN:
		CLACT_AnmFrameChg( p_wk->p_clwk, FX32_CONST(2) );

		// �A�j���t���[���ɂ��킹�Ď����Ă���؂̎����o��
		{
			u32 frame;

			frame = CLACT_AnmFrameGet( p_wk->p_clwk );

			// ����`�F�b�N
			switch( frame ){
			// �����Ŕ��˂�����
			case BCT_TOUCHPEN_ANM_FRAME_FADEOUT:
				if( p_wk->nut_set == FALSE ){
					BCT_EasyNutsSet( p_sys, 128, 96, 128, 64, BCT_NUTSSEQ_MOVEOTHER );
					Snd_SePlay( BCT_SND_SLOW );
					p_wk->nut_set = TRUE;
				}
				break;
				
			// ������
			case BCT_TOUCHPEN_ANM_FRAME_END:
				p_wk->roop ++ ;
				if( p_wk->roop >= BCT_TOUCHPEN_ANM_ROOP_NUM ){
					// �I����
					p_wk->seq = BCT_TOUCHPEN_ANM_SEQ_END_WAIT;
				}else{
					// �������
					p_wk->seq	= BCT_TOUCHPEN_ANM_SEQ_MOVE_WAIT;
					p_wk->wait	= BCT_TOUCHPEN_WAIT;
				}
				BCT_CLIENT_HandNutsDrawEnd( &p_sys->graphic.handnuts );
				break;

			default:
				break;
			}
			// �؂̎����W��ݒ�
			BCT_CLIENT_HandNutsDrawSetMatrix( &p_sys->graphic.handnuts, 
					BCT_TOUCHPEN_OAM_X,
					sc_BCT_TOUCHPEN_ANM_FRAME_Y[ frame ] );
		}
		break;

	// ���̊J�n�܂ł̃E�G�C�g
	case BCT_TOUCHPEN_ANM_SEQ_MOVE_WAIT:		
		p_wk->wait--;
		if( p_wk->wait <= 0 ){
			p_wk->seq = BCT_TOUCHPEN_ANM_SEQ_MOVE_START;
		}
		break;

	// �I���҂�
	case BCT_TOUCHPEN_ANM_SEQ_END_WAIT:
		if( BCT_CLIENT_NutsMoveCheck( p_sys ) == FALSE ){

			// �`��J�n
			CLACT_SetDrawFlag( p_wk->p_clwk, FALSE );
			return TRUE;
		}
		break;
	}

	// �؂̎����쏈��
	BCT_CLIENT_AllNutsMove( p_sys );

	// �؂̎��̏I���Ǘ�
    BCT_CLIENT_AllNutsEndCont( p_sys );

	return FALSE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�r���o��	�X�R�A	������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MDLSCR_Init( BCT_CLIENT_MIDDLE_SCORE* p_wk )
{
	memset( p_wk, 0, sizeof(BCT_CLIENT_MIDDLE_SCORE) );
	p_wk->top_idx = BCT_MDLSCR_TOP_DEF;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o��	�X�R�A	�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	score		���_
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MDLSCR_Set( BCT_CLIENT_MIDDLE_SCORE* p_wk, u32 score, u32 idx )
{
	p_wk->score[ idx ]	= score;
	p_wk->score_msk		|= 1<<idx; 

	if( score == 0 ){
		return ;
	}
	
	
	if( p_wk->top_idx != BCT_MDLSCR_TOP_DEF ){

		// 1�ʂƓ��_���r
		// 1�ʂ͑������񂪂��ɂ���
		// score��1�ȏ�
		if( p_wk->score[ p_wk->top_idx ] < score ){
			p_wk->top_idx = idx;
		}
	}else{

		//  �܂��N�̓_�������ĂȂ��̂ł��̐l�����
		p_wk->top_idx = idx;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o��	�r���o�ߍĎ擾�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MDLSCR_Reset( BCT_CLIENT_MIDDLE_SCORE* p_wk )
{
	p_wk->score_msk = 0;
	p_wk->top_idx	= BCT_MDLSCR_TOP_DEF;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o��	���̂Ƃ���̂P�ʂ�Ԃ�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	BCT_MDLSCR_TOP_DEF	���Ȃ�
 *	@retval	���̈ʒu�̐l�̃C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static u32 BCT_CLIENT_MDLSCR_GetTop( const BCT_CLIENT_MIDDLE_SCORE* cp_wk )
{
	return cp_wk->top_idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l�����̓��_���擾���Ă邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	num			��
 *
 *	@retval	TRUE	�S���̓_���擾����
 *	@retval	FALSE	�܂��S�����͎����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_MDLSCR_CheckInNum( const BCT_CLIENT_MIDDLE_SCORE* cp_wk, u32 num )
{
	int i;
	int in_num;
	
	in_num = 0;
	for( i=0; i<BCT_PLAYER_NUM; i++ ){
		if( (cp_wk->score_msk & (1<<i)) != 0 ){
			in_num ++;
		}
	}
	if( num <= in_num ){
		return TRUE;
	}
	return FALSE;
}



//----------------------------------------------------------------------------
/**
 *  @brief  �o���N�ݒ�
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BankSet( void )
{
    GF_BGL_DISPVRAM vramSetTable = {
        GX_VRAM_BG_32_FG,               // ���C��2D�G���W����BG
        GX_VRAM_BGEXTPLTT_NONE,         // ���C��2D�G���W����BG�g���p���b�g
        GX_VRAM_SUB_BG_128_C,           // �T�u2D�G���W����BG
        GX_VRAM_SUB_BGEXTPLTT_NONE,     // �T�u2D�G���W����BG�g���p���b�g
        GX_VRAM_OBJ_128_B,              // ���C��2D�G���W����OBJ
        GX_VRAM_OBJEXTPLTT_NONE,        // ���C��2D�G���W����OBJ�g���p���b�g
        GX_VRAM_SUB_OBJ_16_I,           // �T�u2D�G���W����OBJ
        GX_VRAM_SUB_OBJEXTPLTT_NONE,    // �T�u2D�G���W����OBJ�g���p���b�g
        GX_VRAM_TEX_0_A,                // �e�N�X�`���C���[�W�X���b�g
        GX_VRAM_TEXPLTT_0123_E          // �e�N�X�`���p���b�g�X���b�g
    };
    GF_Disp_SetBank( &vramSetTable );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�Q�[���̂RD�Z�b�g�A�b�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_3DSetUp( void )
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(2);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��
	G3X_EdgeMarking( TRUE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(26,26,26),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog
	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);
}

//----------------------------------------------------------------------------
/**
 *  @brief  �RD������
 *
 *  @param  p_wk        ���[�N
 *  @param  comm_num    �ʐM�l��
 *  @param	plno		�ʐMID
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_3DInit( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num, u32 plno, u32 heapID )
{
	p_wk->p_3dman = GF_G3DMAN_Init( heapID, GF_G3DMAN_LNK, GF_G3DTEX_128K, 
			GF_G3DMAN_LNK, GF_G3DPLT_64K, BCT_CLIENT_3DSetUp );
	
    // �J�����ݒ�
    BCT_CLIENT_CameraInit( p_wk, comm_num, plno, heapID );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �RD�j��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_3DExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    // �J�����j��
    BCT_CLIENT_CameraExit( p_wk );
    
    GF_G3D_Exit( p_wk->p_3dman );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �O���t�B�b�N�f�[�^������
 *
 *  @param  p_wk        ���[�N
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GraphicInit( BCT_CLIENT* p_wk, u32 heapID )
{
    ARCHANDLE* p_handle;

    // �A���P�[�^�쐬
    sys_InitAllocator( &p_wk->graphic.allocator, heapID, 32 );
    
    // �o���N�ݒ�
    BCT_CLIENT_BankSet();

    // �RD������
    BCT_CLIENT_3DInit( &p_wk->graphic, p_wk->comm_num, p_wk->plno, heapID );

    // BG������
    BCT_CLIENT_BgInit( &p_wk->graphic, heapID );

    // OAM������
    BCT_CLIENT_OamInit( &p_wk->graphic, heapID );

    // MSG������
    BCT_CLIENT_MsgInit( &p_wk->graphic, heapID );

    // �A�[�J�C�u�n���h���I�[�v��
    p_handle = ArchiveDataHandleOpen( ARC_BUCKET_GRAPHIC, heapID );

    // ���C����OAM���\�[�X������
    BCT_CLIENT_MainOamInit( &p_wk->graphic, p_handle, heapID );

	// ���C���ʔw�i������
	BCT_CLIENT_MainBackInit( &p_wk->graphic.mainback, &p_wk->graphic, p_handle, p_wk->comm_num, p_wk->plno, heapID, &p_wk->graphic.allocator );

    // �}���m�[��������
    BCT_CLIENT_MarunomuDrawInit( &p_wk->graphic.marunomu, p_handle, heapID, &p_wk->graphic.allocator );

    // �؂̎�������
    BCT_CLIENT_NutsDrawSysInit( &p_wk->graphic, p_handle, heapID );

    // �����Ă���؂̎�������
    BCT_CLIENT_HandNutsDrawInit( &p_wk->graphic, &p_wk->graphic.handnuts, p_wk->plno, heapID );

	// OAM�؂̎���ʊO�ړ��G�t�F�N�g������
	BCT_CLIENT_OamAwayNutsInit( &p_wk->graphic, &p_wk->graphic.oamnutsaway, p_wk->plno, heapID );

    // BG�ʂ̕`��
    BCT_CLIENT_BgResLoad( &p_wk->graphic, p_handle, p_wk->plno, heapID );

    // �J�n��ʃ��[�N������
    BCT_CLIENT_StartSysInit( &p_wk->graphic.start, &p_wk->graphic, p_wk->cp_gamedata, p_wk->comm_num, p_wk->plno, p_handle, heapID );

    // �I�����[�N������
    BCT_CLIENT_EndSysInit( &p_wk->graphic, p_wk->comm_num, p_wk->plno, heapID );

    // �X�R�A�G�t�F�N�g������
    BCT_CLIENT_ScoreEffectInit( &p_wk->graphic.score_effect, &p_wk->graphic, p_wk->comm_num, p_wk->plno, heapID );

	// BG�D�揇�ʃX�N���[���̏�����
	BCT_CLIENT_BGPRISCRL_Init( &p_wk->graphic.bgpri_scrl, p_wk->plno );	

	// �؂̎��J�E���^�[
	BCT_CLIENT_NUTS_COUNT_Init( &p_wk->graphic.nutscount, &p_wk->graphic, p_handle, heapID );

#ifdef BCT_DEBUG_HITZONE
    BCT_DEBUG_PositionInit( p_handle, heapID );
#endif
    
    ArchiveDataHandleClose( p_handle );


	// �J�E���g���[�N������
	p_wk->graphic.p_countwk = MNGM_COUNT_Init( p_wk->graphic.clactSet, heapID );

}

//----------------------------------------------------------------------------
/**
 *  @brief  �O���t�B�b�N�f�[�^�j��
 *
 *  @param  p_wk        ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GraphicDelete( BCT_CLIENT* p_wk )
{

#ifdef BCT_DEBUG_HITZONE
    BCT_DEBUG_PositionExit();
#endif

	// �؂̎��J�E���^�[
	BCT_CLIENT_NUTS_COUNT_Exit( &p_wk->graphic.nutscount, &p_wk->graphic );

    // �X�R�A�G�t�F�N�g�j��
    BCT_CLIENT_ScoreEffectExit( &p_wk->graphic.score_effect, &p_wk->graphic );

    // �I�����[�N�j��
    BCT_CLIENT_EndSysExit( &p_wk->graphic, p_wk->comm_num );

    // �J�n��ʃ��[�N�j��
    BCT_CLIENT_StartSysExit( &p_wk->graphic.start, &p_wk->graphic );
    
    // BG�ʔj��
    BCT_CLIENT_BgResRelease( &p_wk->graphic );

	// OAM�؂̎���ʊO�ړ��G�t�F�N�g�j��
	BCT_CLIENT_OamAwayNutsExit( &p_wk->graphic, &p_wk->graphic.oamnutsaway );

    // �����Ă���؂̎��j��
    BCT_CLIENT_HandNutsDrawExit( &p_wk->graphic.handnuts );

    // �؂̎��j��
    BCT_CLIENT_NutsDrawSysExit( &p_wk->graphic );
    
    // �}���m�[���j��
    BCT_CLIENT_MarunomuDrawExit( &p_wk->graphic.marunomu, &p_wk->graphic.allocator );

	// ���C���ʔw�i�j��
	BCT_CLIENT_MainBackExit( &p_wk->graphic.mainback, &p_wk->graphic, &p_wk->graphic.allocator );

    // ���C����OAM���\�[�X�j��
    BCT_CLIENT_MainOamExit( &p_wk->graphic );

	// �J�E���g���[�N�j��
	MNGM_COUNT_Exit( p_wk->graphic.p_countwk );
    
    // BG�j��
    BCT_CLIENT_BgExit( &p_wk->graphic );

    // OAM�j��
    BCT_CLIENT_OamExit( &p_wk->graphic );

    // ���b�Z�[�W�j��
    BCT_CLIENT_MsgExit( &p_wk->graphic );

    // �RD�j��
    BCT_CLIENT_3DExit( &p_wk->graphic );

	// �u���C�g�l�X�j��
	G2_BlendNone();
	G2S_BlendNone();
}

//----------------------------------------------------------------------------
/**
 *  @brief  �`�惁�C��
 *
 *  @param  cp_wk   �f�[�^���[�N
 *  @param  p_wk    �`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GraphicMain( const BCT_CLIENT* cp_wk, BCT_CLIENT_GRAPHIC* p_wk )
{
    // �}���m�[���`��f�[�^�X�V
    BCT_CLIENT_MarunomuDrawMain( &p_wk->marunomu, &cp_wk->marunomu, cp_wk->move_type );
	BCT_CLIENT_MarunomuDrawAnmMain( &p_wk->marunomu, &cp_wk->marunomu, cp_wk->move_type );

    // �����Ă���؂̎��`�惁�C��
    BCT_CLIENT_HandNutsDrawMain( &p_wk->handnuts );

	// �\��
	BCT_CLIENT_GraphicDrawCore( cp_wk, p_wk );

	// BG�D�揇�ʃX�N���[�����C��
	BCT_CLIENT_BGPRISCRL_Main( &p_wk->bgpri_scrl, p_wk );

	// �؂̎��J�E���^�[
	BCT_CLIENT_NUTS_COUNT_Main( &p_wk->nutscount );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ۂɕ\���̂ݍs���ꏊ
 *
 *	@param	cp_wk		���[�N
 *	@param	p_wk		�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_GraphicDrawCore( const BCT_CLIENT* cp_wk, BCT_CLIENT_GRAPHIC* p_wk )
{

	// OAM�؂̎���ʊO�G�t�F�N�g�`�惁�C��
	BCT_CLIENT_OamAwayNutsMain( &p_wk->oamnutsaway );
	
	// �X�R�A�G�t�F�N�g���C��
	BCT_CLIENT_ScoreEffectMain( &p_wk->score_effect );
	
    //�R�c�`��J�n
    GF_G3X_Reset();

    // �J�����ݒ�
    BCT_CLIENT_CameraMain( p_wk );

    // ���C�g�ƃA���r�G���g
    NNS_G3dGlbLightVector( 0, 0, -FX32_ONE, 0 );
    NNS_G3dGlbLightColor( 0, GX_RGB( 31,31,31 ) );
    NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
    NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );

    // �`��
    NNS_G3dGePushMtx();
    {
		// ���C���ʔw�i
		BCT_CLIENT_MainBackDraw( &p_wk->mainback );
    }
    NNS_G3dGePopMtx(1);
	
    NNS_G3dGePushMtx();
    {
        // �}���m�[���`��
        BCT_CLIENT_MarunomuDrawDraw( &p_wk->marunomu, &cp_wk->marunomu );
    }
    NNS_G3dGePopMtx(1);

    NNS_G3dGePushMtx();
    {
        // �؂̎��`��
        BCT_CLIENT_NutsDrawSysMain( p_wk, cp_wk->comm_num );
    }
    NNS_G3dGePopMtx(1);

#ifdef BCT_DEBUG_HITZONE
    NNS_G3dGePushMtx();
    {
        BCT_DEBUG_PositionDraw( cp_wk );
    }
    NNS_G3dGePopMtx(1);
#endif
    

    /* �W�I���g���������_�����O�G���W���֘A�������̃X���b�v */
    GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);

    // �Z���A�N�^�[�`��
    CLACT_Draw( p_wk->clactSet );
}

//----------------------------------------------------------------------------
/**
 *  @brief  BG�ʂ̏�����
 *
 *  @param  p_wk    ���[�N
 *  @param  heapID  �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BgInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID )
{
    p_wk->p_bgl = GF_BGL_BglIniAlloc( heapID );

    // BG SYSTEM
    {
        GF_BGL_SYS_HEADER BGsys_data = {
            GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D,
        };
        GF_BGL_InitBG( &BGsys_data );
    }

    // ���C�����1
    {   // �E�B���h�E
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            0, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME1_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME1_M, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME1_M );
    }

    // ���C�����2
    {   // �E�B���h�E
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
            1, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME2_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME2_M, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME2_M );
    }

    // �T�u���0
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            0, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME0_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME0_S, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME0_S );
    }

    // �T�u���1   
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            1, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME1_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME1_S, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME1_S );
    }

	// �T�u���2	
	{
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            2, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME2_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME2_S, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME2_S );
	}

	// �T�u���3	
	{
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            3, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( p_wk->p_bgl, GF_BGL_FRAME3_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME3_S, 32, 0, heapID);
        GF_BGL_ScrClear( p_wk->p_bgl, GF_BGL_FRAME3_S );
	}


    // ���C����ʐݒ�
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );

    // �T�u�ʂ͕\��OFF
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );

    // ���C���ʂɃE�B���h�E�O���t�B�b�N��ݒ�
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, BCT_GRA_BGMAIN_PAL_FONT*0x20, heapID );
    MenuWinGraphicSet(
        p_wk->p_bgl, GF_BGL_FRAME1_M, BCT_GRA_SYSWND_CGX, BCT_GRA_BGMAIN_PAL_SYSWND, 0, heapID );


}

//----------------------------------------------------------------------------
/**
 *  @brief  BG�ʂ̔j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BgExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME1_M );
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME2_M );
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME0_S );
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME1_S );
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME2_S );
    GF_BGL_BGControlExit( p_wk->p_bgl, GF_BGL_FRAME3_S );

    sys_FreeMemoryEz( p_wk->p_bgl );
}

//----------------------------------------------------------------------------
/**
 *  @brief  OAM�ʂ̏�����
 *
 *  @param  p_wk        ���[�N
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID )
{
    int i;

    // OAM�}�l�[�W���[�̏�����
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 126,     // ���C�����OAM�Ǘ��̈�
        0, 31,      // ���C����ʃA�t�B���Ǘ��̈�
        0, 126,     // �T�u���OAM�Ǘ��̈�
        0, 31,      // �T�u��ʃA�t�B���Ǘ��̈�
        heapID);


    // �L�����N�^�}�l�[�W���[������
    {
        CHAR_MANAGER_MAKE cm = {
            BCT_GRA_CHARMAN_NUM,
            BCT_GRA_OAMVRAM_MAIN,
            BCT_GRA_OAMVRAM_SUB,
            0
        };
        cm.heap = heapID;
        InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K );
    }
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(BCT_GRA_PLTTMAN_NUM, heapID);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();

    //�ʐM�A�C�R���p�ɃL�������p���b�g����
    CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
    

    // �Z���A�N�^�[�Z�b�g�쐬
    p_wk->clactSet = CLACT_U_SetEasyInit( BCT_GRA_CLACTNUM, &p_wk->renddata, heapID );

    // �T�[�t�F�[�X�ʒu���ړ�������
    CLACT_U_SetSubSurfaceMatrix( &p_wk->renddata, 0, BCT_GRA_OAMSUBSURFACE_Y );
    
    // �L�����ƃp���b�g�̃��\�[�X�}�l�[�W���쐬
    for( i=0; i<BCT_GRA_RESMAN_NUM; i++ ){
        p_wk->resMan[i] = CLACT_U_ResManagerInit(BCT_GRA_RESMAN_LOADNUM, i, heapID);
    }


	// �t�H���gOAM
	p_wk->p_fontoam_sys = FONTOAM_SysInit( BCT_FONTOAM_WKNUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *  @brief  OAM�ʂ̔j��
 *
 *  @param  p_wk        ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    int i;

	// �t�H���gOAM
	FONTOAM_SysDelete( p_wk->p_fontoam_sys );

    // �A�N�^�[�̔j��
    CLACT_DestSet( p_wk->clactSet );


    for( i=0; i<BCT_GRA_RESMAN_NUM; i++ ){
        CLACT_U_ResManagerDelete( p_wk->resMan[i] );
    }

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    //OAM�����_���[�j��
    REND_OAM_Delete();
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���C����OAM���\�[�X ������
 *
 *  @param  p_wk        ���[�N
 *  @param  p_handle    �A�[�N�n���h��
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainOamInit( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 heapID )
{
    BOOL result;
    // ���C�����OAM���\�[�X�ǂݍ���
    {
        // OAM���\�[�X�Ǎ���
        p_wk->mainoamresobj[ 0 ] = CLACT_U_ResManagerResAddArcChar_ArcHandle(
                    p_wk->resMan[ 0 ], p_handle,
                    NARC_bucket_font_boad_NCGR,
                    FALSE, BCT_MAINOAM_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

        p_wk->mainoamresobj[ 1 ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle(
                    p_wk->resMan[ 1 ], p_handle,
                    NARC_bucket_font_boad_NCLR,
                    FALSE, BCT_MAINOAM_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, 
					BCT_GRA_OAMMAIN_PAL_NUM, heapID );

        p_wk->mainoamresobj[ 2 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_wk->resMan[ 2 ], p_handle,
                NARC_bucket_font_boad_NCER, FALSE,
                BCT_MAINOAM_CONTID, CLACT_U_CELL_RES, heapID );

        p_wk->mainoamresobj[ 3 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_wk->resMan[ 3 ], p_handle,
                NARC_bucket_font_boad_NANR, FALSE,
                BCT_MAINOAM_CONTID, CLACT_U_CELLANM_RES, heapID );

        // �]��
        result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_wk->mainoamresobj[ 0 ] );
        GF_ASSERT( result );
        result = CLACT_U_PlttManagerSetCleanArea( p_wk->mainoamresobj[ 1 ] );
        GF_ASSERT( result );

        // ���\�[�X�����j��
        CLACT_U_ResManagerResOnlyDelete( p_wk->mainoamresobj[ 0 ] );
        CLACT_U_ResManagerResOnlyDelete( p_wk->mainoamresobj[ 1 ] );

        // �Z���A�N�^�[�w�b�_�[�쐬
        CLACT_U_MakeHeader( &p_wk->mainoamheader, 
                BCT_MAINOAM_CONTID, BCT_MAINOAM_CONTID,
                BCT_MAINOAM_CONTID, BCT_MAINOAM_CONTID,
                CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,
                BCT_MAINOAM_BGPRI,
                p_wk->resMan[0], p_wk->resMan[1], 
                p_wk->resMan[2], p_wk->resMan[3],
                NULL, NULL );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���C���ʁ@OAM���\�[�X�j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainOamExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    // ���C�����OAM���\�[�X�j��
    {
        // VRAM�Ǘ�����j��
        CLACT_U_CharManagerDelete( p_wk->mainoamresobj[0] );
        CLACT_U_PlttManagerDelete( p_wk->mainoamresobj[1] );
        
        // ���\�[�X�j��
        CLACT_U_ResManagerResDelete( p_wk->resMan[0], p_wk->mainoamresobj[0] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[1], p_wk->mainoamresobj[1] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[2], p_wk->mainoamresobj[2] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[3], p_wk->mainoamresobj[3] );
    }
    
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���b�Z�[�W�f�[�^������
 *
 *  @param  p_wk        ���[�N
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MsgInit( BCT_CLIENT_GRAPHIC* p_wk, u32 heapID )
{
    p_wk->p_wordset = WORDSET_Create( heapID );
    p_wk->p_msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_lobby_minigame1_dat, heapID );

    p_wk->p_msgstr = STRBUF_Create( BCT_STRBUF_NUM, heapID );
    p_wk->p_msgtmp = STRBUF_Create( BCT_STRBUF_NUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *  @brief  ���b�Z�[�W�f�[�^�j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MsgExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    STRBUF_Delete( p_wk->p_msgtmp );
    STRBUF_Delete( p_wk->p_msgstr );
    MSGMAN_Delete( p_wk->p_msgman );
    WORDSET_Delete( p_wk->p_wordset );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J����������
 *
 *  @param  p_wk        ���[�N
 *  @param  comm_num    �ʐM�l��
 *  @param	plno		�ʎjID
 *  @param  heapID      �q�[�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CameraInit( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num, u32 plno, u32 heapID )
{
    CAMERA_ANGLE angle;
    VecFx32 up;
    MtxFx33 rot;
    
    // �J�����A���b�N
    p_wk->p_camera = GFC_AllocCamera( heapID );

    p_wk->target.x = BCT_CAMERA_TARGET_X;
    p_wk->target.y = BCT_CAMERA_TARGET_Y;
    p_wk->target.z = BCT_CAMERA_TARGET_Z;

    angle.x = BCT_CAMERA_ANGLE_X;
    angle.y = BCT_CAMERA_ANGLEY_NetID[comm_num-1][ plno ];
    angle.z = BCT_CAMERA_ANGLE_Z;

    GFC_InitCameraTDA( 
            &p_wk->target, BCT_CAMERA_DISTANCE,
            &angle, BCT_CAMERA_PEARCE,
            GF_CAMERA_PERSPECTIV,
            TRUE,   
            p_wk->p_camera
            );

    up.x = 0;
    up.y = BCT_CAMERA_UP;
    up.z = 0;
    GFC_SetCamUp( &up, p_wk->p_camera );
    
    GFC_AttachCamera( p_wk->p_camera );

	// Near Far�ݒ�
	GFC_SetCameraClip( BCT_CAMERA_NEAR, BCT_CAMERA_FAR, p_wk->p_camera );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�����j��
 *
 *  @param  p_wk        ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CameraExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    GFC_FreeCamera( p_wk->p_camera );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �J�������C��
 *
 *  @param  p_wk        ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CameraMain( BCT_CLIENT_GRAPHIC* p_wk )
{
    // �J�����ݒ�
    GFC_CameraLookAt();
}

//----------------------------------------------------------------------------
/**
 *  @brief  BG��ʂ̐ݒ�
 *
 *  @param  p_wk        ���[�N
 *  @param  p_handle    �n���h��
 *  @param	plno		�ʐMID
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BgResLoad( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 plno, u32 heapID )
{
	static const u8 sc_SubPal[ BCT_PLAYER_NUM ] = {
		BCT_GRA_BGSUB_PAL_BACK_NETID0,
		BCT_GRA_BGSUB_PAL_BACK_NETID1,
		BCT_GRA_BGSUB_PAL_BACK_NETID2,
		BCT_GRA_BGSUB_PAL_BACK_NETID3,
	};
	
    // �T�u��ʂ�BG
    ArcUtil_HDL_BgCharSet( p_handle, NARC_bucket_tamaire_bg_NCGR, p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );
    ArcUtil_HDL_ScrnSet( p_handle, NARC_bucket_tamaire_bg0_NSCR, p_wk->p_bgl,GF_BGL_FRAME3_S, 0, 0, FALSE, heapID );
    ArcUtil_HDL_ScrnSet( p_handle, NARC_bucket_tamaire_bg1_NSCR, p_wk->p_bgl,GF_BGL_FRAME2_S, 0, 0, FALSE, heapID );
    ArcUtil_HDL_ScrnSet( p_handle, NARC_bucket_tamaire_bg2_NSCR, p_wk->p_bgl,GF_BGL_FRAME1_S, 0, 0, FALSE, heapID );
    ArcUtil_HDL_ScrnSet( p_handle, NARC_bucket_tamaire_bg3_NSCR, p_wk->p_bgl,GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );
    ArcUtil_HDL_PalSet( p_handle, NARC_bucket_tamaire_bg_NCLR, PALTYPE_SUB_BG, 0, BCT_GRA_BGSUB_PAL_NUM*32, heapID );

	// �p���b�g�����킹��
	GF_BGL_ScrPalChange( p_wk->p_bgl, GF_BGL_FRAME3_S, 0, 0, 32, 32, sc_SubPal[plno] );	
	GF_BGL_ScrPalChange( p_wk->p_bgl, GF_BGL_FRAME2_S, 0, 0, 32, 32, BCT_GRA_BGSUB_PAL_NETID0_BACK+(plno*2) );	
	GF_BGL_ScrPalChange( p_wk->p_bgl, GF_BGL_FRAME1_S, 0, 0, 32, 32, BCT_GRA_BGSUB_PAL_NETID0_TOP+(plno*2) );	
	GF_BGL_ScrPalChange( p_wk->p_bgl, GF_BGL_FRAME0_S, 0, 0, 32, 32, BCT_GRA_BGSUB_PAL_NETID0_TOP+(plno*2) );

	GF_BGL_LoadScreenReq( p_wk->p_bgl, GF_BGL_FRAME0_S );
	GF_BGL_LoadScreenReq( p_wk->p_bgl, GF_BGL_FRAME1_S );
	GF_BGL_LoadScreenReq( p_wk->p_bgl, GF_BGL_FRAME2_S );
	GF_BGL_LoadScreenReq( p_wk->p_bgl, GF_BGL_FRAME3_S );
}

//----------------------------------------------------------------------------
/**
 *  @brief  BG��ʂ̔j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BgResRelease( BCT_CLIENT_GRAPHIC* p_wk )
{
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���`��I�u�W�F�N�g������
 *
 *  @param  p_wk        ���[�N
 *  @param  p_handle    �n���h��
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawInit( BCT_MARUNOMU_DRAW* p_wk, ARCHANDLE* p_handle, u32 heapID, NNSFndAllocator* p_allocator )
{
    static const u16 Anm[ BCT_MARUNOMU_ANM_NUM ] = {
        NARC_bucket_maru_robo_ani01_nsbca,
        NARC_bucket_maru_robo_ani02_nsbca,
        NARC_bucket_maru_robo_ani03_nsbca,
        NARC_bucket_maru_robo_ani04_nsbca,
        NARC_bucket_maru_robo_ani05_nsbca,
        NARC_bucket_maru_robo_ani06_nsbca,
    };
	static const u16 Mdl[ BCT_MARUNOMU_MDL_NUM ] = {
		NARC_bucket_maru_robo_col_nsbmd,
		NARC_bucket_maru_robo_ani05_nsbmd,
		NARC_bucket_maru_robo_ani04_nsbmd,
	};
    int i, j, k, idx;
	const NNSG3dResName* cp_node_name;
	const NNSG3dResNodeInfo* cp_nodeinfo;

	memset( p_wk, 0, sizeof(BCT_MARUNOMU_DRAW) );
    
    // ���f���ް��ǂݍ���
	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
	    D3DOBJ_MdlLoadH( &p_wk->mdl[i], p_handle, Mdl[i], heapID );

		// �����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &p_wk->obj[i], &p_wk->mdl[i] );

		// �`��OFF
		D3DOBJ_SetDraw( &p_wk->obj[i], FALSE );

		// ���W�A�g�嗦�̐ݒ�
	    D3DOBJ_SetMatrix( &p_wk->obj[i], sc_MARUNOMU_MAT.x, sc_MARUNOMU_MAT.y + BCT_START_SCRLL3D_Y_S, sc_MARUNOMU_MAT.z );
		D3DOBJ_SetScale( &p_wk->obj[i], BCT_MARUNOMU_SCALE, BCT_MARUNOMU_SCALE, BCT_MARUNOMU_SCALE );

	}
        

    
    // �A�j���ް��ǂݍ���
    for( i=0; i<BCT_MARUNOMU_ANM_NUM; i++ ){
        D3DOBJ_AnmLoadH( &p_wk->anm[i], &p_wk->mdl[ sc_BCT_MARUNOMU_ANM_MDL[i] ], p_handle, 
                Anm[i], heapID, p_allocator );
		// �A�j����Ԃ�ݒ�
		D3DOBJ_AnmSet( &p_wk->anm[i], 0 );

		// �A�j���̔��f�m�[�h�ݒ�
		// �܂��S�m�[�h�ւ̃A�j���̉e����OFF
		j = 0;
		cp_nodeinfo = NNS_G3dGetNodeInfo( p_wk->mdl[ sc_BCT_MARUNOMU_ANM_MDL[i] ].pModel );
		while( (cp_node_name = NNS_G3dGetNodeNameByIdx( cp_nodeinfo, j )) != NULL ){
			NNS_G3dAnmObjDisableID( p_wk->anm[i].pAnmObj, j );
			j++;
		}

		// ���f���ׂ��m�[�h��ݒ�
		for( j=0; j<BCT_MARUNOMU_ANM_NODE_NUM; j++ ){
			idx = NNS_G3dGetNodeIdxByName( cp_nodeinfo, &sc_AnmNodeName[i][j] );
			if( idx != -1 ){
				NNS_G3dAnmObjEnableID( p_wk->anm[i].pAnmObj, idx );
			}
//			OS_TPrintf( "node name %s  idx %d\n", sc_AnmNodeName[i][j].name, idx );
		}
    }

	// �J���[�A�j���ǂݍ���
	for( i=0; i<BCT_MARUNOMU_ANM_COLANM_NUM; i++ ){
		D3DOBJ_AnmLoadH( &p_wk->colanm[i], &p_wk->mdl[ sc_BCT_MARUNOMU_COLANM_MDL[i] ], p_handle, 
				NARC_bucket_maru_robo_col_nsbtp, heapID, p_allocator );

		// �J���[�A�j���ݒ�
		D3DOBJ_AddAnm( &p_wk->obj[ sc_BCT_MARUNOMU_COLANM_MDL[i] ], &p_wk->colanm[i] );
	}


	// �J���[�A�j���p�����[�^������
	p_wk->col_top	= BCT_PLAYER_NUM;
	p_wk->col_rand	= FALSE;


	// �����A�j���ݒ�
	BCT_CLIENT_MarunomuDrawSetWalkAnm( p_wk, FALSE );

	// �I�[�v���A�j����o�^
	D3DOBJ_AddAnm( &p_wk->obj[ BCT_MARUNOMU_MDL_OPEN ], &p_wk->anm[ BCT_MARUNOMU_ANM_OPEN ] );
	D3DOBJ_AnmSet( &p_wk->anm[ BCT_MARUNOMU_ANM_OPEN ], 0 );
	D3DOBJ_SetDraw( &p_wk->obj[ BCT_MARUNOMU_MDL_OPEN ], TRUE );	
	p_wk->set_mouthanm = BCT_MARUNOMU_ANM_OPEN;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���`��I�u�W�F�N�g�j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawExit( BCT_MARUNOMU_DRAW* p_wk, NNSFndAllocator* p_allocator )
{
    int i;
	
    
    // �S���\�[�X�j��
	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
	    D3DOBJ_MdlDelete( &p_wk->mdl[i] ); 
	}

    for( i=0; i<BCT_MARUNOMU_ANM_NUM; i++ ){
        D3DOBJ_AnmDelete( &p_wk->anm[i], p_allocator );
    }

	for( i=0; i<BCT_MARUNOMU_ANM_COLANM_NUM; i++ ){
	    D3DOBJ_AnmDelete( &p_wk->colanm[i], p_allocator );
	}


    memset( p_wk, 0, sizeof(BCT_MARUNOMU_DRAW) );   
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���`��I�u�W�F�N�g���C������
 *
 *  @param  p_wk		���[�N
 *  @param  cp_data		�f�[�^
 *  @param	movetype	���ԂőJ�ڂ��铮��^�C�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawMain( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data, BCT_MARUNOMU_MOVE_TYPE movetype )
{
    // ���W�Ɗp�x�Ɗg�嗦��ݒ�
    BCT_CLIENT_MarunomuDrawMatrixSet( p_wk, cp_data );
	BCT_CLIENT_MarunomuDrawRotaSet( p_wk, cp_data );
	BCT_CLIENT_MarunomuDrawScaleSet( p_wk, cp_data );

	// �A�j���ݒ�
	switch( movetype ){
	// rotax�̒l��ݒ�
    case BCT_MARUNOMU_MOVE_EASY:			// �ȒP����
    case BCT_MARUNOMU_MOVE_FEVER:		// FEVER
		BCT_CLIENT_MarunomuDrawSetMouthAnm( p_wk, BCT_MARUNOMU_ANM_ROTA );
		BCT_CLIENT_MarunomuDrawAnmRotaSet( p_wk, cp_data->rotax );
		BCT_CLIENT_MarunomuDrawSetWalkAnm( p_wk, TRUE );
		break;

	/*
	// �Ǝ��A�j��
    case BCT_MARUNOMU_MOVE_BAKUBAKU_EFF:	// �΂��΂��O�G�t�F�N�g
		BCT_CLIENT_MarunomuDrawSetMouthAnm( p_wk, BCT_MARUNOMU_ANM_PURU );
		BCT_CLIENT_MarunomuDrawLoopMouthAnm( p_wk );
		break;
	//*/
    case BCT_MARUNOMU_MOVE_FEVER_EFF:	// FEVER�O�G�t�F�N�g
		BCT_CLIENT_MarunomuDrawSetMouthAnm( p_wk, BCT_MARUNOMU_ANM_FEV );
		BCT_CLIENT_MarunomuDrawSetWalkAnm( p_wk, FALSE );
		BCT_CLIENT_MarunomuDrawNoLoopMouthAnm_Speed( p_wk, BCT_MARUNOMU_ANM_FEVERSPEED );
		break;
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��A�j�����C��
 *
 *	@param	p_wk
 *	@param	cp_data
 *	@param	movetype 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawAnmMain( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data, BCT_MARUNOMU_MOVE_TYPE movetype )
{
	// ��ɕ����A�j��
	D3DOBJ_AnmLoop( &p_wk->anm[ BCT_MARUNOMU_ANM_WALK ], BCT_MARUNOMU_ANM_SLOWSPEED );

	BCT_CLIENT_MarunomuDrawColAnmMain( p_wk );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���ɉ�]�p�x�@���W��ݒ�
 *
 *  @param  p_wk        ���[�N
 *  @param  cp_data     �}���m�[���f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawMatrixSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data )
{
	int i;

    // ���W��ݒ�
	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
	    D3DOBJ_SetMatrix( &p_wk->obj[i], cp_data->matrix.x, cp_data->matrix.y, cp_data->matrix.z );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�p�x��ݒ�
 *
 *	@param	p_wk
 *	@param	cp_data 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawRotaSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data )
{
	int i;
    fx32 scale;
    u32 setrota;
	
    // ���ʊp�x�ݒ�
    setrota = cp_data->rota + BCT_MARUNOMU_DRAWROTA_ADD;

	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
	    D3DOBJ_SetRota( &p_wk->obj[i], (u16)setrota, D3DOBJ_ROTA_WAY_Y );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g�嗦��ݒ�
 *
 *	@param	p_wk
 *	@param	cp_data 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawScaleSet( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data )
{
	int i;
	
	// �}���m�[���̕`��ɔ��f
	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
	    D3DOBJ_SetScale( &p_wk->obj[i], cp_data->draw_scale, cp_data->draw_scale, cp_data->draw_scale );
	}
}

//----------------------------------------------------------------------------
/**
 *  @brief  �}���m�[���`��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawDraw( BCT_MARUNOMU_DRAW* p_wk, const BCT_MARUNOMU* cp_data )
{
	int i;
/*
	MtxFx33 mtx;
	MtxFx33 tmp;

    MTX_Identity33( &mtx );
    MTX_RotY33( &mtx, 
			FX_SinIdx( cp_data->rota + BCT_MARUNOMU_DRAWROTA_ADD ), 
			FX_CosIdx( cp_data->rota + BCT_MARUNOMU_DRAWROTA_ADD ) );

    MTX_Identity33( &tmp );
    MTX_RotX33( &tmp, 
			FX_SinIdx( FX_GET_ROTA_NUM(40) ), 
			FX_CosIdx( FX_GET_ROTA_NUM(40) ) );

    MTX_Concat33( &tmp, &mtx, &mtx );

	D3DOBJ_DrawRMtx( &p_wk->obj, &mtx );
//*/	
	for( i=0; i<BCT_MARUNOMU_MDL_NUM; i++ ){
		D3DOBJ_Draw( &p_wk->obj[i] );	
	}
}

//----------------------------------------------------------------------------
/**
 *  @brief  ��]�p�x�ɑΉ������A�j���[�V�����t���[����ݒ�
 *
 *  @param  p_wk        ���[�N
 *  @param  rotax       X����]�p�x
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawAnmRotaSet( BCT_MARUNOMU_DRAW* p_wk, u32 rotax )
{
    fx32 frame;


    // X����]�p�x�̃A�j���t���[����ݒ�
	// 90��BCT_MARUNOMU_ANM_FRAME_MAX
	rotax -= BCT_MARUNOMU_ROTA_X_MIN;
    frame = (rotax * 90) / RotKeyR( BCT_MARUNOMU_ROTA_X_MAX_360-BCT_MARUNOMU_ROTA_X_MIN );	// 0�`90�̒l�ɂ���
	frame = (frame * BCT_MARUNOMU_ANM_FRAME_MAX) / 90;
	frame = BCT_MARUNOMU_ANM_FRAME_MAX - frame;
    D3DOBJ_AnmSet( &p_wk->anm[BCT_MARUNOMU_ANM_ROTA], frame );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̃A�j����ύX����
 *
 *	@param	p_wk		���[�N
 *	@param	anmno		�A�j���i���o�[
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawSetMouthAnm( BCT_MARUNOMU_DRAW* p_wk, u32 anmno )
{
	GF_ASSERT( anmno < BCT_MARUNOMU_ANM_NUM );
	
	if( p_wk->set_mouthanm != anmno ){
		
		D3DOBJ_DelAnm( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[p_wk->set_mouthanm] ], &p_wk->anm[ p_wk->set_mouthanm ] );
		D3DOBJ_AddAnm( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[anmno] ], &p_wk->anm[ anmno ] );

		D3DOBJ_AnmSet( &p_wk->anm[ anmno ], 0 );

		// �I�u�W�F�̕\��ONOFF
		D3DOBJ_SetDraw( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[p_wk->set_mouthanm] ], FALSE );	
		D3DOBJ_SetDraw( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[anmno] ], TRUE );	

		p_wk->set_mouthanm = anmno;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�j���̃��[�v�Đ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawLoopMouthAnm( BCT_MARUNOMU_DRAW* p_wk )
{
	D3DOBJ_AnmLoop( &p_wk->anm[ p_wk->set_mouthanm ], BCT_MARUNOMU_ANM_SPEED );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�j���̃��[�v�Ȃ��Đ�
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_MarunomuDrawNoLoopMouthAnm( BCT_MARUNOMU_DRAW* p_wk )
{
	return D3DOBJ_AnmNoLoop( &p_wk->anm[ p_wk->set_mouthanm ], BCT_MARUNOMU_ANM_SPEED );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�j���̃��[�v�Ȃ��Đ�	�A�j���X�s�[�h�w��
 *
 *	@param	p_wk		���[�N
 *	@param	speed		�X�s�[�h
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_MarunomuDrawNoLoopMouthAnm_Speed( BCT_MARUNOMU_DRAW* p_wk, fx32 speed )
{
	return D3DOBJ_AnmNoLoop( &p_wk->anm[ p_wk->set_mouthanm ], speed );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawSetWalkAnm( BCT_MARUNOMU_DRAW* p_wk, BOOL flag )
{
	if( p_wk->walk_anm_flag != flag ){

		if( flag == TRUE ){
			D3DOBJ_AddAnm( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[BCT_MARUNOMU_ANM_WALK] ], &p_wk->anm[ BCT_MARUNOMU_ANM_WALK ] );
		}else{
			D3DOBJ_DelAnm( &p_wk->obj[ sc_BCT_MARUNOMU_ANM_MDL[BCT_MARUNOMU_ANM_WALK] ], &p_wk->anm[ BCT_MARUNOMU_ANM_WALK ] );
		}

		p_wk->walk_anm_flag = flag;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J���[�A�j��	�����_���J���[�A�j����ݒ肷�邩
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O	TRUE�F�����_���A�j���Đ�
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawSetColAnmRand( BCT_MARUNOMU_DRAW* p_wk, BOOL flag )
{
	p_wk->col_rand = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J���[�A�j��	�P�ʂ̐F�ύX
 *
 *	@param	p_wk		���[�N
 *	@param	plno		�v���C���[�i���o�[
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawSetColAnmTop( BCT_MARUNOMU_DRAW* p_wk, u32 plno )
{
	p_wk->col_top = plno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J���[�A�j�����C��
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MarunomuDrawColAnmMain( BCT_MARUNOMU_DRAW* p_wk )
{
	int i;

	if( p_wk->col_rand == TRUE ){
		// �F�����[�v�Đ�
		if( (p_wk->colanm_frame + BCT_MARUNOMU_ANM_SPEED) < BCT_MARUNOMU_ANM_COL_END ){
			p_wk->colanm_frame += BCT_MARUNOMU_ANM_SPEED;
		}else{
			p_wk->colanm_frame = BCT_MARUNOMU_ANM_COL_1PSTART;
		}
	}else{
		u32 anm_idx;

		if( p_wk->col_top == BCT_PLAYER_NUM ){
			anm_idx = BCT_MARUNOMU_ANM_COL_NORMAL;
		}else{
			anm_idx = BCT_MARUNOMU_ANM_COL_1P + p_wk->col_top;
		}
		// �g�b�v�J���[�̂Ƃ���Ń��[�v�Đ�
		// �J�n�t���[���ɂȂ��Ȃ��ĂȂ��Ƃ�
		if( p_wk->colanm_frame < (sc_BCT_MARUNOMU_ANM_COL_DATA[anm_idx][BCT_MARUNOMU_ANM_COL_DATA_START] * FX32_ONE) ){
			p_wk->colanm_frame = sc_BCT_MARUNOMU_ANM_COL_DATA[anm_idx][BCT_MARUNOMU_ANM_COL_DATA_START] * FX32_ONE;
		}
		// �I���t���[���ɂȂ�܂ōĐ�
		else if( (p_wk->colanm_frame + BCT_MARUNOMU_ANM_SPEED) < (sc_BCT_MARUNOMU_ANM_COL_DATA[anm_idx][BCT_MARUNOMU_ANM_COL_DATA_END] * FX32_ONE) ){
			p_wk->colanm_frame += BCT_MARUNOMU_ANM_SPEED;
		}
		// �ŏ��̂Ƃ���Ƀ��[�v
		else{
			p_wk->colanm_frame = sc_BCT_MARUNOMU_ANM_COL_DATA[anm_idx][BCT_MARUNOMU_ANM_COL_DATA_START] * FX32_ONE;
		}
	}

	for( i=0; i<BCT_MARUNOMU_ANM_COLANM_NUM; i++ ){
		D3DOBJ_AnmSet( &p_wk->colanm[i], p_wk->colanm_frame );
	}
}

//----------------------------------------------------------------------------
/**
 *  @brief  �^�b�`�p�l��2D���W���RD���W�ɕύX����
 *
 *  @param  x           �����W
 *  @param  y           �����W
 *  @param  p_mat       3D���W�i�[��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_2DMATto3DMAT( s16 x, s16 y, VecFx32* p_mat )
{
    p_mat->x = BCT_FIELD_DOT2MAT( (255 - x) );
    p_mat->z = BCT_FIELD_PLAYER_Z - BCT_FIELD_DOT2MAT( y );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �RD���W���^�b�`�p�l�����W�ɕύX
 *
 *  @param  cp_mat      �RD���W
 *  @param  p_2dmat     �QD���W�i�[�� 
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_3DMATto2DMAT( const VecFx32* cp_mat, NNSG2dSVec2* p_2dmat )
{
    p_2dmat->x = 255 - BCT_FIELD_MAT2DOT( cp_mat->x );
    p_2dmat->y = BCT_FIELD_MAT2DOT( (-cp_mat->z) + BCT_FIELD_PLAYER_Z );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �x�N�g����NETID�Ɋ֘A�t���ĉ�]������
 *
 *  @param  p_vec   �x�N�g��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_VecNetIDRot( const VecFx32* cp_vec, VecFx32* p_invec, u32 plno, u32 comm_num )
{
    MtxFx33 rot;

    MTX_Identity33( &rot );
    MTX_RotY33( &rot, FX_SinIdx(BCT_NUTS_FIELD_NetIDRota[comm_num-1][ plno ]), 
            FX_CosIdx(BCT_NUTS_FIELD_NetIDRota[comm_num-1][ plno ]) );
    MTX_MultVec33( cp_vec, &rot, p_invec );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �x�N�g����NETID�Ɋ֘A�t���ċt��]������
 *          ��]�������W�����ɖ߂��Ƃ��ȂǂɎg�p
 *
 *  @param  cp_vec
 *  @param  p_invec
 *  @param  plno 
 *  @param  comm_num
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_VecNetIDRetRot( const VecFx32* cp_vec, VecFx32* p_invec, u32 plno, u32 comm_num )
{
    MtxFx33 rot;

    MTX_Identity33( &rot );
    MTX_RotY33( &rot, FX_SinIdx(BCT_NUTS_FIELD_NetIDRota[ comm_num-1 ][ plno ]), 
            FX_CosIdx(BCT_NUTS_FIELD_NetIDRota[ comm_num-1 ][ plno ]) );
    MTX_Inverse33( &rot, &rot );
    MTX_MultVec33( cp_vec, &rot, p_invec );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`��V�X�e���@������
 *
 *  @param  p_wk        ���[�N
 *  @param  p_handle    �n���h��
 *  @param  heapID      �q�[�v
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawSysInit( BCT_CLIENT_GRAPHIC* p_wk, ARCHANDLE* p_handle, u32 heapID  )
{
    BOOL result;
    int i;
	static const u8 sc_BCT_NUTSRES_MDL_TBL[ BCT_NUTSRES_MDLNUM ] = {
		NARC_bucket_kinomi_1p_nsbmd,
		NARC_bucket_kinomi_2p_nsbmd,
		NARC_bucket_kinomi_3p_nsbmd,
		NARC_bucket_kinomi_4p_nsbmd,
		NARC_bucket_kinomi_ex_nsbmd,
	};
    
    // �`��f�[�^�쐬
	for( i=0; i<BCT_NUTSRES_MDLNUM; i++ ){
	    D3DOBJ_MdlLoadH( &p_wk->nutsres.mdl[i], p_handle, sc_BCT_NUTSRES_MDL_TBL[i], heapID );
	}

	// �A�ǂݍ���
	D3DOBJ_MdlLoadH( &p_wk->nutsres.shadowmdl, p_handle, NARC_bucket_maru_kage_nsbmd, heapID );
	NNS_G3dMdlUseGlbAlpha( p_wk->nutsres.shadowmdl.pModel );	// �A���t�@�l�̓v���O�����̂��̂��Q��

    {
        // OAM���\�[�X�Ǎ���
        p_wk->nutsres.resobj[ 0 ] = CLACT_U_ResManagerResAddArcChar_ArcHandle(
                    p_wk->resMan[ 0 ], p_handle,
                    NARC_bucket_kinomi_01_NCGR,
                    FALSE, BCT_GRA_NUTS_OAM_RESID, NNS_G2D_VRAM_TYPE_2DSUB, heapID );

        p_wk->nutsres.resobj[ 1 ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle(
                    p_wk->resMan[ 1 ], p_handle,
                    NARC_bucket_kinomi_NCLR,
                    FALSE, BCT_GRA_NUTS_OAM_RESID, NNS_G2D_VRAM_TYPE_2DSUB, 4, heapID );

        p_wk->nutsres.resobj[ 2 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_wk->resMan[ 2 ], p_handle,
                NARC_bucket_kinomi_01_NCER, FALSE,
                BCT_GRA_NUTS_OAM_RESID, CLACT_U_CELL_RES, heapID );

        p_wk->nutsres.resobj[ 3 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_wk->resMan[ 3 ], p_handle,
                NARC_bucket_kinomi_01_NANR, FALSE,
                BCT_GRA_NUTS_OAM_RESID, CLACT_U_CELLANM_RES, heapID );

        // �]��
        result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_wk->nutsres.resobj[ 0 ] );
        GF_ASSERT( result );
        result = CLACT_U_PlttManagerSetCleanArea( p_wk->nutsres.resobj[ 1 ] );
        GF_ASSERT( result );

        // ���\�[�X�����j��
        CLACT_U_ResManagerResOnlyDelete( p_wk->nutsres.resobj[ 0 ] );
        CLACT_U_ResManagerResOnlyDelete( p_wk->nutsres.resobj[ 1 ] );
    }

    // �Z���A�N�^�[�w�b�_�[�쐬
    CLACT_U_MakeHeader( &p_wk->nutsres.header, 
            BCT_GRA_NUTS_OAM_RESID, BCT_GRA_NUTS_OAM_RESID,
            BCT_GRA_NUTS_OAM_RESID, BCT_GRA_NUTS_OAM_RESID,
            CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,
            BCT_GRA_NUTS_OAM_BGPRI,
            p_wk->resMan[0], p_wk->resMan[1], 
            p_wk->resMan[2], p_wk->resMan[3],
            NULL, NULL );

    // �e���[�N�̏�����
    for( i=0; i<BCT_NUTSBUFFOAM_NUM; i++ ){
        BCT_CLIENT_NutsDrawInit( p_wk, &p_wk->nuts[i], heapID );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`��V�X�e���j��
 *
 *  @param  p_wk    ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawSysExit( BCT_CLIENT_GRAPHIC* p_wk )
{
    int i;

    // �e�؂̎��j��
    for( i=0; i<BCT_NUTSBUFFOAM_NUM; i++ ){
        BCT_CLIENT_NutsDrawExit( p_wk, &p_wk->nuts[i] );
    }

    // ���\�[�X�j��
    {

        // VRAM�Ǘ�����j��
        CLACT_U_CharManagerDelete( p_wk->nutsres.resobj[0] );
        CLACT_U_PlttManagerDelete( p_wk->nutsres.resobj[1] );
        
        // ���\�[�X�j��
        CLACT_U_ResManagerResDelete( p_wk->resMan[0], p_wk->nutsres.resobj[0] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[1], p_wk->nutsres.resobj[1] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[2], p_wk->nutsres.resobj[2] );
        CLACT_U_ResManagerResDelete( p_wk->resMan[3], p_wk->nutsres.resobj[3] );
    }
    for( i=0; i<BCT_NUTSRES_MDLNUM; i++ ){
        D3DOBJ_MdlDelete( &p_wk->nutsres.mdl[i] );
    }
    D3DOBJ_MdlDelete( &p_wk->nutsres.shadowmdl );

    // �S�f�[�^�j��
    memset( &p_wk->nutsres, 0, sizeof(BCT_CLIENT_NUTS_RES) );
    memset( &p_wk->nuts, 0, sizeof(BCT_CLIENT_NUTS_DRAW)*BCT_NUTSBUFFOAM_NUM );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`��ݒ�
 *
 *  @param  p_wk        ���[�N
 *  @param  cp_data     �\������؂̎��f�[�^
 *  @param  comm_num    �ʐM�l��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawSysStart( BCT_CLIENT_GRAPHIC* p_wk, const BCT_CLIENT_NUTS* cp_data, u32 comm_num, u32 plno )
{
    BCT_CLIENT_NUTS_DRAW* p_drawwk;
    p_drawwk = BCT_CLIENT_NutsDrawWkGet( p_wk );

    if( cp_data->data.plno == plno ){
        BCT_CLIENT_NutsDrawStart( p_wk, p_drawwk, cp_data, comm_num );
//      BCT_CLIENT_NutsDrawStartNoOam( p_wk, p_drawwk, cp_data );
    }else{
        BCT_CLIENT_NutsDrawStartNoOam( p_wk, p_drawwk, cp_data );
    }
}

//----------------------------------------------------------------------------
/**
 *  @brief  �`���~
 *  
 *  @param  p_wk        ���[�N
 *  @param  cp_data     �؂̎��f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawSysEnd( BCT_CLIENT_GRAPHIC* p_wk, const BCT_CLIENT_NUTS* cp_data )
{
    int i;

    for( i=0; i<BCT_NUTSBUFFOAM_NUM; i++ ){
        if( p_wk->nuts[i].cp_data == cp_data ){
            // ��~
            BCT_CLIENT_NutsDrawEnd( &p_wk->nuts[i] );
            return ;
        }
    }
    GF_ASSERT( 0 );
}


//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`�惁�C��
 *
 *  @param  p_wk        ���[�N
 *  @param  comm_num    �ʐM�l��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawSysMain( BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num )
{
    int i;

    for( i=0; i<BCT_NUTSBUFFOAM_NUM; i++ ){
        if( p_wk->nuts[i].cp_data != NULL ){
            BCT_CLIENT_NutsDrawMain( &p_wk->nuts[i], p_wk, comm_num );
        }
    }
}


//----------------------------------------------------------------------------
/**
 *  @brief  �`��쐬
 *
 *  @param  p_wk        �O���t�B�b�N�f�[�^
 *  @param  p_data      ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawInit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, u32 heapID )
{
    // OAM�쐬
    {
        CLACT_ADD add = {0};

        add.ClActSet = p_wk->clactSet;
        add.ClActHeader = &p_wk->nutsres.header;
        add.sca.x = FX32_ONE;
        add.sca.y = FX32_ONE;
        add.sca.z = FX32_ONE;
        add.pri   = BCT_GRA_NUTS_OAM_PRI;
        add.DrawArea = NNS_G2D_VRAM_TYPE_2DSUB;
        add.heap  = heapID;
        p_data->p_clwk = CLACT_Add( &add );
        CLACT_SetDrawFlag( p_data->p_clwk, FALSE );
    }

	// �A�O���t�B�b�N�ݒ�
    D3DOBJ_Init( &p_data->shadow, &p_wk->nutsres.shadowmdl );
	D3DOBJ_SetDraw( &p_data->shadow, FALSE );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`��f�[�^�@�j��
 *
 *  @param  p_wk        �O���t�B�b�N���[�N
 *  @param  p_data      �؂̎��O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawExit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data )
{
    CLACT_Delete( p_data->p_clwk );
    memset( p_data, 0, sizeof(BCT_CLIENT_NUTS_DRAW) );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`��J�n
 *
 *  @param  p_wk    ���[�N
 *  @param  p_data  �؂̎��O���t�B�b�N���[�N
 *  @param  cp_data �؂̎��f�[�^
 *  @param  comm_num�ʐM�l��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawStart( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data, u32 comm_num )
{
    // �֘A�t����؂̎��f�[�^�ۑ�
    p_data->cp_data = cp_data;
    p_data->draw2d = TRUE;

	p_data->rota_chg_count = 0;

	// �p���b�g�I�t�Z�b�g�ݒ�
	CLACT_PaletteNoChg( p_data->p_clwk, cp_data->data.plno );
    
    // ���W�ݒ�
    BCT_CLIENT_NutsDrawMatrixSet( p_data, cp_data, comm_num );

    // �\���J�n
    CLACT_SetDrawFlag( p_data->p_clwk, TRUE );


#ifdef BCT_DEBUG
    BCT_DEBUG_NutsDrawNum++;
    OS_Printf( "nuts draw %d\n", BCT_DEBUG_NutsDrawNum );
#endif
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ȃ�RD�`�惂�[�h
 *
 *  @param  p_wk    ���[�N
 *  @param  p_data  �؂̎��O���t�B�b�N���[�N
 *  @param  cp_data �؂̎��f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawStartNoOam( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data )
{
    // �֘A�t����؂̎��f�[�^�ۑ�
    p_data->cp_data = cp_data;

	p_data->rota_chg_count = 0;

    // �\���J�n
    CLACT_SetDrawFlag( p_data->p_clwk, FALSE );
    BCT_CLIENT_Nuts3DDrawOn( p_data, p_wk );

#ifdef BCT_DEBUG
    BCT_DEBUG_NutsDrawNum++;
    OS_Printf( "nuts draw %d\n", BCT_DEBUG_NutsDrawNum );
#endif
}

//----------------------------------------------------------------------------
/**
 *  @brief  �؂̎��`�惁�C��
 *
 *  @param  p_data  ���[�N
 *  @param  comm_num�ʐM�l��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawMain( BCT_CLIENT_NUTS_DRAW* p_data, BCT_CLIENT_GRAPHIC* p_wk, u32 comm_num )
{
    BOOL result;
	D3DOBJ_MDL* p_mdl;

    // 2D�\�����Rd�\����
    if( p_data->draw2d == TRUE ){
        // ���W�����킹��
        result = BCT_CLIENT_NutsDrawMatrixSet( p_data, p_data->cp_data, comm_num );

        //  2D���W���O�ȉ��ɂȂ�����RD�\���ɐ؂�ւ���
        if( result == FALSE ){
            BCT_CLIENT_Nuts3DDrawOn( p_data, p_wk );
        }
    }

	if( p_data->draw2d == FALSE ){
		p_mdl = BCT_CLIENT_Nuts3DMdlGet( p_data->cp_data, &p_wk->nutsres );

		// �؂̎�����O����Ȃ�Â��\���A�����łȂ���Ζ��邭�\��
		if( p_data->cp_data->seq == BCT_NUTSSEQ_MOVEAWAY ){
			
			// �Â�����
			NNS_G3dMdlSetMdlAmbAll( p_mdl->pModel, GX_RGB( 31,31,31 ) );
//			NNS_G3dMdlSetMdlAmbAll( p_mdl->pModel, GX_RGB( 24,24,24 ) );
		}else{
			// ���邭����
			NNS_G3dMdlSetMdlAmbAll( p_mdl->pModel, GX_RGB( 18,18,18 ) );
		}
		

		// �`��
		// �i��ł�������ɉ�]��������
		BCT_CLIENT_NutsDrawRotaSet( p_data );
		// ���W�̐ݒ�
		D3DOBJ_SetMatrix( &p_data->obj, 
				p_data->cp_data->mat.x, p_data->cp_data->mat.y, p_data->cp_data->mat.z  );
		D3DOBJ_Draw( &p_data->obj );

		// �A�̕\��
		if( D3DOBJ_GetDraw( &p_data->shadow ) == TRUE ){
			BCT_CLIENT_NutsDrawShadowMatrixSet( p_data );
			D3DOBJ_Draw( &p_data->shadow );
			BCT_CLIENT_NutsDrawShadowAlpahReset( p_data );

		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�̍��W��ݒ肷��
 *
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawShadowMatrixSet( BCT_CLIENT_NUTS_DRAW* p_data )
{
	BOOL result;
	fx32 y_dif;
	fx32 y;
	fx32 scale;
	int alpha;

	// �}���m�[���͈͓����`�F�b�N
	result = BCT_CLIENT_NutsAwayMarunomuHitCheck( p_data->cp_data );
	if( result == TRUE ){
		// �}���m�[���̉A�̈ʒu�Ɉړ�
		y = BCT_SHADOW_Y_DEL;
	}else{

		// XZ���W�͂��̂܂܂��킹�āAY���W�����ɂ���
		result = BCT_CLIENT_NutsAwayDitchHitCheck( p_data->cp_data );
		if( result == FALSE ){
			// ���̈ʒu�ɉA���o��
			y = BCT_SHADOW_Y_FLOOR;
		}else{
			// �a�̈ʒu�ɉA���o��
			y = BCT_SHADOW_Y_DEL;
		}
	}

	D3DOBJ_SetMatrix( &p_data->shadow, 
			p_data->cp_data->mat.x, y, p_data->cp_data->mat.z  );

	// ������̍����ő傫�������߂�
	y_dif = p_data->cp_data->mat.y - y;
	scale = FX32_ONE + (FX_Div(FX_Mul( y_dif, BCT_SHADOW_SIZE_DIV ), BCT_SHADOW_Y_SIZE_DIF));
	D3DOBJ_SetScale( &p_data->shadow, 
			scale, scale, scale );

	alpha = (FX_Div(FX_Mul( y_dif, FX32_CONST(BCT_SHADOW_ALPHA_DIV) ), BCT_SHADOW_Y_SIZE_DIF)) >> FX32_SHIFT;
	alpha = BCT_SHADOW_ALPHA_MIN + BCT_SHADOW_ALPHA_DIV - alpha;

	// ALPHA��������
	NNS_G3dGlbPolygonAttr(
		0,
		0,
		0,
		0,
		alpha,
		0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�Őݒ肵���A���t�@�l�����Z�b�g
 *
 *	@param	p_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawShadowAlpahReset( BCT_CLIENT_NUTS_DRAW* p_data )
{
	NNS_G3dGlbPolygonAttr(
		0,
		0,
		0,
		0,
		31,
		0 );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �\���̒�~
 *
 *  @param  p_data  ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawEnd( BCT_CLIENT_NUTS_DRAW* p_data )
{
    p_data->cp_data = NULL;
    p_data->draw2d = FALSE;
    CLACT_SetDrawFlag( p_data->p_clwk, FALSE );

	D3DOBJ_SetDraw( &p_data->shadow, FALSE );

#ifdef BCT_DEBUG
    BCT_DEBUG_NutsDrawNum--;
    OS_Printf( "nuts draw %d\n", BCT_DEBUG_NutsDrawNum );
#endif
}


//----------------------------------------------------------------------------
/**
 *  @brief  �󂢂Ă��郏�[�N���擾����
 *
 *  @param  p_wk    ���[�N
 *
 *  @return �؂̎����[�N
 */
//-----------------------------------------------------------------------------
static BCT_CLIENT_NUTS_DRAW* BCT_CLIENT_NutsDrawWkGet( BCT_CLIENT_GRAPHIC* p_wk )
{
    int i;  

    for( i=0; i<BCT_NUTSBUFFOAM_NUM; i++ ){
        if( p_wk->nuts[i].cp_data == NULL ){
            return &p_wk->nuts[i];
        }
    }
    GF_ASSERT( 0 );
    return NULL;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �`����W��ݒ�
 *
 *  @param  p_data  ���[�N
 *  @param  cp_data �؂̎��f�[�^
 *
 *  @retval TRUE    ��ʔ͈͓�
 *  @retval FALSE   ��ʔ͈͊O
 */
//-----------------------------------------------------------------------------
static BOOL BCT_CLIENT_NutsDrawMatrixSet( BCT_CLIENT_NUTS_DRAW* p_data, const BCT_CLIENT_NUTS* cp_data, u32 comm_num )
{
    VecFx32 mat;
    NNSG2dSVec2 vec2d;

    mat = p_data->cp_data->mat;
    BCT_CLIENT_VecNetIDRetRot( &mat, &mat, p_data->cp_data->data.plno, comm_num ); // ��]���Ă���̂����ɖ߂�
    BCT_CLIENT_3DMATto2DMAT( &mat, &vec2d );

    mat.x = vec2d.x << FX32_SHIFT;
    mat.y = (vec2d.y << FX32_SHIFT) + BCT_GRA_OAMSUBSURFACE_Y;
    mat.z = 0;

    CLACT_SetMatrix( p_data->p_clwk, &mat );

    if( (vec2d.x < 0) || (vec2d.y < 0) ){
        return FALSE;
    }
    return TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  3D�`��J�n
 *
 *  @param  p_data  ���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_Nuts3DDrawOn( BCT_CLIENT_NUTS_DRAW* p_data, BCT_CLIENT_GRAPHIC* p_wk )
{
	D3DOBJ_MDL* p_mdl;
	
    CLACT_SetDrawFlag( p_data->p_clwk, FALSE );
    p_data->draw2d = FALSE;

	p_mdl = BCT_CLIENT_Nuts3DMdlGet( p_data->cp_data, &p_wk->nutsres );

	D3DOBJ_Init( &p_data->obj, p_mdl );
	D3DOBJ_SetDraw( &p_data->obj, TRUE );

	// �����̖؂̎��Ȃ�e���o��
	if( p_data->cp_data->seq == BCT_NUTSSEQ_MOVE ){
		D3DOBJ_SetDraw( &p_data->shadow, TRUE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��f�[�^����	�\�����郂�f���̃��\�[�X���擾����
 *	
 *	@param	cp_data		�f�[�^
 *	@param	p_nutsres	�؂̎����\�[�X�f�[�^
 *
 *	@return	���f�����[�N
 */
//-----------------------------------------------------------------------------
static D3DOBJ_MDL* BCT_CLIENT_Nuts3DMdlGet( const BCT_CLIENT_NUTS* cp_data, BCT_CLIENT_NUTS_RES* p_nutsres )
{
	u32 mdlno;

    // 3D�쐬
	// �����_�[�I�u�W�F�N�g�ɓo�^
	if( cp_data->data.special == TRUE ){	// ����؂̎�
		mdlno = BCT_NUTSRES_MDLSPECIAL;
	}else{
		mdlno = cp_data->data.plno;
	}

	return &p_nutsres->mdl[ mdlno ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��`��@��]�p�x�ݒ�
 *
 *	@param	p_data	�f�[�^
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NutsDrawRotaSet( BCT_CLIENT_NUTS_DRAW* p_data )
{
	fx32 power;
	u16 move_rota;
	u16 rota_speed;

	// �����]�p�x��ς��Ă�ƁA�������d���̂ł��܂ɍs��
	p_data->rota_chg_count --;
	if( p_data->rota_chg_count < 0 ){
		power = BCT_CLIENT_NutsNowPowerGet( p_data->cp_data );
		rota_speed = ((power*BCT_NUTSDRAW_ROTA_DIS) / BCT_NUTS_POWER_MAX);

		// �ړ������̊����ŉ�]������ς���
		move_rota = FX_Atan2Idx( p_data->cp_data->data.way.z, p_data->cp_data->data.way.x );
		p_data->rota_speed_x = BCT_NUTSDRAW_ROTA_MIN+(FX_Mul( FX_SinIdx( move_rota ), rota_speed<<FX32_SHIFT ) >> FX32_SHIFT);
		p_data->rota_speed_z = BCT_NUTSDRAW_ROTA_MIN+(FX_Mul( FX_CosIdx( move_rota ), rota_speed<<FX32_SHIFT ) >> FX32_SHIFT);

		p_data->rota_chg_count = BCT_NUTSDRAW_ROTA_CHG_TIMING;
	}

	
	p_data->rota_x += p_data->rota_speed_x;
	p_data->rota_z += p_data->rota_speed_z;
	D3DOBJ_SetRota( &p_data->obj, p_data->rota_x, D3DOBJ_ROTA_WAY_X );
	D3DOBJ_SetRota( &p_data->obj, p_data->rota_z, D3DOBJ_ROTA_WAY_Z );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎��@������
 *
 *  @param  p_wk        ���[�N
 *  @param  p_nuts      �؂̎����[�N
 *  @param	plno		�ʐMID
 *  @param  heapID      �q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawInit( BCT_CLIENT_GRAPHIC* p_wk, BCT_CLIENT_HANDNUTS_DRAW* p_nuts, u32 plno, u32 heapID )
{
    CLACT_ADD add = {0};

    add.ClActSet = p_wk->clactSet;
    add.ClActHeader = &p_wk->nutsres.header;
    add.sca.x = FX32_ONE;
    add.sca.y = FX32_ONE;
    add.sca.z = FX32_ONE;
    add.pri   = BCT_GRA_NUTS_OAM_PRI;
    add.DrawArea = NNS_G2D_VRAM_TYPE_2DSUB;
    add.heap  = heapID;
    p_nuts->p_clwk = CLACT_Add( &add );
    CLACT_SetDrawFlag( p_nuts->p_clwk, FALSE );
	CLACT_PaletteNoChg( p_nuts->p_clwk, plno );
    p_nuts->draw = FALSE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎��`��f�[�^�@�j��
 *
 *  @param  p_nuts  �؂̎����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawExit( BCT_CLIENT_HANDNUTS_DRAW* p_nuts )
{
    CLACT_Delete( p_nuts->p_clwk );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎��`��J�n
 *
 *  @param   p_nuts     �؂̎����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawStart( BCT_CLIENT_HANDNUTS_DRAW* p_nuts )
{
    CLACT_SetDrawFlag( p_nuts->p_clwk, TRUE );
    p_nuts->draw = TRUE;
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎��`��@���C��
 *
 *  @param  p_nuts      �؂̎����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawMain( BCT_CLIENT_HANDNUTS_DRAW* p_nuts )
{   
    VecFx32 mat;
    
    // �\�������������Ȃ�
    if( p_nuts->draw == FALSE ){
        return ;
    }

    // ���̃^�b�`�p�l�����W�ɐݒ肷��
	BCT_CLIENT_HandNutsDrawSetMatrix( p_nuts, sys.tp_x, sys.tp_y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̐ݒ�
 *
 *	@param	p_nuts		���[�N
 *	@param	x			�����W
 *	@param	y			�����W
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawSetMatrix( BCT_CLIENT_HANDNUTS_DRAW* p_nuts, s32 x, s32 y )
{
    VecFx32 mat;

    mat.x = x << FX32_SHIFT;
    mat.y = (y << FX32_SHIFT) + BCT_GRA_OAMSUBSURFACE_Y;
    CLACT_SetMatrix( p_nuts->p_clwk, &mat );
}

//----------------------------------------------------------------------------
/**
 *  @brief  �����Ă���؂̎��`��@�I��
 *
 *  @param  p_nuts      �؂̎����[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_HandNutsDrawEnd( BCT_CLIENT_HANDNUTS_DRAW* p_nuts )
{
    CLACT_SetDrawFlag( p_nuts->p_clwk, FALSE );
    p_nuts->draw = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�̖؂̎�����ʊO�֏o���G�t�F�N�g	�V�X�e��������
 *
 *	@param	p_drawsys		�`��V�X�e��
 *	@param	p_wk			���̃V�X�e���̃��[�N
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamAwayNutsInit( BCT_CLIENT_GRAPHIC* p_drawsys, BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk, u32 plno, u32 heapID )
{
	int i;
    CLACT_ADD add = {0};

    add.ClActSet = p_drawsys->clactSet;
    add.ClActHeader = &p_drawsys->nutsres.header;
    add.sca.x = BCT_OAMAWAYNUTS_SCALE;
    add.sca.y = BCT_OAMAWAYNUTS_SCALE;
    add.sca.z = BCT_OAMAWAYNUTS_SCALE;
    add.pri   = BCT_GRA_NUTS_OAM_PRI;
    add.DrawArea = NNS_G2D_VRAM_TYPE_2DSUB;
    add.heap  = heapID;

	for( i=0; i<BCT_OAMAWAYNUTS_BUFFNUM; i++ ){
	    p_wk->nutsbuff[i].p_clwk = CLACT_Add( &add );
		CLACT_SetDrawFlag( p_wk->nutsbuff[i].p_clwk, FALSE );
		CLACT_SetAffineParam( p_wk->nutsbuff[i].p_clwk, CLACT_AFFINE_NORMAL );
		CLACT_PaletteNoChg( p_wk->nutsbuff[i].p_clwk, plno );
	    p_wk->nutsbuff[i].draw = FALSE;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�̖؂̎�����ʊO�֏o���G�t�F�N�g	�V�X�e���j��
 *
 *	@param	p_drawsys		�`��V�X�e��
 *	@param	p_wk			���̃V�X�e���̃��[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamAwayNutsExit( BCT_CLIENT_GRAPHIC* p_drawsys, BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk )
{
	int i;

	for( i=0; i<BCT_OAMAWAYNUTS_BUFFNUM; i++ ){
		CLACT_Delete( p_wk->nutsbuff[i].p_clwk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ʊO�ֈړ����Ă�������
 *
 *	@param	p_wk	���̃V�X�e���̃��[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamAwayNutsMain( BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk )
{
	int i;
	s32 count_sub;
	s32 x, y;
	fx32 scale;
	VecFx32 scale_vec;
	
	// power��������W�𑫂��āA��ʊO�֌�������I���	
	for( i=0; i<BCT_OAMAWAYNUTS_BUFFNUM; i++ ){
		if( p_wk->nutsbuff[i].draw == TRUE ){

			// �R�o�E���h���Ȃ��痎���Ă���
			// 1�o�E���h
			if( p_wk->nutsbuff[i].count < BCT_OAMAWAYNUTS_MOVE0_COUNT ){
				count_sub = p_wk->nutsbuff[i].count;
				BCT_CLIENT_OamAwayNutsMoveXY( count_sub, BCT_OAMAWAYNUTS_MOVE0_COUNT, BCT_OAMAWAYNUTS_MOVE0_SPEED, BCT_OAMAWAYNUTS_MOVE0_SROTA, BCT_OAMAWAYNUTS_MOVE0_EROTA, &x, &y );
			}
			// 2�o�E���h
			else if( p_wk->nutsbuff[i].count < (BCT_OAMAWAYNUTS_MOVE0_COUNT+BCT_OAMAWAYNUTS_MOVE1_COUNT) ){
				count_sub = p_wk->nutsbuff[i].count - BCT_OAMAWAYNUTS_MOVE0_COUNT;
				
				BCT_CLIENT_OamAwayNutsMoveXY( count_sub, BCT_OAMAWAYNUTS_MOVE1_COUNT, BCT_OAMAWAYNUTS_MOVE1_SPEED, BCT_OAMAWAYNUTS_MOVE1_SROTA, BCT_OAMAWAYNUTS_MOVE1_EROTA, &x, &y );
			}
			// 3�o�E���h
			else{
				count_sub = p_wk->nutsbuff[i].count - (BCT_OAMAWAYNUTS_MOVE0_COUNT+BCT_OAMAWAYNUTS_MOVE1_COUNT);

				BCT_CLIENT_OamAwayNutsMoveXY( count_sub, BCT_OAMAWAYNUTS_MOVE2_COUNT, BCT_OAMAWAYNUTS_MOVE2_SPEED, BCT_OAMAWAYNUTS_MOVE2_SROTA, BCT_OAMAWAYNUTS_MOVE2_EROTA, &x, &y );
			}

			p_wk->nutsbuff[i].count++;

			// ���������Ă���
			scale = BCT_OAMAWAYNUTS_SCALE - ( (p_wk->nutsbuff[i].count * BCT_OAMAWAYNUTS_SCALE_DIV) / BCT_OAMAWATNUTS_MOVE_COUNT);
			scale = FX_Div( scale << FX32_SHIFT, FX32_CONST(10) ); 
			scale_vec.x = scale;
			scale_vec.y = scale;
			CLACT_SetScale( p_wk->nutsbuff[i].p_clwk, &scale_vec );	

			
			if( p_wk->nutsbuff[i].left ){
				p_wk->nutsbuff[i].mat.x += x << FX32_SHIFT;
			}else{
				p_wk->nutsbuff[i].mat.x -= x << FX32_SHIFT;
			}
			p_wk->nutsbuff[i].mat.y += y << FX32_SHIFT;
			CLACT_SetMatrix( p_wk->nutsbuff[i].p_clwk, &p_wk->nutsbuff[i].mat );

			if( (p_wk->nutsbuff[i].mat.x < BCT_OAMAWAYNUTS_DEL_XMIN) ||
				(p_wk->nutsbuff[i].mat.x > BCT_OAMAWAYNUTS_DEL_XMAX) ||
				(p_wk->nutsbuff[i].mat.y < BCT_OAMAWAYNUTS_DEL_YMIN + BCT_GRA_OAMSUBSURFACE_Y) ||
				(p_wk->nutsbuff[i].mat.y > BCT_OAMAWAYNUTS_DEL_YMAX + BCT_GRA_OAMSUBSURFACE_Y) ){
				p_wk->nutsbuff[i].draw = FALSE;
				CLACT_SetDrawFlag( p_wk->nutsbuff[i].p_clwk, FALSE );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ʊO�ֈړ����Ă�������	�J�n����
 *
 *	@param	p_wk	���̃V�X�e���̃��[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamAwayNutsStart( BCT_CLIENT_OAMAWAYNUTS_DRAW* p_wk, s32 x, s32 y )
{
	int i;
	BCT_CLIENT_OAMAWAYNUTS* p_obj = NULL;

	// �󂢂Ă郏�[�N�擾
	for( i=0; i<BCT_OAMAWAYNUTS_BUFFNUM; i++ ){
		if( p_wk->nutsbuff[i].draw == FALSE ){
			p_obj = &p_wk->nutsbuff[i];
			break;
		}
	}
	if( p_obj == NULL ){
//		GF_ASSERT(0);
		return ;	// ����ȏゾ���Ȃ�
	}

	// ���E�ǂ����ɂ��邩�ŃG�t�F�N�g��ς��邭�炢
	if( x < 128 ){
		p_obj->left = TRUE;
	}else{
		p_obj->left = FALSE;
	}
	
	// ���W��ݒ�
	p_obj->mat.x = x << FX32_SHIFT;
	p_obj->mat.y = (y << FX32_SHIFT) + BCT_GRA_OAMSUBSURFACE_Y;
	CLACT_SetMatrix( p_obj->p_clwk, &p_obj->mat );

	// �\���A����J�n
	p_obj->draw = TRUE;
	CLACT_SetDrawFlag( p_obj->p_clwk, TRUE );

	// �J�E���^������
	p_obj->count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ʊO�ֈړ����Ă�������	�ړ��l�����߂�
 *
 *	@param	count		�J�E���g
 *	@param	countmax	�J�E���g�ő�l
 *	@param	speed		�X�s�[�h
 *	@param	srota		�J�n��]�p�x
 *	@param	erota		�I����]�p�x
 *	@param	p_x			���ړ��l�i�[��
 *	@param	p_y			���ړ��l�i�[��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_OamAwayNutsMoveXY( int count, int countmax, int speed, int srota, int erota, s32* p_x, s32* p_y )
{
	s16 rota;
	s32 rota_div;

	// �p�x�̍������߂�
	rota_div = erota - srota;

	// �J�E���g�͌��݂̉�]�p�x�����߂�
	rota = srota + ( (count * rota_div) / countmax );

	// �p�x����ړ��l�����߂�
	*p_x = FX_Mul( Cos360( rota ), speed << FX32_SHIFT ) >> FX32_SHIFT;
	*p_y = FX_Mul( Sin360( rota ), speed << FX32_SHIFT ) >> FX32_SHIFT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C����ʔw�i�@�\��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_graphic	�`��V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	comm_num	�ʐM�l��
 *	@param	plno		�ʐMID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainBackInit( BCT_CLIENT_MAINBACK* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, ARCHANDLE* p_handle, int comm_num, u32 plno, u32 heapID, NNSFndAllocator* p_allocator )
{
	static const u16 CommNumNSBMD[ BCT_PLAYER_NUM ] = {
		NARC_bucket_maru_stage_2p_nsbmd,	// 1�l�p
		NARC_bucket_maru_stage_2p_nsbmd,	// 2�l�p
		NARC_bucket_maru_stage_3p_nsbmd,	// 3�l�p
		NARC_bucket_maru_stage_4p_nsbmd,	// 4�l�p
	};
	static const u16 sc_MainBackAnm[ BCT_MAINBACK_ANM_NUM ] = {
		NARC_bucket_maru_wall_n_nsbta,
		NARC_bucket_maru_wall_f_nsbta,
		NARC_bucket_maru_wall_f_nsbtp,
	};

	static const u16 sc_AnmSetModel[ BCT_MAINBACK_ANM_NUM ] = {
		BCT_MAINBACK_MDL_WALL_N,
		BCT_MAINBACK_MDL_WALL_F,
		BCT_MAINBACK_MDL_WALL_F,
	};
	u32 idx;
	int i;
	
	for( i=0; i<BCT_MAINBACK_MDL_NUM; i++ ){

		if( i==BCT_MAINBACK_MDL_WALL_N ){
			idx = NARC_bucket_maru_wall_n_nsbmd;
		}else if( i==BCT_MAINBACK_MDL_WALL_F ){
			idx = NARC_bucket_maru_wall_f_nsbmd;
		}else{
			idx = CommNumNSBMD[ comm_num-1 ];
		}
		
		// ���f���ް��ǂݍ���
		D3DOBJ_MdlLoadH( &p_wk->mdl[ i ], p_handle, idx, heapID );
		// �����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &p_wk->obj[ i ], &p_wk->mdl[ i ] );
		//�@���W�ݒ�
		D3DOBJ_SetMatrix( &p_wk->obj[ i ], 0, BCT_FIELD_YUKA_DRAW_Y, 0 );
		D3DOBJ_SetScale( &p_wk->obj[ i ], BCT_MAINBACK_SCALE, BCT_MAINBACK_SCALE, BCT_MAINBACK_SCALE );
		
		// FEVER�p�̕ǂ�����\��
		if( i == BCT_MAINBACK_MDL_WALL_F ){
			D3DOBJ_SetDraw( &p_wk->obj[ i ], FALSE );
		}

		// �ǂ݂͂�ȋ��ʂ̕\���p�x�Ŏʂ�悤�ɂ���
		if( (i==BCT_MAINBACK_MDL_WALL_N) || (i==BCT_MAINBACK_MDL_WALL_F) ){
			D3DOBJ_SetRota( &p_wk->obj[ i ], BCT_CAMERA_ANGLEY_NetID[ comm_num-1 ][ plno ], D3DOBJ_ROTA_WAY_Y );	// �P���̊p�x
		}else{
			D3DOBJ_SetRota( &p_wk->obj[ i ], BCT_MAINBACK_ROT, D3DOBJ_ROTA_WAY_Y );	// �P���̊p�x
		}
	}

	// �A�j���ǂݍ���
	for( i=0; i<BCT_MAINBACK_ANM_NUM; i++ ){
		D3DOBJ_AnmLoadH( &p_wk->anm[i], &p_wk->mdl[sc_AnmSetModel[i]], p_handle, 
				sc_MainBackAnm[i], heapID, p_allocator );

		// �o�^
		D3DOBJ_AddAnm( &p_wk->obj[sc_AnmSetModel[i]], &p_wk->anm[i] );
	}

	// �A�j���X�s�[�h�ݒ�
	p_wk->anm_speed = BCT_FEVER_BACK_ANM_SPEED_START;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C����ʔw�i�@�\��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_graphic	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainBackExit( BCT_CLIENT_MAINBACK* p_wk, BCT_CLIENT_GRAPHIC* p_graphic, NNSFndAllocator* p_allocator )
{
	int i;

	for( i=0; i<BCT_MAINBACK_ANM_NUM; i++ ){
		D3DOBJ_AnmDelete( &p_wk->anm[i], p_allocator );
	}

	for( i=0; i<BCT_MAINBACK_MDL_NUM; i++ ){
	    D3DOBJ_MdlDelete( &p_wk->mdl[i] ); 
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C����ʔw�i�@�\������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainBackDraw( BCT_CLIENT_MAINBACK* p_wk )
{
	int i;

	// ��Ƀ��[�v�A�j��
	D3DOBJ_AnmLoop( &p_wk->anm[BCT_MAINBACK_ANM_WALL_N], p_wk->anm_speed );

	// ���̏�Ԃɍ��킹�ăA�j��
	if( p_wk->fever ){
		
		switch( p_wk->fever_anm_seq ){
		case 0:
			// �t�B�[�o�[�A�j���́A�r������A�j�������[�v������
			if( (p_wk->fever_anm_frame + p_wk->fever_anm_speed) < BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS ){
				p_wk->fever_anm_frame += p_wk->fever_anm_speed;
			}else{
				p_wk->fever_anm_frame = 0;
			}

			// �A�j���X�s�[�h����
/*			if( sys.trg & PAD_BUTTON_A ){
				p_wk->fever_anm_speed += FX32_HALF;
				OS_TPrintf( "anm_speed 0x%x\n", p_wk->fever_anm_speed );
			}//*/
			if( (p_wk->fever_anm_speed + BCT_MARUNOMU_ANM_FEVERWALLSPEED_ADD) <= BCT_MARUNOMU_ANM_FEVERWALLSPEED_E ){
//				OS_TPrintf( "anm_speed 0x%x\n", p_wk->fever_anm_speed );
				p_wk->fever_anm_speed += BCT_MARUNOMU_ANM_FEVERWALLSPEED_ADD;
			}else{
				p_wk->fever_anm_speed = BCT_MARUNOMU_ANM_FEVERWALLSPEED_E;
				p_wk->fever_anm_wait = 24;
				p_wk->fever_anm_seq ++;
			}
			break;

		case 1:

			// �t�B�[�o�[�A�j���́A�r������A�j�������[�v������
			if( (p_wk->fever_anm_frame + p_wk->fever_anm_speed) < BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS ){
				p_wk->fever_anm_frame += p_wk->fever_anm_speed;
			}else{
				p_wk->fever_anm_frame = 0;
			}

			p_wk->fever_anm_wait --;
			if( p_wk->fever_anm_wait <= 0 ){
				p_wk->fever_anm_seq ++;
				p_wk->fever_anm_frame = BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS;
			}
			break;
			
		case 2:
			// �t�B�[�o�[�A�j���́A�r������A�j�������[�v������
			if( (p_wk->fever_anm_frame + BCT_MARUNOMU_ANM_FEVERWALLSPEED_SPEED) < BCT_MAINBACK_FEVER_ANM_FRAME_NUM ){
				p_wk->fever_anm_frame += BCT_MARUNOMU_ANM_FEVERWALLSPEED_SPEED;
			}else{
				p_wk->fever_anm_frame = BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS;
			}
			break;

		default:
			GF_ASSERT(0);
			break;
		}

		D3DOBJ_AnmSet( &p_wk->anm[BCT_MAINBACK_ANM_WALL_F], p_wk->fever_anm_frame );
		D3DOBJ_AnmSet( &p_wk->anm[BCT_MAINBACK_ANM_WALL_F_TP], p_wk->fever_anm_frame );
	}

	for( i=0; i<BCT_MAINBACK_MDL_NUM; i++ ){
	    D3DOBJ_Draw( &p_wk->obj[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ǂ����̕ǂ�\������̂���ݒ肷��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainBackSetDrawFever( BCT_CLIENT_MAINBACK* p_wk )
{
	// �ǂ�؂�ւ���
	D3DOBJ_SetDraw( &p_wk->obj[BCT_MAINBACK_MDL_WALL_N], FALSE );
	D3DOBJ_SetDraw( &p_wk->obj[BCT_MAINBACK_MDL_WALL_F], TRUE );

//	p_wk->fever_anm_frame = BCT_MAINBACK_FEVER_ANM_FRAME_LOOPS;
	p_wk->fever_anm_frame = 0;
	p_wk->fever_anm_speed = BCT_MARUNOMU_ANM_FEVERWALLSPEED_S;
	p_wk->fever = TRUE;
	p_wk->fever_anm_seq = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���X�s�[�h��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	speed		�X�s�[�h
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_MainBackSetAnmSpeed( BCT_CLIENT_MAINBACK* p_wk, fx32 speed )
{
	p_wk->anm_speed = speed;
}


//----------------------------------------------------------------------------
/**
 *  @brief  ���ʂƐ����̓����蔻��
 *
 *  @param  cp_mat          �����@�J�n���W
 *  @param  cp_move         �����@�ړ��l
 *  @param  cp_n            ���ʁ@�@���x�N�g��
 *  @param  d               ���_����̕��ʂ̋���
 *  @param  p_hitmat        �Ԃ��������W
 *  @param  p_time          �Ԃ��鎞��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CalcPlaneVecHitCheck( const VecFx32* cp_mat, const VecFx32* cp_move, const VecFx32* cp_n, fx32 d, VecFx32* p_hitmat, fx32* p_time )
{
    fx32 time;
    fx32 move_num;
    fx32 mat_num;

    // �@��         (a,b,c)
    // �����n�_ (x0,y0,z0)
    // �����ړ� (xv,yv,zv)
    // �����鎞�� t
    // ���ʂ̈ʒu d
    // t( axv + byv + czv ) = -ax0 - by0 - cz0 + d
    //    move_num              mat_num
    //  t = (mat_num + d) / move_num
    // ���g�p����t�����߂�
    move_num = FX_Mul( cp_n->x, cp_move->x ) + FX_Mul( cp_n->y, cp_move->y ) + FX_Mul( cp_n->z, cp_move->z );
    mat_num = -FX_Mul( cp_n->x, cp_mat->x ) - FX_Mul( cp_n->y, cp_mat->y ) - FX_Mul( cp_n->z, cp_mat->z );
    time = FX_Div( (mat_num + d), move_num );

//  OS_Printf( "move_num 0x%x\n", move_num );
//  OS_Printf( "mat_num 0x%x\n", mat_num );

    // t������W�����߂�
    p_hitmat->x = cp_mat->x + FX_Mul( cp_move->x, time );
    p_hitmat->y = cp_mat->y + FX_Mul( cp_move->y, time );
    p_hitmat->x = cp_mat->z + FX_Mul( cp_move->z, time );

    *p_time = time;


#if 0

#ifdef BCT_DEBUG
    // ���ʊm�F
    {
        fx32 test_d;

        test_d = FX_Mul( cp_n->x, p_hitmat->x ) + FX_Mul( cp_n->y, p_hitmat->y ) + FX_Mul( cp_n->z, p_hitmat->z );

        OS_Printf( "test_d = 0x%x d = 0x%x\n", test_d, d );
    }
#endif
    
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����^�[�Q�b�gY���W��ݒ�
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	y		�����W
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CameraTargetYSet( BCT_CLIENT_GRAPHIC* p_gra, fx32 y )
{
	p_gra->target.y	= y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����A���O������ݒ�
 *
 *	@param	p_gra		�O���t�B�b�N���[�N
 *	@param	angle		�A���O��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_CameraAngleXSet( BCT_CLIENT_GRAPHIC* p_gra, u16 angle )
{
	CAMERA_ANGLE ca_angle;

	ca_angle	= GFC_GetCameraAngle( p_gra->p_camera );
	ca_angle.x	= angle;
	GFC_SetCameraAngleRev( &ca_angle, p_gra->p_camera );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�D�揇��	�X�N���[���V�X�e��	������
 *
 *	@param	p_wk	���[�N
 *	@param	plno	�v���C���[�Ȃ�΁[
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BGPRISCRL_Init( BCT_CLIENT_BGPRI_SCRL* p_wk, u8 plno )
{
	memset( p_wk, 0, sizeof(BCT_CLIENT_BGPRI_SCRL) );
	p_wk->most_back = BCT_BGPRI_SCRL_START_MOSTBACK;
	p_wk->plno		= plno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�D�揇��	�X�N���[���V�X�e��	���C��
 *
 *	@param	p_wk	���[�N
 *	@param	p_gra	�O���t�B�b�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BGPRISCRL_Main( BCT_CLIENT_BGPRI_SCRL* p_wk, BCT_CLIENT_GRAPHIC* p_gra )
{
	switch( p_wk->seq ){
	// �ҋ@���
	case BCT_BGPRI_SCRL_SEQ_NORMAL:
		break;

	// �G�t�F�N�g�J�n
	case BCT_BGPRI_SCRL_SEQ_START:
		// �G�t�F�N�g�������āAmost_back�̏�Ԃɂ���̂�
		// most_back�̏��̂����ЂƂO�̏�Ԃ�BG�D�揇�ʂ�����
		BCT_CLIENT_BGPRISCRL_SetPri( p_gra, p_wk->most_back-1, p_wk->plno );

		// 
		p_wk->seq	++;
		p_wk->count	= 0;
		
	// �G�t�F�N�g���C��
	case BCT_BGPRI_SCRL_SEQ_TOPFADE:
		if( p_wk->count >= BCT_BGPRI_SCRL_COUNT_MAX ){
			// ���ɍ������ޏ�����
			p_wk->seq	= BCT_BGPRI_SCRL_SEQ_BOTTOMFADE;
			p_wk->count = 0;

			// �D�揇�ʂ�ݒ�	�{����V�u�����N�ōs�����ق������ꂢ�ł���
			BCT_CLIENT_BGPRISCRL_SetPri( p_gra, p_wk->most_back, p_wk->plno );
			G2S_SetBlendAlpha( 1<<p_wk->most_back,
					GX_BLEND_PLANEMASK_BG3,
					0, 16 );
		}else{
			p_wk->count ++;

			// �A���t�@�l�ݒ�
			{
				s32 top, two, three;
				s32 alpha;

				top		= p_wk->most_back;
				two		= p_wk->most_back-2;
				three	= p_wk->most_back-1;
				if( top < 0 ){
					top += BCT_BGPRI_SCRL_MOSTBACK_MAX;
				}
				if( two < 0 ){
					two += BCT_BGPRI_SCRL_MOSTBACK_MAX;
				}
				if( three < 0 ){
					three += BCT_BGPRI_SCRL_MOSTBACK_MAX;
				}
				alpha	= 16 - ((p_wk->count * 16) / BCT_BGPRI_SCRL_COUNT_MAX);
				G2S_SetBlendAlpha( 1<<top,
						(1<<two) | (1<<three) | GX_BLEND_PLANEMASK_BG3,
						alpha, 16-alpha );
			}
		}
		break;

		
	// ���ɍ������ޏ���
	case BCT_BGPRI_SCRL_SEQ_BOTTOMFADE:
		if( p_wk->count >= BCT_BGPRI_SCRL_BOTTOMCOUNT_MAX ){
			// �I��
			p_wk->seq	= BCT_BGPRI_SCRL_SEQ_NORMAL;

			G2S_BlendNone();

		}else{
			p_wk->count ++;

			// �A���t�@�l�ݒ�
			{
				s32 alpha;
				alpha	= (p_wk->count * 16) / BCT_BGPRI_SCRL_BOTTOMCOUNT_MAX;
				G2S_SetBlendAlpha( 1<<p_wk->most_back,
						GX_BLEND_PLANEMASK_BG3,
						alpha, 16-alpha );
			}
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�D�揇��	�X�N���[���V�X�e��	���N�G�X�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BGPRISCRL_Req( BCT_CLIENT_BGPRI_SCRL* p_wk )
{
	p_wk->req_num ++;
	if( p_wk->req_num >= BCT_BGPRI_SCRL_REQ_START_NUM ){
		p_wk->seq		= BCT_BGPRI_SCRL_SEQ_START;
		p_wk->most_back = (p_wk->most_back + 1) % BCT_BGPRI_SCRL_MOSTBACK_MAX;
		p_wk->req_num	= 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ԉ���BG�ʂ��w�肵�āA�D�揇�ʂ�ݒ肷��
 *
 *	@param	p_gra		�O���t�B�b�N�V�X�e��
 *	@param	most_back	��ԉ���BG��
 *	@param	plno		�v���C���[�D�揇��
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_BGPRISCRL_SetPri( BCT_CLIENT_GRAPHIC* p_gra, s16 most_back, u16 plno )
{
	int i;
	s32 bgno;

	// �}�C�i�X�̂܂�most_back���킽���Ă���\��������ł����v
	
	for( i=0; i<BCT_BGPRI_SCRL_MOSTBACK_MAX; i++ ){
		bgno = most_back - i;

		// BG�t���[���v�Z
		if( bgno < 0 ){
			bgno += BCT_BGPRI_SCRL_MOSTBACK_MAX;
		}
		bgno += GF_BGL_FRAME0_S;

		
		// �D�揇�ʐݒ�
		GF_BGL_PrioritySet( bgno, (BCT_BGPRI_SCRL_PRI_MAX - i) );

		// �p���b�g�ݒ�
		if( i==0 ){
			// ��ԉ�
			GF_BGL_ScrPalChange( p_gra->p_bgl, bgno, 0, 0, 32, 32, BCT_GRA_BGSUB_PAL_NETID0_BACK+(plno*2) );
		}else{
			// ����ȊO
			GF_BGL_ScrPalChange( p_gra->p_bgl, bgno, 0, 0, 32, 32, BCT_GRA_BGSUB_PAL_NETID0_TOP+(plno*2) );
		}

		GF_BGL_LoadScreenReq( p_gra->p_bgl, bgno );
	}
}




//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_gra		�O���t�B�b�N���[�N
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_Init( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, ARCHANDLE* p_handle, u32 heapID )
{
	// �q�[�vID�ۑ�
	p_wk->heapID	= heapID;

	
	// �e�[�u���ƂȂ�OAM�̓ǂݍ���
	{
		BOOL result;

        // OAM���\�[�X�Ǎ���
        p_wk->resobj[ 0 ] = CLACT_U_ResManagerResAddArcChar_ArcHandle(
                    p_gra->resMan[ 0 ], p_handle,
                    NARC_bucket_counter_NCGR,
                    FALSE, BCT_NUTS_COUNT_DRAW_RESID, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );

        p_wk->resobj[ 1 ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle(
                    p_gra->resMan[ 1 ], p_handle,
                    NARC_bucket_counter_NCLR,
                    FALSE, BCT_NUTS_COUNT_DRAW_RESID, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID );

        p_wk->resobj[ 2 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_gra->resMan[ 2 ], p_handle,
                NARC_bucket_counter_NCER, FALSE,
                BCT_NUTS_COUNT_DRAW_RESID, CLACT_U_CELL_RES, heapID );

        p_wk->resobj[ 3 ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
                p_gra->resMan[ 3 ], p_handle,
                NARC_bucket_counter_NANR, FALSE,
                BCT_NUTS_COUNT_DRAW_RESID, CLACT_U_CELLANM_RES, heapID );

        // �]��
        result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( p_wk->resobj[ 0 ] );
        GF_ASSERT( result );
        result = CLACT_U_PlttManagerSetCleanArea( p_wk->resobj[ 1 ] );
        GF_ASSERT( result );

        // ���\�[�X�����j��
        CLACT_U_ResManagerResOnlyDelete( p_wk->resobj[ 0 ] );
        CLACT_U_ResManagerResOnlyDelete( p_wk->resobj[ 1 ] );

		// �Z���A�N�^�[�w�b�_�[�쐬
		CLACT_U_MakeHeader( &p_wk->header, 
				BCT_NUTS_COUNT_DRAW_RESID, BCT_NUTS_COUNT_DRAW_RESID,
				BCT_NUTS_COUNT_DRAW_RESID, BCT_NUTS_COUNT_DRAW_RESID,
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,0,
				BCT_NUTS_COUNT_DRAW_BGPRI,
				p_gra->resMan[0], p_gra->resMan[1], 
				p_gra->resMan[2], p_gra->resMan[3],
				NULL, NULL );
	}

	// �e�[�u��������OAM�쐬
	{
		CLACT_ADD_SIMPLE add;
		
		add.ClActSet		= p_gra->clactSet;
		add.ClActHeader		= &p_wk->header;
		add.mat.x			= BCT_NUTS_COUNT_INOUT_SX;
		add.mat.y			= BCT_NUTS_COUNT_YURE_SY;
		add.pri				= BCT_NUTS_COUNT_DRAW_SFPRI;
		add.DrawArea		= NNS_G2D_VRAM_TYPE_2DMAIN;
		add.heap			= heapID;

		p_wk->p_tblwk = CLACT_AddSimple( &add );
		CLACT_SetDrawFlag( p_wk->p_tblwk, FALSE );
	}
	
	// �t�H���gOAM�f�[�^�쐬
	{
		int char_size;
		BOOL result;
		FONTOAM_INIT fontoam_init;

		// ������o�b�t�@�쐬
		p_wk->p_str = STRBUF_Create( 16, heapID );
		
		// �r�b�g�}�b�v
		GF_BGL_BmpWinObjAdd( p_gra->p_bgl, 
				&p_wk->objbmp, 
				BCT_NUTS_COUNT_BMP_SIZX, BCT_NUTS_COUNT_BMP_SIZY, 
				0, 0 );

		// OAM�\���f�[�^���쐬
		p_wk->p_fontoam_data	= FONTOAM_OAMDATA_Make( &p_wk->objbmp, heapID );
		char_size				= FONTOAM_OAMDATA_NeedCharSize( p_wk->p_fontoam_data, NNS_G2D_VRAM_TYPE_2DMAIN );

		// �L�����N�^�̈���m��
		result = CharVramAreaAlloc( char_size, CHARM_CONT_AREACONT,
				NNS_G2D_VRAM_TYPE_2DMAIN, &p_wk->fontoam_chardata );
		GF_ASSERT( result == TRUE );

		// �t�H���g�p�p���b�g
		p_wk->p_fontoam_pltt = CLACT_U_ResManagerResAddArcPltt(
                    p_gra->resMan[1], ARC_FONT,
                    NARC_font_system_ncrl,
                    FALSE, BCT_NUTS_COUNT_FONTOAM_PLTT_ID, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID ); 

        result = CLACT_U_PlttManagerSetCleanArea( p_wk->p_fontoam_pltt );
        GF_ASSERT( result );

        // ���\�[�X�����j��
        CLACT_U_ResManagerResOnlyDelete( p_wk->p_fontoam_pltt );


		// FONTOAM���[�N���쐬
		fontoam_init.fontoam_sys	= p_gra->p_fontoam_sys;
		fontoam_init.bmp			= &p_wk->objbmp;
		fontoam_init.clact_set		= p_gra->clactSet;
		fontoam_init.pltt			= CLACT_U_PlttManagerGetProxy( p_wk->p_fontoam_pltt, NULL );
		fontoam_init.parent			= p_wk->p_tblwk;
		fontoam_init.char_ofs		= p_wk->fontoam_chardata.alloc_ofs;
		fontoam_init.x				= BCT_NUTS_COUNT_FONTOAM_X;
		fontoam_init.y				= BCT_NUTS_COUNT_FONTOAM_Y;
		fontoam_init.bg_pri			= BCT_NUTS_COUNT_FONTOAM_BG_PRI;
		fontoam_init.soft_pri		= BCT_NUTS_COUNT_FONTOAM_SF_PRI;
		fontoam_init.draw_area		= NNS_G2D_VRAM_TYPE_2DMAIN;
		fontoam_init.heap			= heapID;
		
		p_wk->p_fontoam				= FONTOAM_OAMDATA_Init( 
				&fontoam_init, p_wk->p_fontoam_data );

		// �\��OFF
		FONTOAM_SetDrawFlag( p_wk->p_fontoam, FALSE );

		// �r�b�g�}�b�v
		GF_BGL_BmpWinDel( &p_wk->objbmp );

	}

	// ����p�����[�^������
	BCT_AddMoveReqFx( &p_wk->inout_data, 
			BCT_NUTS_COUNT_INOUT_SX, BCT_NUTS_COUNT_INOUT_EX,
			BCT_NUTS_COUNT_INOUT_SS, BCT_NUTS_COUNT_INOUT_COUNTMAX );
	BCT_AddMoveReqFx( &p_wk->yure_data, 
			BCT_NUTS_COUNT_YURE_SY, BCT_NUTS_COUNT_YURE_EY,
			BCT_NUTS_COUNT_YURE_SS, BCT_NUTS_COUNT_YURE_COUNTMAX );

	// �J�E���^������
	p_wk->inout_count	= 0;
	p_wk->yure_count	= BCT_NUTS_COUNT_YURE_COUNTMAX;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_gra		�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_Exit( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra )
{
	// �t�H���gOAM�f�[�^�쐬
	{
		
		// FONTOAM���[�N��j��
		FONTOAM_OAMDATA_Delete( p_wk->p_fontoam ); 

		// �p���b�g�j��
        CLACT_U_PlttManagerDelete( p_wk->p_fontoam_pltt );
        CLACT_U_ResManagerResDelete( p_gra->resMan[1], p_wk->p_fontoam_pltt );


		// �L�����N�^�̈��j��
		CharVramAreaFree( &p_wk->fontoam_chardata );

		// OAM�\���f�[�^���쐬
		FONTOAM_OAMDATA_Free( p_wk->p_fontoam_data );
		
		// ������o�b�t�@�j��
		STRBUF_Delete( p_wk->p_str );
	}

	// �e�[�u���j��
	CLACT_Delete( p_wk->p_tblwk );

	// �e�[�u���ƂȂ�OAM�̃��\�[�X�j��
	{
        // VRAM�Ǘ�����j��
        CLACT_U_CharManagerDelete( p_wk->resobj[0] );
        CLACT_U_PlttManagerDelete( p_wk->resobj[1] );
        
        // ���\�[�X�j��
        CLACT_U_ResManagerResDelete( p_gra->resMan[0], p_wk->resobj[0] );
        CLACT_U_ResManagerResDelete( p_gra->resMan[1], p_wk->resobj[1] );
        CLACT_U_ResManagerResDelete( p_gra->resMan[2], p_wk->resobj[2] );
        CLACT_U_ResManagerResDelete( p_gra->resMan[3], p_wk->resobj[3] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	�J�n
 *		
 *	@param	p_wk		���[�N
 *	@param	count		�J�n���̃J�E���g�l
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_Start( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, u32 count )
{
	// ��������������œ]��
	{
		STRBUF_SetNumber( p_wk->p_str, count, 2, 
				NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT );

		// �r�b�g�}�b�v
		GF_BGL_BmpWinObjAdd( p_gra->p_bgl, 
				&p_wk->objbmp, 
				BCT_NUTS_COUNT_BMP_SIZX, BCT_NUTS_COUNT_BMP_SIZY, 
				0, 0 );

		// ��������
		GF_STR_PrintColor( &p_wk->objbmp, FONT_SYSTEM, p_wk->p_str,
				0, 0, MSG_NO_PUT, BCT_COL_OAM_BLACK, NULL );

		// �]��
		FONTOAM_OAMDATA_ResetBmp( p_wk->p_fontoam, p_wk->p_fontoam_data, &p_wk->objbmp, p_wk->heapID );


		// �r�b�g�}�b�v
		GF_BGL_BmpWinDel( &p_wk->objbmp );
	}

	// �`��J�n
	FONTOAM_SetDrawFlag( p_wk->p_fontoam, TRUE );
	CLACT_SetDrawFlag( p_wk->p_tblwk, TRUE );

	// 
	p_wk->seq = BCT_NUTS_COUNT_SEQ_IN;	

	p_wk->out_wait	= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	�I��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_End( BCT_CLIENT_NUTS_COUNT* p_wk )
{
	// �߂点��
	if( p_wk->seq != BCT_NUTS_COUNT_SEQ_OUT ){

		// ���C�����쒆�̂Ƃ��̂݁A������ƕ\�����Ă���
		// �I��������
		if( p_wk->seq == BCT_NUTS_COUNT_SEQ_MAIN ){
			p_wk->out_wait	= BCT_NUTS_COUNT_OUT_WAIT;
		}
		
		p_wk->seq		= BCT_NUTS_COUNT_SEQ_OUT;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	�����X�V
 *	
 *	@param	p_wk		���[�N
 *	@param	count		�J�E���g�l
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_SetData( BCT_CLIENT_NUTS_COUNT* p_wk, BCT_CLIENT_GRAPHIC* p_gra, u32 count )
{
	// �ގ����łȂ���ΐݒ�
	if( p_wk->seq != BCT_NUTS_COUNT_SEQ_OUT ){
		
		// �������X�V
		{
			STRBUF_SetNumber( p_wk->p_str, count, 2, 
					NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT );

			// �r�b�g�}�b�v
			GF_BGL_BmpWinObjAdd( p_gra->p_bgl, 
					&p_wk->objbmp, 
					BCT_NUTS_COUNT_BMP_SIZX, BCT_NUTS_COUNT_BMP_SIZY, 
					0, 0 );

			// ��������
			GF_STR_PrintColor( &p_wk->objbmp, FONT_SYSTEM, p_wk->p_str,
					0, 0, MSG_NO_PUT, BCT_COL_OAM_BLACK, NULL );

			// �]��
			FONTOAM_OAMDATA_ResetBmp( p_wk->p_fontoam, p_wk->p_fontoam_data, &p_wk->objbmp, p_wk->heapID );

			// �r�b�g�}�b�v
			GF_BGL_BmpWinDel( &p_wk->objbmp );
		}

		// ���A�j���J�n
		p_wk->yure_count = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��J�E���^�[	���C��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_NUTS_COUNT_Main( BCT_CLIENT_NUTS_COUNT* p_wk )
{
	switch( p_wk->seq ){
	// �ҋ@���
	case BCT_NUTS_COUNT_SEQ_WAIT:
		break;
	// �����Ă��铮��
	case BCT_NUTS_COUNT_SEQ_IN:
		if( p_wk->inout_count < BCT_NUTS_COUNT_INOUT_COUNTMAX ){
			p_wk->inout_count ++;
		}else{
			p_wk->seq = BCT_NUTS_COUNT_SEQ_MAIN;
		}
		break;
	// ���C��
	case BCT_NUTS_COUNT_SEQ_MAIN:
		break;
	// �ގ�����
	case BCT_NUTS_COUNT_SEQ_OUT:
		if( p_wk->out_wait > 0 ){
			p_wk->out_wait --;
			break;
		}

		if( p_wk->inout_count > 0 ){
			p_wk->inout_count --;
		}else{
			p_wk->seq = BCT_NUTS_COUNT_SEQ_WAIT;
			// �\��OFF
			FONTOAM_SetDrawFlag( p_wk->p_fontoam, FALSE );
			CLACT_SetDrawFlag( p_wk->p_tblwk, FALSE );
		}
		break;
	}

	// ��ɓ�������
	
	// ���Ă�ꓮ��
	if( p_wk->yure_count < BCT_NUTS_COUNT_YURE_COUNTMAX ){	
		p_wk->yure_count ++;
		p_wk->yure_data.count = p_wk->yure_count;
		// �v�Z
		BCT_AddMoveMainFx( &p_wk->yure_data );
	}

	// ���h��
	{
		p_wk->inout_data.count = p_wk->inout_count;
		// �v�Z
		BCT_AddMoveMainFx( &p_wk->inout_data );
	}


	// ���W�ݒ�
	{
		VecFx32 matrix;
		matrix.x = p_wk->inout_data.x;
		matrix.y = p_wk->yure_data.x;
		
		CLACT_SetMatrix( p_wk->p_tblwk, &matrix );
		FONTOAM_ReflectParentMat( p_wk->p_fontoam );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���b�V���G�t�F�N�g�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_FEVER_EFF_Start( BCT_CLIENT_FEVER_EFF_WK* p_wk )
{
	p_wk->seq	= 0;
	p_wk->move	= TRUE;
	p_wk->wait	= 0; 
	Snd_SePlay( BCT_SND_FEVER_CHIME );// FEVER�`���C��

}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���b�V���G�t�F�N�g���C��
 *
 *	@param	p_wk			���[�N
 *	@param	p_mainback		�w�i�O���t�B�b�N
 *	@param	p_marunomudraw	�}���m�[���`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_FEVER_EFF_Main( BCT_CLIENT_FEVER_EFF_WK* p_wk, BCT_CLIENT_MAINBACK* p_mainback, BCT_MARUNOMU_DRAW* p_marunomudraw )
{
	fx32 speed;
	u32 tempo;
	
	// ���쒆����Ȃ�������Ȃ�����Ȃ�
	if( p_wk->move == FALSE ){
		return ;
	}
	
	switch( p_wk->seq ){
	// �G�t�F�N�g�J�n�E�G�C�g
	case BCT_FEVER_EFF_SEQ_BGM_WAIT:
		p_wk->wait ++;

		// �w�i�A�j���X�s�[�h��ύX
		speed = (p_wk->wait*BCT_FEVER_BACK_ANM_SPEED_DIF) / BCT_FEVER_EFF_FLASHOUT_FLASHOUT_WAIT;
		speed += BCT_FEVER_BACK_ANM_SPEED_START;
		BCT_CLIENT_MainBackSetAnmSpeed( p_mainback, speed );

		// BGM�̃e���|�A�b�v
		tempo = (p_wk->wait*BCT_FEVER_EFF_BGM_TEMPO_DIF) / BCT_FEVER_EFF_FLASHOUT_FLASHOUT_WAIT;
		tempo += BCT_FEVER_EFF_BGM_TEMPO_START;
		Snd_PlayerSetTempoRatio( SND_HANDLE_BGM, tempo );

		// �}���m�[����������
		if( BCT_FEVER_EFF_SE_MARUNOMUMOVE == p_wk->wait ){
			Snd_SePlay( BCT_SND_FEVER_MOVE );
		}

		
		if( p_wk->wait >= BCT_FEVER_EFF_FLASHOUT_FLASHOUT_WAIT ){
			p_wk->seq ++;
		}
		break;
	// �t���b�V���A�E�g
	case BCT_FEVER_EFF_SEQ_BGM_FLASH_OUT:
		ChangeBrightnessRequest( BCT_FEVER_EFF_FLASHOUT_FLASHOUT_SYNC,
				BRIGHTNESS_WHITE, BRIGHTNESS_NORMAL, 
				PLANEMASK_ALL, MASK_MAIN_DISPLAY );
		p_wk->seq ++;
		break;
	case BCT_FEVER_EFF_SEQ_BGM_FLASH_OUTWAIT:
		if( IsFinishedBrightnessChg( MASK_MAIN_DISPLAY ) == TRUE ){

			// �w�i�ύX
			BCT_CLIENT_MainBackSetDrawFever( p_mainback );
			BCT_CLIENT_MainBackSetAnmSpeed( p_mainback, BCT_MARUNOMU_ANM_SPEED ); 
			// FEVER�ɂȂ����烉���_���\��
			BCT_CLIENT_MarunomuDrawSetColAnmRand( p_marunomudraw, TRUE );
			p_wk->seq ++;
		}
		break;

	// �t���b�V���C��
	case BCT_FEVER_EFF_SEQ_BGM_FLASH_IN:
		ChangeBrightnessRequest( BCT_FEVER_EFF_FLASHOUT_FLASHIN_SYNC,
				BRIGHTNESS_NORMAL, BRIGHTNESS_WHITE, 
				PLANEMASK_ALL, MASK_MAIN_DISPLAY );
		p_wk->seq ++;
		break;
	case BCT_FEVER_EFF_SEQ_BGM_FLASH_INWAIT:
		if( IsFinishedBrightnessChg( MASK_MAIN_DISPLAY ) == TRUE ){
			p_wk->seq ++;
		}
		break;

	// �I��
	case BCT_FEVER_EFF_SEQ_BGM_FLASH_END:
		p_wk->move	= FALSE;
		p_wk->seq	= 0;
		break;

	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	FEVER�G�t�F�N�g	�̂������Ă����Ԃ����Z�b�g	�iBGM�̃e���|�j
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_CLIENT_FEVER_EFF_Reset( BCT_CLIENT_FEVER_EFF_WK* p_wk )
{
	Snd_PlayerSetTempoRatio( SND_HANDLE_BGM, 256 );
}





#ifdef BCT_DEBUG_HITZONE

static void BCT_DEBUG_PositionInit( ARCHANDLE* p_handle, u32 heapID )
{
    int i;
    
    D3DOBJ_MdlLoadH( &BCT_DPD.mdl, p_handle, NARC_bucket_kinomi_1p_nsbmd, heapID );

    for( i=0; i<4; i++ ){
        D3DOBJ_Init( &BCT_DPD.obj[i], &BCT_DPD.mdl );
    }
}

#define BCT_DEBUG_HITZONE_MOUTH		// ���̃G���A�\��
#define BCT_DEBUG_HITZONE_BODY		// �{�f�B�G���A�\��
//#define BCT_DEBUG_HITZONE_DITCH		// �a�G���A�\��

static void BCT_DEBUG_PositionDraw( const BCT_CLIENT* cp_client )
{
    int i, j;
    MtxFx43 mtx;
    MtxFx43 tmp;
    MtxFx33 workmtx;
    VecFx32 matrix = { 0,0,0 };
	VecFx32 setmat;
	VecFx32 testmat;
	fx32 r;
	s32 y_dis;

    MTX_Identity33( &workmtx );


#ifdef BCT_DEBUG_HITZONE_MOUTH
    for( i=0; i<4; i++ ){
        NNS_G3dGePushMtx();

        // �ʒu�ݒ�
        NNS_G3dGlbSetBaseTrans(&matrix);
        // �p�x�ݒ�
        NNS_G3dGlbSetBaseRot(&workmtx);
        // �X�P�[���ݒ�
        NNS_G3dGlbSetBaseScale(&BCT_DPD.obj[i].scale);

        NNS_G3dGlbFlush();

        MTX_Identity43( &mtx );

		// 4���_���쐬����
		switch( i ){
		case 0:
			setmat.x = -cp_client->marunomu.mouth_size;
			setmat.z = -FX_Mul( cp_client->marunomu.mouth_size, FX32_CONST( 2 ) );
			setmat.y = 0;
			break;
		case 1:
			setmat.x = -cp_client->marunomu.mouth_size;
			setmat.z = 0;
			setmat.y = 0;
			break;
		case 2:
			setmat.x = cp_client->marunomu.mouth_size;
			setmat.z = -FX_Mul( cp_client->marunomu.mouth_size, FX32_CONST( 2 ) );
			setmat.y = 0;
			break;
		case 3:
			setmat.x = cp_client->marunomu.mouth_size;
			setmat.z = 0;
			setmat.y = 0;
			break;
		}
		

		// �����ŉ�]������
		MTX_MultVec33( &setmat, &cp_client->marunomu.rotaxy_mtx, &setmat );

		// �\���ʒu�Ɉړ�����
		testmat.x = BCT_FIELD_MARUNOMU_X;
		testmat.y = cp_client->marunomu.hit_y;
		testmat.z = cp_client->marunomu.marunomu_mat.z + BCT_MARUNOMU_HIT_DIS_MAX;
		MTX_MultVec33( &testmat, &cp_client->marunomu.rotay_mtx, &testmat );

		setmat.x += testmat.x;
		setmat.y += testmat.y;
		setmat.z += testmat.z;
		

        // �ړ�
        NNS_G3dGeTranslateVec( &setmat );
        NNS_G3dDraw(&BCT_DPD.obj[i].render);
    
        NNS_G3dGePopMtx(1);
    }

	// �Z���^�[���W��\��
	{
        NNS_G3dGePushMtx();

        // �ʒu�ݒ�
        NNS_G3dGlbSetBaseTrans(&matrix);
        // �p�x�ݒ�
        NNS_G3dGlbSetBaseRot(&workmtx);
        // �X�P�[���ݒ�
        NNS_G3dGlbSetBaseScale(&BCT_DPD.obj[0].scale);

        NNS_G3dGlbFlush();

        MTX_Identity43( &mtx );

        // �ړ�
        NNS_G3dGeTranslateVec( &cp_client->marunomu.center_mat );
        NNS_G3dDraw(&BCT_DPD.obj[0].render);
    
        NNS_G3dGePopMtx(1);
	}
#endif

#ifdef BCT_DEBUG_HITZONE_BODY
	// �{�f�B�����蔻����\��
	for( i=0; i<2; i++ ){

		if( i==0 ){
			setmat.y = cp_client->marunomu.matrix.y;
		}else{
			setmat.y = cp_client->marunomu.matrix.y + cp_client->marunomu.height;
		}

		for( j=0; j<4; j++ ){

			NNS_G3dGePushMtx();

			// �ʒu�ݒ�
			NNS_G3dGlbSetBaseTrans(&matrix);
			// �p�x�ݒ�
			NNS_G3dGlbSetBaseRot(&workmtx);
			// �X�P�[���ݒ�
			NNS_G3dGlbSetBaseScale(&BCT_DPD.obj[i].scale);

			NNS_G3dGlbFlush();

			MTX_Identity43( &mtx );

			// �����蔻�蔼�a�����߂�
			y_dis = setmat.y - cp_client->marunomu.marunomu_mat.y;
			y_dis = y_dis>>FX32_SHIFT;
			r = ( y_dis*cp_client->marunomu.hitbody_r_dis ) / (cp_client->marunomu.height >> FX32_SHIFT);
			r = cp_client->marunomu.hitbody_r_max - r;

			// 4���_���쐬����
			switch( j ){
			case 0:
				setmat.x = cp_client->marunomu.marunomu_mat.x - r;
				setmat.z = cp_client->marunomu.marunomu_mat.z - r;
				break;
			case 1:
				setmat.x = cp_client->marunomu.marunomu_mat.x - r;
				setmat.z = cp_client->marunomu.marunomu_mat.z + r;
				break;
			case 2:
				setmat.x = cp_client->marunomu.marunomu_mat.x + r;
				setmat.z = cp_client->marunomu.marunomu_mat.z - r;
				break;
			case 3:
				setmat.x = cp_client->marunomu.marunomu_mat.x + r;
				setmat.z = cp_client->marunomu.marunomu_mat.z + r;
				break;
			}
			
			// �\���ʒu�Ɉړ�����
			MTX_MultVec33( &setmat, &cp_client->marunomu.rotay_mtx, &setmat );

			// �ړ�
			NNS_G3dGeTranslateVec( &setmat );
			NNS_G3dDraw(&BCT_DPD.obj[i].render);
		
			NNS_G3dGePopMtx(1);

		}
	}
#endif

#ifdef BCT_DEBUG_HITZONE_DITCH
	// �a�̈ʒu���\��
    for( i=0; i<8; i++ ){
        NNS_G3dGePushMtx();

        // �ʒu�ݒ�
        NNS_G3dGlbSetBaseTrans(&matrix);
        // �p�x�ݒ�
        NNS_G3dGlbSetBaseRot(&workmtx);
        // �X�P�[���ݒ�
        NNS_G3dGlbSetBaseScale(&BCT_DPD.obj[i%4].scale);

        NNS_G3dGlbFlush();

        MTX_Identity43( &mtx );

		// 4���_���쐬����
		switch( i ){
		case 0:
			setmat.x = BCT_CAMERA_TARGET_X - BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.z = BCT_CAMERA_TARGET_Z - BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 1:
			setmat.x = BCT_CAMERA_TARGET_X - BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.z = BCT_CAMERA_TARGET_Z + BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 2:
			setmat.x = BCT_CAMERA_TARGET_X + BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.z = BCT_CAMERA_TARGET_Z + BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 3:
			setmat.x = BCT_CAMERA_TARGET_X + BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.z = BCT_CAMERA_TARGET_Z - BCT_NUTS_AWAY_DITCH_MIN_R;
			setmat.y = BCT_FIELD_MIN;
			break;

		case 4:
			setmat.x = BCT_CAMERA_TARGET_X - BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.z = BCT_CAMERA_TARGET_Z - BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 5:
			setmat.x = BCT_CAMERA_TARGET_X - BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.z = BCT_CAMERA_TARGET_Z + BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 6:
			setmat.x = BCT_CAMERA_TARGET_X + BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.z = BCT_CAMERA_TARGET_Z + BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		case 7:
			setmat.x = BCT_CAMERA_TARGET_X + BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.z = BCT_CAMERA_TARGET_Z - BCT_NUTS_AWAY_DITCH_MAX_R;
			setmat.y = BCT_FIELD_MIN;
			break;
		}
		
        // �ړ�
        NNS_G3dGeTranslateVec( &setmat );
        NNS_G3dDraw(&BCT_DPD.obj[i%4].render);
    
        NNS_G3dGePopMtx(1);
    }
#endif
}

static void BCT_DEBUG_PositionExit( void )
{
    D3DOBJ_MdlDelete( &BCT_DPD.mdl );
}

#endif

#ifdef BCT_DEBUG_AUTOSLOW

//----------------------------------------------------------------------------
/**
 *	@brief	��������
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
#define BCT_NUTS_SLOW_RENSYA_TIME_AUTOSLOW	( 18 )	// �ŏ������ˑ҂�����
static void BCT_DEBUG_AutoSlow( BCT_CLIENT* p_wk )
{
	// �ʔ��ˊԊu�J�E���g
	p_wk->slow.time += 1+(gf_mtRand()%4);

	// ���ˊԊu
	if( p_wk->slow.time < BCT_NUTS_SLOW_RENSYA_TIME_AUTOSLOW ){
		return ;
	}

	// �؂̎�����
	BCT_EasyNutsSet( p_wk, 128, 96, 128, 66, BCT_NUTSSEQ_MOVE );

	// ���̋����ˊԊu�^�C�}�[���Z�b�g
	p_wk->slow.time = 0;
}

#endif
