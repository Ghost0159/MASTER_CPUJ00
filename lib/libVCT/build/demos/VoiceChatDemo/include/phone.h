#ifndef __PHONE_H__
#define __PHONE_H__

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

GameMode PhoneMain();

typedef struct _app_info
{
    VCTSession*     session;    // ���݂̃Z�b�V���� 
    u8              aid;        // ���ݑI�𒆂�aid  
} app_info;

#ifdef __cplusplus
}
#endif

#endif  // __PHONE_H__
