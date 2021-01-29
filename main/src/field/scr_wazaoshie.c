//============================================================================================
/**
 * @file	scr_wazaoshie.c
 * @bfief	�X�N���v�g�R�}���h�F�Z�����֘A(��풆�t�B�[���h)
 * @author	Satoshi Nohara
 * @date	07.12.17
 *
 * ���X�g�����̎������̂�scr_field.c�ɂ����݂��Ă���
 */
//============================================================================================
#include "common.h"
#include "system/lib_pack.h"
#include "fieldsys.h"

#include "script.h"
#include "scrcmd_def.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "sysflag.h"
#include "syswork.h"
#include "scrcmd.h"
#include "ev_pokemon.h"
#include "itemtool/myitem.h"
#include "itemtool/itemsym.h"

#include "battle/wazano_def.h"
#include "battle/battle_server.h"		//�t�H������`

#include "system/wipe.h"
#include "system/fontproc.h"
#include "system/window.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "scr_wazaoshie.h"
#include "msgdata\msg.naix"
#include "msgdata\msg_ev_win.h"
#include "ev_win.h"
#include "fieldmap.h"
#include "fld_bmp.h"


//==============================================================================================
//
//	�Z�����\����
//
//==============================================================================================
#define WAZA_NUM_MAX			(5)
typedef struct{
	u8 waza_1[WAZA_NUM_MAX];	//�r�b�g�P�ʂŋZ�������邩�������Ă���
}WAZA_OSHIE_DATA;
#include "../fielddata/wazaoshie/waza_oshie.dat"

#if 0
	�t�H�����Ⴂ�ꗗ(07.12.17 ���삳����)	

	�|�P������	�p�^�[��	�p�[�\�i��
	�A���m�[��	�Q�W��		�~			
	�f�I�L�V�X	�S��		��			
	�G�E���X	�Q��		��			
	�v���Y�}	�U��		��			
	�L�}�C����	�Q��		��
	�~�m���X	�R��		��
	�~�m���b�`	�R��		�~
	�V�[�E�V	�Q��		�~
	�V�[�h���S	�Q��		�~
	�|������	�S��		�~
	�`�F���V��	�Q��		�~
	�A�E�X		�P�V��		�~

	�p�[�\�i��������������̂̂݁A�Z�o���̑Ή�����������

	�f�[�^���X�g�̈�ԉ��ɁA�ʃt�H����(�Ƃ����Ă����̂���������)�͂܂Ƃ܂��Ă���
#endif

typedef struct{
	u16 waza;
	u8 aka;
	u8 ao;
	u8 ki;
	u8 midori;
	u32 color;				//���Z�A�ԁ��́A�����S
}WAZA_OSHIE_LIST;

//�����Ă����Z�̃��X�g(���|�P�����ʋZ�K�����X�g�ɂ��킹��)
static const WAZA_OSHIE_LIST waza_oshie_tbl[] = {
	{ WAZANO_DAIBINGU,		2,	4,	2,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_DOROKAKE,		4,	4,	0,	0, WAZA_OSHIE_COL_RED },
	{ WAZANO_RENZOKUGIRI,	0,	8,	0,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_KOGOERUKAZE,	0,	6,	0,	2, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_KOROGARU,		4,	2,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_KAMINARIPANTI,	2,	6,	0,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_HONOONOPANTI,	2,	6,	0,	0, WAZA_OSHIE_COL_BLUE },

	{ WAZANO_BAKADIKARA,	8,	0,	0,	0, WAZA_OSHIE_COL_RED },

	{ WAZANO_REITOUPANTI,	2,	6,	0,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_AIANHEDDO,		6,	0,	2,	0, WAZA_OSHIE_COL_RED },
	{ WAZANO_AKUATEERU,		6,	0,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_AYASIIKAZE,	0,	6,	0,	2, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_IEKI,			4,	0,	2,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_IBIKI,			2,	0,	4,	2, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_URAMI,			0,	0,	8,	0, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_EAKATTAA,		2,	4,	0,	2, WAZA_OSHIE_COL_BLUE },

	//{ WAZANO_GAMAN,		0,	0,	6,	2, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_TEDASUKE,		2,	0,	4,	2, WAZA_OSHIE_COL_YELLOW },

	{ WAZANO_GAMUSYARA,		4,	0,	4,	0, WAZA_OSHIE_COL_RED },
	{ WAZANO_GEKIRIN,		6,	0,	2,	0, WAZA_OSHIE_COL_RED },
	{ WAZANO_GENSINOTIKARA,	6,	0,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_KOUGOUSEI,		0,	0,	2,	6, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_SIGUNARUBIIMU,	2,	2,	2,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_SINENNOZUTUKI,	0,	4,	4,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_SINKUUHA,		2,	4,	0,	2, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_DAITINOTIKARA,	6,	0,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_DASUTOSYUUTO,	4,	2,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_TATUMAKI,		6,	0,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_TANEBAKUDAN,	4,	0,	0,	4, WAZA_OSHIE_COL_RED },
	{ WAZANO_TEPPEKI,		4,	2,	2,	0, WAZA_OSHIE_COL_RED },
	{ WAZANO_DENZIHUYUU,	0,	2,	4,	2, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_TOTTEOKI,		0,	0,	0,	8, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_TOBIHANERU,	4,	0,	2,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_TORIKKU,		0,	4,	4,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_NEPPUU,		4,	2,	0,	2, WAZA_OSHIE_COL_RED },
	{ WAZANO_HATAKIOTOSU,	4,	4,	0,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_HUIUTI,		0,	6,	2,	0, WAZA_OSHIE_COL_BLUE },
	{ WAZANO_SUPIIDOSUTAA,	0,	2,	2,	4, WAZA_OSHIE_COL_YELLOW },
	{ WAZANO_SAWAGU,		0,	0,	6,	2, WAZA_OSHIE_COL_YELLOW },
};
#define WAZA_OSHIE_TBL_MAX		( NELEMS(waza_oshie_tbl) )


//============================================================================================
//
//	�v�����g�^�C�v�錾
//
//============================================================================================
BOOL EvCmdWazaOshieDataCount(VM_MACHINE * core );
BOOL EvCmdWazaOshieChgPokeWaza( VM_MACHINE * core );
//BOOL EvCmdWazaOshiePokeStatusSetProc(VM_MACHINE * core);
//BOOL EvCmdWazaOshiePokeStatusGetResult(VM_MACHINE * core);
BOOL EvCmdWazaOshieItemCheck( VM_MACHINE * core );
BOOL EvCmdWazaOshieItemSub( VM_MACHINE * core );
BOOL EvCmdWazaOshieBmpListStart( VM_MACHINE * core );
static BOOL EvWazaOshieSelWinWait(VM_MACHINE * core);

static u16 GetWazaOshieWazaNo( u16 index );
static u16 GetWazaOshieWazaNoByWazaNo( u16 waza );
static u16 GetWazaOshieMax( void );
static u8 GetWazaOshieDataAdrs( POKEMON_PARAM* poke, u8 no );
static u16 GetWazaOshieDataCount( POKEMON_PARAM* poke, u16 color );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �o������Z�����邩
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieDataCount( VM_MACHINE * core )
{
	POKEMON_PARAM* poke;
	u16 pos		= VMGetWorkValue(core);
	u16 color	= VMGetWorkValue(core);
	u16* ret_wk	= VMGetWork( core );

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(core->fsys->savedata), pos );

	*ret_wk = GetWazaOshieDataCount( poke, color );
	return 0;
}

#if 0
//--------------------------------------------------------------------------------------------
/**
 * @brief	�|�P�����X�e�[�^�X��ʌĂяo��
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	1		�X�N���v�g���烁�C������ɖ߂�
 *
 * @li EvCmdPokeStatusGetResult�ŁA�I�������ʒu���擾���āA���[�N�J�����K�v
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshiePokeStatusSetProc(VM_MACHINE * core)
{
	void** buf	= GetEvScriptWorkMemberAdrs(core->fsys, ID_EVSCR_SUBPROC_WORK);
	u16 pos		= VMGetWorkValue(core);
	u16 waza	= VMGetWorkValue(core);

	//*buf = WazaOshiePokeStatusEvent_SetProc( HEAPID_EVENT, core->fsys, pos, waza );
	*buf = WazaOshiePokeStatusEvent_SetProc( HEAPID_WORLD, core->fsys, pos, waza );

	VM_SetWait( core, EvCmdWaitSubProcEnd );
	return 1;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�|�P�����X�e�[�^�X��ʌĂяo����̌��ʎ擾�ƃ��[�N���
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @return	1		�X�N���v�g���烁�C������ɖ߂�
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshiePokeStatusGetResult(VM_MACHINE * core)
{
	void ** buf;
	u16 * ret_wk;
	PSTATUS_DATA* psd;

	ret_wk = VMGetWork(core);
	buf = GetEvScriptWorkMemberAdrs(core->fsys, ID_EVSCR_SUBPROC_WORK);

	GF_ASSERT(*buf != 0);

	psd = *buf;
	*ret_wk = psd->ret_sel;				//�I�����ꂽ�Z�ʒu
	OS_Printf( "*ret_wk = %d\n", *ret_wk );
//	if (*ret_wk == PL_SEL_POS_EXIT) {
//		*ret_wk = 0xff;
//	}

	sys_FreeMemoryEz(*buf);
	*buf = NULL;

	return 0;
}
#endif

//--------------------------------------------------------------------------------------------
/**
 * �莝���|�P�����̋Z��u��������(���g���ł��g�p)
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieChgPokeWaza( VM_MACHINE * core )
{
	u16 poke_pos= VMGetWorkValue(core);
	u16 waza_pos= VMGetWorkValue(core);
	u16 waza_no	= VMGetWorkValue(core);

	EvPoke_ChangeWaza(	SaveData_GetTemotiPokemon(core->fsys->savedata),
						poke_pos, waza_pos, waza_no );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �w�肵���Z���o����A�C�e�������邩�`�F�b�N
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieItemCheck( VM_MACHINE * core )
{
	int i;
	u8 aka,ao,ki,midori;
	MYITEM* my_item;
	u16 waza	= VMGetWorkValue(core);
	u16* ret_wk	= VMGetWork( core );

	OS_Printf( "waza = %d\n", waza );
	my_item = SaveData_GetMyItem( core->fsys->savedata );

	for( i=0; i < WAZA_OSHIE_TBL_MAX ;i++ ){
		if( waza == waza_oshie_tbl[i].waza ){
			aka		= waza_oshie_tbl[i].aka;
			ao		= waza_oshie_tbl[i].ao;
			ki		= waza_oshie_tbl[i].ki;
			midori	= waza_oshie_tbl[i].midori;
			break;
		}
	}

	if( i == WAZA_OSHIE_TBL_MAX ){
		OS_Printf( "�Z�i���o�[���s���ł��I\n" );
		GF_ASSERT( 0 );
		*ret_wk = 0;
		return 0;
	}

	*ret_wk = 1;

	//�A�C�e���̌��`�F�b�N
	if( MyItem_CheckItem(my_item,ITEM_AKAIKAKERA,aka,HEAPID_EVENT) == FALSE ){
		*ret_wk = 0;
	}
	if( MyItem_CheckItem(my_item,ITEM_AOIKAKERA,ao,HEAPID_EVENT) == FALSE ){
		*ret_wk = 0;
	}
	if( MyItem_CheckItem(my_item,ITEM_KIIROIKAKERA,ki,HEAPID_EVENT) == FALSE ){
		*ret_wk = 0;
	}
	if( MyItem_CheckItem(my_item,ITEM_MIDORINOKAKERA,midori,HEAPID_EVENT) == FALSE ){
		*ret_wk = 0;
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �w�肵���Z���o����A�C�e�������炷
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieItemSub( VM_MACHINE * core )
{
	int i;
	u8 aka,ao,ki,midori;
	MYITEM* my_item;
	u16 waza	= VMGetWorkValue(core);

	OS_Printf( "waza = %d\n", waza );
	my_item = SaveData_GetMyItem( core->fsys->savedata );

	for( i=0; i < WAZA_OSHIE_TBL_MAX ;i++ ){
		if( waza == waza_oshie_tbl[i].waza ){
			aka		= waza_oshie_tbl[i].aka;
			ao		= waza_oshie_tbl[i].ao;
			ki		= waza_oshie_tbl[i].ki;
			midori	= waza_oshie_tbl[i].midori;
			break;
		}
	}

	if( i == WAZA_OSHIE_TBL_MAX ){
		OS_Printf( "�Z�i���o�[���s���ł��I\n" );
		GF_ASSERT( 0 );
	}

	//�A�C�e�����炷
	MyItem_SubItem( my_item, ITEM_AKAIKAKERA, aka, HEAPID_EVENT );
	MyItem_SubItem( my_item, ITEM_AOIKAKERA, ao, HEAPID_EVENT );
	MyItem_SubItem( my_item, ITEM_KIIROIKAKERA, ki, HEAPID_EVENT );
	MyItem_SubItem( my_item, ITEM_MIDORINOKAKERA, midori, HEAPID_EVENT );
	return 0;
}


//==============================================================================================
//
//	static
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * �n���ꂽ�C���f�b�N�X����Z�����̋Z�i���o�[���擾
 *
 * @param   index	���X�g�̉��Ԗڂ�
 *
 * @retval  "�Z�i���o�["
 */
//--------------------------------------------------------------
static u16 GetWazaOshieWazaNo( u16 index )
{
	return waza_oshie_tbl[index].waza;
}

//--------------------------------------------------------------
/**
 * �n���ꂽ�Z�i���o�[����Z�����̋Z�i���o�[���擾
 *
 * @param   waza	�Z�i���o�[
 *
 * @retval  "�Z�i���o�["
 */
//--------------------------------------------------------------
static u16 GetWazaOshieWazaNoByWazaNo( u16 waza )
{
	int i;

	for( i=0; i < WAZA_OSHIE_TBL_MAX ;i++ ){
		if( waza_oshie_tbl[i].waza == waza ){
			return i;
		}
	}

	GF_ASSERT( (0) && "�Z�i���o�[���s���ł��I" );
	return 0;
}

//--------------------------------------------------------------
/**
 * �Z�����̃��X�g�̍ő吔���擾
 *
 * @param   none
 *
 * @retval  "�ő吔"
 */
//--------------------------------------------------------------
static u16 GetWazaOshieMax( void )
{
	return WAZA_OSHIE_TBL_MAX;
}

//���G�N�Z���f�[�^�̕��т����ꂽ��Ή����Ȃ��ƃ_���I(�Q�Ƃ��鎞��-1���Ă���)
enum{
	WAZAOSHIE_DEO_A		= 494,
	WAZAOSHIE_DEO_D		= 495,
	WAZAOSHIE_DEO_S		= 496,

	WAZAOSHIE_MINO_G	= 497,
	WAZAOSHIE_MINO_M	= 498,

	WAZAOSHIE_KIMA		= 499,

	WAZAOSHIE_EU		= 500,

	WAZAOSHIE_PURA_1	= 501,
	WAZAOSHIE_PURA_2	= 502,
	WAZAOSHIE_PURA_3	= 503,
	WAZAOSHIE_PURA_4	= 504,
	WAZAOSHIE_PURA_5	= 505,
};

//--------------------------------------------------------------
/**
 * �G�N�Z�����R���o�[�g�����f�[�^���X�g����A�f�[�^�̃A�h���X���擾
 *
 * @param   monsno	�擾�����������X�^�[�i���o�[
 *
 * @retval  "�f�[�^"
 */
//--------------------------------------------------------------
static u8 GetWazaOshieDataAdrs( POKEMON_PARAM* poke, u8 no )
{
	u16 index;
	u32 form,monsno;

	monsno	= PokeParaGet( poke, ID_PARA_monsno, NULL );
	form	= PokeParaGet( poke, ID_PARA_form_no, NULL );
	index	= monsno;

	//�t�H�����`�F�b�N���ĕK�v�ȃC���f�b�N�X���擾
		
	switch( monsno ){

	//�f�I�L�V�X
	case MONSNO_DEOKISISU:
		if( form == FORMNO_DEOKISISU_ATTACK ){
			index = WAZAOSHIE_DEO_A;
		}else if( form == FORMNO_DEOKISISU_DEFENCE ){
			index = WAZAOSHIE_DEO_D;
		}else if( form == FORMNO_DEOKISISU_SPEED ){
			index = WAZAOSHIE_DEO_S;
		}
		break;

	//�~�m���X
	case MONSNO_MINOMESU:
		if( form == FORMNO_MINOMUTTI_SUNA ){
			index = WAZAOSHIE_MINO_G;
		}else if( form == FORMNO_MINOMUTTI_TETSU ){
			index = WAZAOSHIE_MINO_M;
		}
		break;

	//�L�}�C����
	case MONSNO_KIMAIRAN:
		if( form == FORMNO_GIRATINA_ORIGIN ){
			index = WAZAOSHIE_KIMA;
		}
		break;

	//�G�E���X
	case MONSNO_EURISU:
		if( form == FORMNO_SHEIMI_FLOWER ){
			index = WAZAOSHIE_EU;
		}
		break;

	//�v���Y�}
	case MONSNO_PURAZUMA:
		if( form == FORMNO_ROTOMU_HOT ){
			index = WAZAOSHIE_PURA_1;
		}else if( form == FORMNO_ROTOMU_WASH ){
			index = WAZAOSHIE_PURA_2;
		}else if( form == FORMNO_ROTOMU_COLD ){
			index = WAZAOSHIE_PURA_3;
		}else if( form == FORMNO_ROTOMU_FAN ){
			index = WAZAOSHIE_PURA_4;
		}else if( form == FORMNO_ROTOMU_CUT ){
			index = WAZAOSHIE_PURA_5;
		}
		break;

	};

	//return &waza_oshie_data[index-1].waza_1;
	return waza_oshie_data[index-1].waza_1[no];
}

//--------------------------------------------------------------
/**
 * �Z�����Ŋo������Z�����邩�`�F�b�N
 *
 * @param   monsno	�����X�^�[�i���o�[
 *
 * @retval  "TRUE = ����AFALSE = �Ȃ�"
 */
//--------------------------------------------------------------
static u16 GetWazaOshieDataCount( POKEMON_PARAM* poke, u16 color )
{
	int i,bit,waza_i;
	u8 data,num;
	u32 monsno;
	u16 poke_waza[4];

	monsno = PokeParaGet( poke, ID_PARA_monsno, NULL );

	//�|�P�������o���Ă�Z���擾
	for( waza_i=0; waza_i < 4 ;waza_i++ ){
		poke_waza[waza_i] = PokeParaGet( poke, (ID_PARA_waza1 + waza_i), NULL );
	}

	//�f�[�^�擾

	for( i=0; i < WAZA_NUM_MAX ;i++ ){
		data = GetWazaOshieDataAdrs( poke, i );
		OS_Printf( "data = %d\n", data );
		for( bit=0; bit < 8 ;bit++ ){

			num = ( (data >> bit) & 0x01 );

			//�o������r�b�g�������Ă��āA�n���ꂽ�J���[�Ɠ������`�F�b�N
			if( (num == 1) && (color == waza_oshie_tbl[i*8+bit].color) ){

				//���łɃ|�P�������o���Ă�Z�͏��O
				for( waza_i=0; waza_i < 4 ;waza_i++ ){
					if( poke_waza[waza_i] == waza_oshie_tbl[i*8+bit].waza ){
						break;
					}
				}

				if( waza_i == 4 ){
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


//==============================================================================================
//
//	ev_win�̃��X�g���������ɂ��Ă���
//
//==============================================================================================
//�L���ŁA�\������荀�ڐ����������X�g�����́A�㉺�ɃX�N���[���J�[�\����\��
//�O���t�B�b�N�f�[�^���Ȃ��̂ŁA���̕ӂ͉��ł��B
//#define EV_WIN_SCROLL_CURSOR

#ifdef EV_WIN_SCROLL_CURSOR	//070228
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "field_clact.h"
#include "field/ranking.naix"			//�O���t�B�b�N�f�[�^���Ȃ��̂ŉ�
#endif


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
static void EvWin_MsgManSet( WAZA_OSHIE_WORK* wk, MSGDATA_MANAGER* msgman );
static void EvWin_Init( FIELDSYS_WORK* fsys, WAZA_OSHIE_WORK* wk, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman );

//BMP���X�g
WAZA_OSHIE_WORK * CmdWazaOshieBmpList_Init( FIELDSYS_WORK* fsys, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman );
void CmdWazaOshieBmpList_MakeList( WAZA_OSHIE_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static void CmdWazaOshieBmpList_Start( WAZA_OSHIE_WORK* wk );

static void BmpList_list_make( WAZA_OSHIE_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static u32 BmpList_length_get( WAZA_OSHIE_WORK* wk );
static void BmpList_h_default_set(WAZA_OSHIE_WORK* wk);
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode );
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk );
static void EvBmpList_Del( WAZA_OSHIE_WORK* wk );

#ifdef EV_WIN_SCROLL_CURSOR	//070228
static void EvWin_ActorInit( WAZA_OSHIE_WORK* wk );
static void EvWin_ActorRelease( WAZA_OSHIE_WORK* wk );
#endif


//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�������A���X�g�쐬�A�J�n
 *
 * @param	none
 *
 * @retval	1
 *
 * pos = 0xff�̎��́A�{�[�h�p�ɑS�Ă̋Z���Z�b�g
 */
//--------------------------------------------------------------
BOOL EvCmdWazaOshieBmpListStart( VM_MACHINE * core )
{
	u8 data,num;
	int i,set_pos,bit,waza_i;
	POKEMON_PARAM* poke;
	MSGDATA_MANAGER* man;
	MSGDATA_MANAGER* ev_win_man;
	FIELDSYS_WORK* fsys	= core->fsys;
	WAZA_OSHIE_WORK* waza_oshie_win;
	u16 poke_waza[4];
	u16 waza[WAZA_OSHIE_TBL_MAX];
	WORDSET** wordset			= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_WORDSET );
	u16 pos						= VMGetWorkValue( core );
	u16 color					= VMGetWorkValue( core );
	u16 wk_id					= VMGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	//�|�P�����ւ̃|�C���^�擾
	if( pos != 0xff ){
		poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(core->fsys->savedata), pos );
	}

	//�Z��
	man = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wazaname_dat, HEAPID_EVENT );

	//evwin������
	waza_oshie_win	= CmdWazaOshieBmpList_Init(	fsys, 20, 1, 0, 1, 
									GetEventWorkAdrs(fsys,wk_id), *wordset, 
									GetEvScriptWorkMemberAdrs(core->fsys,ID_EVSCR_MSGWINDAT),
									man );

	//�N���A
	for( i=0; i < WAZA_OSHIE_TBL_MAX ;i++ ){
		waza[i] = 0;
	}

	set_pos = 0;

	//�o������Z���Z�b�g
	if( pos != 0xff ){

		//�|�P�������o���Ă�Z���擾
		for( waza_i=0; waza_i < 4 ;waza_i++ ){
			poke_waza[waza_i] = PokeParaGet( poke, (ID_PARA_waza1 + waza_i), NULL );
		}

		for( i=0; i < WAZA_NUM_MAX ;i++ ){
			data = GetWazaOshieDataAdrs( poke, i );
			OS_Printf( "data = %d\n", data );
			for( bit=0; bit < 8 ;bit++ ){
				//OS_Printf( "�v�f = %d\n", i*8+bit );
				//OS_Printf( "wazano = %d\n", waza_oshie_tbl[i*8+bit] );
				//OS_Printf( "data = %d\n", data );
				num = ( (data >> bit) & 0x01 );

				//�o������r�b�g�������Ă��āA�n���ꂽ�J���[�Ɠ������`�F�b�N
				if( (num == 1) && (color == waza_oshie_tbl[i*8+bit].color) ){
				//if( num == 1 ){
				
					//���łɃ|�P�������o���Ă�Z�͏��O
					for( waza_i=0; waza_i < 4 ;waza_i++ ){
						if( poke_waza[waza_i] == waza_oshie_tbl[i*8+bit].waza ){
							break;
						}
					}

					if( waza_i == 4 ){
						waza[set_pos] = waza_oshie_tbl[i*8+bit].waza;
						set_pos++;
					}
				}
			}
		}
	}else{
		//�{�[�h�p�ɑS�ăZ�b�g
		for( i=0; i < WAZA_OSHIE_TBL_MAX ;i++ ){
			//waza[i] = GetWazaOshieWazaNo( i );
			//�n���ꂽ�J���[�Ɠ������`�F�b�N
			if( color == waza_oshie_tbl[i].color ){
				waza[set_pos] = GetWazaOshieWazaNo( i );
				set_pos++;
			}
		}
		//set_pos = WAZA_OSHIE_TBL_MAX;
	}

	//���X�g�쐬
	for( i=0; i < set_pos ;i++ ){
		CmdWazaOshieBmpList_MakeList( waza_oshie_win, waza[i], EV_WIN_TALK_MSG_NONE, waza[i] );
	}

#if 1
	//ev_win.gmm����u��߂�v���g�p���Ă���
	
	ev_win_man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_ev_win_dat, HEAPID_EVENT );
	EvWin_MsgManSet( waza_oshie_win, ev_win_man );
	CmdWazaOshieBmpList_MakeList(	waza_oshie_win, msg_ev_win_006, 
									EV_WIN_TALK_MSG_NONE, EV_WIN_B_CANCEL );
	MSGMAN_Delete( ev_win_man );

	//�ꉞ�߂�
	EvWin_MsgManSet( waza_oshie_win, man );
#endif

	//���X�g�J�n
	CmdWazaOshieBmpList_Start( waza_oshie_win );
	
	VM_SetWait( core, EvWazaOshieSelWinWait );

	MSGMAN_Delete( man );
	return 1;
}

//�E�F�C�g�֐�
static BOOL EvWazaOshieSelWinWait(VM_MACHINE * core)
{
	FIELDSYS_WORK* fsys = core->fsys;
	u16* ret_wk = GetEventWorkAdrs( fsys, core->reg[0] );	//���ӁI

	if( *ret_wk == EV_WIN_NOTHING ){
		return FALSE;	//�p��
	}

	return TRUE;		//�I��
}


//==============================================================================================
//
//	��`
//
//==============================================================================================
#define EV_WIN_FONT				(FONT_SYSTEM)	//�t�H���g�w��

#define EVWIN_MSG_BUF_SIZE		(40*2)			//���b�Z�[�W�o�b�t�@�T�C�Y

#define EV_WIN_LIST_MAX			(WAZA_OSHIE_TBL_MAX+1)			//BMP���X�g���ڂ̍ő吔
#define	EV_WIN_MSG_MAX			(WAZA_OSHIE_TBL_MAX+1)			//MSG�o�b�t�@�̍ő吔

//BMP���X�g�w�b�_�[��`
#define EV_LIST_LINE			(8)				//�\���ő區�ڐ�
#define EV_LIST_RABEL_X			(1)				//���x���\���w���W
#define EV_LIST_DATA_X			(12)			//���ڕ\���w���W
#define EV_LIST_CURSOR_X		(2)				//�J�[�\���\���w���W
#define EV_LIST_LINE_Y			(1)				//�\���x���W

//�J�[�\����
#define EV_WIN_CURSOR_WIDTH		(12)			//�J�[�\����

#define EV_WIN_DEFAULT_WAIT		(3)				//�L�[���삪�����ɓ���Ȃ��悤�Ɋ�{�E�F�C�g

// ����������g���ĉ������I�I
#define	EVWIN_FREE_CGX			( 1 )			//�t���[�̃L�����ʒu

#ifdef EV_WIN_SCROLL_CURSOR	//070228
#define CURSOR_ANMNO				(0)
#define CURSOR_SPRI					(0)
#define CURSOR_PALT					(1)

#define EVWIN_CHR_H_ID_CURSOR		(13528)
#define EVWIN_PAL_H_ID				(13528)
#define EVWIN_CEL_H_ID_CURSOR		(13528)
#define EVWIN_ANM_H_ID_CURSOR		(13528)

#define EVWIN_ACTMAX				(2)			//�A�N�^�[��(������Ɖ������̂Q��)

#define ACT_RES_PAL_NUM				(3)

//�O���t�B�b�N�f�[�^���Ȃ��̂Œu������
enum{
	ARC_EVWIN_GRA	= ARC_RANKING_GRA,
	NARC_evwin_nclr = NARC_ranking_ranking_nclr,
	NARC_evwin_ncer = NARC_ranking_ranking_ncer,
	NARC_evwin_nanr = NARC_ranking_ranking_nanr,
	NARC_evwin_ncgr = NARC_ranking_ranking_ncgr,
};
#endif


//==============================================================================================
//
//	�\����
//
//==============================================================================================
struct _WAZA_OSHIE_WORK{
	FIELDSYS_WORK * fsys;						//FIELDSYS_WORK�̃|�C���^
	TCB_PTR	tcb;

 	GF_BGL_BMPWIN bmpwin;						//BMP�E�B���h�E�f�[�^
 	GF_BGL_BMPWIN* talk_bmpwin;					//BMP��b�E�B���h�E�f�[�^
	
	STRBUF* msg_buf[EV_WIN_MSG_MAX];			//���b�Z�[�W�f�[�^�̃|�C���^
	MSGDATA_MANAGER* msgman;					//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;							//�P��Z�b�g

	u8  wait;									//�E�F�C�g
	u8  menu_id;								//BMP���j���[ID
	u8  cursor_pos;								//�J�[�\���ʒu
	u8  cancel:1;								//�L�����Z��
	u8  msgman_del_flag:1;						//���b�Z�[�W�}�l�[�W���[�폜�t���O
	u8  dmy:6;									//

	u8  x;										//�E�B���h�E�ʒuX
	u8  y;										//�E�B���h�E�ʒuY
	u8  dmyy;									//���j���[�i���o�[
	u8  list_no;								//���j���[���ڂ̉��Ԗڂ�

	u16* pMsg;									//���b�Z�[�W�f�[�^
	u16* work;									//���ʎ擾���[�N
 
	//BMP���X�g
	BMPLIST_HEADER ListH;						//BMP���X�g�w�b�_�[
	BMPLIST_WORK* lw;							//BMP���X�g�f�[�^
	u16 list_bak;								//���X�g�ʒu�o�b�N�A�b�v
	u16 cursor_bak;								//�J�[�\���ʒu�o�b�N�A�b�v
	BMPLIST_DATA list_Data[EV_WIN_LIST_MAX];	//���X�g�f�[�^
	u16 talk_msg_id[EV_WIN_LIST_MAX];			//���X�g�f�[�^�ɑ΂ɂȂ��b���b�Z�[�WID

	u16 pos_bak;

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	FIELD_CLACT		fcat;						//�t�B�[���h�p�Z���A�N�^�[�ݒ�
	CATS_ACT_PTR	act[EVWIN_ACTMAX];			//CATS���g�����̐�p�A�N�^�[�\����
#endif
};


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�E�B���h�E�@���b�Z�[�W�}�l�[�W���[�Z�b�g
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 * @param	msgman		MSGDATA_MANAGER�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvWin_MsgManSet( WAZA_OSHIE_WORK* wk, MSGDATA_MANAGER* msgman )
{
	wk->msgman = msgman;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�E�B���h�E�@���[�N������
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvWin_Init( FIELDSYS_WORK* fsys, WAZA_OSHIE_WORK* wk, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman )
{
	int i;

	wk->msgman		= msgman;		//���p��
	wk->msgman_del_flag = 0;
	wk->wordset		= wordset;		//�X�N���v�g�������p��

	wk->fsys		= fsys;
	wk->work		= work;
	wk->cancel		= cancel;
	wk->cursor_pos	= cursor;
	wk->x			= x;
	wk->y			= y;
	wk->list_no		= 0;
	wk->talk_bmpwin	= talk_bmpwin;
	wk->wait		= EV_WIN_DEFAULT_WAIT;
	wk->pos_bak		= cursor;

	for( i=0; i < EV_WIN_LIST_MAX ;i++ ){
		wk->list_Data[i].str	= NULL;
		wk->list_Data[i].param	= 0;
		wk->talk_msg_id[i] = EV_WIN_TALK_MSG_NONE;
	}

	//MSGMAN_Create�̌�ɏ���
	for( i=0; i < EV_WIN_MSG_MAX ;i++ ){
		wk->msg_buf[i] = STRBUF_Create( EVWIN_MSG_BUF_SIZE, HEAPID_FIELD );
	}

	//�I�������l���擾���郏�[�N��������
	*wk->work = EV_WIN_NOTHING;

	return;
}


//==============================================================================================
//
//	BMP���X�g�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@������
 *
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	"WAZA_OSHIE_WORK�^�̃A�h���X�ANULL�͎��s"
 */
//--------------------------------------------------------------
WAZA_OSHIE_WORK * CmdWazaOshieBmpList_Init( FIELDSYS_WORK* fsys, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman )
{
	WAZA_OSHIE_WORK* wk;
	int i;

	wk = sys_AllocMemory( HEAPID_FIELD, sizeof(WAZA_OSHIE_WORK) );
	if( wk == NULL ){
		OS_Printf( "ev_win.c Alloc ERROR!" );
		return NULL;
	}
	memset( wk, 0, sizeof(WAZA_OSHIE_WORK) );

	//���[�N������	
	EvWin_Init( fsys, wk, x, y, cursor, cancel, work, wordset, talk_bmpwin, msgman );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�쐬
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CmdWazaOshieBmpList_MakeList( WAZA_OSHIE_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	BmpList_list_make( wk, msg_id, talk_msg_id, param  );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@�J�n
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CmdWazaOshieBmpList_Start( WAZA_OSHIE_WORK* wk )
{
	//�\���ő區�ڐ��`�F�b�N
	if( wk->list_no > EV_LIST_LINE ){
		GF_BGL_BmpWinAdd( wk->fsys->bgl, &wk->bmpwin, FLD_MBGFRM_FONT, wk->x, wk->y, 
									11, EV_LIST_LINE*2, FLD_SYSFONT_PAL, EVWIN_FREE_CGX );
	}else{
		GF_BGL_BmpWinAdd( wk->fsys->bgl, &wk->bmpwin, FLD_MBGFRM_FONT, wk->x, wk->y, 
									11, wk->list_no*2, FLD_SYSFONT_PAL, EVWIN_FREE_CGX );
	}

	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
	MenuWinGraphicSet(
		wk->fsys->bgl, FLD_MBGFRM_FONT, MENU_WIN_CGX_NUM, MENU_WIN_PAL, 0, HEAPID_FIELD );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite(&wk->bmpwin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL);

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	EvWin_ActorInit( wk );
#endif

	//���X�g�w�b�_�[�����ݒ�
	BmpList_h_default_set(wk);

	wk->lw = BmpListSet( (const BMPLIST_HEADER*)&wk->ListH, 0, wk->cursor_pos, HEAPID_FIELD );

	//TCB�ǉ�
	wk->tcb	= TCB_Add( EvBmpList_MainTCB, wk, 0 );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���X�g�쐬
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_list_make( WAZA_OSHIE_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( wk->list_no < EV_WIN_LIST_MAX, "���X�g���ڐ��I�[�o�[�I" );

	{
		//�W�J����
		
		STRBUF* tmp_buf2= STRBUF_Create( EVWIN_MSG_BUF_SIZE, HEAPID_FIELD );	//�R�s�[�p�o�b�t�@

		MSGMAN_GetString( wk->msgman, msg_id, tmp_buf2 );
		WORDSET_ExpandStr( wk->wordset, wk->msg_buf[wk->list_no], tmp_buf2 );	//�W�J
		wk->list_Data[ wk->list_no ].str = (const void *)wk->msg_buf[wk->list_no];

		STRBUF_Delete( tmp_buf2 );
	}

	if( param == EV_WIN_LIST_LABEL ){
		wk->list_Data[ wk->list_no ].param = BMPLIST_RABEL;
		//*pa = BMPLIST_RABEL;
	}else{
		wk->list_Data[ wk->list_no ].param = param;
		//*pa = param;
	}

	wk->talk_msg_id[ wk->list_no ] = talk_msg_id;
	wk->list_no++;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���ڂ̒�����ő啶�������擾
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static u32 BmpList_length_get( WAZA_OSHIE_WORK* wk )
{
	int i;
	u32 ret,tmp_ret;

	ret		= 0;
	tmp_ret = 0;

	for( i=0; i < wk->list_no ;i++ ){
		if( wk->list_Data[i].str == NULL ){
			break;
		}

		ret = FontProc_GetPrintStrWidth( EV_WIN_FONT, (STRBUF*)wk->list_Data[i].str, 0 );
		
		if( tmp_ret < ret ){
			tmp_ret = ret;
		}
	}

	return tmp_ret + EV_WIN_CURSOR_WIDTH;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�w�b�_�[�����ݒ�
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_h_default_set(WAZA_OSHIE_WORK* wk)
{
	wk->ListH.list		= wk->list_Data;
	wk->ListH.call_back = BmpList_CursorMoveCallBack;
	wk->ListH.icon		= NULL;
	wk->ListH.win		= &wk->bmpwin;

	wk->ListH.count		= wk->list_no;
	wk->ListH.line		= EV_LIST_LINE;

	wk->ListH.rabel_x	= EV_LIST_RABEL_X;
	wk->ListH.data_x	= EV_LIST_DATA_X;
	wk->ListH.cursor_x	= EV_LIST_CURSOR_X;
	wk->ListH.line_y	= EV_LIST_LINE_Y;

	wk->ListH.f_col		= FBMP_COL_BLACK;
	wk->ListH.b_col		= FBMP_COL_WHITE;		
	wk->ListH.s_col		= FBMP_COL_BLK_SDW;

	wk->ListH.msg_spc	= 0;
	wk->ListH.line_spc	= 16;
	wk->ListH.page_skip	= BMPLIST_LRKEY_SKIP;
	wk->ListH.font		= EV_WIN_FONT;
	wk->ListH.c_disp_f	= 0;

	wk->ListH.work		= (void*)wk;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode )
{
	u32 count,line;
	u16 list_bak = 0;
	u16 cursor_bak = 0;
	WAZA_OSHIE_WORK* evwin_wk = (WAZA_OSHIE_WORK*)BmpListParamGet(wk,BMPLIST_ID_WORK);

#ifdef EV_WIN_SCROLL_CURSOR	//070228

	//��������
	if( mode == 1 ){
		//
	}

	count = BmpListParamGet( wk, BMPLIST_ID_COUNT );	//���X�g���ڐ�
	line  = BmpListParamGet( wk, BMPLIST_ID_LINE );		//�\���ő區�ڐ�

	//�S�Ă̍��ڂ�\�����Ă��Ȃ���
	if( count > line ){

		BmpListPosGet( wk, &list_bak, &cursor_bak );

		if( list_bak == 0 ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\����\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], FALSE );	//OBJ�̔�\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}else if( list_bak == (count-line) ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\����\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], FALSE );	//OBJ�̔�\���ݒ�
		}else{
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}
	}

#endif

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g���C��
 *
 * @param	tcb			TCB_PTR
 * @param	wk			���[�N�̃A�h���X
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk )
{
	u16 tmp_pos_bak;
	u32	ret;
	WAZA_OSHIE_WORK* swk;
	swk = (WAZA_OSHIE_WORK*)wk;

	//�E�F�C�g
	if( swk->wait != 0 ){
		swk->wait--;
		return;
	}

	//���C�v���͏������Ȃ�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return;
	}

	ret = BmpListMain( swk->lw );

	//BMP���X�g�̑S�̈ʒu���擾
	tmp_pos_bak = swk->pos_bak;		//�ޔ�
	BmpListDirectPosGet( swk->lw, &swk->pos_bak );

	//�J�[�\�������������`�F�b�N
	if( tmp_pos_bak != swk->pos_bak ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	CLACT_Draw( swk->fcat.cas );
#endif

	switch( ret ){
	case BMPLIST_NULL:
		break;
	case BMPLIST_CANCEL:
		if( swk->cancel == TRUE ){			//TRUE = B�L�����Z���L��
			Snd_SePlay( SEQ_SE_DP_SELECT );
			*swk->work = EV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
			EvWin_ActorRelease( wk );		//�폜
#endif
			EvBmpList_Del(wk);
		}
		break;
	default:
		Snd_SePlay( SEQ_SE_DP_SELECT );
		*swk->work = ret;					//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
		EvWin_ActorRelease( wk );			//�폜
#endif
		EvBmpList_Del(wk);
		break;
	};

	return;
};

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�I��
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * SEQ_SE_DP_SELECT��炵�Ă���I
 */
//--------------------------------------------------------------
static void EvBmpList_Del( WAZA_OSHIE_WORK* wk )
{
	int i;

	Snd_SePlay(SEQ_SE_DP_SELECT);	//���ӁI

	BmpListExit( wk->lw, NULL, NULL );
	BmpMenuWinClear( wk->ListH.win, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &wk->bmpwin );

	for( i=0; i < EV_WIN_MSG_MAX ;i++ ){
		STRBUF_Delete( wk->msg_buf[i] );
	}

	if( wk->msgman_del_flag == 1 ){
		//WORDSET_Delete( wk->wordset );
		MSGMAN_Delete( wk->msgman );
	}

	TCB_Delete( wk->tcb );
	sys_FreeMemoryEz( wk );
	return;
}


//==============================================================================================
//
//	�X�N���[���J�[�\��
//
//==============================================================================================
#ifdef EV_WIN_SCROLL_CURSOR	//070228

//�A�N�^�[������
static void EvWin_ActorInit( WAZA_OSHIE_WORK* wk )
{
	int i;
	s16 x = (wk->x * 8) + (BmpList_length_get(wk) / 2);

	//���\�[�X�o�^�ő吔��`�\����
	TCATS_RESOURCE_NUM_LIST	crnl = { 1, 1, 1, 1 };

	//�o�^�p�\���� �P�̓o�^�p(���W�͌�Őݒ肵����)
	static const TCATS_OBJECT_ADD_PARAM_S ActAddParam_S[] =
	{
		//�����
		{
			0, 0, 0,
			CURSOR_ANMNO, CURSOR_SPRI, CURSOR_PALT, NNS_G2D_VRAM_TYPE_2DMAIN,
			{
				EVWIN_CHR_H_ID_CURSOR, EVWIN_PAL_H_ID,
				EVWIN_CEL_H_ID_CURSOR, EVWIN_ANM_H_ID_CURSOR, 0, 0,
			},
			0, 0
		},

		//������
		{
			0, 0, 0,
			CURSOR_ANMNO, CURSOR_SPRI, CURSOR_PALT, NNS_G2D_VRAM_TYPE_2DMAIN,
			{
				EVWIN_CHR_H_ID_CURSOR, EVWIN_PAL_H_ID,
				EVWIN_CEL_H_ID_CURSOR, EVWIN_ANM_H_ID_CURSOR, 0, 0,
			},
			0, 0
		},
	};
	
	//�t�B�[���h�Z���A�N�^�[������
	FieldCellActSet_S( &wk->fcat, &crnl, EVWIN_ACTMAX, HEAPID_FIELD );
	
	{
		ARCHANDLE* hdl;
		
		hdl = ArchiveDataHandleOpen( ARC_EVWIN_GRA, HEAPID_FIELD );

		//���\�[�X�ݒ�
		FldClact_LoadResPlttArcH(	&wk->fcat, hdl, NARC_evwin_nclr,
									0, ACT_RES_PAL_NUM, NNS_G2D_VRAM_TYPE_2DMAIN, 
									EVWIN_PAL_H_ID );

		FldClact_LoadResCellArcH(	&wk->fcat, hdl, NARC_evwin_ncer,
									0, EVWIN_CEL_H_ID_CURSOR );

		FldClact_LoadResCellAnmArcH(&wk->fcat, hdl, NARC_evwin_nanr,
									0, EVWIN_ANM_H_ID_CURSOR );

		FldClact_LoadResourceCharArcH(	&wk->fcat, hdl, NARC_evwin_ncgr,
										0, NNS_G2D_VRAM_TYPE_2DMAIN, EVWIN_CHR_H_ID_CURSOR );
		
		ArchiveDataHandleClose( hdl );
	}

	//�Z���A�N�^�[�ǉ�(�P���p)
	for( i=0; i < EVWIN_ACTMAX; i++ ){
		wk->act[i] = FieldCellActAdd_S( &wk->fcat, &ActAddParam_S[i] );
		CLACT_SetAnmFlag( wk->act[i]->act, 1 );				//�I�[�g�A�j��
		CATS_ObjectEnableCap( wk->act[i], FALSE );			//OBJ�̔�\���ݒ�
	}

	//���W�̐ݒ�
	//OS_Printf( "x = %d\n", x );
	//OS_Printf( "wk->y = %d\n", (wk->y*8) );
	//OS_Printf( "y = %d\n", (wk->list_no*8) );
	CATS_ObjectPosSetCap( wk->act[0], x, (wk->y*8) );
	CATS_ObjectPosSetCap( wk->act[1], x, (wk->list_no*8) );

	//�I�[�g�A�j���őΉ�����Ȃ��āA���W�𓮂����ăA�j���ł������̂����B�B�ۗ�
	
	//�I���J�[�\���̐F�ύX
	//CATS_ObjectPaletteSetCap(wk->act[ACT_CURSOR],CURSOR_PALF);
	
	return;
}

//�A�N�^�[�폜
static void EvWin_ActorRelease( WAZA_OSHIE_WORK* wk )
{
	int i;

	//�A�N�^�[�|�C���^�̍폜
	for( i=0; i < EVWIN_ACTMAX; i++ ){
		if( wk->act[i] != NULL ){
			CATS_ActorPointerDelete_S( wk->act[i] );
		}
	}

	//�Z���A�N�^�[�폜�i�P���p�j
	FieldCellActDelete_S( &wk->fcat );
	return;
}
#endif

//--------------------------------------------------------------------------------------------
/**
 * �Z�����{�[�h�\��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieBoardWrite( VM_MACHINE * core )
{
	FIELDSYS_WORK* fsys = core->fsys;
	WORDSET** wordset	= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_WORDSET );
	EV_WIN_WORK** ev_win= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_EVWIN );	//EV_WIN_WORK�擾
	u8 x				= VMGetU8(core);
	u8 y				= VMGetU8(core);
	u16 no				= VMGetWorkValue(core);
	u16* wk				= VMGetWork(core);

	//OS_Printf( "no = %d\n", no );
	no = GetWazaOshieWazaNoByWazaNo( no );
	//OS_Printf( "no = %d\n", no );
	*ev_win = WazaOshieBoardWrite(	fsys, x, y, wk, *wordset, 
									waza_oshie_tbl[no].aka,
									waza_oshie_tbl[no].ao,
									waza_oshie_tbl[no].ki,
									waza_oshie_tbl[no].midori );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �Z�����{�[�h�폜
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdWazaOshieBoardDel( VM_MACHINE * core )
{
	FIELDSYS_WORK* fsys = core->fsys;
	EV_WIN_WORK** ev_win= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_EVWIN );	//EV_WIN_WORK�擾
	WazaOshieBoardDel( *ev_win );
	return 0;
}


