/*---------------------------------------------------------------------------*
  Project:  NitroSDK - include
  File:     platform.h

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: platform.h,v $
  Revision 1.3  2007/08/22 05:22:32  yosizaki
  fix dependency.

  Revision 1.2  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.1  2006/06/27 08:21:58  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_PLATFORM_HEADER__
#define NITRO_PLATFORM_HEADER__


// for OS_DisableInterrupts()
#include <nitro/os/common/system.h>


/*****************************************************************************/
/* ���ʂ̒�` */

/* �G���f�B�A���܂��̓r�b�g�����̒�` */
#define PLATFORM_ENDIAN_BIG     0
#define PLATFORM_ENDIAN_LITTLE  1


/*****************************************************************************/
/* �v���b�g�t�H�[���ŗL�̒�` */

/* �G���f�B�A�� (PLATFORM_ENDIAN_*) */
#define PLATFORM_BYTES_ENDIAN           PLATFORM_ENDIAN_LITTLE

/* �f�[�^�A�N�Z�X���ɃA�N�Z�X���ł̋��E�������K�v�Ȃ� 1, �����łȂ��Ȃ� 0 */
#define PLATFORM_BYTES_ALIGN            1

/* �v���O�������ӎ�����K�v�̂���L���b�V�����C���̃T�C�Y (�s�v�Ȃ�1) */
#define PLATFORM_CACHE_SIZE             32

/* �L���b�V�����C���̋��E�������K�v�ȕϐ��Ɏw�肷��C���q (�s�v�Ȃ��̒�`) */
#define PLATFORM_ATTRIBUTE_CACHEALIGN   ATTRIBUTE_ALIGN(32)

/* �u���b�N�̐擪�ŌĂяo���\�Ȋ��荞�݋֎~�֐� */
#define PLATFORM_ENTER_CRITICALSECTION()    OSIntrMode bak_interrupt_mode_ = OS_DisableInterrupts()

/* ��L�֐��Ƒ΂ɂȂ銄�荞�݉����֐� */
#define PLATFORM_LEAVE_CRITICALSECTION()    (void)OS_RestoreInterrupts(bak_interrupt_mode_)


/*****************************************************************************/
/* �����n�ŗL�̒�` */

#if	defined(SDK_CW) || defined(__MWERKS__)

/* �r�b�g�t�B�[���h���l�߂��鏇�� (PLATFORM_ENDIAN_*) */
#define PLATFORM_BITFIELDS_ENDIAN       PLATFORM_ENDIAN_LITTLE

/* �R���p�C�����̐���������}�N�� */
#define PLATFORM_COMPILER_ASSERT(expr) \
    extern void platform_compiler_assert ## __LINE__ (char is[(expr) ? +1 : -1])

/*
 * �\���̂��ő僁���o�T�C�Y���傫�Ȓl�Ńp�f�B���O����Ȃ��悤
 * �����I�ɍ\���̒�`�� } �ȍ~�֎w�肷��C���q.
 */
#define PLATFORM_STRUCT_PADDING_FOOTER

/* �֐��̃C�����C���w��q */
#define PLATFORM_ATTRIBUTE_INLINE       SDK_INLINE


#else
# TO BE DEFINED
#endif


/*****************************************************************************/
/* ���ʂ̌^��` */

/*
 * �T�C�Y�w�萮���\����.
 *
 * �����͊O���f�o�C�X��ʐM�H���o�R���Č�������鐮���f�[�^�ɑ΂���
 * �s�p�ӂȒ��ڃA�N�Z�X�̋֎~��\������ړI�őg�ݍ��݌^�̂����Ɏg�p�����.
 * �g�ݍ��݌^���炱�̍\���̂֊i�[����ۂɂ� MI_Store* �Ȃǂ̊֐���,
 * ���̍\���̂���l���Q�Ƃ���ۂɂ� MI_Load* �Ȃǂ̊֐����g�p����.
 */

/* 8bit ���g���G���f�B�A���^ */
typedef struct PLATFORM_LE8
{
    unsigned char byte[1];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_LE8;

/* 16bit ���g���G���f�B�A���^ */
typedef struct PLATFORM_LE16
{
    unsigned char byte[2];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_LE16;

/* 32bit ���g���G���f�B�A���^ */
typedef struct PLATFORM_LE32
{
    unsigned char byte[4];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_LE32;

/* 8bit �r�b�O�G���f�B�A���^ */
typedef struct PLATFORM_BE8
{
    unsigned char byte[1];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_BE8;

/* 16bit �r�b�O�G���f�B�A���^ */
typedef struct PLATFORM_BE16
{
    unsigned char byte[2];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_BE16;

/* 32bit �r�b�O�G���f�B�A���^ */
typedef struct PLATFORM_BE32
{
    unsigned char byte[4];
}
PLATFORM_STRUCT_PADDING_FOOTER PLATFORM_BE32;


/*****************************************************************************/


#endif /* NINTENDO_PLATFORM_HEADER__ */
