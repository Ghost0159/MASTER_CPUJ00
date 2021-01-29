//==============================================================================
/**
 * @file	codein.c
 * @brief	�������̓C���^�[�t�F�[�X
 * @author	goto
 * @date	2007.07.11(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/pm_str.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "system/window.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_fightmsg_dp.h"
#include "system/wipe.h"
#include "system/brightness.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/msg_ds_icon.h"
#include "gflib/msg_print.h"
#include "gflib/touchpanel.h"
#include "poketool/poke_tool.h"
#include "poketool/monsno.h"
#include "poketool/pokeicon.h"
#include "poketool/boxdata.h"
#include "system/bmp_menu.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"

#include "codein_pv.h"
#include "codein_snd.h"



// ----------------------------------------
//
//	�v���g�^�C�v
//
// ----------------------------------------
static void CI_VramBankSet( GF_BGL_INI* bgl );
static void CI_VBlank( void* work );

// ----------------------------------------
//
//	�v���Z�X��`
//
// ----------------------------------------
static PROC_RESULT CI_Proc_Init( PROC * proc, int * seq );
static PROC_RESULT CI_Proc_Main( PROC * proc, int * seq );
static PROC_RESULT CI_Proc_Exit( PROC * proc, int * seq );

const PROC_DATA CodeInput_ProcData = {
	CI_Proc_Init,
	CI_Proc_Main,
	CI_Proc_Exit,
	NO_OVERLAY_ID,	
};


//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT CI_Proc_Init( PROC * proc, int * seq )
{
	CODEIN_WORK* wk;
	
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_CODEIN, HEAP_SIZE_CODEIN );
	
	wk = PROC_AllocWork( proc, sizeof( CODEIN_WORK ), HEAPID_CODEIN );
	memset( wk, 0, sizeof( CODEIN_WORK ) );
	wk->param = *( ( CODEIN_PARAM* )PROC_GetParentWork( proc ) );
	
	///< ������
	sys_VBlankFuncChange( NULL, NULL );		///< VBlank�Z�b�g
	sys_HBlankIntrStop();					///< HBlank���荞�ݒ�~
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane( 0 );
	GXS_SetVisiblePlane( 0 );

	///< �V�X�e�����[�N�̍쐬	
	wk->sys.p_handle = ArchiveDataHandleOpen( ARC_CODE_GRA, HEAPID_CODEIN );	
	wk->sys.bgl		 = GF_BGL_BglIniAlloc( HEAPID_CODEIN );	
	wk->sys.pfd		 = PaletteFadeInit( HEAPID_CODEIN );
	
	PaletteTrans_AutoSet( wk->sys.pfd, TRUE );
	PaletteFadeWorkAllocSet( wk->sys.pfd, FADE_MAIN_BG,	 0x200, HEAPID_CODEIN );
	PaletteFadeWorkAllocSet( wk->sys.pfd, FADE_SUB_BG,	 0x200, HEAPID_CODEIN );
	PaletteFadeWorkAllocSet( wk->sys.pfd, FADE_MAIN_OBJ, 0x200, HEAPID_CODEIN );
	PaletteFadeWorkAllocSet( wk->sys.pfd, FADE_SUB_OBJ,	 0x200, HEAPID_CODEIN );	

	///< Vram�ݒ�
	CI_VramBankSet( wk->sys.bgl );	
	
	///< OAM�ݒ�
	CI_pv_disp_CATS_Init( wk );
	
	///< �ݒ菉����
	CI_pv_ParamInit( wk );
	
	///< Touch Panel�ݒ�
	{
		InitTPSystem();
		InitTPNoBuff( 4 );		
		CI_pv_ButtonManagerInit( wk );
	}
	
	G2_SetBlendAlpha( GX_BLEND_PLANEMASK_NONE,
					  GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 15, 7 );
					 
	G2S_SetBlendAlpha( GX_BLEND_PLANEMASK_NONE,
					   GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 |
					   GX_BLEND_PLANEMASK_BG3, 7, 8 );
	
	sys_VBlankFuncChange( CI_VBlank, wk );
		
	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT CI_Proc_Main( PROC * proc, int * seq )
{
	BOOL bUpdate;
	
	CODEIN_WORK* wk;
	
	wk = PROC_GetWork( proc );
	
	bUpdate = CI_pv_MainUpdate( wk );
	
	return bUpdate ? PROC_RES_FINISH : PROC_RES_CONTINUE;
}


//--------------------------------------------------------------
/**
 * @brief	�I��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT CI_Proc_Exit( PROC * proc, int * seq )
{
	CODEIN_WORK* wk;
	
	wk = PROC_GetWork( proc );
	
	CI_pv_disp_CodeRes_Delete( wk );
	
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2,  VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3,  VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME1_M );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME3_M );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME0_S );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME1_S );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME2_S );
	GF_BGL_BGControlExit( wk->sys.bgl, GF_BGL_FRAME3_S );
	sys_FreeMemoryEz( wk->sys.bgl );
	
	PaletteFadeWorkAllocFree( wk->sys.pfd, FADE_MAIN_BG );
	PaletteFadeWorkAllocFree( wk->sys.pfd, FADE_SUB_BG );
	PaletteFadeWorkAllocFree( wk->sys.pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->sys.pfd, FADE_SUB_OBJ );
	PaletteFadeFree( wk->sys.pfd );
	
	ArchiveDataHandleClose( wk->sys.p_handle );
	
	
	{
		u32 active;
		active = StopTP();
	}
	
	CATS_ResourceDestructor_S( wk->sys.csp, wk->sys.crp );
	CATS_FreeMemory( wk->sys.csp );
	
	BMN_Delete( wk->sys.btn );
	
	PROC_FreeWork( proc );
	
	sys_DeleteHeap( HEAPID_CODEIN );
	
	return PROC_RES_FINISH;
}





//--------------------------------------------------------------
/**
 * @brief	CODEIN_PARAM �̃��[�N���쐬����
 *
 * @param	heap_id	
 * @param	word_len	
 * @param	block	
 *
 * @retval	CODEIN_PARAM*	
 *
 */
//--------------------------------------------------------------
CODEIN_PARAM* CodeInput_ParamCreate( int heap_id, int word_len, int block[], CONFIG* cfg )
{
	int i;
	CODEIN_PARAM* wk = NULL;
	
	wk = sys_AllocMemory( heap_id, sizeof( CODEIN_PARAM ) );

	wk->word_len 	= word_len;	
	wk->strbuf		= STRBUF_Create( word_len + 1, heap_id );
	wk->cfg			= cfg;
	
	for ( i = 0; i < CODE_BLOCK_MAX; i++ ){
		wk->block[ i ] = block[ i ];
		OS_Printf( "block %d = %d\n", i, wk->block[ i ] );
	}
	wk->block[ i ] = block[ i - 1 ];

	return wk;	
}

//--------------------------------------------------------------
/**
 * @brief	CODEIN_PARAM �̃��[�N�����
 *
 * @param	codein_param	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CodeInput_ParamDelete( CODEIN_PARAM* codein_param )
{
	GF_ASSERT( codein_param->strbuf != NULL );
	GF_ASSERT( codein_param != NULL );
	
	STRBUF_Delete( codein_param->strbuf );
	sys_FreeMemoryEz( codein_param );	
}


//--------------------------------------------------------------
/**
 * @brief	VramBankSet
 *
 * @param	bgl	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void CI_VramBankSet( GF_BGL_INI* bgl )
{
	GF_Disp_GX_VisibleControlInit();
	
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
			GX_VRAM_TEX_01_BC,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		//VRAM�N���A
		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}
	
		//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME1_M
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				0x01, 0, 0, FALSE
			},
			///<FRAME2_M
			{
				0, 0, 0x1000, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
				0x02, 0, 0, FALSE
			},
			///<FRAME3_M
			{
				0, 0, 0x1800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				0x03, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME1_M, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME2_M, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME3_M, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME1_M );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME3_M );
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	}
	
	///< �T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			{	/// font
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				0x00, 0, 0, FALSE
			},
			{	/// bg
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				0x01, 0, 0, FALSE
			},
			{
				0
			},
			{
				0
			},
		#if 0
			{	/// list
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				CB_BG_PRI_SCR_LIST, 0, 0, FALSE
			},
			{	/// edit
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				CB_BG_PRI_SCR_EDIT, 0, 0, FALSE
			},
		#endif
		};
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME0_S, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME1_S, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME2_S, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_BGControlSet( bgl, GF_BGL_FRAME3_S, &TextBgCntDat[3], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME0_S );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME1_S );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME2_S );
		GF_BGL_ScrClear( bgl, GF_BGL_FRAME3_S );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	}
	
	///< SUB��ʂ����C����ʂɂ��邽��
	sys.disp3DSW = DISP_3D_TO_SUB;
	GF_Disp_DispSelect();
	
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
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
static void CI_VBlank( void* work )
{
	CODEIN_WORK* wk = work;

	DoVramTransferManager();			///< Vram�]���}�l�[�W���[���s
	
	CATS_RenderOamTrans();

	PaletteFadeTrans( wk->sys.pfd );
	
	GF_BGL_VBlankFunc( wk->sys.bgl );
	
	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}
