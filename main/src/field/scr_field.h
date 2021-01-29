//============================================================================================
/**
 * @file	scr_field.h
 * @bfief	�X�N���v�g�Ŏg�p����v���O����(��풓�t�B�[���h�v���O����)
 * @author	Satoshi Nohara
 * @date	07.12.04
 */
//============================================================================================
#ifndef SCR_FIELD_H
#define SCR_FIELD_H


#include "../../include/pl_bugfix.h"

//==============================================================================================
//
//	�C�x���g�E�B���h�E���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _SEISEKI_WORK SEISEKI_WORK;


//============================================================================================
//
//	�v���O�����ł̂ݎQ�Ƃ����`
//
//============================================================================================
//__ASM_NO_DEF_����`����Ă���ꍇ�A�A�Z���u���\�[�X�Ȃ̂ňȉ��͖����ɂ���
#ifndef	__ASM_NO_DEF_
extern BOOL EvCmdSeisekiBmpListStart( VM_MACHINE * core );
extern BOOL EvCmdTopBreederPowRndGet( VM_MACHINE * core );
extern BOOL EvCmdPokeFormChange( VM_MACHINE * core );
extern BOOL EvCmdMezameruPawaaTypeGet( VM_MACHINE * core );
extern BOOL EvCmdSetFavoritePoke( VM_MACHINE * core );
extern BOOL EvCmdGetFavoritePoke( VM_MACHINE * core );
//extern BOOL EvCmdRotomuFormCheck( VM_MACHINE * core );
extern BOOL EvCmdTemotiRotomuFormChangeGet( VM_MACHINE * core );
extern BOOL EvCmdRotomuFormWazaChange( VM_MACHINE * core );
extern BOOL EvCmdTemotiRotomuFormNoGet( VM_MACHINE * core );
extern BOOL EvCmdVillaListCheck( VM_MACHINE * core );
extern BOOL EvCmdImageClipTvSendFlagReset( VM_MACHINE * core );
extern BOOL EvCmdUnionPokeFormReturn( VM_MACHINE * core );
extern BOOL EvCmdSodateyaPokeFormReturn( VM_MACHINE * core );
extern BOOL EvCmdD35FloorSet( VM_MACHINE * core );
extern BOOL EvCmdFldOBJAllHeightVanishOFF( VM_MACHINE * core );
extern BOOL EvCmdFldOBJAllHeightVanishON( VM_MACHINE * core );
extern BOOL EvCmdWifiHistory( VM_MACHINE * core );
extern BOOL EvCmdFldOBJForceDraw_C04EventOnly( VM_MACHINE * core );
extern BOOL EvCmdFldOBJForceDraw_C04EventAfterOnly( VM_MACHINE * core );
extern BOOL EvCmdSndPlayerVolume( VM_MACHINE * core );
#if PL_S0802_080716_FIX
extern BOOL EvCmdGetPokeSeeFlag( VM_MACHINE * core );
#endif

#endif	/* __ASM_NO_DEF_ */


#endif	/* SCR_FIELD_H */


