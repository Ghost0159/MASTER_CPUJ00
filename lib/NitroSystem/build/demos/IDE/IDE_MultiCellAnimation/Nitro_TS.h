/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_MultiCellAnimation
  File:     Nitro_TS.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
/*
	header file to include NITRO precompile header file
*/

#if !__option (precompile)

	#ifdef	__cplusplus
		/* include precompiled header for C++ source */
		#ifdef	SDK_DEBUG
			#include "Nitro_TS_Debug.mch++"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch++" */
		#elif		SDK_RELEASE
			#include "Nitro_TS_Release.mch++"	/* If a compile error occurs in this line, please precompile "Nitro_TS.pch++" */
		#elif		SDK_FINALROM
			#include "Nitro_TS_ROM.mch++"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch++" */
		#else
			#error("no SDK_ define")
		#endif
	#else
		/* include precompiled header for C source */
		#ifdef	SDK_DEBUG
			#include "Nitro_TS_Debug.mch"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch" */
		#elif		SDK_RELEASE
			#include "Nitro_TS_Release.mch"		/* If a compile error occurs in this line, please precompile "Nitro_TS.pch" */
		#elif		SDK_FINALROM
			#include "Nitro_TS_ROM.mch"			/* If a compile error occurs in this line, please precompile "Nitro_TS.pch" */
		#else
			#error("no SDK_ define")
		#endif
	#endif

#endif
