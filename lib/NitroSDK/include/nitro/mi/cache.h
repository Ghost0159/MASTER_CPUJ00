/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MI - include
  File:     cache.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
#ifndef	NITRO_MI_CACHE_H_
#define	NITRO_MI_CACHE_H_


#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/platform.h>
#include <nitro/mi/device.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*/
/* declarations */

/* �������L���b�V�����\������X�̃y�[�W��� */
typedef struct MICachePage
{
    struct MICachePage *next;   /* ���̗v�f (���X�g�̂��߂Ɏg�p) */
    u32                 offset; /* �y�[�W�擪�ʒu (�y�[�W�T�C�Y�P��) */
    u8                 *buffer; /* �y�[�W�o�b�t�@ */
}
MICachePage;

/* �������L���b�V�����\���� */
typedef struct MICache
{
    u32             pagewidth;
    MICachePage    *valid;
    MICachePage    *invalid;
    MICachePage    *loading;
    int             valid_total;
    int             invalid_total;
    int             loading_total;
}
MICache;

/* �w�肵���y�[�W�T�C�Y�Ɩ�������������̂ɕK�v�ȃo�b�t�@�T�C�Y�̎Z�o�}�N�� */
#define MI_CACHE_BUFFER_WORKSIZE(page, total) \
        ((sizeof(MICachePage) + (page)) * (total))


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
void    MI_InitCache(MICache *cache, u32 page, void *buffer, u32 length);

/*---------------------------------------------------------------------------*
  Name:         MI_ReadCache

  Description:  �L���b�V������f�[�^��ǂݏo��.
                �q�b�g�����y�[�W�͗L�����X�g�̐擪�Ɉړ�����.
                �q�b�g���Ȃ������y�[�W�̓��[�h�v�����X�g�ɒǉ������.

  Arguments:    cache            MICache�\����.
                buffer           �]���惁����.
                                 NULL���w�肵���ꍇ��, �f�[�^��ǂݏo����
                                 �P�ɊY���͈͑S�̂̃L���b�V�������̂ݗv������.
                offset           �]�����I�t�Z�b�g.
                length           �]���T�C�Y.

  Returns:      �S�̈悪�L���b�V���Ƀq�b�g�����TRUE.
 *---------------------------------------------------------------------------*/
BOOL    MI_ReadCache(MICache *cache, void *buffer, u32 offset, u32 length);

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
void    MI_LoadCache(MICache *cache, MIDevice *device);

/*---------------------------------------------------------------------------*
  Name:         MI_IsCacheLoading

  Description:  ���[�h�v�����X�g����łȂ�������.
                ���[�h�v�����X�g��MI_ReadCache�֐��̌Ăяo���Œǉ�����
                MI_LoadCache�֐��̌Ăяo���ŋ�ɂȂ�.

  Arguments:    cache            MICache�\����.

  Returns:      ���[�h�v�����X�g����łȂ����TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL    MI_IsCacheLoading(const MICache *cache)
{
    return (cache->loading != NULL);
}


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_MI_CACHE_H_ */
/*---------------------------------------------------------------------------*
  $Log: cache.h,v $
  Revision 1.1  2007/04/11 08:04:56  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
