/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_etc.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_etc.c,v $
  Revision 1.21  2006/07/06 11:20:52  okubata_ryoma
  undo

  Revision 1.19  2006/02/21 00:10:23  okubata_ryoma
  Copyright fix

  Revision 1.18  2006/02/20 07:43:10  seiki_masashi
  WM_SetGameInfo �֐��̈�������ύX

  Revision 1.17  2005/11/22 09:02:26  seiki_masashi
  WM_SendCommandDirect �֐����g�p����悤�ɕύX

  Revision 1.16  2005/11/02 01:21:32  ooe
  WM_StartTestRxMode, WM_StopTestRxMode��ǉ��B

  Revision 1.15  2005/07/27 07:01:18  ooe
  WM_SetPowerSaveMode��ǉ��B

  Revision 1.14  2005/03/07 00:00:36  terui
  WM_SetWEPKeyEx�ɂ�����p�����[�^�͈̓`�F�b�N���C���B

  Revision 1.13  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.12  2005/02/03 23:40:36  ooe
  Copyright �\�L�̏C���B

  Revision 1.11  2005/02/03 11:33:23  ooe
  WM_SetWEPKeyEx��ǉ��B

  Revision 1.10  2004/12/20 02:49:59  seiki_masashi
  WM_SetWEPKey �ɂāAwepkey �̃L���b�V���X�g�A�������Ă����̂��C��

  Revision 1.9  2004/11/17 07:30:24  terui
  WM_SetGameInfo���ɂ�MI_CpuCopy32��MI_CpuCopy16�ɏC���B

  Revision 1.8  2004/11/09 00:03:44  seiki_masashi
  Null �����ɂ��^�C���A�E�g������

  Revision 1.7  2004/11/02 07:27:14  terui
  �R�����g�C���B

  Revision 1.6  2004/10/22 04:37:28  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.5  2004/10/18 11:09:16  terui
  WM_SetBeaconPeriod�֐�����U����J�ɕύX�B

  Revision 1.4  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.3  2004/10/01 04:03:16  terui
  WM_ENABLE_TESTMODE�X�C�b�`�ɂ��@�\�𐧌�����悤�����B

  Revision 1.2  2004/09/20 10:27:36  ooe
  WMSP_SetBeaconPeriod��ǉ��B

  Revision 1.1  2004/09/10 11:03:30  terui
  wm.c�̕����ɔ����A�V�Kupload�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wm.h>
#include    "wm_arm9_private.h"


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     WM_SIZE_TEMP_USR_GAME_INFO_BUF 128


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u32 tmpUserGameInfoBuf[WM_SIZE_TEMP_USR_GAME_INFO_BUF / sizeof(u32)] ATTRIBUTE_ALIGN(32);


#ifdef  WM_ENABLE_TESTMODE
/*---------------------------------------------------------------------------*
  Name:         WM_StartTestMode

  Description:  �e�X�g���[�h�ł̒ʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                signal      -   0:���ϒ�(data=0), 1:���ϒ�(data=1), 2:PN15�i,
                                3:01�p�^�[��(�X�N�����u������), 4:01�p�^�[��(�X�N�����u���Ȃ�)
                rate        -   1:1Mbps, 2:2Mbps
                channel     -   �f�[�^�𑗐M����`�����l�����w��(1�`14)�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartTestMode(WMCallbackFunc callback, u16 signal, u16 rate, u16 channel)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_TESTMODE, callback);

    // ARM7��FIFO�Œʒm
    {
        WMStartTestModeReq Req;

        Req.apiid = WM_APIID_START_TESTMODE;
        Req.signal = signal;
        Req.rate = rate;
        Req.channel = channel;

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        if (result != WM_ERRCODE_SUCCESS)
        {
            return result;
        }
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StopTestMode

  Description:  �e�X�g���[�h�ł̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StopTestMode(WMCallbackFunc callback)
{
    WMErrCode result;

    // TESTMODE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_TESTMODE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_STOP_TESTMODE, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_STOP_TESTMODE, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StartTestRxMode

  Description:  �e�X�g���[�h�ł̎�M���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                channel     -   �f�[�^����M����`�����l�����w��(1�`14)�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartTestRxMode(WMCallbackFunc callback, u16 channel)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_TESTRXMODE, callback);

    // ARM7��FIFO�Œʒm
    {
        WMStartTestRxModeReq Req;

        Req.apiid = WM_APIID_START_TESTRXMODE;
        Req.channel = channel;

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        if (result != WM_ERRCODE_SUCCESS)
        {
            return result;
        }
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StopTestRxMode

  Description:  �e�X�g���[�h�ł̎�M���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StopTestRxMode(WMCallbackFunc callback)
{
    WMErrCode result;

    // TESTMODE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_TESTMODE_RX);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_STOP_TESTRXMODE, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_STOP_TESTRXMODE, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}
#endif

/*---------------------------------------------------------------------------*
  Name:         WM_SetWEPKey

  Description:  �Í��@�\�A�Í��L�[��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                wepmode     -   0: �Í��@�\�Ȃ�
                                1: RC4(40bit)�Í����[�h
                                2: RC4(104bit)�Í����[�h
                                3: RC4(128bit)�Í����[�h
                wepkey      -   �Í��L�[�f�[�^( 80�o�C�g )�ւ̃|�C���^�B
                                �L�[�f�[�^��4�̃f�[�^�ō\������A���ꂼ��20�o�C�g�B
                                �e20�o�C�g�̂����A
                                 40�r�b�g���[�h�ł�  5 �o�C�g
                                104�r�b�g���[�h�ł� 13 �o�C�g
                                128�r�b�g���[�h�ł� 16 �o�C�g
                                �̃f�[�^���g�p�����B
                                �܂��A���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetWEPKey(WMCallbackFunc callback, u16 wepmode, const u16 *wepkey)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (wepmode > 3)
    {
        WM_WARNING("Parameter \"wepmode\" must be less than %d.\n", 3);
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (wepmode != WM_WEPMODE_NO)
    {
        // �p�����[�^�`�F�b�N
        if (wepkey == NULL)
        {
            WM_WARNING("Parameter \"wepkey\" must not be NULL.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
        if ((u32)wepkey & 0x01f)
        {
            // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
            WM_WARNING("Parameter \"wepkey\" is not 32-byte aligned.\n");
        }

        // �w��o�b�t�@�̃L���b�V���������o��
        DC_StoreRange((void *)wepkey, WM_SIZE_WEPKEY);
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_WEPKEY, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_WEPKEY, 2, (u32)wepmode, (u32)wepkey);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetWEPKeyEx

  Description:  �Í��@�\�A�Í��L�[�A�Í��L�[ID��ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                wepmode     -   0: �Í��@�\�Ȃ�
                                1: RC4(40bit)�Í����[�h
                                2: RC4(104bit)�Í����[�h
                                3: RC4(128bit)�Í����[�h
                wepkeyid    -   4�w�肵��wepkey�̂ǂ���g�p���邩��I�����܂��B
                                0�`3�Ŏw�肵�܂��B
                wepkey      -   �Í��L�[�f�[�^( 80�o�C�g )�ւ̃|�C���^�B
                                �L�[�f�[�^��4�̃f�[�^�ō\������A���ꂼ��20�o�C�g�B
                                �e20�o�C�g�̂����A
                                 40�r�b�g���[�h�ł�  5 �o�C�g
                                104�r�b�g���[�h�ł� 13 �o�C�g
                                128�r�b�g���[�h�ł� 16 �o�C�g
                                �̃f�[�^���g�p�����B
                                �܂��A���̃f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A�����B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetWEPKeyEx(WMCallbackFunc callback, u16 wepmode, u16 wepkeyid, const u8 *wepkey)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (wepmode > 3)
    {
        WM_WARNING("Parameter \"wepmode\" must be less than %d.\n", 3);
        return WM_ERRCODE_INVALID_PARAM;
    }

    if (wepmode != WM_WEPMODE_NO)
    {
        // �p�����[�^�`�F�b�N
        if (wepkey == NULL)
        {
            WM_WARNING("Parameter \"wepkey\" must not be NULL.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
        if (wepkeyid > 3)
        {
            WM_WARNING("Parameter \"wepkeyid\" must be less than %d.\n", 3);
        }
        if ((u32)wepkey & 0x01f)
        {
            // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
            WM_WARNING("Parameter \"wepkey\" is not 32-byte aligned.\n");
        }

        // �w��o�b�t�@�̃L���b�V���������o��
        DC_StoreRange((void *)wepkey, WM_SIZE_WEPKEY);
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_WEPKEY_EX, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_WEPKEY_EX, 3, (u32)wepmode, (u32)wepkey, (u32)wepkeyid);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetGameInfo

  Description:  �Q�[������ݒ肷��BWM_SetParentParameter�ɂ�菉���l�̓Z�b�g
                �����̂ŁA�����ύX����ꍇ�ɗp����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                userGameInfo     - ���[�U�[�Q�[�����ւ̃|�C���^�B
                userGameInfoSize - ���[�U�[�Q�[�����̃T�C�Y�B
                ggid        -   �Q�[���O���[�vID
                tgid        -   �e���|�����O���[�vID
                attr        -   �t���O�Q�B�ȉ��̃t���O�̘_���a��ݒ�B
                                    WM_ATTR_FLAG_ENTRY - �G���g���[����
                                    WM_ATTR_FLAG_MB    - �}���`�u�[�g�󂯕t��
                                    WM_ATTR_FLAG_KS    - �L�[�V�F�A�����O
                                    WM_ATTR_FLAG_CS    - �A���]�����[�h
  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode
WM_SetGameInfo(WMCallbackFunc callback, const u16 *userGameInfo, u16 userGameInfoSize,
               u32 ggid, u16 tgid, u8 attr)
{
    WMErrCode result;

    // �e�@�Ƃ��Ċ������ł��邱�Ƃ��m�F
    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (userGameInfo == NULL)
    {
        WM_WARNING("Parameter \"userGameInfo\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (userGameInfoSize > WM_SIZE_USER_GAMEINFO)
    {
        WM_WARNING("Parameter \"userGameInfoSize\" must be less than %d.\n", WM_SIZE_USER_GAMEINFO);
        return WM_ERRCODE_INVALID_PARAM;
    }

    // �w��̃o�b�t�@����U�ޔ�
    MI_CpuCopy16((void *)userGameInfo, (void *)tmpUserGameInfoBuf, userGameInfoSize);
    DC_StoreRange((void *)tmpUserGameInfoBuf, userGameInfoSize);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_GAMEINFO, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_GAMEINFO, 5,
                             (u32)tmpUserGameInfoBuf,
                             (u32)userGameInfoSize, (u32)ggid, (u32)tgid, (u32)attr);
    if (result != WM_ERRCODE_SUCCESS)
    {
        return result;
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetBeaconIndication

  Description:  �r�[�R������Mindicate�̗L��/������؂�ւ���B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                flag        -   0: ����
                                1: �L��

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetBeaconIndication(WMCallbackFunc callback, u16 flag)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if ((0 != flag) && (1 != flag))
    {
        WM_WARNING("Parameter \"flag\" must be \"0\" of \"1\".\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_BEACON_IND, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_BEACON_IND, 1, (u32)flag);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetLifeTime

  Description:  ���C�t�^�C����ݒ肷��

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                tableNumber -   ���C�t�^�C����ݒ肷��CAM�e�[�u���ԍ�(0xFFFF�ł��ׂẴe�[�u��)
                camLifeTime -   CAM�̃��C�t�^�C��(100ms�P�ʁF0xFFFF�Ŗ���)
                frameLifeTime - �ݒ肷��t���[���̃��C�t�^�C���̃r�[�R���Ԋu(100ms�P�ʁF0xFFFF�Ŗ���)
                mpLifeTime  -   MP �ʐM�̃��C�t�^�C��(100ms�P�ʁF0xFFFF�Ŗ���)

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode
WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime,
               u16 mpLifeTime)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_LIFETIME, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_LIFETIME, 4,
                             (u32)tableNumber,
                             (u32)camLifeTime, (u32)frameLifeTime, (u32)mpLifeTime);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_MeasureChannel

  Description:  �`�����l���̎g�p�󋵂𑪒肷��

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                ccaMode     -   CCA���샂�[�h
                                0: �L�����A�Z���X�̂݁BED臒l�͖����B
                                1: ED臒l�̂ݗL���B
                                2: �L�����A�Z���X��ED臒l�̘_���ρB
                                3: �L�����A�Z���X��ED臒l�̘_���a�B
                EDThreshold -   ED臒l(0�`61) -60dBm �` -80dBm
                channel     -   ��������`�����l��(1���MeasureChannel��1�̃`�����l���̂�)
                measureTime -   �������鎞��

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode
WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold, u16 channel,
                  u16 measureTime)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_MEASURE_CHANNEL, callback);

    // ARM7��FIFO�Œʒm
    {
        WMMeasureChannelReq Req;

        Req.apiid = WM_APIID_MEASURE_CHANNEL;
        Req.ccaMode = ccaMode;
        Req.edThreshold = edThreshold;
        Req.channel = channel;
        Req.measureTime = measureTime;

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }
    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_InitWirelessCounter

  Description:  WirelessCounter������������

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_InitWirelessCounter(WMCallbackFunc callback)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_INIT_W_COUNTER, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_INIT_W_COUNTER, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_GetWirelessCounter

  Description:  Wireless NIC�̑���M�t���[�����A����M�G���[�t���[�������擾����

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_GetWirelessCounter(WMCallbackFunc callback)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_GET_W_COUNTER, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_GET_W_COUNTER, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetEntry

  Description:  �e�@�Ƃ��āA�q�@����̐ڑ��󂯕t���ۂ�؂�ւ���B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                enabled     -   �G���g���[����/�s���t���O�BTRUE:���AFALSE:�s���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetEntry(WMCallbackFunc callback, BOOL enabled)
{
    WMErrCode result;

    // �e�@�ȊO�ł͎��s�s��
    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_ENTRY, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_ENTRY, 1, (u32)enabled);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_SetBeaconPeriod

  Description:  Beacon�̊Ԋu��ύX����

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                beaconPeriod - Beacon�Ԋu(10�`1000 TU(1024��s))

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_SetBeaconPeriod(WMCallbackFunc callback, u16 beaconPeriod)
{
    WMErrCode result;

    // �e�@�ȊO�ł͎��s�s��
    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_BEACON_PERIOD, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_BEACON_PERIOD, 1, (u32)beaconPeriod);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetPowerSaveMode

  Description:  PowerSaveMode��ύX����

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetPowerSaveMode(WMCallbackFunc callback, BOOL powerSave)
{
    WMErrCode result;

    // DCF�q�@�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_PS_MODE, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_PS_MODE, 1, (u32)powerSave);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
