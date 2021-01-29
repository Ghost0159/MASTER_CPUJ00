//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_draw.c
 *	@brief		wifi	�o�g�����[��
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "system/clact_util.h"
#include "system/wipe.h"
#include "system/render_oam.h"

#define __WBR_DRAW_H_GLOBAL
#include "wbr_draw.h"

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
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���C���V�[�P���X
//=====================================
enum{
	WBR_MAINSEQ_WIPEIN,	
	WBR_MAINSEQ_WIPEIN_WAIT,	
	WBR_MAINSEQ_MAIN,
	WBR_MAINSEQ_WIPEOUT,
	WBR_MAINSEQ_WIPEOUT_WAIT,
};

#define WBR_CHARMAN_CONTNUM	( 32 )	// �L�����N�^�}�l�[�W���Ǘ���
#define WBR_PLTTMAN_CONTNUM	( 32 )	// �L�����N�^�}�l�[�W���Ǘ���
#define WBR_CLACT_CONTNUM	( 64 )	// �Z���A�N�^�[�Ǘ���
#define WBR_CHARVRAM_MSIZE	( 128*1024 )	// OAM�L�����N�^�T�C�Y
#define WBR_CHARVRAM_SSIZE	( 16*1024 )	// OAM�L�����N�^�T�C�Y
#define WBR_BGFRAME_BACK	( GF_BGL_FRAME0_M )	// �w�i��
#define WBR_BGFRAME_MSG		( GF_BGL_FRAME1_M )	// ���b�Z�[�W��




//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�O���t�B�b�N�֌W���[�N
//=====================================
typedef struct {
	GF_BGL_INI*	p_bgl;
	CLACT_SET_PTR	p_clset;
	CLACT_U_EASYRENDER_DATA	renddata;
} WBR_GRAPHIC;


//-------------------------------------
///	wifi	�o�g�����[��	���[�N
//=====================================
typedef struct {
	WBR_GRAPHIC	graphic;	// �O���t�B�b�N�f�[�^
} WBR_WK;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void Wbr_Vblank( void* p_wk );


// �O���t�B�b�N�֌W
static void Wbr_GraphicInit( WBR_GRAPHIC* p_gra, u32 heapID );
static void Wbr_GraphicExit( WBR_GRAPHIC* p_gra );
static void Wbr_GraphicVblank( WBR_GRAPHIC* p_gra );
static void Wbr_BankInit( void );
static void Wbr_BankExit( void );
static void Wbr_BgInit( WBR_GRAPHIC* p_gra, u32 heapID );
static void Wbr_BgExit( WBR_GRAPHIC* p_gra );
static void Wbr_BgLoad( WBR_GRAPHIC* p_gra, u32 heapID );
static void Wbr_BgRelease( WBR_GRAPHIC* p_gra );
static void Wbr_ClactInit( WBR_GRAPHIC* p_gra, u32 heapID );
static void Wbr_ClactExit( WBR_GRAPHIC* p_gra );
static void Wbr_ClactLoad( WBR_GRAPHIC* p_gra, u32 heapID );
static void Wbr_ClactRelease( WBR_GRAPHIC* p_gra );

//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X������
 *
 *	@param	proc	���[�N
 *	@param	seq		�V�[�P���X�|�C���^
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WbrProc_Init( PROC *proc,int *seq)
{
	WBR_WK* p_wk = NULL;
	WBR_PROC_PARAM* p_pp = (WBR_PROC_PARAM*)PROC_GetParentWork(proc);

	// �v���Z�X���[�N�쐬
	p_wk = PROC_AllocWork(proc,sizeof(WBR_WK),HEAPID_WIFI_BATTLEROOM);
	MI_CpuClear8(p_wk,sizeof(WBR_WK));
	
	// ���荞�ݐݒ�
	sys_VBlankFuncChange( Wbr_Vblank, p_pp );	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	// �O���t�B�b�N������
	Wbr_GraphicInit( &p_wk->graphic, HEAPID_WIFI_BATTLEROOM );

	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X���C��
 *
 *	@param	proc	���[�N
 *	@param	seq		�V�[�P���X�|�C���^
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WbrProc_Main( PROC *proc,int *seq)
{
	WBR_WK* p_wk = PROC_GetWork(proc);
	WBR_PROC_PARAM* p_pp = (WBR_PROC_PARAM*)PROC_GetParentWork(proc);

	switch( *seq ){
	case WBR_MAINSEQ_WIPEIN:	
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_DOORIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WIFI_BATTLEROOM );
		(*seq)++;
		break;
		
	case WBR_MAINSEQ_WIPEIN_WAIT:	
		if( WIPE_SYS_EndCheck() == TRUE ){
			(*seq)++;
		}
		break;
		
	case WBR_MAINSEQ_MAIN:
		if( sys.trg & PAD_BUTTON_A ){
			(*seq)++;
		}
		break;
		
	case WBR_MAINSEQ_WIPEOUT:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_DOOROUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WIFI_BATTLEROOM );
		(*seq)++;
		break;
		
	case WBR_MAINSEQ_WIPEOUT_WAIT:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return PROC_RES_FINISH;
		}
		break;
		
	default:
		break;
	}

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X�I��
 *
 *	@param	proc	���[�N
 *	@param	seq		�V�[�P���X�|�C���^
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WbrProc_End( PROC *proc,int *seq)
{
	WBR_WK* p_wk = PROC_GetWork(proc);
	WBR_PROC_PARAM* p_pp = (WBR_PROC_PARAM*)PROC_GetParentWork(proc);


	// �O���t�B�b�N�j��
	Wbr_GraphicExit( &p_wk->graphic );	

	// �v���Z�X���[�N�j��
	PROC_FreeWork(proc);

	return PROC_RES_FINISH;
}



//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐��S
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_Vblank( void* p_wk )
{
	WBR_WK* p_wbr = p_wk;

	// �O���t�B�b�NV�u�����N�֐�
	Wbr_GraphicVblank( &p_wbr->graphic );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N������
 *
 *	@param	p_gra		�O���t�B�b�N���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void Wbr_GraphicInit( WBR_GRAPHIC* p_gra, u32 heapID )
{
	// �o���N�ݒ�
	Wbr_BankInit();

	// BGL������
	Wbr_BgInit( p_gra, heapID );

	// �Z���A�N�^�[������
	Wbr_ClactInit( p_gra, heapID );

	// BG�ǂݍ���
	Wbr_BgLoad( p_gra, heapID );

	// �Z���A�N�^�[�ǂݍ���
	Wbr_ClactLoad( p_gra, heapID );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�j��
 *
 *	@param	p_gra		�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_GraphicExit( WBR_GRAPHIC* p_gra )
{
	// BG�ǂݍ���
	Wbr_BgRelease( p_gra );

	// �Z���A�N�^�[�ǂݍ���
	Wbr_ClactRelease( p_gra );

	// BGL������
	Wbr_BgExit( p_gra );

	// �Z���A�N�^�[������
	Wbr_ClactExit( p_gra );

	// �o���N�ݒ�
	Wbr_BankExit();
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_GraphicVblank( WBR_GRAPHIC* p_gra )
{
    // BG��������
	GF_BGL_VBlankFunc( p_gra->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�
 */
//-----------------------------------------------------------------------------
static void Wbr_BankInit( void )
{
    GF_BGL_DISPVRAM tbl = {
        GX_VRAM_BG_256_AC,				// ���C��2D�G���W����BG
        GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
        GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
        GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
		GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
        GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
        GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
        GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
        GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
        GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
	};
    GF_Disp_SetBank( &tbl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�j��
 */
//-----------------------------------------------------------------------------
static void Wbr_BankExit( void )
{
	// VRAM�ݒ胊�Z�b�g
	GX_ResetBankForBG();			// ���C��2D�G���W����BG
	GX_ResetBankForBGExtPltt();		// ���C��2D�G���W����BG�g���p���b�g
	GX_ResetBankForSubBG();			// �T�u2D�G���W����BG
	GX_ResetBankForSubBGExtPltt();	// �T�u2D�G���W����BG�g���p���b�g
	GX_ResetBankForOBJ();			// ���C��2D�G���W����OBJ
	GX_ResetBankForOBJExtPltt();	// ���C��2D�G���W����OBJ�g���p���b�g
	GX_ResetBankForSubOBJ();		// �T�u2D�G���W����OBJ
	GX_ResetBankForSubOBJExtPltt();	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_ResetBankForTex();			// �e�N�X�`���C���[�W
	GX_ResetBankForTexPltt();		// �e�N�X�`���p���b�g
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�ݒ�
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void Wbr_BgInit( WBR_GRAPHIC* p_gra, u32 heapID )
{
	GF_ASSERT( p_gra->p_bgl == NULL );
	
	// BGL�쐬
	p_gra->p_bgl = GF_BGL_BglIniAlloc( heapID );

	
	{
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS,GX_BGMODE_0,GX_BGMODE_0, GX_BG0_AS_3D
		};
		GF_BGL_InitBG(&BGsys_data);
	}

	{	// �w�i
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_256,
			GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x20000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( p_gra->p_bgl, WBR_BGFRAME_BACK, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( WBR_BGFRAME_BACK, 32, 0, heapID );
		GF_BGL_ScrClear( p_gra->p_bgl, WBR_BGFRAME_BACK );
	}

	{	// ���b�Z�[�W
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( p_gra->p_bgl, WBR_BGFRAME_MSG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( WBR_BGFRAME_MSG, 32, 0, heapID );
		GF_BGL_ScrClear( p_gra->p_bgl, WBR_BGFRAME_MSG );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�ݒ�j��
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_BgExit( WBR_GRAPHIC* p_gra )
{
    GF_BGL_BGControlExit( p_gra->p_bgl, WBR_BGFRAME_BACK );
    GF_BGL_BGControlExit( p_gra->p_bgl, WBR_BGFRAME_MSG	 );
	sys_FreeMemoryEz( p_gra->p_bgl );
	p_gra->p_bgl = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�f�[�^�ǂݍ���
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void Wbr_BgLoad( WBR_GRAPHIC* p_gra, u32 heapID )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�f�[�^�j��
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_BgRelease( WBR_GRAPHIC* p_gra )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e��������
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void Wbr_ClactInit( WBR_GRAPHIC* p_gra, u32 heapID )
{
	// OAM�}�l�[�W��������
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 124,		// ���C�����OAM�Ǘ��̈�
        0, 31,		// ���C����ʃA�t�B���Ǘ��̈�
        0, 124,		// �T�u���OAM�Ǘ��̈�
        0, 31,		// �T�u��ʃA�t�B���Ǘ��̈�
		heapID );
	
	// �L�����N�^�}�l�[�W���[������
	{
		CHAR_MANAGER_MAKE cm = {
			WBR_CHARMAN_CONTNUM,
			WBR_CHARVRAM_MSIZE,
			WBR_CHARVRAM_SSIZE,
			0
		};
		cm.heap = heapID;
		InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K );
	}
	// �p���b�g�}�l�[�W���[������
	InitPlttManager(WBR_PLTTMAN_CONTNUM, heapID);

	// �ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

	// �ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);

	// �Z���A�N�^�[�Z�b�g�쐬
    p_gra->p_clset = CLACT_U_SetEasyInit( WBR_CLACT_CONTNUM, &p_gra->renddata, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e���j��
 *
 *	@param	p_gra	�O���t�B�b�N�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_ClactExit( WBR_GRAPHIC* p_gra )
{
    //OAM�����_���[�j��
    REND_OAM_Delete();

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

	// �Z���A�N�^�[�Z�b�g�j��
    CLACT_DestSet(p_gra->p_clset);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�f�[�^�ǂݍ���
 *
 *	@param	p_gra		�O���t�B�b�N�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */	
//-----------------------------------------------------------------------------
static void Wbr_ClactLoad( WBR_GRAPHIC* p_gra, u32 heapID )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�f�[�^�j��
 *
 *	@param	p_gra		�O���t�B�b�N�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void Wbr_ClactRelease( WBR_GRAPHIC* p_gra )
{
}


