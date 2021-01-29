/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - libfsdemo
  File:     util.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.h,v $
  Revision 1.4  2006/01/18 02:12:38  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:32  yosizaki
  do-indent.

  Revision 1.2  2004/07/12 13:15:26  yosizaki
  add some functions.

  Revision 1.1  2004/07/09 00:57:36  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if	!defined(NITRO_DEMOS_FS_LIBFSDEMO_UTIL_H_)
#define NITRO_DEMOS_FS_LIBFSDEMO_UTIL_H_


#include <nitro.h>


#if	defined(__cplusplus)
extern  "C"
{
#endif


/* �A���[�i�� V �u�����N�n���h���̏����� */
    void    LIBFSDEMO_Init(void);

/* fgets �Ɠ��� */
    char   *LIBFSDEMO_fgets(char *buf, int len, FSFile *p_file);

/* strlen �Ɠ��� */
    int     LIBFSDEMO_strlen(const char *str);


/* �w��f�B���N�g���ȉ��̈ꗗ���f�o�b�O�o�� */
    void    LIBFSDEMO_DumpDirectory(FSFile *p_dir);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


#endif                                 /* NITRO_DEMOS_FS_LIBFSDEMO_UTIL_H_ */
