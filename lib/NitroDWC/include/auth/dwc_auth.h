/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - dwc_auth
  File:     dwc_auth.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_auth.h,v $
  Revision 1.42  2006/06/13 00:00:22  nakata
  �u���E�U�����̃G���[���ڒǉ�

  Revision 1.41  2006/04/27 07:06:09  takayama
  ingamesn��10��������25�����Ɋg���B

  Revision 1.40  2006/03/09 04:06:36  nakata
  �ꕔ���s�R�[�h������łȂ������������C��

  Revision 1.39  2006/03/02 05:21:58  nakata
  DWC_Svc���W���[���ǉ�
  DWC_Util_Alloc/DWC_Util_Free�֐��̖��O��DWC_Auth_Alloc/DWC_Auth_Free�֐��ɖ��O��ύX

  Revision 1.38  2006/03/01 02:43:53  nakata
  CAL�擾�@�\��dwc_auth.c���番������

  Revision 1.37  2006/02/24 05:31:08  nakata
  �K�v�Ȃ��R�����g���폜�����B

  Revision 1.36  2006/01/17 07:15:38  nakata
  Fixed the bug related to determining ingamesn length in DWCi_RemoteLoginProcess().

  Revision 1.35  2005/12/26 06:57:43  nakata
  Changed DWC_AUTH_INGAMESN_NOTCHECKED to DWC_AUTH_INGAMESN_NOT_CHECKED.
  Changed DWC_INGAMESN_NOTCHECKED to DWC_INGAMESN_NOT_CHECKED.

  Revision 1.34  2005/12/26 06:43:19  nakata
  Moved ingamesn check functions from dwc_common.c to dwc_login.c.

  Revision 1.33  2005/12/26 04:23:17  nakata
  Added NAS return code 40 related functions to Auth.
  Added DWC_GetIngamesnCheckResult() to dwc_common.c/dwc_common.h.
  Fixed the bug that incorrect ingamesn is sent when auth is called in netcheck.

  Revision 1.32  2005/12/21 12:02:19  nakata
  Rollback Auth related code to versiont "2005-11-21 08:00:00"
  Fixed retry times count bug in DWC_Auth.

  Revision 1.21  2005/11/17 07:52:34  nakata
  Now DWC_Auth/DWC_Netcheck/DWC_Http don't use DWC_Alloc/DWC_Free.
  DWC_Netcheck sends whole HTTP packet including HTTP header to NAS
  when receives 302 message from conntest.

  Revision 1.20  2005/10/07 13:10:53  nakata
  Added SSL priority setting code to DWC_Http. But disabled by DWC_HTTP_SETSSLPRIO switch.
  Fixed ingamesn length detection part. It used strlen and now it uses wcslen for ingamesn.

  Revision 1.19  2005/09/07 08:01:02  nakata
  Modified DWC_Auth_GetError and DWC_Auth_GetResult to work when DWC_Auth
  is not initialized

  Revision 1.18  2005/08/22 11:48:02  nakata
  Add runtime custom nas setting function to DWC_Auth.

  Revision 1.17  2005/08/20 07:01:18  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_AUTH_H_
#define DWC_AUTH_H_

#include <nitro.h>

#include "dwc_http.h"

#include "ac/dwc_ac.h"

#ifdef __cplusplus
extern "C" {
#endif // [nakata] __cplusplus

//=============================================================================
// [nakata] ��`
//=============================================================================

// [nakata] �萔��`
#define DWCAUTH_INGAMESN_LEN	(25+1)
#define DWCAUTH_GSBRCDBYTES		(11+1)

// [nakata] DWCAuth�̌��ʂ��i�[����\����

// [nakata] NAS�G���[�R�[�h�ꗗ
// 0�܂��͐��̒l	����
//
// -20100	DS���Ŕ��������G���[(�������s��/���C�u�������������s�Ȃ�)�ɂ��F�؂������ł��Ȃ�
// -20101	�T�[�o���ňȉ��ɒ�`�������̈ȊO�̃G���[���������F�؂������ł��Ȃ�
// -20102	���M����MAC�A�h���X��NAS��BAN���X�g�ɓ����Ă���
// -20103	���O�C��/�V�K���[�U�[�쐬�Ɏg�p�������[�U�[�����璊�o����MAC�A�h���X��DS�̂��̂łȂ�
// -20104	ID�����łɎg�p����Ă���AID��ω������ă��g���C�����Ă��V�K�쐬�Ɏ��s����
// -20105	ID�����݂��Ȃ����߃��O�C���ł��Ȃ�
// -20106	����DS�����邱�Ƃ��ł���ID���̏���𒴂���ID����낤�Ƃ��Ă���
// -20107	���M�����Q�[���R�[�h��NAS�ɓo�^����Ă��Ȃ�
// -20108	ID�̎g�p�������؂�Ă���
// -20109	DS����K�v�ȏ�񂪑����Ă��Ă��Ȃ��A�s���A�N�Z�X�^��
// -20110	���M�����Q�[���R�[�h�ł�WiFi�R�l�N�V�����T�[�r�X���I�����Ă���
//
// -20200	NAS�����v���Z�X�G���[
// -20301	NAS�@�\������
//
// -20403	�{�������Ȃ�(HTTP�T�[�o�G���[)
// -20404	�{���Ώۃt�@�C���Ȃ�(HTTP�T�[�o�G���[)
// -20500	�T�[�o�C���^�[�i���G���[(HTTP�T�[�o�G���[)
// -20503	HTTP�T�[�o�����G���[/RADIUS�G���[(HTTP�T�[�o�G���[)

typedef struct
{
	int returncode;			// [nakata] NAS�G���[�R�[�h
	
	// [nakata] �F�؂̌��ʓ���ꂽ�e����(������)
	char retcode[3+1];		// [nakata] HTTP���U���g
	char datetime[14+1];	// [nakata] YYYYMMDDHHMMSS�`���̃T�[�o����
	char locator[50+1];		// [nakata] GameSpy�T�[�o�̃h���C����
	char token[300+1];		// [nakata] �g�[�N��������
	char challenge[8+1];	// [nakata] �`�������W�R�[�h
	char cookie[64+1];		// [nakata] �N�b�L�[
} DWCAuthResult;

// [nakata] DWCAuth�̃G���[��Ԃ������񋓌^
typedef enum
{
	DWCAUTH_E_NOERR,				// [nakata] �������/������
	DWCAUTH_E_BMINITERR,			// [nakata] DS��Flash�A�N�Z�X�p���C�u�������������s
	DWCAUTH_E_MEMERR,				// [nakata] �������m�ێ��s
	DWCAUTH_E_ACERR,				// [nakata] �����ڑ����C�u��������ڑ��󋵂��擾�ł��Ȃ�
	DWCAUTH_E_HTTPINITERR,			// [nakata] DWCHttp���������ł��Ȃ�
	DWCAUTH_E_CALERR,				// [nakata] CAL���擾�Ɏ��s����
	DWCAUTH_E_INVALIDPARAM,			// [nakata] �s���ȃp�����[�^���^�����Ă���
	DWCAUTH_E_CANTADDHTTPHEADER,	// [nakata] HTTP�p�P�b�g�Ƀw�b�_�A�C�e����ǉ��ł��Ȃ�
	DWCAUTH_E_CANTADDPOSTITEM,		// [nakata] HTTP�p�P�b�g��POST�A�C�e����ǉ��ł��Ȃ�
	DWCAUTH_E_DNSERR,				// [nakata] DNS�G���[
	DWCAUTH_E_CANTSTARTHTTPTHREAD,	// [nakata] HTTP�X���b�h���J�n�ł��Ȃ�
	DWCAUTH_E_HTTPCONERR,			// [nakata] TCP�ڑ����m���ł��Ȃ�
	DWCAUTH_E_HTTPPARSEERR,			// [nakata] ��M����HTTP�p�P�b�g���s���Ńp�[�X�ł��Ȃ�
	DWCAUTH_E_HTTPERR,				// [nakata] DWCHttp�Ɋւ��関���ނ̃G���[���������Ă���
	DWCAUTH_E_NASPARSEERR,			// [nakata] ��M����NAS�̉������s���Ńp�[�X�ł��Ȃ�
	DWCAUTH_E_BMERR,				// [nakata] DS��Flash����Ɏ��s����
	DWCAUTH_E_NASALREADYEXIST,		// [nakata] NAS��ɍ�낤�Ƃ����A�J�E���g�����ɑ��݂��Ă���
	DWCAUTH_E_NASACCTEXPIRE,		// [nakata] ���O�C�����悤�Ƃ����A�J�E���g��NAS��Ŋ����؂ꂾ����
	DWCAUTH_E_NASERR,				// [nakata] NAS����G���[���A���Ă��Ă���
	DWCAUTH_E_TOOMANYRETRY,			// [nakata] ���g���C����������̂ŃG���[
	DWCAUTH_E_ABORT,
	DWCAUTH_E_FINISH,				// [nakata] �F�؂ɐ�������
	DWCAUTH_E_NOTINITIALIZED,		// [nakata] ����������Ă��Ȃ�
	DWCAUTH_E_MAX
} DWCAuthError;

// [nakata] DWC_Auth�����������邽�߂ɕK�v�ȏ����i�[����\����
typedef struct
{
	// [nakata] �����̃p�����[�^�͑S��NULL�I�[���Ă���K�v������
	unsigned short ingamesn[DWCAUTH_INGAMESN_LEN];	// [nakata] �Q�[�����X�N���[����
	char           gsbrcd[DWCAUTH_GSBRCDBYTES];		// [nakata] Gamespy ID Branch Code
	
	// [nakata] �������Ǘ��֐�
	DWCAuthAlloc	alloc;
	DWCAuthFree 	free;
} DWCAuthParam;

// [nakata] ingamesn�`�F�b�N���ʗ񋓎q
typedef enum
{
	DWC_AUTH_INGAMESN_NOT_CHECKED	= 0,
	DWC_AUTH_INGAMESN_VALID			= 1,
	DWC_AUTH_INGAMESN_INVALID		= 2
} DWCAuthIngamesnCheckResult;

extern DWCAuthIngamesnCheckResult DWCauthingamesncheckresult;

//=============================================================================
// [nakata] �֐�
//=============================================================================

//-----------------------------------------------------------------------------
// �@�\ : DWC_Auth�Ŏg�p����J�X�^��NAS�A�h���X��ݒ肷��
// ���� : nasaddr - �g�p����NAS��URL������
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Auth_SetCustomNas(const char *nasaddr);

//-----------------------------------------------------------------------------
// �@�\ : DWCAuth�Ŏg�p����\���̂̏�����/�������m��
// ���� : param - �������p�����[�^
//      : http  - DWCAuth���g�p����DWCHttp�\���̂ւ̃|�C���^
// �Ԓl : DWCAuthError
//-----------------------------------------------------------------------------
DWCAuthError DWC_Auth_Create(DWCAuthParam *param, void *http);
DWCAuthError DWC_Auth_Create_WFCUtil(DWCAuthParam *param, void *http);

//-----------------------------------------------------------------------------
// �@�\ : DWCAuth�A�{�[�g
//        
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Auth_Abort(void);

//-----------------------------------------------------------------------------
// �@�\ : DWCAuth�Ŏg�p�����������̉��/�l�b�g���[�N�ؒf
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Auth_Destroy(void);

//-----------------------------------------------------------------------------
// �@�\ : DWCAuth�X���b�h���I������̂�OS_JoinThread���đ҂�
// ���� : �Ȃ�
// �Ԓl : �Ȃ�
//-----------------------------------------------------------------------------
void DWC_Auth_Join(void);

//-----------------------------------------------------------------------------
// �@�\ : �r���I��DWCAuth�̃G���[�R�[�h���擾����
//        (DWCAuth�̏�Ԃ��|�[�����O����ꍇ�ɂ��g����)
// ���� : �Ȃ�
// �Ԓl : DWCAuthError
//-----------------------------------------------------------------------------
DWCAuthError DWC_Auth_GetError(void);

//-----------------------------------------------------------------------------
// �@�\ : Mutex���g�p������DWCAuth�̌��ʂ��擾����
//        (Auth�X���b�h���I�����Ă���g�����ƁADWC_Auth_Destroy�O�Ɏg������)
// ���� : �Ȃ�
// �Ԓl : DWCAuthResult
//-----------------------------------------------------------------------------
void DWC_Auth_GetResult(DWCAuthResult *result);

#ifdef __cplusplus
}
#endif // [nakata] __cplusplus


#endif // DWC_AUTH_H_
