/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.3  2005/11/21 10:23:49  kitase_hirotake
  OS_InitPrintServer �̍폜

  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:45  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

/* ���ɖ����I�Ɏg�p���Ă��郉�C�u���� */
#include <nitro/os.h>
#include <nitro/fs.h>
#include <nitro/pad.h>
#include <nitro/spi.h>


/* ��ʕ`��p�̃��[�`�� */
#include "draw.h"

/* �e���[�h�̃C���^�t�F�[�X */
#include "mode.h"


/*****************************************************************************/
/* constant */

/* �����T���v�����O�p�萔 */
#define     SAMPLING_FREQUENCE      4  // Touch panel sampling frequence per frame
#define     SAMPLING_BUFSIZE        (SAMPLING_FREQUENCE + 1)    // AutoSampling buffer size
#define     SAMPLING_START_VCOUNT   0  // base vcount value in auto sampling.


/*****************************************************************************/
/* variable */

/* �e���[�h�̃C���^�t�F�[�X */
BOOL    (*UpdateFrame) (int frame_count, const InputData * input, int player_count,
                        int own_player_id);
void    (*DrawFrame) (int frame);
void    (*EndFrame) (FSOverlayID *p_next_mode);

/* �^�b�`�p�l���p�����T���v�����O�o�b�t�@ */
static TPData tp_auto_buf[SAMPLING_BUFSIZE];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �f�t�H���g��V�u�����N����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitApp

  Description:  �A�v���P�[�V������{�����̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitApp(void)
{
    /* OS �̏����� */
    OS_Init();
    OS_InitThread();
    OS_InitTick();
    /* �A���[�i�̏����� */
    {
        OSHeapHandle hh;
        void   *tmp = OS_InitAlloc(OS_ARENA_MAIN,
                                   OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
        OS_SetArenaLo(OS_ARENA_MAIN, tmp);
        hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
        if (hh < 0)
            OS_Panic("ARM9: Fail to create heap...\n");
        hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    }
    /* ���荞�݂�L���� */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    /* �^�b�`�p�l���̏����� */
    {
        TPCalibrateParam calibrate;
        TP_Init();
        if (TP_GetUserInfo(&calibrate))
            TP_SetCalibrateParam(&calibrate);
        (void)TP_RequestAutoSamplingStart(SAMPLING_START_VCOUNT, SAMPLING_FREQUENCE, tp_auto_buf,
                                          SAMPLING_BUFSIZE);
    }
    /* �t�@�C���V�X�e���̏����� */
    FS_Init(FS_DMA_NOT_USE);
}

/*---------------------------------------------------------------------------*
  Name:         GetInput

  Description:  ���݂̓��͏�Ԃ��擾���܂�.

  Arguments:    input             �擾�������̊i�[��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetInput(InputData * input)
{
    if (input)
    {
        const u16 hold_bak = input->hold_bits;
        u16     hold_bits;
        /* �^�b�`�p�l�������X�V */
        const TPData *const cur_tp = tp_auto_buf + TP_GetLatestIndexInAuto();
        if (cur_tp->touch == TP_TOUCH_OFF)
            input->tp.touch = TP_TOUCH_OFF;
        else if (cur_tp->validity == TP_VALIDITY_VALID)
            TP_GetCalibratedPoint(&input->tp, cur_tp);
        /* �p�b�h���(����у^�b�`�y���ڐG�r�b�g)���X�V */
        hold_bits = (u16)(PAD_Read() | (input->tp.touch ? PAD_BUTTON_TP : 0));
        input->push_bits = (u16)(~hold_bak & hold_bits);
        input->hold_bits = hold_bits;
        input->release_bits = (u16)(hold_bak & ~hold_bits);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �A�v���P�[�V�����̃��C���G���g���|�C���g.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* ��ʑJ�ڂ̋��ʕϐ� */
    enum
    { PLAYER_MAX = 1 };                /* �v���[���[�ő吔 (�ʐM�ΐ퓙���l��) */
    int     player_count;              /* �v���[���[���� */
    int     own_player_id;             /* ���g�̃v���[���[�ԍ� */
    InputData input[PLAYER_MAX];       /* ���͏��z�� */
    FSOverlayID cur_mode;              /* �O��̃��[�h */
    FSOverlayID prev_mode;             /* ���݂̃��[�h */
    FS_EXTERN_OVERLAY(top_menu);       /* �������[�h (�g�b�v���j���[) */

    /*
     * �A�v���P�[�V�����t���[�����[�N�Ɖ�ʑJ�ڏ�Ԃ����������܂�.
     */
    {
        InitApp();
        InitDraw();
        SetClearColor(RGB555_CLEAR);
        cur_mode = prev_mode = FS_OVERLAY_ID(top_menu);
        UpdateFrame = NULL;
        DrawFrame = NULL;
        EndFrame = NULL;
        MI_CpuClear8(input, sizeof(input));
        player_count = 1;
        own_player_id = 0;
    }

    /*
     * �I�[�o�[���C�Ƃ��Ċe���[�h���\����, �������J��Ԃ��J�ڂ��܂�.
     */
    for (;;)
    {

        /*
         * ���݂̃��[�h���J�n���܂�.
         * ���[�h�����ƂƂ��� NitroStaticInit() �Ŏ�������������
         * �K�v�ȃC���^�t�F�[�X�����Ȑݒ肳��܂�.
         * ���[�h�̏I��������, ���݂̃��[�h���g�� UpdateFrame �ŕԂ��܂�.
         */
        {
            int     frame = 0;
            if (!FS_LoadOverlay(MI_PROCESSOR_ARM9, cur_mode))
            {
                OS_TPanic("failed to load specified mode(%08X)", cur_mode);
            }
            GetInput(&input[own_player_id]);
            for (;;)
            {
                GetInput(&input[own_player_id]);
                if (!UpdateFrame(frame, input, player_count, own_player_id))
                    break;
                DrawBegin();
                DrawFrame(frame);
                DrawEnd();
                ++frame;
            }
        }

        /*
         * ���݂̃��[�h���I����, ���̃��[�h�ֈڍs���܂�.
         * ���݂̃��[�h���ڍs��𖾎��I�ɐݒ肵�Ȃ������ꍇ,
         * �O��̃��[�h�֖߂邱�ƂɂȂ�܂�.
         */
        {
            FSOverlayID next_mode = prev_mode;
            EndFrame(&next_mode);
            if (!FS_UnloadOverlay(MI_PROCESSOR_ARM9, cur_mode))
            {
                OS_TPanic("failed to unload specified mode(%08X)", cur_mode);
            }
            prev_mode = cur_mode;
            cur_mode = next_mode;
        }

    }

}
