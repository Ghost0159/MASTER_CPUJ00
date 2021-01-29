#include <nitro.h>
#include <dwc.h>

#include "init.h"
#include "dbs.h"

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_FRAME  1   // 想定するゲームフレーム（1/60を1とする）
#define AUTH_SERVER DWC_CONNECTINET_AUTH_TEST

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static DWCInetControl stConnCtrl;
static BOOL sPrintOverride;

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void InitDWC(void);
static void NetConfigMain(void);
static void StartIPMain(void);
static void StopIPMain(void);
static void GameWaitVBlankIntr(void);
static void VBlankIntr(void);

static void CheckProfMain(void);

//----------------------------------------------------------------------------
// initialized variable
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  メモリ確保関数
 *---------------------------------------------------------------------------*/
static void* AllocFunc(DWCAllocType name, u32 size, int align)
{
    void* ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();
    ptr = OS_AllocFromMain(size);
    OS_RestoreInterrupts(old);

    return ptr;
}

/*---------------------------------------------------------------------------*
  メモリ開放関数
 *---------------------------------------------------------------------------*/
static void FreeFunc(DWCAllocType name, void* ptr, u32 size)
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if (!ptr) return;

    old = OS_DisableInterrupts();
    OS_FreeToMain(ptr);
    OS_RestoreInterrupts(old);
}

/*---------------------------------------------------------------------------*
  OS_TPrintf()のコンソール出力対応
 *---------------------------------------------------------------------------*/
static char common_buffer[256];

#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif

extern void OS_TVPrintf(const char *fmt, va_list vlist);
void OS_TVPrintf(const char *fmt, va_list vlist)
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);

#ifndef SDK_FINALROM
    OS_PutString(common_buffer);
#endif

    if (sPrintOverride)
    {
        dbs_CVPrintf(NULL, fmt, vlist);
    }
}

#ifdef SDK_FINALROM
extern void OS_TPrintf(const char *fmt, ...);
void OS_TPrintf(const char *fmt, ...)
{
    va_list vlist;

    va_start(vlist, fmt);
    OS_TVPrintf(fmt, vlist);
    va_end(vlist);
}
#endif

/*---------------------------------------------------------------------------*
  Aボタン入力待ち
 *---------------------------------------------------------------------------*/
static void WaitKey(void)
{
    while(!(PAD_Read() & PAD_BUTTON_A))
    {
        GameWaitVBlankIntr();
    }
    while(PAD_Read() & PAD_BUTTON_A)
    {
        GameWaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  メインルーチン
 *---------------------------------------------------------------------------*/
void NitroMain ()
{
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    // スタック溢れチェック初期設定
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);

    Heap_Init();

    // Vブランク割り込み許可
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    // 出力初期化
    dbs_DemoInit();
    sPrintOverride = TRUE;  // OS_TPrintf()の出力をconsoleにつなぐ.

    // 表示開始
    OS_WaitVBlankIntr();    // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // DWCの初期化
    InitDWC();

    // Wi-Fiコネクション設定
    NetConfigMain();

    // インターネット接続
    StartIPMain();

    /* 【注意】認証サーバへのログイン処理は省略しています */

    // 不正文字列のチェック開始待ち
    OS_TPrintf("\nPress [A] to start profanity check.\n\n");
    WaitKey();

    // 不正文字列のチェック
    CheckProfMain();

    // 切断待ち
    OS_TPrintf("\nPress [A] to disconnect.\n\n");
    WaitKey();

    // インターネット切断
    StopIPMain();

    // 終了
    OS_DumpHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE);
    OS_TPrintf("\n");
    OS_TPrintf("*************************\n");
    OS_TPrintf("Please turn off NDS power\n");
    OS_TPrintf("*************************\n");
    while (1)
    {
        GameWaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  DWCの初期化
 *---------------------------------------------------------------------------*/
static void InitDWC(void)
{
    static u8 work[DWC_INIT_WORK_SIZE] ATTRIBUTE_ALIGN(32);

    // デバッグ表示レベル指定
    DWC_SetReportLevel(DWC_REPORTFLAG_ALL);

    // DWC初期化
    if (DWC_Init(work) == DWC_INIT_RESULT_DESTROY_OTHER_SETTING)
    {
        OS_TPrintf("Wi-Fi setting might be broken.\n");
    }

    // メモリ確保/解放関数を設定
    DWC_SetMemFunc(AllocFunc, FreeFunc);
}

/*---------------------------------------------------------------------------*
  Wi-Fiコネクション設定
 *---------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static void NetConfigMain(void)
{
    (void)FS_LoadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1));

    sPrintOverride = FALSE; // OS_TPrintf()の出力を一時的に元に戻す.
    dbs_DemoFinalize();

    DWC_SetAuthServer(AUTH_SERVER);
    (void)DWC_StartUtilityEx(DWC_LANGUAGE_JAPANESE, DWC_UTILITY_TOP_MENU_FOR_JPN);

    (void)FS_UnloadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1));

    // Vブランク割り込み許可
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    // 出力初期化
    dbs_DemoInit();
    sPrintOverride = TRUE;  // OS_TPrintf()の出力をconsoleにつなぐ.

    // 表示開始
    OS_WaitVBlankIntr();    // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();
}

/*---------------------------------------------------------------------------*
  インターネット接続
 *---------------------------------------------------------------------------*/
static void StartIPMain(void)
{
    DWC_InitInet(&stConnCtrl);
    DWC_SetAuthServer(AUTH_SERVER);
    DWC_ConnectInetAsync();

    // 接続処理
    while (!DWC_CheckInet())
    {
        DWC_ProcessInet();
        GameWaitVBlankIntr();
    }

    // 接続結果確認
    if (DWC_GetInetStatus() != DWC_CONNECTINET_STATE_CONNECTED)
    {
        OS_TPanic("Cannot connect.\n");
    }
}

static void StopIPMain(void)
{
    while(!DWC_CleanupInetAsync())
    {
        GameWaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  不正文字列チェックメインルーチン
 *---------------------------------------------------------------------------*/
// 判定用文字列(50個まで/合計で終端文字を含めて501文字まで)
static const u16 *words[] =
{
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"01234567890", // 12
    L"badword",     //  8
    L"012345678901" // 13
};

#define PROFTEST_WORDS_NUM (sizeof(words)/sizeof(u16*))

static void CheckProfMain(void)
{
    DWCProfState state;
    char prof_result[PROFTEST_WORDS_NUM];
    int  prof_badwordsnum;
    int  i;

    // 不正文字列チェック処理を開始する
    OS_TPrintf("\nStart profanity check...\n");
    if (DWC_CheckProfanityAsync(words, PROFTEST_WORDS_NUM, NULL, 0, prof_result, &prof_badwordsnum) == FALSE)
        OS_TPanic("DWC_CheckProfanityAsync failed.");

    do {
        state = DWC_CheckProfanityProcess();
        GameWaitVBlankIntr();
    } while (state == DWC_PROF_STATE_OPERATING);

    if (state == DWC_PROF_STATE_SUCCESS)
    {
        OS_TPrintf("\nSuccessed.\n");
        OS_TPrintf("Bad words: %d/%d\n", prof_badwordsnum, PROFTEST_WORDS_NUM);
        OS_TPrintf("Result   : ");
        for(i = 0; i < PROFTEST_WORDS_NUM; i++)
            OS_TPrintf("%d", prof_result[i]);
        OS_TPrintf("\n");
    }
    else
    {
        int          errorCode;
        DWCErrorType errorType;
        DWCError     lastError;
        
        lastError = DWC_GetLastErrorEx(&errorCode, &errorType);
        OS_TPrintf("\nFailed.\n");
        OS_TPrintf("Error     : %d\n", lastError);
        OS_TPrintf("Error code: %d\n", -errorCode);
        OS_TPrintf("Error type: %d\n", errorType);
    }
}

/*---------------------------------------------------------------------------*
  Vブランク待ち関数
 *---------------------------------------------------------------------------*/
static void GameWaitVBlankIntr(void)
{
    const char loopstr[] = "/|\\-";
    static u32 loopcnt = 0;
    int i;

    dbs_Print(30, 0, "%c", loopstr[((loopcnt)>>2)&3]);
    loopcnt++;

    dbs_DemoUpdate();

    // 想定するゲームフレームに合わせてVブランク待ちの回数を変える
    for (i = 0; i < GAME_FRAME; i++)
    {
        OS_WaitVBlankIntr();
    }

    Heap_Debug();
    dbs_DemoLoad();

    // スタック溢れチェック
    OS_CheckStack(OS_GetCurrentThread());
}


/*---------------------------------------------------------------------------*
  Vブランク割り込み関数
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
