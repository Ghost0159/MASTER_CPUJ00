/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - libraries
  File:     mi_cache.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro/math.h>
#include <nitro/mi/memory.h>
#include <nitro/mi/cache.h>


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         MI_InitCache

  Description:  �������L���b�V����������.

  Arguments:    cache            ����������MICache�\����.
                page             1�y�[�W������̃o�b�t�@�T�C�Y.
                                 4�ȏ��2�ׂ̂���ł���K�v������.
                buffer           �y�[�W�Ǘ����Ɏg�p����o�b�t�@.
                length           buffer�̃T�C�Y.
                                 length / (sizeof(MICachePage) + page)
                                 �̐������̃y�[�W���X�g�ɕ��������.
                                 �e�y�[�W(N=0,1,...)�̃o�b�t�@�擪�A�h���X��
                                 (buffer + N * page) �ƂȂ邱�Ƃ��ۏ؂����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MI_InitCache(MICache *cache, u32 page, void *buffer, u32 length)
{
    /* ���݂̎����̓s����, ���[�h�T�C�Y��菬���ȃy�[�W�͔�Ή� */
    SDK_ASSERT(page >= sizeof(u32));
    /* �����o������ */
    cache->pagewidth = MATH_CTZ(page);
    cache->valid_total = 0;
    cache->invalid_total = 0;
    cache->loading_total = 0;
    cache->valid = NULL;
    cache->invalid = NULL;
    cache->loading = NULL;
    /* �y�[�W���� */
    {
        u32             total = length / (sizeof(MICachePage) + page);
        u8             *buf = (u8*)buffer;
        MICachePage   *inf = (MICachePage*)&buf[total * page];
        cache->invalid_total += total;
        for (; total > 0; --total)
        {
            inf->offset = 0;
            inf->buffer = buf;
            inf->next = cache->invalid;
            cache->invalid = inf;
            inf += 1;
            buf += page;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_TouchCachePages

  Description:  �w�肵���y�[�W�͈͂̃��[�h������v������.
                �������X�g����ł���ΗL�����X�g���I�[����j������.

  Arguments:    cache            MICache�\����.
                head             ���[�h�Ώۂ̐擪�y�[�W�ԍ�.
                bitset           ���[�h�Ώۃy�[�W�̃r�b�g�Z�b�g.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_TouchCachePages(MICache *cache, u32 head, u32 bitset)
{
    {
        PLATFORM_ENTER_CRITICALSECTION();
        MICachePage  **load;
        /* �v�����X�g���������č���Ƃ̏d����������Ώ��O */
        for (load = &cache->loading; *load; load = &(*load)->next)
        {
            MICachePage *p = *load;
            u32     pos = p->offset - head;
            if ((pos < 32) && ((bitset & (1 << pos)) != 0))
            {
                bitset &= ~(1UL << pos);
            }
        }
        /* �������X�g���s������ꍇ, �L�����X�g���I�[����j�� */
        {
            int     rest = MATH_CountPopulation(bitset) - cache->invalid_total;
            if (rest > 0)
            {
                int             del = cache->valid_total;
                MICachePage  **valid;
                for (valid = &cache->valid; *valid; valid = &(*valid)->next)
                {
                    if (--del < rest)
                    {
                        MICachePage **pp;
                        for (pp = &cache->invalid; *pp; pp = &(*pp)->next)
                        {
                        }
                        *pp = *valid;
                        *valid = NULL;
                        cache->valid_total -= rest;
                        cache->invalid_total += rest;
                        break;
                    }
                }
            }
        }
        /* �������X�g�̐擪����v�����X�g�̏I�[�ֈړ� */
        while (cache->invalid && bitset)
        {
            MICachePage *p = cache->invalid;
            u32     pos = MATH_CTZ(bitset);
            cache->invalid = p->next;
            p->offset = head + pos;
            p->next = NULL;
            *load = p;
    	    load = &p->next;
            --cache->invalid_total;
            --cache->loading_total;
            bitset &= ~(1UL << pos);
        }
        PLATFORM_LEAVE_CRITICALSECTION();
    }
    /* �y�[�W������ΓI�ɕs������ꍇ�͎g�p���@���s���Ȃ̂�1�񂾂��x�� */
    if (bitset)
    {
        static BOOL output_once = FALSE;
        if (!output_once)
        {
            output_once = TRUE;
            OS_TWarning("not enough cache-page! "
                        "MI_TouchCache() failed. "
                        "(total pages = %d, but need more %d)",
                        cache->invalid_total + cache->valid_total,
                        MATH_CountPopulation(bitset));
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         MI_ReadCache

  Description:  �L���b�V������f�[�^��ǂݏo��.
                �q�b�g�����y�[�W�͗L�����X�g�̐擪�Ɉړ�����.

  Arguments:    cache            MICache�\����.
                buffer           �]���惁����.
                                 NULL���w�肵���ꍇ��, �f�[�^��ǂݏo����
                                 �P�ɊY���͈͑S�̂̃L���b�V�������̂ݗv������.
                offset           �]�����I�t�Z�b�g.
                length           �]���T�C�Y.

  Returns:      �S�̈悪�L���b�V���Ƀq�b�g�����TRUE.
 *---------------------------------------------------------------------------*/
BOOL MI_ReadCache(MICache *cache, void *buffer, u32 offset, u32 length)
{
    BOOL    retval = TRUE;

    /* 32�y�[�W�P�ʂŐ擪���珇�ɔ��� */
    const u32   unit = (1UL << cache->pagewidth);
    u32     head = (offset >> cache->pagewidth);
    u32     tail = ((offset + length + unit - 1UL) >> cache->pagewidth);
    u32     pages;
    for (; (pages = MATH_MIN(tail - head, 32UL)), (pages > 0); head += pages)
    {
        /* �L�����X�g����Y���y�[�W������ */
        u32     bitset = (1UL << pages) - 1UL;
        {
            PLATFORM_ENTER_CRITICALSECTION();
            MICachePage **pp;
            for (pp = &cache->valid; *pp && bitset; pp = &(*pp)->next)
            {
                MICachePage *p = *pp;
                u32     pos = p->offset - head;
                if ((pos < pages) && ((bitset & (1UL << pos)) != 0))
                {
                    if (buffer)
                    {
                        /*
                         * TODO:
                         *     �璷�ȃ��X�g������������L���Ȏ������ۂĂ�̂�
                         *     �q�b�g�����y�[�W��L�����X�g�̐擪�ֈړ������ق���
                         *     �p�t�H�[�}���X�͂�����x���シ�邩������Ȃ�.
                         *     (�ύX�̍ۂ͂��� for ���[�v���l���̂���!)
                         */
                        u32     len = unit;
                        int     src = 0;
                        int     dst = (int)((p->offset << cache->pagewidth) - offset);
                        if (dst < 0)
                        {
                            len += dst;
                            src -= dst;
                            dst = 0;
                        }
                        if (dst + len > length)
                        {
                            len = length - dst;
                        }
                        MI_CpuCopy8(&p->buffer[src], &((u8*)buffer)[dst], len);
                    }
                    bitset &= ~(1UL << pos);
                }
            }
            PLATFORM_LEAVE_CRITICALSECTION();
        }
        /* �y�[�W�t�H���g���������ꍇ */
        if (bitset)
        {
            retval = FALSE;
            WFSi_TouchCachePages(cache, head, bitset);
        }
    }
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         MI_LoadCache

  Description:  ���[�h�v�����X�g�ɑ��݂���S�y�[�W�̃��[�h���������s.
                ���[�h�v�����X�g����ł������ꍇ�͉��������������ɐ����Ԃ�,
                �Ăяo�����Ƀ��[�h�v�����X�g�֒ǉ����ꂽ�ꍇ�͂������������.

  Note:         ���̊֐��̓f�o�C�X���u���b�L���O���Ă��悢�R���e�L�X�g����
                �K�؂ȃ^�C�~���O�ŌĂяo���K�v������.
                ���Ȃ킿�A���荞�݃n���h���Ȃǂ���Ăяo���Ă͂Ȃ�Ȃ�.

  Arguments:    cache            MICache�\����.
                device           ���[�h�ΏۂƂȂ�f�o�C�X.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MI_LoadCache(MICache *cache, MIDevice *device)
{
    for (;;)
    {
        /* ���[�h�v�����X�g�̐擪���擾 */
        MICachePage *p = cache->loading;
        if (!p)
        {
            break;
        }
        /* �f�o�C�X����y�[�W��ǂݍ��� */
        (void)MI_ReadDevice(device, p->buffer,
                            (p->offset << cache->pagewidth),
                            (1UL << cache->pagewidth));
        /* ���[�h���������y�[�W��L�����X�g�̐擪�֑}�� */
        {
            PLATFORM_ENTER_CRITICALSECTION();
            cache->loading = p->next;
            p->next = cache->valid;
            cache->valid = p;
            ++cache->valid_total;
            ++cache->loading_total;
            PLATFORM_LEAVE_CRITICALSECTION();
        }
    }
}


/*---------------------------------------------------------------------------*
  $Log: mi_cache.c,v $
  Revision 1.1  2007/04/11 08:06:25  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
