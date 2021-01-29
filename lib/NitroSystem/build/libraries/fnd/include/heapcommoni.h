/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - fnd
  File:     heapcommoni.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_FND_HEAPCOMMONI_H_
#define NNS_FND_HEAPCOMMONI_H_

#include <nitro/types.h>
#include <nnsys/fnd/config.h>

#ifdef __cplusplus
extern "C" {
#endif


/* =======================================================================
    �^��`
   ======================================================================== */

typedef s32 NNSiIntPtr;     // void*�|�C���^�Ƒ��ݕϊ��\�ȕ����t�������^
typedef u32 NNSiUIntPtr;    // void*�|�C���^�Ƒ��ݕϊ��\�ȕ������������^


/* ========================================================================
    �}�N���֐�
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndRoundUp

  Description:  �w�肵�����E�l�ɍ��킹�邽�߁A��ʕ����ɐ؂�グ�܂��B

  Arguments:    value:      �ΏۂƂȂ�f�[�^
                alignment:  ���E�l

  Returns:      �w�肵�����E�ɐ؂�グ��ꂽ�l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define NNSi_FndRoundUp(value, alignment) \
    (((value) + (alignment-1)) & ~(alignment-1))

#define NNSi_FndRoundUpPtr(ptr, alignment) \
    ((void*)NNSi_FndRoundUp(NNSiGetUIntPtr(ptr), alignment))

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndRoundDown

  Description:  �w�肵�����E�l�ɍ��킹�邽�߁A���ʕ����ɐ؂艺���܂��B

  Arguments:    value:      �ΏۂƂȂ�f�[�^
                alignment:  ���E�l

  Returns:      �w�肵�����E�ɐ؂艺����ꂽ�l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define NNSi_FndRoundDown(value, alignment) \
    ((value) & ~(alignment-1))

#define NNSi_FndRoundDownPtr(ptr, alignment) \
    ((void*)NNSi_FndRoundDown(NNSiGetUIntPtr(ptr), alignment))

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndGetBitValue

  Description:  ����̃r�b�g�ʒu�̒l���擾���܂��B

  Arguments:    data:  �擾����r�b�g�f�[�^���܂ރf�[�^�B
                st:    �J�n�r�b�g(0����n�܂�)�B
                bits:  �r�b�g��(31�ȉ��Ƃ���)�B

  Returns:      ����̃r�b�g�ʒu�̒l��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
#define     NNSi_FndGetBitValue(data, st, bits) \
    (((data) >>(st)) & ((1 <<(bits)) -1))

/*---------------------------------------------------------------------------*
  Name:         NNSi_FndSetBitValue

  Description:  ����̃r�b�g�ʒu�ɒl���Z�b�g���܂��B

  Arguments:    data:  �Z�b�g����r�b�g�f�[�^���i�[����ϐ��B
                st:    �J�n�r�b�g(0����n�܂�)�B
                bits:  �r�b�g��(31�ȉ��Ƃ���)�B
                val:   �Z�b�g����r�b�g�f�[�^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#define     NNSi_FndSetBitValue(data, st, bits, val)                        \
                do                                                          \
                {                                                           \
                    u32 maskBits = (u32)((1 <<(bits)) -1);                   \
                    u32 newVal = (val) & maskBits; /* ���S�̂��߃}�X�N */    \
                    (void)(maskBits <<= st);                                 \
                    (data) &= ~maskBits; /* �Z�b�g����̈���N���A */        \
                    (data) |= newVal <<(st);                                 \
                } while(FALSE);


/* ========================================================================
    inline�֐�
   ======================================================================== */

/* ------------------------------------------------------------------------
    �|�C���^����
   ------------------------------------------------------------------------ */

NNS_FND_INLINE NNSiUIntPtr
NNSiGetUIntPtr(const void* ptr)
{
    return (NNSiUIntPtr)ptr;
}

NNS_FND_INLINE u32
GetOffsetFromPtr(const void* start, const void* end)
{
    return NNSiGetUIntPtr(end) - NNSiGetUIntPtr(start);
}

NNS_FND_INLINE void*
AddU32ToPtr(void* ptr, u32 val)
{
    return (void*)( NNSiGetUIntPtr(ptr) + val );
}

NNS_FND_INLINE const void*
AddU32ToCPtr(const void* ptr, u32 val)
{
    return (const void*)( NNSiGetUIntPtr(ptr) + val );
}

NNS_FND_INLINE void*
SubU32ToPtr(void* ptr, u32 val)
{
    return (void*)( NNSiGetUIntPtr(ptr) - val );
}

NNS_FND_INLINE const void*
SubU32ToCPtr(const void* ptr, u32 val)
{
    return (const void*)( NNSiGetUIntPtr(ptr) - val );
}

NNS_FND_INLINE int
ComparePtr(const void* a, const void* b)
{
    const u8* wa = a;
    const u8* wb = b;

    return wa - wb;
}


NNS_FND_INLINE u16
GetOptForHeap(const NNSiFndHeapHead* pHeapHd)
{
    return (u16)NNSi_FndGetBitValue(pHeapHd->attribute, 0, 8);
}

NNS_FND_INLINE void
SetOptForHeap(
    NNSiFndHeapHead*    pHeapHd,
    u16                 optFlag
)
{
    NNSi_FndSetBitValue(pHeapHd->attribute, 0, 8, optFlag);
}


/* ------------------------------------------------------------------------
    �������[�U
   ------------------------------------------------------------------------ */

NNS_FND_INLINE void
FillAllocMemory(
    NNSiFndHeapHead*    pHeapHd,
    void*               address,
    u32                 size
)
{

    if (GetOptForHeap(pHeapHd) & NNS_FND_HEAP_OPT_0_CLEAR)
    {
        MI_CpuFill32(address, 0, size);
    }
    else
    {
        #if ! defined(NNS_FINALROM)
            if (GetOptForHeap(pHeapHd) & NNS_FND_HEAP_OPT_DEBUG_FILL)
            {
                MI_CpuFill32(address, NNS_FndGetFillValForHeap(NNS_FND_HEAP_FILL_ALLOC), size);
            }
        #endif
    }
}

#if defined(NNS_FINALROM)
    #define FillFreeMemory(pHeapHd, address, size)  ((void) 0)
#else
    NNS_FND_INLINE void
    FillFreeMemory(
        NNSiFndHeapHead*    pHeapHd,
        void*               address,
        u32                 size
    )
    {
        if (GetOptForHeap(pHeapHd) & NNS_FND_HEAP_OPT_DEBUG_FILL)
        {
            MI_CpuFill32(address, NNS_FndGetFillValForHeap(NNS_FND_HEAP_FILL_FREE), size);
        }
    }
#endif

#if defined(NNS_FINALROM)
    #define FillNoUseMemory(pHeapHd, address, size)  ((void) 0)
#else
    NNS_FND_INLINE void
    FillNoUseMemory(
        NNSiFndHeapHead*    pHeapHd,
        void*               address,
        u32                 size
    )
    {
        if (GetOptForHeap(pHeapHd) & NNS_FND_HEAP_OPT_DEBUG_FILL)
        {
            MI_CpuFill32(address, NNS_FndGetFillValForHeap(NNS_FND_HEAP_FILL_NOUSE), size);
        }
    }
#endif


/* =======================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

void        NNSi_FndInitHeapHead(
                NNSiFndHeapHead*    pHeapHd,
                u32                 signature,
                void*               heapStart,
                void*               heapEnd,
                u16                 optFlag);

void        NNSi_FndFinalizeHeap(
                NNSiFndHeapHead* pHeapHd);

void        NNSi_FndDumpHeapHead(
                NNSiFndHeapHead* pHeapHd);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_FND_HEAPCOMMONI_H_ */
#endif
