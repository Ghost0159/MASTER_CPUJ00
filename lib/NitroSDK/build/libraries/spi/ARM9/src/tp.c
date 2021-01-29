/*---------------------------------------------------------------------------*
  Project:  NitroSDK - libraries - tp
  File:     tp.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: tp.c,v $
  Revision 1.36.18.1  2008/01/18 01:06:18  seiki_masashi
  HEAD �ƃ}�[�W

  Revision 1.38  2007/12/20 01:06:08  terui
  Correct copyright year.

  Revision 1.37  2007/12/19 01:10:55  terui
  TP_Wait[All]Busy() �ɂ����āA�ҋ@����̕��A���������ɖ������Ă���ꍇ��
  �����݋���ԂɊւ�炸��������悤�ɏC���B

  Revision 1.36  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.35  2005/04/14 05:07:42  takano_makoto
  �L�����u���[�V�����p�����[�^���s���ȏꍇ�ɍ��W�l��0�Ƃ��ĕԂ��悤�C��

  Revision 1.34  2005/04/14 04:05:10  takano_makoto
  TP_GetUserInfo�����TRUE��Ԃ��悤�ɏC��

  Revision 1.33  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.32  2005/02/18 08:54:02  yasu
  ����N�x�ύX

  Revision 1.31  2005/02/18 07:29:41  yasu
  Signed/Unsigned �ϊ��x���}��

  Revision 1.30  2004/12/29 06:08:57  takano_makoto
  TP_GetRatestRawPointInAuto�֐��ɍ��W��ё΍􏈗���ǉ�

  Revision 1.29  2004/12/29 04:00:26  takano_makoto
  TP_RequestSetStability�֐��̈����ɉߋ��Ƃ̌݊��̂���retry�p�����[�^��ǉ�(�����ł͎g�p����Ȃ�)

  Revision 1.28  2004/12/29 02:04:34  takano_makoto
  SetStability�֐���retry�p�����[�^��p�~

  Revision 1.27  2004/12/14 00:20:29  takano_makoto
  TP_GetLatestRawPointInAuto����tpState.index�̒l�����炩���ߑޔ����Ă����悤�ɏC��

  Revision 1.26  2004/12/13 09:01:33  takano_makoto
  ����A�v���P�[�V������validity�����Ă��Ȃ��ꍇ�̂��߁ATP_GetLatestRawPointInAuto�֐���INVALID�̏ꍇ�ł����W�l���ꉞ�ݒ肵�Ă����B

  Revision 1.25  2004/12/08 06:16:37  takano_makoto
  TP_GetLatestRawPointInAuto�ŁAinvalid�ȍ��W�l���������ł��Ȃ������ꍇ�ɂ� invalid�t���O�𗧂Ă��l��Ԃ��悤�ɏC���B

  Revision 1.24  2004/11/02 10:26:33  takano_makoto
  fix typo.

  Revision 1.23  2004/11/02 10:19:34  takano_makoto
  fix typo.

  Revision 1.22  2004/10/20 06:34:45  terui
  LCD�̃��C������`����ύX

  Revision 1.21  2004/09/27 10:06:37  takano_makoto
  branch�Ƃ�Merge

  Revision 1.20.2.1  2004/09/27 10:02:33  takano_makoto
  TP_CalcCalivrateParam�Ŋ��荞�݋֎~�̂܂܃G���[�I������ꍇ���������o�O���C��

  Revision 1.20  2004/09/13 05:49:56  yasu
  merged with NitroSDK_2_00pr6_branch

  Revision 1.19.2.1  2004/09/13 05:43:02  yasu
  Support SDK_NVRAM_FORMAT

  Revision 1.19  2004/09/03 04:29:15  terui
  NVRAM�����[�U�[���Ɋւ���\���̒�`�����ɔ����C���B

  Revision 1.18  2004/08/25 09:29:17  terui
  calibration���ǂݏo��������TS_VERSION�ɂ��؂蕪����悤�ɕύX�B

  Revision 1.17  2004/07/27 07:31:10  yasu
  Add #pragma  warn_padding off

  Revision 1.16  2004/06/10 06:52:15  yasu
  add comment @ TP_WaitBusy

  Revision 1.15  2004/06/10 05:27:03  yasu
  Add TP_GetCalibratedResult

  Revision 1.14  2004/06/10 05:00:47  takano_makoto
  TP_WaitBusy, TP_WaitAllBusy�ł�ASSERT�C��

  Revision 1.13  2004/06/10 04:41:37  yasu
  fix assert condition in TP_RequestAutoSamplingStartAsync

  Revision 1.12  2004/06/09 04:16:00  takano_makoto
  TP_GetCalibratePoint()��invalid�Ȓl�ɑ΂��Ă��ϊ������s����悤�ɏC��

  Revision 1.11  2004/06/02 06:14:37  takano_makoto
  �o�O�C���ATP_RequestAutoSamplingStartAsync(), TP_SetCallback()�̈����ύX

  Revision 1.10  2004/05/31 07:54:35  takano_makoto
  PXI���M����Ɋ��荞�݂����������ꍇ�̃o�O�C��

  Revision 1.9  2004/05/21 10:01:39  takano_makoto
  TP_GetUserInfo()��ǉ�

  Revision 1.8  2004/05/18 01:38:48  terui
  ARM7�Ƃ�PXI�����@�\�ɑΉ�

  Revision 1.7  2004/05/14 00:42:18  takano_makoto
  Modify Calibrate Parameter format.

  Revision 1.6  2004/04/21 12:13:38  takano_makoto
  Add Async function.

  Revision 1.5  2004/04/19 12:00:01  takano_makoto
  modify any functions interface.

  Revision 1.4  2004/04/16 02:13:37  takano_makoto
  Modify for STRB warning.

  Revision 1.3  2004/04/15 13:55:18  takano_makoto
  Chang any function name.

  Revision 1.2  2004/04/15 11:43:17  takano_makoto
  Change header file path.

  Revision 1.1  2004/04/15 10:56:17  takano_makoto
  Initial Update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/spi/ARM9/tp.h>
#include <nitro/spi/common/config.h>
#include "spi.h"

#define TP_RAW_MAX  0x1000
#define TP_CALIBRATE_DOT_INV_SCALE_SHIFT    (28 - TP_CALIBRATE_DOT_SCALE_SHIFT)
#define TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT (TP_CALIBRATE_DOT_SCALE_SHIFT - TP_CALIBRATE_ORIGIN_SCALE_SHIFT)

/*===========================================================================*
    Static function prototype definition
 *===========================================================================*/

static void TPi_TpCallback(PXIFifoTag tag, u32 data, BOOL err);

/*---------------------------------------------------------------------------*
    Static variables definition
 *---------------------------------------------------------------------------*/
typedef struct
{
    s32     x0;                        // X���W�̐ؕ�
    s32     xDotSize;                  // X�h�b�g��
    s32     xDotSizeInv;               // X����

    s32     y0;                        // Y���W�̐ؕ�
    s32     yDotSize;                  // Y�h�b�g��
    s32     yDotSizeInv;               // Y����

}
TPiCalibrateParam;

#pragma  warn_padding off
static struct
{
    TPRecvCallback callback;           // TP�l���擾�������ɌĂяo�����[�U�R�[���o�b�N�֐�
    TPData  buf;                       // TP�l�P��擾����TP�l��M�o�b�t�@
    u16     index;                     // �I�[�g�T���v�����O���̍ŐV�o�b�t�@�C���f�b�N�X
    u16     frequence;                 // �I�[�g�T���v�����O���̂P�t���[���T���v�����O��
    TPData *samplingBufs;              // �I�[�g�T���v�����O����TP�l�o�b�t�@�|�C���^
    u16     bufSize;                   // �I�[�g�T���v�����O����TP�o�b�t�@�T�C�Y
    // PADDING 2 BYTE
    TPiCalibrateParam calibrate;       // �L�����u���[�V�����p�����[�^
    u16     calibrate_flg;             // �L�����u���[�V�����t���O

    vu16    state;                     // �^�b�`�p�l���̏��
    vu16    err_flg;                   // �G���[�t���O
    vu16    command_flg;               // ���N�G�X�g�R�}���h���s���t���O
}
tpState;
#pragma  warn_padding reset


/*---------------------------------------------------------------------------*
    Inline sub-routine definition
    
    �����̖��߂ɑ΂���ARM7����̉�����PXI���C�u�����o�R�ŕԂ���܂��B
    PXI�Ƀ^�O"PXI_FIFO_TAG_TOUCHPANEL"�̃R�[���o�b�N���w�肷�邱�Ƃ�
    ARM7����̉����𓾂邱�Ƃ��ł��܂��B
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         TPi_SamplingNow

  Description:  �^�b�`�p�l�������T���v�����O����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL TPi_SamplingNow(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT
                               | SPI_PXI_END_BIT
                               | (0 << SPI_PXI_INDEX_SHIFT)
                               | (SPI_PXI_COMMAND_TP_SAMPLING << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         TPi_AutoSamplingOn

  Description:  �^�b�`�p�l���̃I�[�g�T���v�����O���J�n����B

  Arguments:    vCount -    �T���v�����O���s��V�J�E���g�B
                            1�t���[���ɕ�����T���v�����O����ꍇ�A������
                            �N�_��1�t���[���������������B
                frequency - 1�t���[���ɉ���T���v�����O���邩�̕p�x�B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL TPi_AutoSamplingOn(u16 vCount, u8 frequency)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT
                               | (0 << SPI_PXI_INDEX_SHIFT)
                               | (SPI_PXI_COMMAND_TP_AUTO_ON << 8) | (u32)frequency, 0))
    {
        return FALSE;
    }

    // Send packet [1]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | (u32)vCount, 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         TPi_AutoSamplingOff

  Description:  �^�b�`�p�l���̃I�[�g�T���v�����O���~����B

  Arguments:    None.

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL TPi_AutoSamplingOff(void)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT
                               | SPI_PXI_END_BIT
                               | (0 << SPI_PXI_INDEX_SHIFT)
                               | (SPI_PXI_COMMAND_TP_AUTO_OFF << 8), 0))
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         TPi_SetupStability

  Description:  �T���v�����O�ɂ�������蔻��p�����[�^��ݒ肷��B

  Arguments:    range - �A�������T���v�����O�ɂ����āA���o�d�������肵���Ƃ݂Ȃ��덷�B
                        �Ȃ��A���o�l��12bit�� 0 �` 4095�B

  Returns:      BOOL - ���߂�PXI�o�R�Ő���ɑ��M�ł����ꍇTRUE�A
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static inline BOOL TPi_SetupStability(u16 range)
{
    // Send packet [0]
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                               SPI_PXI_START_BIT
                               | SPI_PXI_END_BIT
                               | (0 << SPI_PXI_INDEX_SHIFT)
                               | (SPI_PXI_COMMAND_TP_SETUP_STABILITY << 8) | (u32)range, 0))
    {
        return FALSE;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         TPi_CopyTpFromSystemWork

  Description:  ARM7����shared memory�֏������܂ꂽ�^�b�`�p�l���l�̃f�[�^
                ��ʗ̈�ɓǂݏo���B

  Returns:      result - �ǂݏo���ꂽ�ŐV�̃^�b�`�p�l���l���i�[����B
 *---------------------------------------------------------------------------*/
static inline void TPi_CopyTpFromSystemWork(TPData *result)
{
    SPITpData spi_tp;

    spi_tp.halfs[0] = *(u16 *)(&(OS_GetSystemWork()->touch_panel[0]));
    spi_tp.halfs[1] = *(u16 *)(&(OS_GetSystemWork()->touch_panel[2]));

    // �V�X�e���̈悩��ǂݏo��( 2�o�C�g�A�N�Z�X )
    result->x = (u16)spi_tp.e.x;
    result->y = (u16)spi_tp.e.y;
    result->touch = (u8)spi_tp.e.touch;
    result->validity = (u8)spi_tp.e.validity;
}


/*---------------------------------------------------------------------------*
  Name:         TPi_ErrorAtPxi

  Description:  ARM7�Ƃ�PXI�ʐM�ɂ����ăG���[�����������ꍇ�̏����B
                �R�[���o�b�N���ݒ肳��Ă���ꍇ�ɂ�, TP_RESULT_PXI_BUSY
                �R�[���o�b�N��Ԃ��܂��B
                
  Arguments:    command - ���N�G�X�g�̎��
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void TPi_ErrorAtPxi(TPRequestCommand command)
{

    tpState.err_flg |= (1 << command);

    if (tpState.callback)
    {
        (tpState.callback) (command, TP_RESULT_PXI_BUSY, 0);
    }
}


/*===========================================================================*
    Function definition
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         TPi_TpCallback

  Description:  PXI��ARM7����^�b�`�p�l���֘A�̃��b�Z�[�W���󂯎�����ꍇ��
                �Ăяo�����֐��B
                ARM7����̃^�b�`�p�l���l�̑ޔ����s���A�R�[���o�b�N�֐���
                �ݒ肳��Ă���ꍇ�ɂ́A�X�Ƀ��[�U�[�R�[���o�b�N���Ăяo���B
                

  Arguments:    tag -  PXI����ʂ��������邽�߂̃^�O�B
                data - ARM7����̃��b�Z�[�W�f�[�^�B
                err -  PXI�]���̃G���[�t���O�B
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void TPi_TpCallback(PXIFifoTag tag, u32 data, BOOL err)
{
#pragma unused(tag)

    u16     result;
    u16     command;

    result = (u16)(data & SPI_PXI_DATA_MASK);
    command = (u16)((result & 0x7f00) >> 8);

    // PXI�]���G���[
    if (err)
    {
        TPi_ErrorAtPxi((TPRequestCommand)command);
        return;
    }

    if (command == SPI_PXI_COMMAND_TP_AUTO_SAMPLING)
    {
        // �I�[�g�T���v�����O�̎��s�����ʒm
//        tpState.index = (u16) (result & 0x00FF);
        tpState.index++;
        if (tpState.index >= tpState.bufSize)
        {
            tpState.index = 0;
        }

        // Shared�̈悩��^�b�`�p�l���l��ޔ�
        TPi_CopyTpFromSystemWork(&tpState.samplingBufs[tpState.index]);

        if (tpState.callback)
        {
            (tpState.callback) ((TPRequestCommand)command, TP_RESULT_SUCCESS, (u8)tpState.index);
        }
        return;
    }

    if (!(data & SPI_PXI_END_BIT))
    {
        return;
    }

    SDK_ASSERT(result & 0x8000);

    switch ((u8)(result & 0x00ff))
    {
    case SPI_PXI_RESULT_SUCCESS:
        // ���������ꍇ�̏���
        switch (command)
        {
        case SPI_PXI_COMMAND_TP_SAMPLING:
            // �T���v�����O���ʂ��擾
            TPi_CopyTpFromSystemWork(&tpState.buf);
            tpState.state = TP_STATE_READY;
            break;

        case SPI_PXI_COMMAND_TP_AUTO_ON:
            tpState.state = TP_STATE_AUTO_SAMPLING;
            break;

        case SPI_PXI_COMMAND_TP_AUTO_OFF:
            tpState.state = TP_STATE_READY;
            break;
        }

        // �r�W�[�t���O��������
        tpState.command_flg &= ~(1 << command);

        // �R�[���o�b�N�Ăяo��
        if (tpState.callback)
        {
            (tpState.callback) ((TPRequestCommand)command, TP_RESULT_SUCCESS, 0);
        }
        break;

    case SPI_PXI_RESULT_EXCLUSIVE:
        result = TP_RESULT_EXCLUSIVE;
        goto common;

    case SPI_PXI_RESULT_INVALID_PARAMETER:
        result = TP_RESULT_INVALID_PARAMETER;
        goto common;

    case SPI_PXI_RESULT_ILLEGAL_STATUS:
        result = TP_RESULT_ILLEGAL_STATUS;

      common:
        // �G���[����
        // �G���[�t���O�𗧂Ă�
        tpState.err_flg |= (1 << command);
        tpState.command_flg &= ~(1 << command);

        if (tpState.callback)
        {
            (tpState.callback) ((TPRequestCommand)command, (TPRequestResult)(result & 0xff), 0);
        }
        break;

    case SPI_PXI_RESULT_INVALID_COMMAND:
    default:
        // �ُ�I��
//        OS_Printf("result=%x\n",result);
        OS_Panic("Get illegal TP command from ARM7!\n");
        return;
    }
}




/*---------------------------------------------------------------------------*
  Name:         TP_Init

  Description:  �^�b�`�p�l�����C�u�����̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_Init(void)
{
    static u16 initial = FALSE;

    if (initial)
    {
        return;
    }
    initial = TRUE;

    //****************************************************************
    // Initialize TP.
    //****************************************************************
    PXI_Init();

    tpState.index = 0;
    tpState.callback = NULL;
    tpState.samplingBufs = NULL;
    tpState.state = TP_STATE_READY;
    tpState.calibrate_flg = FALSE;
    tpState.command_flg = 0;
    tpState.err_flg = 0;

    // 2003/05/18 Add by terui.
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_TOUCHPANEL, PXI_PROC_ARM7))
    {
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_TOUCHPANEL, TPi_TpCallback);
}


/*---------------------------------------------------------------------------*
  Name:         TP_GetUserInfo

  Description:  �L�����u���[�V�����̂��߂̃p�����[�^��{�̓��t���b�V�����������
                �擾����֐��B

  Returns:      param - �p�����[�^���擾����̈�ւ̃|�C���^�B
                BOOL  - �l�̎擾�ɐ��������ꍇ��TRUE
                        �L���Ȓl��������Ȃ������ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL TP_GetUserInfo(TPCalibrateParam *calibrate)
{
    NVRAMConfig *info = (NVRAMConfig *)(OS_GetSystemWork()->nvramUserInfo);
//    s16 x0, y0, xdot, ydot;
    u16     x1, y1, x2, y2, dx1, dy1, dx2, dy2;

    SDK_NULL_ASSERT(calibrate);

#if ( defined( SDK_TS ) && ( SDK_TS_VERSION >= 200 || SDK_NVRAM_FORMAT >= 100 ) )
    x1 = info->ncd.tp.raw_x1;
    y1 = info->ncd.tp.raw_y1;
    dx1 = (u16)(info->ncd.tp.dx1);
    dy1 = (u16)(info->ncd.tp.dy1);
    x2 = info->ncd.tp.raw_x2;
    y2 = info->ncd.tp.raw_y2;
    dx2 = (u16)(info->ncd.tp.dx2);
    dy2 = (u16)(info->ncd.tp.dy2);
#else
    x1 = *(u16 *)(&(info->ncd.tp.calib_data[0]));
    y1 = *(u16 *)(&(info->ncd.tp.calib_data[1]));
    dy1 = (u16)(*(u16 *)(&(info->ncd.tp.calib_data[2])) >> 8);
    dx1 = MI_ReadByte((u8 *)(&(info->ncd.tp.calib_data[2])));
    x2 = *(u16 *)(&(info->ncd.tp.calib_data[3]));
    y2 = *(u16 *)(&(info->ncd.tp.calib_data[4]));
    dy2 = (u16)(*(u16 *)(&(info->ncd.tp.calib_data[5])) >> 8);
    dx2 = MI_ReadByte((u8 *)(&(info->ncd.tp.calib_data[5])));
#endif

    /* �L�����u���[�V�����l�̗L���t���O���Ȃ��̂łƂ肠���� */
    if ((x1 == 0 && x2 == 0 && y1 == 0 && y2 == 0) ||
        (TP_CalcCalibrateParam(calibrate, x1, y1, dx1, dy1, x2, y2, dx2, dy2) != 0))
    {
        // IPL�Ńf�[�^��CRC�`�F�b�N�����Ă��Ȃ��̂ŁA�ُ�l�̏ꍇ�͂��ׂẴp�����[�^��
        // 0�ŃN���A�������TRUE��Ԃ��܂��B 
        // ���̏ꍇ�L�����u���[�g���TP�l�͏��(0,0)�ɂȂ�܂��B
        calibrate->x0 = 0;
        calibrate->y0 = 0;
        calibrate->xDotSize = 0;
        calibrate->yDotSize = 0;
        return TRUE;
    }
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         TP_SetCalibrateParam

  Description:  �L�����u���[�V�����̂��߂̃p�����[�^���Z�b�g����B
                ���̊֐��Ńp�����[�^��ݒ肷��ƁA�ȍ~TP_WaitCalibratedResult()
                TP_GetLatestCalibratedPointInAuto(), TP_GetCalibratedPoint(),
                TP_GetUnCalibratedPoint()�ɂ����Ă��̒l���g�p���ăL�����u���[�V������
                �s����B�h�b�g�T�C�Y�̋t���͂��̎��_�ł��炩���ߌv�Z�����B

  Arguments:    param - �ݒ肷��L�����u���[�V�����p�����[�^�ւ̃|�C���^�B
                        NULL���ݒ肳�ꂽ�ꍇ�ɂ́A�ȍ~�L�����u���[�V�����ɂ����W�ϊ���
                        �s���Ȃ��B (�f�t�H���g�l: NULL)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_SetCalibrateParam(const TPCalibrateParam *param)
{
    OSIntrMode enabled;

    if (param == NULL)
    {
        tpState.calibrate_flg = FALSE;
        return;
    }

    // �\��xDotSize, yDotSIze�̋t�����v�Z���Ă���
    enabled = OS_DisableInterrupts();

    if (param->xDotSize != 0)
    {
        CP_SetDiv32_32(0x10000000, (u32)param->xDotSize);

        tpState.calibrate.x0 = param->x0;
        tpState.calibrate.xDotSize = param->xDotSize;
        tpState.calibrate.xDotSizeInv = (s32)CP_GetDivResult32();
    }
    else
    {
        tpState.calibrate.x0 = 0;
        tpState.calibrate.xDotSize = 0;
        tpState.calibrate.xDotSizeInv = 0;
    }

    if (param->yDotSize != 0)
    {
        CP_SetDiv32_32(0x10000000, (u32)param->yDotSize);

        tpState.calibrate.y0 = param->y0;
        tpState.calibrate.yDotSize = param->yDotSize;
        tpState.calibrate.yDotSizeInv = (s32)CP_GetDivResult32();
    }
    else
    {
        tpState.calibrate.y0 = 0;
        tpState.calibrate.yDotSize = 0;
        tpState.calibrate.yDotSizeInv = 0;
    }

    (void)OS_RestoreInterrupts(enabled);

    tpState.calibrate_flg = TRUE;

}


/*---------------------------------------------------------------------------*
  Name:         TP_SetCallback

  Description:  �^�b�`�p�l�����珈�����ʂ��Ԃ��Ă������ɌĂяo��
                ���[�U�R�[���o�b�N�֐���ݒ肷��B

  Arguments:    callback - ���[�U�R�[���o�b�N�֐��|�C���^�B
                           NULL�̏ꍇ�̓R�[���o�b�N���Ăяo���Ȃ�(�f�t�H���g:NULL)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_SetCallback(TPRecvCallback callback)
{
    OSIntrMode enabled;

    enabled = OS_DisableInterrupts();
    tpState.callback = callback;
    (void)OS_RestoreInterrupts(enabled);
}



/*---------------------------------------------------------------------------*
  Name:         TP_RequestSamplingAsync

  Description:  ARM7�ɑ΂��ă^�b�`�p�l���l��v������B
                ���̊֐����R�[��������TP_WaitRawResult() �܂��́A
                TP_WaitCalibratedResult()�Ń^�b�`�p�l���l��ǂݏo�����Ƃ��ł���B
                �I�[�g�T���v�����O���͎g�p�ł��Ȃ��B
                
  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_RequestSamplingAsync(void)
{
    OSIntrMode enabled;

    SDK_ASSERT(!(tpState.command_flg & TP_REQUEST_COMMAND_FLAG_SAMPLING));

    enabled = OS_DisableInterrupts();

    if (TPi_SamplingNow() == FALSE)
    {
        // PXI���M���s
        (void)OS_RestoreInterrupts(enabled);
        TPi_ErrorAtPxi(TP_REQUEST_COMMAND_SAMPLING);
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_SAMPLING;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_SAMPLING;

    (void)OS_RestoreInterrupts(enabled);
}


/*---------------------------------------------------------------------------*
  Name:         TP_WaitRawResult

  Description:  ARM7����^�b�`�p�l���l���Z�b�g�����̂�҂��A
                �L�����u���[�g����Ă��Ȃ����̒l��ǂݏo���B
                �����œ�����l�̓`���^�����O�΍�ς݁B

  Arguments:    None.

  Returns:      result - �^�b�`�p�l���l���擾���邽�߂̕ϐ��ւ̃|�C���^�B
                         x,y���W�̓L�����u���[�g����Ă��Ȃ��l(0�`4095)���Ƃ�B
                u32    - �f�[�^�̎擾�ɐ��������ꍇ�ɂ� 0�A
                         ���s�����ꍇ�ɂ� 0�ȊO��Ԃ��B
 *---------------------------------------------------------------------------*/
u32 TP_WaitRawResult(TPData *result)
{
    SDK_NULL_ASSERT(result);

    TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_SAMPLING);

    if (tpState.err_flg & TP_REQUEST_COMMAND_FLAG_SAMPLING)
    {
        return 1;
    }

    *result = tpState.buf;
    return 0;
}



/*---------------------------------------------------------------------------*
  Name:         TP_GetCalibratedResult

  Description:  ARM7����^�b�`�p�l���l���Z�b�g����Ă���Ƃ݂Ȃ��A
                �L�����u���[�g���ꂽ��ʍ��W�ɑΉ������l��ǂݏo���B
                �����œ�����l�̓`���^�����O�΍�ς݁B

  Arguments:    result - �^�b�`�p�l���l���擾���邽�߂̕ϐ��ւ̃|�C���^�B
                         x,y���W�͉�ʍ��W�ɑΉ������l���Ƃ�B
                         �������L�����u���[�V�����p�����[�^���ݒ肳��Ă��Ȃ��ꍇ
                         �ɂ�(0�`4095)�̃^�b�`�p�l���l��������B
                         
  Returns:      u32    - �f�[�^�̎擾�ɐ��������ꍇ�ɂ� 0�A
                         ���s�����ꍇ�ɂ� 0�ȊO��Ԃ��B
 *---------------------------------------------------------------------------*/
u32 TP_GetCalibratedResult(TPData *result)
{
    SDK_NULL_ASSERT(result);

    if (tpState.err_flg & TP_REQUEST_COMMAND_FLAG_SAMPLING)
    {
        return 1;
    }

    *result = tpState.buf;
    TP_GetCalibratedPoint(result, result);
    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         TP_WaitCalibratedResult

  Description:  ARM7����^�b�`�p�l���l���Z�b�g�����̂�҂��A
                �L�����u���[�g���ꂽ��ʍ��W�ɑΉ������l��ǂݏo���B
                �����œ�����l�̓`���^�����O�΍�ς݁B

  Arguments:    result - �^�b�`�p�l���l���擾���邽�߂̕ϐ��ւ̃|�C���^�B
                         x,y���W�͉�ʍ��W�ɑΉ������l���Ƃ�B
                         �������L�����u���[�V�����p�����[�^���ݒ肳��Ă��Ȃ��ꍇ
                         �ɂ�(0�`4095)�̃^�b�`�p�l���l��������B
                         
  Returns:      u32    - �f�[�^�̎擾�ɐ��������ꍇ�ɂ� 0�A
                         ���s�����ꍇ�ɂ� 0�ȊO��Ԃ��B
 *---------------------------------------------------------------------------*/
u32 TP_WaitCalibratedResult(TPData *result)
{
    TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_SAMPLING);
    return TP_GetCalibratedResult(result);
}


/*---------------------------------------------------------------------------*
  Name:         TP_RequestAutoSamplingStartAsync

  Description:  ARM7�ɑ΂��ă^�b�`�p�l���l�̃I�[�g�T���v�����O�J�n�v�����o���B
                1�t���[����frequence��̃f�[�^���ϓ��ȊԊu�ŃT���v�����O����A
                ���ʂ�samplingBufs�Ŏw�肵���z��֊i�[����B

  Arguments:    vcount       - �I�[�g�T���v�����O���s����ƂȂ�VCOUNT�l��ݒ�B
                frequence    - �P�t���[���ɉ���̃T���v�����O���s�����̐ݒ�B
                samplingBufs - �I�[�g�T���v�����O�����f�[�^���i�[����̈��ݒ�B
                bufSize      - �o�b�t�@�T�C�Y�̐ݒ�B
                               samplingBufs�̔z��busSize�̃����O�o�b�t�@�Ƃ��Ďg�p�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 frequence, TPData samplingBufs[], u16 bufSize)
{
    u32     i;
    OSIntrMode enabled;

    SDK_NULL_ASSERT(samplingBufs);
    SDK_ASSERT(vcount < HW_LCD_LINES);
    SDK_ASSERT(frequence != 0 && frequence <= SPI_TP_SAMPLING_FREQUENCY_MAX);
    SDK_ASSERT(tpState.state == TP_STATE_READY);
    SDK_ASSERT(!(tpState.command_flg & TP_REQUEST_COMMAND_FLAG_AUTO_ON));
    SDK_ASSERT(bufSize != 0);

    tpState.samplingBufs = samplingBufs;
    tpState.index = 0;
    tpState.frequence = frequence;
    tpState.bufSize = bufSize;

    for (i = 0; i < bufSize; i++)
    {
        tpState.samplingBufs[i].touch = TP_TOUCH_OFF;
    }

    enabled = OS_DisableInterrupts();

    if ((u8)TPi_AutoSamplingOn(vcount, (u8)frequence) == FALSE)
    {
        // PXI���M���s
        (void)OS_RestoreInterrupts(enabled);
        TPi_ErrorAtPxi(TP_REQUEST_COMMAND_AUTO_ON);
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_ON;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_AUTO_ON;

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         TP_RequestAutoSamplingStopAsync

  Description:  ARM7�ɑ΂��ă^�b�`�p�l���l�̃I�[�g�T���v�����O��~�v�����o���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_RequestAutoSamplingStopAsync(void)
{
    OSIntrMode enabled;

    SDK_ASSERT(tpState.state == TP_STATE_AUTO_SAMPLING);
    SDK_ASSERT(!(tpState.command_flg & TP_REQUEST_COMMAND_FLAG_AUTO_OFF));

    enabled = OS_DisableInterrupts();

    if (TPi_AutoSamplingOff() == FALSE)
    {
        // PXI���M���s
        (void)OS_RestoreInterrupts(enabled);
        TPi_ErrorAtPxi(TP_REQUEST_COMMAND_AUTO_OFF);
        return;
    }

    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_OFF;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_AUTO_OFF;

    (void)OS_RestoreInterrupts(enabled);

}


/*---------------------------------------------------------------------------*
  Name:         TP_RequestSetStabilityAsync

  Description:  �^�b�`�p�l���̃`���^�����O�΍�p�����[�^��ݒ肷��B
                �l�����肷��܂ł̃��g���C�T���v�����O�񐔂ƁA
                �l�����肵���Ɣ��肷�邽�߂̃����W��ݒ�B

  Arguments:    retry -  ���̈����͓����ł͎g�p����Ă��܂���B
                range -  �l�����肵�����ǂ����𔻒肷�邽�߂̃����W.
                         ���W�l�����͈̔͂Ɏ������Ȃ������ꍇ�ɂ�invalid�ƂȂ�܂��B
                         (�͈�:0�`4095, �f�t�H���g�l:20)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_RequestSetStabilityAsync(u8 retry, u16 range)
{
#pragma unused( retry )
    OSIntrMode enabled;

    SDK_ASSERT(range != 0);
    SDK_ASSERT(range < 255);

    enabled = OS_DisableInterrupts();

    if (TPi_SetupStability(range) == FALSE)
    {
        // PXI���M���s
        (void)OS_RestoreInterrupts(enabled);
        TPi_ErrorAtPxi(TP_REQUEST_COMMAND_SET_STABILITY);
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_SET_STABILITY;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_SET_STABILITY;

    (void)OS_RestoreInterrupts(enabled);
}



/*---------------------------------------------------------------------------*
  Name:         TP_GetLatestRawPointInAuto

  Description:  �I�[�g�T���v�����O���ʂ���A�L���ȍŐV�̃^�b�`�p�l���l���擾����B
                �l��x,y���W�̓L�����u���[�V��������Ă��Ȃ�(0�`4095)�͈̔͂��Ƃ�B

  Arguments:    None.

  Returns:      result - �L���ȍŐV�̃^�b�`�p�l���l���擾���邽�߂̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TP_GetLatestRawPointInAuto(TPData *result)
{
    s32     i, curr_index;
    TPData *tp;

    SDK_NULL_ASSERT(result);
    SDK_NULL_ASSERT(tpState.samplingBufs);

    result->validity = TP_VALIDITY_INVALID_XY;

    curr_index = tpState.index;

    // �T���v�����O�o�b�t�@�T�C�Y���P�̏ꍇ�́A���݂̍��W�l�����̂܂ܕԂ�
    if (tpState.frequence == 1 || tpState.bufSize == 1)
    {
        *result = tpState.samplingBufs[curr_index];
        return;
    }

    // �L���ȃT���v�����O�l�̒��ōŐV�̂��̂�����
    for (i = 0; i < tpState.frequence && i < tpState.bufSize - 1; i++)
    {
        s32     index;

        index = curr_index - i;
        if (index < 0)
        {
            index += tpState.bufSize;
        }

        tp = &tpState.samplingBufs[index];

        if (!tp->touch)
        {
            *result = *tp;
            return;
        }

        // �����ȃf�[�^���܂ޏꍇ�͂P�Â��T���v�����O�l���Q��
        if (result->validity & TP_VALIDITY_INVALID_X)
        {
            /* X���W */
            if (!(tp->validity & TP_VALIDITY_INVALID_X))
            {
                result->x = tp->x;
                if (i != 0)
                {
                    result->validity &= ~TP_VALIDITY_INVALID_X;
                }
            }
        }

        if (result->validity & TP_VALIDITY_INVALID_Y)
        {
            /* Y���W */
            if (!(tp->validity & TP_VALIDITY_INVALID_Y))
            {
                result->y = tp->y;
                if (i != 0)
                {
                    result->validity &= ~TP_VALIDITY_INVALID_Y;
                }
            }
        }

        if (result->validity == TP_VALIDITY_VALID)
        {
            result->touch = TP_TOUCH_ON;
            return;
        }
    }

    // �����ȍ��W�l���c���Ă���ꍇ�͎擾�ł����l��Ԃ��B
    result->touch = TP_TOUCH_ON;
    return;
}



/*---------------------------------------------------------------------------*
  Name:         TP_GetLatestCalibratedPointInAuto

  Description:  �I�[�g�T���v�����O���ʂ���A�L���ȍŐV�̃^�b�`�p�l���l���擾����B
                �l��x,y���W�͉�ʍ��W�͈̔͂��Ƃ�B

  Arguments:    None.

  Returns:      result - �L���ȍŐV�̃^�b�`�p�l���l���擾���邽�߂̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TP_GetLatestCalibratedPointInAuto(TPData *result)
{
    SDK_NULL_ASSERT(result);

    TP_GetLatestRawPointInAuto(result);

    TP_GetCalibratedPoint(result, result);

}


/*---------------------------------------------------------------------------*
  Name:         TP_GetLatestIndexInAuto

  Description:  �I�[�g�T���v�����O�ɂ���Ēl���i�[����郋�[�v�o�b�t�@�̒��ŁA
                �Ō�ɃT���v�����O���ꂽ�f�[�^�̃C���f�b�N�X��Ԃ��B

  Arguments:    None.

  Returns:      u16 - �Ō�ɃT���v�����O���ꂽ�f�[�^�̃C���f�b�N�X�B
 *---------------------------------------------------------------------------*/
u16 TP_GetLatestIndexInAuto(void)
{
    return tpState.index;
}




/*---------------------------------------------------------------------------*
  Name:         TP_CalcCalibrateParam

  Description:  �Q�_�̃^�b�`�p�l���l�Ɖ�ʍ��W����A�L�����u���[�V�����p��
                �p�����[�^���v�Z����B

  Arguments:    raw_x1, raw_y1 - �|�C���g�P�̃^�b�`�p�l���l
                dx1, dy1       - �|�C���g�P�̉�ʍ��W
                raw_x2, raw_y2 - �|�C���g�Q�̃^�b�`�p�l���l
                dx2, dy2,      - �|�C���g�Q�̉�ʍ��W
                
  Returns:      calibrate      - �L�����u���[�V�����p�����[�^
                u32            - �L���ȃp�����[�^�̏ꍇ��0
                                 �L���łȂ��p�����[�^�̏ꍇ��0�ȊO��Ԃ�
                                 
 *---------------------------------------------------------------------------*/
u32 TP_CalcCalibrateParam(TPCalibrateParam *calibrate,
                          u16 raw_x1, u16 raw_y1,
                          u16 dx1, u16 dy1, u16 raw_x2, u16 raw_y2, u16 dx2, u16 dy2)
{
    s32     rx_width, dx_width, ry_width, dy_width;
    s32     tmp32;
    OSIntrMode enabled;

#define IN_S16_RANGE(x) ((x) < 0x8000 && (x) >= -0x8000)

    /*                                                                  */
    /* xDotSize = ((raw_x1 - raw_x2) << SCALE_SHIFT) / (dx1 - dx2)      */
    /* x0 = ((raw_x1 + raw_x2) - (dx1 + dx2) * xDotSize) / 2            */
    /* xDotSize = ((raw_x1 - raw_x2) << SCALE_SHIFT) / (dx1 - dx2)      */
    /* x0 = ((raw_x1 + raw_x2) - (dx1 + dx2) * xDotSize) / 2            */
    /*                                                                  */

    SDK_NULL_ASSERT(calibrate);
    // ���W�͈̔̓`�F�b�N
    if (raw_x1 >= TP_RAW_MAX || raw_y1 >= TP_RAW_MAX || raw_x2 >= TP_RAW_MAX
        || raw_y2 >= TP_RAW_MAX)
    {
        return 1;
    }
    if (dx1 >= GX_LCD_SIZE_X || dx2 >= GX_LCD_SIZE_X || dy1 >= GX_LCD_SIZE_Y
        || dy2 >= GX_LCD_SIZE_Y)
    {
        return 1;
    }
    if (dx1 == dx2 || dy1 == dy2 || raw_x1 == raw_x2 || raw_y1 == raw_y2)
    {
        return 1;
    }

    rx_width = raw_x1 - raw_x2;
    dx_width = dx1 - dx2;

    enabled = OS_DisableInterrupts();

    // xDotSize���v�Z
    CP_SetDiv32_32(((u32)rx_width) << TP_CALIBRATE_DOT_SCALE_SHIFT, (u32)dx_width);

    ry_width = raw_y1 - raw_y2;
    dy_width = dy1 - dy2;

    tmp32 = CP_GetDivResult32();
    CP_SetDiv32_32(((u32)ry_width) << TP_CALIBRATE_DOT_SCALE_SHIFT, (u32)dy_width);

    if (!IN_S16_RANGE(tmp32))
    {
        (void)OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->xDotSize = (s16)tmp32;
    tmp32 = (s16)((((s32)(raw_x1 + raw_x2) << TP_CALIBRATE_DOT_SCALE_SHIFT)
                   - ((s32)(dx1 + dx2) * calibrate->xDotSize)) >> (TP_CALIBRATE_DOT_SCALE_SHIFT -
                                                                   TP_CALIBRATE_ORIGIN_SCALE_SHIFT +
                                                                   1));
    if (!IN_S16_RANGE(tmp32))
    {
        (void)OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->x0 = (s16)tmp32;

    tmp32 = CP_GetDivResult32();
    (void)OS_RestoreInterrupts(enabled);

    if (!IN_S16_RANGE(tmp32))
    {
        return 1;
    }
    calibrate->yDotSize = (s16)tmp32;
    tmp32 = (s16)((((s32)(raw_y1 + raw_y2) << TP_CALIBRATE_DOT_SCALE_SHIFT)
                   - ((s32)(dy1 + dy2) * calibrate->yDotSize)) >> (TP_CALIBRATE_DOT_SCALE_SHIFT -
                                                                   TP_CALIBRATE_ORIGIN_SCALE_SHIFT +
                                                                   1));
    if (!IN_S16_RANGE(tmp32))
    {
        return 1;
    }
    calibrate->y0 = (s16)tmp32;

    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         TP_GetCalibratedPoint
  
  Description:  �^�b�`�p�l���l�����ʍ��W�ւ̕ϊ��������Ȃ��B
                �L�����u���[�V�����p�����[�^���ݒ肳��Ă��Ȃ��ꍇ�ɂ�
                �^�b�`�p�l���l�����̂܂܏o�͂����B
                ����disp��raw�ɂ͓����|�C���^��n���Ă��\��Ȃ��B

  Arguments:    raw      - �ϊ����̃^�b�`�p�l���l�ւ̃|�C���^�B
                
  ReturnValue:  disp     - ��ʍ��W�֕ϊ����ꂽ�l���Ԃ����ϐ��ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TP_GetCalibratedPoint(TPData *disp, const TPData *raw)
{
    TPiCalibrateParam *cal;

    enum
    { MAX_X = GX_LCD_SIZE_X - 1, MAX_Y = GX_LCD_SIZE_Y - 1 };

    // ----------------------------------------
    // dx = (raw_x * x0) / xDotSize
    // dy = (raw_y * y0) / yDotSize
    // ----------------------------------------

    SDK_NULL_ASSERT(disp);
    SDK_NULL_ASSERT(raw);
    SDK_NULL_ASSERT(tpState.calibrate_flg);

    if (!tpState.calibrate_flg)
    {
        *disp = *raw;
        return;
    }

    cal = &tpState.calibrate;

    disp->touch = raw->touch;
    disp->validity = raw->validity;

    if (raw->touch == 0)
    {
        disp->x = 0;
        disp->y = 0;
        return;
    }

    // X���W�ϊ�
    // disp->x = (x - x0) / xDotSize;
    disp->x =
        (u16)((((u64)(raw->x << TP_CALIBRATE_ORIGIN_SCALE_SHIFT) -
                cal->x0) * cal->xDotSizeInv) >> (TP_CALIBRATE_DOT_INV_SCALE_SHIFT +
                                                 TP_CALIBRATE_ORIGIN_SCALE_SHIFT));

    if ((s16)disp->x < 0)
    {
        disp->x = 0;
    }
    else if ((s16)disp->x > MAX_X)
    {
        disp->x = MAX_X;
    }
    // Y���W�ϊ�
    // disp->y = (y - y0) / yDotSize;
    disp->y =
        (u16)((((u64)(raw->y << TP_CALIBRATE_ORIGIN_SCALE_SHIFT) -
                cal->y0) * cal->yDotSizeInv) >> (TP_CALIBRATE_DOT_INV_SCALE_SHIFT +
                                                 TP_CALIBRATE_ORIGIN_SCALE_SHIFT));

    if ((s16)disp->y < 0)
    {
        disp->y = 0;
    }
    else if ((s16)disp->y > MAX_Y)
    {
        disp->y = MAX_Y;
    }
}

/*---------------------------------------------------------------------------*
  Name:         TP_GetUnCalibratedPoint

  Description:  �L�����u���[�V�����̋t�ϊ����ʂ��擾����B
                ��ʍ��W����^�b�`�p�l���l�ւ̕ϊ��B
                �L�����u���[�V�����p�����[�^���ݒ肳��Ă��Ȃ��ꍇ�ɂ�
                ��ʍ��W�����̂܂܏o�͂����B

  Arguments:    dx, dy       - ��ʂ�XY���W�B
                
  ReturnValue:  raw_x, raw_y - �Ή�����^�b�`�p�l���l��Ԃ����߂̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TP_GetUnCalibratedPoint(u16 *raw_x, u16 *raw_y, u16 dx, u16 dy)
{
    TPiCalibrateParam *cal;

    // ----------------------------------------
    // raw_x = dx * xDotSize + x0;
    // raw_y = dy * yDotSize + y0;
    // ----------------------------------------

    SDK_NULL_ASSERT(raw_x);
    SDK_NULL_ASSERT(raw_y);
    SDK_ASSERT(tpState.calibrate_flg);

    if (!tpState.calibrate_flg)
    {
        *raw_x = dx;
        *raw_y = dy;
        return;
    }

    cal = &tpState.calibrate;

    *raw_x =
        (u16)((((s32)(dx * cal->xDotSize) >> TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT) +
               (s32)cal->x0) >> TP_CALIBRATE_ORIGIN_SCALE_SHIFT);
    *raw_y =
        (u16)((((s32)(dy * cal->yDotSize) >> TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT) +
               (s32)cal->y0) >> TP_CALIBRATE_ORIGIN_SCALE_SHIFT);
}


/*---------------------------------------------------------------------------*
  Name:         TP_WaitBusy

  Description:  ARM7���^�b�`�p�l���̗v�����I������܂ŃE�F�C�g����B

  Arguments:    command_flgs - �^�b�`�p�l���ւ̗v���̎�ށB
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_WaitBusy(TPRequestCommandFlag command_flgs)
{
#ifdef  SDK_DEBUG
    if (!(tpState.command_flg & command_flgs))
    {
        return;
    }
#endif
    // ���荞�݂�OFF�̏ꍇ�͖������[�v�Ɋׂ�
    SDK_ASSERT(OS_GetCpsrIrq() == OS_INTRMODE_IRQ_ENABLE);
    SDK_ASSERT(reg_OS_IME == 1);
    SDK_ASSERT(OS_GetIrqMask() & OS_IE_SPFIFO_RECV);

    // ���[�v�̊J�n�O�� ASSERT ������D���荞�݋֎~��Ԃł��̊֐���
    // �Ă΂�邱�Ƃ������ׂ��ł͂Ȃ��DtpState �̃t���O�����ɃN���A�����
    // ���邩�ǂ����͏󋵂ɂ���č��E�����\�������邽��

    while (tpState.command_flg & command_flgs)
    {
        // Do nothing
    }

    return;
}

/*---------------------------------------------------------------------------*
  Name:         TP_WaitAllBusy

  Description:  ARM7���^�b�`�p�l���̗v�������ׂďI������܂ŃE�F�C�g����B

  Arguments:    None.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void TP_WaitAllBusy(void)
{
#ifdef  SDK_DEBUG
    if (!tpState.command_flg)
    {
        return;
    }
#endif
    // ���荞�݂�OFF�̏ꍇ�͖������[�v�Ɋׂ�
    SDK_ASSERT(OS_GetCpsrIrq() == OS_INTRMODE_IRQ_ENABLE);
    SDK_ASSERT(reg_OS_IME == 1);
    SDK_ASSERT(OS_GetIrqMask() & OS_IE_SPFIFO_RECV);

    while (tpState.command_flg)
    {
        // Do nothing
    }

    return;
}


/*---------------------------------------------------------------------------*
  Name:         TP_CheckBusy

  Description:  ARM7�ւ̃^�b�`�p�l���v�����r�W�[���ł��邩���`�F�b�N����B

  Arguments:    command_flgs - �^�b�`�p�l���ւ̗v���̎�ށB
                
  Returns:      u32          - �r�W�[���łȂ���� 0�A
                               �r�W�[���Ȃ�� 0�ȊO��Ԃ�
 *---------------------------------------------------------------------------*/
u32 TP_CheckBusy(TPRequestCommandFlag command_flgs)
{
    return (u32)(tpState.command_flg & command_flgs);
}



/*---------------------------------------------------------------------------*
  Name:         TP_CheckError

  Description:  ARM7�ւ̃^�b�`�p�l���v�����G���[�I�����Ă��Ȃ����`�F�b�N����B

  Arguments:    command_flgs  - �^�b�`�p�l���ւ̗v���̎�ށB
                
  Returns:      u32           - �G���[���������Ă��Ȃ���� 0�A
                                �G���[���������Ă���� 0�ȊO��Ԃ�
 *---------------------------------------------------------------------------*/
u32 TP_CheckError(TPRequestCommandFlag command_flgs)
{
    return (u32)(tpState.err_flg & command_flgs);
}
