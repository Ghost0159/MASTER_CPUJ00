/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_child.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_measure_channel.c,v $
  Revision 1.10  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2005/02/18 11:08:01  yosizaki
  fix around hidden warnings.

  Revision 1.7  2004/12/09 08:15:19  takano_makoto
  �����̎�̎擾���@��OS_GetVBlankCount�֐����g�p����悤�ɏC��

  Revision 1.6  2004/12/08 14:27:30  takano_makoto
  �����d�g�g�p���̃`�����l�����������ꍇ�ɂ͗����Ō���

  Revision 1.5  2004/10/22 04:10:30  takano_makoto
  MBM_MeasureChannelEx��MBM_MeasureChannelInIdle�ɉ���

  Revision 1.4  2004/10/22 02:54:54  takano_makoto
  ���[�U�R�[���o�b�N�O�ɏ�Ԃ��N���A����悤�ɏC��

  Revision 1.3  2004/10/22 02:39:57  takano_makoto
  MBM_MeasureChannelEx��ǉ�

  Revision 1.2  2004/09/28 09:52:44  takano_makoto
  �f�o�b�O�v�����g�̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/wm.h>

#include "mb_measure_channel.h"

#define MBM_DEBUG

#ifdef MBM_DEBUG
#define PRINTF OS_TPrintf
#else
#define PRINTF(...) ((void)0)
#endif

#define MBM_MEASURE_DMA_NO                 2

// �����}�N��
#define RAND()  ( sRand = sRand + 69069UL + 12345 )
#define RAND_INIT(x) ( sRand = (u32)(x) )

enum
{
    MBM_STATE_INIT,                    // �������
    MBM_STATE_MEASURING,               // �J�n���
    MBM_STATE_END_MEASURE,             // �����I��
    MBM_STATE_END,                     // ����OFF���
    MBM_STATE_ERR                      // �G���[���
};


enum
{
    ERRCODE_SUCCESS = 0,               // ����
    ERRCODE_NOMORE_CHANNEL,            // ����ȏ�̃`�����l����������Ȃ�
    ERRCODE_API_ERR                    // WM��API���s�G���[
};

//===========================================================================
// �ϐ��錾
//===========================================================================

static u32 sRand;

static MBMCallbackFunc sUserCallbackFunc = NULL;
static int mbm_measure_state = MBM_STATE_END;
// �ʐM�`�����l���i�[�p
static s16 sChannel;
static u16 sChannelBitmap;
static u16 sChannelBusyRatio;

static BOOL sUseInIdle = FALSE;


//===========================================================================
// �֐��v���g�^�C�v�錾
//===========================================================================

static int wmi_measure_channel(WMCallbackFunc callback, u16 channel);
static void wm_callback(void *arg);
static void start_measure_channel(void);
static u16 state_in_measure_channel(u16 channel);
static void state_out_measure_channel(void *arg);
static void state_in_wm_end(void);
static void user_callback(s16 num);
static void change_mbm_state(u16 state);
static void callback_ready(s16 result);
static s16 select_channel(u16 bitmap);

//===========================================================================
// �֐���`
//===========================================================================

/* ----------------------------------------------------------------------
  �d�g�g�p���̃`�F�b�N
  ---------------------------------------------------------------------- */
static inline int wmi_measure_channel(WMCallbackFunc callback, u16 channel)
{
#define MBM_MEASURE_TIME         30    // 1�t���[���Ɉ��ʐM���Ă���d�g���E���邾���̊Ԋu(ms)
#define MBM_MEASURE_CS_OR_ED     3     // �L�����A�Z���X��ED�l�̘_���a
#define MBM_MEASURE_ED_THRESHOLD 17    // �����f�[�^�ɂ��o���I�ɗL���Ǝv���邨����ED臒l

    /*
     * �d�g�g�p���擾�p�����[�^�Ƃ��āA
     * �����ɂ��o���I�ɗL���Ǝv����l�����Ă��܂��B
     */
    return WM_MeasureChannel(callback, // �R�[���o�b�N�ݒ�
                             MBM_MEASURE_CS_OR_ED,      // CS or ED
                             MBM_MEASURE_ED_THRESHOLD,  // �L�����A�Z���X�݂̂̏ꍇ�͖���
                             channel,  // WM_GetAllowedChannel�Ŏ擾�����`�����l���̂ǂꂩ
                             MBM_MEASURE_TIME); //�P�`�����l��������̒�������(ms)
}



/*---------------------------------------------------------------------------*
  Name:         MBM_MeasureChannel

  Description:  �ł��g�p���̒Ⴂ�`�����l�����������܂��B
                ����off��ԂŃR�[�����Ă��������B
                �����œd�g�𑪒肵����off��Ԃɂ��Ă���callback��Ԃ��܂��B

  Arguments:    wm_buffer   WM���[�N������
                callback    �����������ɌĂяo�����[�U�R�[���o�b�N���w��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBM_MeasureChannel(u8 *wm_buffer, MBMCallbackFunc callback)
{
    sUseInIdle = FALSE;
    sUserCallbackFunc = callback;

    // �������V�[�P���X�J�n�B
    if (mbm_measure_state != MBM_STATE_END)
    {
        user_callback(MBM_MEASURE_ERROR_ILLEGAL_STATE);
        return;
    }

    if (WM_Initialize(wm_buffer, wm_callback, MBM_MEASURE_DMA_NO) != WM_ERRCODE_OPERATING)
    {
        user_callback(MBM_MEASURE_ERROR_INIT_API);
        return;
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBM_MeasureChannelInIdle

  Description:  �ł��g�p���̒Ⴂ�`�����l�����������܂��B
                IDLE�X�e�[�g�ŃR�[�����Ă��������B
                �����œd�g�𑪒肵IDLE�X�e�[�g�ɂ��Ă���callback��Ԃ��܂��B

  Arguments:    callback    �����������ɌĂяo�����[�U�R�[���o�b�N���w��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBM_MeasureChannelInIdle(MBMCallbackFunc callback)
{
    sUseInIdle = TRUE;
    sUserCallbackFunc = callback;

    // �������V�[�P���X�J�n�B
    if (mbm_measure_state != MBM_STATE_END)
    {
        user_callback(MBM_MEASURE_ERROR_ILLEGAL_STATE);
        return;
    }

    start_measure_channel();
}

/* ----------------------------------------------------------------------
  WM�R�[���o�b�N
  ---------------------------------------------------------------------- */
static void wm_callback(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    switch (cb->apiid)
    {
    case WM_APIID_INITIALIZE:
        /* WM_Initialize�̃R�[���o�b�N */
        {
            WMCallback *cb = (WMCallback *)arg;
            if (cb->errcode != WM_ERRCODE_SUCCESS)
            {
                user_callback(MBM_MEASURE_ERROR_INIT_CALLBACK);
                return;
            }
        }
        change_mbm_state(MBM_STATE_INIT);
        start_measure_channel();
        break;

    case WM_APIID_MEASURE_CHANNEL:
        /* WM_MeasureChannel�̃R�[���o�b�N */
        state_out_measure_channel(arg);
        break;

    case WM_APIID_END:
        change_mbm_state(MBM_STATE_END);
        user_callback(sChannel);
        break;

    default:
        OS_TPanic("Get illegal callback");

        break;
    }
}

/* ----------------------------------------------------------------------
  �d�g�g�p�������J�n
  ---------------------------------------------------------------------- */
static void start_measure_channel(void)
{
#define MAX_RATIO 100                  // �`�����l���g�p����0�`100�͈̔�
    u16     result;
    u8      macAddr[6];

    OS_GetMacAddress(macAddr);
    RAND_INIT(OS_GetVBlankCount() + *(u16 *)&macAddr[0] + *(u16 *)&macAddr[2] + *(u16 *)&macAddr[4]);   // ����������
    RAND();

    sChannel = 0;
    sChannelBitmap = 0;
    sChannelBusyRatio = MAX_RATIO + 1;

    result = state_in_measure_channel(1);       // �`�����l���P���珇�Ƀ`�F�b�N

    if (result == ERRCODE_NOMORE_CHANNEL)
    {
        // �g�p�ł���`�����l����1���Ȃ�
        callback_ready(MBM_MEASURE_ERROR_NO_ALLOWED_CHANNEL);
        return;
    }

    if (result == ERRCODE_API_ERR)
    {
        // �G���[�I��
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_API);
        return;
    }

    // �d�g�g�p�������J�n
    change_mbm_state(MBM_STATE_MEASURING);
}


/*---------------------------------------------------------------------------*
  Name:         state_in_measure_channel

  Arguments:    channel     �������J�n����`�����l���ԍ�

  Returns:      ERRCODE_SUCCESS        - ������
                ERRCODE_NOMORE_CHANNEL - ������������`�����l�����Ȃ�
                ERRCODE_API_ERR        - WM_MeasureChannel��API�Ăяo�����s
 *---------------------------------------------------------------------------*/
static u16 state_in_measure_channel(u16 channel)
{
    u16     allowedCannel;

    allowedCannel = WM_GetAllowedChannel();

    while (((1 << (channel - 1)) & allowedCannel) == 0)
    {
        channel++;
        if (channel > 16)
        {
            /* �����ꂽ�`�����l�������ׂĒ��׏I������ꍇ */
            return ERRCODE_NOMORE_CHANNEL;
        }
    }

    if (wmi_measure_channel(wm_callback, channel) != WM_ERRCODE_OPERATING)
    {
        return ERRCODE_API_ERR;
    }
    return ERRCODE_SUCCESS;
}


static void state_out_measure_channel(void *arg)
{
    u16     result;
    u16     channel;
    WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_CALLBACK);
        return;
    }

    channel = cb->channel;
    PRINTF("CH%d = %d\n", cb->channel, cb->ccaBusyRatio);

    if (cb->ccaBusyRatio < sChannelBusyRatio)
    {
        sChannelBusyRatio = cb->ccaBusyRatio;
        sChannelBitmap = (u16)(1 << (channel - 1));
    }
    else if (cb->ccaBusyRatio == sChannelBusyRatio)
    {
        sChannelBitmap |= (u16)(1 << (channel - 1));
    }

    result = state_in_measure_channel(++channel);

    if (result == ERRCODE_NOMORE_CHANNEL)
    {
        // �`�����l�������I��
        callback_ready(select_channel(sChannelBitmap));
        return;
    }

    if (result == ERRCODE_API_ERR)
    {
        // �G���[�I��
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_API);
        return;
    }

    // ERRCODE_SUCCESS�̏ꍇ�͉������Ȃ�

}


/* ----------------------------------------------------------------------
  �R�[���o�b�N�O�ɖ�����OFF�ɂ���B
  ---------------------------------------------------------------------- */
static void callback_ready(s16 result)
{
    sChannel = result;
    if (sUseInIdle)
    {
        change_mbm_state(MBM_STATE_END);
        user_callback(result);
    }
    else
    {
        state_in_wm_end();
        change_mbm_state(MBM_STATE_END_MEASURE);
    }
}

/* ----------------------------------------------------------------------
  WM�I��
  ---------------------------------------------------------------------- */
static void state_in_wm_end(void)
{
    if (WM_End(wm_callback) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("fail WM_End");
    }
}


/* ----------------------------------------------------------------------
  MBM������ԕύX
  ---------------------------------------------------------------------- */
static void change_mbm_state(u16 state)
{
#ifdef MBM_DEBUG
    static const char *STATE_STR[] = {
        "INIT",
        "MEASURING",                   // �J�n���
        "END_MEASURE",                 // �����I��
        "END",                         // ����OFF���
        "ERR"                          // �G���[���
    };
#endif

    PRINTF("%s -> ", STATE_STR[mbm_measure_state]);
    mbm_measure_state = state;
    PRINTF("%s\n", STATE_STR[mbm_measure_state]);
}

/* ----------------------------------------------------------------------
  �`�����l�������肷��
  ---------------------------------------------------------------------- */
static s16 select_channel(u16 bitmap)
{
    s16     i;
    s16     channel = 0;
    u16     num = 0;
    u16     select;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & (1 << i))
        {
            channel = (s16)(i + 1);
            num++;
        }
    }

    if (num <= 1)
    {
        return channel;
    }

    // �����d�g�g�p���̃`�����l�����������݂����ꍇ
    select = (u16)(((RAND() & 0xFF) * num) / 0x100);

    channel = 1;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & 1)
        {
            if (select == 0)
            {
                return (s16)(i + 1);
            }
            select--;
        }
        bitmap >>= 1;
    }

    return 0;
}

/* ----------------------------------------------------------------------
  ���[�U�[�R�[���o�b�N���Ăяo��
  ---------------------------------------------------------------------- */
static void user_callback(s16 type)
{
    MBMCallback arg;

    if (!sUserCallbackFunc)
    {
        return;
    }

    if (type > 0)
    {
        arg.errcode = MBM_MEASURE_SUCCESS;
        arg.channel = (u16)type;
    }
    else
    {
        arg.errcode = type;
        arg.channel = 0;
    }
    sUserCallbackFunc(&arg);
}
