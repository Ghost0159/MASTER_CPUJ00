//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		minigame_tool.h
 *	@brief		�~�j�Q�[���c�[��
 *	@author		tomoya takahashi	
 *	@data		2007.10.22
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __MINIGAME_TOOL_H__
#define __MINIGAME_TOOL_H__

#include "gflib/clact.h"

#include "savedata/mystatus.h"
#include "savedata/savedata_def.h"

#include "application/wifi_lobby/wflby_system.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
// �����Ŏg�p����ʐM�����萔	�i�~�j�Q�[���Ƃ��Ԃ�Ȃ��悤�Ɂj
enum{
	MNGM_SYNC_START	= 128,
	MNGM_SYNC_END,			// �S�̂̏I�������킹�܂��B
	MNGM_SYNC_RESULT_END,	// ���ʉ�ʂ̏I�������킹�܂��B
};

#define MNGM_COUNT_PALNUM		( 3 )	//�J�E���g�_�E���Ŏg�p����p���b�g�{��


// ���犄�K�W�F�b�g�A�b�v�f�[�g�萔
#define MNGM_BALLOON_GADGET_UPDATE	( 10 )

// Balance�{�[���ŉ������Ȃ������Ƃ��ɓ����Ă���score�̒l
#define MNGM_BALANCEBALL_NOTOUCH_SCORE	( 0 )


// Rare�Q�[���^�C�v
typedef enum {
	// �ʂ���
	MNGM_RAREGAME_BUCKET_NORMAL	= 0,
	MNGM_RAREGAME_BUCKET_REVERSE,
	MNGM_RAREGAME_BUCKET_BIG,
	MNGM_RAREGAME_BUCKET_NUM,

	// �ʏ��
	MNGM_RAREGAME_BALANCEBALL_NORMAL = 0,
	MNGM_RAREGAME_BALANCEBALL_BIG,
	MNGM_RAREGAME_BALANCEBALL_NUM,

	// ���犄
	MNGM_RAREGAME_BALLOON_NORMAL = 0,
	MNGM_RAREGAME_BALLOON_THICK,	//��
	MNGM_RAREGAME_BALLOON_FINE,		//��
	MNGM_RAREGAME_BALLOON_NUM,
} MNGM_RAREGAME_TYPE;

// Rare�Q�[���̃p�[�Z���e�[�W	MAX��100
// �ʂ���
#define MNGM_RAREPAR_BUCKET_NORMAL		(75)
#define MNGM_RAREPAR_BUCKET_REVERSE		(15)
#define MNGM_RAREPAR_BUCKET_BIG			(10)
// �ʏ��
#define MNGM_RAREPAR_BALANCEBALL_NORMAL	(85)
#define MNGM_RAREPAR_BALANCEBALL_BIG	(15)
// ���犄
#define MNGM_RAREPAR_BALLOON_NORMAL		(85)
#define MNGM_RAREPAR_BALLOON_THICK		(15)
#define MNGM_RAREPAR_BALLOON_FINE		(0)


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�~�j�Q�[��WiFi���r�[���
//=====================================
typedef struct {
	WFLBY_ITEMTYPE			gadget;
	WFLBY_MINIGAME_PLIDX	plidx;		// �v���C���[�C���f�b�N�X
	WFLBY_VIPFLAG			vipflag;	// VIP�t���O
	BOOL					error_end;	// �G���[�I���`�F�b�N

	// 080627 mystatus�擾�ɂ��g�p
	MYSTATUS*				p_mystate[WFLBY_MINIGAME_MAX];	// �S����MYSTATUS

	// �g�s�b�N�𐶐�����K�v���o�Ă����̂ŁA����SYSTEM���̂����Ă��܂�
	WFLBY_SYSTEM*			p_wflbysystem;	// �������ATOPIC�̐����ɂ����g�p���Ȃ�
} WFLBY_MINIGAME_WK;


//-------------------------------------
///	�G���g���[�A���ʉ�ʋ��ʃ��[�N
//  �z��̒��g�͂��ׂăv���C���[no�ŎQ�Ƃ��܂�
//=====================================
typedef struct {
	SAVEDATA*				p_save;		// �Z�[�u�f�[�^
	u8	pnetid[WFLBY_MINIGAME_MAX];		// �v���C���[�ɑΉ�����NETID
	u8	num;							// �G���g���[�l��
	u8	my_playerid;					// �����̃v���C���[ID
	u8	vchat;							// vchat�Ή���
	u8	replay;							// ������x�V�Ԃ����₷�邩
	WFLBY_MINIGAME_WK*		p_lobby_wk;	// ���r�[�p���
} MNGM_ENRES_PARAM;


//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
typedef struct _MNGM_ENTRYWK MNGM_ENTRYWK;

//-------------------------------------
///	���ʉ�ʃ��[�N
//  �z��̒��g�͂��ׂăv���C���[no�ŎQ�Ƃ��܂�
//=====================================
typedef struct _MNGM_RESULTWK MNGM_RESULTWK;
typedef struct {

	//	�ʂ���A�ʏ��p�X�R�A�f�[�^
	u32 score[ WFLBY_MINIGAME_MAX ];	// �e�Q�[���ł̃X�R�A
	u8	result[ WFLBY_MINIGAME_MAX ];	// �v���C���[No��INDEX�ɂ�������(0.1.2.3)

	// ���犄�p�@��������
	u32 balloon; 

} MNGM_RESULT_PARAM;


//-------------------------------------
///	START	TIMEUP	���[�N
//=====================================
typedef struct _MNGM_COUNTWK MNGM_COUNTWK;




//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�G���g���[��ʁA���ʉ�ʋ��ʃp�����[�^�c�[��
//=====================================
extern void MNGM_ENRES_PARAM_Init( MNGM_ENRES_PARAM* p_param, BOOL lobby_flag, SAVEDATA* p_save, BOOL vchat, WFLBY_MINIGAME_WK* p_lobby_wk );
extern u32 MNGM_ENRES_PARAM_GetNetIDtoPlNO( const MNGM_ENRES_PARAM* cp_param, u32 netid );
extern u32 MNGM_ENRES_PARAM_GetPlNOtoNetID( const MNGM_ENRES_PARAM* cp_param, u32 plno );
extern BOOL MNGM_ENRES_PARAM_GetVipFlag( const MNGM_ENRES_PARAM* cp_param, u32 netid );
extern MYSTATUS* MNGM_ENRES_PARAM_GetMystatus( const MNGM_ENRES_PARAM* cp_param, u32 netid );



//-------------------------------------
///	�G���g���[��ʃ��[�N
//=====================================
extern MNGM_ENTRYWK* MNGM_ENTRY_InitBallSlow( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID );
extern MNGM_ENTRYWK* MNGM_ENTRY_InitBalanceBall( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID );
extern MNGM_ENTRYWK* MNGM_ENTRY_InitBalloon( const MNGM_ENRES_PARAM* cp_commparam, u32 heapID );
extern void MNGM_ENTRY_Exit( MNGM_ENTRYWK* p_wk );
extern BOOL MNGM_ENTRY_Wait( const MNGM_ENTRYWK* cp_wk );
extern MNGM_RAREGAME_TYPE MNGM_ENTRY_GetRareGame( const MNGM_ENTRYWK* cp_wk );

//-------------------------------------
///	���ʉ�ʃ��[�N
//=====================================
extern MNGM_RESULTWK* MNGM_RESULT_InitBallSlow( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param,  u32 heapID );
extern MNGM_RESULTWK* MNGM_RESULT_InitBalanceBall( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 heapID );
extern MNGM_RESULTWK* MNGM_RESULT_InitBalloon( const MNGM_ENRES_PARAM* cp_commparam, const MNGM_RESULT_PARAM* cp_param, u32 heapID );
extern void MNGM_RESULT_Exit( MNGM_RESULTWK* p_wk );
extern BOOL MNGM_RESULT_Wait( const MNGM_RESULTWK* cp_wk );
extern BOOL MNGM_RESULT_GetReplay( const MNGM_RESULTWK* cp_wk );

// ���ʉ�ʃp�����[�^�����c�[��
// ����K�W�F�b�g�p�����[�^�̐ݒ�c�[���Ȃǂ��ǉ����Ă���

// �ʓ����Ƌʏ��́Ascore�f�[�^�����ׂĐݒ肵�Ă��炱�̊֐����Ă�ł��������B
extern void MNGM_RESULT_CalcRank( MNGM_RESULT_PARAM* p_param, u32 plnum );



//-------------------------------------
///	�ؒf�G���[�`�F�b�N
//	Wi-Fi�L��̂Ƃ��ɂ������삵�܂���
//=====================================
extern BOOL MNGM_ERROR_CheckDisconnect( MNGM_ENRES_PARAM* p_commparam );
extern BOOL MNGM_ERROR_DisconnectWait( const MNGM_ENRES_PARAM* cp_commparam );



//-------------------------------------
///	START	TIMEUP	���[�N
//	H�u�����N�g�p���܂��B
//=====================================
extern MNGM_COUNTWK* MNGM_COUNT_Init( CLACT_SET_PTR p_clset, u32 heapID );
extern void MNGM_COUNT_Exit( MNGM_COUNTWK* p_wk );
extern void MNGM_COUNT_StartStart( MNGM_COUNTWK* p_wk );
extern void MNGM_COUNT_StartTimeUp( MNGM_COUNTWK* p_wk );
extern BOOL MNGM_COUNT_Wait( const MNGM_COUNTWK* p_wk );
extern int MNGM_PalNoGet( MNGM_COUNTWK* p_wk );

#endif		// __MINIGAME_TOOL_H__

