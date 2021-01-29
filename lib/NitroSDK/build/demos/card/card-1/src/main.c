/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - demos - card-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.15  2007/08/22 08:31:07  yosizaki
  add CARD_BACKUP_TYPE_FLASH_64MBITS

  Revision 1.14  2007/02/23 01:30:07  yosizaki
  support CARD_BACKUP_TYPE_FLASH_16MBITS.

  Revision 1.13  2006/04/03 01:58:59  yosizaki
  add CARD_BACKUP_TYPE_FLASH_8MBITS.

  Revision 1.12  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.11  2005/11/21 10:18:58  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer �̍폜

  Revision 1.10  2005/10/20 07:47:05  yosizaki
  add CARD_BACKUP_TYPE_FLASH_4MBITS.

  Revision 1.9  2005/09/02 07:39:23  yosizaki
  add CARDResult codes.

  Revision 1.8  2005/07/05 00:58:03  yosizaki
  add FRAM.

  Revision 1.7  2005/06/15 09:33:16  yosizaki
  add CARD_BACKUP_TYPE_EEPROM_512KBITS.

  Revision 1.6  2005/04/06 11:17:13  yosizaki
  small fix about CARD_GetResultCode().

  Revision 1.5  2005/04/05 07:24:32  yosizaki
  change around CARD_IdentifyBackup().

  Revision 1.4  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 08:02:11  yosizaki
  copyright �̔N���C��.

  Revision 1.2  2005/02/18 07:30:10  yosizaki
  fix around hidden warnings.

  Revision 1.1  2004/09/09 12:50:30  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/card.h>
#include <nitro/fs.h>


/* ��ʕ`��p�̃��[�`�� */
#include "draw.h"


/* CARD ���C�u�����p�ϐ� ******************************************************/

/*
 * CARD �o�b�N�A�b�v�̃��b�N�p ID.
 * CARD �n�[�h�E�F�A���\�[�X�� FS ���C�u�����ȂǂƔr���g�p���邽��
 * CARD_LockBackup() / CARD_UnlockBackup() �Ŏg�p���܂�.
 */
static u16 card_lock_id;

/*
 * CARD �A�N�Z�X�֐��̃G���[���̌��ʒl.
 * CARD_GetResultCode() �̕Ԃ�l�� CARD_UnlockBackup() ���ŕύX�����̂�,
 * �A�N�Z�X�����ƃG���[����������Ă���ꍇ�ɂ͂��̓_�ɒ��ӂ��Ă�������.
 */
static CARDResult last_result = CARD_RESULT_SUCCESS;

/*
 * �������݃e�X�g�����s���ꂽ���̃t���O.
 */
static BOOL is_test_run;


/* �T���v���f�� �֐� **********************************************************/

/*---------------------------------------------------------------------------*
  Name:         GetPadDown

  Description:  �����^�C�~���O�̃L�[���͂��擾.

  Arguments:    None.

  Returns:      �O��Ăяo�����獡��Ăяo���܂ł̊Ԃɉ������ꂽ�L�[�̃t���O.
 *---------------------------------------------------------------------------*/
static u32 GetPadDown(void)
{
    static u32 btn = 0;
    static u32 btn_bak = 0;

    btn_bak = btn;
    btn = PAD_Read();
    return (u32)(~btn_bak & btn);
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
}

/*---------------------------------------------------------------------------*
  Name:         InitApp

  Description:  OS ��, �A�v���P�[�V������{�����̏�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitApp(void)
{
    void   *tmp;
    OSHeapHandle hh;

    /* OS �̏����� */
    OS_Init();
    OS_InitThread();
    OS_InitTick();
    /* �A���[�i�̏����� */
    tmp = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tmp);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
        OS_Panic("ARM9: Fail to create heap...\n");
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    /* ���荞�݂�L���� */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         GetCardResultString

  Description:  CARD �֐��̌��ʒl�𕶎���Ŏ擾.

  Arguments:    result           CARDResult ���ʒl.

  Returns:      ���ʒl��������镶����ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
static const char *GetCardResultString(CARDResult result)
{
    switch (result)
    {
    case CARD_RESULT_SUCCESS:
        return "success";
    case CARD_RESULT_FAILURE:
        return "failure";
    case CARD_RESULT_INVALID_PARAM:
        return "invalid param";
    case CARD_RESULT_UNSUPPORTED:
        return "unsupported";
    case CARD_RESULT_TIMEOUT:
        return "timeout";
    case CARD_RESULT_CANCELED:
        return "canceled";
    case CARD_RESULT_NO_RESPONSE:
        return "no response";
    case CARD_RESULT_ERROR:
        return "error";
    default:
        return "unknown error";
    }
}

/*---------------------------------------------------------------------------*
  Name:         SelectDevice

  Description:  �f�o�C�X�I�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SelectDevice(void)
{
    /* *INDENT-OFF* */
    static const struct
    {
        CARDBackupType type;
        const char *comment;
    }
    types_table[] =
    {
        { CARD_BACKUP_TYPE_EEPROM_4KBITS,   "EEPROM    4 kb"},
        { CARD_BACKUP_TYPE_EEPROM_64KBITS,  "EEPROM   64 kb"},
        { CARD_BACKUP_TYPE_EEPROM_512KBITS, "EEPROM  512 kb"},
        { CARD_BACKUP_TYPE_FLASH_2MBITS,    "FLASH     2 Mb"},
        { CARD_BACKUP_TYPE_FLASH_4MBITS,    "FLASH     4 Mb"},
        { CARD_BACKUP_TYPE_FLASH_8MBITS,    "FLASH     8 Mb"},
        { CARD_BACKUP_TYPE_FLASH_16MBITS,   "FLASH    16 Mb"},
        { CARD_BACKUP_TYPE_FLASH_64MBITS,   "FLASH    64 Mb"},
        { CARD_BACKUP_TYPE_FRAM_256KBITS,   "FRAM    256 kb"},
    };
    /* *INDENT-ON* */
    enum
    { types_table_max = sizeof(types_table) / sizeof(*types_table) };

    int     cur = 0;
    BOOL    error = FALSE;
    BOOL    end = FALSE;
    int     i;

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();
        ClearFrame(RGB555_BLACK);

        /* �㉺�L�[�ŃJ�[�\���ړ� */
        if (btn_down & PAD_KEY_DOWN)
        {
            error = FALSE;
            if (++cur >= types_table_max)
                cur -= types_table_max;
        }
        if (btn_down & PAD_KEY_UP)
        {
            error = FALSE;
            if (--cur < 0)
                cur += types_table_max;
        }

        /*
         * A �{�^���Ō��݂̑I���f�o�C�X�ɓ���.
         *
         * ���C�u�����͎w��̃f�o�C�X�����ۂɓ��ڂ���Ă��邩
         * ���f�ł��܂���̂�, �������w�肵�Ă�������.
         */
        if (btn_down & PAD_BUTTON_A)
        {
            CARD_LockBackup((u16)card_lock_id);
            end = CARD_IdentifyBackup(types_table[cur].type);
            if (!end)
            {
                error = TRUE;
                last_result = CARD_GetResultCode();
            }
            CARD_UnlockBackup(card_lock_id);
        }

        /* ������\�� */
        SetTextColor(RGB555_GREEN);
        DrawText(10, 40, "select device!");
        for (i = 0; i < types_table_max; ++i)
        {
            DrawText(20, 60 + 10 * i, types_table[i].comment);
        }
        FillRect(10, 60, 10, i * 10, RGB555_BLACK);
        DrawText(10, 60 + 10 * cur, ">");
        if (error)
        {
            /* �G���[�\�� */
            SetTextColor(RGB555_RED);
            DrawText(10, 160, "error!");
            DrawText(10, 170, "result:\"%s\"", GetCardResultString(last_result));
        }
        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         TestWriteAndVerify

  Description:  �������݃e�X�g���.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void TestWriteAndVerify(void)
{
    const u32 page_size = CARD_GetBackupPageSize();
    const u32 sector_size = CARD_GetBackupSectorSize();
    const u32 total_size = CARD_GetBackupTotalSize();

    OSTick  erase_time = 0;
    u32     pos = 0;

    BOOL    end = FALSE;

    /* �e�����\�� */
    DrawBegin();
    ClearFrame(RGB555_BLACK);
    SetTextColor(RGB555_WHITE);
    if (CARD_IsBackupEeprom())
    {
        DrawText(10, 10, "EEPROM");
    }
    else if (CARD_IsBackupFlash())
    {
        DrawText(10, 10, "FLASH");
    }
    else if (CARD_IsBackupFram())
    {
        DrawText(10, 10, "FRAM");
    }
    DrawText(10, 20, "page:%d  sector:%d", page_size, sector_size);
    DrawText(10, 30, "total:%d", total_size);
    FillRect(10, 50, 256, 100, RGB555_BLACK);
    SetTextColor(RGB555_BLUE);
    MoveTo(0 + 8, 45);
    LineTo(256 - 8, 45);
    LineTo(256 - 8, 182 - 8);
    LineTo(0 + 8, 182 - 8);
    LineTo(0 + 8, 45);
    DrawEnd();

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();

        /* �J�n�O�Ȃ� A �{�^���ŊJ�n */
        if (!is_test_run)
        {
            if (btn_down & PAD_BUTTON_A)
            {
                is_test_run = TRUE;
            }
            SetTextColor(RGB555_WHITE);
            DrawText(10, 50, "press A-BUTTON to test");
        }

        /* B �{�^���Ńe�X�g���~ */
        if (btn_down & PAD_BUTTON_B)
        {
            end = TRUE;
        }

        /* �e�X�g�� */
        if (is_test_run)
        {

            static u8 tmp_buf[65536];
            SDK_ASSERT(sizeof(tmp_buf) >= sector_size);

            FillRect(10, 50, 256 - 10 * 2, 100, RGB555_BLACK);

            SetTextColor(RGB555_GREEN);
            DrawText(10, 50, "now testing...");
            DrawText(10, 60, "address:%d-%d", pos, pos + page_size);

            /*
             * �ȍ~�̃o�b�N�A�b�v�A�N�Z�X�̂��߂� CARD ���\�[�X�����b�N����.
             *
             * ������ CARD_UnlockBackup() ���ĂԂ܂ł̊�
             * FS �ȂǑ��̃��W���[������� CARD �A�N�Z�X��
             * �u���b�L���O����邱�ƂɂȂ�܂�.
             * �����̃f�b�h���b�N�ɒ��ӂ��Ă�������.
             */
            CARD_LockBackup((u16)card_lock_id);
            {
                OSTick  tick;

                int     i;

                for (i = 0; i < page_size; ++i)
                {
                    tmp_buf[i] = (u8)(pos * 3 + i);
                }

                tick = OS_GetTick();

                /* �񓯊��ŏ������� + ���e�m�F���� */
                if (CARD_IsBackupEeprom())
                {
                    CARD_WriteAndVerifyEepromAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_WriteAndVerifyFramAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                }
                else if (CARD_IsBackupFlash())
                {
                    // �قƂ�ǂ�FLASH�f�o�C�X��Write������g�p�\�ł�
                    if (CARD_GetCurrentBackupType() != CARD_BACKUP_TYPE_FLASH_64MBITS)
                    {
                        CARD_WriteAndVerifyFlashAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                    }
                    // �ꕔ�̑�e��FLASH��EraseSector�����Program����̂ݎg�p�\�ł��B
                    // ��������̓Z�N�^�T�C�Y�̐����{�P�ʂ݂̂Ƃ����_�ɒ��ӂ��Ă��������B
                    else
                    {
                        BOOL    programmable = FALSE;
                        if ((pos % sector_size) == 0)
                        {
                            // ���ꂩ�珑�����ޗ̈�͎��O�ɃZ�N�^���Ə�������K�v������܂��B
                            // ���̃T���v���f���ł̓Z�N�^�S�̂̓��e�����������Ă��܂����A
                            // �Z�N�^�̈ꕔ���݂̂��������������ꍇ�ɂ͏����O�̓��e��ޔ�����
                            // ������ɍĂѓ������e���������݂Ȃ����K�v������܂��B
                            erase_time = tick;
                            programmable = CARD_EraseFlashSector(pos, sector_size);
                            tick = OS_GetTick();
                            erase_time = tick - erase_time;
                            last_result = CARD_GetResultCode();
                        }
                        else
                        {
                            // ���łɏ����ς݂̗̈�͂��̂܂܏������݉\�ł��B
                            programmable = TRUE;
                        }
                        if (programmable)
                        {
                            CARD_ProgramAndVerifyFlashAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                        }
                    }
                }

                /*
                 * ���̏�Ŋ�����҂�.
                 *
                 * �f�o�C�X��ނƎw��T�C�Y�ɂ���Ă�
                 * ���ɒ������ԃu���b�L���O����܂��̂�,
                 * �e���[�U�A�v���P�[�V�����̃t���[�����[�N�ɍ����`��
                 * �҂������H�v���Ă�������.
                 * (1 �t���[���� 1 �� CARD_TryWaitBackupAsync() �Ŕ��肷��, ��)
                 */
                (void)CARD_WaitBackupAsync();
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    /* �G���[�Ȃ�ЂƂ܂��I�� */
                    end = TRUE;
                }
                else
                {
                    /* �����Ȃ玞�Ԃ�\�� */
                    tick = OS_GetTick() - tick;
                    DrawText(10, 70, "write:%6d[ms]/%d[BYTE]", (int)OS_TicksToMilliSeconds(tick), page_size);
                    if (erase_time != 0)
                    {
                        DrawText(10, 80, "erase:%6d[ms]/%d[BYTE]", (int)OS_TicksToMilliSeconds(erase_time), sector_size);
                    }
                    /* �e�X�g�A�h���X�����ֈړ� */
                    pos += page_size;
                    if (pos + page_size > total_size)
                    {
                        end = TRUE;
                    }
                }
            }
            CARD_UnlockBackup(card_lock_id);

        }

        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ShowResult

  Description:  �e�X�g���ʕ\�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ShowResult(void)
{

    BOOL    end = FALSE;

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();

        /* A / B / START �{�^���Ŗ߂� */
        if (btn_down & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))
        {
            end = TRUE;
        }

        if (last_result == CARD_RESULT_SUCCESS)
        {
            /* �����\�� */
            SetTextColor(RGB555_WHITE);
            DrawText(10, 100, "done! (success)");
        }
        else
        {
            /* �G���[�\�� */
            SetTextColor(RGB555_RED);
            DrawText(10, 100, "error!");
            DrawText(10, 110, "result:\"%s\"", GetCardResultString(last_result));
        }

        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C�� �G���g���|�C���g.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{

    {
        /* �T���v���A�v���P�[�V�����̏����� */
        InitApp();
        InitDraw();
        SetClearColor(RGB555_CLEAR);
    }

    {
        /* CARD ���C�u�����̏����� */
        s32     ret;

        CARD_Init();
        ret = OS_GetLockID();
        if (ret == OS_LOCK_ID_ERROR)
        {
            OS_Panic("demo fatal error! OS_GetLockID() failed");
        }
        card_lock_id = (u16)ret;
    }

    /* ��ʑJ�� */
    for (;;)
    {
        /* �f�o�C�X�I����� */
        SelectDevice();
        /* �e�X�g�J�n */
        is_test_run = FALSE;
        TestWriteAndVerify();
        /* ���ʕ\�� */
        if (is_test_run)
            ShowResult();
    }

}
