/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - cplusplus-1
  File:     new.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: new.h,v $
  Revision 1.6  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.5  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.3  2004/03/08 08:55:47  yada
  fix top comment

  Revision 1.2  2004/02/20 00:04:05  yasu
  add comments

  Revision 1.1  2004/02/18 04:19:30  yada
  C++ƒfƒ‚


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef MY_NEW_H_
#define MY_NEW_H_

void   *operator    new(std::size_t blocksize);
void   *operator    new[] (std::size_t blocksize);
void operator    delete(void *block) throw();
void operator    delete[] (void *block)throw();

/* MY_NEW_H_ */
#endif
