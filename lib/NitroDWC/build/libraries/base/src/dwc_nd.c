/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_nd.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_nd.c,v $
  Revision 1.37  2007/03/19 09:27:38  nakata
  �f�o�b�O�\����OS_TPrintf����DWC_Printf���g�p����悤�ɕύX�B

  Revision 1.36  2006/09/05 05:37:09  takayama
  DWC_NdCleanupAsync�֐��̕Ԃ�l�� void ���� BOOL �ɕύX�����B

  Revision 1.35  2006/08/25 04:31:35  nakata
  ������������Cleanup���Ă�ASSERT���Ȃ��悤�ɕύX

  Revision 1.34  2006/08/21 07:50:01  nakata
  Nd���C�u�����̏��������s���ɔ������郁�������[�N���C���B

  Revision 1.33  2006/06/22 09:10:20  nakata
  �폜����#pragma unused�����ɖ߂���

  Revision 1.32  2006/06/22 02:34:19  nakata
  �t�@�C���_�E�����[�h���̃o�b�t�@�T�C�Y��DWC_ND_RECVBUF_MIN�����������ꍇ��ASSERT��ǉ��B

  Revision 1.31  2006/06/22 02:03:04  nakata
  �_�E�����[�h���C�u�����̃o�b�t�@�s���G���[����菜�����B(DWCError�̎�ނ𑝂₵�����Ȃ�����)

  Revision 1.30  2006/06/22 00:49:50  nakata
  �_�E�����[�h���C�u�����̃o�b�t�@�s����OS_Panic���Ȃ��悤�ύX

  Revision 1.29  2006/06/08 06:07:33  nakata
  �R�����g�ǉ�

  Revision 1.28  2006/06/08 06:06:54  nakata
  SVL��base64�f�R�[�h����ĕԂ����悤�ɂȂ������߁ADWCi_Nd�ɓn���O��base64�G���R�[�h����悤�ɕύX

  Revision 1.27  2006/06/05 05:49:36  nakata
  �_�E�����[�h��o�b�t�@�̑傫���ƃt�@�C���̑傫�����r����ASSERT������������Ă��������C��

  Revision 1.26  2006/05/30 04:33:51  nakata
  SVL�̃G���[�ԍ��݂̂��c����Nd����DWC_Error��ݒ肷��ύX���ꕔ�Ɏ{����

  Revision 1.25  2006/05/29 11:37:00  nakata
  SVL�G���[�����̂܂ܕԂ����ꍇNd���C�u�������������Ώ����s���Ȃ����߁ANd���C�u�����ɂ����SVL�̃G���[���㏑������悤�ɍďC��

  Revision 1.24  2006/05/29 11:13:33  nakata
  SVL��DWCError��Nd���㏑�����Ȃ��悤�ύX

  Revision 1.23  2006/05/26 04:09:59  nakata
  DWCError��������FATAL�G���[���㏑������̂���߂�

  Revision 1.22  2006/05/26 02:42:44  nakata
  DWCError����̒ǉ�

  Revision 1.21  2006/05/25 10:42:41  nakata
  DWC_Error����̒ǉ�

  Revision 1.20  2006/05/25 05:08:21  nakata
  DWC_Nd/DWC_SVL��DWC�G���[��Ԃ��@�\��ǉ��������Ƃɔ����ύX

  Revision 1.19  2006/05/17 09:01:11  nakata
  NHTTP/DWC_Nd���C�u������5/17�łɍX�V�������Ƃɔ����C��

  Revision 1.18  2006/05/17 00:50:10  nakata
  dwc_error.h�̕ύX�ɂƂ��Ȃ��C��

  Revision 1.17  2006/05/15 01:18:13  nakata
  5/12�ł�DWC_Nd���C�u�����ɑΉ�

  Revision 1.16  2006/05/12 11:24:48  nakata
  ��؂���̎w�E�ɑΉ�

  Revision 1.15  2006/05/10 11:50:58  nakata
  Nd���C�u�����̎d�l�ύX�ɔ����ď������֐���ύX

  Revision 1.14  2006/05/10 04:45:34  nakata
  ���������ɐݒ肳���DWC�G���[���������Ȃ����������C��

  Revision 1.13  2006/05/02 00:57:07  nakata
  �_�~�[�g�[�N�����g�p����悤�ɕύX

  Revision 1.12  2006/05/02 00:27:10  nakata
  DWC_Nd��5��2���łɍX�V

  Revision 1.11  2006/04/28 13:14:03  nakata
  SVL�擾���s���̃G���[���l�b�g���[�N�G���[�ɕύX

  Revision 1.10  2006/04/28 12:23:27  nakata
  �萔���ύX�ɔ����C��

  Revision 1.9  2006/04/28 10:08:43  nakata
  DWC_NdInitAsync��password�ǉ�
  DWC_NdGetFileAsync�̎d�l���ύX�ɔ����֐��ύX
  �e��萔���K�C�h���C���ɉ��������̂ɕύX����w�b�_��ǉ�

  Revision 1.8  2006/04/28 08:12:06  nakata
  SVL�擾�X���b�h���I�����Ȃ��o�O���C��

  Revision 1.7  2006/04/27 12:46:24  takayama
  svc(SVC) -> svl(SVL)

  Revision 1.6  2006/04/27 05:27:54  nakata
  �G���[�R�[�h�֘A�̏C��
  DWC�G���[�������ɂ͊e�֐��̎��s��s�\�ɂ���

  Revision 1.5  2006/04/26 10:52:39  nakata
  DWC_Nd���G���[�R�[�h��ݒ肷��悤�ɕύX

  Revision 1.4  2006/04/26 08:35:13  nakata
  DWC_Nd��dwc.h�����C���N���[�h����Γ��삷��悤�ύX

  Revision 1.3  2006/04/26 05:09:13  nakata
  ���������s���Ƀ��������������Ȃ��o�O���C��

  Revision 1.2  2006/04/26 04:19:23  nakata
  Nd/NHTTP��4��26���łɍX�V

  Revision 1.1  2006/04/25 11:46:46  nakata
  DWC_Nd���b�p��ǉ�



  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include <base/dwc_report.h>
#include <base/dwc_memfunc.h>
#include <base/dwc_nasfunc.h>
#include <base/dwc_error.h>
#include <base/dwc_nd.h>
#include <nd/DWCi_Nd.h>
#include <auth/dwc_svl.h>
#include <auth/util_base64.h>

/* -------------------------------------------------------------------------
        pre-processing switch
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        define
   ------------------------------------------------------------------------- */

// [nakata] Nd���C�u�������g�p����SVL
#define DWC_ND_SVL				"9000"
#define SVLTHREADSTACKSIZE		2048
#define SVLHOSTNAMEMAX			255
#define GAMECDMAX				32
#define PASSWDMAX				32
#define TOKEN_LENGTH			(300/3*4)
/* -------------------------------------------------------------------------
        macro
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        struct
   ------------------------------------------------------------------------- */

// [nakata] DWCNd�̊e�p�����[�^��ۑ����邽�߂̍\����
typedef struct {
	// [nakata] ���������p�����[�^
	DWCNdCallback			callback;
	DWCNdCleanupCallback	cleanupcallback;
	char					gamecd[GAMECDMAX];
	char					passwd[PASSWDMAX];
	
	// [nakata] DWCi_Nd�������t���O
	BOOL			ndinitialized;
	
	// [nakata] SVL�g�[�N���i�[�̈�
	DWCSvlResult	svlresult;
	
	// [nakata] SVL�擾�҂��X���b�h
	u8				svlthreadstack[SVLTHREADSTACKSIZE];
	OSThread		svlthread;
} DWCNdCnt;

/* -------------------------------------------------------------------------
        variable
   ------------------------------------------------------------------------- */

// [nakata] Init���̊e�p�����[�^��ۑ����邽�߂̗̈�
static DWCNdCnt *dwcndcnt = NULL;

/* -------------------------------------------------------------------------
        prototype
   ------------------------------------------------------------------------- */

//


/* -------------------------------------------------------------------------
        function - internal
   ------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:			DWCi_NdAlloc

  Description:	Nd���C�u�������Ŏg�p����郁�����Ǘ��֐�

  Arguments:	size  - �m�ۂ��郁�����u���b�N�̗e��
				align - �m�ۂ��郁�����u���b�N�̃A���C�������g
  Returns:		NULL�ȊO - �m�ۂ��ꂽ�������u���b�N�ւ̃|�C���^
				NULL     - �������m�ێ��s
 *---------------------------------------------------------------------------*/
static void *DWCi_NdAlloc(u32 size, int align)
{
	return DWC_AllocEx(DWC_ALLOCTYPE_ND, size, align);
}

/*---------------------------------------------------------------------------*
  Name:			DWCi_NdFree

  Description:	Nd���C�u�������Ŏg�p����郁�����Ǘ��֐�

  Arguments:	ptr - �������郁�����u���b�N�̗e��
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
static void DWCi_NdFree(void *ptr)
{
	DWC_Free(DWC_ALLOCTYPE_ND, ptr, 0);
}

/*---------------------------------------------------------------------------*
  Name:			DWCi_NdCallback

  Description:	Nd���C�u�����R�[���o�b�N������I�ɏ������G���[�R�[�h�̐ݒ�Ȃǂ��s���֐�

  Arguments:	DWCNdCallback�^���Q��
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
static void DWCi_NdCallback(DWCNdCallbackReason reason, DWCNdError error, int servererror)
{
	SDK_ASSERT(dwcndcnt);
	SDK_ASSERT(dwcndcnt->callback);
	
	// [nakata] �G���[���Ȃ��Ȃ�΂��̂܂܃R�[���o�b�N�֐����Ăяo��
	if(error == DWC_ND_ERROR_NONE) {
		dwcndcnt->callback(reason, error, servererror);
		return;
	}
	
	// [nakata] �G���[���������Ă���ꍇ��DWCi_SetError���ĂԕK�v������
	switch(error) {
	case DWC_ND_ERROR_ALLOC:
		DWCi_SetError(DWC_ERROR_FATAL, DWC_ECODE_SEQ_ADDINS + DWC_ECODE_FUNC_ND + DWC_ECODE_TYPE_ND_ALLOC);
		break;
	case DWC_ND_ERROR_BUSY:
		OS_Panic("You can't request more than one download process to DWC_Nd library.");
		break;
	case DWC_ND_ERROR_HTTP:
		DWCi_SetError(DWC_ERROR_ND_HTTP, DWC_ECODE_SEQ_ADDINS + DWC_ECODE_FUNC_ND + DWC_ECODE_TYPE_ND_HTTP);
		break;
	case DWC_ND_ERROR_BUFFULL:
		DWCi_SetError(DWC_ERROR_ND_ANY, DWC_ECODE_SEQ_ADDINS + DWC_ECODE_FUNC_ND + DWC_ECODE_TYPE_ND_BUFFULL);
		break;
	case DWC_ND_ERROR_DLSERVER:
		DWCi_SetError(DWC_ERROR_ND_ANY, DWC_ECODE_SEQ_ADDINS + DWC_ECODE_FUNC_ND - servererror);
		break;
	case DWC_ND_ERROR_CANCELED:
		break;
	case DWC_ND_ERROR_PARAM:
		OS_Panic("Some Parameters passed to DWC_Nd library are invalid.");
		break;
	case DWC_ND_ERROR_FATAL:
		DWCi_SetError(DWC_ERROR_FATAL, DWC_ECODE_SEQ_ADDINS + DWC_ECODE_FUNC_ND + DWC_ECODE_TYPE_ND_FATAL);
		break;
	default:
		OS_Panic("DWCi_NdCallback: Unknown error type.");
	}
	
	// [nakata] �R�[���o�b�N�֐����Ăяo��
	dwcndcnt->callback(reason, error, servererror);
}

/*---------------------------------------------------------------------------*
  Name:			DWCi_NdCleanupCallback

  Description:	Nd���C�u�����N���[���A�b�v�R�[���o�b�N������I�ɏ������G���[�R�[�h�̐ݒ�Ȃǂ��s���֐�

  Arguments:	DWCNdCleanupCallback�^���Q��
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
static void DWCi_NdCleanupCallback(void)
{
	DWCNdCleanupCallback callback;
	
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] �X���b�h��Join
	OS_JoinThread(&dwcndcnt->svlthread);
	
	// [nakata] ���[�U�[�ݒ肵���R�[���o�b�N��ۑ����Ă���
	callback = dwcndcnt->cleanupcallback;
	
	// [nakata] ���������(������dwcndcnt��NULL�ɂȂ邽��dwcndcnt->ndinitialized�̃N���A�͕K�v�Ȃ�)
	DWCi_NdFree(dwcndcnt);
	dwcndcnt = NULL;
	
	// [nakata] ���[�U�[�ݒ肵���R�[���o�b�N���Ăяo��
	if(callback != NULL)
		callback();
}

/* -------------------------------------------------------------------------
        function - external
   ------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:			DWCi_SVLTokenWaitThread

  Description:	SVL�g�[�N���擾�҂��X���b�h�֐�

  Arguments:	�Ȃ�
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
static void DWCi_SVLTokenWaitThread(void *arg)
{
#pragma unused(arg)
	DWCSvlState	svlstate;
	char		url[SVLHOSTNAMEMAX+1];
	char		token[TOKEN_LENGTH];
	int			errorcode;

	SDK_ASSERT(dwcndcnt);
	SDK_ASSERT(dwcndcnt->callback);
	
	for(;;) {
		svlstate = DWC_SVLProcess();
		if(svlstate == DWC_SVL_STATE_SUCCESS) {
			// [nakata] SVL�g�[�N���擾�����ANd���C�u�����������J�n
			if(strlen(dwcndcnt->svlresult.svlhost)+strlen("https:///download") > SVLHOSTNAMEMAX) {
				DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_SVLTokenWaitThread: DWCi_NdStartup too long svlhost.\n");
				DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_FATAL, -1);
				return;
			}
			OS_SNPrintf(url, SVLHOSTNAMEMAX+1, "https://%s/download", dwcndcnt->svlresult.svlhost);
			
			// [nakata] SVL���g�[�N����base64�f�R�[�h���ĕԂ��悤�ɕύX���ꂽ���߁ADWCi_Nd�ɓn���O��base64�G���R�[�h����
			if(DWC_Auth_Base64Encode(dwcndcnt->svlresult.svltoken, strlen(dwcndcnt->svlresult.svltoken), token, TOKEN_LENGTH) < 0) {
				DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_SVLTokenWaitThread: Can't encode SVL token.\n");
				DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_FATAL, -1);
				return;
			}
			
			if(DWCi_NdStartup(DWCi_NdAlloc, DWCi_NdFree, url, token, dwcndcnt->gamecd, dwcndcnt->passwd, DWCi_NdCallback) == FALSE) {
				// [nakata] Nd���C�u�������������s
				DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_SVLTokenWaitThread: DWCi_NdStartup failed.\n");
				DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_FATAL, -1);
				return;
			}
			
			// [nakata] nd���C�u��������������
			DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_SVLTokenWaitThread: DWCi_NdStartup successed.\n");
			dwcndcnt->ndinitialized = TRUE;
			DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_NONE, -1);
			return;
		}
		else if(svlstate == DWC_SVL_STATE_ERROR) {
			// [nakata] SVL�g�[�N���擾���s
			// [nakata] DWC�G���[�͐ݒ�ς�
			DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_SVLTokenWaitThread: Can't get SVL token.\n");
			
			// [nakata] �R�[���o�b�N�֐����Ăяo��
			(void)DWC_GetLastError(&errorcode);
			DWCi_SetError(DWC_ERROR_ND_ANY, errorcode);
			dwcndcnt->callback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_HTTP, -1);
			
			// [nakata] DWC�G���[�͐ݒ�ς݂����ANd���C�u������SVL�ƈ���đΏ��Ƃ��ĉ�����Ȃ���΂Ȃ�Ȃ�����
			//          �G���[���㏑��
			//DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_HTTP, -1);
			return;
		}
		else if(svlstate == DWC_SVL_STATE_CANCELED) {
			// [nakata] SVL�g�[�N���擾�L�����Z��
			//DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_HTTP, -1);
			
			// [nakata] DWC�G���[�͐ݒ�ς݂����ANd���C�u�����L�����Z�������Ƃ��ăG���[���㏑��
			DWCi_NdCallback(DWC_ND_CBREASON_INITIALIZE, DWC_ND_ERROR_CANCELED, -1);
		}
		OS_Sleep(100);
	}
	return;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdInitAsync

  Description:	Nd���C�u�����������֐�

  Arguments:	callback - Nd���C�u�������Ăяo���ʒm�R�[���o�b�N�֐��ւ̃|�C���^
                password - �_�E�����[�h�T�[�o�ւ̃A�N�Z�X�ɕK�v�ȃp�X���[�h
  Returns:		TRUE  - ����������
				FALSE - ���������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdInitAsync(DWCNdCallback callback, char *gamecd, char *passwd)
{
	SDK_ASSERT(callback);
	SDK_ASSERT(dwcndcnt == NULL);
	SDK_ASSERT(gamecd);
	SDK_ASSERT(passwd);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}
	
	// [nakata] �R���g���[���\���̏�����
	dwcndcnt = DWCi_NdAlloc(sizeof(DWCNdCnt), 32);
	SDK_ASSERT(dwcndcnt);
	dwcndcnt->ndinitialized = FALSE;
	
	MI_CpuClear8(dwcndcnt, sizeof(DWCNdCnt));
	dwcndcnt->callback	= callback;
	strncpy(dwcndcnt->gamecd, gamecd, GAMECDMAX-1);
	strncpy(dwcndcnt->passwd, passwd, PASSWDMAX-1);
	
	// [nakata] SVL�g�[�N���擾�J�n
	if(DWC_SVLGetTokenAsync(DWC_ND_SVL, &dwcndcnt->svlresult) == FALSE) {
		// [nakata] SVL�g�[�N���擾�����J�n���s
		// [nakata] DWC�G���[��SVL�����Őݒ�ς�
		DWC_Printf(DWC_REPORTFLAG_ERROR, "DWC_NdInitAsync: DWC_SVLGetTokenAsync failed.\n");
		
		// [nakata] ���������
		DWCi_NdFree(dwcndcnt);
		dwcndcnt = NULL;
		return FALSE;
	}
	DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_NdInitAsync: DWC_SVLGetTokenAsync started.\n");
	
	// [nakata] SVL�g�[�N����M�҂��X���b�h�N��
	OS_CreateThread(&dwcndcnt->svlthread, DWCi_SVLTokenWaitThread, NULL, dwcndcnt->svlthreadstack+SVLTHREADSTACKSIZE, SVLTHREADSTACKSIZE, 16);
	OS_WakeupThreadDirect(&dwcndcnt->svlthread);
	
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdCleanupAsync

  Description:	Nd���C�u��������֐�

  Arguments:	callback - Nd���C�u�������Ăяo����������ʒm�R�[���o�b�N�֐��ւ̃|�C���^
  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_NdCleanupAsync(DWCNdCleanupCallback callback)
{
	if(dwcndcnt == NULL)
		return FALSE;
	
	dwcndcnt->cleanupcallback = callback;

	if(dwcndcnt->ndinitialized == FALSE)
		DWCi_NdCleanupCallback();
	else
		DWCi_NdCleanupAsync(DWCi_NdCleanupCallback);
	return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdSetAttr

  Description:	�_�E�����[�h�������ݒ�֐�

  Arguments:	attr1 - ����������1
				attr2 - ����������2
				attr3 - ����������3
  Returns:		TRUE  - �����ݒ萬��
				FALSE - �����ݒ莸�s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdSetAttr(const char* attr1, const char* attr2, const char* attr3)
{
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}
	
	// [nakata] �Ԃ�l�ɂ���ăG���[�R�[�h��ݒ肷��
	return DWCi_NdSetAttr((char *)attr1, (char *)attr2, (char *)attr3);
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetFileListNumAsync

  Description:	�t�@�C�����X�g���G���g�����擾�����J�n�֐�

  Arguments:	entrynum - �ʐM�������Ƀt�@�C�����X�g���G���g�������i�[�����̈�ւ̃|�C���^
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileListNumAsync(int* entrynum)
{
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}

	return DWCi_NdGetFileListNumAsync(entrynum);
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdFileListAsync

  Description:	�t�@�C�����X�g�擾�����J�n�֐�

  Arguments:	filelist - �擾���ꂽ�t�@�C�����X�g���i�[�����z��ւ̃|�C���^
				offset   - �擾����t�@�C�����X�g�̊J�n�ʒu�̃I�t�Z�b�g
				num      - �擾����t�@�C�����X�g�̍ő咷
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileListAsync(DWCNdFileInfo* filelist, unsigned  offset, unsigned int num)
{
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}
	
	// [nakata] �t�@�C�����X�g�̈��0�N���A
	MI_CpuClear8(filelist, sizeof(DWCNdFileInfo)*num);
	
	return DWCi_NdGetFileListAsync(filelist, (int)offset, (int)num);
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetFileAsync

  Description:	�t�@�C���擾�����J�n�֐�

  Arguments:	filename - �擾����t�@�C���̖��O
				buf      - �擾���ꂽ�t�@�C���̃f�[�^���i�[�����̈�ւ̃|�C���^
				size     - �擾���ꂽ�t�@�C���̃f�[�^���i�[�����̈�̃T�C�Y
  Returns:		TRUE  - �ʐM���J�n���ꏈ�������҂����
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetFileAsync(DWCNdFileInfo *fileinfo, char* buf, unsigned int size)
{
#pragma unused(size)
	SDK_ASSERT(dwcndcnt);
	SDK_ASSERT(fileinfo);
	SDK_ASSERT(buf);
	SDK_ASSERT(size >= DWC_ND_RECVBUF_MIN);
	SDK_ASSERT(size >= fileinfo->size);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}
	
	return DWCi_NdGetFileAsync(fileinfo, buf, size);
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdCancelAsync

  Description:	���s�������̒��f�ʒm�֐�

  Arguments:	�Ȃ�
  Returns:		TRUE  - �������f�ɐ������������邢�͏��������s���Ă��Ȃ�
				FALSE - �������s
 *---------------------------------------------------------------------------*/
BOOL DWC_NdCancelAsync(void)
{
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}
	
	return DWCi_NdCancelAsync();
}

/*---------------------------------------------------------------------------*
  Name:			DWC_NdGetProgress

  Description:	���s�������̐i�s�x�擾�֐�

  Arguments:	received   - ��M�ς݃f�[�^�̃o�C�g�����i�[����̈�ւ̃|�C���^
				contentlen - ��M���̃f�[�^��Content-Length���i�[����̈�ւ̃|�C���^
  Returns:		TRUE  - �i�s�x�擾����
				FALSE - �i�s�x�擾���s�A���邢�͏��������s���Ă��Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_NdGetProgress(u32* received, u32* contentlen)
{
	SDK_ASSERT(dwcndcnt);
	
	// [nakata] DWC�G���[���������Ă���ꍇ�͂͂���
	if(DWCi_IsError()) {
		return FALSE;
	}

	return DWCi_NdGetProgress(received, contentlen);
}
