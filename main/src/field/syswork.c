//============================================================================================
/**
 * @file	syswork.c
 * @bfief	�v���O�����A�X�N���v�g����A�N�Z�X�����V�X�e�����[�N����
 * @author	Satoshi Nohara
 * @date	06.03.12
 */
//============================================================================================
#include "common.h"
#include "fieldsys.h"
#include "script.h"
#include "field/evwkdef.h"
#include "syswork.h"
#include "sysflag.h"				//SysFlag_
#include "poketool/monsno.h"		//�|�P�����i���o�[

#include "savedata/randomgroup.h"

//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
static BOOL SysWork_Set( EVENTWORK* ev, u16 work, u16 no );
static u16 SysWork_Get( EVENTWORK* ev, u16 work );
BOOL SysWork_FirstPokeNoSet( EVENTWORK* ev, u16 no );
u16 SysWork_FirstPokeNoGet( EVENTWORK* ev );
u16 SysWork_RivalPokeNoGet( EVENTWORK* ev );
u16 SysWork_SupportPokeNoGet( EVENTWORK* ev );


//============================================================================================
//
//	�֐�
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �V�X�e�����[�N�Z�b�g
 *
 * @param	ev			EVENTWORK�ւ̃|�C���^
 * @param	work		���[�N�i���o�[
 * @param	no			�Z�b�g����l
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
static BOOL SysWork_Set( EVENTWORK* ev, u16 work, u16 no )
{
	u16* p	= EventWork_GetEventWorkAdrs( ev, work );

	if( (work < SVWK_START) || (work > SCWK_START ) ){
		GF_ASSERT( (0) && "�s���ȃ��[�NID���n����܂����I" );
		return FALSE;
	}

	if( p == NULL ){
		return FALSE;
	}

	*p = no;
	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * �V�X�e�����[�N�Q�b�g
 *
 * @param	ev			EVENTWORK�̃|�C���^
 * @param	work		���[�N�i���o�[
 *
 * @return	"���[�N�̒l"
 */
//--------------------------------------------------------------------------------------------
static u16 SysWork_Get( EVENTWORK* ev, u16 work )
{
	u16* p	= EventWork_GetEventWorkAdrs( ev, work );

	if( p == NULL ){
		return 0;
	}

	return *p;
}


//============================================================================================
//
//	�A������g���[�i�[ID�i�[���[�N�֘A
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �A������g���[�i�[ID�i�[���[�N�Z�b�g
 *
 * @param	ev			EVENTWORK�ւ̃|�C���^
 * @param	no			�g���[�i�[ID
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_PairTrainerIDSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_PAIR_TRAINER_ID, no );
}

//--------------------------------------------------------------------------------------------
/**
 * �A������g���[�i�[ID�i�[���[�N�Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�g���[�i�[ID"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_PairTrainerIDGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_PAIR_TRAINER_ID );
}


//============================================================================================
//
//	�ŏ��̃|�P�����i���o�[�֘A
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �ŏ��̃|�P�����i���o�[�Z�b�g
 *
 * @param	ev			EVENTWORK�ւ̃|�C���^
 * @param	no			�|�P�����i���o�[
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_FirstPokeNoSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_FIRST_POKE_NO, no );
}

//--------------------------------------------------------------------------------------------
/**
 * �ŏ��̃|�P�����i���o�[�Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�ŏ��̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_FirstPokeNoGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_FIRST_POKE_NO );
}

//--------------------------------------------------------------------------------------------
/**
 * ���C�o���̃|�P�����i���o�[�Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"���C�o���̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_RivalPokeNoGet( EVENTWORK* ev )
{
	u16 rival;
	u16 first = SysWork_Get( ev, SYS_WORK_FIRST_POKE_NO );

#if 1
	if( first == MONSNO_NAETORU ){
		rival = MONSNO_HIKOZARU;
	}else if( first == MONSNO_HIKOZARU ){
		rival = MONSNO_POTTYAMA;
	}else{
		rival = MONSNO_NAETORU;
	}
#endif

	return rival;
}

//--------------------------------------------------------------------------------------------
/**
 * �T�|�[�g�̃|�P�����i���o�[�Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�T�|�[�g�̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_SupportPokeNoGet( EVENTWORK* ev )
{
	u16 support;
	u16 first = SysWork_Get( ev, SYS_WORK_FIRST_POKE_NO );

#if 1
	if( first == MONSNO_NAETORU ){
		support = MONSNO_POTTYAMA;
	}else if( first == MONSNO_HIKOZARU ){
		support = MONSNO_NAETORU;
	}else{
		support = MONSNO_HIKOZARU;
	}
#endif

	return support;
}


//============================================================================================
//
//	������������׊֘A
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * ���݂̋L�^���擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�̒l"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_OokisaRecordGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_OOKISA_RECORD );
}

//--------------------------------------------------------------------------------------------
/**
 * �L�^������������
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no			�̒l
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_OokisaRecordSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_OOKISA_RECORD, no );
}


//============================================================================================
//
//	�o�g���T�[�`���[�֘A
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �}�b�v�J�ڂ������̃��Z�b�g����
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void Sys_BtlSearcherReset( EVENTWORK* ev )
{
	SysFlag_BtlSearcherUseReset( ev );		//�g�p��������
	SysWork_BtlSearcherClearSet( ev, 0 );	//�N���A�J�E���g������
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �o�b�e���[�J�E���g���擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_BtlSearcherBatteryGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_BTL_SEARCHER_BATTERY );
}

//--------------------------------------------------------------------------------------------
/**
 * �o�b�e���[�J�E���g���Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_BtlSearcherBatterySet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_BTL_SEARCHER_BATTERY, no );
}

//--------------------------------------------------------------------------------------------
/**
 * �N���A�J�E���g���擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_BtlSearcherClearGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_BTL_SEARCHER_CLEAR );
}

//--------------------------------------------------------------------------------------------
/**
 * �N���A�J�E���g���Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_BtlSearcherClearSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_BTL_SEARCHER_CLEAR, no );
}




//============================================================================================
//
//
//			�z�z�C�x���g�֘A
//
//
//============================================================================================
//--------------------------------------------------------------------------------------------
/**
 * @brief	���ꂼ��̔z�z�C�x���g�ɑΉ������}�W�b�N�i���o�[��Ԃ�
 */
//--------------------------------------------------------------------------------------------
static get_haihu_magicnumber(int ex_event_id)
{
	static const u16 magic_number[] = {
		0x1209,
		0x1112,
		0x1123,
		0x1103,
	};
	GF_ASSERT(0 <= ex_event_id && ex_event_id < NELEMS(magic_number));
	return magic_number[ex_event_id];
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void SysWork_HaihuEventWorkSet(EVENTWORK * ev, int haihu_id)
{
	SysWork_Set(ev, SYS_WORK_HAIHU_EVENT01 + haihu_id, get_haihu_magicnumber(haihu_id));
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void SysWork_HaihuEventWorkClear(EVENTWORK * ev, int haihu_id)
{
	SysWork_Set(ev, SYS_WORK_HAIHU_EVENT01 + haihu_id, 0);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
BOOL SysWork_HaihuEventWorkCheck(EVENTWORK * ev, int haihu_id)
{
	if (SysWork_Get(ev, SYS_WORK_HAIHU_EVENT01 + haihu_id) == get_haihu_magicnumber(haihu_id)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

//============================================================================================
//
//
//			�B���}�b�v�֘A
//
//
//============================================================================================
//--------------------------------------------------------------------------------------------
/**
 * @brief	���ꂼ��ɑΉ������}�W�b�N�i���o�[��Ԃ�
 */
//--------------------------------------------------------------------------------------------
static get_hidemap_magicnumber(int hidemap_id)
{
	static const u16 magic_number[] = {
		0x0208,
		0x0229,
		0x0312,
		0x1028,
	};
	GF_ASSERT(0 <= hidemap_id && hidemap_id < 4);
	return magic_number[hidemap_id];

}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void SysWork_HideMapWorkSet(EVENTWORK * ev, int hidemap_id)
{
	SysWork_Set(ev, SYS_WORK_HIDEMAP_01 + hidemap_id, get_hidemap_magicnumber(hidemap_id));
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void SysWork_HideMapWorkClear(EVENTWORK * ev, int hidemap_id)
{
	SysWork_Set(ev, SYS_WORK_HIDEMAP_01 + hidemap_id, 0);
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
BOOL SysWork_HideMapWorkCheck(EVENTWORK * ev, int hidemap_id)
{
	if(SysWork_Get(ev, SYS_WORK_HIDEMAP_01 + hidemap_id) == get_hidemap_magicnumber(hidemap_id)) {
		return TRUE;
	} else {
		return FALSE;
	}
}


//--------------------------------------------------------------------------------------------
/**
 * ���R�����̑������N���A
 *
 * @param	ev			EVENTWORK�ւ̃|�C���^
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_ParkWalkCountClear( EVENTWORK* ev)
{
	return SysWork_Set( ev, SYS_WORK_PARK_WALK_COUNT, 0 );
}

//--------------------------------------------------------------------------------------------
/**
 * ���R�����̑������Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�ŏ��̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_ParkWalkCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_PARK_WALK_COUNT );
}

//--------------------------------------------------------------------------------------------
/**
 * ���R�����̑��������{�P
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�ŏ��̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_ParkWalkCountPlus( EVENTWORK* ev )
{
	u16 count;
	count = SysWork_Get( ev, SYS_WORK_PARK_WALK_COUNT );
	if(count < 10000) {		//�����ł̎g�p���l�����đ��߂Ɂi���R�����݂̂Ȃ�Q�O�O��OK�j
		count++;
	}else{
		count = 10000;
	}
	return SysWork_Set( ev, SYS_WORK_PARK_WALK_COUNT, count );
}

//--------------------------------------------------------------------------------------------
/**
 * �V���В��ߐ؂�J�E���^���擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_NewsDaysCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_NEWS_DAYS_COUNT );
}

//--------------------------------------------------------------------------------------------
/**
 * �V���В��ߐ؂�J�E���^���Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_NewsDaysCountSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_NEWS_DAYS_COUNT, no );
}

//--------------------------------------------------------------------------------------------
//			�|�P�����N�W�֘A
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����N�W�����ɒl���Z�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void SysWork_pokelot_value_set( EVENTWORK* ev ,u32 val)
{
	u16 L, H;

	H = (val >> 16) & 0xffff;
	L = val & 0xffff;
	SysWork_Set( ev, SYS_WORK_POKELOT_RND1, L );
	SysWork_Set( ev, SYS_WORK_POKELOT_RND1, H );

}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����N�W�����̒l���Q�b�g
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
u32 SysWork_pokelot_value_get(EVENTWORK* ev)
{
	u16 L, H;
	L = SysWork_Get( ev, SYS_WORK_POKELOT_RND1 );
	H = SysWork_Get( ev, SYS_WORK_POKELOT_RND2 );
	return ((H<<16)|L);
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����N�W�����̒l��������
 *  �Q�[���J�n���̏���������
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void SysWorkInitPokeLot(EVENTWORK* ev)
{
	u16  L, H;

	L = gf_rand();
	H = gf_rand();
	OS_Printf("PokeLot L[%d] H[%d]\n",L,H);
	SysWork_pokelot_value_set(ev,(H<<16) | L);
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����N�W�����̒l��������
 * �����o�߂ɂ��X�V����
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void SysWorkUpdatePokeLot(SAVEDATA *sv,u16 days)
{
	EVENTWORK * ev = SaveData_GetEventWork(sv);
	u32  value;

	value = RandomGroup_GetDefaultRandom(SaveData_GetRandomGroup(sv));

	value = value * 1103515245L + 12345;

	SysWork_pokelot_value_set(ev,value);
}


//--------------------------------------------------------------------------------------------
/**
 * �P���P��A�|�P�����̃��x���Ɠ������̂Ƃ��A�A�C�e��������邨������
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_PokeLevelNoGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_POKE_LEVEL_NO );
}

//--------------------------------------------------------------------------------------------
/**
 * �P���P��A�|�P�����̃��x���Ɠ������̂Ƃ��A�A�C�e��������邨������
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_PokeLevelNoSet( EVENTWORK* ev ,u16 no)
{
	return SysWork_Set( ev, SYS_WORK_POKE_LEVEL_NO, no );
}

//--------------------------------------------------------------------------------------------
/**
 * �P���P��A�|�P�����̃��x���Ɠ������̂Ƃ��A�A�C�e��������邨������
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void SysWorkUpdatePokeLevelNo(SAVEDATA *sv)
{
	EVENTWORK * ev = SaveData_GetEventWork(sv);
	u32  value;

	value = (gf_rand() % 98) + 2;		//2-99
	SysWork_PokeLevelNoSet(ev,value);
}

//--------------------------------------------------------------------------------------------
/**
 * �n����b�J�E���g���擾(�~�J�Q��p)
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_UGTalkCountGet( EVENTWORK* ev )
{
	u16		count;
	count = SysWork_Get( ev, SYS_WORK_UG_TALK_COUNT );
	OS_Printf("SYS_WORK_UG_TALK_COUNT [%d]\n",count);
	return count;
}

//--------------------------------------------------------------------------------------------
/**
 * �n����b�J�E���g���Z�b�g(�~�J�Q��p)
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_UGTalkCountSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_UG_TALK_COUNT , no );
}



//--------------------------------------------------------------------------------------------
/**
 * �n����b�J�E���g���擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_UGTalkCount2Get( EVENTWORK* ev )
{
	u16		count;
	count = SysWork_Get( ev, SYS_WORK_UG_TALK_COUNT2 );
	OS_Printf("SYS_WORK_UG_TALK_COUNT2 [%d]\n",count);
	return count;
}

//--------------------------------------------------------------------------------------------
/**
 * �n����b�J�E���g���Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_UGTalkCount2Set( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_UG_TALK_COUNT2 , no );
}

//--------------------------------------------------------------------------------------------
/**
 * �����`���������Q�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	�J�E���g
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_RensyouCountGet( EVENTWORK* ev)
{
	u16		count;
	count = SysWork_Get( ev, SYS_WORK_RENSYOU_COUNT );
	return count;
}

//--------------------------------------------------------------------------------------------
/**
 * �����`���������Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_RensyouCountSet( EVENTWORK* ev, u16 no )
{
	OS_Printf("SYS_WORK_RENSYOU_COUNT <- [%d]\n",no);
	return SysWork_Set( ev, SYS_WORK_RENSYOU_COUNT , no );
}

//--------------------------------------------------------------------------------------------
/**
 *	�f�p�[�g�w����
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�ŏ��̃|�P�����i���o�["
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_RegularCountPlus( EVENTWORK* ev )
{
	u16 count;
	count = SysWork_Get( ev, SYS_WORK_DEPART_COUNT );
	if(count < 10000) {		//�����[�v���Ȃ��悤�Ƀ��~�b�g��
		count++;
	}else{
		count = 10000;
	}
	return SysWork_Set( ev, SYS_WORK_DEPART_COUNT, count );
}

//--------------------------------------------------------------------------------------------
/**
 *	�f�p�[�g�w����
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_RegularCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_DEPART_COUNT );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���œ�����������l���̗݌v
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_UGToolGiveCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_UG_TOOL_GIVE_COUNT	 );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���œ�����������l�����Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_UGToolGiveCountSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_UG_TOOL_GIVE_COUNT	 , no );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���ł��������@�����񐔂̗݌v
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_UGKasekiDigCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_UG_KASEKI_DIG_COUNT	 );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���ł��������@�����񐔂��Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_UGKasekiDigCountSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_UG_KASEKI_DIG_COUNT , no );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���Ńg���b�v�ɂ������񐔂̗݌v
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"�J�E���g"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_UGTrapHitCountGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_UG_TRAP_HIT_COUNT	 );
}

//--------------------------------------------------------------------------------------------
/**
 *	�n���Ńg���b�v�ɂ������񐔂��Z�b�g
 *
 * @param	ev		EVENTWORK�ւ̃|�C���^
 * @param	no		�Z�b�g�J�E���g
 *
 * @return	"TRUE=�Z�b�g�o�����AFALSE=�Z�b�g�o���Ȃ�����"
 */
//--------------------------------------------------------------------------------------------
BOOL SysWork_UGTrapHitCountSet( EVENTWORK* ev, u16 no )
{
	return SysWork_Set( ev, SYS_WORK_UG_TRAP_HIT_COUNT , no );
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
u16 SysWork_FriendlyStepCountGet(EVENTWORK * ev)
{
	return SysWork_Get(ev, SYS_WORK_FRIENDLY_COUNT);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void SysWork_FriendlyStepCountSet(EVENTWORK * ev, u16 step)
{
	SysWork_Set(ev, SYS_WORK_FRIENDLY_COUNT, step);
}

//--------------------------------------------------------------
/**
 * PL ���W�������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	seq	�����V�[�P���X
 * @retval	nothing
 */
//--------------------------------------------------------------
void SysWork_PLGrassGymTimeSeqSet( EVENTWORK *ev, u16 seq )
{
	SysWork_Set( ev, SYS_WORK_PL_GRASS_GYM_TIME, seq );
}

//--------------------------------------------------------------
/**
 * PL ���W�������擾
 * @param	ev	EVENTWORK *
 * @retval	u16 �����V�[�P���X
 */
//--------------------------------------------------------------
u16 SysWork_PLGrassGymTimeSeqGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_PL_GRASS_GYM_TIME) );
}

//--------------------------------------------------------------
/**
 * ���C�h�p���������^�[���Z�b�g
 *
 * @param	ev		EVENTWORK *
 * @param	turn	�^�[����
 *
 * @retval	nothing
 */
//--------------------------------------------------------------
void SysWork_MaidWinTurnSet( EVENTWORK *ev, u16 turn )
{
	//�^�[������0�I���W���H�̂悤�Ȃ̂�+1���Ă���
	OS_Printf( "btl turn = %d\n", (turn+1) );
	SysWork_Set( ev, SYS_WORK_MAID_WIN_TURN, (turn+1) );
}

//--------------------------------------------------------------
/**
 * ���C�h�p���������^�[���擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�^�[����"
 */
//--------------------------------------------------------------
u16 SysWork_MaidWinTurnGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MAID_WIN_TURN) );
}

//============================================================================================
//
//	�ʑ�
//
//============================================================================================
#include "mapdefine.h"				//ZONE_ID_C01
//#include "zonedata.h"				//ZoneData_GetBGMID
#include "field/situation_local.h"

u16 SysWork_EventT07R0201Get( EVENTWORK *ev );
void SysWork_T07R0201Set( EVENTWORK *ev, u16 no );
void SysWorkUpdateEventT07R0201( SAVEDATA *sv );
static u8 GetEventNoT07R0201( EVENTWORK* ev );

//�����N�ɂ���ĕK�v�ȉƋ�̐�
#define FURNITURE_RANK_2	(8)			//���i�K
#define FURNITURE_RANK_3	(12)		//��O�i�K

//�C�x���g���N����m��
static const u8 t07r0201_event_rand[] = { 25, 75, 90 };

//�����N�ɂ���ċN����C�x���g�̑���
static const u8 t07r0201_event_num[] = { 4, 12, 15 };

//--------------------------------------------------------------
/**
 * @brief	��������C�x���g������(0xff�͔������Ȃ�)
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
static u8 GetEventNoT07R0201( EVENTWORK* ev )
{
	int i;
	u16 count,r,pos;

	//�Ƌ�̐����擾
	count = 0;
	for( i=0; i < VILLA_FTURE_MAX ;i++ ){
		if( SysFlag_VillaFurniture(ev,SYSFLAG_MODE_CHECK,i) == TRUE ){
			count++;
		}
	}

	if( count >= FURNITURE_RANK_3 ){
		pos = 2;
	}else if( count >= FURNITURE_RANK_2 ){
		pos = 1;
	}else{
		pos = 0;
	}

	//�C�x���g�������邩
	r = ( gf_rand() % 100 );
	if( r > t07r0201_event_rand[pos] ){
		return 0xff;
	}

	r = ( gf_rand() % t07r0201_event_num[pos] );
	OS_Printf( "�ʑ��C�x���g�i���o�[ = %d\n", r );
	return r;
}

//--------------------------------------------------------------------------------------------
/**
 * �P���P��A�ʑ��̃C�x���g����
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void SysWorkUpdateEventT07R0201( SAVEDATA *sv )
{
	u32 value;
	EVENTWORK* ev			= SaveData_GetEventWork(sv);
	SITUATION* sit			= SaveData_GetSituation(sv);
	LOCATION_WORK* now		= Situation_GetNowLocation(sit);

	//�X�V�o����]�[�����`�F�b�N
	if( (now->zone_id != ZONE_ID_T07) && (now->zone_id != ZONE_ID_T07R0201) ){
		SysFlag_T07ObjInReset( ev );				//�ʑ��O��OBJ�����ɓ������t���Ooff
		SysFlag_T07R0201PosAppearReset( ev );		//POS��OBJ���\�����ꂽ�t���Ooff
		SysWork_T07R0201Set( ev, GetEventNoT07R0201(ev) );
	}
	return;
}

#define T07R0201_TALK_RAND_MAX		(5)				//�����_����b�̎��
//--------------------------------------------------------------
/**
 * �ʑ��C�x���g�i���o�[�Z�b�g
 *
 * @param	ev		EVENTWORK *
 * @param	no		�C�x���g�i���o�[
 *
 * @retval	nothing
 */
//--------------------------------------------------------------
void SysWork_T07R0201Set( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_EVENT_T07R0201, no );

	//��b�i���o�[���Z�b�g
	SysWork_Set( ev, SYS_WORK_T07R0201_TALK, (gf_rand() % T07R0201_TALK_RAND_MAX) );
}

//--------------------------------------------------------------
/**
 * �ʑ��C�x���g�i���o�[�擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_EventT07R0201Get( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_EVENT_T07R0201) );
}

//--------------------------------------------------------------
/**
 * �L�O�v�����g���[�N�̏�Ԃ��擾(�t�@�N�g���[)
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_MemoryPrintFactory( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MEMORY_PRINT_FACTORY) );
}

//--------------------------------------------------------------
/**
 * �L�O�v�����g���[�N�̏�Ԃ��擾(�X�e�[�W)
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_MemoryPrintStage( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MEMORY_PRINT_STAGE) );
}

//--------------------------------------------------------------
/**
 * �L�O�v�����g���[�N�̏�Ԃ��擾(�L���b�X��)
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_MemoryPrintCastle( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MEMORY_PRINT_CASTLE) );
}

//--------------------------------------------------------------
/**
 * �L�O�v�����g���[�N�̏�Ԃ��擾(���[���b�g)
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_MemoryPrintRoulette( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MEMORY_PRINT_ROULETTE) );
}

//--------------------------------------------------------------
/**
 * �L�O�v�����g���[�N�̏�Ԃ��擾(�^���[)
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_MemoryPrintTower( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MEMORY_PRINT_TOWER) );
}

//--------------------------------------------------------------
/**
 * �j�ꂽ���E�@�C�x���g�i�s�󋵂��擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_TornWorldEventSeqNoGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_TW_EVSEQ) );
}

//--------------------------------------------------------------
/**
 * �j�ꂽ���E�@�C�x���g�i�s�󋵂��Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_TornWorldEventSeqNoSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_TW_EVSEQ, no );
}

//--------------------------------------------------------------
/**
 * �A���Z�E�X�@�C�x���g�i�s�󋵂��擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_AruseusuEventGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_ARUSEUSU) );
}

//--------------------------------------------------------------
/**
 * �A���Z�E�X�@�C�x���g�i�s�󋵂��Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_AruseusuEventSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_ARUSEUSU, no );
}

//--------------------------------------------------------------
/**
 * �V�F�C�~�@�C�x���g�i�s�󋵂��擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
u16 SysWork_SyeimiEventGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_SYEIMI) );
}

//--------------------------------------------------------------
/**
 * �V�F�C�~�@�C�x���g�i�s�󋵂��Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_SyeimiEventSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_SYEIMI, no );
}

//--------------------------------------------------------------
/**
 * ���[�o�X�@�ߊl���������擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�ߊl������"
 */
//--------------------------------------------------------------
u16 SysWork_MuubasuGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_MUUBASU_GET) );
}

//--------------------------------------------------------------
/**
 * ���[�o�X�@�ߊl���������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_MuubasuSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_MUUBASU_GET, no );
}

//--------------------------------------------------------------
/**
 * �A�C�@�ߊl���������擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�ߊl������"
 */
//--------------------------------------------------------------
u16 SysWork_AiGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_AI_GET) );
}

//--------------------------------------------------------------
/**
 * �A�C�@�ߊl���������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_AiSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_AI_GET, no );
}

//--------------------------------------------------------------
/**
 * �t�@�C���[�@�ߊl���������擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�ߊl������"
 */
//--------------------------------------------------------------
u16 SysWork_FaiyaaGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_FAIYAA_GET) );
}

//--------------------------------------------------------------
/**
 * �t�@�C���[�@�ߊl���������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_FaiyaaSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_FAIYAA_GET, no );
}

//--------------------------------------------------------------
/**
 * �T���_�[�@�ߊl���������擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�ߊl������"
 */
//--------------------------------------------------------------
u16 SysWork_SandaaGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_SANDAA_GET) );
}

//--------------------------------------------------------------
/**
 * �T���_�[�@�ߊl���������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_SandaaSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_SANDAA_GET, no );
}

//--------------------------------------------------------------
/**
 * �t���[�U�[�@�ߊl���������擾
 *
 * @param	ev	EVENTWORK *
 *
 * @retval	"�ߊl������"
 */
//--------------------------------------------------------------
u16 SysWork_HuriizaaGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_HURIIZAA_GET) );
}

//--------------------------------------------------------------
/**
 * �t���[�U�[�@�ߊl���������Z�b�g
 * @param	ev	EVENTWORK *
 * @param	no	"�C�x���g�i���o�["
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_HuriizaaSet( EVENTWORK *ev, u16 no )
{
	SysWork_Set( ev, SYS_WORK_HURIIZAA_GET, no );
}

//--------------------------------------------------------------
/**
 * �ړ��|�P�����@���[�N�Z�b�g
 *
 * @param	ev	EVENTWORK *
 * @param	monsno	�����X�^�[�i���o�[
 * @param	num		���[�N�ɃZ�b�g����l
 *
 * @retval	non
 *
 * num = 2 �|����
 * num = 1 �ߊl����
 */
//--------------------------------------------------------------
void SysWork_MovePokeWorkSet( EVENTWORK* ev, u16 monsno, u16 num )
{
	switch( monsno ){

	case MONSNO_AI:
		SysWork_AiSet( ev, num );
		break;

	case MONSNO_MUUBASU:
		SysWork_MuubasuSet( ev, num );
		break;

	case MONSNO_FAIYAA:
		SysWork_FaiyaaSet( ev, num );
		break;

	case MONSNO_SANDAA:
		SysWork_SandaaSet( ev, num );
		break;

	case MONSNO_HURIIZAA:
		SysWork_HuriizaaSet( ev, num );
		break;
	};

	return;
}

//--------------------------------------------------------------
/**
 * �j�ꂽ���E �A�J�M�o���t���O�擾
 * @param	ev	EVENTWORK *
 * @retval	u16	�t���O���e
 */
//--------------------------------------------------------------
u16 SysWork_TwAkagiAppearFlagGet( EVENTWORK *ev )
{
	return( SysWork_Get(ev,SYS_WORK_TW_AKAGI_APPEAR) );
}

//--------------------------------------------------------------
/**
 * �j�ꂽ���E �A�J�M�o���t���O�Z�b�g
 * @param	ev	EVENTWORK *
 * @param	set	�Z�b�g�ԍ�
 * @retval	non
 */
//--------------------------------------------------------------
void SysWork_TwAkagiAppearFlagSet( EVENTWORK *ev, u16 set )
{
	SysWork_Set( ev, SYS_WORK_TW_AKAGI_APPEAR, set );
}

//--------------------------------------------------------------------------------------------
/**
 * WIFI�t�����e�B�A��TEMP�N���A�t���O�擾
 *
 * @param	ev		EVENTWORK�̃|�C���^
 *
 * @return	"0=�N���A���Ă��Ȃ��A1=�N���A���Ă���"
 */
//--------------------------------------------------------------------------------------------
u16 SysWork_WifiFrClearFlagGet( EVENTWORK* ev )
{
	return SysWork_Get( ev, SYS_WORK_WIFI_FR_CLEAR_FLAG );
}


