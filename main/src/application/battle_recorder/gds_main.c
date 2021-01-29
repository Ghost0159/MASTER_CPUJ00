//==============================================================================
/**
 * @file	gds_main.c
 * @brief	GDS���[�h�FPROC����
 * @author	matsuda
 * @date	2008.01.17(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "wifi/dwc_rap.h"
#include "savedata\savedata.h"
#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "savedata/imageclip_data.h"
#include "poketool/boxdata.h"
#include "poketool/monsno.h"
#include "gflib/strbuf_family.h"
#include "savedata/gds_profile.h"

#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/fontproc.h"
#include "gflib/strbuf_family.h"

#include "communication\comm_system.h"
#include "communication\comm_state.h"
#include "communication\comm_def.h"
#include "communication/wm_icon.h"
#include "communication\communication.h"

#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "gds_rap.h"
#include "gds_rap_response.h"
#include "gds_data_conv.h"

#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata/config.h"
#include "savedata\system_data.h"
#include "system/bmp_menu.h"
#include "system/procsys.h"
#include "system/wipe.h"
#include "system/window.h"

#include "application/gds_main.h"
#include "application/br_sys.h"


//==============================================================================
//	�萔��`
//==============================================================================
#define MYDWC_HEAPSIZE		0x20000

///GDS�v���b�N���䂪�g�p����q�[�v�T�C�Y
#define GDSPROC_HEAP_SIZE		(MYDWC_HEAPSIZE + 0x8000)

//==============================================================================
//	�O���[�o���ϐ�
//==============================================================================
static NNSFndHeapHandle _wtHeapHandle;

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void GdsMain_CommInitialize(GDSPROC_MAIN_WORK *gmw);
static void GdsMain_CommFree(GDSPROC_MAIN_WORK *gmw);
static void *AllocFunc( DWCAllocType name, u32   size, int align );
static void FreeFunc(DWCAllocType name, void* ptr,  u32 size);

//==============================================================================
//	�f�[�^
//==============================================================================
///WIFI�ڑ���ʃv���Z�X��`�f�[�^
static const PROC_DATA GdsConnectProcData = {
	GdsConnectProc_Init,
	GdsConnectProc_Main,
	GdsConnectProc_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(gds_comm),
};

#ifdef PM_DEBUG
//��check �e�X�g�p
extern PROC_RESULT GdsTestProc_Init( PROC * proc, int * seq );
extern PROC_RESULT GdsTestProc_Main( PROC * proc, int * seq );
extern PROC_RESULT GdsTestProc_End( PROC * proc, int * seq );
extern PROC_RESULT GdsTestDressUpProc_Main( PROC * proc, int * seq );
#endif


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
PROC_RESULT GdsMainProc_Init( PROC * proc, int * seq )
{
	GDSPROC_MAIN_WORK *gmw;
	
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

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_GDS_MAIN, GDSPROC_HEAP_SIZE);

	gmw = PROC_AllocWork(proc, sizeof(GDSPROC_MAIN_WORK), HEAPID_GDS_MAIN );
	MI_CpuClear8(gmw, sizeof(GDSPROC_MAIN_WORK));
	gmw->proc_param = PROC_GetParentWork(proc);

	// �T�E���h�f�[�^���[�h(�t�B�[���h)
	Snd_DataSetByScene( SND_SCENE_P2P, SEQ_WIFILOBBY, 1 );

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
PROC_RESULT GdsMainProc_Main( PROC * proc, int * seq )
{
	GDSPROC_MAIN_WORK * gmw  = PROC_GetWork( proc );
	enum{
		SEQ_INIT_DPW,
		SEQ_INIT_DPW_WAIT,
		
		SEQ_WIFI_CONNECT,
		SEQ_WIFI_CONNECT_MAIN,
		
		SEQ_BATTLE_RECORDER,
		SEQ_BATTLE_RECORDER_MAIN,
		
		SEQ_WIFI_CLEANUP,
		SEQ_WIFI_CLEANUP_MAIN,
		
		SEQ_END,
	};
	
	switch(*seq){
	case SEQ_INIT_DPW:	//�ʐM���C�u����������
		GdsMain_CommInitialize(gmw);
		*seq = SEQ_INIT_DPW_WAIT;
		break;
	case SEQ_INIT_DPW_WAIT:
		if(CommIsVRAMDInitialize()){
			_wtHeapHandle = gmw->heapHandle;
	
			// wifi�������Ǘ��֐��Ăяo��
			DWC_SetMemFunc( AllocFunc, FreeFunc );
			
			gmw->comm_initialize_ok = TRUE;
			(*seq)++;
		}
		break;
		
	case SEQ_WIFI_CONNECT:	//WIFI�ڑ�
		gmw->sub_proc = PROC_Create(&GdsConnectProcData, gmw, HEAPID_GDS_MAIN);
		(*seq)++;
		break;
	case SEQ_WIFI_CONNECT_MAIN:
		if(ProcMain(gmw->sub_proc) == TRUE){
			PROC_Delete(gmw->sub_proc);
			if(gmw->ret_connect == TRUE){
				gmw->connect_success = TRUE;
				(*seq)++;
			}
			else{
				*seq = SEQ_END;
			}
		}
		break;
	
	case SEQ_BATTLE_RECORDER:	//�o�g�����R�[�_�[(GDS���[�h)
	#ifdef PM_DEBUG
		{
			if(gmw->proc_param->gds_mode == 0xff){
				static const PROC_DATA GdsTestProcData = {
					GdsTestProc_Init,
					GdsTestDressUpProc_Main,	//GdsTestProc_Main,
					GdsTestProc_End,
					NO_OVERLAY_ID,//FS_OVERLAY_ID(gds_comm),
				};
				gmw->sub_proc = PROC_Create(&GdsTestProcData, gmw->proc_param->savedata, 
					HEAPID_GDS_MAIN);
				(*seq)++;
				break;
			}
		}
	#endif
		
		{
			const PROC_DATA *br_proc;
			
			br_proc = BattleRecoder_ProcDataGet(gmw->proc_param->gds_mode);
			gmw->sub_proc = PROC_Create(br_proc, gmw->proc_param->fsys, HEAPID_GDS_MAIN);
			(*seq)++;
		}
		break;
	case SEQ_BATTLE_RECORDER_MAIN:
		if(ProcMain(gmw->sub_proc) == TRUE){
			PROC_Delete(gmw->sub_proc);
			(*seq)++;
		}
		break;
	
	case SEQ_WIFI_CLEANUP:		//WIFI�ؒf
		gmw->sub_proc = PROC_Create(&GdsConnectProcData, gmw, HEAPID_GDS_MAIN);
		(*seq)++;
		break;
	case SEQ_WIFI_CLEANUP_MAIN:
		if(ProcMain(gmw->sub_proc) == TRUE){
			PROC_Delete(gmw->sub_proc);
			gmw->connect_success = FALSE;
			(*seq)++;
		}
		break;

	case SEQ_END:
		return PROC_RES_FINISH;
	}

	if(gmw->comm_initialize_ok == TRUE && gmw->connect_success == TRUE && gmw->ret_connect == TRUE){
		// ��M���x�����N�𔽉f������
		DWC_UpdateConnection();

		// Dpw_Tr_Main() �����͗�O�I�ɂ��ł��Ăׂ�
	//	Dpw_Tr_Main();

		// �ʐM��Ԃ��m�F���ăA�C�R���̕\����ς���
		WirelessIconEasy_SetLevel( WM_LINK_LEVEL_3 - DWC_GetLinkLevel() );
	}

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
PROC_RESULT GdsMainProc_End( PROC * proc, int * seq )
{
	GDSPROC_MAIN_WORK * gmw  = PROC_GetWork( proc );

	GdsMain_CommFree(gmw);
	
	//GDS�v���b�N�Ăяo���p�����[�^���
	sys_FreeMemoryEz(gmw->proc_param);
	
	PROC_FreeWork( proc );				// PROC���[�N�J��

	sys_DeleteHeap( HEAPID_GDS_MAIN );

	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief   �ʐM���C�u�����֘A�̏���������
 *
 * @param   gmw		
 */
//--------------------------------------------------------------
static void GdsMain_CommInitialize(GDSPROC_MAIN_WORK *gmw)
{
	if(gmw->comm_initialize_ok == FALSE){
		OS_TPrintf("Comm�������J�n\n");
		
		//���E������Wifi�ʐM���߂��g�p���邽�߃I�[�o�[���C��ǂݏo��(dpw_tr.c��)
//		Overlay_Load(FS_OVERLAY_ID(worldtrade), OVERLAY_LOAD_NOT_SYNCHRONIZE);

		// DWC���C�u�����iWifi�j�ɓn�����߂̃��[�N�̈���m��
		gmw->heapPtr    = sys_AllocMemory(HEAPID_GDS_MAIN, MYDWC_HEAPSIZE + 32);
		gmw->heapHandle = NNS_FndCreateExpHeap( (void *)( ((u32)gmw->heapPtr + 31) / 32 * 32 ), MYDWC_HEAPSIZE);

		//DWC�I�[�o�[���C�ǂݍ���
		DwcOverlayStart();
		DpwCommonOverlayStart();
		// �C�N�j���[�����]��
		CommVRAMDInitialize();

		OS_TPrintf("Comm�������I��\n");
	}
}

//--------------------------------------------------------------
/**
 * @brief   �ʐM���C�u�����֘A�̉������
 *
 * @param   gmw		
 */
//--------------------------------------------------------------
static void GdsMain_CommFree(GDSPROC_MAIN_WORK *gmw)
{
	if(gmw->comm_initialize_ok == TRUE){
		OS_TPrintf("Comm����J�n\n");
		
		NNS_FndDestroyExpHeap(gmw->heapHandle);
		sys_FreeMemoryEz( gmw->heapPtr );
		DpwCommonOverlayEnd();
		DwcOverlayEnd();

		// �C�N�j���[�������
		CommVRAMDFinalize();

//		Overlay_UnloadID(FS_OVERLAY_ID(worldtrade));
		
		gmw->comm_initialize_ok = FALSE;

		OS_TPrintf("Comm�������\n");
	}
}

/*---------------------------------------------------------------------------*
  �������m�ۊ֐�
 *---------------------------------------------------------------------------*/
static void *AllocFunc( DWCAllocType name, u32   size, int align )
{
#pragma unused( name )
    void * ptr;
    OSIntrMode old;
    old = OS_DisableInterrupts();
    ptr = NNS_FndAllocFromExpHeapEx( _wtHeapHandle, size, align );
    OS_RestoreInterrupts( old );
    if(ptr == NULL){
	}
	
    return ptr;
}

/*---------------------------------------------------------------------------*
  �������J���֐�
 *---------------------------------------------------------------------------*/
static void FreeFunc(DWCAllocType name, void* ptr,  u32 size)
{
#pragma unused( name, size )
    OSIntrMode old;

    if ( !ptr ) return;
    old = OS_DisableInterrupts();
    NNS_FndFreeToExpHeap( _wtHeapHandle, ptr );
    OS_RestoreInterrupts( old );
}
