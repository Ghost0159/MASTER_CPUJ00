//============================================================================================
/**
 * @file	scr_castle.h
 * @bfief	�X�N���v�g�R�}���h�F�o�g���L���b�X��
 * @author	Satoshi Nohara
 * @date	07.06.13
 */
//============================================================================================
#ifndef SCR_CASTLE_H
#define SCR_CASTLE_H


//============================================================================================
//
//	�v���O�����ł̂ݎQ�Ƃ����`
//
//============================================================================================
//__ASM_NO_DEF_����`����Ă���ꍇ�A�A�Z���u���\�[�X�Ȃ̂ňȉ��͖����ɂ���
#ifndef	__ASM_NO_DEF_
extern BOOL EvCmdBattleCastleTools(VM_MACHINE* core);
extern BOOL EvCmdBattleCastleSetContinueNG( VM_MACHINE * core );
extern BOOL EvCmdBattleCastleCommMonsNo(VM_MACHINE* core);
#endif	/* __ASM_NO_DEF_ */


#endif	/* SCR_CASTLE_H */


