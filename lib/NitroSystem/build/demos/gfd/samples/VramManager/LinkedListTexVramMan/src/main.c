/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - LinkedListTexVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/
//
// リンクドリストVRAMマネージャの内部モジュールの動作をテストします。
// 内部モジュールはテクスチャマネージャ、パレットマネージャに共通のコードです。
// 
//
#include <nitro.h>

#include <nnsys/gfd.h>
#include "gfd_demolib.h"

//
// マネージャ初期化パラメータ
//
#define SIZE_VRAMMAN        0x80000     // 管理テクスチャ領域サイズ
#define SIZE_VRAMMAN_4X4    0x21000     // 管理テクスチャ領域サイズ(4X4テクスチャ用)
#define NUM_VRAMMAN_MEMBLK  20          // 管理ブロック数(細分化された空き領域数の最大値となります。)
#define SIZE_PLTTMAN        0x18000     // 管理パレット領域サイズ

//
// 確保テストに使用する領域のサイズ
//
#define ALLOCATE_SIZE               0x4000
#define PLTT_ALLOC_SIZE_4           0x0008
#define PLTT_ALLOC_SIZE_16          0x0020
#define PLTT_ALLOC_SIZE_X           0x0023
#define PLTT_ALLOC_SIZE_4PLTTMAX    0x10000


#define VRAM_SLOT_SIZE    0x20000


#define TEX4X4_ENABLE           TRUE
#define TEX4X4_DISABLE          FALSE
#define PLTT4_TRUE              TRUE
#define PLTT4_FALSE             FALSE

/*---------------------------------------------------------------------------*
  Name:         TexAllocateTest_

  Description:  テクスチャマネージャの初期化テスト
                さまざまなケースで初期化をおこなってみる。
                正しく初期化が行われなかった場合は、マネージャ内部のアサートに失敗するはず。
                
                
  Arguments:    なし
                
  Returns:      なし
  
 *---------------------------------------------------------------------------*/
static void TexManInitTest_()
{
    const u32 szWork = NNS_GfdGetLnkTexVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    const u32 myMgrSize = 0x8000;
    u32 szNrm;
    
    NNS_GFD_NULL_ASSERT( pMgrWork );
    
    //
    // マネージャの初期化
    //
    // 通常用に1スロット以下
    {
        szNrm = myMgrSize;
        // 4x4 用に 0スロット
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
    }
    
    // 通常用に1 - ２スロット
    {
        szNrm = myMgrSize + VRAM_SLOT_SIZE;
        // 4x4 用に 0スロット
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        
        // 4x4 用に 1スロット 以下
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
    }
    
    // 通常用に2 - 3 スロット
    {
        szNrm  = myMgrSize + VRAM_SLOT_SIZE * 2;
        // 4x4 用に 0スロット
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        // 4x4 用に 1スロット 以下
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
        // 4x4 用に 2 スロット 以下
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize + VRAM_SLOT_SIZE,
                                      pMgrWork, szWork, TRUE );
    }        
    
    // 通常用に3 - 4 スロット
    {
        szNrm  = myMgrSize + VRAM_SLOT_SIZE * 3;
        // 4x4 用に 0スロット
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      0,
                                      pMgrWork, szWork, TRUE );
        // 4x4 用に 1スロット 以下
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize,
                                      pMgrWork, szWork, TRUE );
        // 4x4 用に 2 スロット 以下
        NNS_GfdInitLnkTexVramManager( szNrm,
                                      myMgrSize + VRAM_SLOT_SIZE,
                                      pMgrWork, szWork, TRUE );
    }
    
    // その他未チェックで境界値パラメータを調査
    {
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 4,
                                      0,
                                      pMgrWork, szWork, TRUE );
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 4,
                                      VRAM_SLOT_SIZE * 2,
                                      pMgrWork, szWork, TRUE );
        
        NNS_GfdInitLnkTexVramManager( VRAM_SLOT_SIZE * 3,
                                      VRAM_SLOT_SIZE * 2,
                                      pMgrWork, szWork, TRUE );
    }
    
    GfDDemo_Free( pMgrWork );
}

/*---------------------------------------------------------------------------*
  Name:         TexAllocateTest_

  Description:  テクスチャマネージャの確保テスト
                
  Arguments:    なし
                
  Returns:      なし
  
 *---------------------------------------------------------------------------*/
static void TexAllocateTest_()
{
    const u32 szWork = NNS_GfdGetLnkTexVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    
    NNS_GFD_NULL_ASSERT( pMgrWork );
    
    //
    // マネージャの初期化
    //
    NNS_GfdInitLnkTexVramManager( SIZE_VRAMMAN,
                                  SIZE_VRAMMAN_4X4,
                                  pMgrWork,
                                  szWork,
                                  TRUE );
                                  
    
    
    {
        NNSGfdTexKey texKey1, texKey2, texKey3;
    
        // スロットサイズより大きい領域の確保が成功する。
        texKey1 = NNS_GfdAllocTexVram( VRAM_SLOT_SIZE + 0x100, TEX4X4_DISABLE, 0 );
        NNS_GFD_ASSERT( texKey1 != NNS_GFD_ALLOC_ERROR_TEXKEY );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        
        
        // アロケートする
        texKey1 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        texKey2 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        texKey3 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        
        
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey1), NNS_GfdGetTexKeySize(texKey1) );
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey2), NNS_GfdGetTexKeySize(texKey2) );
        //OS_Printf("addr = %x, size = %x\n", NNS_GfdGetTexKeyAddr(texKey3), NNS_GfdGetTexKeySize(texKey3) );
        
        
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey1) == ALLOCATE_SIZE * 0 &&  
                     NNS_GfdGetTexKeySize(texKey1) == ALLOCATE_SIZE );
        
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey2) == ALLOCATE_SIZE * 1 &&  
                     NNS_GfdGetTexKeySize(texKey2) == ALLOCATE_SIZE );
                     
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey3) == ALLOCATE_SIZE * 2 &&  
                     NNS_GfdGetTexKeySize(texKey3) == ALLOCATE_SIZE );
        
        // Free が成功する
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
        
        // 再度同じサイズでアロケート
        texKey2 = NNS_GfdAllocTexVram( ALLOCATE_SIZE, TEX4X4_ENABLE, 0 );
        // 先ほどフリーした領域に確保される
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey2) == ALLOCATE_SIZE * 1 &&  
                     NNS_GfdGetTexKeySize(texKey2) == ALLOCATE_SIZE );
    
        // Free が成功する
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey3 ) == 0 );
        
        
        texKey1 = NNS_GfdAllocTexVram( ALLOCATE_SIZE * 2 , TEX4X4_ENABLE, 0 );
        NNS_GFD_ASSERT( NNS_GfdGetTexKeyAddr(texKey1) == ALLOCATE_SIZE * 0 &&  
                     NNS_GfdGetTexKeySize(texKey1) == ALLOCATE_SIZE * 2  );
        NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
        
        
        // すべてのVRAMを使い切って、確保に失敗する
        // NNS_GFD_MAX_NUM_TEX_VRAM_SLOT       4
        // NNS_GFD_MAX_NUM_TEX4x4_VRAM_SLOT    2
        {
            // 4X4
            texKey1 = NNS_GfdAllocTexVram( 0x20000, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( texKey1 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            texKey2 = NNS_GfdAllocTexVram( 0x01000, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( texKey2 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            //
            // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
            //
            // すべて使いきった。失敗するはず
            //texKey3 = NNS_GfdAllocTexVram( 0x1, TEX4X4_ENABLE, 0 );
            //NNS_GFD_ASSERT( texKey3 == NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            // Normal
            texKey3 = NNS_GfdAllocTexVram( 0x20000, TEX4X4_DISABLE, 0 );
            NNS_GFD_ASSERT( texKey3 != NNS_GFD_ALLOC_ERROR_TEXKEY );
            
            
            // 内部状態をデバック出力してみる
            NNS_GfdDumpLnkTexVramManager();
        
            
            // 開放する
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey1 ) == 0 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey2 ) == 0 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( texKey3 ) == 0 );
            
        
        }
        
        // 小さなサイズの切り上げ処理を確認します
        {
            NNSGfdTexKey    key;
            key = NNS_GfdAllocTexVram( 0, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( 1, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN  );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
            
            key = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN + 1, TEX4X4_ENABLE, 0 );
            NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(key) == NNS_GFD_TEXSIZE_MIN * 2 );
            NNS_GFD_ASSERT( NNS_GfdFreeTexVram( key ) == 0 );
        }
    }
    GfDDemo_Free( pMgrWork );
    
}

/*---------------------------------------------------------------------------*
  Name:         IsValidKey_

  Description:  パレットキーが意図する情報を持つキーか判定します。
                
  Arguments:    addr            意図するアドレス
                size            意図するサイズ
                
  Returns:      パレットキーが意図する情報を持つキーならTRUE
  
 *---------------------------------------------------------------------------*/
static BOOL IsValidKey_( NNSGfdPlttKey key, u32 addr, u32 size )
{
    const u32 kaddr = NNS_GfdGetPlttKeyAddr( key );
    const u32 ksize = NNS_GfdGetPlttKeySize( key );
    
    return (BOOL)( kaddr == addr  &&  ksize == size );
}

/*---------------------------------------------------------------------------*
  Name:         PlttAllocateTest_

  Description:  パレットマネージャの確保テスト
                
  Arguments:    なし
                
  Returns:      なし
  
 *---------------------------------------------------------------------------*/
static void PlttAllocateTest_()
{
    NNSGfdPlttKey     key1, key2, key3;
    
    const u32 szWork 
        = NNS_GfdGetLnkPlttVramManagerWorkSize( NUM_VRAMMAN_MEMBLK );
    void* pMgrWork   = GfDDemo_Allock( szWork );
    
    //
    // マネージャの初期化
    //
    NNS_GfdInitLnkPlttVramManager( SIZE_PLTTMAN, 
                                   pMgrWork,
                                   szWork,
                                   TRUE );
    // 確保、開放
    {
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
    }
    
    
    // 確保 中抜き開放 再度確保 ( ■■■ => ■□■ => ■■■ )
    {
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key3 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1,                  0    , PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_16 * 1, PLTT_ALLOC_SIZE_16 ) );
        NNS_GFD_ASSERT( IsValidKey_( key3, PLTT_ALLOC_SIZE_16 * 2, PLTT_ALLOC_SIZE_16 ) );
        
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_16 * 1, PLTT_ALLOC_SIZE_16 ) );
        
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key3 ) == 0 );
    }
    
    // 4色パレットで 0x10000 以上の領域を確保しようとして失敗してみる
    // 最大限に確保して確保に失敗してみる
    {
        // 4色パレットが参照可能な最大限の大きさで確保する
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4PLTTMAX, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4PLTTMAX ) );
        
        //
        // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
        //
        // 4色パレットはこれ以上、取れないはず
        // key2 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_TRUE, TRUE );
        // NNS_GFD_ASSERT( key2 == NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        // しかし、通常のパレットはまだ取れるはず
        key2 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_FALSE, TRUE );
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_4PLTTMAX, 0x7000 ) );
        NNS_GFD_ASSERT( key2 != NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        //
        // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
        //
        // 今度は、普通に容量不足で失敗する
        // key3 = NNS_GfdAllocPlttVram( 0x7000, PLTT4_FALSE, TRUE );
        // NNS_GFD_ASSERT( key3 == NNS_GFD_ALLOC_ERROR_PLTTKEY );
        
        // 内部状態をデバック出力してみる
        NNS_GfdDumpLnkPlttVramManager();
            
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
    }
    
    
    
    // 4色パレットアラインメントが正しく行われているか？
    //
    {
        // 4色パレット1つ分を確保
        // => 16色パレットを確保 (アラインメントのため8バイトフリーブロックが発生)
        // => 4色パレットを確保 (8バイトフリーブロックが使用されるはず)
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4, PLTT4_TRUE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        key3 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4, PLTT4_TRUE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4 ) );
        // パディングされて addr == PLTT_ALLOC_SIZE_4 とはならないはず
        NNS_GFD_ASSERT( IsValidKey_( key2, 0x10, PLTT_ALLOC_SIZE_16 ) );
        // パディングの際に生成されたフリーブロックが使用されるはず
        NNS_GFD_ASSERT( IsValidKey_( key3, PLTT_ALLOC_SIZE_4, PLTT_ALLOC_SIZE_4 ) );
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key3 ) == 0 );
        
        
        // 4色パレット2つ分を確保
        // => 16色パレットを確保 (アラインメントのためのパディングを回避できる！)
        // => 4色パレットを確保 (したがって確保されるアドレスは異なる！)
        key1 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_4 * 2, PLTT4_TRUE, TRUE );
        key2 = NNS_GfdAllocPlttVram( PLTT_ALLOC_SIZE_16, PLTT4_FALSE, TRUE );
        
        NNS_GFD_ASSERT( IsValidKey_( key1, 0, PLTT_ALLOC_SIZE_4 * 2) );
        // パディングは行われないはず
        NNS_GFD_ASSERT( IsValidKey_( key2, PLTT_ALLOC_SIZE_4 * 2, PLTT_ALLOC_SIZE_16 ) );
        
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key1 ) == 0 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key2 ) == 0 );
    }
    
    // 小さなサイズの切り上げ処理を確認します
    {
        NNSGfdPlttKey    key;
        key = NNS_GfdAllocPlttVram( 0, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( 1, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( NNS_GFD_PLTTSIZE_MIN, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN  );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
        
        key = NNS_GfdAllocPlttVram( NNS_GFD_PLTTSIZE_MIN+1, PLTT4_TRUE, TRUE );
        NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(key) == NNS_GFD_PLTTSIZE_MIN * 2 );
        NNS_GFD_ASSERT( NNS_GfdFreePlttVram( key ) == 0 );
    }
    
    GfDDemo_Free( pMgrWork );
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  サンプルのメイン関数
                
  Arguments:    なし
                
  Returns:      なし
  
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    GFDDemo_CommonInit();
    
    //
    // 基本的な関数群のテスト
    //
    {
        OS_Printf("tests-begin----------\n");
            TexManInitTest_();
            TexAllocateTest_();
            PlttAllocateTest_();
            
        OS_Printf("tests-end------------\n");
    }
    
    //
    // 無限ループ
    //
    while(TRUE){}
}

