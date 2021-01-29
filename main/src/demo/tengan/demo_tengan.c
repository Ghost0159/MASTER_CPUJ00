//==============================================================================
/**
 * @file	�t�@�C����
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.11.05(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include "common.h"
#include "gflib/camera.h"

#include "system/procsys.h"
#include "system/msgdata.h"
#include "system/font_arc.h"
#include "system/brightness.h"
#include "system/arc_tool.h"
#include "system/clact_tool.h"

#include "system/laster.h"
#include "system/wipe.h"


#include "field/field_common.h"
#include "field/field_event.h"
#include "field/fieldsys.h"
#include "field/ev_mapchange.h"



#include "demo_tengan_common.h"

FS_EXTERN_OVERLAY( demo_tenganzan );

//==============================================================
// Prototype
//==============================================================
static void CATS_Init( DEMO_SYSTEM* system );
static void Demo_CameraSet( GF_CAMERA_PTR camera, VecFx32* target );
static void SystemInit( DEMO_SYSTEM* system );
static void SystemExit( DEMO_SYSTEM* system );
static void VramBankSet( GF_BGL_INI* bgl );
static void VBlank( void* work );
static void LasterScroll( TCB_PTR tcb, void* work );

//--------------------------------------------------------------
/**
 * @brief	�v���b�N������
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	PROC_RESULT	
 *
 */
//--------------------------------------------------------------
PROC_RESULT DemoTengan_Proc_Init( PROC * proc, int * seq )
{
	DEMO_TENGAN_WORK* wk;
	
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_DEMO_TENGAN, 0xC0000 );
	
	wk = PROC_AllocWork( proc, sizeof( DEMO_TENGAN_WORK ), HEAPID_DEMO_TENGAN );
	
	memset( wk, 0, sizeof( DEMO_TENGAN_WORK ) );
	
	wk->param = PROC_GetParentWork( proc );
	
	SystemInit( &wk->sys );	
//	Demo_CameraSet( wk->sys.camera_p, &wk->sys.camera_target );
	
	WIPE_SYS_Start( WIPE_PATTERN_WMS,
				    WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN,
				    WIPE_FADE_BLACK,  WIPE_DEF_DIV*2, WIPE_DEF_SYNC, HEAPID_DEMO_TENGAN );
		
	sys_VBlankFuncChange( VBlank, wk );
	
	
	///< ���C�g�̊�{�J���[
	wk->sys.light_color.r = LIGHT_PARAM_MAX;
	wk->sys.light_color.g = LIGHT_PARAM_MAX;
	wk->sys.light_color.b = LIGHT_PARAM_MAX;
	
	wk->sys.evy = LIGHT_PARAM_MAX;
	
//	Snd_BgmPlay( SEQ_BLD_GAME );
	
	Snd_PMVoiceDoubleFlagSet( 1 );
	
	{
		GF_BGL_BMPWIN* win = &wk->sys.win;
		int wintype = CONFIG_GetWindowType( wk->param->cfg );
					
		GF_BGL_BmpWinInit( win );
		GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME1_M, 2, 19, 27, 4, 14, 1 );
		GF_BGL_BmpWinDataFill( win, 0xFF );
		GF_BGL_BmpWinOn( win );
		
					
		TalkWinGraphicSet(wk->sys.bgl, GF_BGL_FRAME1_M, 500, 15, wintype, HEAPID_DEMO_TENGAN );

		PaletteWorkSet_Arc(wk->sys.pfd, ARC_WINFRAME, TalkWinPalArcGet( wintype ),
						   HEAPID_DEMO_TENGAN, FADE_MAIN_BG, 0x20, 15 * 16);

		PaletteWorkSet_Arc( wk->sys.pfd, ARC_FONT, NARC_font_talk_ncrl, 
						   HEAPID_DEMO_TENGAN, FADE_MAIN_BG, 0x20, 14 * 16);
						   
		BmpTalkWinWrite( win, WINDOW_TRANS_ON, 500, 15 );
		
		Demo_MSG_Off( &wk->sys );
	}

	return PROC_RES_FINISH;
}

typedef void* ( *pDemoInit_func )( DEMO_TENGAN_WORK* wk );
typedef BOOL  ( *pDemoMain_func )( void* wk );
typedef BOOL  ( *pDemoExit_func )( void* wk );


static void* Demo_Tengan_Scene0D_Init( DEMO_TENGAN_WORK* main_wk );
static BOOL Demo_Tengan_Scene0D_Main( void* work );
static BOOL Demo_Tengan_Scene0D_Exit( void* work );

static const struct {
	
	pDemoInit_func		init;
	pDemoMain_func		main;
	pDemoExit_func		exit;
	
} Demo_Scene[] = {
	{ Demo_Tengan_Scene02_Init, Demo_Tengan_Scene02_Main, Demo_Tengan_Scene02_Exit },
	{ Demo_Tengan_Scene00_Init, Demo_Tengan_Scene00_Main, Demo_Tengan_Scene00_Exit },
	{ Demo_Tengan_Scene01_Init, Demo_Tengan_Scene01_Main, Demo_Tengan_Scene01_Exit },
//	{ Demo_Tengan_Scene0D_Init, Demo_Tengan_Scene0D_Main, Demo_Tengan_Scene0D_Exit },	
};


//--------------------------------------------------------------
/**
 * @brief	�v���b�N���C��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	PROC_RESULT	
 *
 */
//--------------------------------------------------------------
PROC_RESULT DemoTengan_Proc_Main( PROC * proc, int * seq )
{
	enum {
		
		eDEMO_Init = 0,
		eDEMO_Main,
		eDEMO_Change,
		eDEMO_Exit,		
	};

	DEMO_TENGAN_WORK* wk = PROC_GetWork( proc );
	
//	if ( WIPE_SYS_EndCheck() == FALSE ){ return PROC_RES_CONTINUE; }
	
	switch ( *seq ){
	
	case eDEMO_Init:
		{
			wk->scene_wk = Demo_Scene[ wk->scene_no ].init( wk );

			*seq = eDEMO_Main;
		}
		break;

	case eDEMO_Main:
		{
			BOOL bMain = Demo_Scene[ wk->scene_no ].main( wk->scene_wk );
			
			if ( bMain == FALSE ){
				*seq = eDEMO_Change;
			}
		}
		break;

	case eDEMO_Change:
		{
			BOOL bMain = Demo_Scene[ wk->scene_no ].exit( wk->scene_wk );
			
			if ( bMain == FALSE ){					
				if ( ( ++wk->scene_no ) != NELEMS( Demo_Scene ) ){
					*seq = eDEMO_Init;
				}
				else {
					*seq = eDEMO_Exit;
				}
			}
		}
		break;

	case eDEMO_Exit:
		{
			return PROC_RES_FINISH;
		}
		break;

	default:
		GF_ASSERT( 0 );
		break;
	};

	CATS_Draw( wk->sys.crp );

	return PROC_RES_CONTINUE;
}


//--------------------------------------------------------------
/**
 * @brief	�v���b�N�I��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	PROC_RESULT	
 *
 */
//--------------------------------------------------------------
PROC_RESULT DemoTengan_Proc_Exit( PROC * proc, int * seq )
{
	DEMO_TENGAN_WORK* wk;
	
	wk = PROC_GetWork( proc );
	
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();
	
	
	BmpTalkWinClear( &wk->sys.win, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOff( &wk->sys.win );
	GF_BGL_BmpWinDel( &wk->sys.win );
	

	SystemExit( &wk->sys );
	
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
	
	PROC_FreeWork( proc );
	sys_DeleteHeap( HEAPID_DEMO_TENGAN );
	
	Snd_PMVoiceDoubleFlagSet( 0 );
	
	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief	CATS������
 *
 * @param	system	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void CATS_Init( DEMO_SYSTEM* system )
{
	system->csp = CATS_AllocMemory( HEAPID_DEMO_TENGAN );
	{
		const TCATS_OAM_INIT OamInit = {
			0, 128, 0, 32,
			0, 128, 0, 32,
		};
		const TCATS_CHAR_MANAGER_MAKE Ccmm = {
			48 + 48,			///< ID�Ǘ� main + sub
			1024 * 0x40,		///< 64k
			 512 * 0x20,		///< 32k
			GX_OBJVRAMMODE_CHAR_1D_64K,
			GX_OBJVRAMMODE_CHAR_1D_32K
		};
		BOOL check = CATS_SystemInit( system->csp, &OamInit, &Ccmm, 16 + 16 );
		GF_ASSERT( check );
	}
	
	{
		BOOL active;
		const TCATS_RESOURCE_NUM_LIST ResourceList = {
			48 + 48,	///< �L�����o�^�� 	main + sub
			16 + 16,	///< �p���b�g�o�^��	main + sub
			64,			///< �Z��
			64,			///< �Z���A�j��
			16,			///< �}���`�Z��
			16,			///< �}���`�Z���A�j��
		};
		
		system->crp = CATS_ResourceCreate( system->csp );

		active   = CATS_ClactSetInit( system->csp, system->crp, 64 + 64 );
		GF_ASSERT( active );
		
		active	  = CATS_ResourceManagerInit( system->csp, system->crp, &ResourceList );
		GF_ASSERT( active );
	}
}

//--------------------------------------------------------------
/**
 * @brief	�V�X�e��������
 *
 * @param	system	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void SystemInit( DEMO_SYSTEM* system )
{
	sys_VBlankFuncChange( NULL, NULL );		///< VBlank�Z�b�g
	sys_HBlankIntrStop();					///< HBlank���荞�ݒ�~
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane( 0 );
	GXS_SetVisiblePlane( 0 );
	
	system->p_handle	= ArchiveDataHandleOpen( ARC_DEMO_TENGAN_GRA, HEAPID_DEMO_TENGAN );	
	system->bgl			= GF_BGL_BglIniAlloc( HEAPID_DEMO_TENGAN );
	system->pfd			= PaletteFadeInit( HEAPID_DEMO_TENGAN );
	system->g3Dman		= GF_G3DMAN_Init( HEAPID_DEMO_TENGAN, GF_G3DMAN_LNK, GF_G3DTEX_128K, GF_G3DMAN_LNK, GF_G3DPLT_64K, NULL );
	system->camera_p	= GFC_AllocCamera( HEAPID_DEMO_TENGAN );
	system->man			= MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_d05r0114_dat, HEAPID_DEMO_TENGAN );
	
	PaletteTrans_AutoSet( system->pfd, TRUE );
	PaletteFadeWorkAllocSet( system->pfd, FADE_MAIN_BG, 0x200, HEAPID_DEMO_TENGAN );
	PaletteFadeWorkAllocSet( system->pfd, FADE_SUB_BG,	0x200, HEAPID_DEMO_TENGAN );
	PaletteFadeWorkAllocSet( system->pfd, FADE_MAIN_OBJ,0x200, HEAPID_DEMO_TENGAN );
	PaletteFadeWorkAllocSet( system->pfd, FADE_SUB_OBJ, 0x200, HEAPID_DEMO_TENGAN );

	VramBankSet( system->bgl );
	CATS_Init( system );
	
	G3X_AlphaBlend( TRUE );
	
	sys_InitAllocator( &system->allocator, HEAPID_DEMO_TENGAN, 32 );
}


//--------------------------------------------------------------
/**
 * @brief	�V�X�e���I��
 *
 * @param	system	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void SystemExit( DEMO_SYSTEM* system )
{
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3,  VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME0_M );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME1_M );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME3_M );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME0_S );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME1_S );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME2_S );
	GF_BGL_BGControlExit( system->bgl, GF_BGL_FRAME3_S );
	sys_FreeMemoryEz( system->bgl );

	PaletteFadeWorkAllocFree( system->pfd, FADE_MAIN_BG );
	PaletteFadeWorkAllocFree( system->pfd, FADE_SUB_BG );
	PaletteFadeWorkAllocFree( system->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( system->pfd, FADE_SUB_OBJ );
	PaletteFadeFree( system->pfd );

	ArchiveDataHandleClose( system->p_handle );
	
	GF_G3D_Exit( system->g3Dman );
	GFC_FreeCamera( system->camera_p );
	
	
	CATS_ResourceDestructor_S( system->csp, system->crp );
	CATS_FreeMemory( system->csp );
	
	
	MSGMAN_Delete( system->man );
	
	G3X_AlphaBlend( FALSE );	
}

//--------------------------------------------------------------
/**
 * @brief	Vram�ݒ�
 *
 * @param	bgl	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void VramBankSet( GF_BGL_INI* bgl )
{
	GF_Disp_GX_VisibleControlInit();
	
	{
		GF_BGL_SYS_HEADER BGsys_data = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };
		GF_BGL_InitBG( &BGsys_data );
	}
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
			GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
			GX_VRAM_TEX_0_B,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}
	
	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_M
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				1, 0, 0, FALSE
			},
			///<FRAME1_M
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
			///<FRAME2_M
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
				3, 0, 0, FALSE
			},
			///<FRAME3_M	���g�p
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3800, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
		};
		GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, HEAPID_DEMO_TENGAN );
		GF_BGL_ClearCharSet( GF_BGL_FRAME1_M, 32, 0, HEAPID_DEMO_TENGAN );
		GF_BGL_ClearCharSet( GF_BGL_FRAME2_M, 32, 0, HEAPID_DEMO_TENGAN );
		GF_BGL_ClearCharSet( GF_BGL_FRAME3_M, 32, 0, HEAPID_DEMO_TENGAN );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME0_M, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME1_M, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME2_M, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME3_M, &TextBgCntDat[3], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME0_M );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME1_M );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME3_M );
	}
	///< �T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_S
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
			///<FRAME1_S
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3800, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				2, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME0_S, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME1_S, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME0_S );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME1_S );
	}
	GF_BGL_ClearCharSet( GF_BGL_FRAME0_S, 32, 0, HEAPID_DEMO_TENGAN );
	GF_BGL_ClearCharSet( GF_BGL_FRAME1_S, 32, 0, HEAPID_DEMO_TENGAN );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//--------------------------------------------------------------
/**
 * @brief	VBlank
 *
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void VBlank( void* work )
{
	DEMO_TENGAN_WORK* wk = work;

	DoVramTransferManager();			///< Vram�]���}�l�[�W���[���s
	
	CATS_RenderOamTrans();

	PaletteFadeTrans( wk->sys.pfd );
	
	GF_BGL_VBlankFunc( wk->sys.bgl );
	
	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}


// �f�o�b�O�p�������Ȃ��f��
static void* Demo_Tengan_Scene0D_Init( DEMO_TENGAN_WORK* main_wk )
{
	return NULL;
}
static BOOL Demo_Tengan_Scene0D_Main( void* work )
{
	return FALSE;
}
static BOOL Demo_Tengan_Scene0D_Exit( void* work )
{
	return FALSE;
}