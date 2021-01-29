/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     padkeyboard.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: padkeyboard.c,v $
  Revision 1.5  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.4  2006/02/07 05:14:17  kitase_hirotake
  PadKeyGetchar �̃E�F�C�g�����ύX

  Revision 1.3  2006/02/06 08:30:32  kitase_hirotake
  volatile �̂��Y��

  Revision 1.2  2006/01/10 04:22:10  kitase_hirotake
  PadKeySetString, PadKeyGetInputString �̒ǉ�
  �������[�h, ���[�h�ڍs�̕ύX

  Revision 1.1  2006/01/06 00:23:20  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include "padkeyboard.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define KEY_REPEAT_START    25  /* �L�[���s�[�g�J�n�܂ł̃t���[���� */
#define KEY_REPEAT_SPAN     1   /* �L�[���s�[�g�̊Ԋu�t���[���� */

/* �L�[���͏�� */
typedef struct KeyInformation
{
    u16 cnt;                        /* �����H���͒l */
    u16 trg;                        /* �����g���K���� */
    u16 up;                         /* �����g���K���� */
    u16 rep;                        /* �����ێ����s�[�g���� */
} KeyInformation;

static KeyInformation   gKey;       /* �L�[���� */

static u16  cur_key, bak_key, cur_pressed;

/* �X���b�h�֘A */
#define STACK_SIZE  1024

OSThread    keyboard_thread;

u64         keyboard_stack[STACK_SIZE / sizeof(u64)];
#define KEYBOARD_PRIO   1

/* �L�[�{�[�h�֘A */
static char console_in[INPUT_MAX];
static u8   current_select;
static int  input_mode;
static int  charactor_mode;
volatile static char    keyboard_buf[INPUT_MAX];
static u8   keyboard_cur;

static BOOL keyboard_valid;

/*---------------------------------------------------------------------------*
    �v���g�^�C�v�錾
 *---------------------------------------------------------------------------*/
static void PadKeyboard(void* arg);

/*---------------------------------------------------------------------------*
  Name:         KeyboardValid, KeyboardInvalid

  Description:  �L�[�{�[�h���̗͂L���A������؂�ւ���.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void KeyboardValid()
{
    keyboard_valid = TRUE;
}

void KeyboardInvalid()
{
    keyboard_valid = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[���͏���ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    pKey  - �ҏW����L�[���͏��\���́B

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInformation* pKey)
{
    static u16  repeat_count[12];
    int         i;
    u16         r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     /* �����g���K */
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); /* �����p�����s�[�g */
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      /* �����g���K */
            }
        }
    }

    pKey->cnt = r;  /* �����H�L�[���� */
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V �u�����N �R�[���o�b�N.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    PrintString(6, 0, COLOR_WHITE, "Pad KeyBoard Ver 05/11/15");

    FlushScreen();
    FlushDebugOutput();

    /* �L�[�{�[�h���L���Ȃ�� V �u�����N���Ƃ� keyboard �X���b�h���N���� */
    if (keyboard_valid == TRUE)
    {
        WakeupKeyboardThread();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WakeupKeyboardThread

  Description:  �L�[�{�[�h�X���b�h���N����.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void WakeupKeyboardThread(void)
{
    OS_WakeupThreadDirect(&keyboard_thread);
}

/*---------------------------------------------------------------------------*
  Name:         NextKey

  Description:  �L�[���͉�ʂ��N���A����
                ���񉟉����ꂽ�L�[���͂��擾.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
static void NextKey(void)
{
    /* �L�[�{�[�h��ʂ̃N���A */
    PadkeyClearScreen();

    /* �L�[���͏��擾 */
    KeyRead(&gKey);
}

/*---------------------------------------------------------------------------*
  Name:         InitKeyboardThread

  Description:  �L�[�{�[�h�X���b�h���쐬.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void InitKeyboardThread(void)
{
    /* OS_InitThread() ���Ă΂�Ă��Ȃ��Ȃ�A�Ăяo�� */
    if (!OS_IsThreadAvailable())
    {
        OS_InitThread();
    }

    OS_CreateThread(&keyboard_thread, PadKeyboard, (void*)0x1, keyboard_stack + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    KEYBOARD_PRIO);

    /* V�u�����N�ݒ� */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    /* �L�[�{�[�h��L���ɂ��� */
    keyboard_valid = TRUE;

    /* �L�[�{�[�h���̓X���b�h���N���� */
    WakeupKeyboardThread();
}

/*---------------------------------------------------------------------------*
  Name:         PadKeySetString

  Description:  �w�蕶��������݂̓��͂ɃZ�b�g����.

  Arguments:    string - �Z�b�g���镶����.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void PadKeySetString(char string[])
{
    MI_CpuClear8(console_in, INPUT_MAX);

    for (current_select = 0; string[current_select] != '\0'; current_select++)
    {
        console_in[current_select] = *(string + current_select);
    }

    current_select--;
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetInputString

  Description:  ���������͂����āA������󂯎��.

  Arguments:    string - �Z�b�g���镶����.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
void PadKeyGetInputString(char string[], int size)
{
    char    input_char;
    int     current = 0;

    while (1)
    {
        input_char = PadKeyGetchar();

        if (IndirectCheck(input_char))
        {
            string[current] = input_char;
            if (current < size - 1)
            {
                current++;
            }
        }

        if (input_char == KEY_RETURN)
        {
            string[current] = '\0';

            DebugOutput(">");
            DebugOutput(string);
            DebugOutput("\n");

            CurrentOutput(string);
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetcharAsync

  Description:  �p�b�h���̓f�[�^���ꕶ������(�񓯊�).

  Arguments:    None.

  Returns:      �����f�[�^.
 *---------------------------------------------------------------------------*/
char PadKeyGetcharAsync(void)
{
    static char result;
    static int  i;

    /* �L�[�{�[�h���̓o�b�t�@���Ȃ���ΕԂ� */
    if (keyboard_buf[0] == NULL)
    {
        return KEY_DONT_GET;
    }

    result = keyboard_buf[0];
    for (i = 1; i <= keyboard_cur; i++)
    {
        keyboard_buf[i - 1] = keyboard_buf[i];
    }

    keyboard_cur--;

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyGetchar

  Description:  �p�b�h���̓f�[�^���ꕶ������(����).

  Arguments:    None.

  Returns:      �����f�[�^.
 *---------------------------------------------------------------------------*/
char PadKeyGetchar(void)
{
    static char result;
    static int  i;

    /* �L�[�{�[�h���̓o�b�t�@���Ȃ���΃u���b�N */
    while (keyboard_buf[0] == NULL)
    {
        OS_Sleep(10);
    }

    result = keyboard_buf[0];
    for (i = 1; i <= keyboard_cur; i++)
    {
        keyboard_buf[i - 1] = keyboard_buf[i];
    }

    keyboard_cur--;

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         CharactorScopeCheck

  Description:  �p�b�h���̓f�[�^�ł̃��[�h�O�̕������͂���.

  Arguments:    �R�[�h�������ǂ��炾������.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
#define INCREASE    1
#define DECREASE    2
static void CharactorScopeCheck(int vary)
{
    switch (charactor_mode)
    {
    case ARABIC_NUMERALS:
        if (console_in[current_select] < 0x30)
        {
            console_in[current_select] = 0x39;  /* 9 */
        }
        else if (console_in[current_select] > 0x39)
        {
            console_in[current_select] = 0x30;  /* 0 */
        }
        break;

    case CAPITAL_LETTER:
        if (console_in[current_select] < 0x41)
        {
            console_in[current_select] = 0x5a;  /* Z */
        }
        else if (console_in[current_select] > 0x5a)
        {
            console_in[current_select] = 0x41;  /* A */
        }
        break;

    case SMALL_LETTER:
        if (console_in[current_select] < 0x61)
        {
            console_in[current_select] = 0x7a;  /* z */
        }
        else if (console_in[current_select] > 0x7a)
        {
            console_in[current_select] = 0x61;  /* a */
        }
        break;

    case CHARACTOR_SYMBOL:
        if (vary == INCREASE)
        {
            if ((console_in[current_select] > 0x2f) && (console_in[current_select] < 0x3a))
            {
                console_in[current_select] = 0x3a;
            }
            else if ((console_in[current_select] > 0x40) && (console_in[current_select] < 0x5b))
            {
                console_in[current_select] = 0x5b;
            }
            else if ((console_in[current_select] > 0x60) && (console_in[current_select] < 0x7b))
            {
                console_in[current_select] = 0x7b;
            }
            else if (console_in[current_select] > 0x7e)
            {
                console_in[current_select] = 0x21;
            }
        }
        else
        {
            if (console_in[current_select] < 0x21)
            {
                console_in[current_select] = 0x7e;
            }

            if ((console_in[current_select] > 0x2f) && (console_in[current_select] < 0x3a))
            {
                console_in[current_select] = 0x2f;
            }

            if ((console_in[current_select] > 0x40) && (console_in[current_select] < 0x5b))
            {
                console_in[current_select] = 0x40;
            }

            if ((console_in[current_select] > 0x60) && (console_in[current_select] < 0x7b))
            {
                console_in[current_select] = 0x60;
            }
        }
        break;

    default:
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyboardIndirect

  Description:  Indirect Mode �̃L�[�{�[�h���͉��.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
static void PadKeyboardIndirect(void)
{
    static int  i;
    int         key = (gKey.trg | gKey.rep);

    if (key & PAD_KEY_LEFT)
    {
        if (current_select != 0)
        {
            current_select--;
        }
    }

    if (key & PAD_KEY_RIGHT)
    {
        if (console_in[current_select + 1] != '\0')
        {
            current_select++;
        }
    }

    if (key & PAD_KEY_UP)
    {
        console_in[current_select]++;

        /* ���͕������[�h�ʂ̉�荞�ݏ��� */
        CharactorScopeCheck(INCREASE);
    }

    if (key & PAD_KEY_DOWN)
    {
        console_in[current_select]--;

        /* ���͕������[�h�ʂ̉�荞�ݏ��� */
        CharactorScopeCheck(DECREASE);
    }

    if (key & PAD_BUTTON_Y)
    {
        console_in[current_select] = ' ';
    }

    if (key & PAD_BUTTON_B)
    {
        if (current_select > 0)
        {
            i = --current_select;
            while (console_in[i] != '\0')
            {
                console_in[i] = console_in[i + 1];
                i++;
            }
        }
    }

    if (key & PAD_BUTTON_R)
    {
        /* ���͕������[�h��؂�ւ� */
        charactor_mode = (charactor_mode % MAX_CHARACTOR_MODE) + 1;

        /* ���݂̓��͕������[�h�̏����l�ɐݒ� */
        switch (charactor_mode)
        {
        case ARABIC_NUMERALS:
            console_in[current_select] = 0x30;  /* 0 */
            break;

        case CAPITAL_LETTER:
            console_in[current_select] = 0x41;  /* A */
            break;

        case SMALL_LETTER:
            console_in[current_select] = 0x61;  /* a */
            break;

        case CHARACTOR_SYMBOL:
            console_in[current_select] = 0x21;  /* ! */
            break;

        default:
            break;
        }
    }

    if (key & PAD_BUTTON_L)
    {
        input_mode = DIRECT_MODE;
    }

    if (key & PAD_BUTTON_A)
    {
        current_select++;
        console_in[current_select] = console_in[current_select - 1];
    }

    if (key & PAD_BUTTON_X)
    {
        for (i = 0; console_in[i] != '\0'; i++)
        {
            keyboard_buf[keyboard_cur] = console_in[i];
            keyboard_cur++;
        }

        keyboard_buf[keyboard_cur] = KEY_RETURN;
        keyboard_cur++;
        MI_CpuFill8(console_in, '\0', sizeof(console_in));
        current_select = 0;
        console_in[current_select] = ' ';
    }

    for (i = 0; console_in[i] != '\0'; i++)
    {
        if ((i == current_select) && (input_mode == INDIRECT_MODE))
        {
            PrintString(i + 1, 4, COLOR_RED, "%c", console_in[i]);
        }
        else
        {
            PrintString(i + 1, 4, COLOR_YELLOW, "%c", console_in[i]);
        }
    }

    if ((console_in[current_select] == '\0') || (console_in[current_select] == ' '))
    {
        PrintString(current_select + 1, 4, COLOR_RED, "_");
    }

    switch (charactor_mode)
    {
    case ARABIC_NUMERALS:
        ShowNavigater("INDIRECT MODE           NUMERAL");
        break;

    case CAPITAL_LETTER:
        ShowNavigater("INDIRECT MODE    CAPITAL LETTER");
        break;

    case SMALL_LETTER:
        ShowNavigater("INDIRECT MODE      SMALL LETTER");
        break;

    case CHARACTOR_SYMBOL:
        ShowNavigater("INDIRECT MODE            SYMBOL");
        break;

    default:
        ShowNavigater("INDIRECT MODE");
    }
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyboardDirect

  Description:  Direct Mode �̃L�[�{�[�h���͉��.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
static void PadKeyboardDirect(void)
{
    int i;
    int key = (gKey.trg | gKey.rep);

    if (key & PAD_KEY_LEFT)
    {
        keyboard_buf[keyboard_cur] = KEY_LEFT;
        keyboard_cur++;
    }

    if (key & PAD_KEY_RIGHT)
    {
        keyboard_buf[keyboard_cur] = KEY_RIGHT;
        keyboard_cur++;
    }

    if (key & PAD_KEY_UP)
    {
        keyboard_buf[keyboard_cur] = KEY_UP;
        keyboard_cur++;
    }

    if (key & PAD_KEY_DOWN)
    {
        keyboard_buf[keyboard_cur] = KEY_DOWN;
        keyboard_cur++;
    }

    if (key & PAD_BUTTON_Y)
    {
        keyboard_buf[keyboard_cur] = KEY_SPACE;
        keyboard_cur++;
    }

    if (key & PAD_BUTTON_B)
    {
        keyboard_buf[keyboard_cur] = KEY_BACKSPACE;
        keyboard_cur++;
    }

    if (key & PAD_BUTTON_R)
    {
        /* ���݃L�[������ */
    }

    if (key & PAD_BUTTON_L)
    {
        input_mode = INDIRECT_MODE;
    }

    if (key & PAD_BUTTON_A)
    {
        /* ���݃L�[������ */
    }

    if (key & PAD_BUTTON_X)
    {
        keyboard_buf[keyboard_cur] = KEY_RETURN;
        keyboard_cur++;
    }

    for (i = 0; console_in[i] != '\0'; i++)
    {
        PrintString(i + 1, 4, COLOR_YELLOW, "%c", console_in[i]);
    }

    ShowNavigater("  DIRECT MODE.");
}

/*---------------------------------------------------------------------------*
  Name:         PadKeyboard

  Description:  �p�b�h�ł̃L�[�{�[�h���͉��.

  Arguments:    None.

  Returns:      �Ȃ�.
 *---------------------------------------------------------------------------*/
static void PadKeyboard(void* arg)
{
#pragma unused(arg)

    current_select = 0;
    input_mode = INDIRECT_MODE;
    MI_CpuFill8(console_in, '\0', sizeof(console_in));
    console_in[0] = ' ';
    MI_CpuFill8((void*)keyboard_buf, '\0', sizeof(keyboard_buf));
    keyboard_cur = 0;
    charactor_mode = ARABIC_NUMERALS;

    while (1)
    {
        NextKey();

        if (input_mode == DIRECT_MODE)
        {
            PadKeyboardDirect();
        }
        else
        {
            PadKeyboardIndirect();
        }

        /* �L�[���͂��`�F�b�N������X���b�h�͖��� */
        OS_SleepThread(NULL);
    }
}
