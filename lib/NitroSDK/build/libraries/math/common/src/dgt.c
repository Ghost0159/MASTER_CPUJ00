/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - 
  File:     dgt.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dgt.c,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/04/13 11:57:54  seiki_masashi
  GetDigest -> GetHash

  Revision 1.1  2005/04/11 10:35:18  seiki_masashi
  MATH_CalcMD5, MATH_CalcSHA1 �֐��̒ǉ�


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/math/dgt.h>

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcMD5

  Description:  MD5 ���v�Z����B
  
  Arguments:    digest  MD5 �l���i�[����ꏊ�ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MATH_CalcMD5(void *digest, const void *data, u32 dataLength)
{
    MATHMD5Context context;
    MATH_MD5Init(&context);
    MATH_MD5Update(&context, data, dataLength);
    MATH_MD5GetHash(&context, digest);
}

/*---------------------------------------------------------------------------*
  Name:         MATH_CalcSHA1

  Description:  SHA-1 ���v�Z����B
  
  Arguments:    digest  SHA-1 �l���i�[����ꏊ�ւ̃|�C���^
                data    ���̓f�[�^�̃|�C���^
                dataLength  ���̓f�[�^��
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
void MATH_CalcSHA1(void *digest, const void *data, u32 dataLength)
{
    MATHSHA1Context context;
    MATH_SHA1Init(&context);
    MATH_SHA1Update(&context, data, dataLength);
    MATH_SHA1GetHash(&context, digest);
}
