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
  再接続時にリトライするように変更

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

  Description:  WFS のコールバック関数.
                状態が WFS_STATE_READY になった時点で呼び出されます.
                このコールバックで通知を受けず, 単に任意の位置で
                WFS_GetStatus() を使用して判定することも可能です.

  Arguments:    arg       コールバックに指定したユーザ定義引数.

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

  Description:  WFS に指定するメモリの動的確保関数.

  Arguments:    arg       アロケータに指定したユーザ定義引数.
                size      メモリ確保要求ならば必要なサイズ.
                ptr       NULL の場合はメモリ確保, そうでなければ解放要求.

  Returns:      ptr が NULL なら size 分のメモリを確保してポインタを返すこと.
                そうでなければ ptr のメモリを解放すること.
                解放の場合, 返り値は単に無視される.
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

  Description:  再接続時に接続可能な子機かどうかを判定する関数

  Arguments:    cb      接続してきた子機の情報.

  Returns:      接続を受け付ける場合は TRUE.
                受け付けない場合は FALSE.
 *---------------------------------------------------------------------------*/
static BOOL JudgeConnectableChild(WMStartParentCallback *cb)
{
    /*  cb->aid の子機のマルチブート時のaidをMACアドレスから検索します */
    u16     playerNo = MBP_GetPlayerNo(cb->macAddress);
    OS_TPrintf("MB child(%d) -> DS child(%d)\n", playerNo, cb->aid);
    return (playerNo != 0);
}

/*---------------------------------------------------------------------------*
  Name:         StartWirelessParent

  Description:  親機のワイヤレス通信開始処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StartWirelessParent(void)
{
    /* WFS 初期化および接続処理開始 */
    WFS_InitParent(PORT_WFS, StateCallbackForWFS, AllocatorForWFS,
                   NULL, WH_PARENT_MAX_SIZE, NULL, TRUE);
    WH_SetJudgeAcceptFunc(JudgeConnectableChild);
    (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, WM_GetNextTgid(), GetCurrentChannel());
}

/*---------------------------------------------------------------------------*
  Name:         StartWirelessChild

  Description:  子機のワイヤレス通信開始処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void StartWirelessChild(void)
{
    for (;;)
    {
        /* WFS 初期化, 親機検索および接続処理開始 */
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
        /* 接続失敗なら再試行 */
        if (WH_GetSystemState() == WH_SYSSTATE_CONNECT_FAIL)
        {
            WH_Reset();
            WaitWHState(WH_SYSSTATE_IDLE);
        }
        /* 予期せぬエラーならここで終了 */
        else if (WH_GetSystemState() == WH_SYSSTATE_ERROR)
        {
            for (;;)
            {
                static int over_max_entry_count = 0;
                
                (void)WaitNextFrame();
                
                if (WH_GetLastError() == WM_ERRCODE_OVER_MAX_ENTRY)
                {
                    // 親機への再接続のタイミングで新たなダウンロード子機が親機へ
                    // 接続してしまった場合を考慮して、何度か接続のリトライをする。
                    // リトライに失敗するようならば、本当の OVER_MAX_ENTRY.
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
