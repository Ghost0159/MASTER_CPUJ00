//==============================================================================
/**
 * @file	west_sp.h
 * @brief	�T�|�[�g�֐��p�w�b�_
 * @author	goto
 * @date	2005.07.14(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef	_WEST_SP_H_
#define _WEST_SP_H_

#include "wazaeffect/we_mana.h"
#include "we_sys.h"

#if 0

	�� ���� ��
	
	*.s �ň������߂�define�o�^�́A[ west_sp_def.h ] ���ɋL�q���Ă��������B
	���ۂɌĂяo���֐��́A[ west_sp.c ] �̃e�[�u�����ɋL�q���Ă��������B
	
	�T�|�[�g�p�̊֐��́A�����̐l�����삷��\��������̂�
	west_sp.c ���ɃR�[�f�B���O�����A���[�U�[�l�ŗp�ӂ����t�@�C����
	include����悤�ɂ��Ă�������	

#endif


// ----------------------------------------
//
//	�T�|�[�g�֐��Ăяo���֐�
//
// -----------------------------------------
extern pWeFunc WazaEffectSupportFuncGet(u32 id);


// -----------------------------------------
//
//	�Z���A�N�^�[�p �T�|�[�g�֐��Ăяo��
//
// -----------------------------------------
extern pWeClactFunc WazaEffectSupportFuncGet_CATS(u32 id);


// -----------------------------------------
//
//	OLDACT�p �T�|�[�g�֐��Ăяo��
//
// -----------------------------------------
extern pWeOldActFunc WazaEffectSupportFuncGet_OLDACT(u32 id);

#endif	