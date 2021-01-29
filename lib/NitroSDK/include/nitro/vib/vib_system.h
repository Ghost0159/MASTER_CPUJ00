/*---------------------------------------------------------------------------*
  Project:  NitroSDK - include - nitro - vib
  File:     vib_system.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vib_system.h,v $
  Revision 1.3  2007/01/22 08:48:12  okubata_ryoma
  log�ǉ�

  Revision 1.2  2006/03/03 00:35:00  okubata_ryoma
  include��extern"C"�̊O��

  Revision 1.1  2006/03/02 10:22:14  okubata_ryoma
  vib_system.h�̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef PULSE_VIB_H
#define PULSE_VIB_H

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*
                    Type, Constant
 *-----------------------------------------------------------------------*/

/*!
    �p���X�Z�b�g���̃p���X�̍ő吔�B�ύX�����ꍇ�̓��C�u�������ăr���h���Ă��������B
*/
#define VIB_PULSE_NUM_MAX   6

/*!
    ����on_time�̍ő�l�i0.1ms�P�ʁj
*/
#define VIB_ON_TIME_MAX 15

/*!
    rest_time�̍ŏ��l�i0.1ms�P�ʁj
*/
#define VIB_REST_TIME_MIN   15

/*!
    �p���X�U���̃X�e�[�^�X�������܂��B
    
    1.5ms��ON�A1.5ms��OFF�A1.5ms��ON�Ƃ����p���X���W���I�ȐU���ŁA����ɂ����
    �ł������U���𔭐������邱�Ƃ��ł��܂��B
    
    VIBPulseState �̒l�́A�n�[�h�E�F�A�d�l��A�ȉ��̃��[�������K�v������܂��B
    
    @li ���� on_time ��1.5ms�܂łƂ���B
    @li off_time[n] �͒��O�� on_time[n] �ȏ�̒l��ݒ肷��B
    @li rest_time ��1.5ms�ȏ�Ƃ���B
    
    �Ȃ��A VIB_StartPulse �֐����Ă΂ꂽ�ۂɂ������`�F�b�N���܂��B
    
    @image html pulse_vib.jpg "�p���X�U���̗�(�p���X����3�̏ꍇ)"
*/
typedef struct
{
    u32     pulse_num;                  /*! ���̃p���X�Z�b�g�ŉ���p���X�𔭐������邩�B1�ȏ� VIB_PULSE_NUM_MAX �ȉ��ł���K�v������܂��B */
    u32     rest_time;                  /*! �p���X�Z�b�g�Ԃ̋x�~���Ԃ̒����B1=0.1�~���b�ƂȂ�܂��B */
    u32     on_time[VIB_PULSE_NUM_MAX]; /*! �N�����Ԃ̒����B0���傫���l�Ƃ��Ă��������B1=0.1�~���b�ƂȂ�܂��B */
    u32     off_time[VIB_PULSE_NUM_MAX];/*! ��~���Ԃ̒����B0���傫���l�Ƃ��Ă��������B1=0.1�~���b�ƂȂ�܂��B */
    u32     repeat_num;                 /*! �p���X�Z�b�g���J��Ԃ����B0�̎��́A�I���Ȃ��J��Ԃ��܂��B */
}
VIBPulseState;

/*! �J�[�g���b�W�����R�[���o�b�N�̌^�ł��B */
typedef void (*VIBCartridgePulloutCallback) (void);

/*-----------------------------------------------------------------------*
                    Function external declaration
 *-----------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         VIB_Init

  Description:  �U���J�[�g���b�W���C�u���������������܂��B
                ��x���̊֐����Ă񂾏�Ԃōēx���̊֐����Ă񂾏ꍇ�́A
                VIB_IsCartridgeEnabled �֐��Ɠ����ɂȂ�܂��B
                
                ���̊֐����ŁAPM_AppendPreSleepCallback �֐����Ă�ŁA�X���[�v�ɓ���O��
                �U�����~�߂�R�[���o�b�N��o�^���Ă��܂��B
                
                �܂��A�J�[�g���b�W�̔��������o������U�����~�߂�R�[���o�b�N�����̊֐����œo�^���Ă��܂��̂ŁA
                ���̊֐����Ă񂾌�ŁACTRDG_SetPulledOutCallback �֐��ŃJ�[�g���b�W�����R�[���o�b�N���Z�b�g����ƁA
                VIB_Init �֐��ŃZ�b�g���ꂽ�J�[�g���b�W�������o�R�[���o�b�N���㏑������Ă��܂��܂��B
                ���̏ꍇ�̓Z�b�g�����J�[�g���b�W�����R�[���o�b�N���ŐU�����~�߂�K�v������܂��B
                �J�[�g���b�W�̔����R�[���o�b�N���ŐU�����~�߂�ȊO�ɂ��������s�������ꍇ�� 
                VIB_SetCartridgePulloutCallback �֐��ŃR�[���o�b�N��o�^����
                ���̃R�[���o�b�N���ŏ������s���悤�ɂ��Ă��������B 

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern BOOL VIB_Init(void);

/*---------------------------------------------------------------------------*
  Name:         VIB_End

  Description:  �U���J�[�g���b�W���C�u�����̎g�p���I�����܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void VIB_End(void);

/*---------------------------------------------------------------------------*
  Name:         VIB_StartPulse

  Description:  �p���X�U�����J�n���܂��B
                �����ȑO�̃p���X�U�����I�����Ă��Ȃ��ꍇ�́A��������I�����Ă���n�߂܂��B
                �X�e�[�^�X�̓��C�u�������ŃR�s�[���܂��̂ŁA���������m�ۂ��Ă����K�v�͂���܂���B

  Arguments:    state : �p���X�U���̃X�e�[�^�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void VIB_StartPulse(const VIBPulseState * state);

/*---------------------------------------------------------------------------*
  Name:         VIB_StopPulse

  Description:  �p���X�U�����~���܂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void VIB_StopPulse(void);

/*---------------------------------------------------------------------------*
  Name:         VIB_StopPulse

  Description:  �p���X�U�������s�����ۂ��𔻒肵�܂��B
                VIB_StartPulse �֐��ŐU��ON�ɂ��Ă��� VIB_StopPulse �֐��ŐU��OFF�ɂ���܂ł̊� TRUE ��Ԃ��܂��B

  Arguments:    None.

  Returns:      TRUE  : �p���X�U�����s���Ă�����
                FALSE : �p���X�U�����s���Ă��Ȃ����
 *---------------------------------------------------------------------------*/
extern BOOL VIB_IsExecuting(void);

/*---------------------------------------------------------------------------*
  Name:         VIB_SetCartridgePulloutCallback

  Description:  �J�[�g���b�W�����R�[���o�b�N��o�^���܂��B
                �J�[�g���b�W�������N�������ꍇ�A���C�u�����͒����Ƀp���X�U�����~���܂��B
                ���̊֐���p���ăR�[���o�b�N���o�^����Ă����ꍇ�́A���̌�ɃR�[���o�b�N���Ă΂�܂��B 

  Arguments:    func : �J�[�g���b�W�����R�[���o�b�N�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
extern void VIB_SetCartridgePulloutCallback(VIBCartridgePulloutCallback func);

/*---------------------------------------------------------------------------*
  Name:         VIB_IsCartridgeEnabled

  Description:  �U���J�[�g���b�W���������Ă��邩�ǂ����𔻒肵�܂��B

  Arguments:    None.

  Returns:      TRUE  : �N�����ɐU���J�[�g���b�W���������Ă����� 
                FALSE : �N�����ɐU���J�[�g���b�W���������Ă��Ȃ����
 *---------------------------------------------------------------------------*/
extern BOOL VIB_IsCartridgeEnabled(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PULSE_VIB_H */
