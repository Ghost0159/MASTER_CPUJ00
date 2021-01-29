//============================================================================================
/**
 * @file	scr_wazaoshie.h
 * @bfief	�X�N���v�g�R�}���h�F�Z�����֘A
 * @author	Satoshi Nohara
 * @date	07.12.17
 */
//============================================================================================
#ifndef SCR_WAZAOSHIE_H
#define SCR_WAZAOSHIE_H


//==============================================================================================
//
//	�C�x���g�E�B���h�E���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _WAZA_OSHIE_WORK WAZA_OSHIE_WORK;


//============================================================================================
//
//	�v���O�����ł̂ݎQ�Ƃ����`
//
//============================================================================================
//__ASM_NO_DEF_����`����Ă���ꍇ�A�A�Z���u���\�[�X�Ȃ̂ňȉ��͖����ɂ���
#ifndef	__ASM_NO_DEF_
extern BOOL EvCmdWazaOshieDataCount(VM_MACHINE * core );
extern BOOL EvCmdWazaOshieBmpListStart( VM_MACHINE * core );
extern BOOL EvCmdWazaOshieChgPokeWaza( VM_MACHINE * core );
//extern BOOL EvCmdWazaOshiePokeStatusSetProc(VM_MACHINE * core);
//extern BOOL EvCmdWazaOshiePokeStatusGetResult(VM_MACHINE * core);
extern BOOL EvCmdWazaOshieItemCheck( VM_MACHINE * core );
extern BOOL EvCmdWazaOshieItemSub( VM_MACHINE * core );
extern BOOL EvCmdWazaOshieBoardWrite( VM_MACHINE * core );
extern BOOL EvCmdWazaOshieBoardDel( VM_MACHINE * core );

#endif	/* __ASM_NO_DEF_ */


#endif	/* SCR_WAZAOSHIE_H */


