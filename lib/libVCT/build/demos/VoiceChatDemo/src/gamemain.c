#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "dbs.h"

#include "gamemain.h"

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // ゲーム制御情報構造体 
extern KeyControl stKeyCnt;    // キー入力制御構造体 
extern DWCFriendsMatchControl stDwcCnt;    // DWC制御構造体 

extern DWCUserData stUserData;  // 本体固有のユーザID(本来はユーザの入力であったりゲームごとに割り当てられるものであったりする値)
extern GameSaveData saveData;  

static volatile int stNameCbLevel; // プレイヤー名取得完了待ち件数 

static void LoginCallback(DWCError error, int profileID, void* param);
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param);
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param);
static void DeleteFriendListCallback(int deletedIndex, int srcIndex,void* param);
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param);
static void LoadFromServerCallback(BOOL success, int profileID, char* data, int len, void* param);

/*---------------------------------------------------------------------------*
  オフライン時メイン関数
 *---------------------------------------------------------------------------*/
GameMode GameMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_MAIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
        
        ReadKeyData();  // キーデータ取得 

        DWC_ProcessFriendsMatch();  // DWC通信処理更新 

        if (DWC_GetLastError(NULL)){
            // ログイン・サーバアップデート失敗時の処理
            ShutdownInet();  // ネットワーク切断 
            DWC_ClearError();
            stGameCnt.blocking = FALSE;

            returnSeq = GAME_MODE_MAIN;
        }

        if (stGameCnt.blocking){
            // DWC処理中はキー操作を禁止する
            dbs_DemoUpdate();
            //SVC_WaitVBlankIntr();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();

            // スタック溢れチェック
            OS_CheckStack(OS_GetCurrentThread());
            continue;
        }

        // 次に進むべきモードがセットされていたら認証完了
        if (returnSeq == GAME_MODE_LOGIN){
            // ログインが選択されていた場合
            if (stGameCnt.userData.isValidProfile){
               // 認証成功でプロファイルIDを取得できた場合はループを抜ける
                 break;
            }
            else {
                // 認証失敗の場合はモードを進めないようにする
                returnSeq = GAME_MODE_NUM;
            }
        }
        else if (returnSeq != GAME_MODE_NUM){
            // それ以外の場合は必ず抜ける
            break;
        }
            
        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
           // Aボタンでメニュー決定
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (returnSeq){
            case GAME_MODE_LOGIN:  // ログイン 

                // DWCライブラリ初期化
                // ライブラリが使うソケットの送受信バッファサイズは
                // デフォルトのまま（8KByte）にしておく
                DWC_InitFriendsMatch(&stDwcCnt, &stUserData, GAME_PRODUCTID, GAME_NAME,
                                     GAME_SECRET_KEY, 0, 0,
                                     stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST);

                // 認証用関数を使ってログイン
                DWC_LoginAsync(L"Name", NULL, LoginCallback, &returnSeq);
                break;
            default:
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // 十字キー上でカーソル移動
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
             // 十字キー下でカーソル移動
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if ( stKeyCnt.trg & PAD_BUTTON_Y){
            Heap_Dump();
        }
        ////////// キー操作処理ここまで

        dbs_DemoUpdate();
        //SVC_WaitVBlankIntr();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
        dbs_DemoLoad();
        // スタック溢れチェック
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

/*---------------------------------------------------------------------------*
  ログインコールバック関数
 *---------------------------------------------------------------------------*/
static void LoginCallback(DWCError error, int profileID, void *param)
{
    BOOL result;
    u64 friend_key;
    
    if (error == DWC_ERROR_NONE){
        // 認証成功、プロファイルID取得
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);
        stGameCnt.userData.profileID = profileID;
        stGameCnt.userData.isValidProfile = TRUE;

        // ingamesnチェックの結果を取得する
        if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // 不適切な名前と判断された場合は特別な処理が必要
            OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }

        // 友達登録鍵作成
        friend_key = DWC_CreateFriendKey( &stUserData );
		if( friend_key ){
            OS_TPrintf("friend_key = %lld\n", friend_key);
        }
        
        // 友達リスト同期処理開始
        result = DWC_UpdateServersAsync(stGameCnt.userData.playerName,
                                        UpdateServersCallback, param,
                                        FriendStatusCallback, param,
                                        DeleteFriendListCallback, param);
        if (result == FALSE){
           // 呼んでもいい状態（ログインが完了していない状態）で呼んだ時のみ
            // FALSEが返ってくるので、普通はTRUE
            OS_Panic("--- DWC_UpdateServersAsync error teminated.\n");
        }

        // ストレージサーバセーブ・ロード完了通知コールバックを登録する
        // Register a callback for making notification that storage save/load is complete
        DWC_SetStorageServerCallback(SaveToServerCallback,
                                     LoadFromServerCallback);
    }
    else {
        // 認証失敗
        DispErrorMessage();
        OS_TPrintf("Login failed. %d\n\n", error);

        // もう一度最初から
        *(GameMode *)param = GAME_MODE_NUM;

        stGameCnt.blocking = FALSE;  // ブロッキング解除 
    }
}


/*---------------------------------------------------------------------------*
  友達リスト同期処理完了コールバック関数
 *---------------------------------------------------------------------------*/
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param)
{
#pragma unused(param)
    int i;

    if (error == DWC_ERROR_NONE){
        // 友達リスト同期処理完了
        OS_TPrintf("Updating servers succeeded.\n");

        if (isChanged){
            // 友達リストが変更されていたらセーブする
            // （セーブのつもり）
            MI_CpuCopy32(&stGameCnt.friendList, &saveData.friendList, sizeof(GameFriendList));
        }

        // 友達の通信状態と名前を取得する
        stNameCbLevel = 0;
        for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
            if (DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i])){
                stGameCnt.friendStatus[i] =
                    DWC_GetFriendStatus(&stGameCnt.friendList.keyList[i], NULL);
    
                // プレイヤー名をロード
                if (DWC_LoadOthersDataAsync("\\dwc_name", i, (void *)i)){
                    stNameCbLevel++;
                }
            }
        }

        // 友達の名前が取得できるまでここで待つ
        while (stNameCbLevel){
            ReadKeyData();  // キーデータ取得 

            DWC_ProcessFriendsMatch();  // DWC通信処理更新 

            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
        }

        // 友達リストを表示する
        DispFriendList(FALSE);
    }
    else {
        // 失敗しても特に何もしない
        OS_TPrintf("Failed to update GameSpy servers. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除 
}

/*---------------------------------------------------------------------------*
   友達状態変化通知コールバック関数
 *---------------------------------------------------------------------------*/
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param)
{
#pragma unused(param)
#ifdef SDK_FINALROM
#pragma unused(index, status, statusString)
#endif

    OS_TPrintf("Friend[%d] changed status -> %d (statusString : %s).\n",
               index, status, statusString);
}


/*---------------------------------------------------------------------------*
  友達リスト削除コールバック関数
 *---------------------------------------------------------------------------*/
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param)
{
#pragma unused(param)
#ifdef SDK_FINALROM
#pragma unused(deletedIndex, srcIndex)
#endif
#if 0
     // 友達データの削除・前詰めに合わせて、それに対応するデータも前に詰める
    if (index < GAME_MAX_FRIEND_LIST-1){
        // 友達の通信状態を前に詰める
        MI_CpuCopy8(&stGameCnt.friendStatus[index+1], &stGameCnt.friendStatus[index],
                    (u32)(GAME_MAX_FRIEND_LIST-index-1));

        // 友達の通信状態を前に詰める
        MI_CpuCopy8(&stGameCnt.friendList.playerName[index+1],
                    &stGameCnt.friendList.playerName[index],
                    (u32)(GAME_MAX_PLAYER_NAME*(GAME_MAX_FRIEND_LIST-index-1)));
    }

    stGameCnt.friendStatus[GAME_MAX_FRIEND_LIST-1] = DWC_STATUS_OFFLINE;
    MI_CpuClear32(&stGameCnt.friendList.playerName[GAME_MAX_FRIEND_LIST-1],
                  GAME_MAX_PLAYER_NAME);

#else
    OS_TPrintf("friend[%d] was deleted (equal friend[%d]).\n",
               deletedIndex, srcIndex);
#endif
}


/*---------------------------------------------------------------------------*
  ストレージサーバへのデータセーブ完了コールバック関数
 *---------------------------------------------------------------------------*/
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param)
{
#ifdef SDK_FINALROM
#pragma unused(success, isPublic, param)
#endif

    OS_TPrintf("Saved data to server. %08x\n", (u32)param);
    OS_TPrintf("result %d, isPublic %d.\n", success, isPublic);
}


/*---------------------------------------------------------------------------*
  ストレージサーバからのデータロード完了コールバック関数
 *---------------------------------------------------------------------------*/
static void LoadFromServerCallback(BOOL success, int profileID, char* data, int len, void* param)
{
#ifdef SDK_FINALROM
#pragma unused(profileID, len)
#endif

    OS_TPrintf("Loaded data from server. %08x\n", (u32)(param));
    OS_TPrintf("result %d, profileID %u, data '%s', len %d\n",
               success, profileID, data, len);

    if (success){
        // 汎用key/value型文字列操作関数を使いプレイヤー名を取得
        (void)DWC_GetCommonValueString("dwc_name",
                                       stGameCnt.friendList.playerName[(int)param],
                                       data, '\\');

        stNameCbLevel--;
    }
}


