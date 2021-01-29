//==============================================================================
/**
 * @file	hakai_warp.c
 * @brief	�j�ꂽ���E�˓��f��
 * @author	matsuda
 * @date	2008.04.15(��)
 */
//==============================================================================
#include "common.h"
#include "system/palanm.h"
#include "system/lib_pack.h"
#include "system/snd_tool.h"
#include "system/brightness.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/wipe.h"
#include "system/pm_rtc.h"
#include "system/main.h"
#include "system/savedata.h"
#include "system/fontproc.h"
#include "system/procsys.h"
#include "system/pmfprint.h"

#include "../title/titledemo.naix"
#include "system/d3dobj.h"
#include "demo/hakai_warp.h"


//==============================================================================
//	�萔��`
//==============================================================================
///HEAPID_TITLE_DEMO���m�ۂ���q�[�v�T�C�Y
#define HAKAI_WARP_HEAP_SIZE		(0x50000)

//--------------------------------------------------------------
//	�J�����ݒ�
//--------------------------------------------------------------
#define HAKAI_WARP_CAMERA_MODE			GF_CAMERA_PERSPECTIV	//(GF_CAMERA_ORTHO)

#define HAKAI_WARP_CAMERA_CLIP_NEAR		(0)
#define HAKAI_WARP_CAMERA_CLIP_FAR		(FX32_ONE*300)

///�f���p�J�����ݒ�
#define DEMO_CAMERA_PERSPWAY		(FX_GET_ROTA_NUM(22))
#define DEMO_CAMERA_TX			( 0 )		/// target
#define DEMO_CAMERA_TY			( 0 )
#define DEMO_CAMERA_TZ			( 0 )
///�f���p�J�����̒����_�܂ł̋���
#define DEMO_CAMERA_DISTANCE		(160 << FX32_SHIFT)

///�f���p�J�����̈ړ����x
#define DEMO_CAMERA_MOVE_SPEED		(0xa00)
enum{
	DEMO_CAMERA_MOVE_FRAME = 60,			//�ړ����Ă���t���[��
	DEMO_KAO_ANM_START_FRAME = 75,			//��A�j���J�n�t���[��
	DEMO_CAMERA_ANGLE_START_FRAME = 90 + 160,		//�p�x�ύX�J�n�t���[��
	DEMO_CAMERA_ANGLE_RETURN_FRAME = 95,	//�p�x�ύX�܂�Ԃ��t���[��
	DEMO_CAMERA_ANGLE_END_FRAME = 105,		//�p�x�߂��I���t���[��
	DEMO_CAMERA_DISTANCE_MOVE_FRAME = 100,//115,	//�����l�߂�J�n�t���[��
};
///�f���p�J�����̃X�^�[�g�I�t�Z�b�gZ(�ړ�����)
#define DEMO_CAMERA_MOVE_OFFSET_Z	(DEMO_CAMERA_MOVE_SPEED * DEMO_CAMERA_MOVE_FRAME)

///�f���p�J�����̓˂����ݑO�̊p�x���Z�l�̏����l
#define DEMO_CAMERA_ADD_ANGLE_INIT		(0x10000 - 0x1c7d)	//(40)
#define DEMO_CAMERA_ADD_ANGLE_FRAME		(30)
#define DEMO_CAMERA_ADD_ANGLE_END		(0x10000 - 0x3fef)//(90 - DEMO_CAMERA_ADD_ANGLE_INIT-1)	//�ŏI�I�Ȋp�x��90�x(end+init)���Ɛ^���ÂɂȂ�

///�f���p�J�����̓˂����ݑO�̋������Z�l�̏����l
#define DEMO_CAMERA_ADD_DISTANCE_INIT	(FX32_ONE)
///�f���p�J�����̓˂����ݑO�̃p�[�X���Z�l�̏����l
#define DEMO_CAMERA_ADD_PERSPWAY_INIT	(60 << 8)
///�f���p�J�����̓˂����݂̃p�[�X���Z�l�̍Œ�n
#define DEMO_CAMERA_ADD_PERSPWAY_KEEP	(16 << 8)
///�f���p�J�����̓˂����ݑO�̃p�[�X���Z�l�̏����l
#define DEMO_CAMERA_ADD_PERSPWAY_SUB	(0x0080)


//--------------------------------------------------------------
//	���f��
//--------------------------------------------------------------
#define ANA_3D_X		(0)	//fx32
#define ANA_3D_Y		(0)	//fx32
#define ANA_3D_Z		(0)	//fx32
#define ANA_3D_SCALE	(FX32_ONE)



//==============================================================================
//	�\���̒�`
//==============================================================================
///�j��˓��f������\����
typedef struct _WARP_SYS{
	GF_G3DMAN *g3Dman;
	GF_CAMERA_PTR camera;				///<�J�����ւ̃|�C���^

	TCB_PTR update_tcb;					///<Update�pTCB�ւ̃|�C���^
	
	int frame_count;
	int se_counter;						//SE�Đ��p�̃J�E���^�[(08.04.17)
	
	D3DOBJ ana_obj;
	D3DOBJ_ANM ana_ica_anm;
	D3DOBJ_ANM ana_ita_anm;
	D3DOBJ_MDL ana_mdl;
	
	u32 demo_move_frame;
	int demo_camera_add_angle;
	int demo_camera_add_angle_calc;
	fx32 demo_camera_add_distance;
	int demo_camera_add_persp_way;

	NNSFndAllocator			allocater;
}WARP_SYS;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void HakaiWarp_Update(TCB_PTR tcb, void *work);
static void VBlankFunc( void * work );
static void HakaiWarp_VramBankSet(void);
static void HakaiWarp_CameraInit(WARP_SYS *warp);
static void HakaiWarp_CameraExit(WARP_SYS *warp);
static void Model3DSet( WARP_SYS * warp);
static void Model3DDel(WARP_SYS *warp);
static void Model3D_Update(WARP_SYS *warp);
static GF_G3DMAN * HakaiWarp_3D_Init(int heap_id);
static void HakaiWarpSimpleSetUp(void);
static void HakaiWarp_3D_Exit(GF_G3DMAN *g3Dman);
static void Warp_Demo_CameraMove(WARP_SYS *warp);



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F������
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT HakaiWarpProc_Init( PROC * proc, int * seq )
{
	WARP_SYS *warp;
	
	sys_VBlankFuncChange(NULL, NULL);	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane(0);
	GXS_SetVisiblePlane(0);
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	G2_BlendNone();
	G2S_BlendNone();

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_TITLE_DEMO, HAKAI_WARP_HEAP_SIZE);

	warp = PROC_AllocWork(proc, sizeof(WARP_SYS), HEAPID_TITLE_DEMO );
	MI_CpuClear8(warp, sizeof(WARP_SYS));
//	warp->parent_work = PROC_GetParentWork(proc);
	
//	simple_3DBGInit(HEAPID_TITLE_DEMO);
	warp->g3Dman = HakaiWarp_3D_Init(HEAPID_TITLE_DEMO);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	HakaiWarp_VramBankSet();

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);

	//3D���f���]��
	Model3DSet(warp);

	//�J�����쐬
	HakaiWarp_CameraInit(warp);

	// ���C�v�t�F�[�h�J�n
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
		16, WIPE_DEF_SYNC, HEAPID_TITLE_DEMO );

	//���C����ʐݒ�
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
	GF_Disp_DispOn();

	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	warp->update_tcb = TCB_Add(HakaiWarp_Update, warp, 60000);

	sys_VBlankFuncChange(VBlankFunc, warp);

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F���C��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT HakaiWarpProc_Main( PROC * proc, int * seq )
{
	WARP_SYS * warp  = PROC_GetWork( proc );
	enum{
		SEQ_INIT,
		SEQ_MAIN,
		SEQ_OUT_INIT,
		SEQ_OUT_WAIT,
	};
	
	switch(*seq){
	case SEQ_INIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			(*seq)++;
		}
		break;
	case SEQ_MAIN:
		if( warp->se_counter == 15 ){
			Snd_SePlay( SEQ_SE_PL_SYUWA2 );
		}
		warp->se_counter++;

		warp->frame_count++;
		if(warp->frame_count > 85){
			(*seq)++;
		}
		break;
	case SEQ_OUT_INIT:
		// ���C�v�t�F�[�h�J�n
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK, 
			20, WIPE_DEF_SYNC, HEAPID_TITLE_DEMO );
		(*seq)++;
		break;
	case SEQ_OUT_WAIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			return PROC_RES_FINISH;
		}
		break;
	}

	Warp_Demo_CameraMove(warp);

	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F�I��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT HakaiWarpProc_End( PROC * proc, int * seq )
{
	WARP_SYS * warp  = PROC_GetWork( proc );

	TCB_Delete(warp->update_tcb);

	//3D���f�����
	Model3DDel(warp);
	//�J�����폜
	HakaiWarp_CameraExit(warp);

	//simple_3DBGExit();
	HakaiWarp_3D_Exit(warp->g3Dman);

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	StopTP();		//�^�b�`�p�l���̏I��

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_TITLE_DEMO );

	return PROC_RES_FINISH;
}


//==============================================================================
//	
//==============================================================================
static void HakaiWarp_Update(TCB_PTR tcb, void *work)
{
	WARP_SYS *warp = work;
	
	Model3D_Update(warp);
	GF_G3_RequestSwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);
}

//--------------------------------------------------------------------------------------------
/**
 * VBlank�֐�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VBlankFunc( void * work )
{
	WARP_SYS *warp = work;
	
	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 */
//--------------------------------------------------------------
static void HakaiWarp_VramBankSet(void)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_C,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_01_AB,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		//VRAM�N���A
		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �J�����쐬
 *
 * @param   warp		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HakaiWarp_CameraInit(WARP_SYS *warp)
{
	{//�f���p�J�����Z�b�g
		static const CAMERA_ANGLE DemoCameraAngle = {	//�J�����A���O��
			DEMO_CAMERA_ADD_ANGLE_INIT, FX_GET_ROTA_NUM(0), FX_GET_ROTA_NUM(0),
		};
		VecFx32	target = { DEMO_CAMERA_TX, DEMO_CAMERA_TY, DEMO_CAMERA_TZ };
		
		warp->camera = GFC_AllocCamera(HEAPID_TITLE_DEMO);	//�J�����쐬

		GFC_InitCameraTDA(&target, DEMO_CAMERA_DISTANCE, &DemoCameraAngle,
			DEMO_CAMERA_PERSPWAY, GF_CAMERA_PERSPECTIV, FALSE, warp->camera);

		GFC_SetCameraClip( HAKAI_WARP_CAMERA_CLIP_NEAR, HAKAI_WARP_CAMERA_CLIP_FAR, 
			warp->camera);

		//�X�N���[��IN������̂ł�����Ɨ���
		{
//			VecFx32 move = {0,0,DEMO_CAMERA_MOVE_OFFSET_Z};
//			GFC_ShiftCamera(&move, warp->camera);
		}

		{
			CAMERA_ANGLE angle = {0,0,0,0};

			angle = GFC_GetCameraAngle(warp->camera);
			angle.x = DEMO_CAMERA_ADD_ANGLE_END;
			GFC_SetCameraAngleRev(&angle, warp->camera);
		}
		
		GFC_AttachCamera(warp->camera);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �J�������
 *
 * @param   warp		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HakaiWarp_CameraExit(WARP_SYS *warp)
{
	GFC_FreeCamera(warp->camera);
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g
 *
 * @param   warp		
 */
//--------------------------------------------------------------
static void Model3DSet( WARP_SYS * warp)
{
	ARCHANDLE* hdl;

	sys_InitAllocator( &warp->allocater, HEAPID_TITLE_DEMO, 4 ); 

	hdl  = ArchiveDataHandleOpen(ARC_TITLE_PL, HEAPID_TITLE_DEMO); 

	{//��
		//���f���f�[�^�ǂݍ���
		D3DOBJ_MdlLoadH(&warp->ana_mdl, hdl, NARC_titledemo_op_ana_nsbmd, HEAPID_TITLE_DEMO);
		NNS_G3dMdlUseMdlAlpha(warp->ana_mdl.pModel);
		NNS_G3dMdlUseMdlPolygonID(warp->ana_mdl.pModel);
		//ica�A�j���f�[�^�ǂݍ���
		D3DOBJ_AnmLoadH(&warp->ana_ica_anm, &warp->ana_mdl, hdl, 
			NARC_titledemo_op_ana_nsbca, HEAPID_TITLE_DEMO, &warp->allocater);
		D3DOBJ_AnmSet(&warp->ana_ica_anm, 0);
		//ita�A�j���f�[�^�ǂݍ���
		D3DOBJ_AnmLoadH(&warp->ana_ita_anm, &warp->ana_mdl, hdl, 
			NARC_titledemo_op_ana_nsbta, HEAPID_TITLE_DEMO, &warp->allocater);
		D3DOBJ_AnmSet(&warp->ana_ita_anm, 0);

		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init(&warp->ana_obj, &warp->ana_mdl);
		//���W�ݒ�
		D3DOBJ_SetMatrix( &warp->ana_obj, ANA_3D_X, ANA_3D_Y, ANA_3D_Z);
		D3DOBJ_SetScale(&warp->ana_obj, ANA_3D_SCALE, ANA_3D_SCALE, ANA_3D_SCALE);
		D3DOBJ_SetDraw( &warp->ana_obj, TRUE );
		//�A�j���֘A�t��
		D3DOBJ_AddAnm(&warp->ana_obj, &warp->ana_ica_anm);
		D3DOBJ_AddAnm(&warp->ana_obj, &warp->ana_ita_anm);
	}

	ArchiveDataHandleClose( hdl );

	warp->demo_camera_add_angle = (DEMO_CAMERA_ADD_ANGLE_END - DEMO_CAMERA_ADD_ANGLE_INIT) / DEMO_CAMERA_ADD_ANGLE_FRAME;
	warp->demo_camera_add_angle_calc = DEMO_CAMERA_ADD_ANGLE_INIT;
	warp->demo_camera_add_distance = DEMO_CAMERA_ADD_DISTANCE_INIT;
	warp->demo_camera_add_persp_way = DEMO_CAMERA_ADD_PERSPWAY_INIT;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   warp		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void Model3DDel(WARP_SYS *warp)
{
	D3DOBJ_MdlDelete( &warp->ana_mdl );
	D3DOBJ_AnmDelete(&warp->ana_ica_anm, &warp->allocater);
	D3DOBJ_AnmDelete(&warp->ana_ita_anm, &warp->allocater);
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���X�V
 *
 * @param   warp		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void Model3D_Update(WARP_SYS *warp)
{
	VecFx32 scale_vec, trans;
	MtxFx33 rot;
	
	scale_vec.x = FX32_ONE;
	scale_vec.y = FX32_ONE;
	scale_vec.z = FX32_ONE;
	
	trans.x = 0;
	trans.y = 0;
	trans.z = 0;
	
	MTX_Identity33(&rot);

	//�R�c�`��J�n
	GF_G3X_Reset();
	
	GFC_AttachCamera(warp->camera);
	GFC_SetCameraView(HAKAI_WARP_CAMERA_MODE, warp->camera); //���ˉe�ݒ�
	GFC_CameraLookAt();

	// ���C�g�ƃA���r�G���g
	NNS_G3dGlbLightVector( 0, 0, -FX32_ONE, 0 );
	NNS_G3dGlbLightColor( 0, GX_RGB( 28,28,28 ) );
	NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
	NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
	
	// �ʒu�ݒ�
	NNS_G3dGlbSetBaseTrans(&trans);
	// �p�x�ݒ�
	NNS_G3dGlbSetBaseRot(&rot);
	// �X�P�[���ݒ�
	NNS_G3dGlbSetBaseScale(&scale_vec);

//	NNS_G3dGlbFlush();
	
	
	//�`��
	D3DOBJ_AnmLoop(&warp->ana_ica_anm, FX32_ONE);
	D3DOBJ_AnmLoop(&warp->ana_ita_anm, FX32_ONE);
	NNS_G3dGePushMtx();
	{
		D3DOBJ_Draw( &warp->ana_obj );
	}
	NNS_G3dGePopMtx(1);
}

//--------------------------------------------------------------
/**
 * @brief   �j��˓��f���p3DBG�������֐�
 * 
 * @param   �q�[�vID
 */
//--------------------------------------------------------------
static GF_G3DMAN * HakaiWarp_3D_Init(int heap_id)
{
	GF_G3DMAN *g3Dman;
	
	g3Dman = GF_G3DMAN_Init(heap_id, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
		GF_G3DMAN_LNK, GF_G3DPLT_32K, HakaiWarpSimpleSetUp);
	return g3Dman;
}

static void HakaiWarpSimpleSetUp(void)
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(1);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��
	G3X_EdgeMarking( FALSE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(0,0,0),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog

	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);
}

//--------------------------------------------------------------
/**
 * @brief   �j��˓��f���p3DBG�I������
 *
 * @param   g3Dman		
 */
//--------------------------------------------------------------
static void HakaiWarp_3D_Exit(GF_G3DMAN *g3Dman)
{
	GF_G3D_Exit(g3Dman);
}

//--------------------------------------------------------------
/**
 * @brief   �f���p�J�����̈ړ�
 *
 * @param   warp		
 */
//--------------------------------------------------------------
static void Warp_Demo_CameraMove(WARP_SYS *warp)
{
	VecFx32 move = {0,0,0};
	CAMERA_ANGLE angle = {0,0,0,0};
	int set_angle;
	
	GFC_AddCameraPerspWay(-(warp->demo_camera_add_persp_way >> 8), warp->camera);
	warp->demo_camera_add_persp_way -= DEMO_CAMERA_ADD_PERSPWAY_SUB;
	if(warp->demo_camera_add_persp_way < DEMO_CAMERA_ADD_PERSPWAY_KEEP){
		warp->demo_camera_add_persp_way = DEMO_CAMERA_ADD_PERSPWAY_KEEP;
	}
	warp->demo_move_frame++;
}
