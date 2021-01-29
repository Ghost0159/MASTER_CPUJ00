/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - FrameTexVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/
//
// �t���[��VRAM�}�l�[�W��(�e�N�X�`���C���[�W)���g�p����T���v���ł��B
// ���܂��܂ȃp�^�[���ŗ̈�m�ۂ̃e�X�g���s���܂��B
// 
#include <nitro.h>
#include <nnsys/gfd.h>

//   �v���O�������Ŏg���܂킷�ϐ�
static NNSGfdFrmTexVramState     state_;
static NNSGfdTexKey mem;

//  ���[�W�������Ƃ̋󂫗̈�
//  �u�q�`�l�̋󂫂����C�u�����̎擾�֐��œ�����l�Ɠ������ǂ������m���߂邽�߂Ɏg��
static int vramFreeSize[5];


//  ���̃��[�W�����̋󂫗̈�̑傫�����v�Z����
static int calcFreeSize(int regionNo) 
{
    //
    // �X�e�[�g�̎擾
    //
    NNS_GfdGetFrmTexVramState(&state_);
    
    {
        // �X�e�[�g����󂫗e�ʂ̎擾
        const int size 
            = (int)state_.address[regionNo * 2 + 1] - (int)state_.address[regionNo * 2 + 0];
            
        return size;
    }
}


//   �X���b�g�O�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
void testSlot0();

//   �X���b�g�O,�P�ɂ���,���낢��ȃp�^�[���Ńe�X�g������
//   �X���b�g�Q,�R������ꍇ�͂�����g����������ԂŃe�X�g����
void testSlot01(int numSlot);

//   �X���b�g�O,�P,�Q�ɂ���,���낢��ȃp�^�[���Ńe�X�g������
//   �X���b�g�R������ꍇ�͂�����g����������ԂŃe�X�g����
void testSlot012(int numSlot);

//   �X���b�g�O�C�P�C�Q�C�R�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
void testSlot0123();


//   �X���b�g�Q,�R������ꍇ�͂�����g���؂�
static void exhaustSlot23(int numSlot) {
    if(numSlot == 3) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
    if(numSlot == 4) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
}

//   �X���b�g�R������ꍇ�͂�����g���؂�
static void exhaustSlot3(int numSlot) {
    if(numSlot == 4) {
        mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    }
}


//------------------------------------------------------------------------------
//   �X���b�g�O�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
//------------------------------------------------------------------------------
static void testSlot0() {

    //   �̈���������
    NNS_GfdResetFrmTexVramState();

    vramFreeSize[0] = 0x20000;
    
    //  �W���e�N�X�`���̈�̊m��
    
    //  �܂��͍ł�������8x8�T�C�Y�̂S�F�p���b�g�e�N�X�`���p�̗̈���m�ۂ��Ă݂�
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,FALSE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    //   ��ʃA�h���X�������Ă���
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  �K���ɂ������̗̈�m��
    mem = NNS_GfdAllocTexVram(16 * 16 / 2,FALSE,0);
    vramFreeSize[0] -= 16 * 16 / 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 16 * 16 / 2);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    mem = NNS_GfdAllocTexVram(32 * 16,FALSE,0);
    vramFreeSize[0] -= 32 * 16;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 32 * 16);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    mem = NNS_GfdAllocTexVram(64 * 128,FALSE,0);
    vramFreeSize[0] -= 64 * 128;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //  �󂫗̈�ȏ�̗v�������Ă݂�
    //mem = NNS_GfdAllocTexVram(2048 * 2048,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //  ���̏�Ԃ�state_�ɕۑ�����
    NNS_GfdGetFrmTexVramState(&state_);
    //  �K���ɗ̈�m��
    mem = NNS_GfdAllocTexVram(64 * 128,FALSE,0);
    //   state_�̏�Ԃ��}�l�[�W���ɐݒ�B���ɖ߂��Ă��邩�ǂ������`�F�b�N
    NNS_GfdSetFrmTexVramState(&state_);
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    
    //   �̈���������
    NNS_GfdResetFrmTexVramState();
    vramFreeSize[0] = 0x20000;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    
    //   �܂��̈���m��
    mem = NNS_GfdAllocTexVram(64 * 256,FALSE,0);
    vramFreeSize[0] -= 64 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    {
        // �T�C�Y�[���̗̈���m�ۂ���
        // �e�N�X�`���L�[���\���ł���ŏ��T�C�Y�ɐ؂�グ���Ċm�ۂ����
        mem = NNS_GfdAllocTexVram( 0,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // �ŏ��T�C�Y��菬�����T�C�Y�̊m��
        mem = NNS_GfdAllocTexVram( 1,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // �ŏ��T�C�Y�҂�����̃T�C�Y�̊m��
        mem = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN );
        NNS_GfdResetFrmTexVramState();
        
        // �ŏ��T�C�Y��菭���傫���T�C�Y�̊m��
        mem = NNS_GfdAllocTexVram( NNS_GFD_TEXSIZE_MIN + 1,FALSE,0);
        NNS_GFD_ASSERT( NNS_GfdGetTexKeySize(mem) == NNS_GFD_TEXSIZE_MIN * 2 );
        NNS_GfdResetFrmTexVramState();
    }
}


//------------------------------------------------------------------------------
//   �X���b�g�O�C�P�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
//   �X���b�g�Q�C�R������ꍇ�͂�����g����������ԂŃe�X�g����
//------------------------------------------------------------------------------
static void testSlot01(int numSlot) 
{
    
    //
    // �}�l�[�W���̊Ǘ��X���b�g���ɂ���āA�󂫗̈�̌��������ύX�ɂȂ�̂ŁA
    // �ȉ��̃e�X�g�� �Ǘ��X���b�g�� > 2 �̏ꍇ�̂ݐ������邱�Ƃɒ��ӂ��Ă��������B
    //
    NNS_GFD_ASSERT( numSlot > 2 );

    //   �̈���������
    NNS_GfdResetFrmTexVramState();
    //  �X���b�g�Q,�R������ꍇ�͂�����g���؂�
    exhaustSlot23(numSlot);
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    
    //  �W���e�N�X�`���̈�̊m��
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[0] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  �X���b�g�O�̋󂫗̈悾���ł͑���Ȃ��悤�ȗ̈���m�ۂ��Ă݂�
    //  (�X���b�g�O�ɂ͗̈�͂Ƃ�ꂸ,�P�ɂ݂̂Ƃ���)
    mem = NNS_GfdAllocTexVram(256 * (256 + 32),FALSE,0);  // �X���b�g�O�𔼕���葽���g��
    mem = NNS_GfdAllocTexVram(256 * 256,FALSE,0);  // �X���b�g�P�̃��[�W�����Q�͑S�Ă���
    vramFreeSize[0] -= 256 * (256 + 32);
    vramFreeSize[2] -= 256 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 - 256 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   �X���b�g�P�̃��[�W�����P�̗̈���ʏ�e�N�X�`���p�Ɋm��
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[1] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 - 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   4x4���k�e�N�X�`���p�̗̈���m�ۂ���
    //   ���[�W�����P�ɒʏ�e�N�X�`���ƈ��k�e�N�X�`���̃p���b�g�C���f�b�N�X����������
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    vramFreeSize[1] -= 8 * 8 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //  �󂫗̈�ȏ�̗v�������Ă݂�
    //mem = NNS_GfdAllocTexVram(2048 * 2048,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   �̈���������
    NNS_GfdResetFrmTexVramState();
    //  �X���b�g�Q,�R������ꍇ�͂�����g���؂�
    exhaustSlot23(numSlot);
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //   �X���b�g�O�C�P���t���Ɏg���悤�ȗ̈�̊m��(����͊m�ۂł��Ȃ�)
    //mem = NNS_GfdAllocTexVram(512 * 512,FALSE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   �̈���������
    NNS_GfdResetFrmTexVramState();
    //  �X���b�g�Q,�R������ꍇ�͂�����g���؂�
    exhaustSlot23(numSlot);
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    
    //   4x4���k�e�N�X�`���p�ɗ̈���m��
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[0] -= 8 * 8 / 4;
    vramFreeSize[1] -= 8 * 8 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    //   �W���t�H�[�}�b�g�̃e�N�X�`���ƈႢ���ʃA�h���X����Ƃ��邱�Ƃɒ���
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //    �W���t�H�[�}�b�g�̃e�N�X�`���̈�m�ۂ������
    mem = NNS_GfdAllocTexVram(64 * 256,FALSE,0);
    vramFreeSize[0] -= 64 * 256;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x20000 - 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 64 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //    �܂�4x4���k�e�N�X�`���p�ɗ̈���m��
    mem = NNS_GfdAllocTexVram(16 * 128 / 4,TRUE,0);
    vramFreeSize[0] -= 16 * 128 / 4;
    vramFreeSize[1] -= 16 * 128 / (4 * 4) * 2;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 16 * 128 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //    �X���b�g�O��S�Ďg����
    mem = NNS_GfdAllocTexVram(0x20000 - 8 * 8 / 4 - 64 * 256 - 16 * 128 / 4,FALSE,0);
    vramFreeSize[0] = 0;
    
    //    �X���b�g�P�̃��[�W�����Q�͑S�Ďg��
    mem = NNS_GfdAllocTexVram(256 * 256,FALSE,0);
    vramFreeSize[2] -= 256 * 256;
    
    //   �X���b�g�P�̃��[�W�����P�ɗ̈���m��
    mem = NNS_GfdAllocTexVram(128 * 256,FALSE,0);
    vramFreeSize[1] -= 128 * 256;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 - 128 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 128 * 256);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //   ���ʃA�h���X����m�ۂ���鈳�k�e�N�X�`���̃C���f�b�N�X�e�[�u���ƂԂ����Ď��Ȃ��͂�
    //mem = NNS_GfdAllocTexVram(128 * 256,FALSE,0);
    //NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 0);
    
    //  ���̏�Ԃ�state_�ɕۑ�����
    NNS_GfdGetFrmTexVramState(&state_);
    //  �K���ɗ̈�m��
    mem = NNS_GfdAllocTexVram(8 * 8,FALSE,0);
    //   state_�̏�Ԃ��}�l�[�W���ɐݒ�B���ɖ߂��Ă��邩�ǂ������`�F�b�N
    NNS_GfdSetFrmTexVramState(&state_);
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    
}


//------------------------------------------------------------------------------
//   �X���b�g�O�C�P�C�Q�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
//   �X���b�g�R������ꍇ�͂�����g����������ԂŃe�X�g����
//------------------------------------------------------------------------------
static void testSlot012(int numSlot) {

    //   �̈���������
    NNS_GfdResetFrmTexVramState();
    //  �X���b�g�R������ꍇ�͂�����g���؂�
    exhaustSlot3(numSlot);
    
    //   �X���b�g�Q����݂̃e�X�g��������,�X���b�g�Q���g���؂�����ԂŃX���b�g�O,�P�̃e�X�g������
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    vramFreeSize[3] = 0x20000;
    
    //  �X���b�g�Q�ɕW���e�N�X�`���̈�̊m��
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[3] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    mem = NNS_GfdAllocTexVram(256 * (256 + 64),FALSE,0);
    vramFreeSize[3] -= 256 * (256 + 64);
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * (256 + 64));
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   �X���b�g�O��S�Ă���
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[0] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   �X���b�g�Q��256 * (128 + 256 + 64)�g���Ă���̂�,
    //   256*248��W���e�N�X�`���`���Ŋm�ۂ��悤�Ƃ���ƃX���b�g�P�̃��[�W�����Q����Ƃ���
    mem = NNS_GfdAllocTexVram(256 * 248,FALSE,0);
    vramFreeSize[2] -= 256 * 248;
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x30000 + vramFreeSize[2]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 248);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  �X���b�g�Q��256*64�󂢂Ă��āC�X���b�g�O�͑S�Ă����Ă���
    //  �X���b�g�P�̃��[�W�����Q��256*8�󂢂Ă���
    
    //  ����͊m�ۂł���
    mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    vramFreeSize[2] -= 8 * 8 / (4 * 4) * 2;
    vramFreeSize[3] -= 8 * 8 / 4;
    NNS_GFD_ASSERT(calcFreeSize(2) == vramFreeSize[2]);
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 8 * 8 / 4);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == TRUE);
    
    //   ����̓X���b�g�P�̃��[�W�����Q������Ȃ��Ċm�ۂł��Ȃ�
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //mem = NNS_GfdAllocTexVram(256 * 64 / 4,TRUE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //   ��256*128�̗̈��W���e�N�X�`���`���Ŏ�낤�Ƃ���ƃX���b�g�P�̃��[�W�����P����Ƃ���
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[1] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(1) == vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x20000 + vramFreeSize[1]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //  �X���b�g�Q���g���؂�����ԂŃX���b�g�O�C�P�̃e�X�g
    testSlot01(numSlot);
}


//------------------------------------------------------------------------------
//   �X���b�g�O�C�P�C�Q�C�R�ɂ��Ă��낢��ȃp�^�[���Ńe�X�g������
//------------------------------------------------------------------------------
static void testSlot0123() {
  
    //   �X���b�g�R����݂̃e�X�g��������,�X���b�g�R���g���؂�����ԂŃX���b�g�O�C�P�C�Q�̃e�X�g������
    
    vramFreeSize[0] = 0x20000;
    vramFreeSize[1] = 0x10000;
    vramFreeSize[2] = 0x10000;
    vramFreeSize[3] = 0x20000;
    vramFreeSize[4] = 0x20000;
    
    //  �X���b�g�R�ɕW���e�N�X�`���̈�̊m��
    mem = NNS_GfdAllocTexVram(256 * 128,FALSE,0);
    vramFreeSize[4] -= 256 * 128;
    NNS_GFD_ASSERT(calcFreeSize(4) == vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x60000 + vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 128);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    mem = NNS_GfdAllocTexVram(256 * (256 + 64),FALSE,0);
    vramFreeSize[4] -= 256 * (256 + 64);
    NNS_GFD_ASSERT(calcFreeSize(4) == vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x60000 + vramFreeSize[4]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * (256 + 64));
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //   �X���b�g�Q�C�O��S�Ă���
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[3] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(3) == vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == 0x40000 + vramFreeSize[3]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    // �}�l�[�W���̏�Ԃ��f�o�b�N�o�͂��܂�
    NNS_GfdDumpFrmTexVramManager();
    
    mem = NNS_GfdAllocTexVram(256 * 512,FALSE,0);
    vramFreeSize[0] -= 256 * 512;
    NNS_GFD_ASSERT(calcFreeSize(0) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeyAddr(mem) == vramFreeSize[0]);
    NNS_GFD_ASSERT(NNS_GfdGetTexKeySize(mem) == 256 * 512);
    NNS_GFD_ASSERT(NNS_GfdGetTexKey4x4Flag(mem) == FALSE);
    
    //
    // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
    //    
    //   �X���b�g�R�ɋ󂫗̈悪�����Ă��C4x4���k�e�N�X�`���p�̗̈�͊m�ۂł��Ȃ�
    //mem = NNS_GfdAllocTexVram(8 * 8 / 4,TRUE,0);
    //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_TEXKEY);
    
    //  �X���b�g�R���g���؂�����ԂŃX���b�g�O�C�P�C�Q�̃e�X�g
    testSlot012(4);
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �T���v���̃��C���֐�
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OS_Init();
        
    //  �u�q�`�l���S��(�X���b�g�͂O�A�P�A�Q�A�R)���蓖�Ă��ꍇ
    {
        const char* vramName[1] = {"VRAM-A,B,C,D"};
        const GXVRamTex vramEnum[1] = {GX_VRAM_TEX_0123_ABCD};
        int i;
        
        
        //  VRAM-A�`D�̓��̎l�Z�b�g�ɂ��ꂼ�ꊄ�蓖�ĂĒ��ׂ�
        OS_Printf("\nallocate slot 0,1,2,3\n");
        for(i = 0;i < 1;i ++) {
            OS_Printf("%s  checking\n",vramName[i]);
            GX_SetBankForTex(vramEnum[i]);
            
            //
            // �}�l�[�W���̏�����
            //
            NNS_GfdInitFrmTexVramManager(4,TRUE);
            
            //  �X���b�g�O�C�P�C�Q�C�R�̃e�X�g
            testSlot0123();
            
            NNS_GfdResetFrmTexVramState();
            (void)GX_DisableBankForTex();
            
            OS_Printf("%s  OK\n",vramName[i]);
        }
    }
    
    while(1) {}
}



