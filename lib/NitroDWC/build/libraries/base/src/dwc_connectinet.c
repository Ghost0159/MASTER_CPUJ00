/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - connectinet
  File:     dwc_connectinet.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_connectinet.c,v $
  Revision 1.55  2006/12/28 08:22:31  nakata
  DWC_TESTINET_NOT_INITIALIZED���������Ԃ���Ȃ���肪���������ߏC�������B

  Revision 1.54  2006/11/22 12:29:18  nakata
  DWC_TestInetProcess�ŃG���[�R�[�h��ݒ肷��悤�ɕύX

  Revision 1.53  2006/11/17 06:54:02  nakata
  TestInet�֘A�̒萔�̖��̕ύX

  Revision 1.52  2006/11/07 04:53:58  nakata
  DWC_TestInet�n�̒ǉ�

  Revision 1.51  2006/10/06 12:02:16  takayama
  DWC_AC_GetApType()�̒l�`�F�b�N��ǉ������B

  Revision 1.50  2006/07/27 01:03:02  nakata
  DWC_UpdateConnection�ŃG���[�R�[�h��540xx�ɐݒ肷��悤�ɕύX

  Revision 1.49  2006/06/27 06:43:52  nakata
  FriendsMatch���C�u�����Ȃ��p��DCF�Ď��֐���ǉ�

  Revision 1.48  2006/06/01 07:48:55  nakata
  FINALROM�r���h���̃��[�j���O��}��

  Revision 1.47  2006/05/25 23:54:11  uemura
  DWC_CheckWiFiStation�֐���ǉ�

  Revision 1.46  2006/04/17 07:33:40  nakata
  �u���E�U�p�z�b�g�X�|�b�g�ݒ�֐�DWC_EnableHotSpot��ǉ�

  Revision 1.45  2006/03/09 08:05:52  uemura
  DWC_SetConnectApType�̈�����int����DWCApInfoType�֕ύX

  Revision 1.44  2006/03/09 07:35:13  nakata
  DWCApInfo����spotinfo���������A�֘A������ύX

  Revision 1.43  2006/03/09 06:07:41  uemura
  DWC_DebugConnectInetAsync�̈���WEP��16�i�O�l�������Ă���Ɛ���������ł��Ȃ��o�O���C��
  ������WEPMODE��n���悤�ɕύX���܂���

  Revision 1.42  2006/03/09 04:38:38  uemura
  �R�����g�C��

  Revision 1.41  2006/03/09 02:50:37  uemura
  �R�����g�C��

  Revision 1.40  2006/03/09 02:27:17  uemura
  DWC_SetConnectApType,DWC_DebugConnectInetAsync�̒ǉ�
  ����2�֐���dwc_connect_inet_sample�փR���p�C���X�C�b�`�Ŏg�p�ł���悤�ɒǉ�����

  Revision 1.39  2006/02/24 10:31:29  sotoike
  DWC_CleanupInet �֐��ɂ�����NULL �|�C���^�ւ̃A�N�Z�X�̉\��������̂��C���B

  Revision 1.38  2006/02/23 08:21:56  nakata
  DWCApInfo����̒萔������ύX�B

  Revision 1.37  2006/02/23 06:48:34  nakata
  DWC_GetApInfo�֐����s���ȃf�[�^��Ԃ��o�O���C���B

  Revision 1.36  2006/02/23 04:32:07  sotoike
  (none)

  Revision 1.35  2006/02/23 02:55:40  nakata
  DWC_GetApInfo�֐��֘A�̃}�j���A����ǉ��B

  Revision 1.34  2006/02/23 01:02:19  nakata
  AP���擾�֐�DWC_GetApInfo���̃o�O���C��

  Revision 1.33  2006/02/23 00:53:54  nakata
  AP���擾�֐�DWC_GetApInfo��dwc_connectinet.c�ɒǉ�
  dwc_sample�̕s�K�v�ȃf�o�b�O���b�Z�[�W�\����}��

  Revision 1.32  2006/02/22 05:10:36  sotoike
  stpInetCntl ��NULL�̎�(= DWC_InitInet ���Ă�ł��Ȃ��Ƃ�)
  DWC_ProcessInet ���R�[������ƃn���O�A�b�v��������C��
  Revision 1.31  2006/02/22 04:29:44  sotoike
  DWC_InitInet �� DWC_ConnectInetAsync �̊Ԃ� DWC_GetInetStatus ���ĂԂ�
  NULL�|�C���^�A�N�Z�X�Ńn���O�A�b�v���Ă��������C���B

  Revision 1.30  2006/01/23 04:50:26  arakit
  ����p��ɍ��킹�ăR�����g���C��

  Revision 1.29  2005/12/27 09:29:54  arakit
  DWC_GetUdpPacketDropNum()���AFINALROM�r���h�ł�����ɓ��삷��悤�ɂ����B

  Revision 1.28  2005/11/07 06:18:30  sasakit
  GsStatus->OwnStatus�ɕύX�B

  Revision 1.27  2005/10/18 00:08:25  sasakit
  DWC��auth�ڑ����ɃX���b�h�v���C�I���e�B��������@�\����߂��B

  Revision 1.26  2005/10/17 07:31:44  sasakit
  CPS��SSL���s���̃X���b�h�v���C�I���e�B�𒲐�����@�\���폜�����B

  Revision 1.25  2005/10/17 06:35:56  sasakit
  SSL���s���̃X���b�h�v���C�I���e�B�𒲐�����@�\��L���������B(include�ǉ�)
  Warning�폜

  Revision 1.24  2005/10/17 06:34:33  sasakit
  SSL���s���̃X���b�h�v���C�I���e�B�𒲐�����@�\��L���������B

  Revision 1.23  2005/10/13 08:14:09  arakit
  ��v�ȊO���֐��Ăяo�����ɁA���|�[�g���x��DWC_REPORTFLAG_DEBUG��
  �f�o�b�O�v�����g�\�����s���悤�ɂ����B

  Revision 1.22  2005/10/11 02:00:12  sasakit
  �ڑ����ɃG���[�����������Ƃ��ɁA�����CleanupInet�ł��Ȃ������C���B
  SOCL_CalmDown()��SOC_Startup()�����O�ɌĂׂȂ����̏C���B(�Ăяo����AC�Ɉړ�)

  Revision 1.21  2005/10/06 05:59:34  sasakit
  SSL�̑��{���ݏ�v�Z�X���b�h�̗D�揇�ʂ�������@�\���O�����璲���ł���悤�ɂ����B

  Revision 1.20  2005/10/05 07:10:11  sasakit
  SSL�̑��{���ݏ�v�Z�X���b�h�̗D�揇�ʂ�������֐��̒ǉ��B

  Revision 1.19  2005/10/01 14:57:33  sasakit
  DWC_GetInetStatus()��NULL ptr�A�N�Z�X�łƂ܂�����C���B

  Revision 1.18  2005/10/01 12:07:01  sasakit
  ConnectInet���̏����ɖ�肪�������̂ŁA�C���B

  Revision 1.17  2005/10/01 10:56:44  sasakit
  DWC_CleanupInetAsync()���ɁADWC_ProcessInet()���Ă�ł�DISCONNECTED�ɂȂ�Ȃ��悤�ɂ����B

  Revision 1.16  2005/09/29 09:06:53  sasakit
  DWC_CleanupInetAsync()���Ȃ�ׂ��u���b�N���Ȃ��悤�ɂ����B�iSOCL_CalmDown()�Ή��j

  Revision 1.15  2005/09/27 13:22:41  sasakit
  �ڑ����ʁi�G���[��ԁj��DWC_GetLastError()�ɓ���Ȃ����������C���B

  Revision 1.14  2005/09/20 10:56:51  sasakit
  ���ɖ߂����B
  >> conntest.nintendowifi.net���������̂ŁAwww.nintendo.co.jp���Q�Ƃ���悤�ɂ����B

  Revision 1.13  2005/09/19 18:57:35  sasakit
  conntest.nintendowifi.net���������̂ŁAwww.nintendo.co.jp���Q�Ƃ���悤�ɂ����B

  Revision 1.12  2005/09/17 00:32:28  sasakit
  UDP�p�P�b�g�𗎂Ƃ��������擾����֐��̒ǉ��B
  DWC_GetLinkLevel()�̎������C���B

  Revision 1.11  2005/09/14 01:55:00  sasakit
  AP����̐ؒf�����𐮗������B

  Revision 1.10  2005/09/13 07:29:16  sasakit
  PowerMode��ACTIVE�ɕK������悤�ɂ����B

  Revision 1.9  2005/09/07 07:13:34  sasakit
  DWC_AC�֐��Q�������ɌĂяo���悤�ɃX�e�[�g�}�V������ꂽ�B
  DWC_InitInet()���d�ŌĂяo�����Ƃ��ɁA�G���[�ɂ��Ȃ��悤�ɂ����B�i��������悤�ɂ����j

  Revision 1.8  2005/09/06 09:12:09  sasakit
  ConnectInet���̃G���[�̏�����ǉ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi/socl.h>

#include <ac/dwc_ac.h>
#include <base/dwc_connectinet.h>
#include <base/dwc_memfunc.h>
#include <base/dwc_report.h>
#include <base/dwc_error.h>

#include <auth/dwc_auth.h>
#include <auth/dwc_netcheck.h>

/** --------------------------------------------------------------------
  defines
  ----------------------------------------------------------------------*/
#define DWC_CONNECTINET_DMA_DEFAULT      3
#define DWC_CONNECTINET_POWERMODE_SAVE   0
#define DWC_CONNECTINET_POWERMODE_ACTIVE 1

#define DWC_CONNECTINET_SSL_DEFAULT_PRIORITY 20

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static DWCInetControl* stpInetCntl;

/*---------------------------------------------------------------------------*
  Name:         DWC_InitInetEx

  Description:  �C���^�[�l�b�g�ڑ��Ǘ��̏�����

  Arguments:    inetCntl     : �C���^�[�l�b�g�ڑ��Ǘ��p�̍\���̂ւ̃|�C���^
                dmaNo        : �g�p����dma�ԍ�(wm�Ŏg�p)
                powerMode    : WM�̃p���[���[�h�̐ݒ�
                ssl_priority : ssl thread�̃v���C�I���e�B

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_InitInetEx( DWCInetControl* inetCntl, u16 dmaNo, u16 powerMode, u16 ssl_priority )
{
#ifdef SDK_FINALROM
#pragma unused(powerMode)
#endif
    SDK_ASSERT( powerMode == DWC_CONNECTINET_POWERMODE_ACTIVE );

    if ( stpInetCntl == NULL )
    {
        DWC_Printf( DWC_REPORTFLAG_CONNECTINET, "InitInet dma : %d, powerMode: %d\n", dmaNo, powerMode );

        SDK_ASSERT( inetCntl );

        MI_CpuClear8( inetCntl, sizeof( DWCInetControl ) );

        inetCntl->dmaNo = dmaNo;
        inetCntl->powerMode = DWC_CONNECTINET_POWERMODE_ACTIVE;

        inetCntl->state = DWC_CONNECTINET_STATE_IDLE;
        inetCntl->online = FALSE;

        inetCntl->wontExecuteWDS = FALSE;
        inetCntl->processWDSstate = DWC_WDS_STATE_IDLE;

#ifndef SDK_FINALROM
		inetCntl->connectType = 0;
#endif	// SDK_FINALROM

        stpInetCntl = inetCntl;

#if SDK_WIFI_VERSION_DATE >= 20051005
        CPS_SetSslLowThreadPriority( ssl_priority );
#else
        (void)ssl_priority;
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_InitInet

  Description:  �C���^�[�l�b�g�ڑ��Ǘ��̏�����

  Arguments:    inetCntl  : �C���^�[�l�b�g�ڑ��Ǘ��p�̍\���̂ւ̃|�C���^

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_InitInet( DWCInetControl* inetCntl )
{
    DWC_InitInetEx( inetCntl, DWC_CONNECTINET_DMA_DEFAULT, DWC_CONNECTINET_POWERMODE_ACTIVE, DWC_CONNECTINET_SSL_DEFAULT_PRIORITY );
}

/*---------------------------------------------------------------------------*
  Name:         DWC_SetAuthServer

  Description:  �F�؂Ɏg�p����T�[�o�̕ύX

  Arguments:    type : �F�؃T�[�o�̃^�C�v

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_SetAuthServer( DWCInetAuthType type )
{
    switch ( type )
    {
    case DWC_CONNECTINET_AUTH_TEST:
        DWC_Auth_SetCustomNas("https://nas.test.nintendowifi.net/ac");
        break;

    case DWC_CONNECTINET_AUTH_DEVELOP:
        DWC_Printf(DWC_REPORTFLAG_AC,"******************************************************************************************\n");
        DWC_Printf(DWC_REPORTFLAG_AC," DWC_CONNECTINET_AUTH_DEVELOP: This setting is for authentication server developing ONLY. \n");
        DWC_Printf(DWC_REPORTFLAG_AC,"******************************************************************************************\n");
        DWC_Auth_SetCustomNas("https://nas.dev.nintendowifi.net/ac");
        break;

    case DWC_CONNECTINET_AUTH_RELEASE:
        DWC_Auth_SetCustomNas("https://nas.nintendowifi.net/ac");
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_SetConnectApType

  Description:  �C���^�[�l�b�g�ɐڑ�����ۂ�AP�^�C�v���w�肷��

  Arguments:    type : �ڑ�����AP�̃^�C�v

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
#ifndef SDK_FINALROM
void
DWC_SetConnectApType( DWCApInfoType type )
{
    // ���������ĂȂ��ꍇ�A�f�o�b�O�p�֐��Ƃ������Ƃ�����̂ŃG���[�������s�킸�A�T�[�g������
    SDK_ASSERTMSG( stpInetCntl, "DWC_InitInet[Ex] is not called." );
    
    // nintendo spot �̓^�C�v�w��ɑΉ����Ă��Ȃ�
    SDK_ASSERTMSG( 0 <= type && type <= DWC_APINFO_TYPE_NINTENDOWFC, "Invalid argument 'type'" );
    
    DWC_Printf(DWC_REPORTFLAG_AC,"***************************************************************\n");
    DWC_Printf(DWC_REPORTFLAG_AC," DWC_CONNECTINET_SET_AP_TYPE: This setting is debug ONLY. \n");
    DWC_Printf(DWC_REPORTFLAG_AC,"***************************************************************\n");
    
	// �ڑ�����AP��type���w�肷��
	stpInetCntl->connectType = type + 1;
    
    // nintendo spot�ւ͐ڑ����Ȃ�
    stpInetCntl->wontExecuteWDS = TRUE;
}
#endif	// SDK_FINALROM

/*---------------------------------------------------------------------------*
  Name:         DWC_ConnectInetAsync

  Description:  �C���^�[�l�b�g�ɐڑ�����B

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_ConnectInetAsync( void )
{
    DWCACConfig inp;

    if ( stpInetCntl != NULL )
    {
        if ( stpInetCntl->state != DWC_CONNECTINET_STATE_IDLE )
        {
            return;
            // NOT REACH HERE
        }

        MI_CpuClear8( &inp, sizeof(DWCACConfig) );

        inp.dmaNo     = (u8)stpInetCntl->dmaNo;       //DMA�ԍ�
        inp.powerMode = (u8)stpInetCntl->powerMode;   //�O�FSAVE �P�FACTIVE
        inp.alloc     = (DWCACAlloc)DWC_Alloc;    //�����ڑ������Ŏg�p����alloc�֐��̃|�C���^
        inp.free      = (DWCACFree) DWC_Free;     //�����ڑ������Ŏg�p����free�֐��̃|�C���^

#ifndef SDK_FINALROM
		// �ڑ�AP�^�C�v�̎w��
		inp.option.connectType = (u8)stpInetCntl->connectType;
#endif	// SDK_FINALROM

        DWC_Printf( DWC_REPORTFLAG_CONNECTINET, "ConnectInet Async\n" );

        stpInetCntl->state = DWC_CONNECTINET_STATE_OPERATING;

        if ( !DWC_AC_Create(&inp) )
        {
            // WCM_�̏������Ɏ��s����ƁA�����ɗ���B
            DWCi_SetError( DWC_ERROR_FATAL, -50199 );
        }
    }
    else
    {
        // ���������ĂȂ��B
        DWCi_SetError( DWC_ERROR_FATAL, -50199 );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_DebugConnectInetAsync

  Description:  AP���w�肵�ăC���^�[�l�b�g�ɐڑ�����B

  Arguments:    ssid    : �ڑ�����AP��SSID
                wep     : �ڑ�����AP��WEP�L�[
                wepMode : WEP�L�[�̃��[�h

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_DebugConnectInetAsync( const void *ssid, const void *wep, int wepMode )
{
    DWC_DebugConnectInetExAsync( ssid, wep, wepMode, NULL, DWC_AC_AP_TYPE_USER1 );
}

void
DWC_DebugConnectInetExAsync( const void *ssid, const void *wep, int wepMode, const char *apSpotInfo, int overrideType )
{
	DWC_ConnectInetAsync();

    // nintendo spot �ւ͐ڑ����Ȃ�
    if ( stpInetCntl != NULL )
    {
        stpInetCntl->wontExecuteWDS = TRUE;
    }
    
	if( DWC_GetInetStatus() == DWC_CONNECTINET_STATE_OPERATING )
	{
		// �ڑ�����Đݒ肷��
        DWC_AC_SetSpecifyApEx( ssid, wep, wepMode, apSpotInfo, overrideType );
	}
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CheckInet

  Description:  �C���^�[�l�b�g�ɐڑ����鏈���̐i�s���`�F�b�N����B

  Arguments:    �Ȃ�

  Returns:      TRUE  : ��������
                FALSE : ������
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckInet( void )
{
    if ( stpInetCntl == NULL )
    {
        DWC_Printf( DWC_REPORTFLAG_CONNECTINET, "ConnectInet library is not initialized.\n" );

        return FALSE;
    }

    if ( stpInetCntl->ac_state != 0 )
    {
        stpInetCntl->state = DWC_CONNECTINET_STATE_OPERATED;

        (void)DWC_GetInetStatus(); // ���ʂ�DWC_GetLastError()�Ɋi�[�B

        if ( stpInetCntl->wontExecuteWDS )
            return TRUE; // WDS�ւ̎��s���s��Ȃ��Ȃ�I�� [masuda]
        else
            return FALSE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_ProcessInet

  Description:  �C���^�[�l�b�g�ɐڑ����鏈�������s����B

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_ProcessInet( void )
{
  if( stpInetCntl == NULL){ return; } // DWC_InitInet ���Ă΂�ĂȂ��i�K

    // WDS�̏�������������WDS�̃v���Z�X���R�[������return
    // ����������A����ꂽAP���Őڑ����J�n����B[masuda]
    switch( stpInetCntl->processWDSstate )
    {
        case DWC_WDS_STATE_IDLE:
            break;
        case DWC_WDS_STATE_PROCESS:
            stpInetCntl->ac_state = 0; // �������Ƃ���B�@�{����DWC_AC_Process�֐��Ŏ擾
            stpInetCntl->processWDSstate = DWC_AC_ProcessGetWDSInfo();
            return;
        case DWC_WDS_STATE_COMPLETED:
            DWC_AC_CleanupGetWDSInfo();
            DWC_DebugConnectInetExAsync(
                stpInetCntl->nspotInfo.ssid,
                stpInetCntl->nspotInfo.wep,
                stpInetCntl->nspotInfo.wepMode,
                stpInetCntl->nspotInfo.apnum,
                DWC_AC_AP_TYPE_NINTENDOSPOT
                );
            stpInetCntl->processWDSstate = DWC_WDS_STATE_IDLE;
            return;
        case DWC_WDS_STATE_FAILED:
            // AP���̎擾�Ɏ��s�����ꍇ�́ADWC_GetInetStatus�֐��ŃG���[�Z�b�g�̏���������B
            // DWC_CheckInet����DWC_GetInetStatus���Ă�ł��炤���߂� ac_state �̕ύX���K�v�B
            DWC_AC_CleanupGetWDSInfo();
            stpInetCntl->ac_state = -1; //�@�G���[�����Ƃ���B �{����DWC_AC_Process�֐��Ŏ擾
            stpInetCntl->wontExecuteWDS = TRUE; // DWC_CheckInet�ŏ����I���ƃn���h�������悤��
            return;
        case DWC_WDS_STATE_ERROR:
            return;
        default:
            break;
    }// WDS�������́A�ȉ��̏����ɐi�܂��Ȃ��B[masuda]

    if ( stpInetCntl->state == DWC_CONNECTINET_STATE_OPERATING )
    {
        stpInetCntl->ac_state = DWC_AC_Process();
    }
    else if ( stpInetCntl->state == DWC_CONNECTINET_STATE_CONNECTED )
    {
        // DCF�̐ڑ��`�F�b�N
        if ( stpInetCntl->online )
        {
            if ( WCM_GetPhase() != WCM_PHASE_DCF )
            {
                // �ؒf���ꂽ�B
                stpInetCntl->online = FALSE;
                stpInetCntl->state = DWC_CONNECTINET_STATE_DISCONNECTED;
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         DWC_GetInetStatus

  Description:  �C���^�[�l�b�g�ɐڑ����鏈��������������ɁA��Ԃ��擾����B

  Arguments:    �Ȃ�

  Returns:      �X�e�[�^�X
 *---------------------------------------------------------------------------*/
DWCInetResult
DWC_GetInetStatus( void )
{
  int state;
  static int stateOld; // WDS�ŃG���[�R�[�h�������p�����߂�state�ۑ��p�ϐ� [masuda]
  DWCInetResult result = DWC_CONNECTINET_STATE_NOT_INITIALIZED;

  if ( stpInetCntl == NULL ){ return (result); } // ��������

    // WDS�̏������ł������ꍇ�ADWC_CONNECTINET_STATE_OPERATING�Ƃ���B[masuda]
    switch( stpInetCntl->processWDSstate )
    {
        case DWC_WDS_STATE_IDLE:
            break;
        case DWC_WDS_STATE_PROCESS:
            return DWC_CONNECTINET_STATE_OPERATING;
        case DWC_WDS_STATE_COMPLETED:
            // ���������ꍇ�́ADWC_ProcessInet�Őڑ��������n�߂�B
            return DWC_CONNECTINET_STATE_OPERATING;
        case DWC_WDS_STATE_FAILED:
            // �ʏ�̐ڑ��Ŕ��������G���[���Z�b�g���ďI��
            DWC_ProcessInet(); // DWC_GetInetStatus()��Process����ɌĂ΂ꂽ�ꍇ�̑Ή�(���������)            
            DWCi_SetError( DWC_ERROR_AC_ANY, stateOld );
            result = DWC_CONNECTINET_STATE_ERROR;
            stpInetCntl->state = result;
            stpInetCntl->processWDSstate = DWC_WDS_STATE_ERROR;        
            return result;
        case DWC_WDS_STATE_ERROR: // WDS���G���[�ŏI�����A����������I������Ƃ��͏�ɃG���[��Ԃ��B
            result = DWC_CONNECTINET_STATE_ERROR;
            return result;
        default:
            break;
    }// WDS�������́A�ȉ��̏����ɐi�܂��Ȃ��B[masuda]

  result =  (DWCInetResult)stpInetCntl->state;
  
  if(result == DWC_CONNECTINET_STATE_IDLE){ return (result); } // ���̒i�K�ł͂܂�DWC_AC_GetStatus()���ĂׂȂ�

  state = DWC_AC_GetStatus();

  if ( state ==  DWC_AC_STATE_COMPLETE )
	{
	  // �ڑ��ς݁B
	  result = DWC_CONNECTINET_STATE_CONNECTED;
	  stpInetCntl->state = result;
	  stpInetCntl->online = TRUE;
	  stpInetCntl->wontExecuteWDS = TRUE; // �������������WDS�ڑ��͍s��Ȃ��B[masuda]
	}
  else if ( state < 0 )
	{
	  // �G���[�����B
	  if ( state >= -10 )
		{
		  // FATAL�G���[
		  DWCi_SetError( DWC_ERROR_FATAL, -50199 );
		  
		  result = DWC_CONNECTINET_STATE_FATAL_ERROR;
		  stpInetCntl->state = result;
		}
      else if ( !stpInetCntl->wontExecuteWDS )
        {
          // AP��������Ȃ������B��������Fatal�ȊO�̃G���[��
          // �ڑ��ł��Ȃ������̂ŁANintendoSpot�ɐڑ������݂�B���̎��s��1�x����[masuda]
          // **** WDS�̏����ł́ADWC_ProcessInet�֐��ŏ������I������ƃn���h���O����Ȃ��悤��
          // **** ac base �̃X�e�[�g���U��K�v������B
          
          // Init�֘A�����ꎞ�ۑ�
          DWCInetControl *stepInetCntlTemp = stpInetCntl;
          u16 dmaNoTemp = stpInetCntl->dmaNo;
          u32 ssl_priority_Temp = CPS_GetSslLowThreadPriority();
          
          // �G���[�R�[�h���ꎞ�ۑ�
          stateOld = state;
          
          // �J�������B
          DWC_CleanupInet();
          
          // �ď�����(��������Ȃ���stpInetCntl���g���Ȃ�)
          DWC_InitInetEx( stepInetCntlTemp, dmaNoTemp, DWC_CONNECTINET_POWERMODE_ACTIVE, (u16)ssl_priority_Temp);
          
          // NintendoSpot�̏�� ssid wep wepmode ���擾
          DWC_AC_StartupGetWDSInfo( &(stpInetCntl->nspotInfo) );
          stpInetCntl->processWDSstate = DWC_WDS_STATE_PROCESS;
          
          // �������Ƃ������Ƃɂ���B
          result = DWC_CONNECTINET_STATE_OPERATING;
          stpInetCntl->ac_state = 0; // �������Ƃ���B�@�{����DWC_AC_Process�֐��Ŏ擾
        }
	  else
		{
		  // AC�ŃG���[�����B
          
          // �ʏ�̐ڑ��Ŕ��������G���[�ƁA
          // ���̌��WDS�ւ̐ڑ��Ŕ��������G���[�ŁA���[���V�[�P���X�̃G���[���Z�b�g����
          if( stateOld < state)
          {
            state = stateOld;
          }
		  
		  DWCi_SetError( DWC_ERROR_AC_ANY, state );
		  
		  result = DWC_CONNECTINET_STATE_ERROR;
		  stpInetCntl->state = result;
		}
	}
  else
	{
	  // �������B
	  result = DWC_CONNECTINET_STATE_OPERATING;
	}
  
  return result;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CleanupInet

  Description:  �C���^�[�l�b�g�ɐڑ����I�����A���������J������B

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void
DWC_CleanupInet( void )
{
    // [arakit] main 051013
    DWC_Printf( DWC_REPORTFLAG_DEBUG, "!!DWC_CleanupInet() was called!!\n" );
    
    if ( stpInetCntl == NULL )
    {
        DWC_Printf( DWC_REPORTFLAG_CONNECTINET, "ConnectInet library is not initialized.\n" );
        // �܂��A����������ĂȂ��̂ŁA�������Ȃ��B
        return;
    }

	if(stpInetCntl->state == DWC_CONNECTINET_STATE_IDLE){ // �܂������������m�ۂ���Ă��Ȃ�
	  stpInetCntl = NULL; // �R���g���[�������g��Ȃ�����
	  return; 
	}

    // �J�������B
    while ( !DWC_AC_Destroy() )
    {
        OS_Sleep( 10 );
    }

    stpInetCntl = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         DWC_CleanupInetAsync

  Description:  �C���^�[�l�b�g�ɐڑ����I�����A���������J������B(�񓯊���)
                TRUE���߂�܂ŁA���Q�[���t���[���Ă�ł��������B

  Arguments:    �Ȃ�

  Returns:      TRUE  : �J�����I�������B
                FALSE : �J�����B
 *---------------------------------------------------------------------------*/
BOOL
DWC_CleanupInetAsync( void )
{
    // [arakit] main 051013
    DWC_Printf( DWC_REPORTFLAG_DEBUG, "!!DWC_CleanupInetAsync() was called!!\n" );
    
    if ( stpInetCntl == NULL )
    {
        DWC_Printf( DWC_REPORTFLAG_CONNECTINET, "ConnectInet library is not initialized.\n" );
        // �܂��A����������ĂȂ��̂ŁA�������Ȃ��B
        return TRUE;
    }


    // ����X�e�[�g�̂Ƃ������B
    if ( stpInetCntl->state == DWC_CONNECTINET_STATE_FATAL_ERROR )
    {
        // FATAL�G���[�̂Ƃ��́A�������Ȃ��B
        // [arakit] main 051013
        DWC_Printf( DWC_REPORTFLAG_DEBUG, "CleanupInet Status: FATAL ERROR.\n" );
        return FALSE;
    }
    else if ( stpInetCntl->state == DWC_CONNECTINET_STATE_IDLE ) // IDLE�̂Ƃ��́A�N���A���ďI���B
    {
        DWC_Printf( DWC_REPORTFLAG_DEBUG, "CleanupInet Status: %d\n", stpInetCntl->state );

        stpInetCntl = NULL;

        return TRUE;
    }
    else
    {
        stpInetCntl->state = DWC_CONNECTINET_STATE_DISCONNECTING;
    }

#if 0
#if SDK_WIFI_VERSION_DATE >= 20050929
    // TCP�̑SClose�҂B
    if ( SOCL_CalmDown() != SOCL_ESUCCESS )
    {
        // close���B
        DWC_Printf( DWC_REPORTFLAG_DEBUG, "CleanupInet Status: %d %d\n", stpInetCntl->state, SOCL_CalmDown() );

        return FALSE;
    }
#endif
#endif

    // AC�̊J�������B
    if ( DWC_AC_Destroy() )
    {
        // �ڑ������I��.
        stpInetCntl = NULL;

        return TRUE;
    }

    return FALSE;
}

BOOL
DWCi_CheckDisconnected( void )
{
    if ( stpInetCntl != NULL && stpInetCntl->state == DWC_CONNECTINET_STATE_DISCONNECTED )
    {
        return TRUE;
    }

    return FALSE;
}

WMLinkLevel
DWC_GetLinkLevel( void )
{
#if SDK_WIFI_VERSION_DATE >= 20050914
    return WCM_GetLinkLevel();
#else
    return WM_GetLinkLevel();
#endif
}

int
DWC_GetUdpPacketDropNum( void )
{
//#ifndef SDK_FINALROM
#if SDK_WIFI_VERSION_DATE >= 20050914
    return (int)SOCLi_CheckCount[ SOCL_CHECKCOUNT_UDPRCVCB_2 ];
#else
    return 0;
#endif
//#else
//    return 0;
//#endif
}

// [nakata] �������AP���擾�֘A�̊g������

// [nakata] �O���֐��̒�`
extern u8* WCM_GetApMacAddress( void );
extern u8* WCM_GetApEssid     ( u16* length );

/*---------------------------------------------------------------------------*
  Name:         DWC_GetApInfo

  Description:  ���ݐڑ�����AP�Ɋւ�������擾����B

  Arguments:    apinfo  : �擾���������������ރ������̈�ւ̃|�C���^

  Returns:      TRUE  : �擾�ɐ�������
	            FALSE : �擾�Ɏ��s����
 *---------------------------------------------------------------------------*/
BOOL DWC_GetApInfo(DWCApInfo *apinfo)
{
	OSIntrMode	preirq;
	unsigned char	*wcmapbssid;
	unsigned char	*wcmapessid;
	unsigned short	essidlen;
	u8 apSpotInfo[10];
	
	// [nakata] �i�[��G���A��0�N���A
	MI_CpuClear8(apinfo, sizeof(DWCApInfo));
	apinfo->area = DWC_APINFO_AREA_UNKNOWN;

	// [nakata] �C���^�[�l�b�g�ڑ����m�����Ă��Ȃ��ꍇ�ɂ�FALSE��Ԃ�
	if(DWC_GetInetStatus() != DWC_CONNECTINET_STATE_CONNECTED)
		return FALSE;
	
	// [nakata] AP�^�C�v���擾
	apinfo->aptype = (DWCApInfoType)DWC_AC_GetApType();
	if(apinfo->aptype == DWC_AC_AP_TYPE_FALSE || apinfo->aptype > DWC_AC_AP_TYPE_UNKNOWN)
	{
		apinfo->aptype = (DWCApInfoType)DWC_AC_AP_TYPE_UNKNOWN;
		return FALSE;
	}
	
	// [nakata] BSSID��apinfo���Ɏ擾
	preirq = OS_DisableInterrupts();
	wcmapbssid = WCM_GetApMacAddress();
	DC_InvalidateRange(wcmapbssid, WM_SIZE_BSSID);
	if(wcmapbssid == NULL) {
		preirq = OS_RestoreInterrupts(preirq);
		return FALSE;
	}
	MI_CpuCopy8(wcmapbssid, apinfo->bssid, 6);
	
	// [nakata] �K�v�ɉ�����ESSID��apinfo���Ɏ擾
	if(apinfo->aptype >= 0 && apinfo->aptype < 3) {
		wcmapessid = WCM_GetApEssid(&essidlen);
		DC_InvalidateRange(wcmapessid, WM_SIZE_SSID);
		if(wcmapessid == NULL) {
			preirq = OS_RestoreInterrupts(preirq);
			return FALSE;		
		}
		MI_CpuCopy8(wcmapessid, apinfo->essid, essidlen);
	}
	preirq = OS_RestoreInterrupts(preirq);
	
	// [nakata] AP�̏ꏊ����apinfo���Ɏ擾
	if( DWC_AC_GetApSpotInfo( apSpotInfo ) )
	{
		apinfo->area = (apSpotInfo[0] >= '0' && apSpotInfo[0] <= '9') ? (DWCApInfoArea)(apSpotInfo[0] - '0') : DWC_APINFO_AREA_UNKNOWN;
		MI_CpuCopy8(&apSpotInfo[1], apinfo->spotinfo, DWC_SIZE_SPOTINFO);
	}
	else
	{
		apinfo->area = DWC_APINFO_AREA_UNKNOWN;
	}
	
	return TRUE;
}
// [nakata] �����܂�AP���擾�֘A�̊g������

// [nakata] �������u���E�U�Ή��̊g������

/*---------------------------------------------------------------------------*
  Name:         DWC_EnableHotspot

  Description:  Nintendo Wi-Fi�R�l�N�V�������T�|�[�g���Ȃ�Hotspot�̎g�p��������

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void DWC_EnableHotspot(void)
{
	DWC_Netcheck_SetAllow302(TRUE);
}

// [nakata] �����܂Ńu���E�U�Ή��̊g������

// [uemura] 2006/05/24�ǉ�
/*---------------------------------------------------------------------------*
  Name:         DWC_CheckWiFiStation

  Description:  Wi-Fi�X�e�[�V�������ǂ������m�F����

  Arguments:    ssid  : �m�F����AP��SSID
                len   : SSID�̒���

  Returns:      TRUE  : Wi-Fi�X�e�[�V����
                FALSE : Wi-Fi�X�e�[�V�����ȊO
 *---------------------------------------------------------------------------*/
BOOL
DWC_CheckWiFiStation( const void *ssid, u16 len )
{
	return DWC_AC_CheckWiFiStation( ssid, len );
}

// [uemura] �����܂�

// [nakata] 06/06/27 dwc_main.c����ړ�(�d�����鏈���𓝍����邽��)
/*---------------------------------------------------------------------------*
  DCF�ʐM�����X�V�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE�̂Ƃ��A�ؒf���ꂽ
  �p�r�@�F���Q�[���t���[���Ăяo���ADCF�ʐM�̊Ď����s���B
 *---------------------------------------------------------------------------*/
BOOL DWC_UpdateConnection( void )
{
    DWC_ProcessInet();

    if ( DWCi_CheckDisconnected() )
    {
        // �ؒf���ꂽ�I
        u8 aptype = DWC_AC_GetApType();

        // �����ڑ��̃G���[�R�[�h��ݒ�
        if( aptype == DWC_AC_AP_TYPE_FALSE || aptype > DWC_AC_AP_TYPE_UNKNOWN )
        {
            aptype = DWC_AC_AP_TYPE_UNKNOWN;
        }
        DWCi_SetError( DWC_ERROR_DISCONNECTED, -54000 - aptype );

        DWC_Printf( DWC_REPORTFLAG_DEBUG, "Detected DCF shutdown.\n" );

        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  DCF�ʐM�����I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FDCF���ؒf���ꂽ�Ƃ��ɌĂяo����A�K�v�ȏ���������ꍇ�ɂ����ɋL�q����B
 *---------------------------------------------------------------------------*/
void DWC_ForceShutdown( void )
{
    // �����I�ɌĂ�ł��܂��Ă����܂�Ȃ��֐��������ŌĂяo���B
}
// [nakata] �����܂�

// [nakata] 06/11/06 �C���^�[�l�b�g�ڑ��m�F���������b�v����֐���ǉ�

/*---------------------------------------------------------------------------*
  HTTP���g�p�����C���^�[�l�b�g�ڑ��m�F�֐�(�񓯊�)
  �����@�F�^�C���A�E�g����(�~���b�A0�̏ꍇ��3000����)
  �߂�l�FTRUE  - ����
          FALSE - ���s
  �p�r�@�FHTTP�ʐM���g�p���ăC���^�[�l�b�g�ڑ����m�F����
 *---------------------------------------------------------------------------*/
BOOL DWC_TestInetAsync(int timeout)
{
    if ( stpInetCntl == NULL ) // DWC_InitInet ���Ă΂�ĂȂ�
    {
        return FALSE;
    }
    else if ( stpInetCntl->state != DWC_CONNECTINET_STATE_CONNECTED ) // DWC_ConnectInetAsync ���������Ă��Ȃ�
    {
        return FALSE;
    }

	return DWC_SimpleNetcheckAsync(timeout);
}

/*---------------------------------------------------------------------------*
  HTTP���g�p�����C���^�[�l�b�g�ڑ��m�F�v���Z�X�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FDCF���ؒf���ꂽ�Ƃ��ɌĂяo����A�K�v�ȏ���������ꍇ�ɂ����ɋL�q����B
 *---------------------------------------------------------------------------*/
DWCTestInetResult DWC_TestInetProcess(void)
{
	DWCSimpleNetcheckResult result;
	u8 aptype;
	result = DWC_SimpleNetcheckProcess();
	
	switch(result) {
	case DWC_SIMPLENETCHECK_PROCESSING:
		return DWC_TESTINET_OPERATING;
	case DWC_SIMPLENETCHECK_SUCCESS:
		return DWC_TESTINET_CONNECTED;
	case DWC_SIMPLENETCHECK_FAIL:
	case DWC_SIMPLENETCHECK_SYSFAIL:
		aptype = DWC_AC_GetApType();
		if( aptype == DWC_AC_AP_TYPE_FALSE || aptype > DWC_AC_AP_TYPE_UNKNOWN )
			aptype = DWC_AC_AP_TYPE_UNKNOWN;
        DWCi_SetError(DWC_ERROR_DISCONNECTED, -54100-aptype);
			
		return DWC_TESTINET_DISCONNECTED;
	case DWC_SIMPLENETCHECK_NOTINITIALIZED:
		return DWC_TESTINET_NOT_INITIALIZED;
	}
	
	// [nakata] �\�����Ȃ�����
	return DWC_TESTINET_DISCONNECTED;
}
// [nakata] �����܂�
