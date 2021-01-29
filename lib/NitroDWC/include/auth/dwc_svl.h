/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - dwc_svl
  File:     dwc_svl.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_svl.h,v $
  Revision 1.4  2006/05/29 11:21:50  nakata
  SVL�̃G���[�R�[�h��24000-27000����24000-25000�ɕύX

  Revision 1.3  2006/05/25 05:17:16  nakata
  DWC_SVL��DWC�G���[��Ԃ��@�\��ǉ��������Ƃɔ����ύX

  Revision 1.2  2006/04/27 12:46:24  takayama
  svc(SVC) -> svl(SVL)

  Revision 1.1  2006/04/27 12:31:19  takayama
  dwc_svc.h -> dwc_svl.h

  Revision 1.5  2006/03/23 00:27:01  nakata
  �X�e�[�gDWC_SVL_STATE_DIRTY��ǉ�

  Revision 1.4  2006/03/20 05:00:24  nakata
  *** empty log message ***

  Revision 1.1  2006/03/09 08:54:14  nakata
  svc�֘A�̂��̂��p�b�P�[�W�ɓ���Ȃ��悤�ύX

  Revision 1.2  2006/03/07 01:16:22  nakata
  ���s�R�[�h�C��

  Revision 1.1  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX



  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_SVL_H
#define DWC_SVL_H

#include "util_alloc.h"

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] ��`
//=============================================================================

#define DWC_SVL_ERROR_BASENUM		(-24000)
#define DWC_SVL_ERROR_HTTPBASENUM	(-25000)

#define DWC_SVL_ERROR_FATAL			(-100)
#define DWC_SVL_ERROR_BADDATA		(-101)

#define DWC_SVL_HOST_LENGTH		64
#define DWC_SVL_TOKEN_LENGTH	300
#define DWC_SVL_STATUSDATA_LENGTH	1

// [nakata] SVC�擾���̃X�e�[�g�������񋓌^
typedef enum {
	DWC_SVL_STATE_DIRTY	= 0,// ��������
	DWC_SVL_STATE_IDLE,		// �������s���ł͂Ȃ�
	DWC_SVL_STATE_HTTP,		// HTTP�ʐM��
	DWC_SVL_STATE_SUCCESS,	// �g�[�N���擾����
	DWC_SVL_STATE_ERROR,	// �g�[�N���擾���s
	DWC_SVL_STATE_CANCELED,	// �g�[�N���擾�L�����Z��
	DWC_SVL_STATE_MAX
} DWCSvlState;

// [nakata] SVC�擾���ʂ��i�[����\����
typedef struct {
	BOOL	status;
	char svlhost[DWC_SVL_HOST_LENGTH+1];
	char svltoken[DWC_SVL_TOKEN_LENGTH+1];
} DWCSvlResult;

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : DWC_Svc�Ŏg�p����̈�̊m�ۂƏ�����
// ���� : alloc             - �����Ŏg�p���郁�����m�ۊ֐��ւ̃|�C���^
//      : free              - �����Ŏg�p���郁��������֐��ւ̃|�C���^
// �Ԓl : TRUE  : ����
//        FALSE : ���s
//-----------------------------------------------------------------------------
BOOL		DWC_Svl_Init(DWCAuthAlloc alloc, DWCAuthFree free);

//-----------------------------------------------------------------------------
// �@�\ : DWC_Svc�Ŏg�p�����̈�̉��
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void		DWC_Svl_Cleanup(void);

//-----------------------------------------------------------------------------
// �@�\ : �g�[�N���擾�֐�
// ���� : svc    - �T�[�r�X��ʂ��w�肷��4������ASCII������
//      : result - �擾�������ʂ��i�[����̈�ւ̃|�C���^
// �Ԓl : TRUE  : �擾�J�n
//        FALSE : �擾�J�n�ł��Ȃ�
//-----------------------------------------------------------------------------
BOOL		DWC_Svl_GetTokenAsync(char *svc, DWCSvlResult *result);

//-----------------------------------------------------------------------------
// �@�\ : �g�[�N���擾�����i�s�֐�
// ���� : �Ȃ�
// �Ԓl : DWCSvcState
//-----------------------------------------------------------------------------
DWCSvlState	DWC_Svl_Process(void);

//-----------------------------------------------------------------------------
// �@�\ : �g�[�N���擾�������f�֐�
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void		DWC_Svl_Abort(void);

#ifdef __cplusplus
}
#endif

#endif
