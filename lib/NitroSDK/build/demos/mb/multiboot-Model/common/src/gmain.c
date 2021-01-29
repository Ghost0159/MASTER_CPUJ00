/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     gmain.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: gmain.c,v $
  Revision 1.8  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.7  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.6  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.5  2005/02/18 11:08:22  yosizaki
  fix around hidden warnings.

  Revision 1.4  2004/10/28 01:28:04  takano_makoto
  wmhigh��wireless_shared��wh�ֈڍs

  Revision 1.3  2004/10/21 00:43:07  yosizaki
  add SHARED_WH switch.

  Revision 1.2  2004/09/21 07:30:45  takano_makoto
  small fix

  Revision 1.1  2004/09/21 06:49:41  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "gmain.h"
#include "common.h"
#include "disp.h"
#include "wh.h"



//============================================================================
//  �ϐ���`
//============================================================================

//-----------------------
// �f�[�^�V�F�A�����O�p
//-----------------------
static u8 sSendBuf[256] ATTRIBUTE_ALIGN(32);    // ���M�o�b�t�@( �v32�o�C�g�A���C�� )
static u8 sRecvBuf[256] ATTRIBUTE_ALIGN(32);    // ��M�o�b�t�@( �v32�o�C�g�A���C�� )
static BOOL sRecvFlag[1 + WM_NUM_MAX_CHILD];    // ��M�t���O
static GShareData *sShareDataPtr;

//-----------------------
// �L�[�V�F�A�����O�p
//-----------------------
static WMKeySet sKeySet;


//============================================================================
//  �֐���`
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         GInitDataShare

  Description:  �f�[�^�V�F�A�����O�p�o�b�t�@�̏������ݒ�

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void GInitDataShare(void)
{
    sShareDataPtr = (GShareData *) sSendBuf;

    DC_FlushRange(sSendBuf, 4);
    DC_WaitWriteBufferEmpty();
}


/*---------------------------------------------------------------------------*
  Name:         GStepDataShare

  Description:  �f�[�^�V�F�A�����O�̓�����1�i�߂܂��B

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void GStepDataShare(s32 frame)
{
    sShareDataPtr->count = (u32)frame;
    sShareDataPtr->key = GetPressKey();

    switch (WH_GetSystemState())
    {
        //------------------------------
        // �f�[�^�V�F�A�����O�ʐM
    case WH_SYSSTATE_DATASHARING:
        {
            u16     i;
            u8     *adr;

            if (!WH_StepDS(sSendBuf))
            {
                // �f�[�^�V�F�A�ʐM���s
                return;
            }

            for (i = 0; i < 16; ++i)
            {
                adr = (u8 *)WH_GetSharedDataAdr(i);
                if (adr != NULL)
                {
                    MI_CpuCopy8(adr, &(sRecvBuf[i * sizeof(GShareData)]), sizeof(GShareData));
                    sRecvFlag[i] = TRUE;
                }
                else
                {
                    sRecvFlag[i] = FALSE;
                }
            }
        }
        break;
        //------------------------------
        // �L�[�V�F�A�����O�ʐM
    case WH_SYSSTATE_KEYSHARING:
        {
            (void)WH_GetKeySet(&sKeySet);
            if ((sKeySet.key[0] != 0) || (sKeySet.key[1] != 0))
            {
                OS_TPrintf("0 -> %04x 1 -> %04x\n", sKeySet.key[0], sKeySet.key[1]);
            }
        }
    }
}






/*---------------------------------------------------------------------------*
  Name:         GMain

  Description:  �e�q���ʃ��C�����[�`��

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void GMain(void)
{

    BgPrintStr(4, 3, 0x4, "Send:     %04x-%04x", sShareDataPtr->key, sShareDataPtr->count);
    BgPutString(4, 5, 0x4, "Receive:");
    {
        s32     i;

        for (i = 0; i < (WM_NUM_MAX_CHILD + 1); i++)
        {
            if (sRecvFlag[i])
            {
                GShareData *sd;
                sd = (GShareData *) (&(sRecvBuf[i * sizeof(GShareData)]));

                BgPrintStr(4, (s16)(6 + i), 0x4,
                           "Player%02d: %04x-%04x", i, sd->key, sd->count & 0xffff);
            }
            else
            {
                BgPutString(4, (s16)(6 + i), 0x7, "No child");
            }
        }
    }

    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
    {
        //********************************
        (void)WH_Finalize();
        //********************************
    }
}
