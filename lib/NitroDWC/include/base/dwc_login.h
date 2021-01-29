/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - dwc_login
  File:     dwc_login.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_login.h,v $
  Revision 1.18  2007/04/04 08:46:56  nakata
  ���O�C�������ŐV�K�A�J�E���g�쐬���Ƀ��O�C�����O�A�E�g���s��Ȃ��悤�ɕύX

  Revision 1.17  2006/03/07 01:18:16  nakata
  NAS�Ɋ֘A����@�\�ǉ���dwc_nasfunc.c/dwc_nasfunc.h�ɏW�񂷂�ۂɕK�v�ȕύX���s�����B

  Revision 1.16  2006/01/23 04:50:43  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.15  2005/12/26 06:57:44  nakata
  Changed DWC_AUTH_INGAMESN_NOTCHECKED to DWC_AUTH_INGAMESN_NOT_CHECKED.
  Changed DWC_INGAMESN_NOTCHECKED to DWC_INGAMESN_NOT_CHECKED.

  Revision 1.14  2005/12/26 06:43:20  nakata
  Moved ingamesn check functions from dwc_common.c to dwc_login.c.

  Revision 1.13  2005/12/21 03:08:54  arakit
  ���O�C���̍ۂɃT�[�o�ɃQ�[�����X�N���[���l�[����n����悤�ɂ����B

  Revision 1.12  2005/11/04 05:04:49  arakit
  �F�ؗp�f�[�^���擾��������֐�DWCi_GetAuthInfo()��ǉ������B

  Revision 1.11  2005/11/02 02:49:33  arakit
  �t�@�C���̃C���N���[�h�֌W�𒲐������B

  Revision 1.10  2005/10/07 06:48:45  sasakit
  gpConnect�̃^�C���A�E�g����ꂽ�B

  Revision 1.9  2005/10/06 00:15:11  sasakit
  �ڑ�����Ԃ��擾�ł���悤�ɂ����B

  Revision 1.8  2005/10/01 10:30:49  sasakit
  ���O�C�����ɁAGS��AvailableCheck�Ɏ��s����ƁA�t���[�Y��������C���B

  Revision 1.7  2005/09/05 13:42:15  sasakit
  Login���̔F�؃G���[�̏�����ǉ��B

  Revision 1.6  2005/08/26 08:15:22  arakit
  �EASSERT�ƃ��W���[������ϐ��̃`�F�b�N�𐮗������B
  �E�O���֐�DWC_ShutdownFriendsMatch()�̈������Ƃ�Ȃ��悤�ɕύX�����B
  �܂��A���֐�����e���W���[���̐���ϐ���NULL�N���A����悤�ɂ����B
  �E�G���[�����֐���ύX���A�G���[�Z�b�g�A�G���[�`�F�b�N��K�v�ȉӏ��ɓ��ꂽ�B

  Revision 1.5  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_LOGIN_H_
#define DWC_LOGIN_H_

#include "auth/dwc_auth.h"

#ifdef __cplusplus
extern "C" {
#endif


//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define DWC_LOGIN_AUTH_TIMEOUT       ( 10 * 1000 ) // [ms]
#define DWC_LOGIN_GPCONNECT_TIMEOUT  ( 60 * 1000 ) // [ms]


//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// ���O�C���i�s�󋵗񋓎q
typedef enum {
    DWC_LOGIN_STATE_INIT = 0,         // �������
    DWC_LOGIN_STATE_REMOTE_AUTH,      // �����[�g�F�ؒ�
    DWC_LOGIN_STATE_CONNECTING,       // GP�T�[�o�ڑ���
    DWC_LOGIN_STATE_GPGETINFO,        // GP��login�����Ƃ���lastname�擾��
    DWC_LOGIN_STATE_GPSETINFO,        // GP�ɏ��߂�login�����Ƃ���lastname�ݒ�
    DWC_LOGIN_STATE_CONNECTED,        // �R�l�N�g�������
    DWC_LOGIN_STATE_NUM
} DWCLoginState;

//----------------------------------------------------------------------------
// typedef - function
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  ���O�C�������R�[���o�b�N�^
  �����@�Ferror     DWC�G���[���
          profileID �擾�ł��������̃v���t�@�C��ID
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
 *---------------------------------------------------------------------------*/
typedef void (*DWCLoginCallback)(DWCError error, int profileID, void *param);


//----------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------
// ���O�C������\����
typedef struct DWCstLoginControl
{
    GPConnection* pGpObj;       // gp�R�l�N�V�����\���̃|�C���^�ւ̃|�C���^
    DWCLoginState state;        // ���O�C���i�s��
    int  productID;             // GameSpy���z�z����Q�[���̃v���_�N�gID
    u32  gamecode;              // �C�V�����Q�[���Ɋ��蓖�Ă�gamecode
    //const char* userID;         // �F�ؗp���[�UID
    //const char* password;       // �F�ؗp�p�X���[�h
    const u16* playerName;      // �v���C���[���i�Q�[�����X�N���[���l�[���j�ւ̃|�C���^
    DWCLoginCallback callback;  // ���O�C�������R�[���o�b�N
    void *param;                // ��L�R�[���o�b�N�p�p�����[�^

    DWCUserData* userdata;      // ���[�U�f�[�^�ւ̃|�C���^

    // working
    void* bmwork;
    void* http;
    OSTick startTick;
    u32    connectFlag;
    OSTick connectTick;

    DWCAccLoginId tempLoginId;

    char authToken[GP_AUTHTOKEN_LEN];  // �F�ؗp�g�[�N��
    char partnerChallenge[GP_PARTNERCHALLENGE_LEN];  // �F�ؗp�`�������W�l

    char username[DWC_ACC_USERNAME_STRING_BUFSIZE];
    
    // [nakata] GP���O�C�������̑ޔ��ꏊ
    GPConnectResponseArg    gpconnectresponsearg;

} DWCLoginControl;


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
extern void DWCi_LoginInit(DWCLoginControl* logcnt, DWCUserData* userdata, GPConnection* pGpObj, int productID, u32 gamecode, const u16* playerName, DWCLoginCallback callback, void* param);


extern void DWCi_LoginAsync( void );


extern void DWCi_LoginProcess(void);


extern DWCAccUserData* DWCi_GetUserData(void);


extern void DWCi_StopLogin(DWCError error, int errorCode);


extern void DWCi_ShutdownLogin(void);


extern BOOL DWCi_CheckLogin( void );


extern BOOL DWCi_GetAuthInfo( char** authToken, char** partnerChallenge );


#ifdef __cplusplus
}
#endif


#endif // DWC_LOGIN_H_
