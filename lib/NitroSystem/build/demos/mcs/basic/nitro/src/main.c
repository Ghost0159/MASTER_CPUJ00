/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - basic - nitro
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      mcs���C�u�������g�p���āAPC�Ƃ̃f�[�^�̓ǂݏ������s���܂��B
//      �܂��ŏ���BG��ʂɊG���\������܂��B
//      PC���̃v���O���� basic.exe ���N������ƁA�G���������Ɉړ����܂��B
//      basic.exe ��2�N������ƁA�G���΂ߕ����Ɉړ����܂��B
//
//      A �{�^�������Ńv���O�������I�����܂��B
//
// ============================================================================

#include "sdk_init.h"
#include "nns_util.h"

#include <nnsys/mcs.h>
#include <nnsys/misc.h>

// Windows�A�v���P�[�V�����Ƃ̎��ʂŎg�p����`�����l���l�ł�
static const u16 MCS_CHANNEL0 = 0;
static const u16 MCS_CHANNEL1 = 1;

// �ʒu���̍\����
typedef struct Vec2 Vec2;
struct Vec2
{
    s32     x;
    s32     y;
};

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݃n���h���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
VBlankIntr()
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    NNS_McsVBlankInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         CartIntrFunc

  Description:  �J�[�h���b�W���荞�݃n���h���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
CartIntrFunc()
{
    OS_SetIrqCheckFlag(OS_IE_CARTRIDGE);

    NNS_McsCartridgeInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupt

  Description:  ���荞�݊֘A�̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
InitInterrupt()
{
    (void)OS_DisableIrq();

    // V�u�����N���荞�݂�L���ɂ��AVBlank���荞�ݓ���
    // NNS_McsVBlankInterrupt()���Ă΂��悤�ɂ���
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)GX_VBlankIntr(TRUE);

	// �J�[�g���b�W���荞�݂�L���ɂ��A�J�[�g���b�W���荞�ݓ���
    // NNS_McsCartridgeInterrupt()���Ă΂��悤�ɂ���
    OS_SetIrqFunction(OS_IE_CARTRIDGE, CartIntrFunc);
	(void)OS_EnableIrqMask(OS_IE_CARTRIDGE);

    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         DataRecvCallback

  Description:  PC������f�[�^����M�����Ƃ��ɌĂ΂��R�[���o�b�N�֐��ł��B

                �o�^����R�[���o�b�N�֐����ł̓f�[�^�̑���M���s��Ȃ��ł��������B
                �܂��A���荞�݂��֎~����Ă���ꍇ�����邽�߁A
                ���荞�ݑ҂����[�v���s��Ȃ��ł��������B

  Arguments:    recv:       ��M�����f�[�^�̈ꕔ���邢�͑S�����i�[���Ă���
                            �o�b�t�@�ւ̃|�C���^�B
                recvSize:   recv�ɂ���Ď������o�b�t�@�Ɋi�[����Ă���
                            �f�[�^�̃T�C�Y�B
                userData:   NNS_McsRegisterRecvCallback()�̈���userData��
                            �w�肵���l�B
                offset:     ��M�����f�[�^�̑S����recv�ɂ���Ď������o�b�t�@��
                            �i�[����Ă���ꍇ��0�B
                            ��M�����f�[�^�̈ꕔ���i�[����Ă���ꍇ�́A
                            ��M�����f�[�^�S�̂ɑ΂���0����Ƃ����I�t�Z�b�g�ʒu�B
                totalSize:  ��M�����f�[�^�̑S�̂̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
DataRecvCallback(
    const void* pRecv,
    u32         recvSize,
    u32         userData,
    u32         offset,
    u32         /* totalSize */
)
{
    Vec2 *const pPos = (Vec2*)userData;

	// ��M�o�b�t�@�`�F�b�N
	if (pRecv != NULL && recvSize == sizeof(pPos->y) && offset == 0)
	{
        // y�ʒu����M�����l�ōX�V
        const s32* pRecvPosY = pRecv;
        pPos->y = *pRecvPosY;
    }
}

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �A�[�J�C�u����a�f�f�[�^��ǂݍ��݁A��ʂ�\������T���v��
                �v���O�����ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
static void
MainLoop(Vec2* pPos)
{
    while (TRUE)
    {
        u32 nLength;
        s32 posX;

        NNS_McsPollingIdle();

        // ��M�\�ȃf�[�^�T�C�Y�̎擾
		nLength = NNS_McsGetStreamReadableSize(MCS_CHANNEL0);
        if (nLength > 0 && sizeof(posX) == nLength)
        {
            u32 readSize;

            // �f�[�^�̓ǂݎ��
            if ( NNS_McsReadStream(MCS_CHANNEL0, &posX, sizeof(posX), &readSize)
              && readSize == sizeof(posX)
            )
            {
                pPos->x = posX;
            }
        }

        // ���͈͂���O�ꂽ��A�����l�𔽓]����悤��PC���ɓ`����
        if (pPos->x < 0 || 64 < pPos->x)
        {
            s32 writeData = pPos->x < 0 ? 1: -1;
            // �f�[�^�̏�������
            if (! NNS_McsWriteStream(MCS_CHANNEL0, &writeData, sizeof(writeData)))
            {
                break;
            }
        }

        // ���͈͂���O�ꂽ��A�����l�𔽓]����悤��PC���ɓ`����
        if (pPos->y < 0 || 64 < pPos->y)
        {
            s32 writeData = pPos->y < 0 ? 1: -1;
            // �f�[�^�̏�������
            if (! NNS_McsWriteStream(MCS_CHANNEL1, &writeData, sizeof(writeData)))
            {
                break;
            }
        }

        SVC_WaitVBlankIntr();

        G2_SetBG0Offset(pPos->x, pPos->y);

        ReadGamePad();
        if (IS_TRIGGER(PAD_BUTTON_A))
        {
            break;
        }
    }
}

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �A�[�J�C�u����a�f�f�[�^��ǂݍ��݁A��ʂ�\������T���v��
                �v���O�����ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    u8* mcsWorkMem;
    NNSMcsDeviceCaps deviceCaps;
    NNSMcsRecvCBInfo recvCBInfo;
    Vec2* pPos;

    InitSystem();

    InitInterrupt();
    InitMemory();
    
    // mcs�̏�����
    mcsWorkMem = NNS_FndAllocFromExpHeapEx(gAppHeap, NNS_MCS_WORKMEM_SIZE, 4); // MCS�̃��[�N�p���������m��
    NNS_McsInit(mcsWorkMem);

    InitVRAM();

    InitDisplay();
    LoadPicture();

    pPos = NNS_FndAllocFromExpHeapEx(gAppHeap, sizeof(Vec2), 4);  // �f�[�^�p�̃��������m��

    // �f�[�^�̏�����
    pPos->x = 0;
    pPos->y = 0;

    G2_SetBG0Offset(pPos->x, pPos->y);

    // �f�o�C�X�̃I�[�v��
    if (NNS_McsGetMaxCaps() > 0 && NNS_McsOpen(&deviceCaps))
    {
        void* printBuffer = NNS_FndAllocFromExpHeap(gAppHeap, 1024);        // �v�����g�p�̃o�b�t�@�̊m��
        void* recvBuf = NNS_FndAllocFromExpHeapEx(gAppHeap, 1024, 4);       // ��M�p�o�b�t�@�̊m��

        NNS_NULL_ASSERT(printBuffer);
        NNS_NULL_ASSERT(recvBuf);
        NNS_NULL_ASSERT(pPos);

        // OS_Printf�ɂ��o��
        OS_Printf("device open\n");

        // mcs������o�͂̏�����
        NNS_McsInitPrint(printBuffer, NNS_FndGetSizeForMBlockExpHeap(printBuffer));

        // NNS_McsPrintf�ɂ��o��
        // ���̃^�C�~���O��mcs�T�[�o���ڑ����Ă���΁A�R���\�[���ɕ\������܂��B
        (void)NNS_McsPrintf("device ID %08X\n", deviceCaps.deviceID);

        // �ǂݎ��p�o�b�t�@�̓o�^
        NNS_McsRegisterStreamRecvBuffer(MCS_CHANNEL0, recvBuf, NNS_FndGetSizeForMBlockExpHeap(recvBuf));

        // ��M�R�[���o�b�N�֐��̓o�^
        NNS_McsRegisterRecvCallback(&recvCBInfo, MCS_CHANNEL1, DataRecvCallback, (u32)pPos);

        MainLoop(pPos);

        NNS_McsUnregisterRecvResource(MCS_CHANNEL1);
        NNS_McsUnregisterRecvResource(MCS_CHANNEL0);

        NNS_FndFreeToExpHeap(gAppHeap, recvBuf);        // ��M�p�o�b�t�@�̊m��
        NNS_FndFreeToExpHeap(gAppHeap, printBuffer);    // �v�����g�p�̃o�b�t�@�̊m��

        // NNS_McsPutString�ɂ��o��
        (void)NNS_McsPutString("device close\n");

        // �f�o�C�X���N���[�Y
        (void)NNS_McsClose();
    }
    else
    {
        OS_Printf("device open fail.\n");
        MainLoop(pPos);
    }

    NNS_FndFreeToExpHeap(gAppHeap, pPos);           // �f�[�^�p�̃����������

    while (TRUE) {}
}

