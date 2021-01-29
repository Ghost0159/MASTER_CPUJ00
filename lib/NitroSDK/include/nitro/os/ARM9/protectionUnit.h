/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - include
  File:     protectionUnit.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: protectionUnit.h,v $
  Revision 1.11  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.10  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.9  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.8  2004/05/28 11:03:37  yada
  only fix comment

  Revision 1.7  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.6  2004/02/05 07:27:19  yada
  ���𕶎����IRIS���������̂� NITRO ���� IRIS�ɖ߂����B

  Revision 1.5  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.4  2004/01/16 01:17:55  yasu
  Support SDK_BB

  Revision 1.3  2004/01/15 02:40:01  yada
  IRIS_BB �� SDK_BB �֕ύX

  Revision 1.2  2004/01/14 12:06:35  yada
  protectUnit��protectionUnit �ɕύX

  Revision 1.1  2003/12/08 12:22:34  yada
  ����


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_OS_PROTECTIONUNIT_H_
#define NITRO_OS_PROTECTIONUNIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>


//======================================================================
//          protection unit
//======================================================================
/*---------------------------------------------------------------------------*
  Name:         OS_EnableProtectionUnit

  Description:  enable protection unit

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void    OS_EnableProtectionUnit(void);

/*---------------------------------------------------------------------------*
  Name:         OS_DisableProtectionUnit

  Description:  disable protection unit

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void    OS_DisableProtectionUnit(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_OS_PROTECTIONUNIT_H_ */
#endif
