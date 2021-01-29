#ifndef __REGISTERFRIEND_H__
#define __REGISTERFRIEND_H__

// [nakata] GameSpy SDK�Ƀv���b�g�t�H�[���������邽�߂̒萔
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

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
typedef struct tagRegFriendParam
{
    int step;    // 0:�R�}���h�I�𒆁A1:�o�^�E�폜�F�B�f�[�^���͒� 
    u32 figure;  // ���͒��̌� 
    u32 value;   // ���͒��̒l 
} RegFriendParam;

//�v���g�^�C�v
void RegFriendModeDispCallback(int curIdx, void* param);
GameMode GameRegisterFriendMain(void);

    
#ifdef __cplusplus
}
#endif

#endif  // __REGISTERFRIEND_H__
