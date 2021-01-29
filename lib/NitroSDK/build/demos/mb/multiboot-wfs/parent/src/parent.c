/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     parent.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: parent.c,v $
  Revision 1.1  2005/06/23 09:06:03  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"
#include    "wc.h"

#include    "util.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

/* �]���ݒ�̒��� */
static BOOL is_parent_sync = FALSE;
static int parent_packet_size = parent_packet_max;


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  �e�@/�q�@ �I����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeSelect(void)
{
    KeyInfo key[1];
    int     i;
    int     file_mode = 0;

    enum
    {
        mode_default, mode_shared, mode_max
    };
    static const char *(mode_name[]) =
    {
    "use child's own FS", "share parent's FS",};

    InitFrameLoop(key);
    while (WcGetStatus() == WC_STATUS_READY)
    {
        WaitForNextFrame(key);

        PrintString(3, 10, COLOR_WHITE, "Press A to connect as PARENT");
        PrintString(2, 3, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(2, 4, COLOR_WHITE, "child's FS mode:");
        for (i = 0; i < mode_max; ++i)
        {
            const BOOL focused = (i == file_mode);
            PrintString(5, 5 + i, focused ? COLOR_YELLOW : COLOR_WHITE,
                        "%c%s", focused ? '>' : ' ', mode_name[i]);
        }
        PrintString(5, 5 + mode_max, COLOR_GRAY, " (overlay is always own.)");

        /*
         * �㉺�{�^���Ńt�@�C���V�X�e���̃��[�h�����L�����ꂩ�ɑI�����܂�.
         *   �EDS �_�E�����[�h�v���C�q�@�v���O�������g�����t�@�C���V�X�e��
         *   �E�e�@�v���O���������t�@�C���V�X�e��
         * �O�҂��ʏ�̃��[�h�Ɉʒu�Â����܂���, �e�@�Ǝq�@�̊Ԃ�
         * �قƂ�ǂ̃f�[�^���\�[�X�����L����A�v���P�[�V�����ł����
         * ��҂��̗p���� ROM �e�ʂ������I�ɍ팸���邱�Ƃ��ł��܂�.
         *
         * ��������������̏ꍇ�ɂ�, �I�[�o�[���C��񂾂���
         *   ��Ɏq�@�v���O�������g�̂��̂�񋟂��܂�.
         *   �I�[�o�[���C�܂ŋ��L����悤�ȃA�v���P�[�V�����ł����
         *   �N���[���u�[�g�Ƃ��č\������K�v������܂�.
         */
        if ((key->trg & PAD_KEY_DOWN) && (++file_mode >= mode_max))
        {
            file_mode -= mode_max;
        }
        if ((key->trg & PAD_KEY_UP) && (--file_mode < 0))
        {
            file_mode += mode_max;
        }

        /*
         * A �{�^���Őe�@�Ƃ��Ẵ��C�����X�������J�n���܂�.
         * WFS �� WM_SetPortCallback() �𗘗p���ċ쓮���邽��,
         * WM_Initialize() �̌�� WFS_Init() ���Ăяo���ĉ�����.
         */
        if (key->trg & PAD_BUTTON_A)
        {
            /*
             * �e�@�ł���ꍇ�̂�, �e�[�u�����[�h�͌��ʓI�ł�.
             * �q�@�� WFS �̒��œ��I�m�ۂ��ꂽ���������
             * �e�@���̃t�@�C���V�X�e���e�[�u������ێ����邽��,
             * ���������Ƀe�[�u�����[�h���邱�ƂɈӖ��͂���܂���.
             */
            static BOOL table_loaded = FALSE;
            if (!table_loaded)
            {
                u32     need_size = FS_GetTableSize();
                void   *p_table = OS_Alloc(need_size);
                SDK_ASSERT(p_table != NULL);
                table_loaded = FS_LoadTable(p_table, need_size);
                if (!table_loaded && p_table)
                {
                    OS_Free(p_table);
                }
            }
            WcInit(def_parent_param, FALSE, TRUE, 1);
            WcSetBlockTransferCallback(WCCallbackForWFS);
            WcStartParent();
            /*
             * �e�@�� WFS ���N�����ăt�@�C���T�[�o�ɂȂ�܂�.
             * DS �_�E�����[�h�v���C�q�@���v������t�@�C����I�[�o�[���C��
             * �S�� WFS �������o�R���Đe�@���玩���I�ɒ񋟂���܂�.
             */
            {
                FSFile  file[1];
                FS_InitFile(file);
                if (!FS_OpenFile(file, "data/main.srl"))
                {
                    OS_TPanic("failed to open DS-downloaded program file!");
                }
                WFS_InitParent(port_wbt, StateCallbackForWFS, AllocatorForWFS,
                               NULL, parent_packet_max, file, (file_mode == mode_shared));
                WFS_EnableSync(0);
                (void)FS_CloseFile(file);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  �e�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeParent(void)
{
    KeyInfo key[1];

    InitFrameLoop(key);
    while (WcGetStatus() == MODE_PARENT)
    {

        int     parent_packet_size = WFS_GetPacketSize();
        BOOL    is_parent_sync = ((WFS_GetSyncBitmap() & ~1) == (WFS_GetCurrentBitmap() & ~1));

        WaitForNextFrame(key);

        PrintString(8, 1, COLOR_WHITE, "Parent mode");
        PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");


        {
            /*
             * �㉺�Ńp�P�b�g�T�C�Y�؂�ւ�.
             *
             * ����͒P���ɓ]�����x�։e����^���܂�.
             * ���ύX���_�ŃA�N�Z�X���̏����ɂ�, �ύX�͔��f����܂���.
             */
            int     new_packet_size = parent_packet_size;

            if (key->trg & PAD_KEY_UP)
            {
                new_packet_size += 2;
                if (new_packet_size > parent_packet_max)
                    new_packet_size = WBT_PACKET_SIZE_MIN + 2;
            }
            if (key->trg & PAD_KEY_DOWN)
            {
                new_packet_size -= 2;
                if (new_packet_size < WBT_PACKET_SIZE_MIN + 2)
                    new_packet_size = (parent_packet_max & ~1);
            }
            if (parent_packet_size != new_packet_size)
            {
                parent_packet_size = new_packet_size;
                WFS_SetPacketSize(parent_packet_size);
            }
        }

        /*
         * A �{�^���œ������[�h�ֈڍs.
         *
         * ����ɂ��, �����w�肵���q�@�Q�̂������ł��v�����͂��Ȃ�������
         * �撅�̎q�@���A�N�Z�X���u���b�L���O����܂�.
         * �����g�ݍ��킹�̃t�@�C���𓯂������ŗv������邱�Ƃ�
         * ���炩�ł���ꍇ, ���̐ݒ�ɂ���ĒʐM������������܂�.
         *
         * ���̐ݒ��, �q�@�Q�̃A�N�Z�X�v�����H����������_��
         * �x�����̃f�o�b�O�o�͂ƂƂ��Ɏ����Ń��Z�b�g����܂�.
         */
        if (key->trg & PAD_BUTTON_A)
        {
            is_parent_sync = !is_parent_sync;
            WFS_EnableSync(is_parent_sync ? WFS_GetCurrentBitmap() : 0);
        }

        if (key->trg & PAD_BUTTON_START)
        {
            WcEnd();
        }

        /* ���݂̐ݒ�, �ڑ��󋵂�\�� */
        PrintString(3, 4, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(3, 5, COLOR_WHITE, "parent packet ... %d BYTE", parent_packet_size);

        PrintString(3, 7, COLOR_GREEN, "<toggle A button>");
        PrintString(3, 8, COLOR_WHITE, "sync mode     ... %s",
                    is_parent_sync ? "ENABLE" : "DISABLE");

        PrintString(3, 10, COLOR_GREEN, "bitmap status");
        PrintString(5, 11, COLOR_GREEN, "-  not connected");
        PrintString(5, 12, COLOR_GREEN, "+  idle");
        PrintString(5, 13, COLOR_GREEN, "*  busy");
        PrintString(5, 14, COLOR_GREEN, "!  sync-blocking");

        PrintString(12, 15, COLOR_BLUE, "0123456789ABCDEF");
        {
            int     i;
            const int cur_bitmap = WFS_GetCurrentBitmap();
            const int busy_bitmap = WFS_GetBusyBitmap();
            const int sync_bitmap = WFS_GetSyncBitmap();
            for (i = 0; i < sizeof(u16) * 8; ++i)
            {
                char    c;
                const int bit = (1 << i);
                if ((bit & busy_bitmap) != 0)
                    c = '*';
                else if ((bit & cur_bitmap) != 0)
                    c = '+';
                else
                    c = '-';
                PrintString(12 + i, 16, COLOR_WHITE, "%c", c);
                if ((bit & sync_bitmap) != 0)
                    PrintString(12 + i, 17, COLOR_WHITE, "!");
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
