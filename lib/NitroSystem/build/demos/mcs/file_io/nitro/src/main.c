/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - file_io - nitro
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      mcsライブラリを使用して、PC上のファイルの読み書きと
//      PC上のファイルの列挙を行います。
//      具体的には次のことを行います。
//
//        1. NITRO-Systemルートディレクトリ下のファイル
//             "build\demos\mcs\file_io\nitro\data\test.txt"
//           を読み出し、内容を反転して
//             "build\demos\mcs\file_io\nitro\data\test_wr.txt"
//           という別ファイルとして書き込みます。
//        2. NITRO-Systemルートディレクトリ下の次のディレクトリ
//             "build\demos\mcs\file_io\nitro\data"
//           の下にある拡張子が"txt"のファイルを検索します。
//
// ============================================================================

#include <nitro.h>
#include <extras.h>

#include <nnsys/fnd.h>
#include <nnsys/mcs.h>
#include <nnsys/misc.h>

// このプロジェクトのディレクトリ
#define PROJECT_DIR "%NITROSYSTEM_ROOT%\\build\\demos\\mcs\\file_io\\nitro"

// ファイルデータ管理用構造体
typedef struct FileData FileData;
struct FileData
{
    char*   buf;
    u32     size;
};

static NNSFndHeapHandle gExpHeap;

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Vブランク割り込みハンドラです。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
VBlankIntr(void)
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
CartIntrFunc(void)
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
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);

    // カートリッジ割り込みを有効にし、カートリッジ割り込み内で
    // NNS_McsCartridgeInterrupt()が呼ばれるようにする
    OS_SetIrqFunction(OS_IE_CARTRIDGE, CartIntrFunc);
    (void)OS_EnableIrqMask(OS_IE_CARTRIDGE);

    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileRead

  Description:  ファイルの内容を読み込みます。

  Arguments:    pFileData:  ファイルの情報を保持する構造体へのポインタ。

  Returns:      読み込みに成功すれば TRUE、失敗すれば FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL
SampleFileRead(FileData* pFileData)
{
    void* pDataBuf = 0;
    BOOL bSuccess = FALSE;
    NNSMcsFile info;
    u32 errCode;
    u32 readSize;

    OS_Printf("read test.txt\n");

    // ファイルのオープン
    errCode = NNS_McsOpenFile(
        &info,
        PROJECT_DIR "\\data\\test.txt",
            NNS_MCS_FILEIO_FLAG_READ
          | NNS_MCS_FILEIO_FLAG_INCENVVAR); // PCの環境変数文字列を展開する
    if (errCode)
    {
        OS_Printf("error : NNS_McsOpenFile() test.txt .\n");
        return FALSE;
    }

    // ファイル全体を読み込むためのバッファを確保
    // 全体を文字列とみなすため、終端文字列用の1バイトを余分に確保します。
    pDataBuf = NNS_FndAllocFromExpHeap(gExpHeap, NNS_McsGetFileSize(&info) + 1);
    NNS_NULL_ASSERT(pDataBuf);

    // ファイルの読み込み
    errCode = NNS_McsReadFile(&info, pDataBuf, NNS_McsGetFileSize(&info), &readSize);
    if (0 == errCode)
    {
        pFileData->buf = pDataBuf;
        pFileData->size = readSize;
        pFileData->buf[readSize] = '\0';
        bSuccess = TRUE;
    }
    else
    {
        // 読み込みに失敗したので、メモリを解放
        NNS_FndFreeToExpHeap(gExpHeap, pDataBuf);
    }

    // ファイルのクローズ
    (void)NNS_McsCloseFile(&info);

    return bSuccess;
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileWrite

  Description:  ファイルへ書き込みます。

  Arguments:    pFileData:  ファイルの情報を保持する構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
SampleFileWrite(FileData* pFileData)
{
    NNSMcsFile info;
    u32 errCode;

    OS_Printf("write test_wr.txt\n");

    // ファイルのオープン
    errCode = NNS_McsOpenFile(
        &info,
        PROJECT_DIR "\\data\\test_wr.txt",
            NNS_MCS_FILEIO_FLAG_WRITE       // 書き込みモード
          | NNS_MCS_FILEIO_FLAG_FORCE       // ファイルが存在していたら上書き
          | NNS_MCS_FILEIO_FLAG_INCENVVAR); // PCの環境変数文字列を展開する
    if (errCode)
    {
        OS_Printf("error : NNS_McsOpenFile() test_wr.txt .\n");
        return;
    }

    // ファイルの書き込み
    errCode = NNS_McsWriteFile(&info, pFileData->buf, pFileData->size);
    if (errCode)
    {
        OS_Printf("error : NNS_McsWriteFile().\n");
    }

    // ファイルのクローズ
    (void)NNS_McsCloseFile(&info);
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileIO

  Description:  ファイルからデータを読み取り、
                内容を反転して別のファイルに書き込みます。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
SampleFileIO()
{
    BOOL bSuccess = FALSE;
    FileData fileData;

    // ファイルを読み込む
    bSuccess = SampleFileRead(&fileData);

    if (bSuccess)
    {
        // 内容を反転
        (void)strrev(fileData.buf);

        // ファイルへ書き込む
        SampleFileWrite(&fileData);
        NNS_FndFreeToExpHeap(gExpHeap, fileData.buf);
    }
}

/*---------------------------------------------------------------------------*
  Name:         SampleFind

  Description:  拡張子が txt のファイルを列挙します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void
SampleFind()
{
    NNSMcsFile info;
    NNSMcsFileFindData findData;
    u32 errCode;

    // パターンに一致するファイルを検索
    errCode = NNS_McsFindFirstFile(
        &info,
        &findData,
        PROJECT_DIR "\\data\\*.txt");

    // 一致するファイルが見つからない
    if (errCode == NNS_MCS_FILEIO_ERROR_NOMOREFILES)
    {
        OS_Printf("not match *.txt .\n");
        return;
    }

    // その他のエラー
    if (errCode != 0)
    {
        OS_Printf("error : NNS_McsFindFirstFile()\n");
        return;
    }

    // 次にパターンに一致するファイルを検索。
    // 関数 NNS_McsFindNextFile が0以外を返すまでループします。
    // 一致するファイルがそれ以上ない場合は、
    // NNS_MCS_FILEIO_ERROR_NOMOREFILES が返ります。
    do
    {
        OS_Printf("find filename %s %dbytes.\n", findData.name, findData.size);
        errCode = NNS_McsFindNextFile(&info, &findData);
    }while (0 == errCode);

    if (errCode != NNS_MCS_FILEIO_ERROR_NOMOREFILES)
    {
        OS_Printf("error : NNS_McsFindNextFile()\n");
    }

    (void)NNS_McsCloseFind(&info);
}

void
NitroMain(void)
{
    u8* mcsWorkMem;
    NNSMcsDeviceCaps deviceCaps;
    u8* mcsFileIOWorkMem;

    OS_Init();
    InitInterrupt();

    /*
        メインのアリーナ全てを拡張ヒープに割り当てる
    */
    {
        void *const arenaLo = OS_GetArenaLo(OS_ARENA_MAIN);

        OS_SetArenaLo(OS_ARENA_MAIN, OS_GetArenaHi(OS_ARENA_MAIN));

        gExpHeap = NNS_FndCreateExpHeap(arenaLo, (u32)OS_GetArenaHi(OS_ARENA_MAIN) - (u32)arenaLo);
    }

    // mcsの初期化
    mcsWorkMem = NNS_FndAllocFromExpHeapEx(gExpHeap, NNS_MCS_WORKMEM_SIZE, 4); // MCSのワーク用メモリを確保
    NNS_McsInit(mcsWorkMem);

    if (NNS_McsGetMaxCaps() > 0)
    {
        // デバイスのオープン
        if (NNS_McsOpen(&deviceCaps))
        {
            // DS本体とIS-NITRO-UICで通信する場合は、
            // mcsサーバが接続してない可能性があるので、
            // mcsサーバが接続してくるまで待つ
            if (! NNS_McsIsServerConnect())
            {
                OS_Printf("Waiting server connect.\n");
                do
                {
                    SVC_WaitVBlankIntr();
                }while (! NNS_McsIsServerConnect());
            }

            // ファイル入出力機能の初期化
            mcsFileIOWorkMem = NNS_FndAllocFromExpHeapEx(gExpHeap, NNS_MCS_FILEIO_WORKMEM_SIZE, 4);     // MCS FileI/O のワーク用メモリを確保
            NNS_McsInitFileIO(mcsFileIOWorkMem);

            SampleFileIO();
            SampleFind();

            // デバイスのクローズ
            (void)NNS_McsClose();
        }
        else
        {
            OS_Printf("device open fail.\n");
        }
    }
    else
    {
        OS_Printf("device not found.\n");
    }

    while (TRUE) {}
}

