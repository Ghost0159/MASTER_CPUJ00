/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - include
  File:     china.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: china.h,v $
  Revision 1.4  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.3  2005/05/27 08:57:52  yosizaki
  add OS_ShowAttentionChina.

  Revision 1.2  2005/05/14 01:11:28  terui
  OS_InitChina�֐���ISBN��������w�肷��p�����[�^��ǉ�

  Revision 1.1  2005/04/25 08:23:53  terui
  ������ł̐����Ή��̂��߁A�w�b�_��ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_OS_ARM9_CHINA_H_
#define NITRO_OS_ARM9_CHINA_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#define OS_BURY_STRING_FORCHINA         "[SDK+NINTENDO:FORCHINA]"
#define OS_BUSY_STRING_LEN_FORCHINA     23


/*---------------------------------------------------------------------------*
  Name:         OS_InitChina

  Description:  SDK �� OS ���C�u����������������B
                �w���n�������ł���A�v���P�[�V������p�B
                OS_Init �֐��̑���ɓ��֐����g�p����B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z����w�肷��B
                            {
                                char    ISBN[ 13 ] ,
                                char    �����o�L��[ 12 ] ,
                                char    �V�o����(��)[ 4 ] ,
                                char    �V�o����(�E)[ 4 ]
                            }

  Returns:      None
 *---------------------------------------------------------------------------*/
void    OS_InitChina(const char **isbn);

/*---------------------------------------------------------------------------*
  Name:         OS_ShowAttentionChina

  Description:  �����Ή����S�A�y�ђ��Ӊ�ʂ����莞�ԕ\������B
                ���̊֐��̓��[�U�A�v���P�[�V�������ōĒ�`���邱�Ƃ��ł���B

  Arguments:    isbn    -   ISBN �ԍ����Ɋւ��镶����z��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    OS_ShowAttentionChina(const char **isbn);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* NITRO_OS_ARM9_CHINA_H_ */

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
