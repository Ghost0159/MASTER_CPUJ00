#ifndef __PHONE_H__
#define __PHONE_H__

// [nakata] GameSpy SDKにプラットフォームを教えるための定数
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif

#include "main.h"
#include "vct.h"

#ifdef __cplusplus
extern "C" {
#endif

GameMode PhoneMain();

typedef struct _app_info
{
    VCTSession*     session;    // 現在のセッション 
    u8              aid;        // 現在選択中のaid  
} app_info;

#ifdef __cplusplus
}
#endif

#endif  // __PHONE_H__
