/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d
  File:     gecom.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.25 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/gecom.h>

//
// NOTICE:
// ONLY FOR SINGLE THREADED CODE
//


////////////////////////////////////////////////////////////////////////////////
//
// �X�^�e�B�b�N�ϐ�
//

/*---------------------------------------------------------------------------*
    NNS_G3dFlagGXDmaAsync

    G3D�Ŏg�p�����MI_SendGXCommandAsync���g�p����t���O
    0�ȊO�̏ꍇ�ɂ�Dma�̓]�����������Ă��Ȃ��B
 *---------------------------------------------------------------------------*/
static volatile int NNS_G3dFlagGXDmaAsync  = 0;

//
// NOTICE:
// Ge�֘AAPI��Buffer��NULL�ł����퓮�삵�Ȃ��Ă͂Ȃ�Ȃ�
//

/*---------------------------------------------------------------------------*
    NNS_G3dGeBuffer

    G3D���g�p����W�I���g���R�}���h�o�b�t�@�ւ̃|�C���^�B
    �ʏ�X�^�b�N(DTCM)�̈�̃����������蓖�Ă���B

    �f�B�X�v���C���X�g�̂c�l�`�]���ɃW�I���g���R�}���h�̑��M�v�����������ꍇ�A
    ���̃o�b�t�@�ɃR�}���h�������o�����Ƃɂ��A�b�o�t�̏��������ɐi�߂�
    ���Ƃ��ł���B�傫�ȃf�B�X�v���C���X�g�����I�u�W�F�N�g�ɗL���B
 *---------------------------------------------------------------------------*/
static NNSG3dGeBuffer* NNS_G3dGeBuffer = NULL;


////////////////////////////////////////////////////////////////////////////////
//
// APIs
//


/*---------------------------------------------------------------------------*
    NNS_G3dGeIsSendDLBusy

    G3D�ōs���W�I���g���R�}���h�̃W�I���g���G���W���ւ̂c�l�`�]����
    �]�������ǂ�����Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dGeIsSendDLBusy(void)
{
    return NNS_G3dFlagGXDmaAsync;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeIsBufferExist

    �W�I���g���R�}���h�o�b�t�@�����݂��邩�ǂ�����Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dGeIsBufferExist(void)
{
    return (NNS_G3dGeBuffer != NULL);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeSetBuffer

    NNS_G3dGeBuffer�ɂ܂��W�I���g���R�}���h�o�b�t�@���Z�b�g����Ă��Ȃ��ꍇ�A
    p���W�I���g���R�}���h�o�b�t�@�Ƃ��ăZ�b�g���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeSetBuffer(NNSG3dGeBuffer* p)
{
    NNS_G3D_NULL_ASSERT(p);

    if (NNS_G3dGeBuffer == NULL)
    {
        p->idx = 0;
        NNS_G3dGeBuffer = p;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeReleaseBuffer

    �W�I���g���R�}���h�o�b�t�@�����S�Ɏ�菜���܂��B
    �Ԃ�l�͎�菜���ꂽ�W�I���g���R�}���h�o�b�t�@�ւ̃|�C���^�ł�
 *---------------------------------------------------------------------------*/
NNSG3dGeBuffer*
NNS_G3dGeReleaseBuffer(void)
{
    NNSG3dGeBuffer* p;

    NNS_G3dGeFlushBuffer();

    p = NNS_G3dGeBuffer;
    NNS_G3dGeBuffer = NULL;
    return p;
}


static NNS_G3D_INLINE void
sendNB(const void* src, void* dst, u32 szByte)
{
    MI_CpuSend32(src, dst, szByte);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeFlushBuffer

    �W�I���g���R�}���h��DMA�]�����ł���ΏI�����܂ŃE�F�C�g���A
    �W�I���g���R�}���h�o�b�t�@������΁A�o�b�t�@�̓��e���W�I���g���G���W����
    �����o���܂��B
    ���̊֐��̏I����́A�W�I���g���R�}���h�𒼐ڃW�I���g���G���W���ɑ��M���邱�Ƃ�
    �ł��܂�(���S��SDK��G3_XXX�֐����g����悤�ɂȂ�)�B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeFlushBuffer(void)
{
    if (NNS_G3dFlagGXDmaAsync)
    {
        NNS_G3dGeWaitSendDL();
    }

    if (NNS_G3dGeBuffer &&
        NNS_G3dGeBuffer->idx > 0)
    {
        sendNB(&NNS_G3dGeBuffer->data[0],
               (void*)&reg_G3X_GXFIFO,
               NNS_G3dGeBuffer->idx << 2);
        NNS_G3dGeBuffer->idx = 0;
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeWaitSendDL

    �W�I���g���R�}���h�̂c�l�`�]��(NNS_G3dGeSendDL���g�p�̂���)����������܂�
    �E�F�C�g���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeWaitSendDL(void)
{
    while(NNS_G3dFlagGXDmaAsync)
        ;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeIsImmOK

    �R�}���h�𒼐�FIFO�ɑ��M(g3imm.h)���Ă悢���ǂ�����Ԃ��B
    ���M���Ă��悢�̂Ȃ�ATRUE��Ԃ��B
    �����̓R�}���h�o�b�t�@�����݂��Ȃ�����ŁA����GXDMA�]�����łȂ��Ƃ��ł��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dGeIsImmOK(void)
{
    return (NNS_G3dGeBuffer == NULL || NNS_G3dGeBuffer->idx == 0) &&
           !NNS_G3dGeIsSendDLBusy();
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeIsBufferOK

    numWord�̃f�[�^���W�I���g���R�}���h�o�b�t�@�ɒǉ��\���ǂ�����Ԃ��B
    �\�Ȃ��TRUE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL
NNS_G3dGeIsBufferOK(u32 numWord)
{
    return (NNS_G3dGeBuffer != NULL) &&
           (NNS_G3dGeBuffer->idx + numWord <= NNS_G3D_SIZE_COMBUFFER);
}


//
// NNS_G3dGeSendDL��p
//
static void
simpleUnlock_(void* arg)
{
    *((volatile int*)arg) = 0;
}

#ifdef NNS_G3D_USE_FASTGXDMA
static BOOL NNS_G3dFlagUseFastDma = TRUE;
#else
static BOOL NNS_G3dFlagUseFastDma = FALSE;
#endif


/*---------------------------------------------------------------------------*
    NNS_G3dGeUseFastDma

    ������FALSE�ȊO���w�肷��ƁA�W�I���g���R�}���h��DMA�]����
    MI_SendGXCommandAsyncFast�֐����g�p���܂��B���̏ꍇ�́A
    �v���O���~���O�}�j���A����7�͂́uARM9�V�X�e���o�X�ɂ�����
    DMA�����`���l������N�����Ɋւ��钍�ӎ����v������Ă��g���������B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeUseFastDma(BOOL cond)
{
    NNS_G3dFlagUseFastDma = (cond);
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeSendDL

    �f�B�X�v���C���X�g�����S�ɃW�I���g���G���W���ɏ����o���܂��B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeSendDL(const void* src, u32 szByte)
{
    NNS_G3D_NULL_ASSERT(src);
    NNS_G3D_ASSERT(szByte >= 4);

    // �]���J�n
    if (szByte < 256 || GX_DMAID == GX_DMA_NOT_USE)
    {
        NNS_G3dGeBufferOP_N(*(const u32*)src,
                            (const u32*)src + 1,
                            (szByte >> 2) - 1);
    }
    else
    {
        // �R�}���h�o�b�t�@���Ƀt���b�V�����Ă���
        NNS_G3dGeFlushBuffer();
        NNS_G3dFlagGXDmaAsync = 1;

        if (NNS_G3dFlagUseFastDma)
        {
            MI_SendGXCommandAsyncFast(GX_DMAID,
                                      src,
                                      szByte,
                                      &simpleUnlock_,
                                      (void*)&NNS_G3dFlagGXDmaAsync);
        }
        else
        {
            MI_SendGXCommandAsync(GX_DMAID,
                                  src,
                                  szByte,
                                  &simpleUnlock_,
                                  (void*)&NNS_G3dFlagGXDmaAsync);
        }
    }
}


/*---------------------------------------------------------------------------*
    NNS_G3dGeBufferOP_N

    ������N�Ƃ�W�I���g���R�}���h�𑗐M����B
 *---------------------------------------------------------------------------*/
void
NNS_G3dGeBufferOP_N(u32 op, const u32* args, u32 num)
{
    if (NNS_G3dGeBuffer)
    {
        if (NNS_G3dFlagGXDmaAsync)
        {
            // �o�b�t�@�̋󂫗e�ʂ͏\���ɂ��邩�H
            if (NNS_G3dGeBuffer->idx + 1 + num <= NNS_G3D_SIZE_COMBUFFER)
            {
                NNS_G3dGeBuffer->data[NNS_G3dGeBuffer->idx++] = op;
                if (num > 0)
                {
                    MI_CpuCopyFast(args,
                                   &NNS_G3dGeBuffer->data[NNS_G3dGeBuffer->idx],
                                   num << 2);

                    NNS_G3dGeBuffer->idx += num;
                }
                // �o�b�t�@�����O����
                return;
            }
        }

        // ���ǃo�b�t�@�ɒǉ�����Ă��Ȃ�
        // �o�b�t�@����ɂ��Ă���C�~�f�B�G�C�g�ŏ�������
        if (NNS_G3dGeBuffer->idx != 0)
        {
            NNS_G3dGeFlushBuffer();
        }
        else
        {
            if (NNS_G3dFlagGXDmaAsync)
            {
                NNS_G3dGeWaitSendDL();
            }
        }
    }
    else
    {
        if (NNS_G3dFlagGXDmaAsync)
        {
            NNS_G3dGeWaitSendDL();
        }
    }

    reg_G3X_GXFIFO = op;
    sendNB(args, (void*)&reg_G3X_GXFIFO, num << 2);
}
