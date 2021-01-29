#ifndef __TRANSCEIVER_H__
#define __TRANSCEIVER_H__

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

GameMode TransceiverMain();
void TransceiverModeDispCallback(int curIdx, void *param);

#ifdef __cplusplus
}
#endif

#endif // __TRANSCEIVER_H__
