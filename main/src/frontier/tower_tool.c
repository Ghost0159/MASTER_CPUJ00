//==============================================================================
/**
 * @file	tower_tool.c
 * @brief	�o�g���^���[�֘A�c�[����
 * @author	nohara
 * @date	2007.05.28
 *
 * b_tower_fld.c�̏������ړ�
 *
 * �֘A�\�[�X	field/scr_btower.c		�풓
 *				field/b_tower_scr.c		�t�B�[���h�T�u
 *				field/b_tower_ev.c		�풓
 *				field/b_tower_wifi.c	�풓
 *				field/b_tower_fld.c		�t�B�[���h�T�u
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/pm_str.h"
#include "poketool/poke_tool.h"
#include "poketool/status_rcv.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/zukanwork.h"
#include "savedata/perapvoice.h"
#include "savedata/record.h"
#include "system/procsys.h"
#include "system/pm_str.h"
#include "gflib/strbuf_family.h"
#include "poketool/poke_tool.h"
#include "application/p_status.h"
#include "application/pokelist.h"
#include "field/fieldsys.h"
#include "field/field_event.h"
#include "field/encount_set.h"
#include "field/ev_time.h"
#include "field/fieldobj_code.h"
#include "msgdata/msg.naix"
//�ʐM
#include "communication/comm_tool.h"
#include "communication/comm_system.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"
//�t�����e�B�A�V�X�e��
#include "frontier_tool.h"
#include "frontier_def.h"
//�o�g���^���[
#include "../field/b_tower_scr_def.h"
#include "../field/b_tower_ev.h"
//#include "../field/b_tower_fld.h"
#include "savedata/b_tower_local.h"
#include "tower_tool.h"
//�����f�[�^���o����܂ŃC���N���[�h
#include "battle/attr_def.h"
#include "itemtool/itemsym.h"
#include "battle/wazano_def.h"
#include "battle/battle_common.h"
#include "battle/trtype_def.h"
#include "battle/b_tower_data.h"
#include "battle/trno_def.h"
#include "../field/b_tower_fld.dat"


//==============================================================================
//	extern�錾
//==============================================================================
extern void	BattleParam_TrainerDataMake(BATTLE_PARAM *bp);


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
BOOL FSRomBattleTowerTrainerDataMake(BTOWER_SCRWORK* wk,B_TOWER_PARTNER_DATA *tr_data,u16 tr_no,int cnt,u16 *set_poke_no,u16 *set_item_no,B_TOWER_PAREPOKE_PARAM* poke,int heapID);
void btltower_BtlPartnerSelectWifi(SAVEDATA* sv,B_TOWER_PARTNER_DATA* wk,const u8 round);
BATTLE_PARAM* BtlTower_CreateBattleParam( BTOWER_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param );
//void BTowerComm_SendPlayerData(BTOWER_SCRWORK* wk,SAVEDATA *sv);
//void BTowerComm_SendTrainerData(BTOWER_SCRWORK* wk);
void FSBTowerComm_SendRetireSelect(BTOWER_SCRWORK* wk,u16 retire);
//u16 BTowerComm_RecvPlayerData(BTOWER_SCRWORK* wk,const u16* recv_buf);
//u16 BTowerComm_RecvTrainerData(BTOWER_SCRWORK* wk,const u16* recv_buf);
u16	FSBTowerComm_RecvRetireSelect(BTOWER_SCRWORK* wk,const u16* recv_buf);
int BtlTower_GetPokeArc( u8 play_mode );
int BtlTower_GetTrArc( u8 play_mode );
int BtlTower_GetTrMsgArc( u8 play_mode );

static BOOL BattleTowerPokemonSetAct(BTOWER_SCRWORK* wk,B_TOWER_TRAINER_ROM_DATA *trd,
		u16 tr_no,B_TOWER_POKEMON *pwd,u8 cnt,
		u16 *set_poke_no,u16 *set_item_no,B_TOWER_PAREPOKE_PARAM* poke,int heapID);
static void btltower_BtlPartnerSelectSD(B_TOWER_PARTNER_DATA* wk,const u8 round);
static u32 BattleTowerPokemonParamMake(BTOWER_SCRWORK* wk,B_TOWER_POKEMON* pwd,
		u16 poke_no,u32 poke_id,u32 poke_rnd,u8 pow_rnd,u8 mem_idx,BOOL itemfix,int heapID);
static u32 btower_GetFightType(u8 play_mode);
static void	BattleTowerTrainerDataMake(BATTLE_PARAM *bp,B_TOWER_PARTNER_DATA *tr_data,int cnt,int client_no,int heapID);
static int BtlTower_GetArcVersion( u8 play_mode );


//==============================================================================
//
//	frontier_tool.c�Ɏ�������������̂ł��ƂŒ�������
//	
//==============================================================================

//------------------------------------------------------------------------------
/**
 *	�o�g���^���[�g���[�i�[�f�[�^�����i�����f�[�^��B_TOWER_PARTNER_DATA�\���̂ɓW�J�j
 *
 * @param[in/out]	tr_data		��������B_TOWER_PARTNAER_DATA�\����
 * @param[in]		tr_no		�������ɂȂ�g���[�i�[ID
 * @param[in]		cnt			�g���[�i�[�Ɏ�������|�P�����̐�
 * @param[in]		set_poke_no	�y�A��g��ł���g���[�i�[�̎����|�P�����iNULL���ƃ`�F�b�N�Ȃ��j
 * @param[in]		set_item_no	�y�A��g��ł���g���[�i�[�̎����|�P�����̑����A�C�e��
 *								�iNULL���ƃ`�F�b�N�Ȃ��j
 * @param[in/out]	poke		���I���ꂽ�|�P�����̓�̂̃p�����[�^��
 *								�i�[���ĕԂ��\���̌^�f�[�^�ւ̃|�C���^(NULL���ƃ`�F�b�N�Ȃ��j
 * @param[in]		heapID		�q�[�vID
 *
 * @retval	FALSE	���I���[�v��50��ȓ��ŏI�����
 * @retval	TRUE	���I���[�v��50��ȓ��ŏI���Ȃ�����
 *
 * ���������� b_tower_fld.c RomBattleTowerTrainerDataMake
 */
//------------------------------------------------------------------------------
BOOL FSRomBattleTowerTrainerDataMake(BTOWER_SCRWORK* wk,B_TOWER_PARTNER_DATA *tr_data,u16 tr_no,int cnt,u16 *set_poke_no,u16 *set_item_no,B_TOWER_PAREPOKE_PARAM* poke,int heapID)
{
	BOOL			ret = 0;
	B_TOWER_TRAINER_ROM_DATA	*trd;
	
	//�g���[�i�[�f�[�^�Z�b�g
	trd = Frontier_TrainerDataGet(	&tr_data->bt_trd, tr_no, heapID, 
									BtlTower_GetTrArc(wk->play_mode) );

	//�|�P�����f�[�^���Z�b�g
	ret = BattleTowerPokemonSetAct(wk,trd,tr_no,&tr_data->btpwd[0],cnt,set_poke_no,set_item_no,poke,heapID);
	
	sys_FreeMemoryEz(trd);
	return ret;
}


//============================================================================================
/**
 *	�o�g���^���[�̃|�P���������߂�
 *
 * @param[in]		trd			�g���[�i�[�f�[�^
 * @param[in]		tr_no		�g���[�i�[�i���o�[
 * @param[in/out]	pwd			B_TOWER_POKEMON�\����
 * @param[in]		cnt			�g���[�i�[�Ɏ�������|�P�����̐�
 * @param[in]		set_poke_no	�y�A��g��ł���g���[�i�[�̎����|�P�����iNULL���ƃ`�F�b�N�Ȃ��j
 * @param[in]		set_item_no	�y�A��g��ł���g���[�i�[�̎����|�P�����̑����A�C�e��
 *								�iNULL���ƃ`�F�b�N�Ȃ��j
 * @param[in/out]	poke		���I���ꂽ�|�P�����̓�̂̃p�����[�^��
 *								�i�[���ĕԂ��\���̌^�f�[�^�ւ̃|�C���^(NULL���ƃ`�F�b�N�Ȃ��j
 * @param[in]		heapID		�q�[�vID
 *
 * @retval	FALSE	���I���[�v��50��ȓ��ŏI�����
 * @retval	TRUE	���I���[�v��50��ȓ��ŏI���Ȃ�����
 */
//============================================================================================
static BOOL BattleTowerPokemonSetAct(BTOWER_SCRWORK* wk,B_TOWER_TRAINER_ROM_DATA *trd,
		u16 tr_no,B_TOWER_POKEMON *pwd,u8 cnt,
		u16 *set_poke_no,u16 *set_item_no,B_TOWER_PAREPOKE_PARAM* poke,int heapID)
{
	int	i,j;
	u8	pow_rnd;
	u8	poke_index;
	u32	id;
	int	set_index;
	int	set_index_no[4];
	u32	set_rnd_no[4];
	int	set_count;
	int	loop_count;
	BOOL	ret = 0;
	B_TOWER_POKEMON_ROM_DATA	prd_s;
	B_TOWER_POKEMON_ROM_DATA	prd_d;

	//�莝���|�P������MAX��4�̂܂�
	GF_ASSERT(cnt<=4);

	set_count=0;
	loop_count=0;
	while(set_count!=cnt){
//		poke_index = gf_rand()%trd->use_poke_cnt;
		poke_index = btower_rand(wk)%trd->use_poke_cnt;
		set_index=trd->use_poke_table[poke_index];
		Frontier_PokemonRomDataGet(&prd_d,set_index,BtlTower_GetPokeArc(wk->play_mode));

		//�����X�^�[�i���o�[�̃`�F�b�N�i����̃|�P�����͎����Ȃ��j
		for(i=0;i<set_count;i++){
			Frontier_PokemonRomDataGet(&prd_s,set_index_no[i],BtlTower_GetPokeArc(wk->play_mode));
			if(prd_s.mons_no==prd_d.mons_no){
				break;
			}
		}
		if(i!=set_count){
			continue;
		}

		//�y�A��g��ł���g���[�i�[�̎����|�P�����Ƃ̃`�F�b�N
		if(set_poke_no!=NULL){
			//�����X�^�[�i���o�[�̃`�F�b�N�i����̃|�P�����͎����Ȃ��j
			for(i=0;i<cnt;i++){
				if(set_poke_no[i]==prd_d.mons_no){
					break;
				}
			}
			if(i!=cnt){
				continue;
			}
		}

		//50��܂킵�āA���܂�Ȃ��悤�Ȃ�A����A�C�e���`�F�b�N�͂��Ȃ�
		if(loop_count<50){
			//�����A�C�e���̃`�F�b�N�i����̃A�C�e���͎����Ȃ��j
			for(i=0;i<set_count;i++){
				Frontier_PokemonRomDataGet(	&prd_s,set_index_no[i],
											BtlTower_GetPokeArc(wk->play_mode));
				if((prd_s.item_no)&&(prd_s.item_no==prd_d.item_no)){
					break;
				}
			}
			if(i!=set_count){
				loop_count++;
				continue;
			}
			//�y�A��g��ł���g���[�i�[�̎����|�P�����̑����A�C�e���Ƃ̃`�F�b�N
			if(set_item_no!=NULL){
				//�����A�C�e���̃`�F�b�N�i����̃A�C�e���͎����Ȃ��j
				for(i=0;i<cnt;i++){
					if((set_item_no[i]==prd_d.item_no) && (set_item_no[i]!=0)){
						break;
					}
				}
				if(i!=cnt){
					loop_count++;
					continue;
				}
			}
		}

		set_index_no[set_count]=set_index;
		set_count++;
	}

	pow_rnd=BattleTowerPowRndGet(tr_no);
//	id=(gf_rand()|(gf_rand()<<16));
	id=(btower_rand(wk)|(btower_rand(wk)<<16));

	if(loop_count >= 50){
		ret = TRUE;
	}
	for(i=0;i<set_count;i++){
		set_rnd_no[i] = BattleTowerPokemonParamMake(wk,&(pwd[i]),
			set_index_no[i],id,0,pow_rnd,i,ret,heapID);
	}
	if(poke == NULL){
		return ret;
	}
	//�|�C���^��NULL�łȂ���΁A���I���ꂽ�|�P�����̕K�v�ȃp�����[�^��Ԃ�
	poke->poke_id = id;
	for(i = 0;i< BTOWER_STOCK_PAREPOKE_MAX;i++){
		poke->poke_no[i] = set_index_no[i];
		poke->poke_rnd[i] = set_rnd_no[i];
	}
	return ret;
}

//--------------------------------------------------------------
/**
 *	@brief	Wifi �g���[�i�[���I
 */
//--------------------------------------------------------------
void btltower_BtlPartnerSelectWifi(SAVEDATA* sv,B_TOWER_PARTNER_DATA* wk,const u8 round)
{
	int i;
	
	BTLTOWER_WIFI_DATA*	wifiSave;
	const B_TOWER_TRAINER *tr_src;
	const B_TOWER_POKEMON *poke_src;

	MI_CpuClear8(wk,sizeof(B_TOWER_PARTNER_DATA));
	
	wifiSave = SaveData_GetTowerWifiData(sv);

	if(!TowerWifiData_IsPlayerDataEnable(wifiSave)){
		//�f�[�^���Ȃ�(ROM���疳������I)
		btltower_BtlPartnerSelectSD(wk,round);
		return;
	}

	//�Z�[�u�f�[�^�����������
	TowerWifiData_GetBtlPlayerData(wifiSave,wk,round);
}

//--------------------------------------------------------------
/**
 *	@brief	�V���O���_�u�� �g���[�i�[���I���ʎ擾
 *	
 * @param[in/out]	tr_data		��������B_TOWER_PARTNAER_DATA�\����
 * @param[in]		tr_no		�������ɂȂ�g���[�i�[ID
 * @param[in]		cnt			�g���[�i�[�Ɏ�������|�P�����̐�
 * @param[in]		heapID		�q�[�vID
 */
//--------------------------------------------------------------
static void btltower_BtlPartnerSelectSD(B_TOWER_PARTNER_DATA* wk,const u8 round)
{
	int i;

	const B_TOWER_TRAINER *tr_src;
	const B_TOWER_POKEMON *poke_src;
	const B_TOWER_DMYENEMY	*poke_idx;

	MI_CpuClear8(wk,sizeof(B_TOWER_PARTNER_DATA));

	//�{����ROM/RAM�f�[�^������������Ă���
	tr_src = &(b_tower_trainer_data[round]);
	poke_src = b_tower_poke_data;
	poke_idx = &(b_tower_dmy_enemy[round]);

	//�g���[�i�[�f�[�^�R�s�[
	MI_CpuCopy8(tr_src,&wk->bt_trd,sizeof(B_TOWER_TRAINER));
	wk->bt_trd.dummy = poke_idx->obj_code;

	//�|�P�����f�[�^�R�s�[
	for(i = 0;i < 4;i++){
		MI_CpuCopy8(&(poke_src[poke_idx->pokeno[i]]),&(wk->btpwd[i]),sizeof(B_TOWER_POKEMON));
	}
}

//�����|�P��������̓����_���ł��Ă��邪�������[�v�h�~�̂��߁A
//������x�܂킵����A�|�P�����̕s��v�݂̂��`�F�b�N���āA
//�A�C�e�����Œ�Ŏ������邽�߂̃A�C�e���e�[�u��
static const u16 BattleTowerPokemonItem[]={
	ITEM_HIKARINOKONA,
	ITEM_RAMUNOMI,
	ITEM_TABENOKOSI,
	ITEM_SENSEINOTUME,
};
//============================================================================================
/**
 *	@brief	�o�g���^���[�̃|�P�����p�����[�^����
 *
 *	@param[in/out]	pwd	�|�P�����p�����[�^�̓W�J�ꏊ
 *	@param[in]		poke_no	�^���[rom�f�[�^�|�P�����i���o�[
 *	@param[in]		poke_id	�|�P�����ɃZ�b�g����id
 *	@param[in]		poke_rnd	�|�P�����ɃZ�b�g���������(0�������n���ꂽ��֐����Ő���)
 *	@param[in]		pow_rnd	�|�P�����ɃZ�b�g����pow_rnd�l
 *	@param[in]		mem_idx	�����o�[index�B��̖�or��̖�
 *	@param[in]		itemfix	TRUE�Ȃ�Œ�A�C�e���BFALSE�Ȃ�rom�f�[�^�̃A�C�e��
 *	@param[in]		heapID	�e���|�������������m�ۂ���q�[�vID
 *
 *	@return	personal_rnd:�������ꂽ�|�P�����̌������l
 *
 */
//============================================================================================
static u32 BattleTowerPokemonParamMake(BTOWER_SCRWORK* wk,B_TOWER_POKEMON* pwd,
		u16 poke_no,u32 poke_id,u32 poke_rnd,u8 pow_rnd,u8 mem_idx,BOOL itemfix,int heapID)
{
	int i;
	int	exp;
	u32	personal_rnd;
	u8	friend;
	B_TOWER_POKEMON_ROM_DATA	prd_s;
	
	MI_CpuClear8(pwd,sizeof(B_TOWER_POKEMON));
	
	//�����f�[�^���[�h
	Frontier_PokemonRomDataGet(&prd_s,poke_no,BtlTower_GetPokeArc(wk->play_mode));
	
	//�����X�^�[�i���o�[
	pwd->mons_no=prd_s.mons_no;

	//�t�H�����i���o�[
	pwd->form_no=prd_s.form_no;

	//��������
	if(itemfix){
		//50��ȏ�܂킵�Ă����t���O��TURE�Ȃ�A�����A�C�e���͌Œ�̂��̂���������
		pwd->item_no=BattleTowerPokemonItem[mem_idx];
	}else{
		//rom�f�[�^�ɓo�^���ꂽ�A�C�e������������
		pwd->item_no=prd_s.item_no;
	}

	//�Ȃ��x��255���f�t�H���g
	friend=255;
	for(i=0;i<WAZA_TEMOTI_MAX;i++){
		pwd->waza[i]=prd_s.waza[i];
		//�������������Ă���Ƃ��́A�Ȃ��x��0�ɂ���
		if(prd_s.waza[i]==WAZANO_YATUATARI){
			friend=0;
		}
	}

	//IDNo
	pwd->id_no=poke_id;

	if(poke_rnd == 0){
		//������
		do{
//			personal_rnd=(gf_rand()|gf_rand()<<16);
			personal_rnd=(btower_rand(wk)|btower_rand(wk)<<16);
#if 0
		}while((prd_s.chr!=PokeSeikakuGetRnd(personal_rnd))&&(PokeRareGetPara(poke_id,personal_rnd)==TRUE));
#else
		//�v���`�i�̓^���[���C������(08.03.17)(����������frontier_tool.c�ɂ�����̂Œ��ӁI)
		//�f�[�^�̐��i�ƈ�v���Ă��Ȃ�"��������"���A�̎��́A���[�v����
		}while((prd_s.chr!=PokeSeikakuGetRnd(personal_rnd))||(PokeRareGetPara(poke_id,personal_rnd)==TRUE));
#endif

		//OS_Printf( "���肵��personal_rnd = %d\n", personal_rnd );
		//OS_Printf( "PokeSeikakuGetRnd = %d\n", PokeSeikakuGetRnd(personal_rnd) );
		//OS_Printf( "���A����Ȃ��� = %d\n", PokeRareGetPara(poke_id,personal_rnd) );
		pwd->personal_rnd=personal_rnd;
	}else{
		pwd->personal_rnd = poke_rnd;	//0�łȂ���Έ����̒l���g�p
		personal_rnd = poke_rnd;
	}
	
		
	//�p���[����
	pwd->hp_rnd=pow_rnd;
	pwd->pow_rnd=pow_rnd;
	pwd->def_rnd=pow_rnd;
	pwd->agi_rnd=pow_rnd;
	pwd->spepow_rnd=pow_rnd;
	pwd->spedef_rnd=pow_rnd;

	//�w�͒l
	exp=0;
	for(i=0;i<6;i++){
		if(prd_s.exp_bit&No2Bit(i)){
			exp++;
		}
	}
	if((PARA_EXP_TOTAL_MAX/exp)>255){
		exp=255;
	}else{
		exp=PARA_EXP_TOTAL_MAX/exp;
	}
	for(i = 0;i < 6;i++){
		if(prd_s.exp_bit&No2Bit(i)){
			pwd->exp[i]=exp;
		}
	}

	//�Z�|�C���g
	pwd->pp_count=0;

	//���R�[�h
	pwd->country_code=CasetteLanguage;

	//����
	i=PokePersonalParaGet(pwd->mons_no,ID_PER_speabi2);
	if(i){
		if(pwd->personal_rnd&1){
			pwd->tokusei=i;
		}else{
			pwd->tokusei=PokePersonalParaGet(pwd->mons_no,ID_PER_speabi1);
		}
	}else{
		pwd->tokusei=PokePersonalParaGet(pwd->mons_no,ID_PER_speabi1);
	}

	//�Ȃ��x
	pwd->natuki=friend;

	//�j�b�N�l�[��
	MSGDAT_MonsNameGet(pwd->mons_no,heapID,&(pwd->nickname[0]));

	return personal_rnd;
}

//--------------------------------------------------------------
/**
 * @brief	�o�g���^���[�p�@�o�g���p�����[�^����
 */
//--------------------------------------------------------------
BATTLE_PARAM* BtlTower_CreateBattleParam( BTOWER_SCRWORK* wk, FRONTIER_EX_PARAM* ex_param )
{
	int i;
	u8	val8;
	u32	val32;
	BATTLE_PARAM *bp;
	SAVEDATA* sv;
	POKEPARTY* party;
	POKEMON_PARAM *pp;

	bp = BattleParam_Create(wk->heapID,btower_GetFightType(wk->play_mode));
	
	sv = ex_param->savedata;
	party = SaveData_GetTemotiPokemon(sv);

	BattleParam_SetParamByGameDataCore( bp, NULL, 
										ex_param->savedata,
										ex_param->zone_id,
										ex_param->fnote_data,
										ex_param->bag_cursor,
										ex_param->battle_cursor);

	bp->bg_id = BG_ID_TOWER;			//��{�w�i�w��
	bp->ground_id = GROUND_ID_TOWER;	//��{�n�ʎw��

	//�|�P�����f�[�^�Z�b�g
	pp = PokemonParam_AllocWork(wk->heapID);
	
	//�I�񂾎莝���|�P�������Z�b�g
	val8 = 50;
	PokeParty_Init(bp->poke_party[POKEPARTY_MINE],wk->member_num);
	for(i = 0;i < wk->member_num;i++){
		PokeCopyPPtoPP(PokeParty_GetMemberPointer(party,wk->member[i]),pp);

		//���x������
		if(PokeParaGet(pp,ID_PARA_level,NULL) > val8){
			val32 = PokeLevelExpGet(PokeParaGet(pp,ID_PARA_monsno,NULL),val8);

			PokeParaPut(pp,ID_PARA_exp,&val32);
			PokeParaCalc(pp);
		}
		BattleParam_AddPokemon(bp,pp,POKEPARTY_MINE);
	}
	sys_FreeMemoryEz(pp);

	//�g���[�i�[�f�[�^�����i�������j
	BattleParam_TrainerDataMake(bp);

	//�g���[�i�[�f�[�^(enemy1)���Z�b�g
	BattleTowerTrainerDataMake(bp,&(wk->tr_data[0]),wk->member_num,CLIENT_NO_ENEMY,wk->heapID);

	//AI�Z�b�g
	for( i=0; i < CLIENT_MAX ;i++ ){
		bp->trainer_data[i].aibit = FR_AI_EXPERT;
	}
	OS_Printf( "�g���[�i�[��AI = %d\n", FR_AI_EXPERT );

	switch(wk->play_mode){
	case BTWR_MODE_MULTI:
		//�y�A�f�[�^���Z�b�g
		BattleTowerTrainerDataMake(bp,&(wk->five_data[wk->partner]),wk->member_num,CLIENT_NO_MINE2,wk->heapID);
		//�������͋��ʏ����ŗ���Ă���
	case BTWR_MODE_COMM_MULTI:
	case BTWR_MODE_WIFI_MULTI:
		//�g���[�i�[�f�[�^(enemy2)���Z�b�g
		BattleTowerTrainerDataMake(bp,&(wk->tr_data[1]),wk->member_num,CLIENT_NO_ENEMY2,wk->heapID);
		break;
	default:
		break;
	}
	return bp;
}

//============================================================================================
/**
 *	�o�g���^���[�g���[�i�[�f�[�^����
 *
 * @param[in/out]	bp			��������BATTLE_PARAM�\����
 * @param[in]		tr_data		�������ɂȂ�g���[�i�[�f�[�^
 * @param[in]		cnt			�g���[�i�[�̎����|�P������
 * @param[in]		client_no	��������client_no
 * @param[in]		heapID		�q�[�vID�iPOKEMON_PARAM�̐����ɕK�v�j
 */
//============================================================================================
static void	BattleTowerTrainerDataMake(BATTLE_PARAM *bp,B_TOWER_PARTNER_DATA *tr_data,int cnt,int client_no,int heapID)
{
	int				i,j;
	//PMS_DATA		*pd;
	POKEMON_PARAM	*pp;

#if 0
	//�g���[�i�[ID���Z�b�g
	bp->trainer_id[client_no]=tr_data->bt_trd.player_id;

	//�g���[�i�[�f�[�^���Z�b�g
	bp->trainer_data[client_no].tr_type=tr_data->bt_trd.tr_type;
	PM_strcpy(&bp->trainer_data[client_no].name[0],&tr_data->bt_trd.name[0]);
	pd=(PMS_DATA *)&tr_data->bt_trd.win_word[0];
	bp->trainer_data[client_no].win_word=*pd;
	pd=(PMS_DATA *)&tr_data->bt_trd.lose_word[0];
	bp->trainer_data[client_no].lose_word=*pd;
#else
	BattleParamTrainerDataSet( bp, &tr_data->bt_trd, cnt, client_no, heapID );
#endif

	//�|�P�����f�[�^���Z�b�g
	pp=PokemonParam_AllocWork(heapID);
	for(i=0;i<cnt;i++){
		Frontier_PokeParaMake(&tr_data->btpwd[i],pp,FRONTIER_LEVEL_50);
		PokeParty_Add(bp->poke_party[client_no],pp);
	}

	sys_FreeMemoryEz(pp);
}

//--------------------------------------------------------------
/**
 * @brief	�o�g���^���[�@�v���C���[�h����FIGHT_TYPE��Ԃ�
 */
//--------------------------------------------------------------
static u32 btower_GetFightType(u8 play_mode)
{
	switch(play_mode){
	case BTWR_MODE_SINGLE:
	case BTWR_MODE_WIFI:
		return FIGHT_TYPE_BATTLE_TOWER_1vs1;
	case BTWR_MODE_DOUBLE:
		return FIGHT_TYPE_BATTLE_TOWER_2vs2;
	case BTWR_MODE_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_AI_MULTI;	
	case BTWR_MODE_COMM_MULTI:
	case BTWR_MODE_WIFI_MULTI:
		return FIGHT_TYPE_BATTLE_TOWER_SIO_MULTI;
	}
	return FIGHT_TYPE_BATTLE_TOWER_1vs1;
}

#if 0
//--------------------------------------------------------------
/**
 *	@brief	�o�g���^���[�@���@���ʂƃ����X�^�[No�𑗐M
 */
//--------------------------------------------------------------
void BTowerComm_SendPlayerData(BTOWER_SCRWORK* wk,SAVEDATA *sv)
{
	int i;
	POKEPARTY* party;
	POKEMON_PARAM *pp;
	MYSTATUS	*my = SaveData_GetMyStatus(sv);
	
	wk->send_buf[0] = MyStatus_GetMySex(my);
	party = SaveData_GetTemotiPokemon(sv);
	for(i = 0;i < 2;i++){
		wk->send_buf[1+i] =
			PokeParaGet(PokeParty_GetMemberPointer(party,wk->member[i]),
						ID_PARA_monsno,NULL);
	}
	wk->send_buf[3] = 
		TowerScoreData_SetStage(wk->scoreSave,BTWR_MODE_COMM_MULTI,BTWR_DATA_get);
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���^���[�@�ʐM�}���`�@���I�����g���[�i�[No���q�@�ɑ��M
 */
//--------------------------------------------------------------
void BTowerComm_SendTrainerData(BTOWER_SCRWORK* wk)
{
	MI_CpuCopy8(wk->trainer,wk->send_buf,BTOWER_STOCK_TRAINER_MAX*2);
}
#endif

//--------------------------------------------------------------
/**
 *	@brief	�o�g���^���[�@�ʐM�}���`�@���^�C�A���邩�ǂ������݂��ɑ��M
 *
 *	@param	retire	TRUE�Ȃ烊�^�C�A
 */
//--------------------------------------------------------------
void FSBTowerComm_SendRetireSelect(BTOWER_SCRWORK* wk,u16 retire)
{
	//�����̑I�����ʂ����[�N�ɕۑ�
	wk->retire_f = retire;
	wk->send_buf[0] = retire;
}

#if 0
/**
 *	@brief	�o�g���^���[�@�����Ă����v���C���[�f�[�^���󂯎��
 */
u16 BTowerComm_RecvPlayerData(BTOWER_SCRWORK* wk,const u16* recv_buf)
{
	u16	ret = 0;

	wk->pare_sex = (u8)recv_buf[0];
	wk->pare_poke[0] = recv_buf[1];
	wk->pare_poke[1] = recv_buf[2];
	wk->pare_stage = recv_buf[3];

	wk->partner = 5+wk->pare_sex;

	OS_Printf("sio multi mem = %d,%d:%d,%d\n",wk->mem_poke[0],wk->mem_poke[1],wk->pare_poke[0],wk->pare_poke[1]);
	if(	wk->mem_poke[0] == wk->pare_poke[0] ||
		wk->mem_poke[0] == wk->pare_poke[1]){
		ret += 1;
	}
	if(	wk->mem_poke[1] == wk->pare_poke[0] ||
		wk->mem_poke[1] == wk->pare_poke[1]){
		ret += 2;
	}
	return ret;
}

/**
 *	@brief	�o�g���^���[�@�����Ă����g���[�i�[�f�[�^���󂯎��
 */
u16	BTowerComm_RecvTrainerData(BTOWER_SCRWORK* wk,const u16* recv_buf)
{
	int i;

	if(CommGetCurrentID() == COMM_PARENT_ID){
		return 0;	//�e�͑��M���邾���Ȃ̂Ŏ󂯎��Ȃ�
	}

	MI_CpuCopy8(recv_buf,wk->trainer,BTOWER_STOCK_TRAINER_MAX*2);
	OS_Printf("sio multi trainer01 = %d,%d:%d,%d\n",
			wk->trainer[0],wk->trainer[1],wk->trainer[2],wk->trainer[3]);
	OS_Printf("sio multi trainer02 = %d,%d:%d,%d\n",
			wk->trainer[4],wk->trainer[5],wk->trainer[6],wk->trainer[7]);
	OS_Printf("sio multi trainer03 = %d,%d:%d,%d\n",
			wk->trainer[8],wk->trainer[9],wk->trainer[10],wk->trainer[11]);
	OS_Printf("sio multi trainer04 = %d,%d\n",
			wk->trainer[12],wk->trainer[13]);
	return 1;
}
#endif

/**
 *	@brief	�o�g���^���[�@�����Ă������^�C�A���邩�ǂ����̌��ʂ��󂯎��
 *
 *	@retval	0	���^�C�A���Ȃ�
 *	@retval	1	���^�C�A����
 */
u16	FSBTowerComm_RecvRetireSelect(BTOWER_SCRWORK* wk,const u16* recv_buf)
{
	int i;

	OS_Printf("sio multi retire = %d,%d\n",wk->retire_f,recv_buf[0]);
	if(wk->retire_f || recv_buf[0]){
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�|�P�����̃A�[�J�C�u���擾
 */
//--------------------------------------------------------------
int BtlTower_GetPokeArc( u8 play_mode )
{
	if( BtlTower_GetArcVersion(play_mode) == 0 ){
		return ARC_BTD_PM;			//�c�o
	}

	return ARC_PL_BTD_PM;			//�v���`�i
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�g���[�i�[�̃A�[�J�C�u���擾
 */
//--------------------------------------------------------------
int BtlTower_GetTrArc( u8 play_mode )
{
	if( BtlTower_GetArcVersion(play_mode) == 0 ){
		return ARC_BTD_TR;			//�c�o
	}

	return ARC_PL_BTD_TR;			//�v���`�i
}

//--------------------------------------------------------------
/**
 * @brief	�ΐ�g���[�i�[�̃��b�Z�[�W�A�[�J�C�u���擾
 */
//--------------------------------------------------------------
int BtlTower_GetTrMsgArc( u8 play_mode )
{
	if( BtlTower_GetArcVersion(play_mode) == 0 ){
		return NARC_msg_tower_trainer_dat;			//�c�o
	}

	return NARC_msg_tower_trainerpl_dat;			//�v���`�i
}

//--------------------------------------------------------------
/**
 * @brief	�o�[�W�������`�F�b�N���āA�ǂ���̃A�[�J�C�u���擾���`�F�b�N
 *
 * @retval	"0=DP�A1=�v���`�i"
 */
//--------------------------------------------------------------
static int BtlTower_GetArcVersion( u8 play_mode )
{
	MYSTATUS* my1;
	MYSTATUS* my2;
	u8 rom_code1,rom_code2;

	switch( play_mode ){

	//�ʐM
	case BTWR_MODE_COMM_MULTI:
	case BTWR_MODE_WIFI_MULTI:

		my1 = CommInfoGetMyStatus( 0 );
		if( my1 == NULL ){
			OS_Printf( "CommInfoGetMyStatus(0)�擾�o���܂���ł���\n" );
			GF_ASSERT(0);
		}

		my2 = CommInfoGetMyStatus( 1 );
		if( my2 == NULL ){
			OS_Printf( "CommInfoGetMyStatus(1)�擾�o���܂���ł���\n" );
			GF_ASSERT(0);
		}

		//�����R�[�h�擾
		rom_code1 = MyStatus_GetRomCode( my1 );
		rom_code2 = MyStatus_GetRomCode( my2 );

#if 0
		//DP�܂܂�Ă�����
		if( (rom_code1 == VERSION_DIAMOND) || (rom_code1 == VERSION_PEARL) ){
			return 0;			//�c�o
		}

		//DP�܂܂�Ă�����
		if( (rom_code2 == VERSION_DIAMOND) || (rom_code2 == VERSION_PEARL) ){
			return 0;			//�c�o
		}
#else
		//DP�܂܂�Ă�����
		if( (rom_code1 == POKEMON_DP_ROM_CODE) || (rom_code2 == POKEMON_DP_ROM_CODE) ){
			return 0;			//�c�o
		}
#endif

		return 1;			//�v���`�i(������ƃv���`�i��)
	};

	return 1;				//�v���`�i(������ƃv���`�i��)
}


