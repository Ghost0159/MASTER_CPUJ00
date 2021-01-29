//============================================================================================
/**
 * @file	scr_stage.h
 * @bfief	�X�N���v�g�R�}���h�F�o�g���X�e�[�W
 * @author	Satoshi Nohara
 * @date	06.06.13
 */
//============================================================================================
#ifndef SCR_STAGE_H
#define SCR_STAGE_H


//============================================================================================
//
//	�v���O�����ł̂ݎQ�Ƃ����`
//
//============================================================================================
//__ASM_NO_DEF_����`����Ă���ꍇ�A�A�Z���u���\�[�X�Ȃ̂ňȉ��͖����ɂ���
#ifndef	__ASM_NO_DEF_
extern BOOL EvCmdBattleStageTools(VM_MACHINE* core);
extern BOOL EvCmdBattleStageSetContinueNG( VM_MACHINE * core );
extern BOOL EvCmdBattleStageCommMonsNo(VM_MACHINE* core);
extern BOOL EvCmdBattleStageNumberName( VM_MACHINE * core );
extern BOOL EvCmdBattleStageRecordPokeGet( VM_MACHINE * core );
extern BOOL EvCmdBattleStageTotalRecordGetEx( VM_MACHINE * core );
extern BOOL EvCmdTVStageRensyouCheck( VM_MACHINE * core );
#endif	/* __ASM_NO_DEF_ */


#endif	/* SCR_STAGE_H */


