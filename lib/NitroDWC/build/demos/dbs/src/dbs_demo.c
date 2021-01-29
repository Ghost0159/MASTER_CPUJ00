/*---------------------------------------------------------------------------*

  dbsライブラリのふつうの使い方のデモ 兼 簡易初期化インタフェイス

  - とりあえず画面に文字を出したいだけな人向けの機能です
  - サブ画面のBG0を使います
  - VRAM-Hを使います
  - VRAM-Hの32キロのうち、先頭の６キロ（0x0000～0x17FF）を使います。
  - SDKデモのDEMOInitDisplaySubBG0Only相当の働き＋dbsの初期化 をします。
  - サブ画面の下半分にコンソールをひとつ作ります
  - 例外時ハンドラが設定されます

 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include "dbs.h"
#include "dbs_demo.h"


static tDbsConsole sDemoConsole;
static char        sDemoLineBuf[ 32 * 100 ];



void dbs_DemoInit( void )
{
    GX_SetBankForSubBG( GX_VRAM_SUB_BG_32_H );
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);

    G2S_SetBG0Control( GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                       GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x00000,
                       GX_BG_EXTPLTT_01 );

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0x1000,
                      GX_BG_CHARBASE_0x00000,
                      GX_BG_EXTPLTT_01
                      );

    G2S_SetBG0Priority(0);

    G2S_BG0Mosaic(FALSE);

    dbs_Init( eDBS_INIT_SUB_BG0,
              0,  
              0xE,
              0xF,
              0x8,
              8 );

    // ユーザ例外コールバックを設定します
    OS_SetUserExceptionHandler( dbs_DisplayException, NULL );
    //---- ここでキャッシュをフラッシュするのは、
    //     例外が発生するとプロテクションユニットが変更されるので
    //     例外に関するアドレス等は確実に書き込んでおきたいから。
    DC_FlushAll();

    // コンソール作成
    dbs_CreateConsole( & sDemoConsole, sDemoLineBuf, sizeof(sDemoLineBuf), 0, 5, 32, 19 );
    
}

void dbs_DemoFinalize( void )
{
    dbs_DestroyConsole( &sDemoConsole );
}



void dbs_DemoUpdate( void )
{
    dbs_DrawConsole();
}



void dbs_DemoLoad( void )
{
    dbs_Load();
}
