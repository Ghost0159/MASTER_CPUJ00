//==============================================================================================
/**
 * @file	castle_sys.h
 * @brief	�u�o�g���L���b�X���v�V�X�e���w�b�_�[
 * @author	Satoshi Nohara
 * @date	07.07.05
 */
//==============================================================================================
#ifndef _CASTLE_SYS_H_
#define _CASTLE_SYS_H_

#include "common.h"
#include "system/procsys.h"


//==============================================================================================
//
//	�^�N�g���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _CASTLE_MINE_WORK CASTLE_MINE_WORK;
typedef	struct _CASTLE_ENEMY_WORK CASTLE_ENEMY_WORK;
typedef	struct _CASTLE_RANK_WORK CASTLE_RANK_WORK;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�p���b�g��`
#define BC_TALKWIN_PAL			(10)				//��b�E�B���h�E�g�p�p���b�g�i���o�[
#define BC_MENU_PAL				(11)				//���j���[�E�B���h�E�g�p�p���b�g�i���o�[
#define BC_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BC_FONT_PAL				(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[

#define CASTLE_VANISH_ON		(0)					//��\��
#define CASTLE_VANISH_OFF		(1)					//�\��

//���\�[�X�}�l�[�W���[�I�u�W�F�N�g�i���o�[
enum{
	RES_OBJ_CSR = 0,								//�J�[�\��
	RES_OBJ_ITEMICON,								//�A�C�e���A�C�R��
	RES_OBJ_ITEMKEEP,								//�A�C�e���������Ă���A�C�R��
	RES_OBJ_ICON1,									//�A�C�R��1
	RES_OBJ_ICON2,									//�A�C�R��2
	RES_OBJ_ICON3,									//�A�C�R��3
	RES_OBJ_ICON4,									//�A�C�R��4
	RES_OBJ_MAX,									//�ő吔(���\�[�X�t�@�C����1�A���Ƃ̓A�C�R��)
};

//�Ǘ�CHAR_ID
enum{
	ID_CHAR_CSR = 0,								//�J�[�\��
	ID_CHAR_ITEMICON,								//�A�C�e���A�C�R��
	ID_CHAR_ITEMKEEP,								//�A�C�e���������Ă���A�C�R��
	ID_CHAR_ICON1,									//�A�C�R��1
	ID_CHAR_ICON2,									//�A�C�R��2
	ID_CHAR_ICON3,									//�A�C�R��3
	ID_CHAR_ICON4,									//�A�C�R��4
	ID_CHAR_MAX,									//
};

//�Ǘ�PLTT_ID
enum{
	ID_PLTT_CSR = 0,								//�J�[�\��
	ID_PLTT_ITEMICON,								//�A�C�e���A�C�R��
	ID_PLTT_ITEMKEEP,								//�A�C�e���������Ă���A�C�R��
	ID_PLTT_ICON,									//�A�C�R��
	ID_PLTT_MAX,									//
};

//�Ǘ�CELL_ID
enum{
	ID_CELL_CSR = 0,								//�J�[�\��
	ID_CELL_ITEMICON,								//�A�C�e���A�C�R��
	ID_CELL_ITEMKEEP,								//�A�C�e���������Ă���A�C�R��
	ID_CELL_ICON,									//�A�C�R��
	ID_CELL_MAX,									//
};

//�Ǘ�CELLANM_ID
enum{
	ID_CELLANM_CSR = 0,								//�J�[�\��
	ID_CELLANM_ITEMICON,							//�A�C�e���A�C�R��
	ID_CELLANM_ITEMKEEP,							//�A�C�e���������Ă���A�C�R��
	ID_CELLANM_ICON,								//�A�C�R��
	ID_CELLANM_MAX,									//
};

//�Ǘ�ID
enum{
	ID_OBJ_CSR = 0,									//�J�[�\��
	ID_OBJ_ITEMICON,								//�A�C�e���A�C�R��
	ID_OBJ_ITEMKEEP,								//�A�C�e���������Ă���A�C�R��
	//ID_OBJ_ITEMKEEP1,								//�A�C�e���������Ă���A�C�R��1
	//ID_OBJ_ITEMKEEP2,								//�A�C�e���������Ă���A�C�R��2
	//ID_OBJ_ITEMKEEP3,								//�A�C�e���������Ă���A�C�R��3
	//ID_OBJ_ITEMKEEP4,								//�A�C�e���������Ă���A�C�R��4
	ID_OBJ_ICON1,									//�A�C�R��1
	ID_OBJ_ICON2,									//�A�C�R��2
	ID_OBJ_ICON3,									//�A�C�R��3
	ID_OBJ_ICON4,									//�A�C�R��4
	ID_OBJ_MAX,
};

//MINE:POKEICON 4 + ITEMKEEP 4 + CSR 2 + SCR 2 + POKESEL 1 + EFF 1 + HP 4 + ITEMICON 1  =
//ENEMY:POKEICON 4 + ITEMKEEP 4 + CSR 2 + SCR 2 + POKESEL 1 + EFF 1 + HP 4				= 
//RANK:CSR 2 + POKESEL 1 + EFF 1 + HATA 2*(3*3)											= 22

//�A�C�R��4 + �A�C�e�������Ă���A�C�R��4 + �J�[�\��2 + �X�N���[���J�[�\��2
//�|�P�I���J�[�\��1 + �G�t�F�N�g1 + HP�o�[4 + ��2*(3*3)
//#define CLACT_OBJ_MAX	(12)//(8)							//�Z���A�N�^�[�ɐݒ肷��ő�A�N�^�[��
#define CLACT_OBJ_MAX	(40)//(8)							//�Z���A�N�^�[�ɐݒ肷��ő�A�N�^�[��

//�A�j���i���o�[
enum{
	CASTLE_ANM_MODORU_ZIBUN = 0,
	CASTLE_ANM_SELECT_ZIBUN,
	CASTLE_ANM_SELECT_AITE,
	CASTLE_ANM_CURSOR_ITEM, 
	CASTLE_ANM_SCR_CURSOR_U, 
	CASTLE_ANM_SCR_CURSOR_D, 
	CASTLE_ANM_BALL_STOP, 
	CASTLE_ANM_BALL_MOVE, 
	CASTLE_ANM_KAIHUKU_EFF, 
	CASTLE_ANM_LV_UP, 
	CASTLE_ANM_LV_DOWN, 
	CASTLE_ANM_RANK_UP, 
	CASTLE_ANM_HATA, 
	CASTLE_ANM_HP_AKA, 
	CASTLE_ANM_HP_KIIRO, 
	CASTLE_ANM_HP_MIDORI, 
	CASTLE_ANM_ITEMGET_EFF, 
	CASTLE_ANM_MODORU_AITE, 
};

#define BC_FRAME_WIN	(GF_BGL_FRAME1_M)
#define BC_FRAME_TYPE	(GF_BGL_FRAME0_M)
#define BC_FRAME_SLIDE	(GF_BGL_FRAME2_M)			//MULTI�ƃ����^�����莞
#define BC_FRAME_BG		(GF_BGL_FRAME3_M)			//SINGLE,DOUBLE
#define BC_FRAME_SUB	(GF_BGL_FRAME0_S)			//�{�[��

typedef struct{
	s16  x;
	s16  y;
}POS;

//���薳���̒�`
#define CASTLE_DECIDE_TYPE_NONE	(0xff)

#define CASTLE_RANK_MAX			(3)					//�ő僉���N3

//#define CASTLE_COMM_WAIT		(60)				//�ʐM�E�F�C�g
#define CASTLE_COMM_WAIT		(30)				//�ʐM�E�F�C�g


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern const PROC_DATA CastleProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT CastleProc_Init( PROC * proc, int * seq );
extern PROC_RESULT CastleProc_Main( PROC * proc, int * seq );
extern PROC_RESULT CastleProc_End( PROC * proc, int * seq );

//�ʐM
extern void CastleMine_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void CastleMine_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work);
extern void CastleMine_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
extern void CastleMine_CommRecvBufModoru(int id_no,int size,void *pData,void *work);
extern void CastleEnemy_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void CastleEnemy_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work);
extern void CastleEnemy_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
extern void CastleEnemy_CommRecvBufModoru(int id_no,int size,void *pData,void *work);
extern void CastleRank_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void CastleRank_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work);
extern void CastleRank_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
extern void CastleRank_CommRecvBufModoru(int id_no,int size,void *pData,void *work);


#endif //_CASTLE_SYS_H_


