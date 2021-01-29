#include <nitro.h>
#include "utility.h"


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static KeyControl *pKeyCnt;


/*---------------------------------------------------------------------------*
  コントローラ構造体初期化関数
 *---------------------------------------------------------------------------*/
void InitKeyData(KeyControl *pKeyCntSrc)
{
    int i;

    pKeyCnt = pKeyCntSrc;
    pKeyCnt->cont    = 0;
    pKeyCnt->trg     = 0;
    pKeyCnt->release = 0;
    pKeyCnt->repeat  = 0;
    pKeyCnt->checkPushButton = 0;
    pKeyCnt->repeatDelayTime = 0;
    pKeyCnt->repeatPulseTime = 0;
    for (i = 0; i < 10; i++){
        pKeyCnt->pushCount[i] = 0;
        pKeyCnt->repeatSeq[i] = 0;
    }
}


/*---------------------------------------------------------------------------*
  コントローラ読み込み関数
 *---------------------------------------------------------------------------*/
void ReadKeyData(void)
{
    u16 readData;
    int i;

    readData = PAD_Read();
    pKeyCnt->trg     = (u16)(readData & (readData ^ pKeyCnt->cont));  // トリガ入力
    pKeyCnt->release = (u16)(pKeyCnt->cont & (readData ^ pKeyCnt->cont));  // リリース
    pKeyCnt->cont    = readData;  // ベタ入力
    pKeyCnt->repeat  = 0;         // リピート入力

    // キーリピート状態のセット
    for (i = 0; i < 10; i++){
        if (pKeyCnt->cont & (pKeyCnt->checkPushButton & (1 << i))){
            pKeyCnt->pushCount[i]++;
            if (pKeyCnt->repeatSeq[i] == 0){
                // delay時間中
                if (pKeyCnt->pushCount[i] == pKeyCnt->repeatDelayTime){
                    // リピートトリガ入力発生（delay）
                    pKeyCnt->repeat |= pKeyCnt->checkPushButton & (1 << i);
                    pKeyCnt->pushCount[i] = 0;
                    pKeyCnt->repeatSeq[i]++;
                }
            }
            else if (pKeyCnt->pushCount[i] == pKeyCnt->repeatPulseTime){
                // pulse時間中
                // リピートトリガ入力発生（pulse）
                pKeyCnt->repeat |= pKeyCnt->checkPushButton & (1 << i);
                pKeyCnt->pushCount[i] = 0;
            }
        }
        else {
            pKeyCnt->pushCount[i] = 0;
            pKeyCnt->repeatSeq[i] = 0;
        }
    }
}


/*---------------------------------------------------------------------------*
  キーリピート設定関数
 *---------------------------------------------------------------------------*/
// この関数を呼び出すとそれまでのキーリピート設定はクリアされる
// よってkeyFlg = 0 でこの関数を呼び出せば、キーリピート設定をクリアできる
void SetKeyRepeat(u16 keyFlg, u16 delay, u16 pulse)
{
    int i;

    pKeyCnt->repeat = 0;
    pKeyCnt->checkPushButton = keyFlg;
    pKeyCnt->repeatDelayTime = delay;
    pKeyCnt->repeatPulseTime = pulse;
    
    for (i = 0; i < 10; i++){
        pKeyCnt->pushCount[i] = 0;
        pKeyCnt->repeatSeq[i] = 0;
    }
}


/*---------------------------------------------------------------------------*
  リピートキー追加関数
 *---------------------------------------------------------------------------*/
void AddKeyRepeat(u16 keyFlg)
{
    int i;

    
    pKeyCnt->repeat &= ~keyFlg;
    pKeyCnt->checkPushButton |= keyFlg;

    for (i = 0; i < 10; i++){
        if (keyFlg & (1 << i)){
            pKeyCnt->pushCount[i] = 0;
            pKeyCnt->repeatSeq[i] = 0;
        }
    }
}


/*---------------------------------------------------------------------------*
  べき乗関数
 *---------------------------------------------------------------------------*/
// 整数の計算では逆数は0にしかならないため、指数は正の数しか取らない
s32 UtilPow(s32 x, u32 n)
{
    s32 r;
    
    r = 1;
    while (n){
        if (n & 1){
            r *= x;
        }
        x *= x;
        n >>= 1;
    }

    return r;
}
