/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - STUBS - libraries
  File:     dummy_md5.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dummy_md5.c,v $
  Revision 1.2  2006/03/10 09:22:20  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/07/29 05:10:17  terui
  md5 �v�Z�p API ��MATH �֐��̃��b�p�[�Ƃ��ĐV�K�ǉ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
 * ���L�̂R�̊֐��ɂ��āANitroParty �����̂����߂�̂ŁA
 * �d���Ȃ� inline �ł͂Ȃ����֐����쐬
 */
#include <ninet/nwbase/md5.h>

void MD5Init(MD5_CTX* context)
{
    MATH_MD5Init((MATHMD5Context*)context);
}

void MD5Update(MD5_CTX* context, unsigned char* buf, unsigned int len)
{
    MATH_MD5Update((MATHMD5Context*)context, (const void*)buf, len);
}

void MD5Final(unsigned char digest[16], MD5_CTX* context)
{
    MATH_MD5GetHash((MATHMD5Context*)context, (void*)digest);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
