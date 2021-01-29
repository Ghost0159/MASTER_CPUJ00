/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - dwc_netcheck
  File:     dwc_netcheck.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_netcheck.h,v $
  Revision 1.19  2006/11/07 04:54:35  nakata
  DWC_SimpleNetcheck�n�̒ǉ�

  Revision 1.18  2006/04/06 09:21:33  nakata
  302�G���[�𖳎����郂�[�h��ݒ肷��֐���ǉ��B

  Revision 1.17  2006/04/04 07:47:35  nakata
  ���s�R�[�h�C���B

  Revision 1.16  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX

  Revision 1.15  2005/12/21 12:02:19  nakata
  Rollback Auth related code to versiont "2005-11-21 08:00:00"
  Fixed retry times count bug in DWC_Auth.

  Revision 1.12  2005/09/12 06:19:29  nakata
  Added DWC_Netcheck_SetCustomBlacklist function.
  Refined DWC_Http timeout/receive finish detection.
  Modified some Makefile for successful compilation.

  Revision 1.11  2005/08/20 10:28:07  sasakit
  �s����\���Ă���s�̑΍�B

  Revision 1.10  2005/08/20 07:01:18  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_NETCHECK_H_
#define DWC_NETCHECK_H_
/*****************************************************************************/
/*                                                                           */
/*  �ڑ������؃��C�u���� DWC_Netcheck �w�b�_�t�@�C��                         */
/*      dwc_netcheck.h                                                       */
/*          ver. 0.5                                                         */
/*                                                                           */
/*****************************************************************************/

#include "dwc_auth.h"

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] ��`
//=============================================================================

// [nakata] DWCNetcheck�̃G���[��Ԃ������񋓌^
typedef enum {
	DWCNETCHECK_E_NOERR,			// [nakata] �������/������

	DWCNETCHECK_E_HTTPINITERR,		// [nakata] DWCHttp���������ł��Ȃ�
	DWCNETCHECK_E_HTTPPARSEERR,		// [nakata] ��M����HTTP�p�P�b�g���s���Ńp�[�X�ł��Ȃ�
	DWCNETCHECK_E_HTTPERR,			// [nakata] DWCHttp�Ɋւ��関���ނ̃G���[���������Ă���
	DWCNETCHECK_E_MEMERR,			// [nakata] �������m�ێ��s
	DWCNETCHECK_E_AUTHINITERR,		// [nakata] DWCAuth���������ł��Ȃ�
	DWCNETCHECK_E_AUTHERR,			// [nakata] DWCAuth�Ɋւ��関���ނ̃G���[���������Ă���
	DWCNETCHECK_E_302TWICE,			// [nakata] Wayport�F�؂��s�����̂�302�G���[���o��
	DWCNETCHECK_E_CANTADDPOSTITEM,	// [nakata] HTTP�p�P�b�g��POST�A�C�e����ǉ��ł��Ȃ�
	DWCNETCHECK_E_NASPARSEERR,		// [nakata] NAS�p�P�b�g�Ƃ��Đ���Ƀp�[�X�ł��Ȃ�
	
	DWCNETCHECK_E_NETUNAVAIL,		// [nakata] (DWCNetcheck����)�l�b�g���[�N�ڑ��s�\ 
	DWCNETCHECK_E_NETAVAIL,			// [nakata] (DWCNetcheck����)�l�b�g���[�N�ڑ��\ 

	DWCNETCHECK_E_MAX
} DWCNetcheckError;

// [nakata] DWCNetcheck�̏������p�����[�^
typedef struct
{
	DWCAuthAlloc	alloc;	// [nakata] �������m�ۊ֐�
	DWCAuthFree	free;	// [nakata] ����������֐�
	
	void *bmworkarea;		// [nakata] BM�p�̈�(32bit�A���C�������g���ꂽ�ÓI�ȃ������̈�)
} DWCNetcheckParam;

// [nakata] SIMPLE_CHECK�p
typedef enum
{
	DWC_SIMPLENETCHECK_PROCESSING = 0,
	DWC_SIMPLENETCHECK_SUCCESS,
	DWC_SIMPLENETCHECK_FAIL,
	DWC_SIMPLENETCHECK_SYSFAIL,
	DWC_SIMPLENETCHECK_NOTINITIALIZED
} DWCSimpleNetcheckResult;

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : DWCNetcheck�Ŏg�p����\���̂̏�����/�������m��/�X���b�h���s
// ���� : param - �������p�����[�^
// �Ԓl : DWCNetcheckError
//-----------------------------------------------------------------------------
DWCNetcheckError DWC_Netcheck_Create(DWCNetcheckParam *param);

//-----------------------------------------------------------------------------
// �@�\ : DWCNetcheck�Ŏg�p�����������̉��/�l�b�g���[�N�ؒf
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Netcheck_Destroy(void);

//-----------------------------------------------------------------------------
// �@�\ : DWCNetcheck�X���b�h�A�{�[�g
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Netcheck_Abort(void);

//-----------------------------------------------------------------------------
// �@�\ : DWCNetcheck�X���b�h���I������̂�OS_JoinThread���đ҂�
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Netcheck_Join(void);

//-----------------------------------------------------------------------------
// �@�\ : DWC_Netcheck�̓�����|�[�����O���邽�߂̊֐�
// ���� : �Ȃ�
// �Ԓl : BOOL - DWCNETCHECK_NOERR : �܂��������I����Ă��Ȃ�
//             - ����ȊO          : Netcheck�������I�����
//-----------------------------------------------------------------------------
DWCNetcheckError DWC_Netcheck_GetError(void);

//-----------------------------------------------------------------------------
// �@�\ : Mutex���g�p������DWCNetcheck�̌��ʂ��擾����
//        (Auth�X���b�h���I�����Ă���g�����ƁADWC_Netcheck_Destroy�O�Ɏg������)
// ���� : �Ȃ�
// �Ԓl : int
//-----------------------------------------------------------------------------
int DWC_Netcheck_GetReturnCode(void);

//-----------------------------------------------------------------------------
// �@�\ : DWC_Netcheck��Blacklist�T�C�g��ݒ肷��
// ���� : const char * - Blacklist�T�C�g��URL
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Netcheck_SetCustomBlacklist(const char *url);

//-----------------------------------------------------------------------------
// �@�\ : DWC_Netcheck��302�G���[���l�b�g���[�N�ڑ��ς݂ƔF�����邩�ǂ����̃t���O��ݒ肷��
// ���� : allow302 - TRUE  - DWC_Netcheck��302�G���[��ڑ������Ƃ���
//                   FALSE - DWC_Netcheck��302�G���[��ڑ������Ƃ��Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Netcheck_SetAllow302(BOOL allow302);

//-----------------------------------------------------------------------------
// �@�\ : SIMPLE_CHECK��������
// ���� : �^�C���A�E�g����(�~���b)
// �Ԓl : TRUE - SIMPLE_CHECK���J�n����
//      : FALSE - SIMPLE_CHECK���J�n�ł��Ȃ�����
//-----------------------------------------------------------------------------
BOOL DWC_SimpleNetcheckAsync(int timeout);

//-----------------------------------------------------------------------------
// �@�\ : SIMPLE_CHECK��process����
// ���� : �Ȃ�
// �Ԓl : DWCSimpleNetcheckResult
//-----------------------------------------------------------------------------
DWCSimpleNetcheckResult DWC_SimpleNetcheckProcess(void);

#ifdef __cplusplus
}
#endif // [nakata] __cplusplus

#endif // DWC_NETCHECK_H_
