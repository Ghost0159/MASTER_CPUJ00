/*---------------------------------------------------------------------------*
  Project:  NitroSDK - build - libraries - ctrdg
  File:     ctrdg_vib.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

/*-----------------------------------------------------------------------*
                    Type, Constant
 *-----------------------------------------------------------------------*/
#ifdef SDK_FINALROM
#define VIBi_FatalError(...) (void)0;
#else
#define VIBi_FatalError(...) OS_Panic(__VA_ARGS__)
#endif

#define VIBi_INTR_DELAY_TICK    (19)

#define VIBi_BITID              (2)

typedef struct
{
    u32     current_pos;                /* �p���X�Z�b�g���̌��݈ʒu */
    u32     rest_pos;                   /* �x�~���ԂɂȂ�ʒu */
    u32     rest_tick;                  /* �x�~���Ԃ̒����B1 = 1 Tick */
    u32     on_tick[VIB_PULSE_NUM_MAX]; /* �N�����Ԃ̒����B1 = 1 Tick */
    u32     off_tick[VIB_PULSE_NUM_MAX];/* ��~���Ԃ̒����B1 = 1 Tick */
    BOOL    is_enable;                  /* �U�����Ă���Ƃ�TRUE�ɂȂ� */
    u32     repeat_num;                 /* �p���X�Z�b�g���J��Ԃ����B0�̂Ƃ��́A�I���Ȃ��J��Ԃ��܂��B */
    u32     current_count;              /* ����p���X�Z�b�g���J��Ԃ������������܂��B */
    VIBCartridgePulloutCallback cartridge_pullout_callback;     /* �J�[�g���b�W�����̃R�[���o�b�N */
}
VIBiPulseInfo;

/*-----------------------------------------------------------------------*
                    Function prototype
 *-----------------------------------------------------------------------*/

//--- Auto Function Prototype --- Don't comment here.
BOOL    VIB_Init(void);
void    VIB_End(void);
void    VIB_StartPulse(const VIBPulseState * state);
void    VIB_StopPulse(void);
BOOL    VIB_IsExecuting(void);
void    VIB_SetCartridgePulloutCallback(VIBCartridgePulloutCallback func);
BOOL    VIB_IsCartridgeEnabled(void);
static inline u32 VIBi_PulseTimeToTicks(u32 pulse_time);
static BOOL VIBi_PulledOutCallbackCartridge(void);
static void VIBi_MotorOnOff(VIBiPulseInfo * pulse_vib);
static void VIBi_SleepCallback(void *);
//--- End of Auto Function Prototype

/*-----------------------------------------------------------------------*
                    Variable
 *-----------------------------------------------------------------------*/

/* �L���b�V����32�o�C�g�P�ʂŃA�N�Z�X����̂Ő擪�𑵂��� */
static VIBiPulseInfo pulse_vib ATTRIBUTE_ALIGN(32);
static PMSleepCallbackInfo sc_info;

/*-----------------------------------------------------------------------*
                    Global Function Definition
 *-----------------------------------------------------------------------*/

/*!
    �p���X�U�������������܂��B\n
    ��d�Ăт��s�����ꍇ�́AVIB_IsCartridgeEnabled �֐��Ɠ����ɂȂ�܂��B
    
    ���̊֐����ŁANitroSDK�� PM_AppendPreSleepCallback �֐���p���āA�X���[�v�ɓ���O��
    �U�����~�߂�R�[���o�b�N���o�^����܂��B
    
    @retval TRUE    ���������������܂����B
    @retval FALSE   �����������s���܂����B
*/
BOOL VIB_Init(void)
{

    static BOOL is_initialized;

    if (is_initialized)
    {
        return VIB_IsCartridgeEnabled();
    }
    is_initialized = TRUE;

    if (CTRDGi_IsBitIDAtInit(VIBi_BITID))
    {
        MI_CpuClearFast(&pulse_vib, sizeof(pulse_vib));
        CTRDG_SetPulledOutCallback(VIBi_PulledOutCallbackCartridge);

        /* �X���[�v�ɓ���O�̃R�[���o�b�N��o�^���� */
        PM_SetSleepCallbackInfo(&sc_info, VIBi_SleepCallback, NULL);
        PM_AppendPreSleepCallback(&sc_info);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*!
    �U���J�[�g���b�W���C�u�����̎g�p���~�߂܂��B
    
    �p���X�U���̒�~�A
    VIB_Init �֐��œo�^�����X���[�v�O�̃R�[���o�b�N�̍폜�A
    �J�[�g���b�W�����R�[���o�b�N�̍폜�A
    ���s���Ă��܂��B
*/
void VIB_End(void)
{

    VIB_StopPulse();
    PM_DeletePreSleepCallback(&sc_info);
    CTRDG_SetPulledOutCallback(NULL);
}

/*!
    �p���X�U�����J�n���܂��B\n
    �����ȑO�̃p���X�U�����I�����Ă��Ȃ��ꍇ�́A��������I�����Ă���n�߂܂��B\n
    �X�e�[�^�X�̓��C�u�������ŃR�s�[���܂��̂ŁA���������m�ۂ��Ă����K�v�͂���܂���B
    
    �U���J�n�O�Ƀn�[�h�E�F�A�����̃`�F�b�N���s���܂��B
    �����ɔ����Ă����ꍇ�ADEBUG����RELEASE�r���h�ł́AOS_Panic �֐��Ń��b�Z�[�W��\�����A
    �v���O�������~���܂��BFINALROM�r���h�ł́A�p���X�U�����J�n����܂���B
    
    @sa VIBPulseState
    
    @param state    �p���X�U���̃X�e�[�^�X
*/
void VIB_StartPulse(const VIBPulseState * state)
{

    if (!VIB_IsCartridgeEnabled())
        return;
    
    VIB_StopPulse();
    {
        int     i;

        /* ON���Ԃ̃`�F�b�N */
        for (i = 0; i < state->pulse_num; i++)
        {
            /* ON���Ԃ�0�łȂ����ǂ��� */
            if (state->on_time[i] == 0)
            {
                VIBi_FatalError("pulse_vib: on_time[%d] must not be 0.\n", i);
                return;
            }
            /* ON���Ԃ� VIB_ON_TIME_MAX �𒴂��Ă��Ȃ����ǂ��� */
            if (state->on_time[i] > VIB_ON_TIME_MAX)
            {
                VIBi_FatalError("pulse_vib: on_time[%d] is over VIB_ON_TIME_MAX.\n", i);
                return;
            }
        }
        
        /* OFF���Ԃ̃`�F�b�N */
        for (i = 0; i < state->pulse_num - 1; i++)
        {
            /* OFF���Ԃ�0�łȂ����ǂ��� */
            if (state->off_time[i] == 0)
            {
                VIBi_FatalError("pulse_vib: off_time[%d] must not be 0.\n", i);
                return;
            }
            /* OFF���Ԃ����O��ON���Ԃ𒴂��Ă��Ȃ��� */
            if (state->on_time[i] > state->off_time[i])
            {
                VIBi_FatalError("pulse_vib: on_time[%d] is over off_time[%d].\n", i, i);
                return;
            }
        }
        /* REST���Ԃ� VIB_REST_TIME_MIN �����ɂȂ��Ă��Ȃ��� */
        if (state->rest_time < VIB_REST_TIME_MIN)
        {
            VIBi_FatalError("pulse_vib: rest_time is less than VIB_REST_TIME_MIN.\n", i);
            return;
        }
    }

    pulse_vib.rest_tick = VIBi_PulseTimeToTicks(state->rest_time) - VIBi_INTR_DELAY_TICK;
    pulse_vib.repeat_num = state->repeat_num;
    pulse_vib.current_count = 0;

    pulse_vib.current_pos = 0;

    {
        u32     i;

        for (i = 0; i < VIB_PULSE_NUM_MAX; i++)
        {
            pulse_vib.on_tick[i] = VIBi_PulseTimeToTicks(state->on_time[i]) - VIBi_INTR_DELAY_TICK;
            pulse_vib.off_tick[i] =
                VIBi_PulseTimeToTicks(state->off_time[i]) - VIBi_INTR_DELAY_TICK;
        }
    }
    pulse_vib.rest_pos = state->pulse_num * 2 - 1;

    pulse_vib.is_enable = TRUE;
    /* pulse_vib�\���̂̃|�C���^�𑗂� */
    VIBi_MotorOnOff(&pulse_vib);
}

/*!
    �p���X�U�����~���܂��B
*/
void VIB_StopPulse(void)
{

    if (pulse_vib.is_enable)
    {

        pulse_vib.is_enable = FALSE;
        /* pulse_vib�\���̂̃|�C���^�𑗂� */
        VIBi_MotorOnOff(&pulse_vib);
    }
}

/*!
    �p���X�U�����I���������ۂ���Ԃ��܂��B�Ō��rest_time���I��������_�ŏI�������ƌ��Ȃ���܂��B
    
    @retval TRUE    �p���X�U���͏I�����Ă��܂���B
    @retval FALSE   �p���X�U���͏I�����Ă��܂��B
*/
BOOL VIB_IsExecuting(void)
{

    return pulse_vib.is_enable;
}

/*!
    �J�[�g���b�W�����R�[���o�b�N��o�^���܂��B
    
    �J�[�g���b�W�������N�������ꍇ�A���C�u�����͒����Ƀp���X�U�����~���܂��B@n
    ���̊֐���p���ăR�[���o�b�N���o�^����Ă����ꍇ�́A���̌�ɃR�[���o�b�N��
    �Ă΂�܂��B
    
    @param func �J�[�g���b�W�����R�[���o�b�N
*/
void VIB_SetCartridgePulloutCallback(VIBCartridgePulloutCallback func)
{

    pulse_vib.cartridge_pullout_callback = func;
}

/*!
    �U���J�[�g���b�W���L���ȏ�Ԃ��ǂ�����Ԃ��܂��B
    �i��x�����������s�����ꍇ�́ATRUE��Ԃ��܂���B�j
    
    @retval TRUE    �U���J�[�g���b�W���L���ȏ�Ԃł���
    @retval FALSE   �U���J�[�g���b�W���L���ȏ�Ԃł͂Ȃ��B
*/
BOOL VIB_IsCartridgeEnabled(void)
{

    return CTRDG_IsBitID(VIBi_BITID);
}

/*-----------------------------------------------------------------------*
                    Local Function Definition
 *-----------------------------------------------------------------------*/

static inline u32 VIBi_PulseTimeToTicks(u32 pulse_time)
{

    return ((OS_SYSTEM_CLOCK / 1000) * (pulse_time)) / 64 / 10;
}

static BOOL VIBi_PulledOutCallbackCartridge(void)
{

    VIB_StopPulse();
    if (pulse_vib.cartridge_pullout_callback)
    {
        pulse_vib.cartridge_pullout_callback();
    }

    return FALSE;                      /* �����Ƀ\�t�g���~���Ȃ� */
}

/*!
    Turn on and off vibration.
    
    pulse_vib->is_enable
        @li TRUE     Turn on vibration.
        @li FALSE    Turn off vibration.
*/
static void VIBi_MotorOnOff(VIBiPulseInfo * pulse_vib)
{
    /* �ݒ肵�����L���������t���b�V�� */
    DC_FlushRange(pulse_vib, sizeof(VIBiPulseInfo));
    if (pulse_vib->is_enable == TRUE)
    {
        CTRDG_SendToARM7(pulse_vib);
    }
    else
    {
        CTRDG_SendToARM7(NULL);
    }
}

static void VIBi_SleepCallback(void *)
{

    VIB_StopPulse();
}
