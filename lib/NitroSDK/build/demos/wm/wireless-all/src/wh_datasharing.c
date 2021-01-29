/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_datasharing.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_datasharing.c,v $
  Revision 1.3  2006/11/15 08:21:40  kitase_hirotake
  �Đڑ����Ƀ��g���C����悤�ɕύX

  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS �̃R�[���o�b�N�֐�.
                ��Ԃ� WFS_STATE_READY �ɂȂ������_�ŌĂяo����܂�.
                ���̃R�[���o�b�N�Œʒm���󂯂�, �P�ɔC�ӂ̈ʒu��
                WFS_GetStatus() ���g�p���Ĕ��肷�邱�Ƃ��\�ł�.

  Arguments:    arg       �R�[���o�b�N�Ɏw�肵�����[�U��`����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StateCallbackForWFS(void *arg)
{
    (void)arg;
    switch (WFS_GetStatus())
    {
    case WFS_STATE_READY:
        OS_TPrintf("WFS ready.\n");
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         AllocatorForWFS

  Description:  WFS �Ɏw�肷�郁�����̓��I�m�ۊ֐�.

  Arguments:    arg       �A���P�[�^�Ɏw�肵�����[�U��`����.
                size      �������m�ۗv���Ȃ�ΕK�v�ȃT�C�Y.
                ptr       NULL �̏ꍇ�̓������m��, �����łȂ���Ή���v��.

  Returns:      ptr �� NULL �Ȃ� size ���̃��������m�ۂ��ă|�C���^��Ԃ�����.
                �����łȂ���� ptr �̃�������������邱��.
                ����̏ꍇ, �Ԃ�l�͒P�ɖ��������.
 *---------------------------------------------------------------------------*/
static void *AllocatorForWFS(void *arg, u32 size, void *ptr)
{
    (void)arg;
    if (!ptr)
        return OS_Alloc(size);
    else
    {
        OS_Free(ptr);
        return NULL;
    }
}

/*---------------------------------------------------------------------------*
  Name:         JudgeConnectableChild

  Description:  �Đڑ����ɐڑ��\�Ȏq�@���ǂ����𔻒肷��֐�

  Arguments:    cb      �ڑ����Ă����q�@�̏��.

  Returns:      �ڑ����󂯕t����ꍇ�� TRUE.
                �󂯕t���Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
static BOOL JudgeConnectableChild(WMStartParentCallback *cb)
{
    /*  cb->aid �̎q�@�̃}���`�u�[�g����aid��MAC�A�h���X���猟�����܂� */
    u16     playerNo = MBP_GetPlayerNo(cb->macAddress);
    OS_TPrintf("MB child(%d) -> DS child(%d)\n", playerNo, cb->aid);
    return (playerNo != 0);
}

/*---------------------------------------------------------------------------*
  Name:         StartWirelessParent

  Description:  �e�@�̃��C�����X�ʐM�J�n����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StartWirelessParent(void)
{
    /* WFS ����������ѐڑ������J�n */
    WFS_InitParent(PORT_WFS, StateCallbackForWFS, AllocatorForWFS,
                   NULL, WH_PARENT_MAX_SIZE, NULL, TRUE);
    WH_SetJudgeAcceptFunc(JudgeConnectableChild);
    (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, WM_GetNextTgid(), GetCurrentChannel());
}

/*---------------------------------------------------------------------------*
  Name:         StartWirelessChild

  Description:  �q�@�̃��C�����X�ʐM�J�n����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StartWirelessChild(void)
{
    for (;;)
    {
        /* WFS ������, �e�@��������ѐڑ������J�n */
        WaitWHState(WH_SYSSTATE_IDLE);
        WFS_InitChild(PORT_WFS, StateCallbackForWFS, AllocatorForWFS, NULL);
        WH_SetGgid(WH_GGID);
        (void)WH_ChildConnectAuto(WH_CONNECTMODE_DS_CHILD,
                                  (const u8 *)MB_GetMultiBootParentBssDesc()->bssid, 0);
        while ((WH_GetSystemState() == WH_SYSSTATE_BUSY) ||
               (WH_GetSystemState() == WH_SYSSTATE_SCANNING))
        {
            (void)WaitNextFrame();
            PrintString(9, 11, 0xf, "Now working...");
            if (IS_PAD_TRIGGER(PAD_BUTTON_START))
            {
                (void)WH_Finalize();
            }
        }
        /* �ڑ����s�Ȃ�Ď��s */
        if (WH_GetSystemState() == WH_SYSSTATE_CONNECT_FAIL)
        {
            WH_Reset();
            WaitWHState(WH_SYSSTATE_IDLE);
        }
        /* �\�����ʃG���[�Ȃ炱���ŏI�� */
        else if (WH_GetSystemState() == WH_SYSSTATE_ERROR)
        {
            for (;;)
            {
                static int over_max_entry_count = 0;
                
                (void)WaitNextFrame();
                
                if (WH_GetLastError() == WM_ERRCODE_OVER_MAX_ENTRY)
                {
                    // �e�@�ւ̍Đڑ��̃^�C�~���O�ŐV���ȃ_�E�����[�h�q�@���e�@��
                    // �ڑ����Ă��܂����ꍇ���l�����āA���x���ڑ��̃��g���C������B
                    // ���g���C�Ɏ��s����悤�Ȃ�΁A�{���� OVER_MAX_ENTRY.
                    if (over_max_entry_count < 10)
                    {
                        WH_Reset();
                        
                        over_max_entry_count++;
                        
                        break;
                    }
                    else
                    {
                        PrintString(5, 13, 0xf, "OVER_MAX_ENTRY");
                    }
                }
                PrintString(5, 10, 0x1, "======= ERROR! =======");
            }
        }
        else
        {
            break;
        }
    }

}
