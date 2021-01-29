/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     parent.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: parent.c,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:02  yosizaki
  do-indent.

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
    �����ϐ���`
 *---------------------------------------------------------------------------*/

/* �]���ݒ�̒��� */
static BOOL is_parent_sync = FALSE;
static int parent_packet_size = parent_packet_max;


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  �e�@ �ʐM��ʂł̏����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeParent(const KeyInfo * p_key)
{
    int     parent_packet_size = WFS_GetPacketSize();
    BOOL    is_parent_sync = ((WFS_GetSyncBitmap() & ~1) == (WFS_GetCurrentBitmap() & ~1));

    PrintString(8, 1, COLOR_WHITE, "Parent mode");
    PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");


    {                                  /*
                                        * �㉺�Ńp�P�b�g�T�C�Y�؂�ւ�.
                                        *
                                        * ����͒P���ɓ]�����x�։e����^���܂�.
                                        * ���ύX���_�ŃA�N�Z�X���̏����ɂ�, �ύX�͔��f����܂���.
                                        */
        int     new_packet_size = parent_packet_size;

        if (p_key->trg & PAD_KEY_UP)
        {
            new_packet_size += 2;
            if (new_packet_size > parent_packet_max)
                new_packet_size = WBT_PACKET_SIZE_MIN + 2;
        }
        if (p_key->trg & PAD_KEY_DOWN)
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
    if (p_key->trg & PAD_BUTTON_A)
    {
        is_parent_sync = !is_parent_sync;
        WFS_EnableSync(is_parent_sync ? WFS_GetCurrentBitmap() : 0);
    }

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }

    /* ���݂̐ݒ�, �ڑ��󋵂�\�� */
    PrintString(3, 4, COLOR_GREEN, "<UP & DOWN key>");
    PrintString(3, 5, COLOR_WHITE, "parent packet ... %d BYTE", parent_packet_size);

    PrintString(3, 7, COLOR_GREEN, "<toggle A button>");
    PrintString(3, 8, COLOR_WHITE, "sync mode     ... %s", is_parent_sync ? "ENABLE" : "DISABLE");

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


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
