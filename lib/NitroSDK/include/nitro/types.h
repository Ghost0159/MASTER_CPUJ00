/*---------------------------------------------------------------------------*
  Project:  NitroSDK - - types definition
  File:     types.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: types.h,v $
  Revision 1.40  2006/09/08 00:47:41  okubata_ryoma
  enumsalwaysint��gcc����include����Ă���ꍇ�͗L���ɂȂ�Ȃ��悤�ɕύX

  Revision 1.39  2006/09/07 06:22:28  okubata_ryoma
  add #pragma enumsalwaysint on

  Revision 1.38  2006/03/17 00:33:54  yosizaki
  support SDK_FORCE_EXPORT

  Revision 1.37  2006/02/24 00:53:29  yosizaki
  move position of SDK_INLINE definition.

  Revision 1.36  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.35  2005/11/14 05:17:25  okubata_ryoma
  ���C��

  Revision 1.33  2005/09/05 01:39:14  yasu
  SDK_BOOL_ALREADY_DEFINED_�@�̒ǉ�

  Revision 1.32  2005/06/23 12:16:27  yasu
  SDK_LITTLE_ENDIAN ��`�̒ǉ�

  Revision 1.31  2005/03/04 10:13:03  yasu
  �R���p�C���ˑ���������������

  Revision 1.30  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.29  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.28  2004/08/03 03:14:29  yasu
  Fix comment

  Revision 1.27  2004/05/14 11:57:44  takano_makoto
  Add option SDK_HAS_NO_LONG_LONG_INT_ for environment of VC or BCB.

  Revision 1.26  2004/04/23 04:24:14  yada
  add comment for ATTRIBUTE_ALIGN

  Revision 1.25  2004/04/23 04:14:31  yada
  add ATTRIBUTE_ALIGN

  Revision 1.24  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.23  2004/01/29 08:47:04  nishida_kenji
  Change typedefs for better type check.
  s32: signed int -> signed long
  u32: unsigned int -> unsigned long
  BOOL: s32 -> int

  Revision 1.22  2004/01/05 04:11:16  nishida_kenji
  add f32, vf32

  Revision 1.21  2003/12/02 12:09:22  yasu
  �A�Z���u���̑g�ݍ��݃v���v���Z�b�T�� #<SPACE> ���R�����g�����Ă��܂����Ƃ̑Ώ�

  Revision 1.20  2003/11/27 06:35:17  yada
  comment����

  Revision 1.19  2003/11/11 09:14:41  nishida_kenji
  RegTypeNNv�^��typedef

  Revision 1.18  2003/11/11 06:10:18  nishida_kenji
  �Œ菬����s���gx/gxcommon.h�Ɉړ�

  Revision 1.17  2003/11/11 05:46:19  yada
  BOOL, TRUE, FALSE ��ǉ��BINLINE, SDK_ASSERT() �𕪗��B

  Revision 1.16  2003/11/11 04:12:52  nishida_kenji
  fx16rs�̒ǉ�

  Revision 1.15  2003/11/05 07:40:40  yasu
  VecFx16 �� padding �x����ق点��

  Revision 1.14  2003/11/04 12:04:59  yasu
  aligned �̒ǉ��@������ CW �̂�

  Revision 1.13  2003/11/04 11:45:20  yasu
  NULL �Ȃǂ̒ǉ�

  Revision 1.12  2003/11/04 09:37:03  yada
  REG_BASE �O���܂����B

  Revision 1.11  2003/11/04 04:52:20  Nishida_Kenji
  �r�b�g�t�B�[���h���~�߂�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_TYPES_H_
#define NITRO_TYPES_H_

// enum �� int �^�Ɠ����T�C�Y�łȂ��� SDK ������ɓ��삵�Ȃ�����
#ifdef __MWERKS__
#pragma enumsalwaysint on
#endif

// Endian
#define SDK_LITTLE_ENDIAN
#define SDK_IS_LITTLE_ENDIAN	1
#define SDK_IS_BIG_ENDIAN	0

#ifdef  SDK_ASM
#else  //SDK_ASM

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long u32;

#ifdef SDK_HAS_NO_LONG_LONG_INT_
typedef unsigned __int64 u64;
#else
typedef unsigned long long int u64;
#endif

typedef signed char s8;
typedef signed short int s16;
typedef signed long s32;

#ifdef SDK_HAS_NO_LONG_LONG_INT_
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

/* option for the compiler which deals dead-strip */
#ifdef  SDK_CW_FORCE_EXPORT_SUPPORT
#define SDK_FORCE_EXPORT   __declspec(force_export)
#else
#define SDK_FORCE_EXPORT   
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif //SDK_ASM


/* static inline �� inline �ɂ��āA�R�[�h�T�C�Y���팸�������ꍇ�́A�}�N�� SDK_INLINE ��ύX���Ă��������B
   (static inline �� inline �ɂ����֐��ɐ錾������ꍇ�́ASDK_DECL_INLINE ���ύX���Ă��������B)
   �ύX�����ꍇ�̓��C�u�������ăr���h���Ă��������B 
   
   �������Ainline �֐��̎��̉��̕s����C�����ꂽ�o�[�W�����̃R���p�C�����g�p���Ă���
   ���̕s���������邽�߂� inline �� static inline �ɂ��Ă����֐��̏ꍇ�̂�
   ���̃}�N�����g�p���� static inline �� inline �ɕύX����ƃR�[�h�T�C�Y���팸���邱�Ƃ��ł��܂��B
   
   ���ӁF���̃}�N���͌��i�K�ł́AFX_, VEC_, MTX_ �V���[�Y�̂ݗL���ƂȂ��Ă��܂��B*/
#define SDK_INLINE      static inline
#define SDK_DECL_INLINE static


/* NITRO_TYPES_H_ */
#endif
