/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay
  File:     func.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: func.h,v $
  Revision 1.5  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.3  2004/09/29 07:01:29  yosizaki
  add comments.
  add extern "C".
  change overlay functions' interface.

  Revision 1.2  2004/09/09 05:04:45  yosizaki
  add macro OVERLAY_CODE_BY_XXX

  Revision 1.1  2004/09/06 04:36:21  yosizaki
  (none)

  Revision 1.1  2004/04/02 12:21:22  yosizaki
  (none)


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __NITROSDK_DEMO_FS_OVERLAY_FUNC_H
#define __NITROSDK_DEMO_FS_OVERLAY_FUNC_H

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
