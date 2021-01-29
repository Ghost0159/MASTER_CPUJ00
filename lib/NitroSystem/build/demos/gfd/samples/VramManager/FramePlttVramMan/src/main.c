/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - FramePlttVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/
//
// フレームVRAMマネージャ(テクスチャパレット)を使用するサンプルです。
// さまざまなパターンで領域確保を行います。
// 
#include <nitro.h>
#include <nnsys/gfd.h>


//   プログラム内で使いまわす変数
static NNSGfdFrmPlttVramState           state_;
static NNSGfdPlttKey                    mem;
static int                              vramLowerPtr,vramHigherPtr;

//  ＶＲＡＭの上位ポインタ,下位ポインタをマネージャのものと照合する
static BOOL checkVramPtr() {
     NNS_GfdGetFrmPlttVramState(&state_);
	return ((state_.address[0] == vramLowerPtr) && (state_.address[1] == vramHigherPtr));
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
    
    {
        const char* vramName[6] 
            = {"VRAM-F","VRAM-G","VRAM-F,G","VRAM-E","VRAM-E,F","VRAM-E,F,G"};
        const GXVRamTexPltt vramEnum[6] 
            = {GX_VRAM_TEXPLTT_0_F,GX_VRAM_TEXPLTT_0_G,GX_VRAM_TEXPLTT_01_FG,
               GX_VRAM_TEXPLTT_0123_E,GX_VRAM_TEXPLTT_01234_EF,GX_VRAM_TEXPLTT_012345_EFG};
        const int vramSize[6] 
            = {16 * 1024,16 * 1024,(16 + 16) * 1024,64 * 1024,(64 + 16) * 1024,(64 + 16 + 16) * 1024};
       
       
       
        //  ＶＲＡＭ組み合わせ全てのパターンをテクスチャパレットに割り当ててテスト
        int i;
        for(i = 0;i < 6;i ++) 
        {
            //  ポインタの初期値設定
            vramLowerPtr = 0;
            vramHigherPtr = vramSize[i];
            
            OS_Printf("%s  checking\n",vramName[i]);
            GX_SetBankForTexPltt(vramEnum[i]);
            
            //
            // マネージャの初期化
            //
            NNS_GfdInitFrmPlttVramManager( (u32)vramSize[i], TRUE );
            
            //   ４色パレットでない領域を下位アドレスから確保
            mem = NNS_GfdAllocPlttVram(0x100,FALSE,TRUE);
            vramLowerPtr += 0x100;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x100);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x100);
            
            //  ４色パレットでない領域を上位アドレスから確保
            mem = NNS_GfdAllocPlttVram(0x100,FALSE,FALSE);
            vramHigherPtr -= 0x100;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x100);
            
            //  ４色パレットの領域を下位アドレスから確保
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            //   ４色パレットだけアライメントが0x8なのでそれが上手くいっているか確かめる
            mem = NNS_GfdAllocPlttVram(0x10,TRUE,TRUE);
            vramLowerPtr += 0x10;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x10);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            
            // マネージャの状態をデバック出力する
            NNS_GfdDumpFrmPlttVramManager();
            
            //  アドレスが0x****8なのでここで４色パレットでないテクスチャ用に領域を確保
            //  今のアドレス+0x8のアドレスから領域が確保される
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,TRUE);
            vramLowerPtr += 0x10 + 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x10);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            
            //  0x10000を超えたサイズのＶＲＡＭを割り当てていて,
            //  かつ,0x10000以上の領域全てが確保済みでなければ
            //  ４色パレットのテクスチャ領域を上位側から確保することは出来ない
            if(vramSize[i] > 0x10000) {
                //
                // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
                //
                //   これは確保できない
                //mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
                //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
                
                //   0x10000を超える領域を全て確保する
                mem = NNS_GfdAllocPlttVram((u32)(vramSize[i] - 0x10000),FALSE,FALSE);
                vramHigherPtr -= vramSize[i] - 0x10000;
                NNS_GFD_ASSERT(checkVramPtr());
                NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
                NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == vramSize[i] - 0x10000);
            }
            
            //   上のif内で確保できなかった場合でも今度は確保できる
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            
            //   下位側のアドレスの時と同様,
            //   ４色パレットだけアライメントが0x8なのでそれが上手くいっているか確かめる
            mem = NNS_GfdAllocPlttVram(0x10,TRUE,FALSE);
            vramHigherPtr -= 0x10;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            //  アドレスが0x****8なのでここで４色パレットでないテクスチャ用に領域を確保
            //  今のアドレス-0x8のアドレスから領域が確保される
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,FALSE);
            vramHigherPtr -= 0x10 + 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            
            //
            // メモリ確保動作の失敗を確認したい場合はコメント部分を実行してみてください
            //
            //  空き容量以上の領域を要求して断られる
            //mem = NNS_GfdAllocPlttVram(vramSize[i],TRUE,FALSE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],FALSE,FALSE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],TRUE,TRUE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],FALSE,TRUE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            
            //  現在の状態をstate_に保存
            NNS_GfdGetFrmPlttVramState(&state_);
            
            //  上位側,下位側に適当な領域を確保
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,FALSE);
            mem = NNS_GfdAllocPlttVram(0x18,TRUE,FALSE);
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,TRUE);
            mem = NNS_GfdAllocPlttVram(0x18,TRUE,TRUE);
            
            //  state_に記録されているものをマネージャに戻す
            NNS_GfdSetFrmPlttVramState(&state_);
            NNS_GFD_ASSERT(checkVramPtr());
            
            
            {
                // サイズゼロ
                mem = NNS_GfdAllocPlttVram(0x0,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN より小さい
                mem = NNS_GfdAllocPlttVram(0x1,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN ちょうど
                mem = NNS_GfdAllocPlttVram(NNS_GFD_PLTTSIZE_MIN,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN よりすこし大きい
                mem = NNS_GfdAllocPlttVram(NNS_GFD_PLTTSIZE_MIN+1,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN * 2);
                NNS_GfdSetFrmPlttVramState(&state_);
            }
            
            (void)GX_DisableBankForTex();
            
            OS_Printf("%s  OK\n",vramName[i]);
        }
    }
    
    while(1) {}
}

