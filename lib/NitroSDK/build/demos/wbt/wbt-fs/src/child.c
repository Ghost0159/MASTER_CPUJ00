/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     child.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: child.c,v $
  Revision 1.7  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/14 09:35:46  yosizaki
  change about FS_RedFile().

  Revision 1.5  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 09:26:34  yosizaki
  fix around hidden warnings.

  Revision 1.3  2004/11/16 06:36:22  yosizaki
  change to use FS_LoadOverlayImageAsync.

  Revision 1.2  2004/10/28 13:10:16  yosizaki
  fix comments.

  Revision 1.1  2004/10/25 10:19:00  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"

#include    "util.h"
#include    "wc.h"
#include    "func.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    �����萔��`
 *---------------------------------------------------------------------------*/

/* �I�[�o�[���C�e�X�g�p�z�� */
FS_EXTERN_OVERLAY(main_overlay_1);
FS_EXTERN_OVERLAY(main_overlay_2);
FS_EXTERN_OVERLAY(main_overlay_3);

static FSOverlayID ovl_id[] = {
    FS_OVERLAY_ID(main_overlay_1),
    FS_OVERLAY_ID(main_overlay_2),
    FS_OVERLAY_ID(main_overlay_3),
};

enum
{ OVL_ID_MAX = sizeof(ovl_id) / sizeof(*ovl_id) };
typedef void (*FUNC_TABLE) (char *);

const FUNC_TABLE func_table[] = {
    func_1,
    func_2,
    func_3,
};


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

/* �t�@�C������p */
static FSFile file[1];
static char tmp_buf[0x40000];

static int file_index;
static int file_max;
static char file_path[8][256];

/* �I�[�o�[���C�e�X�g�Ǘ��ϐ� */
static BOOL is_overlay_test = FALSE;
static BOOL is_overlay_loaded = FALSE;
static int current_overlay = 0;
static FSOverlayInfo ovl_info[1];


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/

static void ModeChildFile(const KeyInfo * p_key);       /* �q�@ �t�@�C���e�X�g */
static void ModeChildOverlay(const KeyInfo * p_key);    /* �q�@ �I�[�o�[���C�e�X�g */


/*---------------------------------------------------------------------------*
  Name:         CreateFileList

  Description:  �f�B���N�g������t�@�C����񋓂��܂�.

  Arguments:    pe         �ċA�I�ɋ��L���� FSDirEntry �\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CreateFileList(FSDirEntry *pe)
{
    FSFile  d;
    FS_InitFile(&d);
    if (FS_SeekDir(&d, &pe->dir_id))
    {
        while (FS_ReadDir(&d, pe))
        {
            if (file_max >= sizeof(file_path) / sizeof(*file_path))
                break;
            if (pe->is_directory)
                CreateFileList(pe);
            else
            {
                FSFile  f;
                FS_InitFile(&f);
                if (FS_OpenFileFast(&f, pe->file_id))
                {
                    if (FS_GetPathLength(&f) <= (int)sizeof(*file_path))
                    {
                        if (FS_GetPathName(&f, file_path[file_max], sizeof(*file_path)))
                        {
                            ++file_max;
                        }
                    }
                    (void)FS_CloseFile(&f);
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrepareOverlay

  Description:  ���ݑI�����Ă���I�[�o�[���C�����[�h���܂�.
                �������񓯊��œǂݍ��ޒi�K�܂Ői��,
                FS_StartOverlay() �͌Ă΂�܂���.

  Arguments:    new_overlay  �V�������[�h����I�[�o�[���C�̃C���f�b�N�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrepareOverlay(int new_overlay)
{
    /*
     * �I�[�o�[���C��ǂݍ��ލۂɂ�,
     * �̈���������鑼�̑S�ẴI�[�o�[���C��
     * �A�����[�h����K�v������܂�.
     */
    if (is_overlay_loaded)
    {
        is_overlay_loaded = FALSE;
        (void)FS_UnloadOverlayImage(ovl_info);
    }

    current_overlay = new_overlay;

    /*
     * �܂��I�[�o�[���C�����擾���܂�.
     * �e�[�u���� WFS �o�R�Ŏ�M�ς݂ł��̂�,
     * ���̊֐��͂������ɐ����Ԃ��܂�.
     */
    (void)FS_LoadOverlayInfo(ovl_info, MI_PROCESSOR_ARM9, ovl_id[current_overlay]);
    {
        /*
         * �����ăI�[�o�[���C���W���[���̈�Ɏ��f�[�^��ǂݍ��݂܂�.
         * �����͒P�� FS_LoadOverlay() �̌Ăяo���ł��\�ł���,
         * �����o�R�ł̓��������ɂȂ邽�ߔ��ɒ������Ԃ�������܂�.
         *
         * �҂����Ԃɂ��A�v���P�[�V�����t���[�����[�N���쓮����ɂ�,
         * �ȉ��̂悤��FS_LoadOverlayImageAsync()���̗p���邩�܂���
         * FS_LoadOverlay() ���Ăяo����p�X���b�h��p�ӂ���K�v������܂�.
         */
        if (!FS_LoadOverlayImageAsync(ovl_info, file))
        {
            OS_Panic("FS_LoadOverlayImageAsync() failed!");
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  �q�@ �ʐM��ʁB

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeChild(const KeyInfo * p_key)
{
    static BOOL init_flag = FALSE;
    if (!init_flag)
    {
        init_flag = TRUE;
        /* �t�@�C���e�X�g�p�I�u�W�F�N�g�̏����� */
        FS_InitFile(file);
    }
    {
        BOOL    bak_state = is_overlay_test;
        if (is_overlay_test)
            ModeChildOverlay(p_key);
        else
            ModeChildFile(p_key);
        if (bak_state != is_overlay_test)
        {
            ClearLine();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChildFile

  Description:  �q�@ �ʐM��ʂł̃t�@�C���ǂݍ��݃e�X�g�����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChildFile(const KeyInfo * p_key)
{
    int     i;

    PrintString(8, 1, COLOR_WHITE, "Child mode");
    PrintString(2, 3, COLOR_WHITE, "ReadFile test.");
    PrintString(2, 19, COLOR_WHITE, "Press B to move Overlay test.");
    PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");

    if (WFS_GetStatus() == WFS_STATE_READY)
    {

        /* �t�@�C�����X�g�𐶐� */
        if (file_max == 0)
        {
            FSFile  d;
            FSDirEntry e;
            FS_InitFile(&d);
            (void)FS_FindDir(&d, "");
            MI_WriteByte(e.name + 0, (u8)'\0');
            (void)FS_TellDir(&d, &e.dir_id);
            CreateFileList(&e);
        }

        /* �ǂݍ��ނׂ��t�@�C�� ID ��I�� */
        if (p_key->trg & PAD_KEY_UP)
        {
            if (--file_index < 0)
            {
                file_index = file_max - 1;
            }
        }
        if (p_key->trg & PAD_KEY_DOWN)
        {
            if (++file_index > file_max - 1)
            {
                file_index = 0;
            }
        }

        PrintString(3, 5, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(3, 6, COLOR_WHITE, "select testing file");
        PrintString(2, 7 + file_index, COLOR_WHITE, ">");
        for (i = 0; i < file_max; ++i)
        {
            PrintString(4, 7 + i, COLOR_WHITE, file_path[i]);
        }
        PrintString(3, 15, COLOR_GREEN, "<press A button>");

        /* �t�@�C�����[�h���󂫂̏�� */
        if (!FS_IsFile(file))
        {

            /* B �{�^���ŃI�[�o�[���C�e�X�g�� */
            if (p_key->trg & PAD_BUTTON_B)
            {
                is_overlay_test = TRUE;
                PrepareOverlay(current_overlay);
            }

            /* A �{�^���őI���t�@�C�������[�h */
            else if (p_key->trg & PAD_BUTTON_A)
            {

                if (!FS_OpenFile(file, file_path[file_index]))
                {
                    file_index = 0;
                    OS_TPrintf("No file:\"%s\"\n", file_path[file_index]);
                }
                else
                {
                    /*
                     * �t�@�C����ǂ݂ɂ����܂�.
                     *
                     * �񓯊����[�h�ɂ���Ƒ����ɏ�����Ԃ��܂���,
                     * ���̃T���v���ł͎��ۂ̓]�����x���擾���邽�߂�
                     * ���̏�œ����������s���܂�.
                     * ���ۂ̃Q�[���A�v���P�[�V�����ɂ�����
                     * ���C���X���b�h���炱�̂悤�ɌĂяo����
                     * ���ɒ����ԃQ�[���V�[�P���X���u���b�L���O����
                     * �t���[�����[�N�S�̂Ɉ��e�����o�邱�Ƃɒ��ӂ��Ă�������.
                     *
                     * �񓯊����[�h�łǂ̂悤�ɓǂݍ��ނׂ�����,
                     * �I�[�o�[���C�̃��[�h�����������Q�Ƃ�������.
                     */
                    int     ret;
                    int     bps = 0;
                    int     rest;
                    u64     milli;
                    OSTick  t;
                    t = OS_GetTick();
                    rest = (int)(FS_GetLength(file) - FS_GetPosition(file));

                    PrintString(3, 16, COLOR_WHITE, "reading... %d BYTE", rest);

                    /*
                     * WFS_End �֐����Ă΂�Ȃ����蓯�������͒��~����܂���̂�,
                     * �����łŐؒf���m�֑Ή�����ꍇ�͈ȉ��̂悤�Ƀ|�[�����O���܂�.
                     * �X���b�h�ڏ������|�[�����O�� CPU ���Ԃ��L����Ƃ����_������,
                     * ���̏����͊�{�I�� FS_ReadFile �֐��Ɠ����ł�.
                     *
                     * ���������Ƃ͕ʂ̃X���b�h�Œ���I�ɐؒf���m�����Ă���ꍇ�Ȃ�
                     * ���̂悤�ȏ����͕K�v�Ȃ�, FS_ReadFile �֐��𒼐ڎg�p�\�ł�.
                     * (WFS_End �֐������荞�݃n���h������Ăяo�����Ƃ�
                     *  �ł��Ȃ��Ƃ����_�ɒ��ӂ��Ă�������)
                     */
                    {
                        ret = FS_ReadFileAsync(file, tmp_buf, sizeof(tmp_buf));
                        while (FS_IsBusy(file))
                        {
                            if (WcGetStatus() == WC_STATUS_READY)
                            {
                                WFS_End();
                                ret = -1;
                            }
                        }
                    }
                    /*
                     * �T���v���ł͒��f���͒P�� Panic �Ƃ��Ă��܂���,
                     * �A�v���P�[�V�����ŕ��A�����Ɉڂ邱�Ƃ��\�ł�.
                     * (FS_GetResultCode �֐��� FS_RESULT_ERROR ��Ԃ��܂�)
                     */
                    if (ret != rest)
                    {
                        OS_Panic("FS_ReadFileAsync() returned invalid length!");
                    }
                    milli = OS_TicksToMilliSeconds(OS_GetTick() - t);
                    if (milli)
                    {
                        u64     d = ret * 8;
                        d *= 1000;
                        bps = (int)(d / milli);
                        PrintLine(3, 16, "read done %d BYTE", ret);
                        PrintLine(3, 17, "%d [bps]/%d [Bps]", bps, bps / 8);
                    }
                    (void)FS_CloseFile(file);
                }
            }
        }
    }

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChildOverlay

  Description:  �q�@ �ʐM��ʂł̃t�@�C���I�[�o�[���C�e�X�g�����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChildOverlay(const KeyInfo * p_key)
{
    PrintString(8, 1, COLOR_WHITE, "Child mode");
    PrintString(2, 3, COLOR_WHITE, "Overlay test.");
    PrintString(2, 19, COLOR_WHITE, "Press B to move ReadFile test.");
    PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");

    if (WFS_GetStatus() == WFS_STATE_READY)
    {

        PrintString(3, 5, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(3, 6, COLOR_WHITE, "select overlay");
        PrintString(18, 6 + current_overlay, COLOR_WHITE, ">");
        PrintString(20, 6, COLOR_WHITE, "func_1");
        PrintString(20, 7, COLOR_WHITE, "func_2");
        PrintString(20, 8, COLOR_WHITE, "func_3");

        PrintString(3, 10, COLOR_GREEN, "<press A button>");
        PrintString(3, 11, COLOR_BLUE, "(call overlay func)");

        /* �t�@�C�����[�h���󂫂̏�ԂȂ�I�[�o�[���C����\ */
        if (!FS_IsFile(file))
        {

            /* B �{�^���� ReadFile �e�X�g�� */
            if (p_key->trg & PAD_BUTTON_B)
            {
                is_overlay_test = FALSE;
            }
            else
            {
                /* �㉺�ŃI�[�o�[���C�؂�ւ� */
                int     new_overlay = current_overlay;

                if (p_key->trg & PAD_KEY_DOWN)
                {
                    if (++new_overlay >= OVL_ID_MAX)
                        new_overlay -= OVL_ID_MAX;
                }
                if (p_key->trg & PAD_KEY_UP)
                {
                    if (--new_overlay < 0)
                        new_overlay += OVL_ID_MAX;
                }
                if (current_overlay != new_overlay)
                {
                    PrepareOverlay(new_overlay);
                }
                /* A �{�^���Ō��݂̊֐����R�[�� */
                else if (p_key->trg & PAD_BUTTON_A)
                {
                    char    tmp[64];
                    (*(func_table[current_overlay])) (tmp);
                    PrintLine(3, 11, tmp);
                }
            }
        }
        else
        {
            /* �I�[�o�[���C��M���Ȃ�i����\�� */
            if (FS_IsBusy(file))
            {
                int     current, total;
                PrintLine(2, 14, "loading...%8d BYTE", FS_GetLength(file));
                if (WFS_GetCurrentDownloadProgress(&current, &total))
                {
                    PrintLine(2, 15, "progress %8d / %8d", current, total);
                }
            }
            /* ���[�h�����Ȃ�I�[�o�[���C�N�� */
            else
            {
                if (!is_overlay_loaded)
                {
                    PrintLine(2, 14, "loading done. %8d BYTE", FS_GetLength(file));
                    PrintLine(2, 15, "");
                    FS_StartOverlay(ovl_info);
                    (void)FS_CloseFile(file);
                    is_overlay_loaded = TRUE;
                }
            }
        }
    }

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
