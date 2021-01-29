/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - shared
  File:     output.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: output.h,v $
  Revision 1.2  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/06 00:23:35  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITROWIFI_SOC_SHARED_OUTPUT_H_)
#define NITROWIFI_SOC_SHARED_OUTPUT_H_

#include <nitro.h>

#ifdef __cplusplus

extern "C"
{
#endif

/*****************************************************************************/

/* function */

/*---------------------------------------------------------------------------*
  Name:         InitDebugOutputHook

  Description:  �f�o�b�O�o�͂��T�u��ʂɂ��o�͂���悤�ɏ�����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitDebugOutputHook(void);

/*---------------------------------------------------------------------------*
  Name:         DebugOutput

  Description:  �f�o�b�O�o�͂��t�b�N���ă��O�ɒǉ�.

  Arguments:    s                �o�͕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    DebugOutput(const char* s);

/*---------------------------------------------------------------------------*
  Name:         CurrentOutput

  Description:  �J�����g���e����������Z�b�g.

  Arguments:    s                �o�͕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CurrentOutput(const char* s);

/*---------------------------------------------------------------------------*
  Name:         FlushDebugOutput

  Description:  �t�b�N�����f�o�b�O�o�̓��O���T�u��ʂɔ��f.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    FlushDebugOutput(void);

#ifdef __cplusplus

} /* extern "C" */
#endif

#endif /* NITROWIFI_SOC_SHARED_OUTPUT_H_ */
