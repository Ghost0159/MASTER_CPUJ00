#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <nitro/types.h>

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
// �R���g���[�����͍\����
// �����̃L�[�Ƀ��s�[�g�ݒ���s�Ȃ��ꍇ�́A�S�ē���DelayTime��PulseTime����
// �ݒ�ł��Ȃ�
//  
typedef struct {
    u8  repeatSeq[10];    // �L�[���s�[�g�̃X�e�b�v�i0:Delay, 1:Pulse�j 
    u16 cont;             // �x�^���� 
    u16 trg;              // �g���K���� 
    u16 release;          // �����[�X����  
    u16 repeat;           // ���s�[�g�g���K���� 
    u16 checkPushButton;  // �L�[���s�[�g��Ԃ𒲂ׂ�{�^���̃t���O 
    u16 repeatDelayTime;  // �ŏ��̃g���K���͂���L�[���s�[�g�J�n�܂ł̊Ԋu 
    u16 repeatPulseTime;  // �L�[���s�[�g�Ԋu 
    u16 pad;              // �p�f�B���O 
    u16 pushCount[10];    // 1�̃{�^�������������Ă��鎞�Ԃ��v�� 
} KeyControl;


//----------------------------------------------------------------------------
// function
//----------------------------------------------------------------------------
extern void InitKeyData(KeyControl *pKeyCntSrc);
extern void ReadKeyData(void);
extern void SetKeyRepeat(u16 keyFlg, u16 delay, u16 pulse);
extern void AddKeyRepeat(u16 keyFlg);

extern s32  UtilPow(s32 x, u32 n);


#ifdef __cplusplus
}
#endif

#endif  // __UTILITY_H__
