#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

// [nakata] GameSpy SDKにプラットフォームを教えるための定数
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif

//#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

GameMode GameMain(void);

    
#ifdef __cplusplus
}
#endif

#endif  // __GAMEMAIN_H__
