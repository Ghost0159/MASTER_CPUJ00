/*---------------------------------------------------------------------------*
  Project:  RevolutionDWC public include file
  File:     ./nonport/pc/dwci_np_types.h

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/

#ifndef DWCI_NP_TYPES_H_
#define DWCI_NP_TYPES_H_

// Endian
#define SDK_LITTLE_ENDIAN
#define SDK_IS_LITTLE_ENDIAN	1
#define SDK_IS_BIG_ENDIAN	0

#ifdef  SDK_ASM
#else  //SDK_ASM

#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned char u8;
    typedef unsigned short int u16;
    typedef unsigned long u32;

#ifdef WIN32
    typedef unsigned __int64 u64;
#else
typedef unsigned long long int u64;
#endif

    typedef signed char s8;
    typedef signed short int s16;
    typedef signed long s32;

#ifdef WIN32
    typedef signed __int64 s64;
#else
typedef signed long long int s64;
#endif

    typedef volatile u8 vu8;
    typedef volatile u16 vu16;
    typedef volatile u32 vu32;
    typedef volatile u64 vu64;

    typedef volatile s8 vs8;
    typedef volatile s16 vs16;
    typedef volatile s32 vs32;
    typedef volatile s64 vs64;

    typedef float f32;
    typedef volatile f32 vf32;

    /*
    io_register_list_XX.h�Ŏg�p����}�N���ƌ^
    */

    typedef u8 REGType8;
    typedef u16 REGType16;
    typedef u32 REGType32;
    typedef u64 REGType64;

    typedef vu8 REGType8v;
    typedef vu16 REGType16v;
    typedef vu32 REGType32v;
    typedef vu64 REGType64v;


#ifndef SDK_BOOL_ALREADY_DEFINED_
#ifndef BOOL
    typedef int BOOL;
#endif //BOOL
#endif //SDK_ALREAD_DEFINE_BOOL_

#ifndef TRUE
    // Any non zero value is considered TRUE
#define TRUE                    1
#endif //TRUE

#ifndef FALSE
#define FALSE                   0
#endif // FALSE


#ifndef NULL
#ifdef  __cplusplus
#define NULL                    0
#else  // __cplusplus
#define NULL                ((void *)0)
#endif // __cplusplus
#endif // NULL

    // for compatibility with GAMECUBE
#if     defined(SDK_CW) || defined(__MWERKS__)
#ifndef ATTRIBUTE_ALIGN
#define ATTRIBUTE_ALIGN(num) __attribute__ ((aligned(num)))
#endif
#endif

    // Weak symbol
#if     defined(SDK_CW) || defined(__MWERKS__)
#define SDK_WEAK_SYMBOL  __declspec(weak)
#elif   defined(SDK_PRODG)
#define SDK_WEAK_SYMBOL
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif //SDK_ASM

/* NITRO_TYPES_H_ */
#endif


/* static inline �� inline �ɂ��āA�R�[�h�T�C�Y���팸�������ꍇ�́A�}�N�� SDK_INLINE ��ύX���Ă��������B
   (static inline �� inline �ɂ����֐��ɐ錾������ꍇ�́ASDK_DECL_INLINE ���ύX���Ă��������B)
   �ύX�����ꍇ�̓��C�u�������ăr���h���Ă��������B

   �������Ainline �֐��̎��̉��̕s����C�����ꂽ�o�[�W�����̃R���p�C�����g�p���Ă���
   ���̕s���������邽�߂� inline �� static inline �ɂ��Ă����֐��̏ꍇ�̂�
   ���̃}�N�����g�p���� static inline �� inline �ɕύX����ƃR�[�h�T�C�Y���팸���邱�Ƃ��ł��܂��B

   ���ӁF���̃}�N���͌��i�K�ł́AFX_, VEC_, MTX_ �V���[�Y�̂ݗL���ƂȂ��Ă��܂��B*/
#define SDK_INLINE      static inline
#define SDK_DECL_INLINE static

/// 1970�N����̌o�ߕb��
typedef s64 DWCUtcTime;

//---- unit of tick
typedef u32 DWCTick;

// C�W����tm�ƌ݊����̂���^
typedef struct
{
    int	tm_sec;
    int	tm_min;
    int	tm_hour;
    int	tm_mday;
    int	tm_mon;
    int	tm_year;
    int	tm_wday;
    int	tm_yday;
    int	tm_isdst;
}
DWCTmTime;




