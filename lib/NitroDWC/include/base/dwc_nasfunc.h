/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_nasfunc
  File:     dwc_nasfunc.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_nasfunc.h,v $
  Revision 1.12  2006/11/17 06:57:56  nakata
  �s��������`�F�b�N�̃w�b�_��ǉ�

  Revision 1.11  2006/05/25 09:07:18  nakata
  DWC_NASLOGIN_STATE_CANCELED��ǉ�

  Revision 1.10  2006/04/27 12:46:24  takayama
  svc(SVC) -> svl(SVL)

  Revision 1.9  2006/04/05 02:15:22  nakata
  ��؂���Ɏw�E���ꂽ���_���C���B

  Revision 1.8  2006/03/27 05:49:47  nakata
  �v���t�B�b�N�XNAL��NasLogin�ɕύX�B

  Revision 1.7  2006/03/23 05:16:01  nakata
  DWC_NAL�֌W�̃R�[�h��ǉ��B

  Revision 1.6  2006/03/20 07:47:24  nakata
  SVL�̎d�l�C���ɑΉ������ύX�B

  Revision 1.5  2006/03/20 05:02:33  nakata
  *** empty log message ***

  Revision 1.4  2006/03/17 07:58:39  nakata
  �R�����g�`����ύX�B

  Revision 1.3  2006/03/09 04:11:44  nakata
  DWC_GetDateTime�֐���ǉ�

  Revision 1.2  2006/03/07 01:57:40  nakata
  DWC_GetNasTime�֐��̃��t�@�����X���}�j���A���ɒǉ��B

  Revision 1.1  2006/03/07 01:18:16  nakata
  NAS�Ɋ֘A����@�\�ǉ���dwc_nasfunc.c/dwc_nasfunc.h�ɏW�񂷂�ۂɕK�v�ȕύX���s�����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_NASFUNC_H_
#define DWC_NASFUNC_H_

#include "auth/dwc_auth.h"
#include "auth/dwc_svl.h"
#include "auth/dwc_prof.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =======================================================================
        enum
   ======================================================================== */

/* [nakata] ingamesn�`�F�b�N���ʗ񋓎q */
typedef enum {
	DWC_INGAMESN_NOT_CHECKED	= DWC_AUTH_INGAMESN_NOT_CHECKED,
	DWC_INGAMESN_VALID			= DWC_AUTH_INGAMESN_VALID,
	DWC_INGAMESN_INVALID		= DWC_AUTH_INGAMESN_INVALID
} DWCIngamesnCheckResult;

// [nakata] DWC_Nal�̃X�e�[�g�������񋓌^
typedef enum {
	DWC_NASLOGIN_STATE_DIRTY	= 0,	// ����������Ă��Ȃ�
	DWC_NASLOGIN_STATE_IDLE,			// �������s���ł͂Ȃ�
	DWC_NASLOGIN_STATE_HTTP,			// HTTP�ʐM��
	DWC_NASLOGIN_STATE_SUCCESS,		// ���O�C������
	DWC_NASLOGIN_STATE_ERROR,		// ���O�C�����s
	DWC_NASLOGIN_STATE_CANCELED,	// ���O�C���L�����Z��
	DWC_NASLOGIN_STATE_MAX
} DWCNasLoginState;

/* =======================================================================
        external function(ingamesn�֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_GetIngamesnCheckResult

  Description:	NAS�ɂ��ingamesn����̌��ʂ��擾����

  Arguments:	�Ȃ�

  Returns:		DWC_INGAMESN_NOTCHECKED	���݃`�F�b�N��(���O�C��������)
                DWC_INGAMESN_VALID		���O�̃��O�C����ingamesn��NAS�Ɏ󂯓����ꂽ
				DWC_INGAMESN_INVALID	���O�̃��O�C����ingamesn��NAS�ɋ��ۂ��ꂽ
 *---------------------------------------------------------------------------*/
DWCIngamesnCheckResult DWC_GetIngamesnCheckResult(void);

/* =======================================================================
        external function(NAS���Ԋ֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_GetIngamesnCheckResult

  Description:	NAS�̎��Ԃ�RTC�`���Ŏ擾����

  Arguments:	date	�擾���ʂ�RTCDate���������ޗ̈�ւ̃|�C���^
				time	�擾���ʂ�RTCTime���������ޗ̈�ւ̃|�C���^

  Returns:		TRUE	�擾����
				FALSE - �擾���s
 *---------------------------------------------------------------------------*/
BOOL DWC_GetDateTime(RTCDate *date, RTCTime *time);

/* =======================================================================
        external function(SVL�֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLGetToken

  Description:	�g�[�N���擾�֐�

  Arguments:	svl		�T�[�r�X��ʂ��w�肷��4������ASCII������
				result	�擾�������ʂ��i�[����̈�ւ̃|�C���^

  Returns:		TRUE	�擾�J�n
				FALSE	�擾�J�n�ł��Ȃ�
 *---------------------------------------------------------------------------*/
BOOL DWC_SVLGetTokenAsync(char *svl, DWCSvlResult *result);

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLProcess

  Description:	SVL�g�[�N���擾�����i�s�֐�

  Arguments:	�Ȃ�

  Returns:		DWCSvlState
 *---------------------------------------------------------------------------*/
DWCSvlState DWC_SVLProcess(void);

/*---------------------------------------------------------------------------*
  Name:			DWC_SVLAbort

  Description:	SVL�g�[�N���擾�����i�s�֐�

  Arguments:	�Ȃ�

  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_SVLAbort(void);

/* =======================================================================
        external function(NAS�F�؊֘A)
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginAsync

  Description:	NAS���O�C���J�n

  Arguments:	�Ȃ�

  Returns:		TRUE  - ���O�C�������J�n����
				FALSE - ���O�C�������J�n���s
 *---------------------------------------------------------------------------*/
BOOL DWC_NASLoginAsync(void);

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginProcess

  Description:	NAS���O�C�������i�s

  Arguments:	�Ȃ�

  Returns:		DWCNasLoginState
 *---------------------------------------------------------------------------*/
DWCNasLoginState DWC_NASLoginProcess(void);

/*---------------------------------------------------------------------------*
  Name:			DWC_NASLoginAbort

  Description:	�C�V���F�؃T�[�o���O�C���������f�֐�

  Arguments:	�Ȃ�

  Returns:		�Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_NASLoginAbort(void);

#ifdef __cplusplus
}
#endif

#endif // DWC_LOGIN_H_
