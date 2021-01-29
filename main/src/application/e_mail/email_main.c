//==============================================================================
/**
 * @file	emai_main.c
 * @brief	E���[����ʃ��C��PROC
 * @author	matsuda
 * @date	2007.10.17(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "libdpw/dpw_tr.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/window.h"
#include "gflib/touchpanel.h"
#include "system/bmp_menu.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "savedata/wifilist.h"
#include "savedata/zukanwork.h"
#include "communication/communication.h"
#include "demo/title.h"	//TitleProcData

#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_gtc.h"
#include "msgdata/msg_wifi_system.h"

#include "application/email_main.h"
#include "savedata/config.h"
#include "system/pm_overlay.h"
#include "system/main.h"
#include "application/email.h"
#include "savedata/email_savedata.h"
#include "application/wifi_p2pmatch.h"
#include "field/sysflag.h"


FS_EXTERN_OVERLAY( title );

//==============================================================================
//	�萔��`
//==============================================================================
#define MYDWC_HEAPSIZE		0x20000

enum{
	SEQ_INIT_DPW,
	SEQ_INIT_DPW_WAIT,
	SEQ_INIT,
	SEQ_MAIN,
	SEQ_END,
};

//==============================================================================
//	�O���[�o���ϐ�
//==============================================================================
static NNSFndHeapHandle _wtHeapHandle;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void Email_CommInitialize(EMAIL_SYSWORK *esys);
static void Email_CommFree(EMAIL_SYSWORK *esys);
static void *AllocFunc( DWCAllocType name, u32   size, int align );
static void FreeFunc(DWCAllocType name, void* ptr,  u32 size);

static void * SubFuncCall_EmailMenu(EMAIL_SYSWORK *esys);
static void ReturnFuncCall_EmailMenu(EMAIL_SYSWORK *esys);
static void * SubFuncCall_AddressInput(EMAIL_SYSWORK *esys);
static void ReturnFuncCall_AddressInput(EMAIL_SYSWORK *esys);
static void * SubFuncCall_GSProfileIDGet(EMAIL_SYSWORK *esys);
static void ReturnFuncCall_GSProfileIDGet(EMAIL_SYSWORK *esys);


//==============================================================================
//	PROC�f�[�^
//==============================================================================
FS_EXTERN_OVERLAY(email);
FS_EXTERN_OVERLAY(worldtrade);

///E���[���ݒ��ʃv���Z�X��`�f�[�^
const PROC_DATA EmailProcData = {
	EmailProc_Init,
	EmailProc_Main,
	EmailProc_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(email),
};


///E���[�����j���[��ʃv���Z�X��`�f�[�^
static const PROC_DATA EmailMenuProcData = {
	EmailMenu_Enter_Init,
	EmailMenu_Enter_Main,
	EmailMenu_Enter_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(email),
};

///E���[�����l���͉�ʃv���Z�X��`�f�[�^
static const PROC_DATA EmailInputProcData = {
	EmailInput_Init,
	EmailInput_Main,
	EmailInput_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(email),
};

///E���[���A�h���X���͉�ʃv���Z�X��`�f�[�^�@��check�@��ŏ���
static const PROC_DATA EmailAddressProcData = {
	EmailAddress_Init,
	EmailAddress_Main,
	EmailAddress_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(email),
};

FS_EXTERN_OVERLAY(wifi_p2pmatch);
// �F�B�R�[�h�擾�v���Z�X��`�f�[�^
static const PROC_DATA WifiP2PMatchProcData = {
	WifiP2PMatchProc_Init,
	WifiP2PMatchProc_Main,
	WifiP2PMatchProc_End,
	FS_OVERLAY_ID(wifi_p2pmatch),
};


///�T�uPROC�Ăяo�����̃p�����[�^�쐬�֐�
typedef void * (*EMAIL_SUBPROC_PARAM_FUNC)(EMAIL_SYSWORK *esys);
///�T�uPROC�I�����̃p�����[�^���n���֐�
typedef void (*EMAIL_SUBPROC_RETURN_FUNC)(EMAIL_SYSWORK *esys);

///PROC�f�[�^�̃|�C���^�e�[�u���@��EMAIL_SUBPROC_???�ƕ��т𓯂��ɂ��Ă������ƁI
static const struct{
	EMAIL_SUBPROC_PARAM_FUNC param_func;
	EMAIL_SUBPROC_RETURN_FUNC return_func;
	const PROC_DATA *proc_data;
	int comm_free_call;			//TRUE���ʐM���C�u�����Ȃǂ����������Ԃ�PROC�����
}EmailProcDataTbl[] = {
	{SubFuncCall_EmailMenu, ReturnFuncCall_EmailMenu, &EmailMenuProcData, FALSE},
	{SubFuncCall_AddressInput, ReturnFuncCall_AddressInput, &EMail_ProcData, FALSE},
	{SubFuncCall_AddressInput, ReturnFuncCall_AddressInput, &EMail_ProcData, FALSE},
	{SubFuncCall_AddressInput, ReturnFuncCall_AddressInput, &EMail_ProcData, FALSE},
	{SubFuncCall_AddressInput, ReturnFuncCall_AddressInput, &EMail_ProcData, FALSE},
	{SubFuncCall_GSProfileIDGet, ReturnFuncCall_GSProfileIDGet, &WifiP2PMatchProcData, TRUE},
};


//============================================================================================
//	�v���Z�X�֐�
//============================================================================================
//==============================================================================
/**
 * $brief   ���E�����������ʏ�����
 *
 * @param   esys		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailProc_Init( PROC * proc, int * seq )
{
	EMAIL_SYSWORK *esys;

	//E���[���Ǘ��p�q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_EMAIL_MANAGE, 0x28000 );
	//�ʐM�A�C�R�����o���̂ɕK�v
	sys_CreateHeap( HEAPID_BASE_SYSTEM, HEAPID_COMMICON, 0x300 );

	esys = PROC_AllocWork(proc, sizeof(EMAIL_SYSWORK), HEAPID_EMAIL_MANAGE );
	MI_CpuClear8(esys, sizeof(EMAIL_SYSWORK));
	esys->savedata = ((MAINWORK*)PROC_GetParentWork(proc))->savedata;
	esys->config = SaveData_GetConfig(esys->savedata);
	esys->email_address = STRBUF_Create(EMAIL_ADDRESS_LEN_SIZE, HEAPID_EMAIL_MANAGE);
	esys->now_email_address = STRBUF_Create(EMAIL_ADDRESS_LEN_SIZE, HEAPID_EMAIL_MANAGE);

	//�T�E���h�f�[�^�Z�b�g
	Snd_DataSetByScene( SND_SCENE_EMAIL, SEQ_WIFILOBBY, 1 );
		
	//�ŏ��Ɏ��s����T�u�v���Z�X
	esys->sub_nextprocess = EMAIL_SUBPROC_MENU;
	
	return PROC_RES_FINISH;
}

//==============================================================================
/**
 * $brief   E���[���Ǘ����C��
 *
 * @param   esys		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailProc_Main( PROC * proc, int * seq )
{
	EMAIL_SYSWORK * esys  = PROC_GetWork( proc );
	BOOL result;
	
	if(esys->comm_initialize_ok == TRUE){
		// ��M���x�����N�𔽉f������
		DWC_UpdateConnection();

		// Dpw_Tr_Main() �����͗�O�I�ɂ��ł��Ăׂ�
		Dpw_Tr_Main();

		// �ʐM��Ԃ��m�F���ăA�C�R���̕\����ς���
		WirelessIconEasy_SetLevel( WM_LINK_LEVEL_3 - DWC_GetLinkLevel() );
	}

	switch(*seq){
	case SEQ_INIT_DPW:
		Email_CommInitialize(esys);
		*seq = SEQ_INIT_DPW_WAIT;
		break;
	case SEQ_INIT_DPW_WAIT:
		if(CommIsVRAMDInitialize()){
			_wtHeapHandle = esys->heapHandle;
	
			// wifi�������Ǘ��֐��Ăяo��
			DWC_SetMemFunc( AllocFunc, FreeFunc );
			
			esys->comm_initialize_ok = TRUE;
			*seq = SEQ_INIT;
		}
		break;
	case SEQ_INIT:
		esys->sub_proc = PROC_Create(EmailProcDataTbl[esys->sub_nextprocess].proc_data, 
			EmailProcDataTbl[esys->sub_nextprocess].param_func(esys), HEAPID_EMAIL_MANAGE);
		esys->now_process = esys->sub_nextprocess;
		esys->sub_nextprocess = EMAIL_SUBPROC_END;
		*seq = SEQ_MAIN;
		break;
	case SEQ_MAIN:
		if(ProcMain(esys->sub_proc) == TRUE){
			EmailProcDataTbl[esys->now_process].return_func(esys);
			PROC_Delete(esys->sub_proc);
			if(esys->sub_nextprocess == EMAIL_SUBPROC_END){
				*seq = SEQ_END;
			}
			else{
				if(EmailProcDataTbl[esys->sub_nextprocess].comm_free_call == TRUE){
					Email_CommFree(esys);
					*seq = SEQ_INIT;
				}
				else if(esys->comm_initialize_ok == TRUE){
					*seq = SEQ_INIT;
				}
				else{
					*seq = SEQ_INIT_DPW;
				}
			}
		}
		break;
	case SEQ_END:
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}


//==============================================================================
/**
 * $brief   ���E�����������ʏI��
 *
 * @param   esys		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailProc_End(PROC *proc, int *seq)
{
	EMAIL_SYSWORK * esys  = PROC_GetWork( proc );

	Email_CommFree(esys);
	
	STRBUF_Delete(esys->now_email_address);
	STRBUF_Delete(esys->email_address);

	PROC_FreeWork( proc );				// PROC���[�N�J��

	sys_DeleteHeap( HEAPID_EMAIL_MANAGE );
	sys_DeleteHeap( HEAPID_COMMICON );

	//-- �^�C�g����ʂ� --//
	Main_SetNextProc( FS_OVERLAY_ID(title), &TitleProcData);

	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief   �ʐM���C�u�����֘A�̏���������
 *
 * @param   esys		
 */
//--------------------------------------------------------------
static void Email_CommInitialize(EMAIL_SYSWORK *esys)
{
	if(esys->comm_initialize_ok == FALSE){
		OS_TPrintf("Comm�������J�n\n");
		
		//���E������Wifi�ʐM���߂��g�p���邽�߃I�[�o�[���C��ǂݏo��(dpw_tr.c��)
		Overlay_Load(FS_OVERLAY_ID(worldtrade), OVERLAY_LOAD_NOT_SYNCHRONIZE);

		// DWC���C�u�����iWifi�j�ɓn�����߂̃��[�N�̈���m��
		esys->heapPtr    = sys_AllocMemory(HEAPID_EMAIL_MANAGE, MYDWC_HEAPSIZE + 32);
		esys->heapHandle = NNS_FndCreateExpHeap( (void *)( ((u32)esys->heapPtr + 31) / 32 * 32 ), MYDWC_HEAPSIZE);

		//DWC�I�[�o�[���C�ǂݍ���
		DwcOverlayStart();
		DpwCommonOverlayStart();
		// �C�N�j���[�����]��
		CommVRAMDInitialize();
		sys_SleepOK(SLEEPTYPE_COMM);  // �X���[�v�֎~�������B���ۂɒʐM���鎞�ɂ����瑤��NG�ɂ���

		OS_TPrintf("Comm�������I��\n");
	}
}

//--------------------------------------------------------------
/**
 * @brief   �ʐM���C�u�����֘A�̉������
 *
 * @param   esys		
 */
//--------------------------------------------------------------
static void Email_CommFree(EMAIL_SYSWORK *esys)
{
	if(esys->comm_initialize_ok == TRUE){
		OS_TPrintf("Comm����J�n\n");
		
		NNS_FndDestroyExpHeap(esys->heapHandle);
		sys_FreeMemoryEz( esys->heapPtr );
		DpwCommonOverlayEnd();
		DwcOverlayEnd();

		// �C�N�j���[�������
		CommVRAMDFinalize();

		Overlay_UnloadID(FS_OVERLAY_ID(worldtrade));
		
		esys->comm_initialize_ok = FALSE;

		OS_TPrintf("Comm�������\n");
	}
}


//==============================================================================
/**
 * @brief   �T�u�v���Z�X���[�h�̐؂�ւ��w��
 *
 * @param   esys			
 * @param   subprccess	
 * @param   mode		
 *
 * @retval  none		
 */
//==============================================================================
void Email_SubProcessChange( EMAIL_SYSWORK *esys, int subprccess, int mode )
{
	esys->sub_nextprocess  = subprccess;
	esys->sub_process_mode = mode;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u�v���Z�X���[�h�������ŏI�����[�h�ɂ���
 *
 * @param   esys		
 *
 * �ʐM�G���[�Ȃǂł̋����I���p
 */
//--------------------------------------------------------------
void Email_SubProcessEndSet(EMAIL_SYSWORK *esys)
{
	esys->sub_nextprocess = EMAIL_SUBPROC_END;
}

//--------------------------------------------------------------
/**
 * @brief   ���j���[��ʂ��N������Ƃ��̃��[�h���Z�b�g
 *
 * @param   esys				
 * @param   mode				�N�����[�h
 */
//--------------------------------------------------------------
void Email_RecoveryMenuModeSet(EMAIL_SYSWORK *esys, int mode)
{
	esys->sub_menu_recovery_mode = mode;
}

//--------------------------------------------------------------
/**
 * @brief   ���j���[��ʂ��N������Ƃ��̃��[�h�擾
 *
 * @param   esys				
 * @param   mode				�N�����[�h
 */
//--------------------------------------------------------------
int Email_RecoveryMenuModeGet(EMAIL_SYSWORK *esys)
{
	return esys->sub_menu_recovery_mode;
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


//--------------------------------------------------------------
/**
 * @brief   EMAIL_SYSWORK�Ɏ����Ă���E���[���A�h���X���Z�[�u�f�[�^�̕��ɃZ�b�g����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
void Email_AddressSaveWorkSet(EMAIL_SYSWORK *esys)
{
	char *address_char;
	
	address_char = sys_AllocMemory(HEAPID_EMAIL_MANAGE, sizeof(char) * EMAIL_ADDRESS_LEN_SIZE);
	
	Email_Strcode_to_Ascii(esys->email_address, address_char, HEAPID_EMAIL_MANAGE);
	EMAILSAVE_AddressSet(esys->savedata, address_char);
	
	sys_FreeMemoryEz(address_char);
}

//--------------------------------------------------------------
/**
 * @brief   EMAIL_SYSWORK�Ɏ����Ă���F�؃R�[�h(��4��)���Z�[�u�f�[�^�̕��ɃZ�b�g����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
void Email_AuthenticateCodeSaveWorkSet(EMAIL_SYSWORK *esys)
{
	EMAILSAVE_ParamSet(esys->savedata, EMAIL_PARAM_AUTH_CODE_HIGH, esys->authenticate_rand_code);
	EMAILSAVE_ParamSet(esys->savedata, EMAIL_PARAM_AUTH_CODE_LOW, esys->ret_authenticate_code);
}

//--------------------------------------------------------------
/**
 * @brief   EMAIL_SYSWORK�Ɏ����Ă���p�X���[�h���Z�[�u�f�[�^�̕��ɃZ�b�g����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
void Email_PasswordSaveWorkSet(EMAIL_SYSWORK *esys)
{
	EMAILSAVE_ParamSet(esys->savedata, EMAIL_PARAM_PASSWORD, esys->ret_password);
}

//--------------------------------------------------------------
/**
 * @brief   �F�ؗp�v���t�B�[���쐬
 *
 * @param   esys			
 */
//--------------------------------------------------------------
void Email_DCProfileCreate(EMAIL_SYSWORK *esys)
{
	u32 auth_veri;
	
	auth_veri = EMAILSAVE_DCProfileCreate_AuthStart(esys->savedata, &esys->dc_profile);
	
	Email_AuthenticateRandCodeSet(esys, auth_veri);
}

//--------------------------------------------------------------
/**
 * @brief   Email_DCProfileCreate�ō쐬�ς݂̃v���t�B�[���f�[�^�ɑ΂���EMAIL_SYSWORK��
 * 			�������Ă���E���[���A�h���X���Z�b�g����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
void Email_DCProfileSet_Address(EMAIL_SYSWORK *esys)
{
	Email_Strcode_to_Ascii(esys->email_address, esys->dc_profile.mailAddr, HEAPID_EMAIL_MANAGE);
}

//--------------------------------------------------------------
/**
 * @brief   Email_DCProfileCreate�ō쐬�ς݂̃v���t�B�[���f�[�^�ɑ΂���EMAIL_SYSWORK��
 * 			�������Ă���F�؃R�[�h(��4��)���Z�b�g����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
void Email_DCProfileSet_Authenticate(EMAIL_SYSWORK *esys)
{
	esys->dc_profile.mailAddrAuthPass = esys->ret_authenticate_code;
}

//--------------------------------------------------------------
/**
 * @brief   �N���C�A���g���Ō��肳���F�؃R�[�h��3�����Z�b�g����
 *
 * @param   esys		
 * @param   code		�F�؃R�[�h��3��
 */
//--------------------------------------------------------------
void Email_AuthenticateRandCodeSet(EMAIL_SYSWORK *esys, u32 code)
{
	esys->authenticate_rand_code = code;
}

//--------------------------------------------------------------
/**
 * @brief   �N���C�A���g���Ō��肳���F�؃R�[�h��3�����擾����
 *
 * @retval		�F�؃R�[�h��3��
 */
//--------------------------------------------------------------
u32 Email_AuthenticateRandCodeGet(EMAIL_SYSWORK *esys)
{
	return esys->authenticate_rand_code;
}

//--------------------------------------------------------------
/**
 * @brief   ���͉�ʂ���F�؃R�[�h���Z�b�g����
 *
 * @param   esys		
 * @param   code		�F�؃R�[�h(��4��)�B�L�����Z���I���̏ꍇ��EMAIL_AUTHENTICATE_CODE_CANCEL
 */
//--------------------------------------------------------------
void Email_AuthenticateCodeSet(EMAIL_SYSWORK *esys, u32 code)
{
	esys->ret_authenticate_code = code;
}

//--------------------------------------------------------------
/**
 * @brief   ���͉�ʂœ��͂��ꂽ�F�؃R�[�h���擾����
 *
 * @param   esys		
 * 
 * @retval   code		�F�؃R�[�h(��4��)�B�L�����Z���I���̏ꍇ��EMAIL_AUTHENTICATE_CODE_CANCEL
 */
//--------------------------------------------------------------
u32 Email_AuthenticateCodeGet(EMAIL_SYSWORK *esys)
{
	return esys->ret_authenticate_code;
}

//--------------------------------------------------------------
/**
 * @brief   ���͉�ʂ���p�X���[�h���Z�b�g����
 *
 * @param   esys		
 * @param   code		�F�؃R�[�h(��4��)�B�L�����Z���I���̏ꍇ��EMAIL_PASSWORD_CANCEL
 */
//--------------------------------------------------------------
void Email_PasswordNumberSet(EMAIL_SYSWORK *esys, u32 password)
{
	esys->ret_password = password;
}

//--------------------------------------------------------------
/**
 * @brief   ���͉�ʂœ��͂��ꂽ�p�X���[�h���擾����
 *
 * @param   esys		
 * 
 * @retval   code		�F�؃R�[�h(��4��)�B�L�����Z���I���̏ꍇ��EMAIL_PASSWORD_CANCEL
 */
//--------------------------------------------------------------
u32 Email_PasswordNumberGet(EMAIL_SYSWORK *esys)
{
	return esys->ret_password;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���͉�ʏI���t���O���Z�b�g����
 *
 * @param   esys		
 * @param   flag		EMAIL_ADDRESS_RET_SET or EMAIL_ADDRESS_RET_CANCEL
 */
//--------------------------------------------------------------
void Email_AddressReturnFlagSet(EMAIL_SYSWORK *esys, int flag)
{
	esys->ret_address_flag = flag;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���͉�ʏI���t���O���擾����
 *
 * @param   esys		
 * 
 * @retval	EMAIL_ADDRESS_RET_SET or EMAIL_ADDRESS_RET_CANCEL
 */
//--------------------------------------------------------------
int Email_AddressReturnFlagGet(EMAIL_SYSWORK *esys)
{
	return esys->ret_address_flag;
}

//--------------------------------------------------------------
/**
 * @brief   E���[���A�h���X���Z�b�g����(�Z�b�g����Ă���)STRBUF�̃|�C���^���擾����
 *
 * @param   esys		
 */
//--------------------------------------------------------------
STRBUF * Email_AddressStrbufGet(EMAIL_SYSWORK *esys)
{
	return esys->email_address;
}


//==============================================================================
//	�T�uPROC�Ăяo�����̃p�����[�^�쐬�֐�
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���j���[��ʌĂяo���p�����[�^�쐬�֐�
 *
 * @param   esys		
 *
 * @retval  SubProc��ParentWork�Ƃ��ēn�����[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void * SubFuncCall_EmailMenu(EMAIL_SYSWORK *esys)
{
	return esys;
}

//--------------------------------------------------------------
/**
 * @brief   ���j���[��ʂ���߂��Ă������̃p�����[�^���n���֐�
 *
 * @param   esys		
 */
//--------------------------------------------------------------
static void ReturnFuncCall_EmailMenu(EMAIL_SYSWORK *esys)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �A�h���X���͉�ʌĂяo���p�����[�^�쐬�֐�
 *
 * @param   esys		
 *
 * @retval  SubProc��ParentWork�Ƃ��ēn�����[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void * SubFuncCall_AddressInput(EMAIL_SYSWORK *esys)
{
	EMAIL_PARAM *para;
	
	para = sys_AllocMemory(HEAPID_EMAIL_MANAGE, sizeof(EMAIL_PARAM));
	MI_CpuClear8(para, sizeof(EMAIL_PARAM));
	
	para->mode = esys->sub_process_mode;	//EMAIL_MODE_INPUT_???
	
	Email_Ascii_to_Strcode(
		EMAILSAVE_AddressGet(esys->savedata), esys->now_email_address, HEAPID_EMAIL_MANAGE);
	para->now_str = esys->now_email_address;
	para->str = esys->email_address;
	
	para->authenticate_rand_code = esys->authenticate_rand_code;
	para->password = EMAILSAVE_ParamGet(esys->savedata, EMAIL_PARAM_PASSWORD);

#if 0	//���̉�ʌĂяo�����ɒl�̏����������Ă��܂��̂ŃL�����Z���l�Z�b�g����̂�߂�
	para->ret_address_flag = EMAIL_ADDRESS_RET_CANCEL;
	para->ret_authenticate_code = EMAIL_AUTHENTICATE_CODE_CANCEL;
	para->ret_password = 0;
#else
	para->ret_address_flag = esys->ret_address_flag;
	para->ret_authenticate_code = esys->ret_authenticate_code;;
	para->ret_password = esys->ret_password;
#endif

	esys->sub_proc_parent_work = para;
	return esys->sub_proc_parent_work;
}

//--------------------------------------------------------------
/**
 * @brief   �A�h���X���͉�ʌĂяo���p�����[�^�쐬�֐�
 *
 * @param   esys		
 *
 * @retval  SubProc��ParentWork�Ƃ��ēn�����[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void ReturnFuncCall_AddressInput(EMAIL_SYSWORK *esys)
{
	EMAIL_PARAM *para;
	
	para = esys->sub_proc_parent_work;
	
	esys->ret_address_flag = para->ret_address_flag;
	esys->ret_password = para->ret_password;
	esys->ret_authenticate_code = para->ret_authenticate_code;
	
	sys_FreeMemoryEz(esys->sub_proc_parent_work);

	//�T�uPROC�����X�g�ɖ߂����߂ɃZ�b�g
	Email_SubProcessChange(esys, EMAIL_SUBPROC_MENU, 0 );
}

//--------------------------------------------------------------
/**
 * @brief   �A�h���X���͉�ʌĂяo���p�����[�^�쐬�֐�
 *
 * @param   esys		
 *
 * @retval  SubProc��ParentWork�Ƃ��ēn�����[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void * SubFuncCall_GSProfileIDGet(EMAIL_SYSWORK *esys)
{
	WIFIP2PMATCH_PROC_PARAM* para;
	
	para = sys_AllocMemory(HEAPID_EMAIL_MANAGE, sizeof(WIFIP2PMATCH_PROC_PARAM));
	MI_CpuClear8(para, sizeof(WIFIP2PMATCH_PROC_PARAM));
	
	para->pSaveData = esys->savedata;
	para->seq = WIFI_P2PMATCH_DPW;

	esys->comm_initialize_ok = FALSE;
	esys->sub_proc_parent_work = para;
	return esys->sub_proc_parent_work;
}

//--------------------------------------------------------------
/**
 * @brief   �A�h���X���͉�ʌĂяo���p�����[�^�쐬�֐�
 *
 * @param   esys		
 *
 * @retval  SubProc��ParentWork�Ƃ��ēn�����[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void ReturnFuncCall_GSProfileIDGet(EMAIL_SYSWORK *esys)
{
	WIFIP2PMATCH_PROC_PARAM *para;
	
	para = esys->sub_proc_parent_work;

    if( mydwc_checkMyGSID(esys->savedata) ){  // �R�[�h�擾�ɐ���
        SysFlag_WifiUseSet(SaveData_GetEventWork(esys->savedata));
        OS_TPrintf("�F�B�R�[�h�擾����\n");
    }
	
	sys_FreeMemoryEz(esys->sub_proc_parent_work);

	//�T�uPROC�����X�g�ɖ߂����߂ɃZ�b�g
	Email_SubProcessChange(esys, EMAIL_SUBPROC_MENU, 0 );
}

