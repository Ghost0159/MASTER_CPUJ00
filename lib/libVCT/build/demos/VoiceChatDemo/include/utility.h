#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <nitro/types.h>

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
// コントローラ入力構造体
// 複数のキーにリピート設定を行なう場合は、全て同じDelayTimeとPulseTimeしか
// 設定できない
//  
typedef struct {
    u8  repeatSeq[10];    // キーリピートのステップ（0:Delay, 1:Pulse） 
    u16 cont;             // ベタ入力 
    u16 trg;              // トリガ入力 
    u16 release;          // リリース入力  
    u16 repeat;           // リピートトリガ入力 
    u16 checkPushButton;  // キーリピート状態を調べるボタンのフラグ 
    u16 repeatDelayTime;  // 最初のトリガ入力からキーリピート開始までの間隔 
    u16 repeatPulseTime;  // キーリピート間隔 
    u16 pad;              // パディング 
    u16 pushCount[10];    // 1つのボタンを押し続けている時間を計測 
} KeyControl;


//----------------------------------------------------------------------------
// function
//----------------------------------------------------------------------------
extern void InitKeyData(KeyControl *pKeyCntSrc);
extern void ReadKeyData(void);
extern void SetKeyRepeat(u16 keyFlg, u16 delay, u16 pulse);
extern void AddKeyRepeat(u16 keyFlg);

extern s32  UtilPow(s32 x, u32 n);


#ifdef __cplusplus
}
#endif

#endif  // __UTILITY_H__
