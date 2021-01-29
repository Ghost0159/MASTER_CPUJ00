#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

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

void SetRecvBuffer( void );
GameMode GameConnectedMain(void);

#ifdef __cplusplus
}
#endif

#endif  // __TRANSPORT_H__
