/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - basic - nitro
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      mcsライブラリを使用して、PCとのデータの読み書きを行います。
//      まず最初にBG画面に絵が表示されます。
//      PC側のプログラム basic.exe を起動すると、絵が横方向に移動します。
//      basic.exe を2つ起動すると、絵が斜め方向に移動します。
//
//      A ボタン押下でプログラムを終了します。
//
// ============================================================================

#include "sdk_init.h"
#include "nns_util.h"

#include <nnsys/mcs.h>
#include <nnsys/misc.h>

// Windowsアプリケーションとの識別で使用するチャンネル値です
static const u16 MCS_CHANNEL0 = 0;
static const u16 MCS_CHANNEL1 = 1;

// 位置情報の構造体
typedef struct Vec2 Vec2;
struct Vec2
{
    s32     x;
    s32     y;
};

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Vブランク割り込みハンドラです。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
VBlankIntr()
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    NNS_McsVBlankInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         CartIntrFunc

  Description:  カードリッジ割り込みハンドラです。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
CartIntrFunc()
{
    OS_SetIrqCheckFlag(OS_IE_CARTRIDGE);

    NNS_McsCartridgeInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupt

  Description:  割り込み関連の初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
InitInterrupt()
{
    (void)OS_DisableIrq();

    // Vブランク割り込みを有効にし、VBlank割り込み内で
    // NNS_McsVBlankInterrupt()が呼ばれるようにする
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)GX_VBlankIntr(TRUE);

	// カートリッジ割り込みを有効にし、カートリッジ割り込み内で
    // NNS_McsCartridgeInterrupt()が呼ばれるようにする
    OS_SetIrqFunction(OS_IE_CARTRIDGE, CartIntrFunc);
	(void)OS_EnableIrqMask(OS_IE_CARTRIDGE);

    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         DataRecvCallback

  Description:  PC側からデータを受信したときに呼ばれるコールバック関数です。

                登録するコールバック関数内ではデータの送受信を行わないでください。
                また、割り込みが禁止されている場合があるため、
                割り込み待ちループも行わないでください。

  Arguments:    recv:       受信したデータの一部あるいは全部を格納している
                            バッファへのポインタ。
                recvSize:   recvによって示されるバッファに格納されている
                            データのサイズ。
                userData:   NNS_McsRegisterRecvCallback()の引数userDataで
                            指定した値。
                offset:     受信したデータの全部がrecvによって示されるバッファに
                            格納されている場合は0。
                            受信したデータの一部が格納されている場合は、
                            受信したデータ全体に対する0を基準としたオフセット位置。
                totalSize:  受信したデータの全体のサイズ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
DataRecvCallback(
    const void* pRecv,
    u32         recvSize,
    u32         userData,
    u32         offset,
    u32         /* totalSize */
)
{
    Vec2 *const pPos = (Vec2*)userData;

	// 受信バッファチェック
	if (pRecv != NULL && recvSize == sizeof(pPos->y) && offset == 0)
	{
        // y位置を受信した値で更新
        const s32* pRecvPosY = pRecv;
        pPos->y = *pRecvPosY;
    }
}

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  アーカイブからＢＧデータを読み込み、画面を表示するサンプル
                プログラムです。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
static void
MainLoop(Vec2* pPos)
{
    while (TRUE)
    {
        u32 nLength;
        s32 posX;

        NNS_McsPollingIdle();

        // 受信可能なデータサイズの取得
		nLength = NNS_McsGetStreamReadableSize(MCS_CHANNEL0);
        if (nLength > 0 && sizeof(posX) == nLength)
        {
            u32 readSize;

            // データの読み取り
            if ( NNS_McsReadStream(MCS_CHANNEL0, &posX, sizeof(posX), &readSize)
              && readSize == sizeof(posX)
            )
            {
                pPos->x = posX;
            }
        }

        // 一定範囲から外れたら、増分値を反転するようにPC側に伝える
        if (pPos->x < 0 || 64 < pPos->x)
        {
            s32 writeData = pPos->x < 0 ? 1: -1;
            // データの書き込み
            if (! NNS_McsWriteStream(MCS_CHANNEL0, &writeData, sizeof(writeData)))
            {
                break;
            }
        }

        // 一定範囲から外れたら、増分値を反転するようにPC側に伝える
        if (pPos->y < 0 || 64 < pPos->y)
        {
            s32 writeData = pPos->y < 0 ? 1: -1;
            // データの書き込み
            if (! NNS_McsWriteStream(MCS_CHANNEL1, &writeData, sizeof(writeData)))
            {
                break;
            }
        }

        SVC_WaitVBlankIntr();

        G2_SetBG0Offset(pPos->x, pPos->y);

        ReadGamePad();
        if (IS_TRIGGER(PAD_BUTTON_A))
        {
            break;
        }
    }
}

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  アーカイブからＢＧデータを読み込み、画面を表示するサンプル
                プログラムです。

  Arguments:    なし。

  Returns:      なし。
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    u8* mcsWorkMem;
    NNSMcsDeviceCaps deviceCaps;
    NNSMcsRecvCBInfo recvCBInfo;
    Vec2* pPos;

    InitSystem();

    InitInterrupt();
    InitMemory();
    
    // mcsの初期化
    mcsWorkMem = NNS_FndAllocFromExpHeapEx(gAppHeap, NNS_MCS_WORKMEM_SIZE, 4); // MCSのワーク用メモリを確保
    NNS_McsInit(mcsWorkMem);

    InitVRAM();

    InitDisplay();
    LoadPicture();

    pPos = NNS_FndAllocFromExpHeapEx(gAppHeap, sizeof(Vec2), 4);  // データ用のメモリを確保

    // データの初期化
    pPos->x = 0;
    pPos->y = 0;

    G2_SetBG0Offset(pPos->x, pPos->y);

    // デバイスのオープン
    if (NNS_McsGetMaxCaps() > 0 && NNS_McsOpen(&deviceCaps))
    {
        void* printBuffer = NNS_FndAllocFromExpHeap(gAppHeap, 1024);        // プリント用のバッファの確保
        void* recvBuf = NNS_FndAllocFromExpHeapEx(gAppHeap, 1024, 4);       // 受信用バッファの確保

        NNS_NULL_ASSERT(printBuffer);
        NNS_NULL_ASSERT(recvBuf);
        NNS_NULL_ASSERT(pPos);

        // OS_Printfによる出力
        OS_Printf("device open\n");

        // mcs文字列出力の初期化
        NNS_McsInitPrint(printBuffer, NNS_FndGetSizeForMBlockExpHeap(printBuffer));

        // NNS_McsPrintfによる出力
        // このタイミングでmcsサーバが接続していれば、コンソールに表示されます。
        (void)NNS_McsPrintf("device ID %08X\n", deviceCaps.deviceID);

        // 読み取り用バッファの登録
        NNS_McsRegisterStreamRecvBuffer(MCS_CHANNEL0, recvBuf, NNS_FndGetSizeForMBlockExpHeap(recvBuf));

        // 受信コールバック関数の登録
        NNS_McsRegisterRecvCallback(&recvCBInfo, MCS_CHANNEL1, DataRecvCallback, (u32)pPos);

        MainLoop(pPos);

        NNS_McsUnregisterRecvResource(MCS_CHANNEL1);
        NNS_McsUnregisterRecvResource(MCS_CHANNEL0);

        NNS_FndFreeToExpHeap(gAppHeap, recvBuf);        // 受信用バッファの確保
        NNS_FndFreeToExpHeap(gAppHeap, printBuffer);    // プリント用のバッファの確保

        // NNS_McsPutStringによる出力
        (void)NNS_McsPutString("device close\n");

        // デバイスをクローズ
        (void)NNS_McsClose();
    }
    else
    {
        OS_Printf("device open fail.\n");
        MainLoop(pPos);
    }

    NNS_FndFreeToExpHeap(gAppHeap, pPos);           // データ用のメモリを解放

    while (TRUE) {}
}

