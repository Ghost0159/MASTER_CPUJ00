/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/21 11:08:27  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.5  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 12:08:32  seiki_masashi
  warning �΍�

  Revision 1.3  2005/02/16 10:39:42  terui
  wvr��include�w�b�_�\�L���ȗ����B

  Revision 1.2  2005/02/09 09:23:21  terui
  WVR_StartUp�Ăяo������VRAM��ID�𖾎��I�Ɏw�肷��悤�ɏC���B

  Revision 1.1  2005/01/14 06:25:50  terui
  tests�ȉ�����demos�ȉ��Ɉړ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"

#include <nitro/wvr.h>

//============================================================================
//  �v���g�^�C�v�錾
//============================================================================

static void ModeConnect(void);         // �e�@�ւ̐ڑ��J�n
static void ModeError(void);           // �G���[�\�����
static void ModeWorking(void);         // �r�W�[���
static void ChildReceiveCallback(WMmpRecvBuf *data);

static void StartUpCallback(void *arg, WVRResult result);


//============================================================================
//  �ϐ���`
//============================================================================

static u32 gFrame;                     // �t���[���J�E���^

static WMBssDesc gMBParentBssDesc ATTRIBUTE_ALIGN(32);

static volatile u8 startCheck;


//============================================================================
//  �֐���`
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �q�@���C�����[�`��

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{

    // ��ʁAOS�̏�����
    CommonInit();

    // �������}���`�u�[�g����N�������q�@�ł��邩�ǂ������`�F�b�N���܂��B
    if (!MB_IsMultiBootChild())
    {
        OS_Panic("not found Multiboot child flag!\n");
    }

    //--------------------------------------------------------------
    // �}���`�u�[�g�ŋN�������ꍇ�A��U���Z�b�g����ʐM���ؒf����܂��B
    // �u�[�g����u�[�g�����e�@��BssDesc��ێ����Ă��邽�߁A���̏����g����
    // �e�@�֍Đڑ����Ă��������B
    // ���̎��ABssDesc����MAC�A�h���X�݂̂����o����MAC�A�h���X�w���
    // �e�̃X�L��������ڑ����s�Ȃ��ꍇ�͓��ɖ�肠��܂��񂪁A�ێ�����Ă���
    // BssDesc���g���Ē��ɐe�@�ɐڑ����s�Ȃ��ꍇ�ɂ́A�ʐM�T�C�Y��]�����[�h��
    // ���炩���ߐe�q�̊Ԃō��킹�Đݒ肵�Ă����K�v������܂��B
    //--------------------------------------------------------------

    /* 
     * �e�@�ƍĐڑ����邽�߂ɐe�@�̏����擾���܂��B                   
     * �ڑ��ɗ��p����WMBssDesc��32�o�C�g�ɃA���C������Ă���K�v������܂��B
     * �e�@��MAC�A�h���X�ōăX�L�������邱�ƂȂ��ɍĐڑ�������ꍇ�́A
     * �e�@/�q�@�̍ő呗�M�T�C�Y�AKS/CS�t���O�͂��炩���ߍ��킹�Ă����Ă��������B
     * �ăX�L�������s�Ȃ��Ă���ڑ�����ꍇ�͂����̒l�͂��ׂ�0�ō\���܂���B
     */
    MB_ReadMultiBootParentBssDesc(&gMBParentBssDesc, WH_PARENT_MAX_SIZE,        // �e�@�ő呗�M�T�C�Y
                                  WH_CHILD_MAX_SIZE,    // �q�@�ő呗�M�T�C�Y
                                  0,   // �L�[�V�F�A�����O�t���O
                                  0);  // �A���]�����[�h�t���O

    // �e���ăX�L�������邱�Ɩ����ɁA�ڑ�����ꍇ�ɂ͐e�@�Ǝq�@��tgid�����킹��
    // �K�v������܂��B
    // �e�@�͍ċN����ɖ��֌W��IPL����̐ڑ������̂������ׂ�tgid��ύX���A
    // �q�@��������ɍ��킹��tgid��ύX����K�v������܂��B
    // ���̃f���ł͐e�@�A�q�@�Ƃ���tgid��1�C���N�������g���Ă��܂��B
    gMBParentBssDesc.gameInfo.tgid++;

    // ��ʏ�����
    DispInit();
    // �q�[�v�̏�����
    InitAllocateSystem();

    // ���荞�ݗL��
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /*================================================================*/
    // �������C�u�����쓮�J�n
    startCheck = 0;
    if (WVR_RESULT_OPERATING != WVR_StartUpAsync(GX_VRAM_ARM7_128_D, StartUpCallback, NULL))
    {
        OS_TPanic("WVR_StartUpAsync failed. \n");
    }
    while (!startCheck)
    {
    }
    /*================================================================*/

    GInitDataShare();

    //********************************
    // ����������
    (void)WH_Initialize();
    //********************************

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_TPrintf("MB child: Simple DataSharing demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    ReadKey();

    // ���C�����[�v
    for (gFrame = 0; TRUE; gFrame++)
    {
        // �L�[���͏��擾
        ReadKey();

        // �X�N���[���N���A
        BgClear();

        // �ʐM��Ԃɂ�菈����U�蕪��
        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_CONNECT_FAIL:
            {
                // WM_StartConnect()�Ɏ��s�����ꍇ�ɂ�WM�����̃X�e�[�g���s���ɂȂ��Ă����
                // ��xWM_Reset��IDLE�X�e�[�g�Ƀ��Z�b�g����K�v������܂��B
                WH_Reset();
            }
            break;
        case WH_SYSSTATE_IDLE:
            {
                static  retry = 0;
                enum
                {
                    MAX_RETRY = 5
                };

                if (retry < MAX_RETRY)
                {
                    ModeConnect();
                    retry++;
                    break;
                }
                // MAX_RETRY�Őe�@�ɐڑ��ł��Ȃ����ERROR�\��
            }
        case WH_SYSSTATE_ERROR:
            ModeError();
            break;
        case WH_SYSSTATE_BUSY:
        case WH_SYSSTATE_SCANNING:
            ModeWorking();
            break;

        case WH_SYSSTATE_CONNECTED:
        case WH_SYSSTATE_KEYSHARING:
        case WH_SYSSTATE_DATASHARING:
            {
                BgPutString(8, 1, 0x2, "Child mode");
                GStepDataShare(gFrame);
                GMain();
            }
            break;
        }

        // �d�g��M���x�̕\��
        {
            int     level;
            level = WH_GetLinkLevel();
            BgPrintStr(31, 23, 0xf, "%d", level);
        }

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         StartUpCallback

  Description:  �����쓮���䃉�C�u�����̔񓯊��I�ȏ����I�����ʒm�����
                �R�[���o�b�N�֐��B

  Arguments:    arg     -   WVR_StartUpAsync�R�[�����Ɏw�肵�������B���g�p�B
                result  -   �񓯊��֐��̏������ʁB

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StartUpCallback(void *arg, WVRResult result)
{
#pragma unused( arg )

    if (result != WVR_RESULT_SUCCESS)
    {
        OS_TPanic("WVR_StartUpAsync error.[%08xh]\n", result);
    }
    startCheck = 1;
}

/*---------------------------------------------------------------------------*
  Name:         ModeConnect

  Description:  �ڑ��J�n

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeConnect(void)
{
#define USE_DIRECT_CONNECT

    // �e�@�̍ăX�L�����Ȃ��ɒ��ڐڑ�����ꍇ�B
#ifdef USE_DIRECT_CONNECT
    //********************************
    (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD, &gMBParentBssDesc);
    // WH_ChildConnect(WH_CONNECTMODE_MP_CHILD, &gMBParentBssDesc, TRUE);
    // WH_ChildConnect(WH_CONNECTMODE_KS_CHILD, &gMBParentBssDesc, TRUE);
    //********************************
#else
    WH_SetGgid(gMBParentBssDesc.gameInfo.ggid);
    // �e�@�̍ăX�L���������s����ꍇ�B
    //********************************
    (void)WH_ChildConnectAuto(WH_CONNECTMODE_DS_CHILD, gMBParentBssDesc.bssid,
                              gMBParentBssDesc.channel);
    // WH_ChildConnect(WH_CONNECTMODE_MP_CHILD, &gMBParentBssDesc, TRUE);
    // WH_ChildConnect(WH_CONNECTMODE_KS_CHILD, &gMBParentBssDesc, TRUE);
    //********************************
#endif
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�ʒm��ʕ\��

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    BgPrintStr(5, 10, 0x1, "======= ERROR! =======");

    if (WH_GetLastError() == WM_ERRCODE_OVER_MAX_ENTRY)
    {
        BgPrintStr(5, 13, 0xf, "OVER_MAX_ENTRY");
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ��������ʂ�\��

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
static void ModeWorking(void)
{
    BgPrintStr(9, 11, 0xf, "Now working...");

    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
    {
        //********************************
        (void)WH_Finalize();
        //********************************
    }
}
