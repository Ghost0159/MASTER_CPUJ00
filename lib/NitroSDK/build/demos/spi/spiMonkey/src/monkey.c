/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos
  File:     monkey.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: monkey.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.8  2004/08/20 02:44:31  terui
  TS_VERSION=0���ɂ�PM_SetAmp����悤�C���B

  Revision 1.7  2004/08/04 23:44:28  terui
  LCD�o�b�N���C�g����֐��̕ύX

  Revision 1.6  2004/08/02 01:07:04  yada
  PM_GAINAMP_80 -> PM_AMPGAIN_80

  Revision 1.5  2004/07/31 12:46:19  terui
  PMIC���}�C�N�p�ɏ���������R�[�h��ǉ�

  Revision 1.4  2004/06/10 09:41:25  terui
  �T���v�����O�^�C�~���O�̔��f���X���b�h������^�C�}�[�n���h�����Ɉړ��B
  �^�C�}�[�̊J�n���X���b�h���Ɉړ��B
  �X���b�h�̓��b�Z�[�WTYPE�ɂ��T���v�����O�f�o�C�X�𔻒f����d�g�݂ɕύX�B

  Revision 1.3  2004/06/10 05:23:46  yasu
  Change TP_WaitCalibratedResult -> TP_GetCalibratedResult

  Revision 1.2  2004/06/03 11:17:29  terui
  �T���v�����O���[�g��ύX�B

  Revision 1.1  2004/06/02 09:53:25  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    "monkey.h"
#include    <nitro/spi.h>

#ifdef  SDK_TS
#if     ( SDK_TS_VERSION >= 100 )
#include    <nitro/spi/common/pm_common.h>
#include    <nitro/spi/ARM9/pm.h>
#endif
#endif


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void MonkeyThread(void *arg);
static void MonkeySamplingAlarm(void *arg);
static void MonkeyMicCallback(MICResult result, void *arg);
static void MonkeyTpCallback(TPRequestCommand command, TPRequestResult result, u16 index);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static MonkeyWork monkey;


/*---------------------------------------------------------------------------*
  Name:         MonkeyInit

  Description:  SPI�f�o�C�X�T���v�����O�p�X���b�h���N������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyInit(void)
{
    // �T���v�����O�݂̂��s���X���b�h���쐬
    OS_InitMessageQueue(&(monkey.msg_q), monkey.msg_buf, MONKEY_MESSAGE_ARRAY_MAX);
    OS_CreateThread(&(monkey.thread),
                    MonkeyThread,
                    0,
                    (void *)(monkey.stack + (MONKEY_STACK_SIZE / sizeof(u32))),
                    MONKEY_STACK_SIZE, MONKEY_THREAD_PRIORITY);
    OS_WakeupThreadDirect(&(monkey.thread));
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyGetNewTpData

  Description:  �ŐV�̃^�b�`�p�l���T���v�����O�f�[�^���擾����B
                ���łɈ�x�ǂݏo����Ă���f�[�^���ǂ����͍l�����Ă��Ȃ��B

  Arguments:    num   - �擾����f�[�^�����w�肷��B
                array - �f�[�^���擾����z����w�肷��B
                        �z��̐擪����V�������Ƀf�[�^���i�[�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyGetNewTpData(s32 num, TPData *array)
{
    s32     i;
    s32     index;

    index = (s32)(monkey.tpIndex);
    for (i = 0; i < num; i++)
    {
        index = (index + (MONKEY_TP_ARRAY_MAX - 1)) % MONKEY_TP_ARRAY_MAX;
        array[i] = monkey.tpBuf[index];
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyGetNewMicData

  Description:  �ŐV�̃}�C�N�T���v�����O�f�[�^���擾����B
                ���łɈ�x�ǂݏo����Ă���f�[�^���ǂ����͍l�����Ă��Ȃ��B

  Arguments:    num   - �擾����f�[�^�����w�肷��B
                array - �f�[�^���擾����z����w�肷��B
                        �z��̐擪����V�������Ƀf�[�^���i�[�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyGetNewMicData(s32 num, u16 *array)
{
    s32     i;
    s32     index;

    index = (s32)(monkey.micIndex);
    for (i = 0; i < num; i++)
    {
        index = (index + (MONKEY_MIC_ARRAY_MAX - 1)) % MONKEY_MIC_ARRAY_MAX;
        array[i] = monkey.micBuf[index];
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyThread

  Description:  �T���v�����O���s���X���b�h�B

  Arguments:    arg - ���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyThread(void *arg)
{
#pragma unused( arg )

    OSMessage msg;

    // �}�C�NAPI������
    {
        MIC_Init();
        monkey.micIndex = 0;

#ifdef  SDK_TS
        // PMIC������
        PM_Init();
        // AMP�I��
        (void)PM_SetAmp(PM_AMP_ON);
#if ( SDK_TS_VERSION >= 100 )
        // AMP�Q�C������
        (void)PM_SetAmpGain(PM_AMPGAIN_80);
#endif
#if ( SDK_TS_VERSION == 100 )
        // �m�C�Y�΍�ׁ̈ALCD�o�b�N���C�g�I�t
        (void)PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
#endif
#endif

    }
    // �^�b�`�p�l��API������
    {
        TPCalibrateParam calibrate;

        TP_Init();
        if (TP_GetUserInfo(&calibrate))
        {
            TP_SetCalibrateParam(&calibrate);
        }
        else
        {
            OS_Panic("Can't find TP calibration data.");
        }
        TP_SetCallback(MonkeyTpCallback);
        monkey.tpIndex = 0;
    }

    // �T���v�����O�^�C�~���O�p�^�C�}�[���J�n
    OS_CreateAlarm(&(monkey.alarm));
    monkey.timerCount = 0;
    OS_SetPeriodicAlarm(&(monkey.alarm),
                        OS_GetTick(),
                        (MONKEY_MIC_SPAN_TICK * MONKEY_SAMPLING_SPAN_LINE),
                        MonkeySamplingAlarm, NULL);

    while (TRUE)
    {
        // ���b�Z�[�W���󂯎��܂ŃX���b�h�x�~
        (void)OS_ReceiveMessage(&(monkey.msg_q), &msg, OS_MESSAGE_BLOCK);

        // �^�b�`�p�l���̃T���v�����O
        if ((u32)msg == MONKEY_MESSAGE_TYPE_TP)
        {
            TP_RequestSamplingAsync();
        }
        // �}�C�N�̃T���v�����O
        else if ((u32)msg == MONKEY_MESSAGE_TYPE_MIC)
        {
            if (MIC_RESULT_SUCCESS != MIC_DoSamplingAsync(MIC_SAMPLING_TYPE_12BIT,
                                                          &(monkey.micBuf[monkey.micIndex]),
                                                          MonkeyMicCallback, NULL))
            {
                OS_Printf("Monkey: MIC request failure.\n");
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeySamplingAlarm

  Description:  �X���b�h�Ƀ��b�Z�[�W�𑗂�^�C�~���O�𐧌䂷��A���[���n���h���B

  Arguments:    arg - ���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Periodic�^�C�}�[�ƃT���v�����O�̃^�C�~���O�}

    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
MIC   >----           >----           >----           >----           >----
TP          >--------                       >--------                       >--

 *---------------------------------------------------------------------------*/
static void MonkeySamplingAlarm(void *arg)
{
#pragma unused( arg )

    if (0 == (monkey.timerCount % 8))
    {
        (void)OS_SendMessage(&(monkey.msg_q), (void *)MONKEY_MESSAGE_TYPE_MIC, OS_MESSAGE_BLOCK);
    }
    else if (3 == (monkey.timerCount % 16))
    {
        (void)OS_SendMessage(&(monkey.msg_q), (void *)MONKEY_MESSAGE_TYPE_TP, OS_MESSAGE_BLOCK);
    }

    monkey.timerCount++;
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyMicCallback

  Description:  �}�C�N�̃T���v�����O�v���ɑ΂��鉞���R�[���o�b�N�֐��B
                ����ɃT���v�����O�ł����ꍇ�̓f�[�^�i�[��o�b�t�@�ʒu��
                ���ɐi�߂�B

  Arguments:    result - �}�C�N����v���ɑ΂��鏈�����ʁB
                arg    - ���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyMicCallback(MICResult result, void *arg)
{
#pragma unused( arg )

    if (result == MIC_RESULT_SUCCESS)
    {
        // �f�[�^�i�[�o�b�t�@�ʒu�����֐i�߂�
        monkey.micIndex = (u16)((monkey.micIndex + 1) % MONKEY_MIC_ARRAY_MAX);
    }
    else
    {
        // �T�E���h�̉e���Œ��O�̃T���v�����O���������Ă��Ȃ����߁A���s�B
        OS_Printf("Monkey: MIC request failure.\n");
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyTpCallback

  Description:  �^�b�`�p�l���̃T���v�����O�v���ɑ΂��鉞���R�[���o�b�N�֐��B
                ����ɃT���v�����O�ł����ꍇ�̓f�[�^�i�[��o�b�t�@�ʒu��
                ���ɐi�߂�B

  Arguments:    command - �������Ή����鑀��v���R�}���h�������B���g�p�B
                result  - �^�b�`�p�l������v���ɑ΂��鏈�����ʁB
                index   - �����T���v�����O���̃C���f�b�N�X�B���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyTpCallback(TPRequestCommand command, TPRequestResult result, u16 index)
{
#pragma unused( command , index )

    if (result == TP_RESULT_SUCCESS)
    {
        // �T���v�����O���l���X�N���[�����W�l�ɕϊ�
        (void)TP_GetCalibratedResult(&(monkey.tpBuf[monkey.tpIndex]));
        // �f�[�^�i�[�o�b�t�@�ʒu�����֐i�߂�
        monkey.tpIndex = (u16)((monkey.tpIndex + 1) % MONKEY_TP_ARRAY_MAX);
    }
    else
    {
        // �T�E���h�̉e���Œ��O�̃T���v�����O���������Ă��Ȃ����߁A���s�B
        OS_Printf("Monkey: TP request failure.\n");
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
