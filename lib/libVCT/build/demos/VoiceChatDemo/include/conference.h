#ifndef __CONFERENCE_H__
#define __CONFERENCE_H__

// [nakata] GameSpy SDK�Ƀv���b�g�t�H�[���������邽�߂̒萔
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

GameMode ConferenceMain();
void ConferenceModeDispCallback(int curIdx, void *param);

#ifdef __cplusplus
}
#endif

#endif // __CONFERENCE_H__
