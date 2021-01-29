#ifndef __MP_H__
#define __MP_H__

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

void MP_SetData(u16* sendbuf, DWCFriendData* friendlist);
BOOL MP_Match();
void MP_Finish();
    
#ifdef __cplusplus
}
#endif

#endif  // __MP_H__
