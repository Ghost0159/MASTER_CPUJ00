/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - FrameTexVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/
//
// フレームVRAMマネージャ(テクスチャイメージ)を使用するサンプルです。
// さまざまなパターンで領域確保のテストを行います。
// 
#include <nitro.h>
#include <nnsys/gfd.h>

//   プログラム内で使いまわす変数
static NNSGfdFrmTexVramState     state_;
static NNSGfdTexKey mem;

//  リージョンごとの空き領域
//  ＶＲＡＭの空きがライブラリの取得関数で得られる値と同じかどうかを確かめるために使う
static int vramFreeSize[5];


//  そのリージョンの空き領域の大きさを計算する
static int calcFreeSize(int regionNo) 
{
    //
    // ステートの取得
    //
    NNS_GfdGetFrmTexVramState(&state_);
    
    {
        // ステートから空き容量の取得
        const int size 
            = (int)state_.address[regionNo * 2 + 1] - (int)state_.address[regionNo * 2 + 0];
            
        return size;
    }
}


//   スロット０についていろいろなパターンでテストをする
void testSlot0();

//   スロット０,１について,いろいろなパターンでテストをする
//   スロット２,３がある場合はそれを使いきった状態でテストする
void testSlot01(int numSlot);

//   スロット０,１,２について,いろいろなパターンでテストをする
//   スロット３がある場合はそれを使いきった状態でテストする
void testSlot012(int numSlot);

//   スロット０，１，２，３についていろいろなパターンでテストをする
void testSlot0123();


//   スロット２,３がある場合はそれを使い切る
static void exhaustSlot23(int numSlot) {
    if(numSlot == 3) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
    if(numSlot == 4) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
}

//   スロット３がある場合はそれを使い切る
static void exhaustSlot3(int numSlot) {
    if(numSlot == 4) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
}


//------------------------------------------------------------------------------
//   スロット０についていろいろなパターンでテストをする
//------------------------------------------------------------------------------
static void testSlot0() {

    //   領域を解放する
    NNS_GfdResetFrmTexVramState();

    vramFreeSize[0] = 0x20000;
    
    //  標準テクスチャ領域の確保
    
    //  まずは最も小さい8x8サイズの４色パレットテクスチャ用の領域を確保してみる
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,FALSE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    //   上位アドレスから取られていく
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  適当にいくつかの領域確保
    mem = NNS_GfdAllocTexVram(16 * 16 / 2,FALSE,0);
    vramFreeSize[0] -= 16 * 16 / 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 16 * 16 / 2);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    mem = NNS_GfdAllocTexVram(32 * 16,FALSE,0);
    vramFreeSize[0] -= 32 * 16;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 32 * 16);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    mem = NNS_GfdAllocTexVram(64 * 128,FALSE,0);
    vramFreeSize[0] -= 64 * 128;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //  空き領域以上の要求をしてみる
    //mem = NNS_GfdAllocTexVram(2048 * 2048,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //  今の状態をstate_に保存する
    NNS_GfdGetFrmTexVramState(&state_);
    //  適当に領域確保
    mem = NNS_GfdAllocTexVram(64 * 128,FALSE,0);
    //   state_の状態をマネージャに設定。元に戻っているかどうかをチェック
    NNS_GfdSetFrmTexVramState(&state_);
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    
    //   領域を解放する
    NNS_GfdResetFrmTexVramState();
    vramFreeSize[0] = 0x20000;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    
    //   また領域を確保
    mem = NNS_GfdAllocTexVram(64 * 256,FALSE,0);
    vramFreeSize[0] -= 64 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    {
        // サイズゼロの領域を確保する
        // テクスチャキーが表現できる最小サイズに切り上げられて確保される
        mem = NNS_GfdAllocTexVram( 0,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // 最小サイズより小さいサイズの確保
        mem = NNS_GfdAllocTexVram( 1,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // 最小サイズぴったりのサイズの確保
        mem = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // 最小サイズより少し大きいサイズの確保
        mem = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN + 1,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN * 2 );
        NNS_GfdResetFrmTexVramState();
    }
}


//------------------------------------------------------------------------------
//   スロット０，１についていろいろなパターンでテストをする
//   スロット２，３がある場合はそれを使いきった状態でテストする
//------------------------------------------------------------------------------
static void testSlot01(int numSlot) 
{
    
    //
    // マネージャの管理スロット数によって、空き領域の検索順が変更になるので、
    // 以下のテストは 管理スロット数 > 2 の場合のみ成立することに注意してください。
    //
    NNS_GFD_ASSERT( numSlot > 2 );

    //   領域を解放する
    NNS_GfdResetFrmTexVramState();
    //  スロット２,３がある場合はそれを使い切る
    exhaustSlot23(numSlot);
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    
    //  標準テクスチャ領域の確保
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[0] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  スロット０の空き領域だけでは足りないような領域を確保してみる
    //  (スロット０には領域はとられず,１にのみとられる)
    mem = NNS_GfdAllocTexVram(256 * (256 + 32),FALSE,0);  // スロット０を半分より多く使う
    mem = NNS_GfdAllocTexVram(256 * 256,FALSE,0);  // スロット１のリージョン２は全てつかう
    vramFreeSize[0] -= 256 * (256 + 32);
    vramFreeSize[2] -= 256 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 - 256 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   スロット１のリージョン１の領域も通常テクスチャ用に確保
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[1] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 - 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   4x4圧縮テクスチャ用の領域を確保する
    //   リージョン１に通常テクスチャと圧縮テクスチャのパレットインデックスが共存する
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    vramFreeSize[1] -= 8 * 8 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //  空き領域以上の要求をしてみる
    //mem = NNS_GfdAllocTexVram(2048 * 2048,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   領域を解放する
    NNS_GfdResetFrmTexVramState();
    //  スロット２,３がある場合はそれを使い切る
    exhaustSlot23(numSlot);
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //   スロット０，１をフルに使うような領域の確保(これは確保できない)
    //mem = NNS_GfdAllocTexVram(512 * 512,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   領域を解放する
    NNS_GfdResetFrmTexVramState();
    //  スロット２,３がある場合はそれを使い切る
    exhaustSlot23(numSlot);
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    
    //   4x4圧縮テクスチャ用に領域を確保
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    vramFreeSize[1] -= 8 * 8 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    //   標準フォーマットのテクスチャと違い下位アドレスからとられることに注意
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //    標準フォーマットのテクスチャ領域確保を挟んで
    mem = NNS_GfdAllocTexVram(64 * 256,FALSE,0);
    vramFreeSize[0] -= 64 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x20000 - 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //    また4x4圧縮テクスチャ用に領域を確保
    mem = NNS_GfdAllocTexVram(16 * 128 / 4,TRUE,0);
    vramFreeSize[0] -= 16 * 128 / 4;
    vramFreeSize[1] -= 16 * 128 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 16 * 128 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //    スロット０を全て使って
    mem = NNS_GfdAllocTexVram(0x20000 - 8 * 8 / 4 - 64 * 256 - 16 * 128 / 4,FALSE,0);
    vramFreeSize[0] = 0;
    
    //    スロット１のリージョン２は全て使う
    mem = NNS_GfdAllocTexVram(256 * 256,FALSE,0);
    vramFreeSize[2] -= 256 * 256;
    
    //   スロット１のリージョン１に領域を確保
    mem = NNS_GfdAllocTexVram(128 * 256,FALSE,0);
    vramFreeSize[1] -= 128 * 256;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 - 128 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 128 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //   下位アドレスから確保される圧縮テクスチャのインデックステーブルとぶつかって取れないはず
    //mem = NNS_GfdAllocTexVram(128 * 256,FALSE,0);
    //NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 0);
    
    //  今の状態をstate_に保存する
    NNS_GfdGetFrmTexVramState(&state_);
    //  適当に領域確保
    mem = NNS_GfdAllocTexVram(8 * 8,FALSE,0);
    //   state_の状態をマネージャに設定。元に戻っているかどうかをチェック
    NNS_GfdSetFrmTexVramState(&state_);
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    
}


//------------------------------------------------------------------------------
//   スロット０，１，２についていろいろなパターンでテストをする
//   スロット３がある場合はそれを使いきった状態でテストする
//------------------------------------------------------------------------------
static void testSlot012(int numSlot) {

    //   領域を解放する
    NNS_GfdResetFrmTexVramState();
    //  スロット３がある場合はそれを使い切る
    exhaustSlot3(numSlot);
    
    //   スロット２がらみのテストをした後,スロット２を使い切った状態でスロット０,１のテストをする
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    vramFreeSize[3] = 0x20000;
    
    //  スロット２に標準テクスチャ領域の確保
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[3] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    mem = NNS_GfdAllocTexVram(256 * (256 + 64),FALSE,0);
    vramFreeSize[3] -= 256 * (256 + 64);
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * (256 + 64));
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   スロット０を全てつかう
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[0] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   スロット２は256 * (128 + 256 + 64)使われているので,
    //   256*248を標準テクスチャ形式で確保しようとするとスロット１のリージョン２からとられる
    mem = NNS_GfdAllocTexVram(256 * 248,FALSE,0);
    vramFreeSize[2] -= 256 * 248;
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 + vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 248);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  スロット２は256*64空いていて，スロット０は全てつかわれていて
    //  スロット１のリージョン２は256*8空いている
    
    //  これは確保できる
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[2] -= 8 * 8 / (4 * 4) * 2;
    vramFreeSize[3] -= 8 * 8 / 4;
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //   これはスロット１のリージョン２が足りなくて確保できない
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //mem = NNS_GfdAllocTexVram(256 * 64 / 4,TRUE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   今256*128の領域を標準テクスチャ形式で取ろうとするとスロット１のリージョン１からとられる
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[1] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x20000 + vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  スロット２を使い切った状態でスロット０，１のテスト
    testSlot01(numSlot);
}


//------------------------------------------------------------------------------
//   スロット０，１，２，３についていろいろなパターンでテストをする
//------------------------------------------------------------------------------
static void testSlot0123() {
  
    //   スロット３がらみのテストをした後,スロット３を使い切った状態でスロット０，１，２のテストをする
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    vramFreeSize[3] = 0x20000;
    vramFreeSize[4] = 0x20000;
    
    //  スロット３に標準テクスチャ領域の確保
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[4] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(4) == vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x60000 + vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    mem = NNS_GfdAllocTexVram(256 * (256 + 64),FALSE,0);
    vramFreeSize[4] -= 256 * (256 + 64);
    NNS_GFD_ASSERT(calcFreeSize(4) == vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x60000 + vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * (256 + 64));
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   スロット２，０を全てつかう
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[3] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    // マネージャの状態をデバック出力します
    NNS_GfdDumpFrmTexVramManager();
    
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[0] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
    //    
    //   スロット３に空き領域があっても，4x4圧縮テクスチャ用の領域は確保できない
    //mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //  スロット３を使い切った状態でスロット０，１，２のテスト
    testSlot012(4);
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  サンプルのメイン関数
                
  Arguments:    なし
                
  Returns:      なし
  
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OS_Init();
        
    //  ＶＲＡＭを４つ(スロットは０、１、２、３)割り当てた場合
    {
        const char* vramName[1] = {"VRAM-A,B,C,D"};
        const GXVRamTex vramEnum[1] = {GX_VRAM_TEX_0123_ABCD};
        int i;
        
        
        //  VRAM-A～Dの内の四つセットにそれぞれ割り当てて調べる
        OS_Printf("\nallocate slot 0,1,2,3\n");
        for(i = 0;i < 1;i ++) {
            OS_Printf("%s  checking\n",vramName[i]);
            GX_SetBankForTex(vramEnum[i]);
            
            //
            // マネージャの初期化
            //
            NNS_GfdInitFrmTexVramManager(4,TRUE);
            
            //  スロット０，１，２，３のテスト
            testSlot0123();
            
            NNS_GfdResetFrmTexVramState();
            (void)GX_DisableBankForTex();
            
            OS_Printf("%s  OK\n",vramName[i]);
        }
    }
    
    while(1) {}
}



