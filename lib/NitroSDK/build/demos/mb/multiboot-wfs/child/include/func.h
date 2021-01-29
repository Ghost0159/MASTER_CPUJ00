/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     func.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: func.h,v $
  Revision 1.1  2005/06/23 09:05:39  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __NITROSDK_DEMO_MB_MULTIBOOT_WFS_FUNC_H
#define __NITROSDK_DEMO_MB_MULTIBOOT_WFS_FUNC_H

#include <nitro.h>


/******************************************************************************/
/* macro */

/*
 * �I�[�o�[���C���W���[���T�C�Y���璷�ɑ���������ړI�̃}�N��.
 * volatile int v; OVAERLAY_CODE_BY_10000(++v);
 * ���̂悤�Ɏg�p���܂�.
 * �r���h���Ԃ�(���ɍœK������)�ɒ[�ɒ����Ȃ�_�ɒ��ӂ��Ă�������.
 */
#define	OVAERLAY_CODE_BY_10(expr)		(expr), (expr), (expr), (expr), (expr), (expr), (expr), (expr), (expr), (expr)
#define	OVAERLAY_CODE_BY_100(expr)		OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_10(expr))
#define	OVAERLAY_CODE_BY_1000(expr)		OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_100(expr))
#define	OVAERLAY_CODE_BY_10000(expr)	OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_1000(expr))
#define	OVAERLAY_CODE_BY_100000(expr)	OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_10000(expr))
#define	OVAERLAY_CODE_BY_1000000(expr)	OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_100000(expr))
#define	OVAERLAY_CODE_BY_10000000(expr)	OVAERLAY_CODE_BY_10(OVAERLAY_CODE_BY_1000000(expr))


/******************************************************************************/
/* function */

#if	defined(__cplusplus)
extern  "C"
{
#endif


/* �I�[�o�[���C�������Ă��邱�Ƃ��m�F���邽�߂̕�������擾���܂� */
    void    func_1(char *dst);
    void    func_2(char *dst);
    void    func_3(char *dst);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


/******************************************************************************/


#endif                                 /* __NITROSDK_DEMO_FS_OVERLAY_FUNC_H */
