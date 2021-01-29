/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.20  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.19  2005/11/21 11:06:11  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.18  2005/06/10 06:31:30  yosizaki
  fix around ModeSelect().

  Revision 1.17  2005/05/09 04:49:23  yosizaki
  fix about call of WFS_End().

  Revision 1.16  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.15  2004/10/31 23:53:26  yosizaki
  delete WC_DEFAULT_CHANNEL (abandoned symbol)

  Revision 1.14  2004/10/28 13:10:48  yosizaki
  change to use block-transfer callback of wc.

  Revision 1.13  2004/10/25 10:20:52  yosizaki
  move to parent.c and child.c

  Revision 1.12  2004/10/05 10:22:36  yosizaki
  change packet-size within 5600[us].

  Revision 1.11  2004/09/29 07:27:52  yosizaki
  change WFS_Init.

  Revision 1.10  2004/09/29 07:02:00  yosizaki
  merge from branch.

  Revision 1.9  2004/09/15 09:10:57  yosizaki
  add Seek&Read.
  add some sample test.

  Revision 1.8  2004/09/09 14:09:03  yosizaki
  add sample of WFS_GetCurrentDownloadProgress().

  Revision 1.7  2004/09/09 07:38:31  yosizaki
  fix timing of FS_LoadTable.

  Revision 1.6  2004/09/09 05:05:01  yosizaki
  total reform.

  Revision 1.5  2004/09/06 08:46:12  yosizaki
  add routine to check result of FS_ReadFile().

  Revision 1.4  2004/09/06 06:54:09  yosizaki
  small change. (debug output)

  Revision 1.3  2004/09/04 13:26:58  yosizaki
  add test ofoverlay.

  Revision 1.2  2004/09/04 01:45:41  yosizaki
  add some debug-prints.

  Revision 1.1  2004/09/03 01:11:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    WBT ���쓮���Ė����}���`�u�[�g���̎q�@���t�@�C���V�X�e����
	���p���邽�߂̃T���v���v���O�����ł��B

    HOWTO:
        1. �e�@�Ƃ��ĒʐM���J�n����ꍇ�͂`�{�^���������ĉ������B
           ���ӂɓ��� wbt-fs �f���̎q�@�������莟��A�����I�ɂ��̎q�@��
           �ʐM���J�n���܂��B�����ɒʐM�ł���q�@��15��܂łł��B
        2. �q�@�Ƃ��ĒʐM���J�n����ꍇ�͂a�{�^���������ĉ������B
           ���ӂɓ��� wbt-fs �f���̐e�@�������莟��A�����I�ɂ��̐e�@��
           �ʐM���J�n���܂��B
        3. �ڑ����̂��܂��܂ȋ@�\�ɂ��ẮA�e�@ / �q�@�̉�ʕ\����
           �\�[�X�R�[�h���̒��߂��Q�Ƃ��������B
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

/* �����ʐM�p�����[�^ */

#define		WC_PARENT_DATA_SIZE_MAX		128
#define		WC_CHILD_DATA_SIZE_MAX		256

#if	!defined(SDK_MAKEGGID_SYSTEM)
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#endif
#define GGID_WBT_FS                 SDK_MAKEGGID_SYSTEM(0x31)

// �Q�[�����
const WMParentParam def_parent_param[1] = {
    {
     NULL, 0, 0,
     GGID_WBT_FS,
     0x0000,                           // temporary ID (����+1�����)
     1,                                // �G���g���[���t���O
     15,                               // �ő�ڑ��q�@��
     0,                                // �}���`�u�[�g�t���O
     0,                                // �L�[�V�F�A�����O
     1,                                // �A�����M�t���O
     200,                              // beacon �Ԋu
     {0, 0, 0, 0},                     // ���[�U�[��
     {0, 0, 0, 0, 0, 0, 0, 0},         // �Q�[����
     1,                                // �ڑ��`�����l�� (�N������ MeasureChannel����)
     WC_PARENT_DATA_SIZE_MAX,          // �e�@���M�f�[�^�T�C�Y
     WC_CHILD_DATA_SIZE_MAX / 16       // �q�@���M�f�[�^�T�C�Y
     // Reserves
     }
};


/* ��ʑJ�ڃ��[�h */
enum
{
    MODE_SELECT,
    MODE_ERROR,
    MODE_BUSY,
    MODE_PARENT,
    MODE_CHILD,
    MODE_MAX
};


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

/* ��x�ł��q�@����I������� FS ��u��������̂Őe�@�ɂ͂Ȃ�܂��� */
static BOOL is_child = FALSE;


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS �̃R�[���o�b�N�֐�.
                ��Ԃ� WFS_STATE_READY �ɂȂ������_�ŌĂяo����܂�.
                ���̃R�[���o�b�N�Œʒm���󂯂�, �P�ɔC�ӂ̈ʒu��
                WFS_GetStatus() ���g�p���Ĕ��肷�邱�Ƃ��\�ł�.

  Arguments:    arg       �R�[���o�b�N�Ɏw�肵�����[�U��`����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void StateCallbackForWFS(void *arg)
{
    (void)arg;
    switch (WFS_GetStatus())
    {
    case WFS_STATE_READY:
        OS_TPrintf("WFS ready.\n");
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         AllocatorForWFS

  Description:  WFS �Ɏw�肷�郁�����̓��I�m�ۊ֐�.

  Arguments:    arg       �A���P�[�^�Ɏw�肵�����[�U��`����.
                size      �������m�ۗv���Ȃ�ΕK�v�ȃT�C�Y.
                ptr       NULL �̏ꍇ�̓������m��, �����łȂ���Ή���v��.

  Returns:      ptr �� NULL �Ȃ� size ���̃��������m�ۂ��ă|�C���^��Ԃ�����.
                �����łȂ���� ptr �̃�������������邱��.
                ����̏ꍇ, �Ԃ�l�͒P�ɖ��������.
 *---------------------------------------------------------------------------*/
static void *AllocatorForWFS(void *arg, u32 size, void *ptr)
{
    (void)arg;
    if (!ptr)
        return OS_Alloc(size);
    else
    {
        OS_Free(ptr);
        return NULL;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCCallbackForWFS

  Description:  WC �̖��������쓮�����Ԓʒm���󂯂�R�[���o�b�N.

  Arguments:    arg     - �ʒm�� WM �֐��̃R�[���o�b�N�|�C���^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WCCallbackForWFS(WMCallback *arg)
{
    WCStatus wcStatus = WcGetStatus();
    switch (arg->apiid)
    {
    case WM_APIID_START_MP:
        {                              /* MP �X�e�[�g�J�n */
            WMStartMPCallback *cb = (WMStartMPCallback *)arg;
            switch (cb->state)
            {
            case WM_STATECODE_MP_START:
                /*
                 * MP_PARENT �܂��� MP_CHILD �X�e�[�g�ւ̈ڍs��ʒm���邽�߂�
                 * WFS_Start() ���Ăяo���Ă�������.
                 * ���̒ʒm���_�@��, WFS �̓u���b�N�]���̂��߂�
                 * ������ WM_SetMPDataToPort() �֐����Ăяo���n�߂܂�.
                 */
                WFS_Start();
                break;
            }
        }
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* �e�평���� */
    OS_Init();
    OS_InitTick();

    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    /* �T���v���p�`��t���[�����[�N������ */
    UTIL_Init();

    /*
     * �t�@�C���V�X�e��������.
     * �e�@�̏ꍇ�ɂ͗L���� DMA �`�����l�����w�肷�邱�Ƃ��\�ł�.
     * �q�@�̏ꍇ�ɂ͎g�p����Ȃ��̂ŒP�ɖ�������܂�.
     */
    FS_Init(FS_DMA_NOT_USE);

    /* LCD�\���J�n */
    GX_DispOn();
    GXS_DispOn();

    {                                  /* ���C�����[�v */

        static KeyInfo key[1];
        int     frame_count;

        int     cur_mode = MODE_MAX;

        KeyRead(key);
        for (frame_count = 0;; ++frame_count)
        {
            KeyRead(key);

            ClearString();
            PrintString(1, 22, COLOR_WHITE, "frame:%d", frame_count);

            {                          /* ��ʂ̑J�ڔ��� */
                int     new_mode = MODE_ERROR;
                switch (WcGetStatus())
                {
                case WC_STATUS_READY:
                    {
                        /* �O��� WFS �����łɋN�����ł���Έ�x�I�� */
                        if (WFS_GetStatus() != WFS_STATE_STOP)
                        {
                            WFS_End();
                            WcFinish();
                            /* ������ WM, WC, WFS �S�ď������O��ԂɑJ�ڊ��� */
                        }
                    }
                    new_mode = MODE_SELECT;
                    break;
                case WC_STATUS_BUSY:
                    new_mode = MODE_BUSY;
                    break;
                case WC_STATUS_PARENT:
                    new_mode = MODE_PARENT;
                    break;
                case WC_STATUS_CHILD:
                    new_mode = MODE_CHILD;
                    break;
                case WC_STATUS_ERROR:
                    new_mode = MODE_ERROR;
                    break;
                }
                if (cur_mode != new_mode)
                {
                    cur_mode = new_mode;
                    ClearLine();
                }
            }

            {                          /* ��ʏ��� */
                switch (cur_mode)
                {
                case MODE_SELECT:
                    ModeSelect(key);
                    break;
                case MODE_ERROR:
                    ModeError(key);
                    break;
                case MODE_BUSY:
                    ModeWorking(key);
                    break;
                case MODE_PARENT:
                    ModeParent(key);
                    break;
                case MODE_CHILD:
                    ModeChild(key);
                    break;
                }
                FlushLine();
            }

            OS_WaitVBlankIntr();
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  �e�@/�q�@ �I����ʂł̏����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeSelect(const KeyInfo * p_key)
{
    if (!is_child)
    {
        PrintString(3, 10, COLOR_WHITE, "Press A to connect as PARENT");
    }
    PrintString(3, 12, COLOR_WHITE, "Press B to connect as CHILD");

    /*
     * WFS �� WM_SetPortCallback() �𗘗p���ċ쓮���邽��,
     * WM_Initialize() �̌�� WFS_Init() ���Ăяo���ĉ�����.
     */
    if (!is_child && (p_key->trg & PAD_BUTTON_A))
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
        WcInit(def_parent_param, FALSE, /* data-sharing is not used */
               TRUE,                   /* block-transfer is used */
               1 /* non care in this demo (data-sharing frame period) */ );
        WcSetBlockTransferCallback(WCCallbackForWFS);
        WcStartParent();
        /*
         * �e�@�̓f�t�H���g�ňȉ��̐ݒ肩��N�����Ă��܂�.
         * (�����ύX�������ꍇ�͔C�ӂ̃^�C�~���O�Őݒ�\�ł�)
         */
        WFS_InitParent(port_wbt, StateCallbackForWFS, AllocatorForWFS, NULL,
                       parent_packet_max, NULL, TRUE);
        WFS_EnableSync(0);
    }
    else if (p_key->trg & PAD_BUTTON_B)
    {
        is_child = TRUE;
        WcInit(def_parent_param, FALSE, /* data-sharing is not used */
               TRUE,                   /* block-transfer is used */
               1 /* non care in this demo (data-sharing frame period) */ );
        WcSetBlockTransferCallback(WCCallbackForWFS);
        WcStartChild();
        WFS_InitChild(port_wbt, StateCallbackForWFS, AllocatorForWFS, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�\����ʂł̏����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeError(const KeyInfo * p_key)
{
    PrintString(5, 10, COLOR_RED, "======= ERROR! =======");
    PrintString(5, 13, COLOR_WHITE, " Fatal error occured.");
    PrintString(5, 14, COLOR_WHITE, "Please reboot program.");

    (void)p_key;
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  �r�W�[��ʂł̏����B

  Arguments:    key         �L�[����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeWorking(const KeyInfo * p_key)
{
    PrintString(9, 11, COLOR_WHITE, "Now working...");

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
