#ifndef __LOGON_H__
#define __LOGON_H__

// [nakata] GameSpy SDKにプラットフォームを教えるための定数
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif

#ifdef __cplusplus
extern "C" {
#endif

void LogonModeDispCallback(int curIdx, void* param);
GameMode GameLogonMain(void);
    
#ifdef __cplusplus
}
#endif

#endif  // __LOGON_H__
