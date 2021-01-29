/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wep-1
  File:     key.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: key.h,v $
  Revision 1.2  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model �� WEP Key �̓��I�ݒ�������Awep-1 �f���Ƃ��ĐV�K�o�^

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef __KEY_H__
#define __KEY_H__

#define KEY_REPEAT_START 25            // �L�[���s�[�g�J�n�܂ł̃t���[����
#define KEY_REPEAT_SPAN  10            // �L�[���s�[�g�̊Ԋu�t���[����

typedef struct KeyInfo
{
    u16     cnt;                       // �����H���͒l
    u16     trg;                       // �����g���K����
    u16     up;                        // �����g���K����
    u16     rep;                       // �����ێ����s�[�g����
}
KeyInfo;

extern void updateKeys(void);
extern KeyInfo *getKeyInfo(void);

#endif
