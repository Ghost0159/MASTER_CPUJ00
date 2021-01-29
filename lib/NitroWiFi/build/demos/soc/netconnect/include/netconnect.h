/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     netconnect.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: netconnect.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2005/12/22 04:32:58  adachi_hiroaki
  ä»à’é©ìÆê⁄ë±ÉâÉCÉuÉâÉäÇÉfÉÇÇ∆ÇµÇƒí«â¡

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_DEMOS_NETCONNECT_H_
#define NITROWIFI_DEMOS_NETCONNECT_H_

#ifdef __cplusplus

extern "C"
{
#endif

void    NcGlobalInit(void);
void    NcStart(const char* apClass);
void    NcFinish(void);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_DEMOS_NETCOONECT_H_ */
