#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "tp_input.h"
#include "wc.h"
#include "screen.h"
#include "dbs.h"
#include "mp.h"

#include "registerfriend.h"

extern DWCUserData stUserData; // ユーザデータを格納する構造体。
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // ゲーム制御情報構造体
extern KeyControl stKeyCnt;    // キー入力制御構造体

// 友達登録鍵
static char stFriendKey[sizeof(DWCFriendData)];

// サンプルMP交換用友達データ
static DWCFriendData stSampleFriendData ATTRIBUTE_ALIGN(32);

//データ通信用バッファ
static u16* gSendBuf ATTRIBUTE_ALIGN(32);//送信用バッファ

/*---------------------------------------------------------------------------*
  友達登録メイン関数
  *---------------------------------------------------------------------------*/
GameMode GameRegisterFriendMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_REG_FRIEND];
    GameMode returnSeq = GAME_MODE_NUM;
    RegFriendParam cntParam;
    int curIdx = 0;
    int friendIdx;
    int maxFigure;
    int validFlg;
    char tp_data;

    int i, j;

    tp_init();// タッチペン入力の初期化
    
    cntParam.step   = 0;
    cntParam.figure = 0;

    MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
    
    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );

        ReadKeyData();// キーデータ取得

        // 次に進むべきモードがセットされていたら抜ける
        if (returnSeq != GAME_MODE_NUM){
            break;
        }
        
        ////////// 以下キー操作処理
        if (cntParam.step == 0){
            // コマンド選択中
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // Aボタンでメニュー決定
                returnSeq = gameSeq->menuList[curIdx].mode;
                stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

                if ( curIdx == 0 ){
                    // 無線初期化
                    DWC_CleanupInet();
                    
                    // MP通信用データ作成
                    DWC_CreateExchangeToken(&stUserData, &stSampleFriendData);
                    gSendBuf = (u16*)(&stSampleFriendData);
                    
                    mp_match(gSendBuf, stGameCnt.friendList.keyList);//MP通信

                    friendIdx = GetAvailableFriendListIndex();// 同一のMPデータを省く
                    for(i = 0; i < friendIdx; i++){
                        for(j = i+1; j < friendIdx; j++){
                            if(DWC_IsEqualFriendData( (const DWCFriendData*)&stGameCnt.friendList.keyList[i], (const DWCFriendData*)&stGameCnt.friendList.keyList[j])){
                                MI_CpuClear8(&stGameCnt.friendList.keyList[j], sizeof(DWCFriendData));
                            }
                        }
                    }
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                
                else if ((curIdx == 1) || (curIdx == 2)){
                    // 友達登録鍵入力による友達登録、
                    // もしくはインデックスを指定して友達データを削除
                    cntParam.step   = 1;
                    cntParam.figure = 0;
                    cntParam.value  = 0;
                    
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                    
                    if (curIdx == 1){
                        maxFigure = 12;  // 友達登録鍵は最高１２桁
                    }
                    else {
                        maxFigure = 2;   // 友達リストインデックスは最高２桁
                    }
                }
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // 十字キー上でカーソル移動
                curIdx--;
                if (curIdx < 0) curIdx = gameSeq->numMenu-1;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // 十字キー下でカーソル移動
                curIdx++;
                if (curIdx >= gameSeq->numMenu) curIdx = 0;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        else {
            if(cntParam.figure < maxFigure){
                tp_data = tp_input();
				if(tp_data){
                    if(tp_data > 47){//0～9までの数値が入力された場合
                        stFriendKey[cntParam.figure] = tp_data;
                        if (cntParam.figure < maxFigure-1) cntParam.figure++;
                    }
                    else if(tp_data == 1){//<-が入力された場合
                        if (cntParam.figure > 0) cntParam.figure--;
                    }
                    else if(tp_data == 2){//->が入力された場合
                        if (cntParam.figure < maxFigure-1) cntParam.figure++;
                    }
                    // delete用データ
                    cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                // タッチペン入力のための表示
                PrintString(10, 4, 0xf,  "-------------");
                PrintString(10, 5, 0xf,  "l   l   l   l");
                PrintString(10, 6, 0xf,  "l 7 l 8 l 9 l");
                PrintString(10, 7, 0xf,  "l   l   l   l");
                PrintString(10, 8, 0xf,  "l-----------l");
                PrintString(10, 9, 0xf,  "l   l   l   l");
                PrintString(10, 10, 0xf, "l 4 l 5 l 6 l");
                PrintString(10, 11, 0xf, "l   l   l   l");
                PrintString(10, 12, 0xf, "l-----------l");
                PrintString(10, 13, 0xf, "l   l   l   l");
                PrintString(10, 14, 0xf, "l 1 l 2 l 3 l");
                PrintString(10, 15, 0xf, "l   l   l   l");
                PrintString(10, 16, 0xf, "l-----------l");
                PrintString(10, 17, 0xf, "l   l   l   l");
                PrintString(10, 18, 0xf, "l 0 l<--l-->l");
                PrintString(10, 19, 0xf, "l   l   l   l");
                PrintString(10, 20, 0xf, "-------------");

                PrintString(2, 22, 0xf, "Set: PUSH A   Calcel: PUSH B");
            }
            
            // 数値入力中
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // Aボタンで友達の登録・削除を行う
                if (curIdx == 1){
                    // 友達登録
                    if (cntParam.value){
                        friendIdx = GetAvailableFriendListIndex();
                        if (friendIdx == -1){
                            OS_TPrintf("Failed to register new friend. Friend list is full.\n");
                        }
                        else {
                            if( DWC_CheckFriendKey( &stUserData, DWC_StringToFriendKey(stFriendKey)) ){// 友達登録鍵の正当性チェック
                                DWC_CreateFriendKeyToken(&stGameCnt.friendList.keyList[friendIdx], DWC_StringToFriendKey(stFriendKey));// 友達登録鍵による友達登録
                            }
                        }
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // プロファイルID=0は無効
                    }
                }
                else {
                    // 友達削除
                    if (cntParam.value < GAME_MAX_FRIEND_LIST){
                        // 友達リストから友達を削除
                        DWC_DeleteBuddyFriendData(&stGameCnt.friendList.keyList[cntParam.value]);
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // 友達リスト範囲外
                    }
                }

                if (validFlg){
                    // 有効な入力であればコマンド選択に戻る
                    cntParam.step = 0;
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                ClearScreen();
            }
            else if (stKeyCnt.trg & PAD_BUTTON_B){
                MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
                // Bボタンでコマンド選択に戻る
                cntParam.step = 0;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                ClearScreen();
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // 十字キー上下で選択中の数値の増減
                stFriendKey[cntParam.figure]++;
                if(stFriendKey[cntParam.figure] > 57){
                    stFriendKey[cntParam.figure] = '0';
                }
                //delete用データ
                cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // 十字キー上下で選択中の数値の増減
                stFriendKey[cntParam.figure]--;
                if(stFriendKey[cntParam.figure] < 48){
                    stFriendKey[cntParam.figure] = '9';
                }
                //delete用データ
                cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_LEFT){
                // 十字キー左右で選択桁数の移動
                if (cntParam.figure > 0 && cntParam.figure <= maxFigure-1 ) cntParam.figure--;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_RIGHT){
                // 十字キー左右で選択桁数の移動
                if (cntParam.figure >= 0 && cntParam.figure < maxFigure-1 ) cntParam.figure++;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        ////////// キー操作処理ここまで

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // スタック溢れチェック
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

/*---------------------------------------------------------------------------*
  友達登録・消去モードの追加表示用コールバック関数
 *---------------------------------------------------------------------------*/
void RegFriendModeDispCallback(int curIdx, void* param)
{
    RegFriendParam cntParam;
    int i;

    if (!param){
        // 初回はparam = NULLで呼ばれる
        cntParam.step = 0;
    }
    else {
        cntParam = *(RegFriendParam *)param;
    }

    OS_TPrintf("\n");

    if (cntParam.step == 0){
        // コマンド選択中は友達リストを表示
        DispFriendList(FALSE);
    }
    else {
        // 数値入力中
        if (curIdx == 1){
            // 友達登録。プロファイルID入力中
            OS_TPrintf("Set friend key : ");
            for(i = 0; i < 12; i++){
                OS_TPrintf("%c", stFriendKey[i]);
            }
            OS_TPrintf("\n");

            // 選択中の数字を示す為の下線を表示する
            for (i = 0; i < cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                 -\n");
        }
        else {
            // 友達削除。友達リストインデックス入力中
            DispFriendList(FALSE);  // 友達リストを表示
            
            OS_TPrintf("Delete friend Index : ");
            for(i = 0; i < 2; i++){
                OS_TPrintf("%c", stFriendKey[i]);
            }
            OS_TPrintf("\n");

            for (i = 0; i < cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                      -\n");
        }
    }
}

