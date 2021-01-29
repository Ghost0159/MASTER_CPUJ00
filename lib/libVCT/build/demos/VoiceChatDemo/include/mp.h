#ifndef __MP_H__
#define __MP_H__

//  [nakata] GameSpy SDKにプラットフォームを教えるための定数 
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void mp_match(u16* gSendBuf, DWCFriendData* friendlist);
    
#ifdef __cplusplus
}
#endif

#endif  // __MP_H__
