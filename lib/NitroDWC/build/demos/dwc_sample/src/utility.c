#include <nitro.h>
#include "utility.h"


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static KeyControl *pKeyCnt;


/*---------------------------------------------------------------------------*
  �R���g���[���\���̏������֐�
 *---------------------------------------------------------------------------*/
void InitKeyData(KeyControl *pKeyCntSrc)
{
    int i;

    pKeyCnt = pKeyCntSrc;
    pKeyCnt->cont    = 0;
    pKeyCnt->trg     = 0;
    pKeyCnt->release = 0;
    pKeyCnt->repeat  = 0;
    pKeyCnt->checkPushButton = 0;
    pKeyCnt->repeatDelayTime = 0;
    pKeyCnt->repeatPulseTime = 0;
    for (i = 0; i < 10; i++){
        pKeyCnt->pushCount[i] = 0;
        pKeyCnt->repeatSeq[i] = 0;
    }
}


/*---------------------------------------------------------------------------*
  �R���g���[���ǂݍ��݊֐�
 *---------------------------------------------------------------------------*/
void ReadKeyData(void)
{
    u16 readData;
    int i;

    readData = PAD_Read();
    pKeyCnt->trg     = (u16)(readData & (readData ^ pKeyCnt->cont));  // �g���K����
    pKeyCnt->release = (u16)(pKeyCnt->cont & (readData ^ pKeyCnt->cont));  // �����[�X
    pKeyCnt->cont    = readData;  // �x�^����
    pKeyCnt->repeat  = 0;         // ���s�[�g����

    // �L�[���s�[�g��Ԃ̃Z�b�g
    for (i = 0; i < 10; i++){
        if (pKeyCnt->cont & (pKeyCnt->checkPushButton & (1 << i))){
            pKeyCnt->pushCount[i]++;
            if (pKeyCnt->repeatSeq[i] == 0){
                // delay���Ԓ�
                if (pKeyCnt->pushCount[i] == pKeyCnt->repeatDelayTime){
                    // ���s�[�g�g���K���͔����idelay�j
                    pKeyCnt->repeat |= pKeyCnt->checkPushButton & (1 << i);
                    pKeyCnt->pushCount[i] = 0;
                    pKeyCnt->repeatSeq[i]++;
                }
            }
            else if (pKeyCnt->pushCount[i] == pKeyCnt->repeatPulseTime){
                // pulse���Ԓ�
                // ���s�[�g�g���K���͔����ipulse�j
                pKeyCnt->repeat |= pKeyCnt->checkPushButton & (1 << i);
                pKeyCnt->pushCount[i] = 0;
            }
        }
        else {
            pKeyCnt->pushCount[i] = 0;
            pKeyCnt->repeatSeq[i] = 0;
        }
    }
}


/*---------------------------------------------------------------------------*
  �L�[���s�[�g�ݒ�֐�
 *---------------------------------------------------------------------------*/
// ���̊֐����Ăяo���Ƃ���܂ł̃L�[���s�[�g�ݒ�̓N���A�����
// �����keyFlg = 0 �ł��̊֐����Ăяo���΁A�L�[���s�[�g�ݒ���N���A�ł���
void SetKeyRepeat(u16 keyFlg, u16 delay, u16 pulse)
{
    int i;

    pKeyCnt->repeat = 0;
    pKeyCnt->checkPushButton = keyFlg;
    pKeyCnt->repeatDelayTime = delay;
    pKeyCnt->repeatPulseTime = pulse;
    
    for (i = 0; i < 10; i++){
        pKeyCnt->pushCount[i] = 0;
        pKeyCnt->repeatSeq[i] = 0;
    }
}


/*---------------------------------------------------------------------------*
  ���s�[�g�L�[�ǉ��֐�
 *---------------------------------------------------------------------------*/
void AddKeyRepeat(u16 keyFlg)
{
    int i;

    
    pKeyCnt->repeat &= ~keyFlg;
    pKeyCnt->checkPushButton |= keyFlg;

    for (i = 0; i < 10; i++){
        if (keyFlg & (1 << i)){
            pKeyCnt->pushCount[i] = 0;
            pKeyCnt->repeatSeq[i] = 0;
        }
    }
}


/*---------------------------------------------------------------------------*
  �ׂ���֐�
 *---------------------------------------------------------------------------*/
// �����̌v�Z�ł͋t����0�ɂ����Ȃ�Ȃ����߁A�w���͐��̐��������Ȃ�
s32 UtilPow(s32 x, u32 n)
{
    s32 r;
    
    r = 1;
    while (n){
        if (n & 1){
            r *= x;
        }
        x *= x;
        n >>= 1;
    }

    return r;
}
