//============================================================================================
/**
 * @file	scr_tool.c
 * @bfief	�X�N���v�g�Ŏg�p����v���O����(�풓)
 * @author	Satoshi Nohara
 * @date	05.11.16
 */
//============================================================================================
#include "common.h"
#include "gflib/msg_print.h"
#include "system/lib_pack.h"
#include "system/window.h"
#include "savedata/frontier_savedata.h"
#include "savedata/frontier_ex.h"
//#include "fieldmap.h"
#include "fieldsys.h"
#include "zonedata.h"
#include "mapdefine.h"
#include "itemtool/itemsym.h"							//ITEM_KIZUGUSURI
#include "poketool/monsno.h"
#include "field/eventflag.h"							//EventWork_CheckEventFlag
#include "field/evwkdef.h"								//FH_FLAG_START
#include "field/fieldobj.h"
#include "script.h"										//ID_HIDE_ITEM_SCR_OFFSET
#include "scr_tool.h"									//HIDEITEM_ID_FLAG

#include "msgdata/msg_gameover.h"						//msg_
#include "msgdata/msg_hyouka.h"

#include "system/wipe.h"
#include "../frontier/factory_def.h"					//FACTORY_TYPE_SINGLE
#include "../frontier/stage_def.h"						//STAGE_TYPE_SINGLE
#include "../frontier/castle_def.h"						//CASTLE_TYPE_SINGLE
#include "../frontier/roulette_def.h"					//ROULETTE_TYPE_SINGLE

#include "communication/comm_system.h"
#include "communication/comm_info.h"


//============================================================================================
//
//	�v���g�^�C�v�錾	
//
//============================================================================================
u16 GetNumKeta(u32 num);
u16 WazaMachineItemNoCheck( u16 itemno );

u16 GetShinouZukanHyoukaMsgID( u16 num ,u16 c04_arrive);
u16 GetZenkokuZukanHyoukaMsgID( u16 num ,u16 sex);

#ifdef PM_DEBUG
u8 debug_frontier_key_set = 0;
#endif


//============================================================================================
//
//	���̑�
//
//============================================================================================

//--------------------------------------------------------------
/**
 * �n���ꂽ�l�̌������擾
 *
 * @param   num			�l
 *
 * @retval  "����"
 */
//--------------------------------------------------------------
u16 GetNumKeta(u32 num)
{
	if( num / 10 == 0 )	return 1;
	else if( num / 100 == 0 ) return 2;
	else if( num / 1000 == 0 ) return 3;
	else if( num / 10000 == 0 ) return 4;
	else if( num / 100000 == 0 ) return 5;
	else if( num / 1000000 == 0 ) return 6;
	else if( num / 10000000 == 0 ) return 7;
	else if( num / 100000000 == 0 ) return 8;

	return 1;	//���Ă͂܂�Ȃ�������P����Ԃ�
}

//--------------------------------------------------------------
/**
 * �Z�}�V���̃A�C�e���i���o�[���`�F�b�N
 *
 * @param   itemno		�A�C�e���i���o�[
 *
 * @retval  TRUE		�Z�}�V��
 * @retval  FALSE		����ȊO
 */
//--------------------------------------------------------------
u16 WazaMachineItemNoCheck( u16 itemno )
{
	//�Z�}�V�����`�F�b�N
	if( (itemno >= ITEM_WAZAMASIN01) && (itemno <= ITEM_HIDENMASIN08) ){
		return TRUE;
	}

	return FALSE;
}


//============================================================================================
//
//	�}�ӕ]��
//
//============================================================================================

//--------------------------------------------------------------
/**
 * �V���I�E�}�ӕ]�����b�Z�[�WID�擾
 *
 * @param   num		�B��6�C��������������
 * @param	c04_arrive	c04�n�N�^�C�V�e�B�̓����t���O
 *
 * @retval  "���b�Z�[�WID"
 */
//--------------------------------------------------------------
u16 GetShinouZukanHyoukaMsgID( u16 num ,u16 c04_arrive)
{
	//�V���I�E�}�ӊ������Ă邩�`�F�b�N
	if( num <= 15 ){	return msg_hyouka_s00; }
	if( num <= 30 ){	return msg_hyouka_s01; }
	if( num <= 45 ){	return msg_hyouka_s02; }
	if( num <= 60 ){	return msg_hyouka_s03; }
	if( num <= 80 ){	return msg_hyouka_s04; }
	if( num <= 100 ){	return msg_hyouka_s05; }
	if( num <= 120 ){	return msg_hyouka_s06; }
	if( num <= 140 ){	return msg_hyouka_s07; }
	if( num <= 160 ){	return msg_hyouka_s08; }
	if( num <= 180 ){	return msg_hyouka_s09; }
	if( num <= 200 ){	return msg_hyouka_s10; }
	if( num <= 209 ){	return msg_hyouka_s11; }

	//�����Ŋ���
	if(c04_arrive){
		//�n�N�^�C�ɓ������Ă�����A�������ւ����Ń��b�Z�[�W
		return msg_hyouka_doctor_07;
	}else{
		//�������Ă��Ȃ���΁A�S�������˃��b�Z�[�W
		return msg_hyouka_doctor_08;
	}
}

//--------------------------------------------------------------
/**
 * �]�����b�Z�[�WID�擾
 *
 * @param   num		�B�����������߂܂�����
 * @param	sex		��l���̐���
 *
 * @retval  "���b�Z�[�WID"
 */
//--------------------------------------------------------------
u16 GetZenkokuZukanHyoukaMsgID( u16 num ,u16 sex)
{
	if( num <= 39 ){	return msg_hyouka_z00; }
	if( num <= 59 ){	return msg_hyouka_z01; }
	if( num <= 89 ){	return msg_hyouka_z02; }
	if( num <= 119 ){	return msg_hyouka_z03; }
	if( num <= 149 ){	return msg_hyouka_z04; }
	if( num <= 189 ){	return msg_hyouka_z05; }
	if( num <= 229 ){	return msg_hyouka_z06; }
	if( num <= 269 ){	return msg_hyouka_z07; }
	if( num <= 309 ){	return msg_hyouka_z08; }
	if( num <= 349 ){	return msg_hyouka_z09; }
	if( num <= 379 ){	return msg_hyouka_z10; }
	if( num <= 409 ){	return msg_hyouka_z11; }
	if( num <= 429 ){
		if(sex){	//���̎qver
			return msg_hyouka_z19;
		}else{
			return msg_hyouka_z12; 
		}
	}
	if( num <= 449 ){	return msg_hyouka_z13; }
	if( num <= 459 ){	return msg_hyouka_z14; }
	if( num <= 469 ){	return msg_hyouka_z15; }
	if( num <= 475 ){	return msg_hyouka_z16; }
	if( num <= 481 ){	return msg_hyouka_z17; }

	//����
	if(sex){
		return msg_hyouka_z20;
	}else{
		return msg_hyouka_z18;
	}
}

//--------------------------------------------------------------
/**
 * �擪�|�P�����Z�o
 *
 * @param   sv		�|�P�����Z�[�u���\����
 *
 * @retval  �ԍ�
 */
//--------------------------------------------------------------
u16 GetFrontPokemon(SAVEDATA * sv)
{
	POKEMON_PARAM * poke;
	u16 i,max;

	max = PokeParty_GetPokeCount(SaveData_GetTemotiPokemon(sv));

	for(i=0;i<max;i++){
		poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(sv), i);

		//�����X�^�[�ԍ��擾(0�ȊO�Ȃ�^�}�S����Ȃ��j
		if( PokeParaGet( poke, ID_PARA_tamago_flag, NULL ) == 0 ){
			return i;
		}
	}

	return 0;	//���s���ȃ|�P�������΍�i�ʏ�͂����͒ʂ�܂���j
}

//--------------------------------------------------------------
/**
 *	@brief	���W�L���O�C�x���g�@�莝���ɃA�C�X�E���b�N�E�X�`�������邩�ǂ����H
 */
//--------------------------------------------------------------
BOOL EventCheck_IsReziUnseal(SAVEDATA* sv)
{
	int i,j,num,ct = 0;
	POKEPARTY *party;
	static const u16 rezino[] = {MONSNO_REZIROKKU,MONSNO_REZIAISU,MONSNO_REZISUTIRU};
	u16	monsno[6];
	
	party = SaveData_GetTemotiPokemon(sv);
	num = PokeParty_GetPokeCount(party);
	for(i = 0;i < num;i++){
		monsno[i] = PokeParaGet(PokeParty_GetMemberPointer(party,i),
						ID_PARA_monsno,NULL);	
	}
					
	for(i = 0;i < 3;i++){
		for(j = 0;j < num;j++){
			if(monsno[j] == rezino[i]){
				++ct;
				break;
			}
		}
	}
	if(ct == 3){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
///�t�B�[���hOBJ��h�炸�A�j��
//--------------------------------------------------------------
typedef struct _OBJ_SHAKE_ANM{
	FIELD_OBJ_PTR	obj;
	fx32	ofsx,ofsz;
	u16	time;
	u16	r,spd;
}OBJ_SHAKE_ANM;

//--------------------------------------------------------------
/**
 * @brief	�t�B�[���h�I�u�W�F��h�炷�C�x���g 
 * @param	ev	GMEVENT_CONTROL *
 * @param	obj		FIELD_OBJ_PTR	�t�B�[���h�I�u�W�F�ւ̃|�C���^
 * @param	time	�h�炷��
 * @param	spd		�h�炷�X�s�[�h(360������؂�鐔�����悢)
 * @param	ofsx	�h�炷��X	
 * @param	ofsz	�h�炷��Z	
 * @retval	BOOL	TRUE=�C�x���g�I��
 */
//--------------------------------------------------------------
static BOOL FieldobjAnmEvent_ObjShake( GMEVENT_CONTROL *ev )
{
	VecFx32	vp;
	FIELDSYS_WORK *fsys = FieldEvent_GetFieldSysWork( ev );
	OBJ_SHAKE_ANM *wk = FieldEvent_GetSpecialWork( ev );


	vp.x = FX32_CONST(8);//FX_Mul(Sin360(wk->r),wk->ofsx);
	vp.z = FX32_CONST(8);//FX_Mul(Sin360(wk->r),wk->ofsz);
	vp.x = FX_Mul(Sin360(wk->r),wk->ofsx);
	vp.z = FX_Mul(Sin360(wk->r),wk->ofsz);
	vp.y = 0;
	FieldOBJ_VecDrawOffsOutSideSet(wk->obj,&vp);

	wk->r += wk->spd;
	if(wk->r >= 360){
		wk->r = 0;
		wk->time--;
	}
	if(wk->time == 0){
		vp.x = vp.y = vp.z = 0;//FX32_CONST(16);
		FieldOBJ_VecDrawOffsOutSideSet(wk->obj,&vp);
		sys_FreeMemoryEz(wk);
		return TRUE;
	}
	return FALSE;
}

void EventCmd_ObjShakeAnm( GMEVENT_CONTROL *event,FIELD_OBJ_PTR obj,
		u16 time,u16 spd,u16 ofsx,u16 ofsz)
{
	FIELDSYS_WORK *fsys = FieldEvent_GetFieldSysWork( event );
	
	OBJ_SHAKE_ANM *wk = sys_AllocMemoryLo( HEAPID_WORLD,sizeof(OBJ_SHAKE_ANM));
	MI_CpuClear8(wk,sizeof(OBJ_SHAKE_ANM));	
	
	wk->ofsx = FX32_CONST(ofsx);
	wk->ofsz = FX32_CONST(ofsz);
	wk->time = time;
	wk->spd = spd;
	wk->obj = obj;
	FieldEvent_Call( fsys->event, FieldobjAnmEvent_ObjShake, wk);
}

//--------------------------------------------------------------
///�t�B�[���hOBJ��Blink������A�j��
//--------------------------------------------------------------
typedef struct _OBJ_BLINK_ANM{
	FIELD_OBJ_PTR	obj;
	u16	count;
	u16	time;
	u8	ct;
	u8	flag;
}OBJ_BLINK_ANM;

//--------------------------------------------------------------
/**
 * @brief	�t�B�[���h�I�u�W�F��Blink������A�j���C�x���g 
 * @param	ev		GMEVENT_CONTROL *
 * @param	obj		FIELD_OBJ_PTR	�t�B�[���h�I�u�W�F�ւ̃|�C���^
 * @param	count	Blink�������
 * @param	time	onoff��؂�ւ���C���^�[�o��
 */
//--------------------------------------------------------------
static BOOL FieldobjAnmEvent_ObjBlink( GMEVENT_CONTROL *ev )
{
	FIELDSYS_WORK *fsys = FieldEvent_GetFieldSysWork( ev );
	OBJ_BLINK_ANM *wk = FieldEvent_GetSpecialWork( ev );

	FieldOBJ_StatusBitSet_Vanish(wk->obj, wk->flag);

	if(wk->ct++ >= wk->time){
		wk->flag ^= 1;
		wk->ct = 0;
		if(wk->count-- == 0){
			sys_FreeMemoryEz(wk);
			return TRUE;
		}
	}
	return FALSE;
}

void EventCmd_ObjBlinkAnm(GMEVENT_CONTROL *event,FIELD_OBJ_PTR obj,u16 count,u16 time)
{
	FIELDSYS_WORK *fsys = FieldEvent_GetFieldSysWork( event );
	
	OBJ_BLINK_ANM *wk = sys_AllocMemoryLo( HEAPID_WORLD,sizeof(OBJ_BLINK_ANM));
	MI_CpuClear8(wk,sizeof(OBJ_BLINK_ANM));	
	
	wk->count = count;
	wk->time = time;
	wk->obj = obj;
	wk->flag = 0;
	FieldEvent_Call( fsys->event, FieldobjAnmEvent_ObjBlink, wk);
}


//============================================================================================
//
//	�t�@�N�g���[�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
int FactoryScr_GetWinRecordID( u8 level, u8 type );
int FactoryScr_GetMaxWinRecordID( u8 level, u8 type );
int FactoryScr_GetTradeRecordID( u8 level, u8 type );
int FactoryScr_GetMaxTradeRecordID( u8 level, u8 type );

//--------------------------------------------------------------
/**
 * @brief	���݂̘A�����̃��R�[�h��`���擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdFactorySetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int FactoryScr_GetWinRecordID( u8 level, u8 type )
{
	int ret_id;

	switch( type ){

	case FACTORY_TYPE_SINGLE:
		ret_id = FRID_FACTORY_SINGLE_RENSHOU_CNT;
		break;

	case FACTORY_TYPE_DOUBLE:
		ret_id = FRID_FACTORY_DOUBLE_RENSHOU_CNT;
		break;

	case FACTORY_TYPE_MULTI:
		ret_id = FRID_FACTORY_MULTI_COMM_RENSHOU_CNT;
		break;

	case FACTORY_TYPE_WIFI_MULTI:
		ret_id = FRID_FACTORY_MULTI_WIFI_RENSHOU_CNT;
		break;
	};

	return ret_id + (level * FACTORY_RECID_SIZE);		//LV50��LV100���킹��
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̃��R�[�h��`���擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdFactorySetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int FactoryScr_GetMaxWinRecordID( u8 level, u8 type )
{
	int ret_id;

	switch( type ){

	case FACTORY_TYPE_SINGLE:
		ret_id = FRID_FACTORY_SINGLE_RENSHOU;
		break;

	case FACTORY_TYPE_DOUBLE:
		ret_id = FRID_FACTORY_DOUBLE_RENSHOU;
		break;

	case FACTORY_TYPE_MULTI:
		ret_id = FRID_FACTORY_MULTI_COMM_RENSHOU;
		break;

	case FACTORY_TYPE_WIFI_MULTI:
		ret_id = FRID_FACTORY_MULTI_WIFI_RENSHOU;
		break;
	};

	return ret_id + (level * FACTORY_RECID_SIZE);		//LV50��LV100���킹��
}

//--------------------------------------------------------------
/**
 * @brief	���݂̌������̃��R�[�h��`���擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdFactorySetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int FactoryScr_GetTradeRecordID( u8 level, u8 type )
{
	int ret_id;

	switch( type ){

	case FACTORY_TYPE_SINGLE:
		ret_id = FRID_FACTORY_SINGLE_TRADE_CNT;
		break;

	case FACTORY_TYPE_DOUBLE:
		ret_id = FRID_FACTORY_DOUBLE_TRADE_CNT;
		break;

	case FACTORY_TYPE_MULTI:
		ret_id = FRID_FACTORY_MULTI_COMM_TRADE_CNT;
		break;

	case FACTORY_TYPE_WIFI_MULTI:
		ret_id = FRID_FACTORY_MULTI_WIFI_TRADE_CNT;
		break;
	};

	return ret_id + (level * FACTORY_RECID_SIZE);		//LV50��LV100���킹��
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̌������̃��R�[�h��`���擾
 *
 * @param	wk		FACTORY_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdFactorySetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int FactoryScr_GetMaxTradeRecordID( u8 level, u8 type )
{
	int ret_id;

	switch( type ){

	case FACTORY_TYPE_SINGLE:
		ret_id = FRID_FACTORY_SINGLE_TRADE;
		break;

	case FACTORY_TYPE_DOUBLE:
		ret_id = FRID_FACTORY_DOUBLE_TRADE;
		break;

	case FACTORY_TYPE_MULTI:
		ret_id = FRID_FACTORY_MULTI_COMM_TRADE;
		break;

	case FACTORY_TYPE_WIFI_MULTI:
		ret_id = FRID_FACTORY_MULTI_WIFI_TRADE;
		break;
	};

	return ret_id + (level * FACTORY_RECID_SIZE);		//LV50��LV100���킹��
}


//============================================================================================
//
//	�o�g���X�e�[�W�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
int StageScr_GetTypeLevelRecordID( u8 type, u8 csr_pos );
int StageScr_GetWinRecordID( u8 type );
int StageScr_GetMaxWinRecordID( u8 type );
int StageScr_GetMonsNoRecordID( u8 type );
int StageScr_GetExMaxWinRecordID( u8 type );

//--------------------------------------------------------------
/**
 * @brief	�^�C�v���x���̃��R�[�h��`���擾
 *
 * @param	wk		STAGE_SCRWORK�̃|�C���^
 *
 * "scr_stage_sub.c EvCmdStageSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int StageScr_GetTypeLevelRecordID( u8 type, u8 csr_pos )
{
	int ret_id;

	switch( type ){

	case STAGE_TYPE_SINGLE:
		ret_id = FRID_STAGE_SINGLE_TYPE_LEVEL_0;
		break;

	case STAGE_TYPE_DOUBLE:
		ret_id = FRID_STAGE_DOUBLE_TYPE_LEVEL_0;
		break;

	case STAGE_TYPE_MULTI:
		ret_id = FRID_STAGE_MULTI_COMM_TYPE_LEVEL_0;
		break;

	case STAGE_TYPE_WIFI_MULTI:
		ret_id = FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_0;
		break;
	};

	//�I�t�Z�b�g�����킷(2���i�[���Ă���)
	ret_id += (csr_pos / 2);

	OS_Printf( "==============\nTypeLevel���擾%d\n" );
	OS_Printf( "type = %d\n", type );
	OS_Printf( "ret_id = %d\n", ret_id );
	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	���݂̘A�����̃��R�[�h��`���擾
 *
 * @param	wk		STAGE_SCRWORK�̃|�C���^
 *
 * "scr_stage_sub.c EvCmdStageSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int StageScr_GetWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case STAGE_TYPE_SINGLE:
		ret_id = FRID_STAGE_SINGLE_RENSHOU_CNT;
		break;

	case STAGE_TYPE_DOUBLE:
		ret_id = FRID_STAGE_DOUBLE_RENSHOU_CNT;
		break;

	case STAGE_TYPE_MULTI:
		ret_id = FRID_STAGE_MULTI_COMM_RENSHOU_CNT;
		break;

	case STAGE_TYPE_WIFI_MULTI:
		ret_id = FRID_STAGE_MULTI_WIFI_RENSHOU_CNT;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̃��R�[�h��`���擾
 *
 * @param	wk		STAGE_SCRWORK�̃|�C���^
 *
 * "scr_stage_sub.c EvCmdStageSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int StageScr_GetMaxWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case STAGE_TYPE_SINGLE:
		ret_id = FRID_STAGE_SINGLE_RENSHOU;
		break;

	case STAGE_TYPE_DOUBLE:
		ret_id = FRID_STAGE_DOUBLE_RENSHOU;
		break;

	case STAGE_TYPE_MULTI:
		ret_id = FRID_STAGE_MULTI_COMM_RENSHOU;
		break;

	case STAGE_TYPE_WIFI_MULTI:
		ret_id = FRID_STAGE_MULTI_WIFI_RENSHOU;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	���풆�̃|�P�����̎�ރ��R�[�h��`���擾
 *
 * @param	wk		STAGE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
int StageScr_GetMonsNoRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case STAGE_TYPE_SINGLE:
		ret_id = FRID_STAGE_SINGLE_MONSNO;
		break;

	case STAGE_TYPE_DOUBLE:
		ret_id = FRID_STAGE_DOUBLE_MONSNO;
		break;

	case STAGE_TYPE_MULTI:
		ret_id = FRID_STAGE_MULTI_COMM_MONSNO;
		break;

	case STAGE_TYPE_WIFI_MULTI:
		ret_id = FRID_STAGE_MULTI_WIFI_MONSNO;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�O���Z�[�u�̍ő�A�����̃��R�[�h��`���擾
 *
 * @param	wk		STAGE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
int StageScr_GetExMaxWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case STAGE_TYPE_SINGLE:
		ret_id = FREXID_STAGE_RENSHOU_SINGLE;			///<�X�e�[�W�F�V���O���ő�A����
		break;

	case STAGE_TYPE_DOUBLE:
		ret_id = FREXID_STAGE_RENSHOU_DOUBLE;			///<�X�e�[�W�F�_�u���ő�A����
		break;

	case STAGE_TYPE_MULTI:
		ret_id = FREXID_STAGE_RENSHOU_MULTI;			///<�X�e�[�W�F���C�����X�ő�A����
		break;

	case STAGE_TYPE_WIFI_MULTI:
		ret_id = FREXID_STAGE_RENSHOU_MULTI;			///<�X�e�[�W�F���C�����X�ő�A����
		//ret_id = FREXID_STAGE_RENSHOU_WIFI,			///<�X�e�[�W�FWIFI�ő�A����
		GF_ASSERT( 0 );
		break;
	};

	return ret_id;
}


//============================================================================================
//
//	�o�g���L���b�X���Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
int CastleScr_GetRankRecordID( u8 type, u8 id );
int CastleScr_GetWinRecordID( u8 type );
int CastleScr_GetMaxWinRecordID( u8 type );
int CastleScr_GetCPRecordID( u8 type );
int CastleScr_GetUsedCPRecordID( u8 type );
int CastleScr_GetRemainderCPRecordID( u8 type );

//--------------------------------------------------------------
/**
 * @brief	�����N(�񕜁A�����^���A���)�̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdCastleSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int CastleScr_GetRankRecordID( u8 type, u8 id )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_RANK_RECOVER;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_RANK_RECOVER;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_RANK_RECOVER;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_RANK_RECOVER;
		break;
	};

	//�񕜁A�����^���A���
	ret_id += id;
	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	���݂̘A�����̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdCastleSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int CastleScr_GetWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_RENSHOU_CNT;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_RENSHOU_CNT;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_RENSHOU_CNT;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_RENSHOU_CNT;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdCastleSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int CastleScr_GetMaxWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_RENSHOU;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_RENSHOU;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_RENSHOU;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_RENSHOU;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	���݂�CP�̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
int CastleScr_GetCPRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_CP;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_CP;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_CP;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_CP;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�g�p����CP�̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
int CastleScr_GetUsedCPRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_USED_CP;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_USED_CP;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_USED_CP;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_USED_CP;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�������L�^�������̎c��CP�̃��R�[�h��`���擾
 *
 * @param	wk		CASTLE_SCRWORK�̃|�C���^
 */
//--------------------------------------------------------------
int CastleScr_GetRemainderCPRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case CASTLE_TYPE_SINGLE:
		ret_id = FRID_CASTLE_SINGLE_REMAINDER_CP;
		break;

	case CASTLE_TYPE_DOUBLE:
		ret_id = FRID_CASTLE_DOUBLE_REMAINDER_CP;
		break;

	case CASTLE_TYPE_MULTI:
		ret_id = FRID_CASTLE_MULTI_COMM_REMAINDER_CP;
		break;

	case CASTLE_TYPE_WIFI_MULTI:
		ret_id = FRID_CASTLE_MULTI_WIFI_REMAINDER_CP;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 *	@brief	(�^���[)�Q���w�肵���莝���|�P�����̏����`�F�b�N
 *
 *	@retval	0	�Q��OK
 *	@retval	1	�����|�P����������
 *	@retval 2	�����A�C�e���������Ă���
 */
//--------------------------------------------------------------
#if 0
int TowerScr_CheckEntryPokeSub( u8* pos_tbl, u8 max, SAVEDATA* savedata );
int TowerScr_CheckEntryPokeSub( u8* pos_tbl, u8 max, SAVEDATA* savedata )
{
	u16	i = 0,j = 0;
	u16	monsno[BTOWER_STOCK_MEMBER_MAX],itemno[BTOWER_STOCK_MEMBER_MAX];
	POKEPARTY* party;
	POKEMON_PARAM* poke;

	party = SaveData_GetTemotiPokemon(savedata);

	for( i=0; i < max; i++ ){

		poke = PokeParty_GetMemberPointer(party,pos_tbl[i]);

		monsno[i] = PokeParaGet(poke,ID_PARA_monsno,NULL);
		itemno[i] = PokeParaGet(poke,ID_PARA_item,NULL);

		if(i == 0){
			continue;
		}
		for(j = 0;j < i;j++){
			if(monsno[i] == monsno[j]){
				//�����|�P����������
				return 1;
			}
			if((itemno[i] != 0) && (itemno[i] == itemno[j])){
				//�����A�C�e���������Ă���
				return 2;
			}
		}
	}
	return 0;
}
#endif


//--------------------------------------------------------------
/**
 * @brief	�F�B�ԍ��擾
 *
 * @param	wk			FACTORY_SCRWORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
#include "wifi/dwc_rap.h"
int Frontier_GetFriendIndex( u32 record_no );
int Frontier_GetFriendIndex( u32 record_no )
{
	//���R�[�h�i���o�[�ɂ���ĕ���
	//
	//�ʐM��Ԃ��`�F�b�N�Ȃǂ��K�v�����B
	
	if( record_no < FRID_SINGLE_END ){			//CLEAR�̃��R�[�h�̒ǉ��ꏊ�ɂ��킹��
	//if( record_no < FRID_MAX ){
		return FRONTIER_RECORD_NOT_FRIEND;
	}

	//if( CommIsInitialize() ){
	return mydwc_getFriendIndex();
}

//--------------------------------------------------------------
/**
 * @brief	�ڑ����̑����Version���擾
 *
 * @param	none	
 *
 * @retval	u8			rom_code
 *
 * �_�C���A�p�[����0(POKEMON_DP_ROM_CODE)
 * �v���`�i�Ȃǂ�VERSION_PLATINUM�̒l������
 */
//--------------------------------------------------------------
u8 Frontier_GetPairRomCode( void );
u8 Frontier_GetPairRomCode( void )
{
	MYSTATUS* status;
	
	status = CommInfoGetMyStatus( CommGetCurrentID() ^ 1 );
	GF_ASSERT( status != NULL );

	return MyStatus_GetRomCode( status );
}

//--------------------------------------------------------------
/**
 * @brief	�����A����̂ǂ��炩��DP���`�F�b�N
 *
 * @param	none	
 *
 * @retval	"1 = DP����"
 */
//--------------------------------------------------------------
u8 Frontier_CheckDPRomCode( SAVEDATA* sv );
u8 Frontier_CheckDPRomCode( SAVEDATA* sv )
{
	if( MyStatus_GetRomCode( SaveData_GetMyStatus(sv) ) == POKEMON_DP_ROM_CODE ){
		return 1;
	}

	if( Frontier_GetPairRomCode() == POKEMON_DP_ROM_CODE ){
		return 1;
	}

	return 0;
}


//============================================================================================
//
//	�o�g�����[���b�g�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
int RouletteScr_GetWinRecordID( u8 type );
int RouletteScr_GetMaxWinRecordID( u8 type );
int RouletteScr_GetCPRecordID( u8 type );
int RouletteScr_GetUsedCPRecordID( u8 type );
int RouletteScr_GetRemainderCPRecordID( u8 type );

//--------------------------------------------------------------
/**
 * @brief	���݂̘A�����̃��R�[�h��`���擾
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdRouletteSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int RouletteScr_GetWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case ROULETTE_TYPE_SINGLE:
		ret_id = FRID_ROULETTE_SINGLE_RENSHOU_CNT;
		break;

	case ROULETTE_TYPE_DOUBLE:
		ret_id = FRID_ROULETTE_DOUBLE_RENSHOU_CNT;
		break;

	case ROULETTE_TYPE_MULTI:
		ret_id = FRID_ROULETTE_MULTI_COMM_RENSHOU_CNT;
		break;

	case ROULETTE_TYPE_WIFI_MULTI:
		ret_id = FRID_ROULETTE_MULTI_WIFI_RENSHOU_CNT;
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̃��R�[�h��`���擾
 *
 * @param	wk		ROULETTE_SCRWORK�̃|�C���^
 *
 * "scrcmd.c EvCmdRouletteSetContinueNG�ł������悤�ȏ��������Ă���̂Œ��ӁI"
 */
//--------------------------------------------------------------
int RouletteScr_GetMaxWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case ROULETTE_TYPE_SINGLE:
		ret_id = FRID_ROULETTE_SINGLE_RENSHOU;
		break;

	case ROULETTE_TYPE_DOUBLE:
		ret_id = FRID_ROULETTE_DOUBLE_RENSHOU;
		break;

	case ROULETTE_TYPE_MULTI:
		ret_id = FRID_ROULETTE_MULTI_COMM_RENSHOU;
		break;

	case ROULETTE_TYPE_WIFI_MULTI:
		ret_id = FRID_ROULETTE_MULTI_WIFI_RENSHOU;
		break;
	};

	return ret_id;
}


//============================================================================================
//
//	�^���[�Ŏg�p(frontier,field�Ŏg�p�����)
//
//============================================================================================
int TowerScr_GetWinRecordID( u8 type );
int TowerScr_GetMaxWinRecordID( u8 type );

//--------------------------------------------------------------
/**
 * @brief	���݂̘A�����̃��R�[�h��`���擾
 *
 * @param	type		�V���O���Ȃ�
 */
//--------------------------------------------------------------
int TowerScr_GetWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case BTWR_MODE_SINGLE:
		ret_id = FRID_TOWER_SINGLE_RENSHOU_CNT;
		break;

	case BTWR_MODE_DOUBLE:
		ret_id = FRID_TOWER_DOUBLE_RENSHOU_CNT;
		break;

	case BTWR_MODE_MULTI:		//AI�}���`
		ret_id = FRID_TOWER_MULTI_NPC_RENSHOU_CNT;
		break;

	case BTWR_MODE_COMM_MULTI:	//�}���`
		ret_id = FRID_TOWER_MULTI_COMM_RENSHOU_CNT;
		break;

	case BTWR_MODE_WIFI:		//WIFI�_�E�����[�h
		ret_id = FRID_TOWER_WIFI_DL_RENSHOU_CNT;
		break;

	case BTWR_MODE_WIFI_MULTI:	//WIFI�}���`
		ret_id = FRID_TOWER_MULTI_WIFI_RENSHOU_CNT;
		break;

	default:
		GF_ASSERT_MSG( 0, "tower type���Ή����Ă��܂���\n" );
		break;
	};

	return ret_id;
}

//--------------------------------------------------------------
/**
 * @brief	�ő�A�����̃��R�[�h��`���擾
 *
 * @param	type		�V���O���Ȃ�
 */
//--------------------------------------------------------------
int TowerScr_GetMaxWinRecordID( u8 type )
{
	int ret_id;

	switch( type ){

	case BTWR_MODE_SINGLE:
		ret_id = FRID_TOWER_SINGLE_RENSHOU;
		break;

	case BTWR_MODE_DOUBLE:
		ret_id = FRID_TOWER_DOUBLE_RENSHOU;
		break;

	case BTWR_MODE_MULTI:		//AI�}���`
		ret_id = FRID_TOWER_MULTI_NPC_RENSHOU;
		break;

	case BTWR_MODE_COMM_MULTI:	//�}���`
		ret_id = FRID_TOWER_MULTI_COMM_RENSHOU;
		break;

	case BTWR_MODE_WIFI:		//WIFI�_�E�����[�h
		ret_id = FRID_TOWER_WIFI_DL_RENSHOU;
		break;

	case BTWR_MODE_WIFI_MULTI:	//WIFI�}���`
		ret_id = FRID_TOWER_MULTI_WIFI_RENSHOU;
		break;

	default:
		GF_ASSERT_MSG( 0, "tower type���Ή����Ă��܂���\n" );
		break;
	};

	return ret_id;
}


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
u16 FrontierScrTools_CheckEntryPokeNum( u16 num, SAVEDATA* savedata );

//--------------------------------------------------------------
/**
 *	@brief	�t�����e�B�A�@�Q���\�ȃ|�P�������̃`�F�b�N
 *
 *	@param	num			�Q���ɕK�v�ȃ|�P������
 *	@param	savedata	�Z�[�u�f�[�^�ւ̃|�C���^
 *
 *	@retval	"0=�Q���\�A����ȊO�͑���Ȃ���"
 *
 *	�L���b�X���ƃ��[���b�g�Ŏg�p���Ă���
 */
//--------------------------------------------------------------
u16 FrontierScrTools_CheckEntryPokeNum( u16 num, SAVEDATA* savedata )
{
	u8 i,ct,pokenum;
	u16	monsno;
	POKEPARTY* party;
	POKEMON_PARAM* pp;
	
	party	= SaveData_GetTemotiPokemon( savedata );
	pokenum = PokeParty_GetPokeCount( party );

	//�莝���̐����K�v�ȃ|�P��������������
	if( pokenum < num ){
		return (num - pokenum);
	}

	for( i=0, ct=0; i < pokenum ;i++ ){
		pp		= PokeParty_GetMemberPointer( party, i );
		monsno	= PokeParaGet( pp, ID_PARA_monsno, NULL );		//�|�P�����i���o�[
		
		//�^�}�S�łȂ����`�F�b�N
		if( PokeParaGet(pp,ID_PARA_tamago_flag,NULL) != 0 ){
			continue;
		}

		//�o�g���^���[�ɎQ���ł��Ȃ��|�P���������`�F�b�N
		if( BattleTowerExPokeCheck_MonsNo(monsno) == TRUE ){
			continue;
		}

		ct++;
	}

	//������ʉ߂����|�P���������`�F�b�N
	if( ct < num ){
		return (num - ct);
	}

	return 0;
}


