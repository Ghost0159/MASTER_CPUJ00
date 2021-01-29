//==============================================================================
/**
 * @file	frontier_tool.c
 * @brief	�o�g���t�����e�B�A�{�݂ŋ��ʂŎg����悤�ȃc�[��
 * @author	matsuda
 * @date	2007.03.20(��)
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/pm_str.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "frontier_tool.h"

#include "itemtool/itemsym.h"

#include "gflib/strbuf_family.h"
#include "system/msgdata.h"
#include "poketool/poke_memo.h"
#include "msgdata\msg.naix"
#include "battle/wazano_def.h"
#include "communication/comm_system.h"
#include "communication/comm_info.h"
#include "communication/comm_wifihistory.h"
#include "savedata/frontier_savedata.h"

#include "battle/trtype_def.h"
#include "field/fieldobj_code.h"
#include "../field/zonedata.h"

#include "wifi/dwc_rap.h"
#include "../fielddata/maptable/zone_id.h"


//==============================================================================
//	�f�[�^
//==============================================================================
///�^���[�ɏo������g���[�i�[�^�C�v����OBJ�R�[�h
static const u16 btower_trtype2objcode[][2] = {
	{TRTYPE_BTFIVE1,SEVEN1},	///<�ܐl�O1
	{TRTYPE_BTFIVE2,SEVEN2},	///<�ܐl�O1
	{TRTYPE_BTFIVE3,SEVEN3},	///<�ܐl�O1
	{TRTYPE_BTFIVE4,SEVEN4},	///<�ܐl�O1
	{TRTYPE_BTFIVE5,SEVEN5},	///<�ܐl�O1
	{TRTYPE_TANPAN,	BOY2},	///<����p��������
	{TRTYPE_MINI,	GIRL1},	///<�~�j�X�J�[�g
	{TRTYPE_SCHOOLB,	BOY1},	///<���キ������
	{TRTYPE_SCHOOLG,	GIRL3},	///<���キ������
	{TRTYPE_PRINCE,	GORGGEOUSM},	///<���ڂ������
	{TRTYPE_PRINCESS,	GORGGEOUSW},	///<�����傤����
	{TRTYPE_CAMPB,	CAMPBOY},	///<�L�����v�{�[�C
	{TRTYPE_PICNICG,	PICNICGIRL},	///<�s�N�j�b�N�K�[��
	{TRTYPE_UKIWAB,	BABYBOY1},	///<������{�[�C
	{TRTYPE_UKIWAG,	BABYGIRL1},	///<������K�[��
	{TRTYPE_DAISUKIM,	MIDDLEMAN1},	///<���������N���u
	{TRTYPE_DAISUKIW,	MIDDLEWOMAN1},	///<���������N���u
	{TRTYPE_WAITER,	WAITER},	///<�E�G�[�^�[
	{TRTYPE_WAITRESS,	WAITRESS},	///<�E�G�[�g���X
	{TRTYPE_BREEDERM,	MAN1},	///<�|�P�����u���[�_�[
	{TRTYPE_BREEDERW,	WOMAN1},	///<�|�P�����u���[�_�[
	{TRTYPE_CAMERAMAN,	CAMERAMAN},	///<�J�����}��
	{TRTYPE_REPORTER,	REPORTER},	///<���|�[�^�[
	{TRTYPE_FARMER,	FARMER},	///<�ڂ����傤��������
	{TRTYPE_COWGIRL,	COWGIRL},	///<�J�E�K�[��
	{TRTYPE_CYCLINGM,	CYCLEM},	///<�T�C�N�����O��
	{TRTYPE_CYCLINGW,	CYCLEW},	///<�T�C�N�����O��
	{TRTYPE_KARATE,	FIGHTER},	///<����Ă���
	{TRTYPE_BATTLEG,	GIRL2},	///<�o�g���K�[��
	{TRTYPE_VETERAN,	OLDMAN1},	///<�x�e�����g���[�i�[
	{TRTYPE_MADAM,	LADY},	///<�}�_��
	{TRTYPE_ESPM,	MYSTERY},	///<�T�C�L�b�J�[
	{TRTYPE_ESPW,	MYSTERY},	///<�T�C�L�b�J�[
	{TRTYPE_RANGERM,	MAN3},	///<�|�P���������W���[
	{TRTYPE_RANGERW,	WOMAN3},	///<�|�P���������W���[
	{TRTYPE_ELITEM,	MAN3},	///<�G���[�g�g���[�i�[
	{TRTYPE_ELITEW,	WOMAN3},	///<�G���[�g�g���[�i�[
	{TRTYPE_COLDELITEM,	MAN5},	///<�G���[�g�g���[�i�[���i�����j
	{TRTYPE_COLDELITEW,	WOMAN5},	///<�G���[�g�g���[�i�[���i�����j
	{TRTYPE_DRAGON,	MAN3},	///<�h���S������
	{TRTYPE_MUSHI,	BOY3},	///<�ނ��Ƃ肵�傤�˂�
	{TRTYPE_SHINOBI,	BABYBOY1},	///<�ɂ񂶂Ⴒ����
	{TRTYPE_JOGGER,	SPORTSMAN},	///<�W���M���O��
	{TRTYPE_FISHING,	FISHING},	///<��т�
	{TRTYPE_SAILOR,	SEAMAN},	///<�ӂȂ̂�
	{TRTYPE_MOUNT,	MOUNT},	///<��܂��Ƃ�
	{TRTYPE_ISEKI,	EXPLORE},	///<�������}�j�A
	{TRTYPE_GUITARIST,	MAN2},	///<�M�^���X�g
	{TRTYPE_COLLECTOR,	BIGMAN},	///<�|�P�����R���N�^�[
	{TRTYPE_HEADS,	BADMAN},	///<�X�L���w�b�Y
	{TRTYPE_SCIENTIST,	ASSISTANTM},	///<���񂫂イ����
	{TRTYPE_GENTLE,	GENTLEMAN},	///<�W�F���g���}��
	{TRTYPE_WORKER,	WORKMAN},	///<�����傤����
	{TRTYPE_PIERROT,	CLOWN},	///<�s�G��
	{TRTYPE_POLICE,	POLICEMAN},	///<���܂�肳��
	{TRTYPE_GAMBLER,	GORGGEOUSM},	///<�M�����u���[
	{TRTYPE_BIRD,	WOMAN3},	///<�Ƃ����
	{TRTYPE_PARASOL,	AMBRELLA},	///<�p���\�����˂�����
	{TRTYPE_SISTER,	WOMAN2},	///<���ƂȂ̂��˂�����
	{TRTYPE_AROMA,	WOMAN1},	///<�A���}�Ȃ��˂�����
	{TRTYPE_IDOL,	IDOL},	///<�A�C�h��
	{TRTYPE_ARTIST,	ARTIST},	///<���������
	{TRTYPE_POKEGIRL,	PIKACHU},	///<�|�P������������
};
#define TRTYPE2OBJCODE_MAX	(NELEMS(btower_trtype2objcode))


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
u8 Frontier_PowRndGet(u16 tr_no);
BOOL Frontier_PokemonIndexCreate( B_TOWER_TRAINER_ROM_DATA* trd, const u16 check_pokeno[], const u16 check_itemno[], int check_num, int get_count, u16 get_pokeindex[], int heap_id );
void Frontier_EnemyPokeMake( u8 set_num, u16 tr_index, u16 tr_index2, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd, u8 flag );


//==============================================================================
//
//	
//
//==============================================================================
//---------------------------------------------------------------------------------------------
/**
 *	�o�g���^���[�g���[�i�[�����f�[�^�̓ǂݏo��
 *
 * @param[in]	trainer_index	�g���[�i�[�i���o�[
 * @param[in]	heap_id	�������m�ۂ��邽�߂̃q�[�vID
 *
 * @retval		�擾�����g���[�i�[�����f�[�^�ւ̃|�C���^
 *
 * �g���[�i�[�f�[�^���ϒ��ȈׁA���Ń������m��
 * �I�����͌Ăяo�������������J������K�v������܂��B
 *
 * ���������� b_tower_fld.c BattleTowerTrainerRomDataGet
 */
//---------------------------------------------------------------------------------------------
B_TOWER_TRAINER_ROM_DATA * Frontier_TrainerRomDataGet(u16 trainer_index,int heap_id,int arcTR)
{
	return	ArchiveDataLoadMalloc(arcTR,trainer_index,heap_id);
}

//---------------------------------------------------------------------------------------------
/**
 *	�o�g���^���[�|�P���������f�[�^�̓ǂݏo��
 *
 * @param[in]	prd		�ǂݏo�����|�P�����f�[�^�̊i�[��
 * @param[in]	index	�ǂݏo���|�P�����f�[�^�̃C���f�b�N�X
 *
 * �g���[�i�[�f�[�^�ƈႢ�Œ蒷�ׁ̈A�O������i�[�惁������n��
 *
 * ���������� b_tower_fld.c BattleTowerPokemonRomDataGet
 */
//---------------------------------------------------------------------------------------------
void Frontier_PokemonRomDataGet(B_TOWER_POKEMON_ROM_DATA *prd,int index,int arcID)
{
	ArchiveDataLoad(prd,arcID,index);
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���^���[�g���[�i�[�����f�[�^���o�b�t�@�֓W�J
 *
 * @param   bt_trd				�g���[�i�[�f�[�^�i�[��
 * @param   trainer_index		�g���[�i�[Index
 * @param   heap_id				�q�[�vID
 *
 * @retval  �g���[�i�[ROM�f�[�^�ւ̃|�C���^
 *
 *	B_TOWER_TRAINER_ROM_DATA�^���������m�ۂ��ĕԂ��̂ŁA
 *	�Ăяo�����������I�ɉ�����邱��
 *
 * ���������� b_tower_fld.c RomTrainerDataAlloc
 */
//--------------------------------------------------------------
B_TOWER_TRAINER_ROM_DATA * Frontier_TrainerDataGet(B_TOWER_TRAINER *bt_trd, int trainer_index, int heap_id, int arcTR)
{
	B_TOWER_TRAINER_ROM_DATA	*trd;
	MSGDATA_MANAGER *man = MSGMAN_Create(MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_btdtrname_dat, heap_id);
	STRBUF			*name;

	MI_CpuClear8(bt_trd, sizeof(B_TOWER_TRAINER));
	
	trd = Frontier_TrainerRomDataGet(trainer_index, heap_id, arcTR);

	//�g���[�i�[ID���Z�b�g
	bt_trd->player_id=trainer_index;

	//�g���[�i�[�o�����b�Z�[�W
	bt_trd->appear_word[0] = 0xFFFF;
	bt_trd->appear_word[1] = trainer_index*3;
	//�g���[�i�[�f�[�^���Z�b�g
	bt_trd->tr_type=trd->tr_type;
	name=MSGMAN_AllocString(man,trainer_index);
	STRBUF_GetStringCode(name,&bt_trd->name[0],BUFLEN_PERSON_NAME);
	STRBUF_Delete(name);

	MSGMAN_Delete(man);

	return trd;
}

//�����|�P��������̓����_���ł��Ă��邪�������[�v�h�~�̂��߁A
//������x�܂킵����A�|�P�����̕s��v�݂̂��`�F�b�N���āA
//�A�C�e�����Œ�Ŏ������邽�߂̃A�C�e���e�[�u��
static const u16 FrontierPokemonItem[]={
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
 *	@param[in]		poke_index	�^���[rom�f�[�^�|�P�����i���o�[
 *	@param[in]		poke_id	�|�P�����ɃZ�b�g����id
 *	@param[in]		poke_rnd	�|�P�����ɃZ�b�g���������(0�������n���ꂽ��֐����Ő���)
 *	@param[in]		pow_rnd	�|�P�����ɃZ�b�g����pow_rnd�l
 *	@param[in]		mem_idx	�����o�[index�B���̖ڂ�(itemfix��TRUE�ɂ����ꍇ�Œ�A�C�e���擾�Ɏg�p)
 *	@param[in]		itemfix	TRUE�Ȃ�Œ�A�C�e���BFALSE�Ȃ�rom�f�[�^�̃A�C�e��
 *	@param[in]		heapID	�e���|�������������m�ۂ���q�[�vID
 *
 *	@return	personal_rnd:�������ꂽ�|�P�����̌������l
 *
 * ���������� tower_tool.c BattleTowerPokemonParamMake
 */
//============================================================================================
u32 Frontier_PokemonParamMake(B_TOWER_POKEMON* pwd,
		u16 poke_index,u32 poke_id,u32 poke_rnd,u8 pow_rnd,u8 mem_idx,BOOL itemfix,int heapID,int arcID)
{
	int i;
	int	exp;
	u32	personal_rnd;
	u8	friend;
	B_TOWER_POKEMON_ROM_DATA	prd_s;
	
	MI_CpuClear8(pwd,sizeof(B_TOWER_POKEMON));
	
	//�����f�[�^���[�h
	Frontier_PokemonRomDataGet(&prd_s,poke_index,arcID);
	
	//�����X�^�[�i���o�[
	pwd->mons_no=prd_s.mons_no;
	
	//�t�H�����i���o�[(08.04.04)
	pwd->form_no=prd_s.form_no;

	//��������
	if(itemfix){
		//50��ȏ�܂킵�Ă����t���O��TURE�Ȃ�A�����A�C�e���͌Œ�̂��̂���������
		if(mem_idx >= NELEMS(FrontierPokemonItem)){
			mem_idx %= NELEMS(FrontierPokemonItem);	//�ꉞ
		}
		pwd->item_no=FrontierPokemonItem[mem_idx];
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
	//OS_Printf( "���i = %d\n", prd_s.chr );

	if(poke_rnd == 0){
		//������
		do{
			personal_rnd=(gf_rand()|gf_rand()<<16);
#if 0
		}while((prd_s.chr!=PokeSeikakuGetRnd(personal_rnd))&&(PokeRareGetPara(poke_id,personal_rnd)==TRUE));
#else
		//�v���`�i�̓^���[���C������(08.03.17)(���̊֐��ł͂Ȃ��̂Œ��ӁI)
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
	
		
	//�p���[����(0-31)
	pwd->hp_rnd=pow_rnd;
	pwd->pow_rnd=pow_rnd;
	pwd->def_rnd=pow_rnd;
	pwd->agi_rnd=pow_rnd;
	pwd->spepow_rnd=pow_rnd;
	pwd->spedef_rnd=pow_rnd;
	OS_Printf( "����\npower_rnd = %d\n\n", pwd->power_rnd );

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
 * @brief   �o�g���^���[�p�|�P�����f�[�^����POKEMON_PARAM�𐶐�
 *
 * @param   src			�o�g���^���[�p�|�P�����f�[�^
 * @param   dest		POKEMON_PARAM�����
 * @param   level		FRONTIER_LEVEL_50 = LV50�AFRONTIER_LEVEL_OPEN = LV100�A����ȊO�͓n���ꂽ�l
 *
 * ���������� b_tower_fld.c BtlTower_PokeParaMake
 */
//--------------------------------------------------------------
void Frontier_PokeParaMake(const B_TOWER_POKEMON* src,POKEMON_PARAM* dest, u8 level)
{
	int i;
	u32 power_rnd;						//���ӁIu32�K�v
	u8	buf8,waza_pp,poke_level;
	u16	buf16;
	u32	buf32;
	
	PokeParaInit(dest);

	//OS_Printf( "monsno = %d\n", src->mons_no );

	//monsno,level,pow_rnd,idno
	if( level == FRONTIER_LEVEL_50 ){
		poke_level = 50;
	}else if( level == FRONTIER_LEVEL_OPEN ){
		poke_level = 100;
	}else{
		poke_level = level;
	}

	//OS_Printf( "power_rnd = %d\n", PokeParaGet(dest,ID_PARA_power_rnd,NULL) );
	//OS_Printf( "hp_rnd = %d\n", PokeParaGet(dest,ID_PARA_hp_rnd,NULL) );
	//OS_Printf( "pow_rnd = %d\n", PokeParaGet(dest,ID_PARA_pow_rnd,NULL) );
	//OS_Printf( "def_rnd = %d\n", PokeParaGet(dest,ID_PARA_def_rnd,NULL) );
	//OS_Printf( "agi_rnd = %d\n", PokeParaGet(dest,ID_PARA_agi_rnd,NULL) );

#if 0
	//���̊֐���u32��power_rnd�n��
	PokeParaSetPowRnd(dest,src->mons_no,poke_level,(src->power_rnd & 0x3FFFFFFF),src->personal_rnd);
#else
	//���A���o�Ȃ��悤�ɕύX(07.08.08)
	power_rnd = (src->power_rnd & 0x3FFFFFFF);
	OS_Printf( "\n�Z�b�g power_rnd = %d\n\n", power_rnd );

	//�����̊֐���u8(0-31)��pow_rnd�n��(���Ƃ�power_rnd�ɍēxPut����悤�ɂ���)
	PokeParaSet(dest, src->mons_no, poke_level, power_rnd, 
				RND_SET, src->personal_rnd, ID_NO_RARE, 0 );

	//PokeParaSet( dest, src->mons_no, poke_level, 0, RND_SET, src->personal_rnd, ID_NO_RARE, 0 );
	
	//��������u32��power_rnd���ăZ�b�g�I
	PokeParaPut( dest, ID_PARA_power_rnd, &power_rnd );

	PokeParaCalc( dest );
#endif

	//OS_Printf( "power_rnd = %d\n", PokeParaGet(dest,ID_PARA_power_rnd,NULL) );
	//OS_Printf( "hp_rnd = %d\n", PokeParaGet(dest,ID_PARA_hp_rnd,NULL) );
	//OS_Printf( "pow_rnd = %d\n", PokeParaGet(dest,ID_PARA_pow_rnd,NULL) );
	//OS_Printf( "def_rnd = %d\n", PokeParaGet(dest,ID_PARA_def_rnd,NULL) );
	//OS_Printf( "agi_rnd = %d\n", PokeParaGet(dest,ID_PARA_agi_rnd,NULL) );

	//form_no
	buf8 = src->form_no;
	PokeParaPut(dest,ID_PARA_form_no,&buf8);
	
	//�����A�C�e���ݒ�
	PokeParaPut(dest,ID_PARA_item,&src->item_no);
	
	//�Z�ݒ�
	for(i = 0;i < 4;i++){
		buf16 = src->waza[i];
		PokeParaPut(dest,ID_PARA_waza1+i,&buf16);
		buf8 = (src->pp_count >> (i*2))&0x03;
		PokeParaPut(dest,ID_PARA_pp_count1+i,&buf8);

		//pp�Đݒ�
		waza_pp = (u8)PokeParaGet(dest,ID_PARA_pp_max1+i,NULL);
		PokeParaPut(dest,ID_PARA_pp1+i,&waza_pp);
	}
	
	//ID�ݒ�
	buf32 = src->id_no;	
	PokeParaPut(dest,ID_PARA_id_no,&buf32);

	//�o���l�ݒ�
	buf8 = src->hp_exp;
	PokeParaPut(dest,ID_PARA_hp_exp,&buf8);
	buf8 = src->pow_exp;
	PokeParaPut(dest,ID_PARA_pow_exp,&buf8);
	buf8 = src->def_exp;
	PokeParaPut(dest,ID_PARA_def_exp,&buf8);
	buf8 = src->agi_exp;
	PokeParaPut(dest,ID_PARA_agi_exp,&buf8);
	buf8 = src->spepow_exp;
	PokeParaPut(dest,ID_PARA_spepow_exp,&buf8);
	buf8 = src->spedef_exp;
	PokeParaPut(dest,ID_PARA_spedef_exp,&buf8);

	//�����ݒ�
	PokeParaPut(dest,ID_PARA_speabino,&src->tokusei);
	
	//�Ȃ��ǐݒ�
	PokeParaPut(dest,ID_PARA_friend,&src->natuki);
	
	//NG�l�[���t���O���`�F�b�N
	if(src->ngname_f){
		//�f�t�H���g�l�[����W�J����
		MSGDATA_MANAGER* man;
		STRBUF* def_name;
	
		man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_monsname_dat, HEAPID_FIELD );
		def_name = MSGMAN_AllocString(man,src->mons_no);
		
		PokeParaPut(dest,ID_PARA_nickname_buf,def_name);

		STRBUF_Delete(def_name);
		MSGMAN_Delete(man);
	}else{
		//�j�b�N�l�[��
		PokeParaPut(dest,ID_PARA_nickname,src->nickname);
	}

	//�J���g���[�R�[�h
	PokeParaPut(dest,ID_PARA_country_code,&src->country_code);
	//�p�����[�^�Čv�Z
	PokeParaCalc(dest);
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[�^�C�v����l��OBJ�R�[�h��Ԃ�
 *
 * @param   tr_type		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
u16 Frontier_TrType2ObjCode(u8 tr_type)
{
	int i = 0;

	for(i = 0;i < TRTYPE2OBJCODE_MAX;i++){
		if(btower_trtype2objcode[i][0] == tr_type){
			return btower_trtype2objcode[i][1];
		}
	}
	return BOY1;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����z�������i�[
 *
 * @param	wk			FACTORY_SCRWORK�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 *
 * �e���Ƃ��Z�b�g���Ȃ��ƁA�X�e�[�^�X��ʂ̕\�������܂������Ȃ��̂ŃZ�b�g
 */
//--------------------------------------------------------------
void Frontier_BirthInfoSet( SAVEDATA* sv, POKEMON_PARAM* poke );
void Frontier_BirthInfoSet( SAVEDATA* sv, POKEMON_PARAM* poke )
{
	u32 id_no;
	STRBUF* str;
	MSGDATA_MANAGER* msgman;
	int placeID;
	//TRMEMO_SETID setID	= TRMEMO_EGGBIRTH_PLACESET;
	TRMEMO_SETID setID		= TRMEMO_POKE_PLACESET;					//�ʏ�擾(�ߊl��)
	MYSTATUS* my			= SaveData_GetMyStatus( sv );

	//Frontier_PokeParaMake�ŃZ�b�g����Ă���̂őޔ�
	id_no = PokeParaGet( poke, ID_PARA_id_no, NULL );

	PokeParaBirthInfoSet(	poke, SaveData_GetMyStatus(sv),
							//ball,        place,  ground_id,  heapID
							ITEM_MONSUTAABOORU, 0, 0, HEAPID_WORLD );

	placeID	= ZoneData_GetPlaceNameID( ZONE_ID_D32R0301 );			//�u�o�g���t�@�N�g���[�v�Œ�

	//��������my�͎����̃f�[�^��n���Ă��܂��Ă悢�̂��H
	//IDNo,�e�̐��ʁA�e�̖��O���Z�b�g�����
	TrainerMemoSetPP( poke, my, setID, placeID, HEAPID_WORLD );

	msgman = MSGMAN_Create(	MSGMAN_TYPE_NORMAL, ARC_MSG, 
							NARC_msg_factory_oyaname_dat, HEAPID_WORLD );
	str = MSGMAN_AllocString( msgman, 0 );
	PokeParaPut( poke, ID_PARA_oyaname_buf, str );					//�u�H�H�H�H�H�v�Œ�
	PokeParaPut( poke, ID_PARA_id_no, &id_no );						//�ޔ����Ă�����IDNo�Z�b�g
	STRBUF_Delete(str);
	MSGMAN_Delete( msgman );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����p�[�e�B�Ƀ|�P������ǉ�
 *
 * @param	wk			FACTORY_SCRWORK�^�̃|�C���^
 * @param	party		POKEPARTY�^�̃|�C���^
 * @param	poke		POKEMON_PARAM�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void Frontier_PokePartyAdd( SAVEDATA* sv, POKEPARTY* party, POKEMON_PARAM* poke );
void Frontier_PokePartyAdd( SAVEDATA* sv, POKEPARTY* party, POKEMON_PARAM* poke )
{
	Frontier_BirthInfoSet( sv, poke );	//�e���Ƃ��Z�b�g
	PokeParty_Add( party, poke );
	return;
}

#if 0
//--------------------------------------------------------------
/**
 * @brief	�F�B�ԍ��擾
 *
 * @param	wk			FACTORY_SCRWORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
int Frontier_GetFriendIndex( u32 record_no );
int Frontier_GetFriendIndex( u32 record_no )
{
	//���R�[�h�i���o�[�ɂ���ĕ���
	//
	//�ʐM��Ԃ��`�F�b�N�Ȃǂ��K�v�����B
	
	if( record_no < FRID_MAX ){
		return FRONTIER_RECORD_NOT_FRIEND;
	}

	//if( CommIsInitialize() ){
	return mydwc_getFriendIndex();
}
#endif


//==============================================================================
//
//	�|�P�����A�C�R���A�j��
//
//==============================================================================
#include "application/app_tool.h"
#include "poketool/pokeicon.h"

void Frontier_PokeIconAnmChg( CLACT_WORK_PTR awp, u8 anm );
void Frontier_PokeIconPosSet( CLACT_WORK_PTR awp, s16 init_x, s16 init_y, u8 flag );
u8 Frontier_PokeIconAnmNoGet( u16 hp, u16 hp_max );

//--------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j���؂�ւ�
 *
 * @param	awk		�A�N�^�[���[�N
 * @param	anm		�A�j���ԍ�
 *
 * @return	none
 */
//--------------------------------------------------------------
void Frontier_PokeIconAnmChg( CLACT_WORK_PTR awp, u8 anm )
{
	if( CLACT_AnmGet(awp) == anm ){ 
		return; 
	}

	CLACT_AnmFrameSet( awp, 0 );
	CLACT_AnmChg( awp, anm );
	CLACT_AnmFrameChg( awp, FX32_ONE );
	return;
}

//--------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j�����W�Z�b�g
 *
 * @param	awk		�A�N�^�[���[�N
 * @param	flag	1=�I�����Ă���|�P����
 *
 * @return	none
 */
//--------------------------------------------------------------
void Frontier_PokeIconPosSet( CLACT_WORK_PTR awp, s16 init_x, s16 init_y, u8 flag )
{
	VecFx32 vec;

	vec.x = init_x * FX32_ONE;
	vec.y = init_y * FX32_ONE;
	vec.z = 0;

	if( flag == 1 ){
		if( CLACT_AnmFrameGet(awp) == 0 ){
			vec.y = (init_y - 3)  * FX32_ONE;
		}else{
			vec.y = (init_y + 1)  * FX32_ONE;
		}
	}

	CLACT_SetMatrix( awp, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j��No�擾
 *
 * @param	awk		�A�N�^�[���[�N
 * @param	flag	1=�I�����Ă���|�P����
 *
 * @return	none
 */
//--------------------------------------------------------------
u8 Frontier_PokeIconAnmNoGet( u16 hp, u16 hp_max )
{
	switch( GetHPGaugeDottoColor(hp,hp_max,48) ){
	case HP_DOTTO_MAX:
		return POKEICON_ANM_HPMAX;
	case HP_DOTTO_GREEN:	// ��
		return POKEICON_ANM_HPGREEN;
	case HP_DOTTO_YELLOW:	// ��
		return POKEICON_ANM_HPYERROW;
	case HP_DOTTO_RED:		// ��
		return POKEICON_ANM_HPRED;
	};

#if 0	//�f�o�b�N//////////////////////////////////////////////////////////
		if( sys.cont & PAD_BUTTON_L ){
			return POKEICON_ANM_HPMAX;
		}else if( sys.cont & PAD_BUTTON_R ){
			return POKEICON_ANM_HPGREEN;
		}else if( sys.cont & PAD_BUTTON_SELECT ){
			return POKEICON_ANM_HPYERROW;
		}else if( sys.cont & PAD_BUTTON_START ){
			return POKEICON_ANM_HPRED;
		}
#endif	////////////////////////////////////////////////////////////////////

	return POKEICON_ANM_HPMAX;
}


//==============================================================================
//
//	�o�g���^���[�g���[�i�[�f�[�^����(tower_tool.c����ړ�)
//
//==============================================================================
void FSRomBattleTowerTrainerDataMake2( B_TOWER_PARTNER_DATA *tr_data, u16 tr_no, int heapID, int arcTR );
void BattleParamTrainerDataSet( BATTLE_PARAM* bp, B_TOWER_TRAINER* bt_trd, int cnt, int client_no, int heapID );
u32 Frontier_PokemonParamCreate( B_TOWER_POKEMON *pwd, u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id, int arcPM );
u32 Frontier_PokemonParamCreateStage( B_TOWER_POKEMON *pwd, u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id );

//------------------------------------------------------------------------------
/**
 *	�o�g���^���[�g���[�i�["�̂�"�f�[�^�����i�����f�[�^��B_TOWER_PARTNER_DATA�\���̂ɓW�J�j
 *
 * @param[in/out]	tr_data		��������B_TOWER_PARTNAER_DATA�\����
 * @param[in]		tr_no		�������ɂȂ�g���[�i�[ID
 *								�iNULL���ƃ`�F�b�N�Ȃ��j
 * @param[in]		heapID		�q�[�vID
 *
 * @retval	none
 */
//------------------------------------------------------------------------------
void FSRomBattleTowerTrainerDataMake2( B_TOWER_PARTNER_DATA *tr_data, u16 tr_no, int heapID, int arcTR )
{
	B_TOWER_TRAINER_ROM_DATA* trd;

	//�g���[�i�[�f�[�^�Z�b�g
	trd = Frontier_TrainerDataGet( &tr_data->bt_trd, tr_no, heapID, arcTR );

	sys_FreeMemoryEz(trd);
	return;
}

//--------------------------------------------------------------
/**
 *	BATTLE_PARAM�Ƀg���[�i�[�f�[�^�Z�b�g
 *
 * @param[in/out]	bp			��������BATTLE_PARAM�\����
 * @param[in]		tr_data		�������ɂȂ�g���[�i�[�f�[�^
 * @param[in]		cnt			�g���[�i�[�̎����|�P������
 * @param[in]		client_no	��������client_no
 * @param[in]		heapID		�q�[�vID�iPOKEMON_PARAM�̐����ɕK�v�j
 */
//--------------------------------------------------------------
void BattleParamTrainerDataSet( BATTLE_PARAM* bp, B_TOWER_TRAINER* bt_trd, int cnt, int client_no, int heapID )
{
	PMS_DATA		*pd;
	POKEMON_PARAM	*pp;

	//�g���[�i�[ID���Z�b�g
	bp->trainer_id[client_no]				= bt_trd->player_id;

	//�g���[�i�[�f�[�^���Z�b�g
	bp->trainer_data[client_no].tr_type		= bt_trd->tr_type;
	PM_strcpy( &bp->trainer_data[client_no].name[0], &bt_trd->name[0] );

	pd = (PMS_DATA*)&bt_trd->win_word[0];
	bp->trainer_data[client_no].win_word	= *pd;

	pd = (PMS_DATA*)&bt_trd->lose_word[0];
	bp->trainer_data[client_no].lose_word	= *pd;
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P��������
 *
 * @param   pwd				�|�P�����p�����[�^�̓W�J�ꏊ
 * @param   poke_index		�|�P����Index
 * @param   position		�莝���ʒu
 * @param   pow_rnd			�p���[����
 * @param   personal_rnd	������(0�̏ꍇ�͓����Ŏ�������)
 * @param   heap_id			�q�[�vID
 *
 * @retval  �������ꂽ�|�P�����̌������l
 */
//--------------------------------------------------------------
u32 Frontier_PokemonParamCreate( B_TOWER_POKEMON *pwd, u16 poke_index, int position, u8 pow_rnd, u32 personal_rnd, int heap_id, int arcPM )
{
	u32 id, poke_rnd, set_rnd_no;
	
	id = gf_rand() | (gf_rand() << 16);

	//set_rnd_no = Frontier_PokemonParamMake(pwd, poke_index, id, 0, 
	set_rnd_no = Frontier_PokemonParamMake(pwd, poke_index, id, personal_rnd, 
		pow_rnd, position, FALSE, heap_id, arcPM);
	
	return set_rnd_no;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�Ŏg�p����|�P��������x�ɕ����C����(07.12.04�X�e�[�W�ȊO)
 *
 * @param   pwd					�|�P�����p�����[�^�W�J�ꏊ(set_num���̗v�f��������z��ł��鎖)
 * @param   range				�|�P�����f�[�^���o�͈̓f�[�^�ւ̃|�C���^(�p���[�����擾�Ɏg�p)
 * @param   poke_index[]		�|�P����Index�̔z��ւ̃|�C���^
 * @param   personal_rnd[]		�������̔z��ւ̃|�C���^(NULL�̏ꍇ�͓����Ŏ�������)
 * @param   ret_personal_rnd[]	������������Ŏ������������ꍇ�̎󂯎��ꏊ(�s�v�Ȃ�NULL)
 * @param   set_num				���̂̃|�P�����𐶐����邩
 * @param   heap_id				�q�[�vID
 */
//--------------------------------------------------------------
void Frontier_PokemonParamCreateAll(B_TOWER_POKEMON *pwd, u16 poke_index[], 
	u8 pow_rnd[], u32 personal_rnd[], u32 ret_personal_rnd[], int set_num, int heap_id, int arcPM)
{
	int i;
	u32 per_rnd;
	u8 power_rnd;
	
	if(personal_rnd == NULL){
		per_rnd = 0;
	}
	
	for(i = 0; i < set_num; i++){
		power_rnd	= (pow_rnd == NULL) ? 0 : pow_rnd[i];
		per_rnd		= (personal_rnd == NULL) ? 0 : personal_rnd[i];
		per_rnd		= Frontier_PokemonParamCreate(	&pwd[i], poke_index[i], i, 
													power_rnd, per_rnd, heap_id, arcPM);
		if( ret_personal_rnd != NULL ){
			ret_personal_rnd[i] = per_rnd;
			OS_Printf( "per_rnd = %d\n", per_rnd );
		}
	}

	return;
}

//---------------------------------------------------------------------------------------------
/**
 *	�o�g���^���[�g���[�i�[�̎����|�P�����̃p���[���������肷��
 *
 * @param	tr_no	�g���[�i�[�i���o�[
 *
 * @return	�p���[����
 *
 * src/field/b_tower_fld.c�Ɠ���
 */
//---------------------------------------------------------------------------------------------
u8 Frontier_PowRndGet(u16 tr_no)
{
	u8	pow_rnd;

	if(tr_no<100){
		pow_rnd=(0x1f/8)*1;
	}
	else if(tr_no<120){
		pow_rnd=(0x1f/8)*2;
	}
	else if(tr_no<140){
		pow_rnd=(0x1f/8)*3;
	}
	else if(tr_no<160){
		pow_rnd=(0x1f/8)*4;
	}
	else if(tr_no<180){
		pow_rnd=(0x1f/8)*5;
	}
	else if(tr_no<200){
		pow_rnd=(0x1f/8)*6;
	}
	else if(tr_no<220){
		pow_rnd=(0x1f/8)*7;
	}
	else{
		pow_rnd=0x1f;
	}
	return pow_rnd;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�Ŏg�p����|�P������Index�����߂�
 *			�����|�P�����A��������ɂȂ�Ȃ��悤��Index���擾���܂�
 *
 * @param   trd					B_TOWER_TRAINER_ROM_DATA�^�̃|�C���^
 * @param   check_pokeno[]		���h�~�Ń`�F�b�N����|�P�����ԍ�(Index�ł͂Ȃ����ɒ���)
 * @param   check_itemno[]		���h�~�Ń`�F�b�N����A�C�e���ԍ�
 * @param   check_num			check_pokeno��check_itemno�̃f�[�^��
 * @param   get_count			���̂̃|�P����Index���擾���邩
 * @param   ret_pokeindex[]		���܂����|�P����Index�̌��ʑ����(get_count���̔z�񐔂����鎖!)
 * @param   heap_id				�q�[�vID
 *
 * @retval	FALSE	���I���[�v��50��ȓ��ŏI�����
 * @retval	TRUE	���I���[�v��50��ȓ��ŏI���Ȃ�����
 *
 * ���h�~���g�p���Ȃ��ꍇ��check_num��0�ɂ��Ă��������B(check_pokeno, check_itemno��NULL)
 */
//--------------------------------------------------------------
BOOL Frontier_PokemonIndexCreate( B_TOWER_TRAINER_ROM_DATA* trd, const u16 check_pokeno[], const u16 check_itemno[], int check_num, int get_count, u16 get_pokeindex[], int heap_id )
{
	int set_count,set_index,loop_count;
	B_TOWER_POKEMON_ROM_DATA prd_d[6];
	int i,poke_index;
	
	GF_ASSERT(get_count <= 6);	//prd_d�̔z�񐔂𒴂��Ă����ASSERT
	
	set_count	= 0;
	loop_count	= 0;
	
	OS_Printf( "use_poke_cnt = %d\n", trd->use_poke_cnt );

	while( set_count != get_count ){

		//�������_���v�m�F
		poke_index = ( gf_rand() % trd->use_poke_cnt );
		//OS_Printf( "poke_index = %d\n", poke_index );
		//poke_index = btower_rand(wk)%trd->use_poke_cnt;
		
		set_index = trd->use_poke_table[poke_index];
		Frontier_PokemonRomDataGet( &prd_d[set_count], set_index, ARC_PL_BTD_PM );
		OS_Printf( "set_index = %d\n", set_index );
		
		//���܂Ő������Ă����|�P�����Ɣ���Ă��Ȃ����`�F�b�N
		for(i = 0; i < set_count; i++){
			if(prd_d[i].mons_no == prd_d[set_count].mons_no 
					|| prd_d[i].item_no == prd_d[set_count].item_no){
				break;	//�����
			}
		}

		OS_Printf( "i = %d\n", i );
		OS_Printf( "set_count = %d\n", set_count );

		if(i != set_count){
			OS_Printf( "continue\n" );
			continue;
		}
		
		if( loop_count < 50 ){
			//�`�F�b�N�p�ɓn����Ă��郂���X�^�[�i���o�[�A�����A�C�e���̔��`�F�b�N
			for(i = 0; i < check_num; i++){
				if(prd_d[set_count].mons_no == check_pokeno[i] 
						|| prd_d[set_count].item_no ==  check_itemno[i]){
					break;	//�����
				}
			}
			if(i != check_num){
				loop_count++;
				continue;
			}
		}

		get_pokeindex[set_count] = set_index;
		set_count++;
	}
	
	if( loop_count >= 50 ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�G���o���ŏ��̃|�P����3(4)�C�����߂�
 *
 * @param	set_num
 * @param	tr_index
 * @param	tr_index2			�y�A
 * @param	enemy_poke_index
 * @param	enemy_poke
 * @param	personal_rnd
 *
 * @return	
 *
 * �ȉ��̏��ŏ������Ă���
 * �����^��6
 * (���`�F�b�N:�����^��6) �G3
 * (���`�F�b�N:�莝��3)(���`�F�b�N:�O�̓G3) �G3
 * ...
 */
//--------------------------------------------------------------
void Frontier_EnemyPokeMake( u8 set_num, u16 tr_index, u16 tr_index2, u16* enemy_poke_index, B_TOWER_POKEMON* enemy_poke, u8* pow_rnd, u32* personal_rnd, u8 flag )
{
	int i;
	B_TOWER_POKEMON poke;
	u16 poke_check_tbl[4];
	u16 item_check_tbl[4];
	B_TOWER_TRAINER bt_trd;
	B_TOWER_TRAINER_ROM_DATA* trd;
	B_TOWER_POKEMON_ROM_DATA prd_d;

	//ROM����g���[�i�[�f�[�^���m��
	trd = Frontier_TrainerDataGet( &bt_trd, tr_index, HEAPID_WORLD, ARC_PL_BTD_TR );

	///////////////////////////////////////////////////////////////
	//�ŏ��́A��l�ڂ̃g���[�i�[����Ȃ��őI�o
	//
	//���̌�A��l�ڂ̃g���[�i�[����e�[�u�������������ƂɑI�o
	//
	//�g���[�i�[�f�[�^�����ɂ��Ȃ��Ƃ����Ȃ�
	///////////////////////////////////////////////////////////////

	//�p���[�������
	for( i=0; i < set_num; i++ ){
		pow_rnd[i] = Frontier_PowRndGet( tr_index );
	}

	//�V���O���̎�
	if( flag == 0  ){

		//�G�|�P�����̃C���f�b�N�X�擾(���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����)
		Frontier_PokemonIndexCreate(trd, poke_check_tbl, item_check_tbl, 0,
									set_num, enemy_poke_index, HEAPID_WORLD );

	//�}���`�̎�
	}else{

		//�G�|�P�����̃C���f�b�N�X�擾(���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����)
		Frontier_PokemonIndexCreate(trd, poke_check_tbl, item_check_tbl, 0,
									(set_num / 2), enemy_poke_index, HEAPID_WORLD );

		//���f�[�^�쐬
		for( i=0; i < (set_num / 2) ;i++ ){
			Frontier_PokemonRomDataGet( &prd_d, enemy_poke_index[i], ARC_PL_BTD_PM );
			poke_check_tbl[i] = prd_d.mons_no;
			item_check_tbl[i] = prd_d.item_no;
		}

		//ROM����g���[�i�[�f�[�^���m��
		sys_FreeMemoryEz( trd );
		trd = Frontier_TrainerDataGet( &bt_trd, tr_index2, HEAPID_WORLD, ARC_PL_BTD_TR );

		//�G�|�P�����̃C���f�b�N�X�擾(���|�P�A�A�C�e���A�f�[�^���A�擾���鐔�A�����)
		Frontier_PokemonIndexCreate(trd, poke_check_tbl, item_check_tbl, (set_num / 2),
									(set_num / 2), &enemy_poke_index[set_num / 2], HEAPID_WORLD );

		//�p���[�������
		for( i=0; i < (set_num / 2) ;i++ ){
			pow_rnd[i+(set_num / 2)] = Frontier_PowRndGet( tr_index2 );
		}
	}

	sys_FreeMemoryEz( trd );

	//�G�|�P��������x�ɐ���(personal�Ȃ�)
	Frontier_PokemonParamCreateAll(	enemy_poke, enemy_poke_index, 
									pow_rnd, NULL, personal_rnd, set_num, HEAPID_WORLD,
									ARC_PL_BTD_PM );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[�̖��O��buf�ɃZ�b�g
 *
 * @param	wordset		WORDSET�^�̃|�C���^
 * @param	bufID		�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
#if 0
void Frontier_PairNameSet( WORDSET* wordset, u16 bufID )
{
	STRBUF* strbuf;
	MYSTATUS* my;						//�p�[�g�i�[��MyStatus

	strbuf = STRBUF_Create( (PERSON_NAME_SIZE + EOM_SIZE), HEAPID_WORLD );

	//�p�[�g�i�[��MyStatus���擾
	my = CommInfoGetMyStatus( 1 - CommGetCurrentID() );

	//���O�擾(STRBUF�ɃR�s�[)
	MyStatus_CopyNameString( my, strbuf );

	WORDSET_RegisterWord(	wordset, bufID, strbuf, 
							MyStatus_GetMySex(my), 0, PM_LANG );

	STRBUF_Delete( strbuf );
	return;
}
#endif

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[�̖��O���o�b�t�@�ɃZ�b�g
 *
 * @param	wordset		WORDSET�^�̃|�C���^
 * @param	bufID		�o�b�t�@ID
 *
 * @return	none
 */
//--------------------------------------------------------------
void Frontier_PairNameWordSet( WORDSET* wordset, u32 bufID )
{
	WORDSET_RegisterPlayerName( wordset, bufID, 
								CommInfoGetMyStatus( (CommGetCurrentID() ^ 1) ) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   MYSTATUS����g�p����OBJCODE�����肷��
 * @param   my_status		MYSTATUS�ւ̃|�C���^
 * @retval  OBJCODE
 */
//--------------------------------------------------------------
int FrontierTool_MyObjCodeGet(const MYSTATUS *my_status)
{
	u32 my_sex;
	int objcode, rom_code;
	
	my_sex = MyStatus_GetMySex(my_status);
	rom_code = MyStatus_GetRomCode(my_status);
	switch(rom_code){
	case VERSION_PLATINUM:
	case VERSION_GOLD:	//��check ������b��Ńv���`�i�Ɠ�����l���L�������o��
	case VERSION_SILVER:
	default:
		objcode = (my_sex == PM_MALE) ? HERO : HEROINE;
		break;
	case POKEMON_DP_ROM_CODE:	//�_�C�����p�[��
		//��check DP�p�̎�l���L������OBJCODE��ݒ肷��\��
		objcode = (my_sex == PM_MALE) ? DPHERO : DPHEROINE;
		break;
	}
	
	return objcode;
}

//--------------------------------------------------------------
/**
 * @brief   �n���V�o�^
 *
 * @param   SAVEDATA	savedata
 *
 * @retval  none
 */
//--------------------------------------------------------------
void FrontierTool_WifiHistory( SAVEDATA* savedata )
{
	WIFI_HISTORY* pHistory = SaveData_GetWifiHistory( savedata );
	Comm_WifiHistoryCheck( pHistory );
	return;
}


