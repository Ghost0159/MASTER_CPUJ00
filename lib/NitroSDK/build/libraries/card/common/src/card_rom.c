/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_rom.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: card_rom.c,v $
  Revision 1.40  2007/06/20 08:04:31  yosizaki
  fix about CRADi_ReadCard.

  Revision 1.39  2007/06/06 01:44:04  yasu
  ����N�x�C��

  Revision 1.38  2007/05/28 04:15:18  yosizaki
  fix around cache invalidation.

  Revision 1.37  2006/08/23 00:39:35  yosizaki
  add cardi_rom_header_addr.

  Revision 1.36  2006/04/24 05:51:53  yosizaki
  fix about CARDi_TryReadCardDma.

  Revision 1.35  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.34  2005/11/30 23:51:57  yosizaki
  fix warning.

  Revision 1.33  2005/11/29 01:20:29  yosizaki
  fix comments.

  Revision 1.32  2005/11/09 07:39:13  adachi_hiroaki
  ARM7����ROM�ǂݍ��݂𖳌���

  Revision 1.31  2005/11/07 01:10:01  okubata_ryoma
  SDK_ASSERT_ON_COMPILE����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.30  2005/06/27 23:51:25  yosizaki
  smalll fix. (remove 'const')

  Revision 1.29  2005/06/27 00:26:48  yosizaki
  fix CARDi_TryReadCardDma().

  Revision 1.28  2005/05/12 11:19:42  yosizaki
  optimize some functions and calling interfaces.
  add comment.

  Revision 1.27  2005/04/28 08:12:38  yosizaki
  add assertions about DMA-0 limitation.

  Revision 1.26  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.25  2005/02/16 01:00:31  yosizaki
  fix around CARD-DMA.

  Revision 1.24  2005/02/15 09:41:38  yosizaki
  delete unnecessary flush-command.

  Revision 1.23  2005/01/24 06:35:13  yosizaki
  copyright �N�\�L�C��.

  Revision 1.22  2005/01/19 12:18:00  yosizaki
  delete pragma `pack'.

  Revision 1.21  2004/12/15 09:44:45  yasu
  CARD �A�N�Z�X�C�l�[�u���@�\�̒ǉ�

  Revision 1.20  2004/12/08 08:53:49  yosizaki
  delete CARDi_ReadRomID()

  Revision 1.19  2004/11/22 07:38:29  yosizaki
  add cardi_rom_base.

  Revision 1.18  2004/11/12 05:45:10  yosizaki
  add CARDi_CheckValidity().

  Revision 1.17  2004/10/18 00:22:19  yosizaki
  change latency1 of READ-ID.

  Revision 1.16  2004/10/08 11:58:20  yosizaki
  move CARD-DMA setting to mi/mi_dma_card.c.
  delete unused functions.

  Revision 1.15  2004/10/04 04:39:13  yada
  avoid to call CARD_InitPulledOutCallback() twice
  even if CARD_Init() and FS_Init() are called.

  Revision 1.14  2004/09/27 13:45:08  yada
  consider for SMALL_BUILD

  Revision 1.13  2004/09/15 05:54:30  yada
  add card pulled off callback setting

  Revision 1.12  2004/09/02 11:54:42  yosizaki
  fix CARD include directives.

  Revision 1.11  2004/08/31 10:59:28  yosizaki
  add errorcode CARD_RESULT_SUCCESS in some functions.

  Revision 1.10  2004/08/30 04:33:22  yada
  remove MI_XXX definitions because those are already defined in dma.h

  Revision 1.9  2004/08/23 10:38:10  yosizaki
  add write-command.
  add retry count on writing timeout.

  Revision 1.8  2004/07/28 13:18:06  yosizaki
  asynchronous mode support.

  Revision 1.7  2004/07/23 08:29:06  yosizaki
  fix CARD_ReadRomID.

  Revision 1.6  2004/07/19 13:15:15  yosizaki
  move thread to card_common.c

  Revision 1.5  2004/07/13 07:59:25  yosizaki
  fix CARD-asynchronous operation.

  Revision 1.4  2004/07/10 10:19:50  yosizaki
  add card_common.c

  Revision 1.3  2004/07/09 08:40:27  yosizaki
  change implement of synchronous mode.

  Revision 1.2  2004/07/08 13:43:56  yosizaki
  change ROM-access (support all case)

  Revision 1.1  2004/06/28 01:52:48  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>


#include "../include/card_common.h"
#include "../include/card_rom.h"


/*****************************************************************************/
/* variable */

CARDRomStat rom_stat ATTRIBUTE_ALIGN(32);

/* (�ʏ�� 0) */
extern u32 cardi_rom_base;
u32     cardi_rom_base;
u32     cardi_rom_header_addr = HW_ROM_HEADER_BUF;

SDK_COMPILER_ASSERT(sizeof(rom_stat) % 32 == 0);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         CARDi_OnReadPageDirect

  Description:  ���ړ]�����1�y�[�W�]��������̃R�[���o�b�N

  Arguments:    arg        CARDRomStat �\����

  Returns:      �܂��]�����K�v�ł���� TRUE.
 *---------------------------------------------------------------------------*/
static inline BOOL CARDi_OnReadPageDirect(CARDRomStat * arg)
{
    CARDiCommon *p = &cardi_common;
    (void)arg;
    p->src += CARD_ROM_PAGE_SIZE;
    p->dst += CARD_ROM_PAGE_SIZE;
    p->len -= CARD_ROM_PAGE_SIZE;
    return (p->len > 0);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadFromCache

  Description:  �\�ł���΍ŋ߂̃��[�h�L���b�V������ROM�f�[�^�]��

  Arguments:    p          CARDRomStat �\����

  Returns:      �܂��]�����K�v�ł���� TRUE.
 *---------------------------------------------------------------------------*/
static BOOL CARDi_ReadFromCache(CARDRomStat * p)
{
    CARDiCommon *c = &cardi_common;
    const u32 cur_page = CARD_ALIGN_HI_BIT(c->src);
    if (cur_page == (u32)p->cache_page)
    {
        const u32 mod = c->src - cur_page;
        u32     len = CARD_ROM_PAGE_SIZE - mod;
        if (len > c->len)
            len = c->len;
        MI_CpuCopy8(p->cache_buf + mod, (void *)c->dst, len);
        c->src += len;
        c->dst += len;
        c->len -= len;
    }
    return (c->len > 0);
}


#if defined(SDK_TS) || defined(SDK_ARM7)


/*---------------------------------------------------------------------------*
  Name:         CARDi_SetRomOp

  Description:  �J�[�h�R�}���h�ݒ�

  Arguments:    cmd1       �R�}���h���[�h1
                cmd2       �R�}���h���[�h2

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_SetRomOp(u32 cmd1, u32 cmd2)
{
    /* CARD ROM �̊����҂� (�O�̂���) */
    while ((*(vu32 *)REG_CARDCNT & CARD_START) != 0) ;
    /* �}�X�^�[�C�l�[�u�� */
    *(vu8 *)REG_CARD_MASTER_CNT = CARDMST_SEL_ROM | CARDMST_ENABLE | CARDMST_IF_ENABLE;
    {                                  /* �R�}���h�ݒ� */
        vu8    *const p_cmd = (vu8 *)REG_CARD_CMD;
        p_cmd[0] = (u8)(cmd1 >> (8 * 3));
        p_cmd[1] = (u8)(cmd1 >> (8 * 2));
        p_cmd[2] = (u8)(cmd1 >> (8 * 1));
        p_cmd[3] = (u8)(cmd1 >> (8 * 0));
        p_cmd[4] = (u8)(cmd2 >> (8 * 3));
        p_cmd[5] = (u8)(cmd2 >> (8 * 2));
        p_cmd[6] = (u8)(cmd2 >> (8 * 1));
        p_cmd[7] = (u8)(cmd2 >> (8 * 0));
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_SetRomOpReadPage1

  Description:  1 �y�[�W���[�h�R�}���h

  Arguments:    src        ���[�h����ROM�I�t�Z�b�g

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CARDi_SetRomOpReadPage1(u32 src)
{
    CARDi_SetRomOp((u32)(MROMOP_G_READ_PAGE | (src >> 8)), (u32)(src << 24));
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_SetCardDma

  Description:  1 �y�[�W���� CARD-DMA ���J�n

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_SetCardDma(void)
{
    CARDiCommon *const c = &cardi_common;
    CARDRomStat *const p = &rom_stat;
    /* �܂� CARD-DMA �̐ݒ���s�� */
    MIi_CardDmaCopy32(c->dma, (const void *)REG_CARD_DATA, (void *)c->dst, CARD_ROM_PAGE_SIZE);
    /* ���� CARD-ROM �̐ݒ���s�� */
    CARDi_SetRomOpReadPage1(c->src);
    *(vu32 *)REG_CARDCNT = p->ctrl;
    /* ��������񓯊��N������ */
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_OnReadCard

  Description:  CARD-DMA �����n���h��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_OnReadCard(void)
{
    CARDRomStat *const p = &rom_stat;
    CARDiCommon *const c = &cardi_common;
    MI_StopDma(c->dma);
    if (!CARDi_OnReadPageDirect(p))
    {
        (void)OS_DisableIrqMask(OS_IE_CARD_DATA);
        (void)OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
        CARDi_ReadEnd();
    }
    else
    {
        CARDi_SetCardDma();
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_TryReadCardDma

  Description:  CARD-DMA �ł���Ȃ�ݒ肵, TRUE ��Ԃ�

  Arguments:    p          CARDRomStat �\����

  Returns:      ���ۂ� CARD-DMA ���J�n�����ꍇ�� TRUE.
 *---------------------------------------------------------------------------*/
BOOL CARDi_TryReadCardDma(CARDRomStat * p)
#if defined(SDK_TS)
{
    CARDiCommon *const c = &cardi_common;
    const u32 dst = c->dst;
    u32     len = c->len;
    /* �ȉ��̑S�����𖞂����΍���̎w��͈͂� DMA �]���\ */
    const BOOL is_async = !(dst & 31) &&    /* �]���拫�E����(32) */
        (c->dma <= MI_DMA_MAX_NUM) &&       /* �L���� DMA �`�����l�� */
        !CARDi_IsInTcm(dst, len) &&         /* �]���悪 TCM �̈�ł͂Ȃ� */
        !CARD_ALIGN_LO_BIT(c->src | len) && /* �]����&�T�C�Y���E����(512) */
        (len > 0);
    p->ctrl = CARDi_GetRomFlag(CARD_COMMAND_PAGE);
    if (is_async)
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
#if defined(SDK_ARM9)
        /* �]���͈͂̃T�C�Y�ɉ����ăL���b�V���������̕��@��؂�ւ� */
        if (len < c->flush_threshold_ic)
        {
            IC_InvalidateRange((void *)dst, len);
        }
        else
        {
            IC_InvalidateAll();
        }
        if (len < c->flush_threshold_dc)
        {
            u32     pos = dst;
            u32     mod = (dst & (HW_CACHE_LINE_SIZE - 1));
            if (mod)
            {
                pos -= mod;
                DC_StoreRange((void *)(pos), HW_CACHE_LINE_SIZE);
                DC_StoreRange((void *)(pos + len), HW_CACHE_LINE_SIZE);
                len += HW_CACHE_LINE_SIZE;
            }
            DC_InvalidateRange((void *)pos, len);
            DC_WaitWriteBufferEmpty();
        }
        else
        {
            DC_FlushAll();
        }
#endif
        (void)OS_SetIrqFunction(OS_IE_CARD_DATA, CARDi_OnReadCard);
        (void)OS_ResetRequestIrqMask(OS_IE_CARD_DATA);
        (void)OS_EnableIrqMask(OS_IE_CARD_DATA);
        (void)OS_RestoreInterrupts(bak_psr);
        CARDi_SetCardDma();
    }
    return is_async;
}
#else
{
    (void)p;
    return FALSE;
}
#endif /* defined(SDK_TS) */


/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadCard

  Description:  �ݒ肳�ꂽ���e�Ɋ�Â��ăJ�[�h�A�N�Z�X�𒼐ڎ��s.

  Arguments:    p          �A�N�Z�X���e���ݒ肳�ꂽ�\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_ReadCard(CARDRomStat * p)
{
#if defined(SDK_ARM9) \
    || (defined(SDK_ARM7) && defined(SDK_ARM7_READROM_SUPPORT))

    /*
     * CPU (��, �\�Ȃ� DMA)�ɂ�铯���]��.
     * �E4 BYTE �A���C������Ă���ꍇ
     * �E1 BYTE �Y���Ă�ꍇ
     * �E2 BYTE �Y���Ă�ꍇ
     * �E3 BYTE �Y���Ă�ꍇ
     * �e�X�ɂ��ăY���␳�]����g�݉\�Ȍ��荂����.
     * �Ƃ肠�������̓e���|�����Ɉڂ��� MI_CpuCopy8 �ɗ���.
     *
     * CARD �̃��[�h�P�ʃr�W�[���[���ɒ����̂ł����,
     * �Ō�̃y�[�W�����K���e���|�����֓ǂݍ��ނ悤�ɂ��Ă�
     * �p�t�H�[�}���X�ɉe����^���Ȃ�.
     */
    CARDiCommon *const c = &cardi_common;
    for (;;)
    {
        const u32 len = CARD_ROM_PAGE_SIZE;
        u32     src = CARD_ALIGN_HI_BIT(c->src);
        u32     dst;
        /* �L���b�V���o�R���K�v������ */
        if ((src != c->src) || ((c->dst & 3) != 0) || (c->len < len))
        {
            dst = (u32)p->cache_buf;
            p->cache_page = (void *)src;
        }
        else
        {
            dst = c->dst;
        }
        /* ����̐ݒ���s�� */
        CARDi_SetRomOpReadPage1(src);
        {                              /* CPU �]�� (�J��Ԃ����[�v) */
            u32     pos = 0;
            *(vu32 *)REG_CARDCNT = p->ctrl;
            for (;;)
            {
                const u32 ctrl = *(vu32 *)REG_CARDCNT;
                if ((ctrl & CARD_DATA_READY) != 0)
                {
                    /* �w��T�C�Y�������i�[ */
                    u32     data = *(vu32 *)REG_CARD_DATA;
                    if (pos * sizeof(u32) < len)
                        ((u32 *)dst)[pos++] = data;
                }
                if (!(ctrl & CARD_START))
                    break;
            }
        }
        /* ����̓]����������i�߂� */
        if (dst == c->dst)
        {
            if (!CARDi_OnReadPageDirect(p))
                break;
        }
        else
        {
            if (!CARDi_ReadFromCache(p))
                break;
        }
    }
#else
#pragma unused(p)
#endif
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomIDCore

  Description:  �J�[�h ID �̓ǂݏo��

  Arguments:    None.

  Returns:      �J�[�h ID.
 *---------------------------------------------------------------------------*/
u32 CARDi_ReadRomIDCore(void)
{
    CARDi_SetRomOp(MROMOP_G_READ_ID, 0);
    *(vu32 *)REG_CARDCNT = (u32)(CARDi_GetRomFlag(CARD_COMMAND_ID) & ~CARD_LATENCY1_MASK);
    while (!(*(vu32 *)REG_CARDCNT & CARD_DATA_READY))
    {
    }
    return *(vu32 *)REG_CARD_DATA;
}

#endif /* defined(SDK_TS) || defined(SDK_ARM7) */


/********************************************************************/


#if defined(SDK_ARM7) && defined(SDK_ARM7_READROM_SUPPORT)

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomCore

  Description:  ���N�G�X�g�ɂ��J�[�h�]��.
                ��ɓ�������.

  Arguments:    src        �]����
                dst        �]����
                len        �]���T�C�Y

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_ReadRomCore(const void *src, void *dst, u32 len)
{
    CARDRomStat *const p = &rom_stat;
    CARDiCommon *const c = &cardi_common;

    /* ����̓]���p�����[�^�ݒ� */
    c->dma = (MI_DMA_MAX_NUM + 1);
    c->src = (u32)src;
    c->dst = (u32)dst;
    c->len = (u32)len;
    c->callback = NULL;
    c->callback_arg = NULL;
    p->ctrl = CARDi_GetRomFlag(CARD_COMMAND_PAGE);
    cardi_common.cur_th = OS_GetCurrentThread();
    CARDi_ReadCard(p);
}

#endif

#if defined(SDK_TS)
/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomID

  Description:  �J�[�h ID �̓ǂݏo��

  Arguments:    None.

  Returns:      �J�[�h ID.
 *---------------------------------------------------------------------------*/
u32 CARDi_ReadRomID(void)
{
    CARDRomStat *const p = &rom_stat;
    CARDiCommon *const c = &cardi_common;

    u32     ret = 0;

    SDK_ASSERT(CARD_IsAvailable());
    SDK_ASSERTMSG(CARDi_GetTargetMode() == CARD_TARGET_ROM, "must be locked by CARD_LockRom()");

    CARDi_WaitTask(c, NULL, NULL);

    /* ���ڃA�N�Z�X�\�Ȃ̂ł����Ŏ��s */
    ret = CARDi_ReadRomIDCore();
    CARDi_ReadEnd();
    return ret;
}
#endif

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRomSyncCore

  Description:  �����J�[�h�ǂݍ��݂̋��ʏ���

  Arguments:    c          CARDiCommon �\����

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CARDi_ReadRomSyncCore(CARDiCommon * c)
{
    CARDRomStat *const p = &rom_stat;
    (void)c;
    /*
     * �O��A�N�Z�X�����L���b�V�������ŉ������Ȃ��Ȃ�
     * ��������ۂ� ROM �A�N�Z�X. (CARD / CARTRIDGE / PXI)
     */
    if (CARDi_ReadFromCache(p))
    {
        (*p->read_func) (p);
    }
    CARDi_ReadEnd();
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadRom

  Description:  ROM ���[�h�̊�{�֐�

  Arguments:    dma        �g�p���� DMA �`�����l��
                src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊����[�h���w�肷��Ȃ� TRUE

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_ReadRom(u32 dma,
                   const void *src, void *dst, u32 len,
                   MIDmaCallback callback, void *arg, BOOL is_async)
{
    CARDRomStat *const p = &rom_stat;
    CARDiCommon *const c = &cardi_common;

    SDK_ASSERT(CARD_IsAvailable());    /* CARD_Init ���Ă΂�Ă��邩���� */
    SDK_ASSERT(CARDi_GetTargetMode() == CARD_TARGET_ROM);
    SDK_ASSERTMSG((dma != 0), "cannot specify DMA channel 0");

    /* CARD �A�N�Z�X�ɑ΂��鐳��������
       �����A�N�Z�X�֎~�Ȃ�֐������� OS_Panic ���� */
    CARD_CheckEnabled();

    /* ARM9 ���̔r���҂� */
    CARDi_WaitTask(c, callback, arg);

    /* ����̓]���p�����[�^�ݒ� */
    c->dma = dma;
    c->src = (u32)((u32)src + cardi_rom_base);
    c->dst = (u32)dst;
    c->len = (u32)len;
    if (dma <= MI_DMA_MAX_NUM)
        MI_StopDma(dma);

    if (CARDi_TryReadCardDma(p))
    {
        if (!is_async)
            CARD_WaitRomAsync();
    }
    else if (is_async)
    {
        CARDi_SetTask(CARDi_ReadRomSyncCore);
    }
    else
    {
        c->cur_th = OS_GetCurrentThread();
        CARDi_ReadRomSyncCore(c);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_Init

  Description:  initialize CARD library.
                this function is called in OS_Init().

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARD_Init(void)
{
    CARDiCommon *const p = &cardi_common;

    if (!p->flag)
    {
        p->flag = CARD_STAT_INIT;
        p->src = p->dst = p->len = 0;
        p->dma = (u32)~0;
        p->callback = NULL;
        p->callback_arg = NULL;

        cardi_rom_base = 0;

        CARDi_InitCommon();

        rom_stat.read_func = CARDi_GetRomAccessor();

#if !defined(SDK_SMALL_BUILD)
        //---- for detect pulled out card
        CARD_InitPulledOutCallback();
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WaitRomAsync

  Description:  wait for completion of current asynchronous ROM operation.

  Arguments:    None.

  Returns:      if the newest operation is successful, TRUE.
 *---------------------------------------------------------------------------*/
void CARD_WaitRomAsync(void)
{
    (void)CARDi_WaitAsync();
}

/*---------------------------------------------------------------------------*
  Name:         CARD_TryWaitRomAsync

  Description:  check if asynchronous ROM operation has been completed.

  Arguments:    None.

  Returns:      if operation has been completed, TRUE.
 *---------------------------------------------------------------------------*/
BOOL CARD_TryWaitRomAsync(void)
{
    return CARDi_TryWaitAsync();
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_GetRomAccessor

  Description:  ��������ƂɈقȂ�J�[�h�A�N�Z�X�֐��ւ̃|�C���^��Ԃ�.

  Arguments:    None.

  Returns:      TS �łł��邱�̎����ɂ����Ă͏�� CARDi_ReadCard().
 *---------------------------------------------------------------------------*/
void    (*CARDi_GetRomAccessor(void)) (CARDRomStat *)
{
    return CARDi_ReadCard;
}
