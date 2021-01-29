#ifndef __MAIN_H__
#define __MAIN_H__

// [nakata] GameSpy SDKにプラットフォームを教えるための定数
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif


#ifdef __cplusplus
extern "C" {
#endif
    

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_MAX_SAVE_FILE   4   // ゲーム内のセーブファイル数（＝プレイヤー数）
#define GAME_MAX_FRIEND_LIST 32  // 友達登録最大数
#define GAME_MAX_PLAYER_NAME 16  // ゲーム内プレイヤーネーム長
    

// ゲームモード列挙子
typedef enum
{
    GAME_MODE_MAIN = 0,    // オフラインのメインゲーム
    GAME_MODE_REG_FRIEND,  // 友達登録・消去
    GAME_MODE_LOGIN,       // ログイン表示
    GAME_MODE_CONNECTED,   // マッチメイク終了後
    GAME_MODE_NETCONFIG_LANGUAGE,   // Wi-Fiコネクション設定の使用言語選択
    GAME_MODE_NETCONFIG,   // Wi-Fiコネクション設定
    GAME_MODE_STARTIP,     // IP取得
    GAME_MODE_STOPIP,      // IP開放
    GAME_MODE_BACKUP,      // バックアップ関連処理
    GAME_MODE_NUM
} GameMode;


//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
// ゲームシーケンス関数型
// 戻り値は次に進むゲームモード
typedef GameMode (*GameSeqFunc)(void);

// モードごとに追加表示を行うためのコールバック型
typedef void (*GameAddedDispCallback)(int curIdx, void* param);


typedef struct tagSavedPlayerData
{
    char playerName[GAME_MAX_PLAYER_NAME];  // ゲーム内プレイヤー名
} SavedPlayerData;

// ゲームで使うデータを付加した友達リスト
typedef struct tagGameFriendList
{
    DWCFriendData keyList[GAME_MAX_FRIEND_LIST];  // DWC形式の友達リスト
    char playerName[GAME_MAX_FRIEND_LIST][GAME_MAX_PLAYER_NAME];  // プレイヤー名
} GameFriendList;

// ゲームセーブデータ構造体
typedef struct tagGameSaveData
{
    SavedPlayerData playerData[GAME_MAX_SAVE_FILE];
    int  profileID;        // ユーザ固有のプロファイルID
    BOOL isValidProfile;   // プロファイルIDが有効かどうか
    GameFriendList friendList;  // ゲームで使うデータを付加した友達リスト
} GameSaveData;


// ゲームシーケンス制御リスト要素構造体
typedef struct tagGameMenuElement
{
    const char* msg;  // 対応するメッセージ
    GameMode mode;    // 対応するゲームモード。モードが変わらない時はGAME_MODE_NUMを指定
    BOOL blocking;    // 選択時に状態変化完了まで操作不能にする必要があるか
} GameMenuElement;

// ゲームシーケンス制御リスト構造体
typedef struct tagGameSequence
{
    const char* modeName;  // モード名
    GameSeqFunc seqFunc;   // 対応する関数
    GameAddedDispCallback dispCallback;  // 対応する追加表示コールバック
    int numMenu;           // メニューの数
    GameMenuElement menuList[8];  // メニューリスト
} GameSequence;


typedef struct tagGameUserData
{
    int  playerIdx;       // ゲーム内プレイヤーインデックス
    char playerName[GAME_MAX_PLAYER_NAME];  // 現在プレイ中のゲーム内プレイヤー名
    int  profileID;       // ユーザ固有のプロファイルID
    BOOL isValidProfile;  // プロファイルIDが有効かどうか
} GameUserData;

// ゲーム制御情報構造体
typedef struct tagGameControl
{
    GameUserData userData;  // プレイ中のユーザデータ
    GameMode mode;          // 現在のゲームモード
    BOOL blocking;          // TRUEなら操作不能（ネットワーク状態変化待ち）状態
    const char* essID;      // 使用するAPのESSID
    GameFriendList friendList;  // ゲームで使うデータを付加した友達リスト
    u8  friendStatus[GAME_MAX_FRIEND_LIST];  // 友達の通信状態。インデックスは友達リストのものと対応
} GameControl;

// 友達無指定ピアマッチメイク用追加キーデータ構造体
typedef struct tagGameMatchExtKeys
{
    u8  keyID;        // マッチメイク用キーID
    u8  isStr;        // 0:値がint型、1:値が文字列
    u16 pad;          // パディング
    char keyStr[16];  // マッチメイク用キー文字列
    char svalue[16];  // キーに対応する値（文字列）
    int ivalue;       // キーに対応する値（int型）
} GameMatchExtKeys;


//----------------------------------------------------------------------------
// function
//----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif

#endif  // __MAIN_H__
