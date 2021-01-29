/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_cache.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_cache.c,v $
  Revision 1.6  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.5  2005/12/21 14:25:56  yosizaki
  fix about page-cache.

  Revision 1.4  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.2  2004/11/11 11:51:13  yosizaki
  change argument of MBi_ReadFromCache. (const_cast)
  add member 'is_hit' to avoid endless cache-missing.

  Revision 1.1  2004/11/10 13:14:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "mb_cache.h"


/*---------------------------------------------------------------------------*
  Name:         MBi_InitCache

  Description:  �L���b�V�����X�g������������.
                

  Arguments:    pl         �L���b�V�����X�g

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_InitCache(MBiCacheList * pl)
{
    MI_CpuClear8(pl, sizeof(*pl));
}

/*---------------------------------------------------------------------------*
  Name:         MBi_AttachCacheBuffer

  Description:  �L���b�V�����X�g�փo�b�t�@�����蓖�Ă�.
                
  Arguments:    pl         �L���b�V�����X�g
                ptr        ���蓖�Ă�o�b�t�@
                src        ptr �̃\�[�X�A�h���X
                len        ptr �̃o�C�g�T�C�Y
                state      �w�肷�鏉�����
                           (MB_CACHE_STATE_READY �� MB_CACHE_STATE_LOCKED)

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBi_AttachCacheBuffer(MBiCacheList * pl, u32 src, u32 len, void *ptr, u32 state)
{
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    {
        /* ���o�^�̃y�[�W������ */
        MBiCacheInfo *pi = pl->list;
        for (;; ++pi)
        {
            if (pi >= &pl->list[MB_CACHE_INFO_MAX])
            {
                OS_TPanic("MBi_AttachCacheBuffer() failed! (over maximum count)");
            }
            if (pi->state == MB_CACHE_STATE_EMPTY)
            {
                pi->src = src;
                pi->len = len;
                pi->ptr = (u8 *)ptr;
                pi->state = state;
                break;
            }
        }
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_ReadFromCache

  Description:  �w�肵���A�h���X�̓��e���L���b�V�����烊�[�h����.

  Arguments:    pl         �L���b�V�����X�g
                src        ���[�h���A�h���X
                len        ���[�h�T�C�Y (BYTE)
                dst        ���[�h��A�h���X.

  Returns:      �L���b�V���Ƀq�b�g����΃��[�h���� TRUE, �����łȂ���� FALSE.
 *---------------------------------------------------------------------------*/
BOOL MBi_ReadFromCache(MBiCacheList * pl, u32 src, void *dst, u32 len)
{
    BOOL    ret = FALSE;
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    {
        /* �g�p�\�ȃy�[�W�݂̂����� */
        const MBiCacheInfo *pi = pl->list;
        for (; pi < &pl->list[MB_CACHE_INFO_MAX]; ++pi)
        {
            if (pi->state >= MB_CACHE_STATE_READY)
            {
                /* �ΏۃA�h���X���͈͓������� */
                const int ofs = (int)(src - pi->src);
                if ((ofs >= 0) && (ofs + len <= pi->len))
                {
                    /* �L���b�V���q�b�g�����̂Ń��[�h */
                    MI_CpuCopy8(pi->ptr + ofs, dst, len);
                    pl->lifetime = 0;
                    ret = TRUE;
                    break;
                }
            }
        }
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         MBi_TryLoadCache

  Description:  �w�肵���A�h���X�̓��e���L���b�V���փ��[�h����.
                �ł��Ԓn�̏����� READY �̃y�[�W�L���b�V�����j�������.

  Arguments:    pl         �L���b�V�����X�g
                src        �����[�h���A�h���X
                len        �����[�h�T�C�Y (BYTE)

  Returns:      �����[�h���J�n�ł���� TRUE, �����łȂ���� FALSE.
                (�����[�h�����̃G���W�����V�X�e�����ŒP��̏ꍇ,
                 �O��̏����𖢗��Ȃ炱�̊֐��� FALSE ��Ԃ��ł��낤)
 *---------------------------------------------------------------------------*/
BOOL MBi_TryLoadCache(MBiCacheList * pl, u32 src, u32 len)
{
    BOOL    ret = FALSE;
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    {
        /* �����[�h�\�ȃy�[�W�݂̂����� */
        MBiCacheInfo *trg = NULL;
        MBiCacheInfo *pi = pl->list;
        for (; pi < &pl->list[MB_CACHE_INFO_MAX]; ++pi)
        {
            if (pi->state == MB_CACHE_STATE_READY)
            {
                /* �ł��Ԓn����������Ό��Ɏc�� */
                if (!trg || (trg->src > pi->src))
                {
                    trg = pi;
                }
            }
        }
        /* �����[�h�Ώۂ̃y�[�W��������Ώ����v�� */
        if (trg)
        {
            /* �����Ƀ^�X�N�X���b�h�ւ̏����v�����L�q */
            (void)src;
            (void)len;
        }
        OS_TPanic("reload-system is not yet!");
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
    return ret;
}
