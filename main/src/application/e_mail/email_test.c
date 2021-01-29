//==============================================================================
/**
 * @file	email_input.c
 * @brief	�ȒP�Ȑ���������
 * @author	matsuda
 * @date	2007.10.19(��)
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


//#include "application/worldtrade.h"
//#include "worldtrade_local.h"

#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_gtc.h"
#include "msgdata/msg_wifi_system.h"

#include "application/email_main.h"
#include "msgdata/msg_email.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata/config.h"
#include "savedata\system_data.h"

#include "../wifi_p2pmatch/wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`


//============================================================================================
//	�v���Z�X�֐�
//============================================================================================
//==============================================================================
/**
 * $brief   ���E�����������ʏ�����
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailAddress_Init( PROC * proc, int * seq )
{
	EMAIL_SYSWORK *esys;
	
	esys = PROC_GetParentWork(proc);

	//E���[����ʗp�q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_EMAIL, 0x70000 );
	
	Email_Ascii_to_Strcode("matsuda@gamefreak.co.jp", Email_AddressStrbufGet(esys), HEAPID_EMAIL);
	Email_AddressReturnFlagSet(esys, EMAIL_ADDRESS_RET_SET);
	
	return PROC_RES_FINISH;
}


//==============================================================================
/**
 * $brief   ���E�����������ʃ��C��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailAddress_Main( PROC * proc, int * seq )
{
	return PROC_RES_FINISH;
}


//==============================================================================
/**
 * $brief   ���E�����������ʏI��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailAddress_End(PROC *proc, int *seq)
{
	EMAIL_SYSWORK *esys;

	esys = PROC_GetParentWork(proc);
	//�T�uPROC�����X�g�ɖ߂����߂ɃZ�b�g
	Email_SubProcessChange( esys, EMAIL_SUBPROC_MENU, 0 );


	sys_DeleteHeap( HEAPID_EMAIL );

	return PROC_RES_FINISH;
}


