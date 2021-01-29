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
