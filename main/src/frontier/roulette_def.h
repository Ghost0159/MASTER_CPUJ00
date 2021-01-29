//============================================================================================
/**
 * @file	roulette_def.h
 * @bfief	�u�o�g�����[���b�g�v��`
 * @author	Satoshi Nohara
 * @date	07.09.06
 */
//============================================================================================
#ifndef	__ROULETTE_DEF_H__
#define	__ROULETTE_DEF_H__


//============================================================================================
//
//	��`(enum�֎~)
//
//============================================================================================
#define ROULETTE_LAP_ENEMY_MAX		(7)							//1�T���̓G�g���[�i�[�̍ő吔
#define ROULETTE_LAP_MAX			(8)							///���[���b�g�̍ő����
#define ROULETTE_LAP_MULTI_ENEMY_MAX	(ROULETTE_LAP_ENEMY_MAX*2)//�}���`1�T���̓G�g���[�i�[�̍ő吔

#define ROULETTE_ENTRY_POKE_MAX		(3)							//�v���C���[1�l�̍ő�Q���|�P������

//�^�C�v
#define ROULETTE_TYPE_SINGLE		(0)
#define ROULETTE_TYPE_DOUBLE		(1)
#define ROULETTE_TYPE_MULTI			(2)
#define ROULETTE_TYPE_WIFI_MULTI	(3)
#define ROULETTE_TYPE_MAX			(4)							//�ő吔

//�|�P������
#define ROULETTE_MINE_POKE_MAX	(4)									//�����|�P�����f�[�^�̍ő吔
#define ROULETTE_ENEMY_POKE_MAX	(4)									//�G�|�P�����f�[�^�̍ő吔
#define ROULETTE_TOTAL_POKE_MAX	(ROULETTE_MINE_POKE_MAX+ROULETTE_ENEMY_POKE_MAX)//���`�F�b�N�Ɏg�p

//�߂�l�ɕK�v�ȃ��[�N�̐�
#define	ROULETTE_PARTY_POKE_MAX		(6)								//POKEPARTY�̃|�P������(6���ő�)
#define ROULETTE_RET_WORK_MAX		(ROULETTE_PARTY_POKE_MAX)		//ret_work[]
#define ROULETTE_SEL_WORK_MAX		(ROULETTE_PARTY_POKE_MAX)		//sel_poke_no[]

//�߂�l
#define ROULETTE_RET_CANCEL			(0xff)							//�������Ȃ��ŃL�����Z������

#define CSR_WAIT_TBL_MAX			(8)								//�J�[�\���X�s�[�h�e�[�u���ő吔

//--------------------------------------------------------------------------------------------
//ROULETTE_SCRWORK�̎擾�R�[�h(roulette.s�Ŏg�p)
//--------------------------------------------------------------------------------------------
//#define FR_ID_SET_MODE					(0)
//#define FR_ID_SET_LEVEL					(1)
#define FR_ID_SET_TYPE					(2)
#define FR_ID_GET_RET_WORK				(3)
#define FR_ID_GET_RENSYOU				(4)
#define FR_ID_INC_RENSYOU				(5)
#define FR_ID_COMM_COMMAND_INITIALIZE	(6)
#define FR_ID_SYSTEM_RESET				(7)
//#define FR_ID_CLEAR_PLAY_DATA			(8)
#define FR_ID_IS_SAVE_DATA_ENABLE		(9)
#define FR_ID_SAVE_REST_PLAY_DATA		(10)
#define FR_ID_SET_PANEL_BG				(11)
#define FR_ID_GET_PANEL_BP				(12)
//#define FR_ID_SET_NG					(13)
#define FR_ID_INC_ROUND					(14)
#define FR_ID_GET_ITEM					(15)
#define FR_ID_GET_DECIDE_EV_NO			(16)
#define FR_ID_GET_TYPE					(17)
#define FR_ID_SET_TEMP_ITEM				(18)
#define FR_ID_GET_LAP					(19)
#define FR_ID_GET_TR_OBJ_CODE			(20)
#define FR_ID_SET_LOSE					(21)
#define FR_ID_SET_CLEAR					(22)
#define FR_ID_GET_ROUND					(23)
#define FR_ID_GET_RETIRE_FLAG			(24)
#define FR_ID_ICON_CHANGE				(25)
//#define FR_ID_CLEAR_PARENT_CHECK_FLAG	(26)
//#define FR_ID_GET_PAIR_DECIDE_CHECK		(27)
#define FR_ID_CHECK_COMM_TYPE			(28)
#define FR_ID_RESOURCE_SET_POKE_ICON	(29)
#define FR_ID_RESOURCE_FREE_POKE_ICON	(30)
#define FR_ID_ADD_POKE_ICON				(31)
#define FR_ID_DEL_POKE_ICON				(32)
#define FR_ID_RESOURCE_SET_ITEMKEEP_ICON	(33)
#define FR_ID_RESOURCE_FREE_ITEMKEEP_ICON	(34)
#define FR_ID_ADD_ITEMKEEP_ICON			(35)
#define FR_ID_DEL_ITEMKEEP_ICON			(36)
#define FR_ID_SET_MAIN_BG				(37)
#define FR_ID_TR_NAME					(38)
#define FR_ID_ADD_EMITTER				(39)
#define	FR_ID_SET_B_TOWER_PARTNER_DATA	(40)
#define FR_ID_MINE_CHG_POKE_ICON_VANISH	(41)
#define FR_ID_ENEMY_CHG_POKE_ICON_VANISH	(42)
#define FR_ID_EFF_PAL_CHG				(43)
#define FR_ID_OBJ_PAL_CHG				(44)
#define FR_ID_GET_BP_POINT				(45)
#define FR_ID_PANEL_BG_OFF				(46)
#define	FR_ID_GET_POKE_SAFE_FLAG		(47)
#define FR_ID_CHECK_BRAIN				(48)
#define FR_ID_ENEMY_POKE_CREATE			(49)
#define FR_ID_ENEMY_POKE_CREATE_2		(50)
#define FR_ID_BRAIN_APPEAR_MSG_CHK		(51)
#define FR_ID_RECIEVE_COUNT_CLEAR		(52)


//--------------------------------------------------------------------------------------------
//d32r0501.ev�Ŏg�p
//--------------------------------------------------------------------------------------------
#define ROULETTE_TOOL_CHK_ENTRY_POKE_NUM	(0)
#define ROULETTE_TOOL_GET_CLEAR_FLAG		(1)
#define ROULETTE_TOOL_GET_CLEAR_MONSNO		(2)
#define ROULETTE_TOOL_SET_NEW_CHALLENGE		(3)
#define ROULETTE_TOOL_SELECT_POKE			(4)


//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------
//�ʐM�o�b�t�@�̃T�C�Y(send_buf,recv_buf)
#define ROULETTE_COMM_BUF_LEN			(40)				//34*2=68
#define ROULETTE_HUGE_BUF_LEN			(512)//(1024)

//�ʐM�^�C�v
//fssc_roulette_sub.c
#define ROULETTE_COMM_BASIC				(0)
#define ROULETTE_COMM_TR				(1)
#define ROULETTE_COMM_ENEMY				(2)
#define ROULETTE_COMM_RETIRE			(3)
#define ROULETTE_COMM_TRADE_YESNO		(4)
#define ROULETTE_COMM_TEMOTI			(5)
//�����N�ʐM
#define ROULETTE_COMM_RANK_PAIR			(30)
#define ROULETTE_COMM_RANK_UP_TYPE		(31)
#define ROULETTE_COMM_RANK_CSR_POS		(32)

//�ʐM����1�l�̃|�P������
#define	ROULETTE_COMM_POKE_NUM			(2)

//�ʐM����2�l�̃|�P������
#define	ROULETTE_COMM_POKE_TOTAL_NUM	(2*2)

//����(�}���`�Ȃ�G�̐���4�ȂǁA1�l�������2�ɂȂ�)
#define ROULETTE_FLAG_SOLO				(0)					//2
#define ROULETTE_FLAG_TOTAL				(1)					//2+2=4

//�ʐM�l��
#define ROULETTE_COMM_PLAYER_NUM		(2)

//�e�̌���
#define ROULETTE_SCR_MENU_NONE			(0)
#define ROULETTE_SCR_MENU_TEMOTI		(1)
#define ROULETTE_SCR_MENU_TRAINER		(2)
#define ROULETTE_SCR_MENU_RANK			(3)
#define ROULETTE_SCR_MENU_TAISEN		(4)
#define ROULETTE_SCR_MENU_NUKEMITI		(5)
#define ROULETTE_SCR_MENU_MAX			(6)

//�u�߂�v�\���E�F�C�g
#define ROULETTE_MODORU_WAIT			(15)


//--------------------------------------------------------------------------------------------
//�C�x���g�i���o�[(�p�l���i���o�[)(���炳�Ȃ��I)
//--------------------------------------------------------------------------------------------
#define ROULETTE_EV_ENEMY_HP_DOWN		( 0 )			//�G
#define ROULETTE_EV_ENEMY_DOKU			( 1 )
#define ROULETTE_EV_ENEMY_MAHI			( 2 )
#define ROULETTE_EV_ENEMY_YAKEDO		( 3 )
#define ROULETTE_EV_ENEMY_NEMURI		( 4 )
#define ROULETTE_EV_ENEMY_KOORI			( 5 )
#define ROULETTE_EV_ENEMY_SEED_GET		( 6 )
#define ROULETTE_EV_ENEMY_ITEM_GET		( 7 )
#define ROULETTE_EV_ENEMY_LV_UP			( 8 )
#define ROULETTE_EV_MINE_HP_DOWN		( 9 )			//����
#define ROULETTE_EV_MINE_DOKU			( 10 )
#define ROULETTE_EV_MINE_MAHI			( 11 )
#define ROULETTE_EV_MINE_YAKEDO			( 12 )
#define ROULETTE_EV_MINE_NEMURI			( 13 )
#define ROULETTE_EV_MINE_KOORI			( 14 )
#define ROULETTE_EV_MINE_SEED_GET		( 15 )
#define ROULETTE_EV_MINE_ITEM_GET		( 16 )
#define ROULETTE_EV_MINE_LV_UP			( 17 )
#define ROULETTE_EV_PLACE_HARE			( 18 )			//��ɋN����
#define ROULETTE_EV_PLACE_AME			( 19 )
#define ROULETTE_EV_PLACE_SUNAARASI		( 20 )
#define ROULETTE_EV_PLACE_ARARE			( 21 )
#define ROULETTE_EV_PLACE_KIRI			( 22 )
#define ROULETTE_EV_PLACE_TRICK			( 23 )
#define ROULETTE_EV_PLACE_SPEED_UP		( 24 )
#define ROULETTE_EV_PLACE_SPEED_DOWN	( 25 )
#define ROULETTE_EV_PLACE_RANDOM		( 26 )
#define ROULETTE_EV_EX_POKE_CHANGE		( 27 )			//����
#define ROULETTE_EV_EX_BP_GET			( 28 )
#define ROULETTE_EV_EX_BTL_WIN			( 29 )
#define ROULETTE_EV_EX_SUKA				( 30 )
#define ROULETTE_EV_EX_BP_GET_BIG		( 31 )
#define ROULETTE_EV_MAX					( 32 )			//�ő吔
#define ROULETTE_EV_NONE				( ROULETTE_EV_MAX )

//�C�x���g�J�n�ʒu(roulette_def.h�ɂ���C�x���g�i���o�[���炳�Ȃ��悤�ɂ���I)
#define ROULETTE_EV_ENEMY_START			( ROULETTE_EV_ENEMY_HP_DOWN )
#define ROULETTE_EV_MINE_START			( ROULETTE_EV_MINE_HP_DOWN )
#define ROULETTE_EV_PLACE_START			( ROULETTE_EV_PLACE_HARE )
#define ROULETTE_EV_EX_START			( ROULETTE_EV_EX_POKE_CHANGE )


//--------------------------------------------------------------------------------------------
//�p�l���̃J���[��`
//--------------------------------------------------------------------------------------------
#define ROULETTE_PANEL_COLOR_RED		( 0 )
#define ROULETTE_PANEL_COLOR_BLACK		( 1 )
#define ROULETTE_PANEL_COLOR_WHITE		( 2 )
#define ROULETTE_PANEL_COLOR_YELLOW		( 3 )
#define ROULETTE_PANEL_COLOR_MAX		( 4 )


//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------
//���񐔂ɂ���āA��{���x��ݒ肷��悤�ɂȂ����̂ŁA���̒�`�͈Ӗ����Ȃ�
#define CSR_SPEED_LEVEL_DEFAULT			(3)				//�J�[�\���X�s�[�h�̃f�t�H���g���x��

//���R�[�h�Z�b�g�ɓn���O�ɐ��������Ȃ��ƃ_���I
//�d�l���́u�Z�[�u�f�[�^�ɂ��āv�̗��ɍő吔�������Ă���
#define ROULETTE_RENSYOU_MAX			(9999)			//�A����


#endif	//__ROULETTE_DEF_H__


