//============================================================================================
/**
 * @file	scr_n_park.h
 * @bfief	�X�N���v�g�R�}���h�F���R�����֘A
 * @author	Tomomichi Ohta
 * @date	06.06.26
 *
 * 07.12.05 Satoshi Nohara
 */
//============================================================================================
#ifndef SCR_N_PARK_H
#define SCR_N_PARK_H

//============================================================================================
//
//	�v���O�����ł̂ݎQ�Ƃ����`
//
//============================================================================================
//__ASM_NO_DEF_����`����Ă���ꍇ�A�A�Z���u���\�[�X�Ȃ̂ňȉ��͖����ɂ���
#ifndef	__ASM_NO_DEF_
extern BOOL EvCmdNaturalParkWalkCountClear(VM_MACHINE * core);
extern BOOL EvCmdNaturalParkWalkCountGet(VM_MACHINE * core);
extern BOOL EvCmdNaturalParkAccessoryNoGet(VM_MACHINE * core);
extern BOOL EvCmdParkManItemIndexGet(VM_MACHINE * core);
extern BOOL EvCmdParkManItemKindGet(VM_MACHINE * core);
extern BOOL EvCmdParkManItemNoGet(VM_MACHINE * core);
#endif	/* __ASM_NO_DEF_ */


#endif	/* SCR_N_PARK_H */

