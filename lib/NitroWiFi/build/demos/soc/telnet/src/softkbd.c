

#include "kbd_tiny.h"

#include "softkbd.h"

#define CHAT_KEYBOARD_PLTT  0xA // キーボード用カラーパレット番号
#define BUFFER_LENGTH       128

static GXScrText32x32 kbd_scr;
static OSMutex          SoftkbdBufferMutex;
static u8       SoftkbdInputBuffer[BUFFER_LENGTH];
static s16          SoftkbdBufferTail = 0, SoftkbdBufferHead = 0;

#if 0
static void         SampleTpEventCallback(void* param, KBDTpEvent event, u16 pressed_id, u16 current_id, int x, int y);

// 追加タッチイベントリスト。メンバ group_offset は必ず0を設定して下さい。
// また、メンバ id は0x0100～0xfeffの間で自由に値を設定して下さい。
static const KBDButton  SampleTpEvent[] = {
    {
        {0, 0, 16, 16}, 0, 0x100,
    },
    {
        {240, 0, 16, 16}, 0, 0x200,
    }
};

static void SampleTpEventCallback(void* param, KBDTpEvent event, u16 pressed_id, u16 current_id, int x, int y)
{
    (void)param;
    (void)x;
    (void)y;

    OS_Printf("Event on (%d, %d) : ", x, y);

    switch (event)
    {
    case KBD_TP_DOWN:
        // タッチオン時の押されたボタンのIDを表示
        OS_Printf("TOUCH ON id %d.\n", current_id);
        break;

    case KBD_TP_UP:
        // タッチオフ時の最初に押されたボタンと離す直前に押されていたボタンのIDを表示
        OS_Printf("TOUCH OFF id %d -> id %d.\n", pressed_id, current_id);
        break;

    case KBD_TP_REPEAT:
        // リピート中のボタンのIDを表示
        if (pressed_id == current_id)
        {
            // リピートトリガ発生
            OS_Printf("TOUCH REPEAT  id %d.\n", current_id);
        }
        else
        {
            // リピートトリガ発生待ち
            OS_Printf("WAIT REPEAT.. id %d.\n", current_id);
        }
        break;

    case KBD_TP_DRAG:
        // ドラッグ中のボタンのIDを表示
        OS_Printf("DRAG id %d -> id %d.\n", pressed_id, current_id);
        break;

    default:
        break;
    }
}
#endif

static void SoftkbdReadThread(void* )
{
    static s16  posTailNext = 0;
    static u8   c;
    static u16  keyData = 0;
    while (TRUE)
    {
#ifdef WITH_KBD
        keyData = KBD_GetInput();
#else
        keyData = TinykbdGetInput();
#endif
        if (keyData > 0)
        {
            c = (u8) (keyData & 0xff);

            // 入力されたときの処理。
            //OS_Printf("%c, %02x\n", KBD_GetInput(), KBD_GetInput());
            // KBD_ConvertAll( L'は' );などで変換可能。
            if (PAD_Read() & PAD_BUTTON_L)
            {
                if (c >= 'a' && c <= 'z')
                {
                    c = (u8) (c - 'a' + 1);
                }
                else if (c >= 'A' && c <= '`')
                {
                    c = (u8) (c - 'A' + 1);
                }
            }

            OS_LockMutex(&SoftkbdBufferMutex);
            SoftkbdInputBuffer[SoftkbdBufferTail] = c;
            posTailNext = (s16) ((SoftkbdBufferTail + 1) & (BUFFER_LENGTH - 1));
            if (posTailNext != SoftkbdBufferHead)
            {
                SoftkbdBufferTail = posTailNext;
            }

            OS_UnlockMutex(&SoftkbdBufferMutex);
        }

        OS_WaitVBlankIntr();
#ifdef WITH_KBD
        KBD_Update();
        GXS_LoadBG1Char(KBD_GetCharImg(), 0, KBD_GetCharSize());
#else
        TinykbdUpdate();
#endif
    }
}

static u32 SoftkbdReadInner(void* pBuffer, u32 nLength)
{
    u32 nCopyLength;
    if (SoftkbdBufferHead < SoftkbdBufferTail)
    {
        nCopyLength = MATH_MIN(nLength, SoftkbdBufferTail - SoftkbdBufferHead);
    }
    else if (SoftkbdBufferTail < SoftkbdBufferHead)
    {
        nCopyLength = MATH_MIN(nLength, BUFFER_LENGTH - SoftkbdBufferHead);
    }
    else
    {
        return 0;
    }

    if (nCopyLength > 0)
    {
        MI_CpuCopy8((void*) &SoftkbdInputBuffer[SoftkbdBufferHead], pBuffer, nCopyLength);
        nLength -= nCopyLength;
        SoftkbdBufferHead = (s16) ((SoftkbdBufferHead + nCopyLength) & (BUFFER_LENGTH - 1));
    }

    if (nLength > 0 && nCopyLength > 0)
    {
        return SoftkbdRead((u8*)pBuffer + nCopyLength, nLength) + nCopyLength;
    }

    return nCopyLength;
}

u32 SoftkbdRead(void* pBuffer, u32 nLength)
{
    u32 length;
    OS_LockMutex(&SoftkbdBufferMutex);
    length = SoftkbdReadInner(pBuffer, nLength);
    OS_UnlockMutex(&SoftkbdBufferMutex);
    return length;
}

void SoftkbdInit(int x, int y)
{
    {
        TPCalibrateParam    param;
        (void)TP_GetUserInfo(&param);
        TP_SetCalibrateParam(&param);
    }

#ifdef WITH_KBD
    // ソフトキーボード処理全般の初期化
    KBD_Init(x, y, FALSE, NULL, NULL);
    KBD_Change(KBD_KIND_ASCII);

    // 追加タッチイベントを設定する
    //    KBD_SetExtTpEvent(SampleTpEvent, 2, SampleTpEventCallback, NULL);
    GXS_LoadBG1Char(KBD_GetCharImg(), 0, KBD_GetCharSize());

    // ソフトキーボード用スクリーンデータ作成
    KBD_FillScreen(&kbd_scr, CHAT_KEYBOARD_PLTT, 0);
    GXS_LoadBG1Scr(&kbd_scr, 0, sizeof(kbd_scr));

    // ソフトキーボード用パレットロード
    KBD_LoadPltt(CHAT_KEYBOARD_PLTT);
#else
    TinykbdInit();

#endif
    G2S_SetBG1Offset(-x, -y);

    OS_InitMutex(&SoftkbdBufferMutex);
    {
        static OSThread threadRead;
        static u8       stackThread[4096] ATTRIBUTE_ALIGN(32);

        if (!OS_IsThreadInList(&threadRead))
        {
            OS_CreateThread(&threadRead, SoftkbdReadThread, NULL, stackThread + sizeof(stackThread),
                            sizeof(stackThread), 16 - 1);
            OS_WakeupThreadDirect(&threadRead);
        }
    }
}
