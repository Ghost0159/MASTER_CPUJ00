/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - mbp
  File:     mbp.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mbp.c,v $
  Revision 1.10  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/22 03:17:54  yosizaki
  fix about MBP_STATE_REBOOTING state.

  Revision 1.8  2005/04/08 05:47:55  yosizaki
  bitmap判定の修正.

  Revision 1.7  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.6  2005/02/18 10:38:30  yosizaki
  fix around hidden warnings.

  Revision 1.5  2004/12/03 08:13:32  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX に修正、 MBP_USING_PREVIOUS_DOWNLOADスイッチを追加

  Revision 1.4  2004/11/24 06:15:12  yosizaki
  rename MB_StartParentEx/MB_EndEx

  Revision 1.3  2004/11/18 07:57:41  takano_makoto
  MB_SetParentCommSize, MB_SetParentCommParamで子機送信サイズの設定引数を廃止

  Revision 1.2  2004/11/12 11:57:35  yosizaki
  add option for cloneboot.

  Revision 1.1  2004/11/12 09:02:39  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "mbp.h"




//============================================================================
//  プロトタイプ宣言
//============================================================================

static BOOL MBP_RegistFile(const MBGameRegistry *gameInfo);
static inline void MBP_AddBitmap(u16 *pBitmap, u16 aid);
static inline void MBP_RemoveBitmap(u16 *pBitmap, u16 aid);
static inline void MBP_DisconnectChild(u16 aid);
static inline void MBP_DisconnectChildFromBmp(u16 aid);
static void ParentStateCallback(u16 child_aid, u32 status, void *arg);
static void MBP_ChangeState(u16 state);


// コールバックはIRQで動作していますのでコールバック内で複雑な処理をする場合には、
// lcfファイルでIRQスタックサイズを少し大きめに設定しておいた方が安全です。
//
// 特にOS_Printf()は大量のスタックを消費するので、
// コールバック内ではできるだけ軽量版のOS_TPrintf()を使用するようにしてください。

#define MBP_DEBUG
#if defined( MBP_DEBUG )
#define MBP_Printf          OS_TPrintf //
#else
#define MBP_Printf(...)     ((void)0)
#endif



//============================================================================
//  変数定義
//============================================================================

// 子機接続情報
static MBPState mbpState;
static MBPChildInfo childInfo[MBP_CHILD_MAX];

/* MB ライブラリへ割り当てるワーク領域 */
static u32 *sCWork = NULL;

/* 子機起動バイナリ転送用バッファ */
static u8 *sFilebuf = NULL;

//============================================================================
//  関数定義
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         MBP_Init

  Description:  マルチブート親機情報を初期化する。

  Arguments:    ggid  ゲーム配信時の親のGGIDを指定.
                tgid  ゲーム配信時の親のTGIDを指定.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Init(u32 ggid, u16 tgid)
{
    /* 子機画面へ表示する親機情報設定用 */
    MBUserInfo myUser;

    /* MB子機画面に表示する親機情報としてどのような値を設定しなければ   *
     * ならないかは厳密には決定していません。                           *
     * ここではIPLで登録されたユーザ情報をMB親機情報として設定しますが、*
     * ゲーム内で設定した名前をセットする場合もあるかもしれません。     *
     * ただし2バイトコードで設定するのを忘れないでください。            */
#if ( SDK_NVRAM_FORMAT >= 100 )
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    myUser.favoriteColor = info.favoriteColor;
    myUser.nameLength = (u8)info.nickNameLength;
    MI_CpuCopy8(info.nickName, myUser.name, (u32)(info.nickNameLength * 2));
#else  // 旧IPL用
    NVRAMConfig *info = (NVRAMConfig *)(OS_GetSystemWork()->nvramUserInfo);

#define DEFAULT_FAVORIT_COLOR    1
    myUser.favoriteColor = DEFAULT_FAVORIT_COLOR;
    myUser.nameLength = (u8)info->ncd.owner.nickname.length;
    MI_CpuCopy8(info->ncd.owner.nickname.name, myUser.name, NVRAM_CONFIG_NICKNAME_LENGTH * 2);
#endif

    myUser.playerNo = 0;               // 親機は0番

    // ステータス情報を初期化
    mbpState = (const MBPState)
    {
    MBP_STATE_STOP, 0, 0, 0, 0, 0, 0};

    /* MB 親機制御を開始します. */
    // MBワーク領域確保。
#if !defined(MBP_USING_MB_EX)
    sCWork = OS_Alloc(MB_SYSTEM_BUF_SIZE);
#else
    /* 外部で WM の初期化を済ませていればワークサイズは少なくしても構いません. */
    sCWork = OS_Alloc(MB_SYSTEM_BUF_SIZE - WM_SYSTEM_BUF_SIZE);
#endif

    if (MB_Init(sCWork, &myUser, ggid, tgid, MBP_DMA_NO) != MB_SUCCESS)
    {
        OS_Panic("ERROR in MB_Init\n");
    }

    // 最大接続子機数を設定(親機の数は抜いた値を設定)
    (void)MB_SetParentCommParam(MB_COMM_PARENT_SEND_MIN, MBP_CHILD_MAX);

    MB_CommSetParentStateCallback(ParentStateCallback);

    MBP_ChangeState(MBP_STATE_IDLE);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_Start

  Description:  マルチブート親機開始

  Arguments:    gameInfo 配信バイナリ情報.
                channel  使用するチャンネル.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Start(const MBGameRegistry *gameInfo, u16 channel)
{
    SDK_ASSERT(MBP_GetState() == MBP_STATE_IDLE);

    MBP_ChangeState(MBP_STATE_ENTRY);
#if !defined(MBP_USING_MB_EX)
    if (MB_StartParent(channel) != MB_SUCCESS)
#else
    /* WM ライブラリを IDLE ステートに移行しておき, 直接開始します. */
    if (MB_StartParentFromIdle(channel) != MB_SUCCESS)
#endif
    {
        MBP_Printf("MB_StartParent fail\n");
        MBP_ChangeState(MBP_STATE_ERROR);
        return;
    }

    /* --------------------------------------------------------------------- *
     * MB_StartParent()コール時に初期化されてしまいます。
     * MB_RegisterFile()は必ずMB_StartParent()の後で登録してください。
     * --------------------------------------------------------------------- */

    /* ダウンロードプログラムファイル情報を登録します. */
    if (!MBP_RegistFile(gameInfo))
    {
        OS_Panic("Illegal multiboot gameInfo\n");
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_RegistFile

  Description:  マルチブートバイナリの登録

  Arguments:    gameInfo マルチブートバイナリ情報へのポインタ.
                         このメンバのうち romFilePathp が NULL の場合,
                         クローンブートを指定されたものとして動作します.

  Returns:      ファイルのオープンが成功したら TRUE.
                失敗したら FALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL MBP_RegistFile(const MBGameRegistry *gameInfo)
{
    FSFile  file, *p_file;
    u32     bufferSize;
    BOOL    ret = FALSE;

    /* --------------------------------------------------------------------- *
     * MB_StartParent()よりも先にMB_RegisterFile()でバイナリを登録しても、
     * MB_StartParent()コール時に初期化されてしまいます。
     * MB_RegisterFile()は必ずMB_StartParent()の後で登録してください。
     * --------------------------------------------------------------------- */

    /*
     * この関数の仕様として, romFilePathp が NULL であれば
     * クローンブートとして動作します.
     * そうでなければ指定されたファイルを子機プログラムとして扱います.
     */
    if (!gameInfo->romFilePathp)
    {
        p_file = NULL;
    }
    else
    {
        /* 
         * プログラムファイルは FS_ReadFile() で読み出せる必要があります.
         * 通常はプログラムを CARD-ROM 内にファイルとして保持するはずなので
         * 問題はありませんが, もし特殊なマルチブートシステムを想定する場合,
         * FSArchive で独自アーカイブを構築して対処してください. 
         */
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, gameInfo->romFilePathp))
        {
            /* ファイルが開けない, */
            OS_Warning("Cannot Register file\n");
            return FALSE;
        }
        p_file = &file;
    }

    /*
     * セグメント情報のサイズを取得.
     * 正当なダウンロードプログラムでない場合,
     * このサイズに 0 が返ってきます.
     */
    bufferSize = MB_GetSegmentLength(p_file);
    if (bufferSize == 0)
    {
        OS_Warning("specified file may be invalid format.\"%s\"\n",
                   gameInfo->romFilePathp ? gameInfo->romFilePathp : "(cloneboot)");
    }
    else
    {
        /*
         * ダウンロードプログラムのセグメント情報を読み込むメモリを確保.
         * ファイルのレジストに成功した場合はMB_End()が呼ばれるまで
         * この領域が使用されます。
         * このメモリはサイズさえ充分であれば 静的に用意されていても構いません.
         */
        sFilebuf = OS_Alloc(bufferSize);
        if (sFilebuf == NULL)
        {
            /* セグメント情報を格納するバッファの確保失敗 */
            OS_Warning("can't allocate Segment buffer size.\n");
        }
        else
        {
            /* 
             * セグメント情報をファイルから抽出.
             * 抽出したセグメント情報は, ダウンロードプログラム配信中
             * メインメモリ上に常駐させておく必要があります.
             */
            if (!MB_ReadSegment(p_file, sFilebuf, bufferSize))
            {
                /* 
                 * 不正なファイルによりセグメント抽出に失敗
                 * サイズ取得が成功したにも関わらずここで抽出処理が失敗するのは,
                 * ファイルハンドルに何か変更を与えた場合などです.
                 * (ファイルを閉じた, 位置をシークした, ...)
                 */
                OS_Warning(" Can't Read Segment\n");
            }
            else
            {
                /*
                 * 抽出したセグメント情報と MBGameRegistry で
                 * ダウンロードプログラムを登録します.
                 */
                if (!MB_RegisterFile(gameInfo, sFilebuf))
                {
                    /* 不正なプログラム情報により登録失敗 */
                    OS_Warning(" Illegal program info\n");
                }
                else
                {
                    /* 処理は正しく完了しました */
                    ret = TRUE;
                }
            }
            if (!ret)
                OS_Free(sFilebuf);
        }
    }

    /* クローンブートでなければファイルをクローズ */
    if (p_file == &file)
    {
        (void)FS_CloseFile(&file);
    }

    return ret;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_AcceptChild

  Description:  子機のリクエストを受け付ける。

  Arguments:    child_aid       接続子機のaid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_AcceptChild(u16 child_aid)
{
    if (!MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_ACCEPT))
    {
        // リクエストに失敗した場合その子機を切断する。
        MBP_DisconnectChild(child_aid);
        return;
    }

    MBP_Printf("accept child %d\n", child_aid);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_KickChild

  Description:  接続子機をキック。

  Arguments:    child_aid       接続子機のaid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_KickChild(u16 child_aid)
{
    if (!MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_KICK))
    {
        // リクエストに失敗した場合その子機を切断する。
        MBP_DisconnectChild(child_aid);
        return;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();

        mbpState.requestChildBmp &= ~(1 << child_aid);
        mbpState.connectChildBmp &= ~(1 << child_aid);

        (void)OS_RestoreInterrupts(enabled);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_StartDownload

  Description:  子機へダウンロード開始信号を送信。

  Arguments:    child_aid       接続子機のaid

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartDownload(u16 child_aid)
{
    if (!MB_CommStartSending(child_aid))
    {
        // リクエストに失敗した場合その子機を切断する。
        MBP_DisconnectChild(child_aid);
        return;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();

        mbpState.entryChildBmp &= ~(1 << child_aid);
        mbpState.downloadChildBmp |= 1 << child_aid;

        (void)OS_RestoreInterrupts(enabled);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBP_StartDownloadAll

  Description:  現在エントリー中の全子機に対してデータ転送を開始する。
                以降、子機のエントリー受付を行なわない。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartDownloadAll(void)
{
    u16     i;

    // エントリー受付終了
    MBP_ChangeState(MBP_STATE_DATASENDING);

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.connectChildBmp & (1 << i)))
        {
            continue;
        }

#ifdef MBP_USING_PREVIOUS_DOWNLOAD
        /* エントリー後即データ送信を開始している場合 */
        // エントリー中でない子機は切断する
        if (MBP_GetChildState(i) == MBP_CHILDSTATE_CONNECTING)
        {
            MBP_DisconnectChild(i);
            continue;
        }
#else
        /* データ送信を全子機一斉に開始する場合 */
        if (mbpState.requestChildBmp & (1 << i))
        {
            // 現在エントリー中の子機は後で準備ができてから処理をする
            continue;
        }

        // エントリー中でない子機は切断する
        if (!(mbpState.entryChildBmp & (1 << i)))
        {
            MBP_DisconnectChild(i);
            continue;
        }

        // エントリー中の子機はダウンロード開始
        MBP_StartDownload(i);
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBP_IsBootableAll

  Description:  接続されている子機すべてがブート可能状態になっているかを
                チェックします。

  Arguments:    None.

  Returns:      全子機がブート可能であれば       TRUE
                ブート不能な子機が一台でもいれば FALSE
 *---------------------------------------------------------------------------*/
BOOL MBP_IsBootableAll(void)
{
    u16     i;

    if (mbpState.connectChildBmp == 0)
    {
        return FALSE;
    }

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.connectChildBmp & (1 << i)))
        {
            continue;
        }

        if (!MB_CommIsBootable(i))
        {
            return FALSE;
        }
    }
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_StartRebootAll

  Description:  ブート可能な接続子機をすべてブートする。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartRebootAll(void)
{
    u16     i;
    u16     sentChild = 0;

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.bootableChildBmp & (1 << i)))
        {
            continue;
        }
        if (!MB_CommBootRequest(i))
        {
            // リクエストに失敗した場合その子機を切断する。
            MBP_DisconnectChild(i);
            continue;
        }
        sentChild |= (1 << i);
    }

    // 接続子機が0になったらエラー終了
    if (sentChild == 0)
    {
        MBP_ChangeState(MBP_STATE_ERROR);
        return;
    }

    // 状態を変更しデータ転送の受付を終了します。
    MBP_ChangeState(MBP_STATE_REBOOTING);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_Cancel

  Description:  マルチブートをキャンセルする。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Cancel(void)
{
    MBP_ChangeState(MBP_STATE_CANCEL);
#if !defined(MBP_USING_MB_EX)
    MB_End();
#else
    MB_EndToIdle();
#endif
}

/*---------------------------------------------------------------------------*
  Name:         MBPi_CheckAllReboot

  Description:  接続中の全子機が MB_COMM_PSTATE_BOOT_STARTABLE 状態か判定し,
                そうであれば MB ライブラリ終了処理を開始する.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBPi_CheckAllReboot(void)
{
    if ((mbpState.state == MBP_STATE_REBOOTING) &&
        (mbpState.connectChildBmp == mbpState.rebootChildBmp))
    {
        MBP_Printf("call MB_End()\n");
#if !defined(MBP_USING_MB_EX)
        MB_End();
#else
        MB_EndToIdle();
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         ParentStateCallback

  Description:  親機状態通知コールバック

  Arguments:    child_aid       子機aid.
                status          コールバック状態
                arg             オプション引数
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentStateCallback(u16 child_aid, u32 status, void *arg)
{
#if defined( MBP_DEBUG )
    // デバッグ出力用
    static const char *MB_CALLBACK_TABLE[] = {
        "MB_COMM_PSTATE_NONE",
        "MB_COMM_PSTATE_INIT_COMPLETE",
        "MB_COMM_PSTATE_CONNECTED",
        "MB_COMM_PSTATE_DISCONNECTED",
        "MB_COMM_PSTATE_KICKED",
        "MB_COMM_PSTATE_REQ_ACCEPTED",
        "MB_COMM_PSTATE_SEND_PROCEED",
        "MB_COMM_PSTATE_SEND_COMPLETE",
        "MB_COMM_PSTATE_BOOT_REQUEST",
        "MB_COMM_PSTATE_BOOT_STARTABLE",
        "MB_COMM_PSTATE_REQUESTED",
        "MB_COMM_PSTATE_MEMBER_FULL",
        "MB_COMM_PSTATE_END",
        "MB_COMM_PSTATE_ERROR",
        "MB_COMM_PSTATE_WAIT_TO_SEND",
    };
#endif

    MBP_Printf("get callback %s %d\n", MB_CALLBACK_TABLE[status], child_aid);

    switch (status)
    {
        //-----------------------------------------------------------
        // 親機の初期化処理完了時の通知
    case MB_COMM_PSTATE_INIT_COMPLETE:
        // None.
        break;

        //-----------------------------------------------------------
        // 子機からの接続がきた瞬間の通知
    case MB_COMM_PSTATE_CONNECTED:
        {
            // 親機がエントリー受付状態以外の場合の接続は受け付けない
            if (MBP_GetState() != MBP_STATE_ENTRY)
            {
                break;
            }

            MBP_AddBitmap(&mbpState.connectChildBmp, child_aid);
            // 子機MacAddressの保存
            WM_CopyBssid(((WMStartParentCallback *)arg)->macAddress,
                         childInfo[child_aid - 1].macAddress);

            childInfo[child_aid - 1].playerNo = child_aid;
        }
        break;

        //-----------------------------------------------------------
        // 子機が接続を切った時の通知
    case MB_COMM_PSTATE_DISCONNECTED:
        {
            // 子機のリブート以外の条件で切断された場合はエントリーを削除します。
            if (MBP_GetChildState(child_aid) != MBP_CHILDSTATE_REBOOT)
            {
                MBP_DisconnectChildFromBmp(child_aid);
                // それをふまえたうえで, 全員ブート済みか判定します.
                MBPi_CheckAllReboot();
            }
        }
        break;

        //-----------------------------------------------------------
        // 子機からエントリーリクエストが来た瞬間の通知
    case MB_COMM_PSTATE_REQUESTED:
        {
            const MBUserInfo *userInfo;

            userInfo = (MBUserInfo *)arg;

            OS_TPrintf("callback playerNo = %d\n", userInfo->playerNo);

            // 親機がエントリー受付状態でない場合にエントリー要求をしてきた
            // 子機は確認なしにキックする。
            if (MBP_GetState() != MBP_STATE_ENTRY)
            {
                MBP_KickChild(child_aid);
                break;
            }

            // 子機のエントリーを受け付ける
            mbpState.requestChildBmp |= 1 << child_aid;

            MBP_AcceptChild(child_aid);

            // MB_COMM_PSTATE_CONNECTEDのタイミングではまだUserInfoがセットされていないので
            // MB_CommGetChildUserはREQUESTED以降の状態で呼ばないと意味がありません。
            userInfo = MB_CommGetChildUser(child_aid);
            if (userInfo != NULL)
            {
                MI_CpuCopy8(userInfo, &childInfo[child_aid - 1].user, sizeof(MBUserInfo));
            }
            MBP_Printf("playerNo = %d\n", userInfo->playerNo);
        }
        break;

        //-----------------------------------------------------------
        // 子機へのACCEPTに対するACKの通知
    case MB_COMM_PSTATE_REQ_ACCEPTED:
        // 特に処理は必要ない。
        break;
        //-----------------------------------------------------------
        // 子機からのダウンロード要求を受けた時の通知
    case MB_COMM_PSTATE_WAIT_TO_SEND:
        {
            // 子機の状態をエントリー済み=ダウンロード待ちの状態へ変更
            // 割り込み中の処理なので特に割り込み禁止設定にせずに変更
            mbpState.requestChildBmp &= ~(1 << child_aid);
            mbpState.entryChildBmp |= 1 << child_aid;

            // メインルーチンからMBP_StartDownload()がコールされるとデータ送信を開始する
            // すでにデータ送信状態に入ってしまっている場合はこの子機へもデータ送信を開始する。
#ifdef MBP_USING_PREVIOUS_DOWNLOAD
            /* エントリー後即データ送信を開始する場合 */
            MBP_StartDownload(child_aid);
#else
            /* データ送信を全子機一斉に開始する場合 */
            if (MBP_GetState() == MBP_STATE_DATASENDING)
            {
                MBP_StartDownload(child_aid);
            }
#endif
        }
        break;

        //-----------------------------------------------------------
        // 子機をキックした時の通知
    case MB_COMM_PSTATE_KICKED:
        // None.
        break;

        //-----------------------------------------------------------
        // 子機へバイナリ送信を開始する時の通知
    case MB_COMM_PSTATE_SEND_PROCEED:
        // None.
        break;

        //-----------------------------------------------------------
        // 子機へのバイナリ送信が終了した時の通知
    case MB_COMM_PSTATE_SEND_COMPLETE:
        {
            // 割り込み中の処理なので特に割り込み禁止設定にせずに変更
            mbpState.downloadChildBmp &= ~(1 << child_aid);
            mbpState.bootableChildBmp |= 1 << child_aid;
        }
        break;

        //-----------------------------------------------------------
        // 子機に対してブートが正しく完了した時の通知
    case MB_COMM_PSTATE_BOOT_STARTABLE:
        {
            // 割り込み中の処理なので特に割り込み禁止設定にせずに変更
            mbpState.bootableChildBmp &= ~(1 << child_aid);
            mbpState.rebootChildBmp |= 1 << child_aid;

            // 全子機がブート完了した場合には親機も再接続処理に入る
            MBPi_CheckAllReboot();
        }
        break;

        //-----------------------------------------------------------
        // 対象子機へブート要求の送信を開始した時の通知
        // コールバックとしては返らない。
    case MB_COMM_PSTATE_BOOT_REQUEST:
        // None.
        break;

        //-----------------------------------------------------------
        // メンバーがフルになった場合の通知
    case MB_COMM_PSTATE_MEMBER_FULL:
        // None.
        break;

        //-----------------------------------------------------------
        // マルチブート終了時の通知
    case MB_COMM_PSTATE_END:
        {
            if (MBP_GetState() == MBP_STATE_REBOOTING)
                // リブート処理が完了、MBを終了して子機と再接続を行なう
            {
                MBP_ChangeState(MBP_STATE_COMPLETE);
            }
            else
                // シャットダウン完了、STOP状態へ戻す
            {
                MBP_ChangeState(MBP_STATE_STOP);
            }

            // ゲーム配信用に使用していたバッファをクリア
            // MB_COMM_PSTATE_ENDのコールバックが帰ってきた時点で
            // ワークは開放されているのでFreeしてしまってよい。
            if (sFilebuf)
            {
                OS_Free(sFilebuf);
                sFilebuf = NULL;
            }
            if (sCWork)
            {
                OS_Free(sCWork);
                sCWork = NULL;
            }

            /* MB_Endを呼びworkを開放することで初期化されるためMB_UnregisterFile *
             * は省略することができます。                                        */

        }
        break;

        //-----------------------------------------------------------
        // エラー発生時の通知
    case MB_COMM_PSTATE_ERROR:
        {
            MBErrorStatus *cb = (MBErrorStatus *)arg;

            switch (cb->errcode)
            {
                //------------------------------
                // 通知レベルのエラー. 特に処理を行なう必要はない.
            case MB_ERRCODE_WM_FAILURE:
                // None.
                break;
                //------------------------------
                // 無線をリセットする必要があるレベルのエラー。
            case MB_ERRCODE_INVALID_PARAM:
            case MB_ERRCODE_INVALID_STATE:
            case MB_ERRCODE_FATAL:
                MBP_ChangeState(MBP_STATE_ERROR);
                break;
            }
        }
        break;

        //-----------------------------------------------------------
    default:
        OS_Panic("Get illegal parent state.\n");
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_ChangeState

  Description:  親機状態変更

  Arguments:    state       変更する状態.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBP_ChangeState(u16 state)
{
#if defined( MBP_DEBUG )
    // デバッグ出力用
    static const char *STATE_NAME[] = {
        "MBP_STATE_STOP",
        "MBP_STATE_IDLE",
        "MBP_STATE_ENTRY",
        "MBP_STATE_DATASENDING",
        "MBP_STATE_REBOOTING",
        "MBP_STATE_COMPLETE",
        "MBP_STATE_CANCEL",
        "MBP_STATE_ERROR"
    };
#endif

    SDK_ASSERT(state < MBP_STATE_NUM);

    MBP_Printf("%s -> %s\n", STATE_NAME[mbpState.state], STATE_NAME[state]);
    mbpState.state = state;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetState

  Description:  親機状態の取得

  Arguments:    None.

  Returns:      親機状態.
 *---------------------------------------------------------------------------*/
u16 MBP_GetState(void)
{
    return mbpState.state;
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetConnectState

  Description:  接続情報の取得

  Arguments:    現在の接続状態を取得するためのバッファ領域.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_GetConnectState(MBPState * state)
{
    MI_CpuCopy8(&mbpState, state, sizeof(MBPState));
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildBmp

  Description:  子機の接続ビットマップを取得します

  Arguments:    取得する接続ビットマップの種類.

  Returns:      子機状態.
 *---------------------------------------------------------------------------*/
u16 MBP_GetChildBmp(MBPBmpType bmpType)
{
    u16     tmpBitmap;
    static const u16 *BITMAP_TABLE[MBP_BMPTYPE_NUM] = {
        &mbpState.connectChildBmp,
        &mbpState.requestChildBmp,
        &mbpState.entryChildBmp,
        &mbpState.downloadChildBmp,
        &mbpState.bootableChildBmp,
        &mbpState.rebootChildBmp,
    };

    SDK_ASSERT(bmpType < MBP_BMPTYPE_NUM);

    tmpBitmap = *(BITMAP_TABLE[bmpType]);

    return tmpBitmap;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildState

  Description:  子機の接続状態を取得

  Arguments:    aid     接続子機番号.

  Returns:      子機の状態.
 *---------------------------------------------------------------------------*/
MBPChildState MBP_GetChildState(u16 aid)
{
    MBPState tmpState;
    u16     bitmap = (u16)(1 << aid);

    // 割り込みによって不意に接続状態が変わる事があるため
    // 割り込みを禁止してその時点の状態のスナップショットを取得
    OSIntrMode enabled = OS_DisableInterrupts();
    if ((mbpState.connectChildBmp & bitmap) == 0)
    {
        (void)OS_RestoreInterrupts(enabled);
        return MBP_CHILDSTATE_NONE;    // 未接続
    }
    MI_CpuCopy8(&mbpState, &tmpState, sizeof(MBPState));
    (void)OS_RestoreInterrupts(enabled);

    // 接続状態判定
    if (tmpState.requestChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_REQUEST; // 接続リクエスト中
    }
    if (tmpState.entryChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_ENTRY;   // エントリー中
    }
    if (tmpState.downloadChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_DOWNLOADING;      // ダウンロード中
    }
    if (tmpState.bootableChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_BOOTABLE; // ブート準備完了
    }
    if (tmpState.rebootChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_REBOOT;  // リブート済み
    }

    return MBP_CHILDSTATE_CONNECTING;  // コネクト中
}


/*---------------------------------------------------------------------------*
  Name:         MBP_AddBitmap

  Description:  接続状態ビットップに子機を追加

  Arguments:    pBitmap     追加するビットマップへのポインタ.
                aid         追加する子機のaid

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_AddBitmap(u16 *pBitmap, u16 aid)
{
    // 値の変更中の不意な割り込みによって状態が壊れないように
    // 割り込みを禁止します。
    OSIntrMode enabled = OS_DisableInterrupts();
    *pBitmap |= (1 << aid);
    (void)OS_RestoreInterrupts(enabled);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_RemoveBitmap

  Description:  接続状態ビットマップから子機を削除

  Arguments:    pBitmap     削除するビットマップへのポインタ.
                aid         削除する子機のaid

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_RemoveBitmap(u16 *pBitmap, u16 aid)
{
    // 値の変更中の不意な割り込みによって状態が壊れないように
    // 割り込みを禁止します。
    OSIntrMode enabled = OS_DisableInterrupts();
    *pBitmap &= ~(1 << aid);
    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_DisconnectChildFromBmp

  Description:  子機が切断された時にすべてのビットマップから子機のフラグを削除
                します。

  Arguments:    aid     切断された子機のaid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_DisconnectChildFromBmp(u16 aid)
{
    u16     aidMask = (u16)~(1 << aid);

    OSIntrMode enabled = OS_DisableInterrupts();

    mbpState.connectChildBmp &= aidMask;
    mbpState.requestChildBmp &= aidMask;
    mbpState.entryChildBmp &= aidMask;
    mbpState.downloadChildBmp &= aidMask;
    mbpState.bootableChildBmp &= aidMask;
    mbpState.rebootChildBmp &= aidMask;

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_DisconnectChild

  Description:  子機を切断します。

  Arguments:    aid     切断する子機のaid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_DisconnectChild(u16 aid)
{
    // 将来的にMB_Disconnectが用意されるかもしれません。
    MBP_Printf(" WM_Disconnect %d\n", aid);
    MBP_DisconnectChildFromBmp(aid);
    MB_DisconnectChild(aid);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildInfo

  Description:  子機のユーザ情報へのポインタを取得します。

  Arguments:    child_aid   子機のaid.

  Returns:      ユーザ情報へのポインタ.
 *---------------------------------------------------------------------------*/
const MBPChildInfo *MBP_GetChildInfo(u16 child_aid)
{
    if (!(mbpState.connectChildBmp & (1 << child_aid)))
    {
        return NULL;
    }
    return &childInfo[child_aid - 1];
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetPlayerNo

  Description:  子機をリブートして再接続した後に、マルチブート子機だった時代の
                プレイヤー番号(aid)をMACアドレスを元に検索し取得できます。

  Arguments:    macAddress      子機のMACアドレス.

  Returns:      マルチブート接続時の子機aid.
 *---------------------------------------------------------------------------*/
u16 MBP_GetPlayerNo(const u8 *macAddress)
{
    u16     i;

    for (i = 1; i < MBP_CHILD_MAX + 1; i++)
    {
        if ((mbpState.connectChildBmp & (1 << i)) == 0)
        {
            continue;
        }
        if (WM_IsBssidEqual(macAddress, childInfo[i - 1].macAddress))
        {
            return childInfo[i - 1].playerNo;
        }
    }

    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildMacAddress

  Description:  マルチブート子機のMACアドレスを取得

  Arguments:    aid     子機のaid.

  Returns:      MACアドレスへのポインタ.
 *---------------------------------------------------------------------------*/
const u8 *MBP_GetChildMacAddress(u16 aid)
{
    if (!(mbpState.connectChildBmp & (1 << aid)))
    {
        return NULL;
    }
    return childInfo[aid - 1].macAddress;
}
