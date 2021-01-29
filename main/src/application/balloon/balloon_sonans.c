//==============================================================================
/**
 * @file	balloon_sonans.c
 * @brief	���D����Q�[���̃\�[�i���X���䃋�[�`��
 * @author	matsuda
 * @date	2007.11.13(��)
 */
//==============================================================================
#include "common.h"
#include "system/clact_tool.h"
#include "system/palanm.h"
#include "system/pmfprint.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/particle.h"
#include "system/brightness.h"
#include "system/snd_tool.h"
#include "communication/communication.h"
#include "communication/wm_icon.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "communication/wm_icon.h"
#include "system/msgdata_util.h"
#include "system/procsys.h"
#include "system/d3dobj.h"
#include "system/fontoam.h"

#include "balloon_common.h"
#include "balloon_comm_types.h"
#include "balloon_game_types.h"
#include "application/balloon.h"
#include "balloon_game.h"
#include "balloon_tcb_pri.h"
#include "balloon_sonans.h"
#include "balloon_send_recv.h"
#include "balloon_control.h"
#include "balloon_tool.h"
#include "balloon_snd_def.h"

#include "graphic/balloon_gra_def.h"
#include "balloon_id.h"
#include "../../particledata/pl_etc/pl_etc_particle_def.h"
#include "../../particledata/pl_etc/pl_etc_particle_lst.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�\�[�i���X��Y�����ɘA�Ȃ�A���l�p�`�|���S���̐�
#define SONANS_QUAD_STRIP_NUM		(8)
///�\�[�i���X�ɘA�Ȃ��Ă���A�����܂ł̃|���S�����ϑΏۂɂȂ邩(�c��͓����Ȃ��Ȃ�)
#define SONANS_VARIABLE_VERTEX_NUM	(SONANS_QUAD_STRIP_NUM - 1)

///�\�[�i���X�̒��_���W���X�g�̒���(fx32)
#define SONANS_VERTEX_LEN			(128 * FX32_ONE)
///�\�[�i���X�̒��_���W���X�g�̒���(fx32)
#define SONANS_VERTEX_LEN_HALF		(SONANS_VERTEX_LEN/2)
///�\�[�i���X�̉ϑΏۃ|���S���̒��_���W���X�g��
#define SONANS_VARIABLE_VERTEX_LEN	(SONANS_VERTEX_LEN / SONANS_QUAD_STRIP_NUM * SONANS_VARIABLE_VERTEX_NUM)
///�\�[�i���X�ׂ̒�钷��
#define SONANS_VERTEX_PUSH_LEN		(SONANS_VARIABLE_VERTEX_LEN - 12*FX32_ONE)

///�\�[�i���X�̍��[���_���WX
#define SONANS_VERTEX_LEFT			(0)
///�\�[�i���X�̉E�[���_���WX
#define SONANS_VERTEX_RIGHT			(SONANS_VERTEX_LEN)
///�\�[�i���X�̉��[���_���WX
#define SONANS_VERTEX_TOP			(0)
///�\�[�i���X�̏�[���_���WX
#define SONANS_VERTEX_BOTTOM		(-SONANS_VERTEX_LEN)

///�\�[�i���X�̃e�N�X�`���T�C�Y:X
#define SONANS_TEX_SIZE_X			(128)
///�\�[�i���X�̃e�N�X�`���T�C�Y:Y
#define SONANS_TEX_SIZE_Y			(128)

///�\�[�i���X�̃X�P�[���T�C�YX�@�����_��fx16�ׁ̈A���_��FX16_ONE�ɂ��Ă����X�P�[���ł��̕��g�傷��
#define SONANS_SCALE_SIZE_X			(6 * 16*FX32_ONE)	//6grid(1grid=16unit)
///�\�[�i���X�̃X�P�[���T�C�YY
#define SONANS_SCALE_SIZE_Y			(6 * 16*FX32_ONE)
///�\�[�i���X�̃X�P�[���T�C�YZ
#define SONANS_SCALE_SIZE_Z			(FX32_ONE)

///�\�[�i���X�̃X�P�[���T�C�YX(���A�F��)
#define SONANS_SCALE_SIZE_FINE_X		(SONANS_SCALE_SIZE_X - 2*16*FX32_ONE)
///�\�[�i���X�̃X�P�[���T�C�YY(���A�F��)
#define SONANS_SCALE_SIZE_FINE_Y		(SONANS_SCALE_SIZE_Y + 1*16*FX32_ONE/2)
///�\�[�i���X�̃X�P�[���T�C�YX(���A�F��)
#define SONANS_SCALE_SIZE_THICK_X		(SONANS_SCALE_SIZE_X + 3*16*FX32_ONE)
///�\�[�i���X�̃X�P�[���T�C�YY(���A�F��)
#define SONANS_SCALE_SIZE_THICK_Y		(SONANS_SCALE_SIZE_Y + 1*16*FX32_ONE)

///�\�[�i���X�̏Ə��̏������WX(2D�I�ȃh�b�g�ʒu)
#define SONANS_AIM_INIT_X		(128)
///�\�[�i���X�̏Ə��̏������WY(2D�I�ȃh�b�g�ʒu)
#define SONANS_AIM_INIT_Y		(40)
///�\�[�i���X�̏Ə��̏I�[���WY(2D�I�ȃh�b�g�ʒu)
#define SONANS_AIM_END_Y		(256-48)

///�\�[�i���X�̃e�N�X�`���O���t�B�b�N�̑����̋󔒃T�C�Y(���j�b�g�P��)
#define SONANS_TEX_SPACE_SIZE	(9)
///����̏c�T�C�Y(���j�b�g�P��)
#define DAIZA_UNIT_SIZE			(10)

///�Ə��̓����蔻��X(���S����̔��a)
#define AIM_RANGE_HALF_X		(56)
///�Ə��̓����蔻��Y(���S����̔��a)
#define AIM_RANGE_HALF_Y		(16)
///�Ə��̒��S�������Ă���A�Ƃ�����������锼�a
#define AIM_CENTER_RANGE_HALF_X		(16)	//(AIM_RANGE_HALF_X / 2)

///�Ə��Ƀq�b�g�������̈ʒu
enum{
	AIM_HIT_CENTER,			///<�Ə��̒��S�Ƀq�b�g
	AIM_HIT_LEFT,			///<�Ə��̍����Ƀq�b�g
	AIM_HIT_RIGHT,			///<�Ə��̉E���Ƀq�b�g
};

///�\�[�i���X�ő厞�̖ʐ�
#define SONANS_MAX_AREA		(((((SONANS_VERTEX_RIGHT-SONANS_VERTEX_LEFT) + (SONANS_VERTEX_RIGHT-SONANS_VERTEX_LEFT)) * (fx64)(SONANS_VERTEX_PUSH_LEN)) >> FX32_SHIFT) / 2)

///�{���e�[�W���ő�̎��A�\�[�i���X���ő剽���܂ő傫�����邩(100��2�{):X
#define SONANS_VOLTAGE_SCALE_X_MAX	(30)
///�{���e�[�W���ő�̎��A�\�[�i���X��1�t���[�����Ƃɉ������傫�����邩:X
#define SONANS_VOLTAGE_SCALE_ADD_X	(8)


///�{���e�[�W���ő�̎��̓_�ŃA�j���̃E�F�C�g�Ԋu
#define VOLTAGE_ANM_FLASH_WAIT			(4)

//==============================================================================
//	�\���̒�`
//==============================================================================
///fx16��POINT�^
typedef struct{
	fx16 x;
	fx16 y;
}POINT_FX16;

///fx32��POINT�^
typedef struct{
	fx32 x;
	fx32 y;
}POINT_FX32;

///s32��POINT�^
typedef struct{
	s32 x;
	s32 y;
}POINT_S32;

///���_���W���X�g
typedef struct{
	POINT_FX32 lu;			///<���_���W�F����
	POINT_FX32 ld;			///<���_���W�F����
	POINT_FX32 ru;			///<���_���W�F�E��
	POINT_FX32 rd;			///<���_���W�F�E��
}VERTEX_LIST;

///�e�N�X�`�����_���W���X�g
typedef struct{
	POINT_FX32 lu;			///<���_���W�F����
	POINT_FX32 ld;			///<���_���W�F����
	POINT_FX32 ru;			///<���_���W�F�E��
	POINT_FX32 rd;			///<���_���W�F�E��
}TEX_COORD_LIST;

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X���쐧��p�V�X�e�����[�N
 */
//--------------------------------------------------------------
typedef struct _SONANS_SYS{
	NNSG3dResFileHeader *tex_file;			///<�ǂݍ��񂾃e�N�X�`���[�t�@�C���ւ̃|�C���^
	NNSG3dResTex	*texture;
	u16 rotate;
	VERTEX_LIST vertex_list[SONANS_QUAD_STRIP_NUM];
	TEX_COORD_LIST tex_coord_list[SONANS_QUAD_STRIP_NUM];
	
	int tp_x;
	int tp_y;
	int tp_cont;
	int tp_trg;
	
	int aim_x;
	int aim_y;
	int aim_hit_x;
	int aim_hit_y;
	int aim_hit_y_zure;				///<�Ə��̒��S���炢������ă^�b�`��������ێ�(Y���W)
	fx32 aim_hit_center_y_fx;
	CATS_ACT_PTR aim_cap;			///<�Ə��A�N�^�[�ւ̃|�C���^
	
	int push_mode;
	int backup_tp_y;
	
	int before_area;				///<�^�b�`�J�n���̖ʐ�
	int pomp_area;					///<�^�b�`�𗣂������ɓ����A�ŏI�I�ɉ������񂾖ʐ�
	
	fx32 return_power;				///<�\�[�i���X��߂����̃p���[�l

	u16 *palette_work_src;			///<�\�[�i���X�̃p���b�g���[�N(���f�[�^)
	u16 *palette_work_dest;			///<�\�[�i���X�̃p���b�g���[�N(�]���f�[�^)
	u32 palette_size;				///<�\�[�i���X�̃J���[�p���b�g�T�C�Y(byte)
	u32 palette_trans_adrs;			///<�p���b�g�]����̃A�h���X
	
	s32 voltage;					///<�\�[�i���X�̃{���e�[�W�l
	
	fx32 scale_x;
	fx32 scale_y;
	s16 scale_voltage_x;
	s16 scale_voltage_y;
	
	s16 rare_scale_x;				///<���A�Q�[���ɂ��X�P�[���l:X
	s16 rare_scale_y;				///<���A�Q�[���ɂ��X�P�[���l:Y
	
	MNGM_RAREGAME_TYPE rare_type;	///<���A�^�C�v
	
	u8 snd_push_se;					///<TRUE:�\�[�i���X�������Ă���SE�Đ���
	u8 voltage_anm_wait;			///<�{���e�[�W�ő�̎��̃A�j���E�F�C�g
	u8 color_trans_req;				///<TRUE:V�u�����N�Ńp���b�g�]�����N�G�X�g
	
#ifdef PM_DEBUG
	int debug_aim_on_off;			///<TRUE�F�Ə��\���@�@FALSE:�Ə���\��
#endif
}SONANS_SYS;


//==============================================================================
//	�^��`
//==============================================================================
///Sonans_VertexCalc�֐��̖߂�l
typedef enum{
	VERTEX_CALC_RET_TRUE,			///<����I��
	VERTEX_CALC_RET_RELEASE,		///<�y���̃^�b�`��Ԃ����������K�v������
	VERTEX_CALC_RET_CARRY_OVER,		///<����A�����v�Z�ŕ␳����������
}VERTEX_CALC_RET;


//==============================================================================
//	�f�[�^
//==============================================================================
static const fx16	 gCubeGeometryFx[] = {
	-FX16_HALF, FX16_HALF, 0,
	-FX16_HALF, -FX16_HALF, 0,
	FX16_HALF, FX16_HALF, 0,
	FX16_HALF, -FX16_HALF, 0,

	(FX16_ONE + FX16_ONE*2), FX16_ONE, 0,
	(FX16_ONE + FX16_ONE*2), -FX16_ONE, 0,

	(FX16_ONE + FX16_ONE*4), FX16_ONE, 0,
	(FX16_ONE + FX16_ONE*4), -FX16_ONE, 0,


	FX16_ONE, FX16_ONE, 0,
	FX16_ONE, FX16_ONE*2, 0,
	-FX16_ONE, FX16_ONE, 0,
	-FX16_ONE, FX16_ONE*2, 0,
};

static const fx16 gCubeNormalFx[3 * 6] = {
	0, 0, FX16_ONE - 1,
	0, FX16_ONE - 1, 0,
	FX16_ONE - 1, 0, 0,
	0, 0, -FX16_ONE + 1,
	0, -FX16_ONE + 1, 0,
	-FX16_ONE + 1, 0, 0,
};

static const fx32 gCubeTexCoordFx[] = {	//s, t
	0, 0,
	0, 128*FX32_ONE,
	128*FX32_ONE, 0,
	128*FX32_ONE, 128*FX32_ONE,

	64*FX32_ONE, 0,
	64*FX32_ONE, 128*FX32_ONE,
	96*FX32_ONE, 0,
	96*FX32_ONE, 128*FX32_ONE,
};


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�Ə��̃A�N�^�[�w�b�_(���C����ʗp)
static const TCATS_OBJECT_ADD_PARAM_S AimObjParam = {
	0, 0, 0,		//x, y, z
	0, BALLOON_SOFTPRI_AIM, 0,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_BALLOON_AIM,			//�L����
		PLTTID_OBJ_AIM,			//�p���b�g
		CELLID_BALLOON_AIM,			//�Z��
		CELLANMID_BALLOON_AIM,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	BALLOON_BGPRI_AIM,				//BG�v���C�I���e�B
	0,									//Vram�]���t���O
};


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�\�[�i���X�̂����蔻��
static const struct{
	u8 left;
	u8 right;
	u8 top;
	u8 bottom;
}SonansHitRect[] = {	//�m�[�}���A���A�P�A���A�Q
	{64, 192, 16, 168},		//MNGM_RAREGAME_BALLOON_NORMAL
	{40, 256-40, 0, 168},		//MNGM_RAREGAME_BALLOON_THICK
	{84, 256-84, 8, 168},		//MNGM_RAREGAME_BALLOON_FINE
};

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void Sonas_TexLoad(SONANS_SYS *sns);
static void Sonans_VertexListInit(SONANS_SYS_PTR sns);
static void Sonans_TexCoordListInit(SONANS_SYS_PTR sns);
static void Sonans_VertexListUpdate(SONANS_SYS_PTR sns);
static void Sonans_PolygonCreate(SONANS_SYS_PTR sns);
static BOOL Sonans_AimHitCheck(SONANS_SYS_PTR sns);
static int Sonans_VertexUpdate(SONANS_SYS_PTR sns);
static VERTEX_CALC_RET Sonans_VertexCalc(SONANS_SYS_PTR sns, int air_back_mode, fx32 dot_offset);
static int Sonans_AimHitPosGet(SONANS_SYS_PTR sns, int air_back_mode);
static int VertexCalc_TouchPower(SONANS_SYS_PTR sns, fx32 dot_offset, 
	fx32 now_y, fx32 now_reverse_y, fx32 *new_y, fx32 *new_reverse_y);
static fx32 Sonans_AreaGet(SONANS_SYS_PTR sns);
static int Sonans_NowAreaParcentGet(SONANS_SYS_PTR sns);
static void Sonans_Vertex_FX32_to_Conv(fx32 x, fx32 y, fx16 *ret_x, fx16 *ret_y);
static void Sonans_PushModeReset(SONANS_SYS_PTR sns);
static int Sonans_PompArea_to_AirPower(SONANS_SYS_PTR sns, int pomp_area);
static void Sonans_ReturnPowerCalc(SONANS_SYS_PTR sns, int pomp_area);
static void Sonans_PaletteTrans(SONANS_SYS_PTR sns);
static void Sonans_VoltagePaletteUpdate(SONANS_SYS_PTR sns, int voltage);
static BOOL Sonans_VoltageAdd(SONANS_SYS_PTR sns, int pomp_area);
static void Sonans_VoltageReset(SONANS_SYS_PTR sns);
static void Sonans_VoltageMaxAnime(SONANS_SYS_PTR sns);
static void Sonans_ParticleSet(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns, int pomp_area, int voltage_max);

#define DEBUG_MAT
#ifdef DEBUG_MAT
void DEMO_Set3DDefaultMaterial(BOOL bUsediffuseAsVtxCol, BOOL bUseShininessTbl);
void DEMO_Set3DDefaultShininessTable();
#endif

static void Aim_ResourceLoad(BALLOON_GAME_PTR game);
static void Aim_ResourceFree(BALLOON_GAME_PTR game);
static CATS_ACT_PTR Aim_ActorCreate(BALLOON_GAME_PTR game);
static void Aim_Update(SONANS_SYS_PTR sns, CATS_ACT_PTR cap);
static void Aim_ActorDelete(CATS_ACT_PTR cap);


//==============================================================================
//	�O���f�[�^
//==============================================================================
extern const s32 AirRareRevision[];


//==============================================================================
//
//	�C�����C���֐�
//
//==============================================================================
inline void vtx(int idx)
{
	NNS_G3dGeVtx(gCubeGeometryFx[idx * 3], 
		gCubeGeometryFx[idx * 3 + 1], gCubeGeometryFx[idx * 3 + 2]);
}

inline void normal(int idx)
{
	NNS_G3dGeNormal(gCubeNormalFx[idx * 3], gCubeNormalFx[idx * 3 + 1],gCubeNormalFx[idx * 3 + 2]);
}

inline void tex_coord(int idx)
{
	NNS_G3dGeTexCoord(gCubeTexCoordFx[idx * 2], gCubeTexCoordFx[idx * 2 + 1]);
}


//==============================================================================
//
//	���C��
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief	�\�[�i���X�֘A������
 *
 * @param	sns		�\�[�i���X���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
SONANS_SYS_PTR Sonas_Init(BALLOON_GAME_PTR game)
{
	SONANS_SYS_PTR sns;
	
	sns = sys_AllocMemory(HEAPID_BALLOON, sizeof(SONANS_SYS));
	MI_CpuClear8(sns, sizeof(SONANS_SYS));
	sns->rare_type = game->bsw->raregame_type;
	
	//�e�N�X�`�����[�h
	Sonas_TexLoad(sns);
	
	//���_���W���X�g�̏����l�ݒ�
	Sonans_VertexListInit(sns);
	Sonans_TexCoordListInit(sns);
	
	switch(game->bsw->raregame_type){
	case MNGM_RAREGAME_BALLOON_FINE:
		sns->scale_x = SONANS_SCALE_SIZE_FINE_X;		//���A���ǂ����Ńf�t�H���g��ς���
		sns->scale_y = SONANS_SCALE_SIZE_FINE_Y;
		break;
	case MNGM_RAREGAME_BALLOON_THICK:
		sns->scale_x = SONANS_SCALE_SIZE_THICK_X;		//���A���ǂ����Ńf�t�H���g��ς���
		sns->scale_y = SONANS_SCALE_SIZE_THICK_Y;
		break;
	default:
		sns->scale_x = SONANS_SCALE_SIZE_X;		//���A���ǂ����Ńf�t�H���g��ς���
		sns->scale_y = SONANS_SCALE_SIZE_Y;
		break;
	}
	
	sns->aim_x = SONANS_AIM_INIT_X;
	sns->aim_y = SONANS_AIM_INIT_Y;

	Aim_ResourceLoad(game);
	sns->aim_cap = Aim_ActorCreate(game);
	Aim_Update(sns, sns->aim_cap);
	
	sns->return_power = SONANS_RETURN_POWER_INIT;
	
	return sns;
}

//--------------------------------------------------------------
/**
 * @brief	�\�[�i���X�֘A�̏I��
 *
 * @param	sns		�\�[�i���X���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Sonans_Exit(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns)
{
	Aim_ActorDelete(sns->aim_cap);
	Aim_ResourceFree(game);

	sys_FreeMemoryEz(sns->tex_file);
	sys_FreeMemoryEz(sns->palette_work_src);
	sys_FreeMemoryEz(sns->palette_work_dest);
	
	sys_FreeMemoryEz(sns);
}

//--------------------------------------------------------------
/**
 * @brief	�\�[�i���X����X�V����
 *
 * @param	sns		�\�[�i���X���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Sonans_Update(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns)
{
	sns->pomp_area = 0;
	
	if(game->countdown_eff == BALLOON_COUNTDOWN_TOUCHPEN_DEMO){
		sns->tp_x = game->pen.tp_x;
		sns->tp_y = game->pen.tp_y;
		sns->tp_cont = game->pen.tp_cont;
		sns->tp_trg = game->pen.tp_trg;
	}
	else if(game->game_start == FALSE || game->game_finish == TRUE){
		sns->tp_x = 0;
		sns->tp_y = 0;
		sns->tp_cont = 0;
		sns->tp_trg = 0;
	}
	else{
		sns->tp_x = sys.tp_x;
		sns->tp_y = sys.tp_y;
		sns->tp_cont = sys.tp_cont;
		sns->tp_trg = sys.tp_trg;
	}
	
#ifdef PM_DEBUG
	if(sys.trg & PAD_BUTTON_L){
//		sns->debug_aim_on_off ^= 1;
	}
#endif
	
	Sonans_AimHitCheck(sns);
	if(sns->tp_cont == 0){
		Sonans_PushModeReset(sns);
		
		//�\�[�i���X�����S�ɉ񕜂����牟������SE���~�߂�
		if(sns->vertex_list[0].ru.y == SONANS_VERTEX_TOP 
				&& sns->vertex_list[0].ru.y == SONANS_VERTEX_TOP){
			sns->snd_push_se = 0;
			//Snd_SeStopBySeqNo( SE_SONANS_PUSH, 0 );
		}
	}
	Sonans_VertexUpdate(sns);
	
	Sonans_PolygonCreate(sns);
	
	
	Aim_Update(sns, sns->aim_cap);
	
	//�{��MAX��Ԃɂ��g��
	if(sns->scale_voltage_x > 0){
		sns->scale_voltage_x += SONANS_VOLTAGE_SCALE_ADD_X;
		if(sns->scale_voltage_x > SONANS_VOLTAGE_SCALE_X_MAX){
			sns->scale_voltage_x = SONANS_VOLTAGE_SCALE_X_MAX;
		}
		Sonans_VoltageMaxAnime(sns);
	}
	
	//�G�A�[���o
	if(game->game_finish == FALSE && game->game_start == TRUE){
		if(sns->pomp_area > 0){
			BALLOON_AIR_DATA air_data;
			int voltage_max = FALSE, voltage_reset = FALSE;
			s32 air;
			
			//�{���e�[�W
			if(sns->voltage < SONANS_VOLTAGE_MAX){
				voltage_max = Sonans_VoltageAdd(sns, sns->pomp_area);
				if(voltage_max == TRUE){
					sns->scale_voltage_x = SONANS_VOLTAGE_SCALE_ADD_X;
					sns->voltage_anm_wait = 0;
					Snd_SePlay(SE_SONANS_BIG);
				}
			}
			else{
				Sonans_VoltageReset(sns);
				sns->scale_voltage_x = 0;
				voltage_reset = TRUE;
			}

			air = Sonans_PompArea_to_AirPower(sns, sns->pomp_area);
			if(voltage_reset == TRUE){
				air += air * VOLTAGE_MAX_REVISE / 100;
			}
			BalloonTool_AirDataCreate(game, game->balloon_no, air, &air_data);
			BalloonTool_SendAirPush(game, &air_data);
			BalloonTool_PlayerAirParamAdd(game, &air_data);
			
			//GREAT or GOOD��ԂȂ�߂鑬�x�A�b�v
			Sonans_ReturnPowerCalc(sns, sns->pomp_area);
			Sonans_ParticleSet(game, sns, sns->pomp_area, voltage_reset);
			
			sns->pomp_area = 0;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�FV�u�����N�X�V����
 *
 * @param   game		
 * @param   sns			
 */
//--------------------------------------------------------------
void Sonans_VBlank(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns)
{
	if(sns->color_trans_req == TRUE){
		sns->color_trans_req = 0;
		Sonans_PaletteTrans(sns);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �������񂾖ʐς���C�͂ɕϊ�
 *
 * @param   sns				�\�[�i���X���[�N�ւ̃|�C���^
 * @param   pomp_area		�������񂾖ʐ�(100% MAX)
 *
 * @retval  ��C��
 */
//--------------------------------------------------------------
static int Sonans_PompArea_to_AirPower(SONANS_SYS_PTR sns, int pomp_area)
{
	int air;
	
	if(pomp_area >= SONANS_POMP_AREA_MAX_LINE){
		air = SONANS_POMP_MAX_AIR;
	}
	else{
		air = pomp_area * 10;	//1% = 10cc
	}
	
	//���A�Q�[���␳
	air = air * AirRareRevision[sns->rare_type] / 100;
	return air;
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̖߂�͂��A�������񂾖ʐςɂ���čX�V����
 *
 * @param   sns		
 * @param   pomp_area		�������񂾖ʐ�(100%MAX)
 */
//--------------------------------------------------------------
static void Sonans_ReturnPowerCalc(SONANS_SYS_PTR sns, int pomp_area)
{
	if(sns->pomp_area >= SONANS_POMP_AREA_GREAT_LINE){
		sns->return_power += SONANS_RETURN_POWER_ADD_GREAT;
	}
	else if(sns->pomp_area >= SONANS_POMP_AREA_GOOD_LINE){
		sns->return_power += SONANS_RETURN_POWER_ADD_GOOD;
	}
	else{	//���s�Ȃ珉���l�ɖ߂�
		sns->return_power = SONANS_RETURN_POWER_INIT;
	}
	
	if(sns->return_power > SONANS_RETURN_POWER_MAX){
		sns->return_power = SONANS_RETURN_POWER_MAX;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X���������񂾎��̃p�[�e�B�N�����Z�b�g����
 *
 * @param   game
 * @param   pomp_area		�������񂾖ʐ�(100%MAX)
 * @param   voltage_max		TRUE:�{���e�[�W���ő�̎��ɉ�������
 */
//--------------------------------------------------------------
static void Sonans_ParticleSet(BALLOON_GAME_PTR game, SONANS_SYS_PTR sns, int pomp_area, int voltage_max)
{
	if(voltage_max == TRUE && sns->pomp_area >= SONANS_POMP_AREA_GREAT_LINE){
		BalloonParticle_EmitAdd(game, BALLOON_HINOMARU);
		Snd_SePlay(SE_GREAT);
	}
	else if(sns->pomp_area >= SONANS_POMP_AREA_GOOD_LINE){
		;
	}
	else{	//���s
		if(sns->vertex_list[0].lu.y < sns->vertex_list[0].ru.y){
			BalloonParticle_EmitAdd(game, BALLOON_PUSYU2);
			Snd_SePlay(SE_SONANS_AIR);
		}
		else if(sns->vertex_list[0].lu.y > sns->vertex_list[0].ru.y){
			BalloonParticle_EmitAdd(game, BALLOON_PUSYU);
			Snd_SePlay(SE_SONANS_AIR);
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   Push��Ԃ����Z�b�g����
 *
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Sonans_PushModeReset(SONANS_SYS_PTR sns)
{
	if(sns->push_mode == FALSE){
		return;
	}
	sns->push_mode = FALSE;
	sns->backup_tp_y = -1;
	
	//�^�b�`�𗣂�����̖ʐ�
	{
		int after_area;
	
		after_area = Sonans_NowAreaParcentGet(sns);
		sns->pomp_area = sns->before_area - after_area;
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("�҂����ʐ� = %d, before = %d, after = %d\n", sns->pomp_area, sns->before_area, after_area);
	#endif
		if(sns->pomp_area < SONANS_POMP_AREA_IGNORE_LINE){
			sns->pomp_area = 0;
		#ifdef OSP_BALLOON_ON
			OS_TPrintf("���Ȃ���C�Ȃ̂Ŗ���\n");
		#endif
		}
	}
	
//	sns->snd_push_se = 0;
//	Snd_SeStopBySeqNo( SE_SONANS_PUSH, 0 );
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̒��_���X�g���쐬���A�|���S���`��
 *
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 *
 *�@���t���[���ĂԕK�v������܂��B
 */
//--------------------------------------------------------------
static void Sonans_PolygonCreate(SONANS_SYS_PTR sns)
{
	u32 tex_address, pltt_address;

//	GF_G3X_Reset();
	
#if 0
	if(sys.cont & PAD_BUTTON_Y){
		sns->rotate = 0;
	}
	if(sys.cont & PAD_BUTTON_X){
		sns->rotate += 256;
	}
#endif

#if 0
	//---------------------------------------------------------------------------
	// Set up a camera matrix
	//---------------------------------------------------------------------------
	{
		VecFx32 Eye = { 0, 0, FX32_ONE };	// Eye position
		VecFx32 at = { 0, 0, 0 };  // Viewpoint
		VecFx32 vUp = { 0, FX32_ONE, 0 };	// Up

		G3_LookAt(&Eye, &vUp, &at, NULL);
	}
#endif

	//---------------------------------------------------------------------------
	// Set up light colors and direction.
	// Notice that light vector is transformed by the current vector matrix
	// immediately after LightVector command is issued.
	//
	// GX_LIGHTID_0: white, downward
	//---------------------------------------------------------------------------
	NNS_G3dGeLightVector(GX_LIGHTID_0, FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
	NNS_G3dGeLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

	NNS_G3dGePushMtx();

	{
		fx16	s = FX_SinIdx(sns->rotate);
		fx16	c = FX_CosIdx(sns->rotate);

		NNS_G3dGeTranslate(0, (SONANS_TEX_SPACE_SIZE + DAIZA_UNIT_SIZE) * FX32_ONE, -5 * FX32_ONE);

		//NNS_G3dGeMultMtx33
		
//		G3_RotX(s, c);
//		G3_RotY(s, c);
//		G3_RotZ(s, c);

		NNS_G3dGeScale(sns->scale_x 
			+ sns->scale_x * (sns->scale_voltage_x + sns->rare_scale_x) / 100,
			sns->scale_y + sns->scale_y * sns->scale_voltage_y / 100, SONANS_SCALE_SIZE_Z);
	}

	{
		NNS_G3dGeMtxMode(GX_MTXMODE_TEXTURE);
		NNS_G3dGeIdentity();
		// Use an identity matrix for the texture matrix for simplicity
		NNS_G3dGeMtxMode(GX_MTXMODE_POSITION_VECTOR);
	}

	// Set the material color( diffuse, ambient , specular ) as basic white 
#ifdef DEBUG_MAT
	DEMO_Set3DDefaultMaterial(TRUE, TRUE);
	DEMO_Set3DDefaultShininessTable();
#endif

	tex_address = NNS_GfdGetTexKeyAddr(sns->texture->texInfo.vramKey);
	pltt_address = NNS_GfdGetPlttKeyAddr(sns->texture->plttInfo.vramKey);

	NNS_G3dGeTexImageParam(GX_TEXFMT_PLTT256,		// use 16 colors palette texture
					 GX_TEXGEN_TEXCOORD,	// use texcoord
					 GX_TEXSIZE_S128,		// 64 pixels
					 GX_TEXSIZE_T128,		// 64 pixels
					 GX_TEXREPEAT_NONE, 	// no repeat
					 GX_TEXFLIP_NONE,		// no flip
					 GX_TEXPLTTCOLOR0_TRNS,//USE	// use color 0 of the palette
					 tex_address	   // the offset of the texture image
		);

	NNS_G3dGeTexPlttBase(pltt_address,  // the offset of the texture palette
				   GX_TEXFMT_PLTT256 // 16 colors palette texture
		);

	NNS_G3dGePolygonAttr(
			GX_LIGHTMASK_0,				// ���C�g�𔽉f���Ȃ�
			GX_POLYGONMODE_MODULATE,	// ���W�����[�V�������[�h
			GX_CULL_NONE,//GX_CULL_BACK,				// �J���o�b�N���s��
			0,							// �|���S���h�c
			31,							// �A���t�@�l
			GX_POLYGON_ATTR_MISC_NONE
			);

	Sonans_VertexListUpdate(sns);

	NNS_G3dGePopMtx(1);

	NNS_G3dGeFlushBuffer();

	// swapping the polygon list RAM, the vertex RAM, etc.
//	G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}


//==============================================================================
//
//	���[�J���c�[��
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief	�\�[�i���X�̃e�N�X�`���[���[�h
 *
 * @param	sns		�\�[�i���X���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Sonas_TexLoad(SONANS_SYS *sns)
{
	//�e�N�X�`���t�@�C���ǂݍ���
	sns->tex_file = ArchiveDataLoadMalloc(
		ARC_BALLOON_GRA, SONANS_NSBTX, HEAPID_BALLOON);
	sns->texture = NNS_G3dGetTex(sns->tex_file);	// �e�N�X�`�����\�[�X�擾
	
	//VRAM�֓W�J
	LoadVRAMTexture(sns->texture);

	//�p���b�g�����[�N�փR�s�[����(�\�[�i���X��Ԃ����Ă�������)
	{
		u32 sz;
		void* pData;
		u32 from;

		sz = (u32)sns->texture->plttInfo.sizePltt << 3;
		pData = (u8*)sns->texture + sns->texture->plttInfo.ofsPlttData;
		from = (sns->texture->plttInfo.vramKey & 0xffff) << 3;
		
		GF_ASSERT(sns->palette_work_src == NULL);
		sns->palette_work_src = sys_AllocMemory(HEAPID_BALLOON, sz);
		sns->palette_work_dest = sys_AllocMemory(HEAPID_BALLOON, sz);
		MI_CpuCopy8(pData, sns->palette_work_src, sz);
		MI_CpuCopy8(pData, sns->palette_work_dest, sz);
		DC_FlushRange(sns->palette_work_dest, sz);
		OS_TPrintf("�\�[�i���X�̃p���b�g�T�C�Y��%d\n", sz);
		sns->palette_trans_adrs = from;
		sns->palette_size = sz;
	}
	
	//�e�N�X�`���C���[�W��VRAM�֓W�J���I������̂ŁA���̂�j��
	{
		u8* texImgStartAddr;
		u32 newSize;

		GF_ASSERT(sns->texture->texInfo.ofsTex != 0);
		texImgStartAddr = (u8*)sns->texture + sns->texture->texInfo.ofsTex;
		// �q�[�v�̐擪����e�N�X�`���C���[�W�܂ł̃T�C�Y
		newSize = (u32)(texImgStartAddr - (u8*)sns->tex_file);
		sys_CutMemoryBlockSize( sns->tex_file, newSize );
	}
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̃p���b�g�]��
 *
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_PaletteTrans(SONANS_SYS_PTR sns)
{
    GX_BeginLoadTexPltt();
	
	GX_LoadTexPltt(sns->palette_work_dest, sns->palette_trans_adrs, sns->palette_size);
	
    GX_EndLoadTexPltt();
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̃p���b�g�X�V����
 *
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_VoltagePaletteUpdate(SONANS_SYS_PTR sns, int voltage)
{
	int evy;
	
	evy = (SONANS_VOLTAGE_EVY_MAX << 8) * voltage / SONANS_VOLTAGE_MAX;
	evy = (evy + 0x80) >> 8;	//�l�̌ܓ�
	if(evy > 16){	//���肦�Ȃ����ǔO�̂���
		evy = 16;
	}
#ifdef OSP_BALLOON_ON
	OS_TPrintf("sonans evy = %d\n", evy);
#endif
	SoftFade(sns->palette_work_src, sns->palette_work_dest, 
		sns->palette_size / 2, evy, SONANS_VOLTAGE_COLOR);
	
	DC_FlushRange(sns->palette_work_dest, sns->palette_size);
//	Sonans_PaletteTrans(sns);	//������悤�Ȃ�V�u�����N�]���ɕς���
	sns->color_trans_req = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̃{���e�[�W�����Z����
 *
 * @param   sns		
 * @param   pomp_area		�������񂾖ʐ�(100%MAX)
 *
 * @retval  TRUE:�{���e�[�WMAX
 */
//--------------------------------------------------------------
static BOOL Sonans_VoltageAdd(SONANS_SYS_PTR sns, int pomp_area)
{
	BOOL ret = FALSE;
	
	sns->voltage += pomp_area;
	if(sns->voltage >= SONANS_VOLTAGE_MAX){
		sns->voltage = SONANS_VOLTAGE_MAX;
		ret = TRUE;
	}

	//�{���e�[�W�ɂ���ăJ���[�̉����Z�������Ȃ�
	Sonans_VoltagePaletteUpdate(sns, sns->voltage);

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �{���e�[�W�����Z�b�g����
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_VoltageReset(SONANS_SYS_PTR sns)
{
	sns->voltage = 0;
	Sonans_VoltagePaletteUpdate(sns, sns->voltage);
}

//--------------------------------------------------------------
/**
 * @brief   �{���e�[�W���ő�̎��̓_�ŃA�j��
 *
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_VoltageMaxAnime(SONANS_SYS_PTR sns)
{
	if(sns->scale_voltage_x == 0){
		return;
	}
	
	sns->voltage_anm_wait++;
	if(sns->voltage_anm_wait == VOLTAGE_ANM_FLASH_WAIT){
		Sonans_VoltagePaletteUpdate(sns, sns->voltage/2);
	}
	else if(sns->voltage_anm_wait == VOLTAGE_ANM_FLASH_WAIT * 2){
		Sonans_VoltagePaletteUpdate(sns, sns->voltage);
		sns->voltage_anm_wait = 0;
	}
}


//==============================================================================
//==============================================================================
#ifdef DEBUG_MAT

#define DEMO_INTENSITY_DF   23
#define DEMO_INTENSITY_AM   8
#define DEMO_INTENSITY_SP   31

static const GXRgb DEMO_DIFFUSE_COL =
GX_RGB(DEMO_INTENSITY_DF, DEMO_INTENSITY_DF, DEMO_INTENSITY_DF);
static const GXRgb DEMO_AMBIENT_COL =
GX_RGB(DEMO_INTENSITY_AM, DEMO_INTENSITY_AM, DEMO_INTENSITY_AM);
static const GXRgb DEMO_SPECULAR_COL =
GX_RGB(DEMO_INTENSITY_SP, DEMO_INTENSITY_SP, DEMO_INTENSITY_SP);
static const GXRgb DEMO_EMISSION_COL = GX_RGB(0, 0, 0);

static const u32 LIGHTING_L_DOT_S_SHIFT = 8;


void DEMO_Set3DDefaultMaterial(BOOL bUsediffuseAsVtxCol, BOOL bUseShininessTbl)
{
    NNS_G3dGeMaterialColorDiffAmb(DEMO_DIFFUSE_COL,   // diffuse
                            DEMO_AMBIENT_COL,   // ambient
                            bUsediffuseAsVtxCol // use diffuse as vtx color if TRUE
        );

    NNS_G3dGeMaterialColorSpecEmi(DEMO_SPECULAR_COL,  // specular
                            DEMO_EMISSION_COL,  // emission
                            bUseShininessTbl    // use shininess table if TRUE
        );
}


#include <nitro/code32.h>              // avoid byte access problems
void DEMO_Set3DDefaultShininessTable()
{
    u8      i;
    u32     shininess_table[32];
    u8     *pShininess = (u8 *)&shininess_table[0];
    const u8 tableLength = 32 * sizeof(u32);

    for (i = 0; i < tableLength - 1; i++)
    {
        // ShininessTable is ( 0.8 ) fixed point foramt, so we have to right-shift 8 bit for One multiply.
        // pShininess = (i*2+1)^4
        pShininess[i] = (u8)(((s64)(i * 2 + 1) * (i * 2 + 1) * (i * 2 + 1) * (i * 2 + 1)) >> (LIGHTING_L_DOT_S_SHIFT * (4 - 1)));       // byte access
    }

    // set the max value specialy   
    pShininess[tableLength - 1] = 0xFF;

    NNS_G3dGeShininess(&shininess_table[0]);
}

#include <nitro/codereset.h>


#endif






//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̒��_���W���X�g�������l�ɐݒ肷��
 *
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_VertexListInit(SONANS_SYS_PTR sns)
{
	int y;
	fx32 set_y;
	fx32 add_y;
	
	add_y = (SONANS_VERTEX_BOTTOM - SONANS_VERTEX_TOP) / SONANS_QUAD_STRIP_NUM;
//	OS_TPrintf("vertex add_y = %x\n", add_y);
	
	for(y = 0; y < SONANS_QUAD_STRIP_NUM; y++){
		set_y = SONANS_VERTEX_TOP + y * add_y;
		
		sns->vertex_list[y].lu.x = SONANS_VERTEX_LEFT;
		sns->vertex_list[y].lu.y = set_y;
		sns->vertex_list[y].ld.x = SONANS_VERTEX_LEFT;
		sns->vertex_list[y].ld.y = set_y + add_y;
		sns->vertex_list[y].ru.x = SONANS_VERTEX_RIGHT;
		sns->vertex_list[y].ru.y = set_y;
		sns->vertex_list[y].rd.x = SONANS_VERTEX_RIGHT;
		sns->vertex_list[y].rd.y = set_y + add_y;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̃e�N�X�`�����W���X�g�������l�ɐݒ肷��
 *
 * @param   sns		
 */
//--------------------------------------------------------------
static void Sonans_TexCoordListInit(SONANS_SYS_PTR sns)
{
	int y;
	fx32 set_y;
	fx32 add_y;
	
	add_y = SONANS_TEX_SIZE_Y * FX32_ONE / SONANS_QUAD_STRIP_NUM;

	for(y = 0; y < SONANS_QUAD_STRIP_NUM; y++){
		set_y = y * add_y;
		
		sns->tex_coord_list[y].lu.x = 0;
		sns->tex_coord_list[y].lu.y = set_y;
		sns->tex_coord_list[y].ld.x = 0;
		sns->tex_coord_list[y].ld.y = set_y + add_y;
		sns->tex_coord_list[y].ru.x = SONANS_TEX_SIZE_X * FX32_ONE;
		sns->tex_coord_list[y].ru.y = set_y;
		sns->tex_coord_list[y].rd.x = SONANS_TEX_SIZE_X * FX32_ONE;
		sns->tex_coord_list[y].rd.y = set_y + add_y;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���_���W�ݒ�
 *
 * @param   sns		
 */
//--------------------------------------------------------------
#define SONANS_VERTEX_BASE		(FX16_ONE)
#define SONANS_VERTEX_BASE_HALF	(SONANS_VERTEX_BASE / 2)
static void Sonans_VertexListUpdate(SONANS_SYS_PTR sns)
{
	int y;
	fx64 calc_x, calc_y;
	fx16 set_x, set_y;	//vertex�͒��_�̌X�����v�Z���₷���悤��fx32�Ńh�b�g���W�Ŏ����Ă���̂�
						//���_���X�g���Z�b�g����O��SONANS_VERTEX_BASE��MAX�ɂ����l�ɕϊ����Ă��
	
//	OS_TPrintf("���_�`��J�n\n");
	for(y = 0; y < SONANS_QUAD_STRIP_NUM; y++){
		// �`��J�n
//		NNS_G3dGeBegin(GX_BEGIN_QUAD_STRIP);// ���_���X�g�̊J�n(�A���l�p�`�|���S���ł̕`��錾)
		NNS_G3dGeBegin(GX_BEGIN_QUADS);// ���_���X�g�̊J�n(�A���l�p�`�|���S���ł̕`��錾)
		
		//����
		Sonans_Vertex_FX32_to_Conv(
			sns->vertex_list[y].lu.x, sns->vertex_list[y].lu.y, &set_x, &set_y);
		NNS_G3dGeTexCoord(sns->tex_coord_list[y].lu.x, sns->tex_coord_list[y].lu.y);
		normal(0);
		NNS_G3dGeVtx(set_x, set_y, 0);

		//����
		Sonans_Vertex_FX32_to_Conv(
			sns->vertex_list[y].ld.x, sns->vertex_list[y].ld.y, &set_x, &set_y);
		NNS_G3dGeTexCoord(sns->tex_coord_list[y].ld.x, sns->tex_coord_list[y].ld.y);
		normal(0);
		NNS_G3dGeVtx(set_x, set_y, 0);

		//�E��
		Sonans_Vertex_FX32_to_Conv(
			sns->vertex_list[y].rd.x, sns->vertex_list[y].rd.y, &set_x, &set_y);
		NNS_G3dGeTexCoord(sns->tex_coord_list[y].rd.x, sns->tex_coord_list[y].rd.y);
		normal(0);
		NNS_G3dGeVtx(set_x, set_y, 0);

		//�E��
		Sonans_Vertex_FX32_to_Conv(
			sns->vertex_list[y].ru.x, sns->vertex_list[y].ru.y, &set_x, &set_y);
		NNS_G3dGeTexCoord(sns->tex_coord_list[y].ru.x, sns->tex_coord_list[y].ru.y);
		normal(0);
		NNS_G3dGeVtx(set_x, set_y, 0);

		//�`��I��
		NNS_G3dGeEnd();
	}
//	OS_TPrintf("���_�`��I��\n");
}

static void Sonans_Vertex_FX32_to_Conv(fx32 x, fx32 y, fx16 *ret_x, fx16 *ret_y)
{
	fx32 set_x, set_y;
	
	set_x = SONANS_VERTEX_BASE * (fx64)x / SONANS_VERTEX_LEN;
	set_y = SONANS_VERTEX_BASE * (fx64)y / SONANS_VERTEX_LEN;
	GF_ASSERT(set_x <= FX16_MAX);
	GF_ASSERT(set_x >= FX16_MIN);
	GF_ASSERT(set_y <= FX16_MAX);
	GF_ASSERT(set_y >= FX16_MIN);

//	OS_TPrintf("set_x0 = %x, set_y0 = %x, tex_coord_x = %x, tex_coord_y = %x\n", set_x, set_y, sns->tex_coord_list[y].lu.x, sns->tex_coord_list[y].lu.y);

	set_x -= SONANS_VERTEX_BASE_HALF;	//�\�[�i���X�̒��S�����_�ɂ����
	set_y += SONANS_VERTEX_BASE_HALF;
	GF_ASSERT(set_x <= FX16_MAX);
	GF_ASSERT(set_x >= FX16_MIN);
	GF_ASSERT(set_y <= FX16_MAX);
	GF_ASSERT(set_y >= FX16_MIN);
	
	*ret_x = set_x;
	*ret_y = set_y;
}

//--------------------------------------------------------------
/**
 * @brief   �Ə��Ƀ|�C���g�������`�F�b�N
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 * @retval  TRUE:�q�b�g
 */
//--------------------------------------------------------------
static BOOL Sonans_AimHitCheck(SONANS_SYS_PTR sns)
{
	int aim_y;
	
	if(sns->push_mode == TRUE || 		//���ɉ�����Ă�����
			sns->tp_trg == 0){		//�������t���[���łȂ��Ɣ��肵�Ȃ�
		return FALSE;
	}
	
	aim_y = (sns->aim_hit_center_y_fx >> FX32_SHIFT) + SonansHitRect[sns->rare_type].top;
#if 0
	OS_TPrintf("bbb aim_y = %d, aim_hit_center_y_fx = %d\n", aim_y, sns->aim_hit_center_y_fx);
	if(sns->tp_x < sns->aim_x - AIM_RANGE_HALF_X 
			|| sns->tp_x > sns->aim_x + AIM_RANGE_HALF_X
			|| sns->tp_y < aim_y - AIM_RANGE_HALF_Y 
			|| sns->tp_y > aim_y + AIM_RANGE_HALF_Y){
		return FALSE;	//�Ə��͈̔͊O
	}
	
	sns->push_mode = TRUE;
	sns->aim_hit_x = sns->tp_x;
	sns->aim_hit_y = sns->tp_y;
	sns->aim_hit_y_zure = sns->tp_y - aim_y;	//���S���炢������ă^�b�`��������ێ�
	sns->aim_hit_center_y_fx = 
		-(sns->vertex_list[0].ru.y + (sns->vertex_list[0].lu.y - sns->vertex_list[0].ru.y) / 2);
	OS_TPrintf("aim hit : x = %d, y = %d, center_y = %d\n", sns->aim_hit_x, sns->aim_hit_y, sns->aim_hit_center_y_fx);
	
	//�^�b�`�J�n���̖ʐ�
	sns->before_area = Sonans_NowAreaParcentGet(sns);
	OS_TPrintf("�^�b�`�J�n���̖ʐ� = %d\n", sns->before_area);
#else
	if(sns->tp_x < SonansHitRect[sns->rare_type].left
			|| sns->tp_x > SonansHitRect[sns->rare_type].right
			|| sns->tp_y < aim_y	//SonansHitRect[sns->rare_type].top
			|| sns->tp_y > SonansHitRect[sns->rare_type].bottom){
		return FALSE;	//�Ə��͈̔͊O
	}
	
	sns->push_mode = TRUE;
	sns->aim_hit_x = sns->tp_x;
	sns->aim_hit_y = sns->tp_y;
	sns->aim_hit_y_zure = sns->tp_y - aim_y;	//���S���炢������ă^�b�`��������ێ�
	sns->aim_hit_center_y_fx = 
		-(sns->vertex_list[0].ru.y + (sns->vertex_list[0].lu.y - sns->vertex_list[0].ru.y) / 2);
#ifdef OSP_BALLOON_ON
	OS_TPrintf("aim hit : x = %d, y = %d, center_y = %d\n", sns->aim_hit_x, sns->aim_hit_y, sns->aim_hit_center_y_fx);
#endif

	//�^�b�`�J�n���̖ʐ�
	sns->before_area = Sonans_NowAreaParcentGet(sns);
#ifdef OSP_BALLOON_ON
	OS_TPrintf("�^�b�`�J�n���̖ʐ� = %d\n", sns->before_area);
#endif
#endif
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̒��_���W�X�V����
 *
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 *
 * @retval  
 */
//--------------------------------------------------------------
static int Sonans_VertexUpdate(SONANS_SYS_PTR sns)
{
	int mode = 0;
	fx32 dot_offset;
	VERTEX_CALC_RET ret;
	int tp_offset;
	
	if(sns->push_mode == TRUE){
		if(sns->tp_y < sns->backup_tp_y){
		//	Sonans_PushModeReset(sns);	�T�d�Ƀ^�b�`���Ȃ��Ɖ�ʂɂ��Ă����̃u���ŏ�s���̂ł�߂�
		//	Snd_SeStopBySeqNo( SE_SONANS_PUSH, 0 );
			sns->snd_push_se = 0;
			return FALSE;	//������փy���𓮂�����
		}
		else if(sns->tp_y == sns->backup_tp_y){
		//	Snd_SeStopBySeqNo( SE_SONANS_PUSH, 0 );
			sns->snd_push_se = 0;
			return TRUE;	//�ʒu���ς���Ă��Ȃ��̂Ōv�Z��������K�v�Ȃ�
		}
		
		//if(sns->snd_push_se == 0){
		if(sns->backup_tp_y != -1 && sns->tp_y > sns->backup_tp_y 
				&& Snd_SePlayCheck(SE_SONANS_PUSH) == FALSE){
			Snd_SePlay(SE_SONANS_PUSH);
			sns->snd_push_se = TRUE;
		}

		tp_offset = sns->tp_y - sns->aim_hit_y_zure;
		//OS_TPrintf("before tp_offset = %d, tp_y = %d, zure = %d\n", tp_offset, sns->tp_y, sns->aim_hit_y_zure);
//		tp_offset -= sns->backup_offset_tp_y;
//		OS_TPrintf("after tp_offset = %d, tp_y = %d, zure = %d\n", tp_offset, sns->tp_y, sns->aim_hit_y_zure);
		//dot_offset = tp_y - now_center_y;
		dot_offset = (tp_offset << FX32_SHIFT) 
			- (sns->aim_hit_center_y_fx + (SonansHitRect[sns->rare_type].top << FX32_SHIFT));
//		dot_offset = ((sns->aim_hit_y + tp_offset) << FX32_SHIFT) - sns->aim_hit_center_y_fx;
//		dot_offset = (sns->tp_y - sns->backup_tp_y) << FX32_SHIFT;
		//dot_offset = tp_y - (sns->aim_hit_y << FX32_SHIFT);
		
		//���̂܂܂��ƃy�������炷�̂��������\�[�i���X���ׂ�Ă����̂ł��������ɕ␳��������
		//dot_offset = dot_offset * 10 / 100;
		
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("tp_y = %d, tp_offset = %d, dot_offset = %d, aim_hit_y = %d, center_y_fx = %d\n", sns->tp_y, tp_offset, dot_offset, sns->aim_hit_y, sns->aim_hit_center_y_fx);
	#endif
	
		sns->backup_tp_y = sns->tp_y;
	}
	else{	//�^�b�`���Ă��Ȃ����(��C��߂�)
		mode = TRUE;
		dot_offset = -sns->return_power;
	}
	
	ret = Sonans_VertexCalc(sns, mode, dot_offset);
	switch(ret){
	case VERTEX_CALC_RET_TRUE:
	default:
		break;
	case VERTEX_CALC_RET_RELEASE:
	case VERTEX_CALC_RET_CARRY_OVER:
		Sonans_PushModeReset(sns);
		break;
	}
	sns->aim_hit_center_y_fx = -(sns->vertex_list[0].ru.y + (sns->vertex_list[0].lu.y - sns->vertex_list[0].ru.y) / 2);

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �^�b�`�ʒu�ɂ�蒸�_���W�̍Čv�Z
 *
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 * @param   air_back_mode		TRUE:��C�߂胂�[�h�A�@FALSE:�^�b�`�y���ŉ������݃��[�h
 * @param   dot_offset			���_�ɉ��Z���鐔�l
 *
 * @retval  TRUE:����I��
 * @retval  FALSE:���W�̏���A�����␳�����������B(�^�b�`������ԂȂ�^�b�`����������)
 */
//--------------------------------------------------------------
static VERTEX_CALC_RET Sonans_VertexCalc(SONANS_SYS_PTR sns, int air_back_mode, fx32 dot_offset)
{
	fx32 tp_y, left_y, right_y, now_left_y, now_right_y, new_left_y, new_right_y;
	fx32 now_center_y, new_center_y;
	fx32 rad_x, rad_y;
	u16 right_rad, left_rad;
	int carry_over = FALSE;
	
//	OS_TPrintf("air_back_mode = %d, dot_offset = %d\n", air_back_mode, dot_offset);
	if(air_back_mode == FALSE && dot_offset < 0){
//		return VERTEX_CALC_RET_RELEASE;	//������փy���𓮂����Ă���̂ŏI��
	}

	tp_y = sns->tp_y << FX32_SHIFT;				//�Œ菭����
//	left_y = sns->hit_vertex_lu_y;		//�^�b�`����J�n���̍��㒸�_Y�ʒu
//	right_y = sns->hit_vertex_ru_y;		//�^�b�`����J�n���̉E�㒸�_Y�ʒu
	now_left_y = -sns->vertex_list[0].lu.y;
	now_right_y = -sns->vertex_list[0].ru.y;
	now_center_y = now_right_y + (now_left_y - now_right_y) / 2;
	
	//OS_TPrintf("now_left_y = %d, now_right_y = %d, now_center_y = %d\n", now_left_y, now_right_y, now_center_y);
	//OS_TPrintf("dot_offset = %d, aim_hit_y = %d, aim_hit_center_y = %d\n", dot_offset, sns->aim_hit_y, sns->aim_hit_center_y_fx);
	
	switch(Sonans_AimHitPosGet(sns, air_back_mode)){
	case AIM_HIT_CENTER:
		new_left_y = now_left_y + dot_offset;
		new_right_y = now_right_y + dot_offset;
		break;
	case AIM_HIT_LEFT:
		carry_over = VertexCalc_TouchPower(sns, dot_offset, 
			now_left_y, now_right_y, &new_left_y, &new_right_y);
		break;
	case AIM_HIT_RIGHT:
		carry_over = VertexCalc_TouchPower(sns, dot_offset, 
			now_right_y, now_left_y, &new_right_y, &new_left_y);
		break;
	}
	//OS_TPrintf("aaa tp_y = %d, now_left_y = %d, new_left_y = %d, now_right_y = %d, new_right_y = %d\n", tp_y, now_left_y, new_left_y, now_right_y, new_right_y);
	new_center_y = new_right_y + (new_left_y - new_right_y) / 2;
	//OS_TPrintf("aaa now_center_y = %d, new_center_y = %d\n", now_center_y, new_center_y);
	
	if(now_center_y == new_center_y && now_left_y == new_left_y && now_right_y == new_right_y){
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("�v�ZCANCEL\n");
	#endif
		return VERTEX_CALC_RET_TRUE;	//�����l���ς���Ă��Ȃ��̂ł����ŏI��
	}
	
	//�\�[�i���X�̏���A�����`�F�b�N
	if(new_center_y < 0){
		new_center_y = 0;
//		carry_over = TRUE;	����ɃL�����[�I�[�o�[RETURN�͂���Ȃ�
		//OS_TPrintf("����␳\n");
	}
	if(new_left_y < 0){
		new_left_y = 0;
//		carry_over = TRUE;
	}
	if(new_right_y < 0){
		new_right_y = 0;
//		carry_over = TRUE;
	}
	
	if(new_center_y > SONANS_VERTEX_PUSH_LEN){
		new_center_y = SONANS_VERTEX_PUSH_LEN;
		carry_over = TRUE;
		//OS_TPrintf("�����␳\n");
	}
	if(new_left_y > SONANS_VERTEX_PUSH_LEN){
		new_left_y = SONANS_VERTEX_PUSH_LEN;
		carry_over = TRUE;
	}
	if(new_right_y > SONANS_VERTEX_PUSH_LEN){
		new_right_y = SONANS_VERTEX_PUSH_LEN;
		carry_over = TRUE;
	}
	
//	sns->aim_hit_center_y_fx = new_center_y;	//�Ə��ʒu�X�V
	
	//�p�x
	{
		fx32 rad_x, rad_y;
		int point_x;
		
	#if 0	//aim_hit_x�ł��������X�̈ʒu�Ŋp�x���ς��̂ł�����ۂ��񂾂���
			//��������ςȈׁA�Œ�l�ɂ���
		point_x = sns->aim_hit_x - 128;
	#else
		point_x = 64;	//X�p�x�Œ�l(���̐��l������������قǃ\�[�i���X�̓|���p�x���}�ɂȂ�)
	#endif
	
		switch(Sonans_AimHitPosGet(sns, air_back_mode)){
		case AIM_HIT_CENTER:
		default:
			if(new_left_y < new_right_y){
				rad_x = point_x * FX32_ONE;
			}
			else if(new_left_y > new_right_y){
				rad_x = -point_x * FX32_ONE;
			}
			else{
				rad_x = 0;
			}
			rad_x = point_x * FX32_ONE;
			rad_y = new_right_y - new_center_y;
			right_rad = FX_Atan2Idx(rad_y, rad_x);
			left_rad = right_rad + 0x8000;	//180�x���] (u16�ŃI�[�o�[�t���[���N�������O��)
			break;
		case AIM_HIT_RIGHT:
			rad_x = point_x * FX32_ONE;//sns->aim_hit_x * FX32_ONE;
			rad_y = (new_right_y - new_center_y);
			right_rad = FX_Atan2Idx(rad_y, rad_x);
			left_rad = right_rad + 0x8000;	//180�x���] (u16�ŃI�[�o�[�t���[���N�������O��)
			break;
		case AIM_HIT_LEFT:
			rad_x = -point_x * FX32_ONE;//sns->aim_hit_x * FX32_ONE;
			rad_y = new_left_y - new_center_y;
			left_rad = FX_Atan2Idx(rad_y, rad_x);
			right_rad = left_rad + 0x8000;	//180�x���] (u16�ŃI�[�o�[�t���[���N�������O��)
			break;
		}
		//OS_TPrintf("aim_hit_x = %x, dot_offset = %x\n", sns->aim_hit_x, dot_offset);
		//OS_TPrintf("rad_x = %x, rad_y = %x, left_rad = %x, right_rad = %x, new_right_y = %x, new_center_y = %x, new_left_y = %x\n", rad_x, rad_y, left_rad, right_rad, new_right_y, new_center_y, new_left_y);

		//�e�X�g
//		OS_TPrintf("rad 45,45 = %x\n", FX_Atan2Idx(45*FX32_ONE,45*FX32_ONE));
//		OS_TPrintf("rad -45,-45 = %x\n", FX_Atan2Idx(-45*FX32_ONE,-45*FX32_ONE));
//		OS_TPrintf("rad -45,45 = %x\n", FX_Atan2Idx(-45*FX32_ONE,45*FX32_ONE));
//		OS_TPrintf("rad 45,-45 = %x\n", FX_Atan2Idx(45*FX32_ONE,-45*FX32_ONE));
//		OS_TPrintf("rad 0,45 = %x\n", FX_Atan2Idx(0*FX32_ONE,45*FX32_ONE));
//		OS_TPrintf("rad 0,-45 = %x\n", FX_Atan2Idx(0*FX32_ONE,-45*FX32_ONE));
//		OS_TPrintf("rad 45,0 = %x\n", FX_Atan2Idx(45*FX32_ONE,0*FX32_ONE));
//		OS_TPrintf("rad -45,0 = %x\n", FX_Atan2Idx(-45*FX32_ONE,0*FX32_ONE));
	}
	
	//���_���W�X�V
	{
		fx32 furihaba = SONANS_VERTEX_LEN_HALF / FX32_ONE;

		sns->vertex_list[0].lu.x = furihaba * FX_CosIdx(left_rad) + SONANS_VERTEX_LEN_HALF;
		sns->vertex_list[0].lu.y = -(furihaba * FX_SinIdx(left_rad) + new_center_y);
		sns->vertex_list[0].ru.x = furihaba * FX_CosIdx(right_rad) + SONANS_VERTEX_LEN_HALF;
		sns->vertex_list[0].ru.y = -(furihaba * FX_SinIdx(right_rad) + new_center_y);
		
		//OS_TPrintf("furihaba = %x, left_rad = %x, right_rad = %x, new_center_y = %x\n", furihaba, left_rad, right_rad, new_center_y);
		//OS_TPrintf("lu.x = %x, SinIdx = %x, lu.y = %x, CosIdx = %x\n", sns->vertex_list[0].lu.x, FX_SinIdx(left_rad), sns->vertex_list[0].lu.y, FX_CosIdx(left_rad));
		//OS_TPrintf("ru.x = %x, SinIdx = %x, ru.y = %x, CosIdx = %x\n", sns->vertex_list[0].ru.x, FX_SinIdx(right_rad), sns->vertex_list[0].ru.y, FX_CosIdx(right_rad));
		
		//OS_TPrintf("ld.x = %x, ld.y = %x, rd.x = %x, rd.y = %x\n", sns->vertex_list[0].ld.x, sns->vertex_list[0].ld.y, sns->vertex_list[0].rd.x, sns->vertex_list[0].rd.y);
		
		{
			int i;
			fx32 add_y_line;
			s32 add_rad;
			
			add_y_line = (SONANS_VARIABLE_VERTEX_LEN - new_center_y) / SONANS_VARIABLE_VERTEX_NUM;
			if(right_rad > 0x8000){
				add_rad = -(0x10000 - right_rad) / SONANS_VARIABLE_VERTEX_NUM;
			}
			else{
				add_rad = right_rad / SONANS_VARIABLE_VERTEX_NUM;
			}
		
			for(i = 1; i < SONANS_VARIABLE_VERTEX_NUM; i++){
				sns->vertex_list[i].lu.x = furihaba * FX_CosIdx(left_rad - add_rad * i) + SONANS_VERTEX_LEN_HALF;
				sns->vertex_list[i].lu.y = -(furihaba * FX_SinIdx(left_rad - add_rad * i) + new_center_y + add_y_line * i);
				sns->vertex_list[i].ru.x = furihaba * FX_CosIdx(right_rad - add_rad * i) + SONANS_VERTEX_LEN_HALF;
				sns->vertex_list[i].ru.y = -(furihaba * FX_SinIdx(right_rad - add_rad * i) + new_center_y + add_y_line * i);
			
				sns->vertex_list[i-1].ld.x = sns->vertex_list[i].lu.x;
				sns->vertex_list[i-1].ld.y = sns->vertex_list[i].lu.y;
				sns->vertex_list[i-1].rd.x = sns->vertex_list[i].ru.x;
				sns->vertex_list[i-1].rd.y = sns->vertex_list[i].ru.y;
				
				//OS_TPrintf("ver %dlu.x = %x, ver %dlu.y = %x\n", i, sns->vertex_list[i].lu.x, i, sns->vertex_list[i].lu.y);
				//OS_TPrintf("ver %dru.x = %x, ver %dru.y = %x\n", i, sns->vertex_list[i].ru.x, i, sns->vertex_list[i].ru.y);
			}
		}
	}

	if(carry_over == TRUE){
		return VERTEX_CALC_RET_CARRY_OVER;
	}
	return VERTEX_CALC_RET_TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �Ə��Ƀq�b�g�������̈ʒu����𓾂�
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 * @retval  �ʒu�t���O(AIM_HIT_???)
 */
//--------------------------------------------------------------
static int Sonans_AimHitPosGet(SONANS_SYS_PTR sns, int air_back_mode)
{
	if(air_back_mode == TRUE){
		return AIM_HIT_CENTER;
	}
	
	if(sns->aim_hit_x < sns->aim_x - AIM_CENTER_RANGE_HALF_X){
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("AIM_HIT_LEFT\n");
	#endif
		return AIM_HIT_LEFT;
	}
	if(sns->aim_hit_x > sns->aim_x + AIM_CENTER_RANGE_HALF_X){
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("AIM_HIT_RIGHT\n");
	#endif
		return AIM_HIT_RIGHT;
	}
#ifdef OSP_BALLOON_ON
	OS_TPrintf("AIM_HIT_CENTER\n");
#endif
	return AIM_HIT_CENTER;
}

//--------------------------------------------------------------
/**
 * @brief   �^�b�`�y���̈ʒu�A���E�̌��ݒ��_����A�V�������E���_���W���擾����
 *
 * @param   sns				�\�[�i���X���[�N�ւ̃|�C���^
 * @param   tp_y			�^�b�`�y����Y�l
 * @param   now_y			�^�b�`�y���̒l�𔽉f����Ώۂ̒��_���W(���E�ǂ��炩)
 * @param   now_reverse_y	now_y�̋t���̒��_���W
 * @param   new_y			now_y���̒��_�̐V����Y���W
 * @param   new_reverse_y	�t���̒��_�̐V����Y���W
 *
 * @retval  TRUE:����A�����␳�����@FALSE:����I���B
 */
//--------------------------------------------------------------
static int VertexCalc_TouchPower(SONANS_SYS_PTR sns, fx32 dot_offset, 
	fx32 now_y, fx32 now_reverse_y, fx32 *new_y, fx32 *new_reverse_y)
{
	int ret = FALSE;
	
	*new_y = now_y;
	*new_reverse_y = now_reverse_y;
	
	//����A�����𒴂��Ȃ��悤�ɕ␳
	if(now_y + dot_offset > SONANS_VERTEX_PUSH_LEN){
		dot_offset -= (now_y + dot_offset) - SONANS_VERTEX_PUSH_LEN;
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("�␳����\n");
	#endif
		ret = TRUE;
	}
	
	*new_y = now_y + dot_offset;
#ifdef OSP_BALLOON_ON
	OS_TPrintf("ccc now_y = %d, now_reverse_y = %d, new_y = %d, new_reverse_y = %d\n", now_y, now_reverse_y, *new_y, *new_reverse_y);
#endif
	if(now_reverse_y <= now_y){	//�ύX�O�̋t���̒��_���������A��̈ʒu�ɂ���Ȃ��25%�㏸
		*new_reverse_y = now_reverse_y - (dot_offset * 25 / 100);
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("25\%UP now_reverse_y = %d, new = %d, dot = %d\n", now_reverse_y, new_reverse_y, dot_offset);
	#endif
	}
	else{	//�Ⴂ�ʒu�ɂ���ꍇ�͌���ێ�
		*new_reverse_y = now_reverse_y;
	#ifdef OSP_BALLOON_ON
		OS_TPrintf("����ێ� now_reverse_y = %d, new = %d, dot = %d\n", now_reverse_y, new_reverse_y, dot_offset);
	#endif
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �\�[�i���X�̖ʐς����߂�
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 * @retval  �ʐ�
 */
//--------------------------------------------------------------
static fx32 Sonans_AreaGet(SONANS_SYS_PTR sns)
{
	fx32 jyoutei, katei, takasa;
	fx32 jyoutei_y, jyoutei_x, katei_x;
	fx32 now_area;
	int max_pos = SONANS_VARIABLE_VERTEX_NUM - 1;
	
	//��ԏ�̒��_�ƁA��ԉ��̒��_�̑�`�̖ʐςŎZ�o����
	
	//��`�ʐςŋ��߂���悤�ɏ��̍����͍��E��Y�̒��S�ɂ���B
	//(���E�̍������o���o�����Ƒ�`�̌����ł͋��߂��Ȃ�)
	jyoutei_y = sns->vertex_list[0].ru.y + (sns->vertex_list[0].lu.y - sns->vertex_list[0].ru.y) / 2;
	takasa = -(sns->vertex_list[max_pos].ld.y - jyoutei_y);
	
	jyoutei_x = sns->vertex_list[0].ru.x - sns->vertex_list[0].lu.x;
	katei_x = sns->vertex_list[max_pos].rd.x 
		- sns->vertex_list[max_pos].ld.x;
		
	//now_area = (jyoutei_x + katei_x) * takasa / 2;
	now_area = FX_Mul(jyoutei_x + katei_x, takasa) / 2;
	
#ifdef OSP_BALLOON_ON
	OS_TPrintf("jyoutei_y = %d, takasa = %d, jyoutei_x = %d, katei_x = %d, ver0lu = %d, verMaxlu = %d, now_area = %d\n", jyoutei_y, takasa, jyoutei_x, katei_x, sns->vertex_list[0].lu.y, sns->vertex_list[max_pos].lu.y, now_area);
#endif
	
	return now_area;
}

//--------------------------------------------------------------
/**
 * @brief   ���݂̃\�[�i���X�̖ʐς��ő厞�ɔ�ׂĉ�%�̏�Ԃ��擾����
 *
 * @param   sns		�\�[�i���X���[�N�ւ̃|�C���^
 *
 * @retval  ���݂̖ʐσp�[�Z���e�[�W(100�����ő�)
 */
//--------------------------------------------------------------
static int Sonans_NowAreaParcentGet(SONANS_SYS_PTR sns)
{
	fx32 now_area;
	int now_parcent;
	
	now_area = Sonans_AreaGet(sns);
	now_parcent = 100 * (fx64)now_area / SONANS_MAX_AREA;
#ifdef OSP_BALLOON_ON
	OS_TPrintf("���݂̖ʐρ@���@%d��, now_area = %d, max = %d, 16now = %x, 16max = %x\n", now_parcent, now_area, SONANS_MAX_AREA, now_area, SONANS_MAX_AREA);
#endif

	return now_parcent;
}


//--------------------------------------------------------------
/**
 * @brief   �Ə����\�[�X�o�^
 *
 * @param   game		
 */
//--------------------------------------------------------------
static void Aim_ResourceLoad(BALLOON_GAME_PTR game)
{
	ARCHANDLE* hdl;

	//�n���h���I�[�v��
	hdl  = ArchiveDataHandleOpen(ARC_BALLOON_GRA,  HEAPID_BALLOON); 

	CATS_LoadResourcePlttWorkArcH(game->pfd, FADE_MAIN_OBJ, game->csp, game->crp, 
		hdl, AIM_NCLR, 0, 
		1, NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_OBJ_AIM);

	CATS_LoadResourceCharArcH(game->csp, game->crp, hdl, 
		AIM_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, CHARID_BALLOON_AIM);
	CATS_LoadResourceCellArcH(game->csp, game->crp, hdl, AIM_NCER, 0, CELLID_BALLOON_AIM);
	CATS_LoadResourceCellAnmArcH(game->csp, game->crp, hdl, AIM_NANR, 
		0, CELLANMID_BALLOON_AIM);

	//�n���h������
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------
/**
 * @brief   �Ə����\�[�X���
 *
 * @param   game		
 */
//--------------------------------------------------------------
static void Aim_ResourceFree(BALLOON_GAME_PTR game)
{
	CATS_FreeResourceChar(game->crp, CHARID_BALLOON_AIM);
	CATS_FreeResourceCell(game->crp, CELLID_BALLOON_AIM);
	CATS_FreeResourceCellAnm(game->crp, CELLANMID_BALLOON_AIM);
	CATS_FreeResourcePltt(game->crp, PLTTID_OBJ_AIM);
}

//--------------------------------------------------------------
/**
 * @brief   �Ə��A�N�^�[����
 *
 * @param   game		
 *
 * @retval  ���������Ə��A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
static CATS_ACT_PTR Aim_ActorCreate(BALLOON_GAME_PTR game)
{
	CATS_ACT_PTR cap;
	TCATS_OBJECT_ADD_PARAM_S act_head;
	
	//-- �A�N�^�[���� --//
	act_head = AimObjParam;
	cap = CATS_ObjectAdd_S(game->csp, game->crp, &act_head);
	CATS_ObjectEnableCap(cap, CATS_ENABLE_FALSE);
	
	CATS_ObjectUpdate(cap->act);
	return cap;
}

//--------------------------------------------------------------
/**
 * @brief   �Ə��A�N�^�[�X�V����
 *
 * @param   cap		�Ə��A�N�^�[�ւ̃|�C���^
 * @param   x		���WX
 * @param   y		���WY
 * @param   anmseq	�A�j���V�[�P���X�ԍ�
 */
//--------------------------------------------------------------
static void Aim_Update(SONANS_SYS_PTR sns, CATS_ACT_PTR cap)
{
	int anmseq;
	
	if(sns->push_mode == FALSE){
		anmseq = 0;
	}
	else{
		anmseq = Sonans_AimHitPosGet(sns, 0) + 1;
	}
	
	CATS_ObjectPosSetCap_SubSurface(cap, sns->aim_x, 
		(sns->aim_hit_center_y_fx >> FX32_SHIFT) + SonansHitRect[sns->rare_type].top, 
		BALLOON_SUB_ACTOR_DISTANCE);
	CATS_ObjectAnimeSeqSetCap(cap, anmseq );
	CATS_ObjectUpdate(cap->act);
#ifdef PM_DEBUG
	CATS_ObjectEnableCap(cap, sns->debug_aim_on_off);
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �Ə��A�N�^�[�폜
 *
 * @param   cap		�Ə��A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Aim_ActorDelete(CATS_ACT_PTR cap)
{
	CATS_ActorPointerDelete_S(cap);
}

