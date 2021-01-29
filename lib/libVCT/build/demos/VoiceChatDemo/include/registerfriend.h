#ifndef __REGISTERFRIEND_H__
#define __REGISTERFRIEND_H__

// [nakata] GameSpy SDKにプラットフォームを教えるための定数
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif

//#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
typedef struct tagRegFriendParam
{
    int step;    // 0:コマンド選択中、1:登録・削除友達データ入力中 
    u32 figure;  // 入力中の桁 
    u32 value;   // 入力中の値 
} RegFriendParam;

//プロトタイプ
void RegFriendModeDispCallback(int curIdx, void* param);
GameMode GameRegisterFriendMain(void);

    
#ifdef __cplusplus
}
#endif

#endif  // __REGISTERFRIEND_H__
