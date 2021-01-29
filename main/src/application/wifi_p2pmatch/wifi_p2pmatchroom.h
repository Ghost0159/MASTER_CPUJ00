//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_p2pmatchroom.h
 *	@brief		wifi	�}�b�`���O���[��
 *	@author		tomoya takahashi
 *	@data		2007.01.31
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_P2PMATCHROOM_H__
#define __WIFI_P2PMATCHROOM_H__

#include "clact.h"
#include "bg_system.h"
#include "system/arc_util.h"
#include "application/wifi_2dmap/wf2dmap_map.h"
#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_judge.h"
#include "application/wifi_2dmap/wf2dmap_objdraw.h"
#include "application/wifi_2dmap/wf2dmap_scroll.h"
#include "application/wifi_2dmap/wf2dmap_scrdraw.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

#include "wifi_p2pmatchroom_map.h"


#undef GLOBAL
#ifdef	__WIFI_P2PMATCHROOM_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
enum{	// �Ǘ����\�[�X���
	MCR_CLACT_RESCHAR,
	MCR_CLACT_RESPLTT,
	MCR_CLACT_RESCELL,
	MCR_CLACT_RESCELLANM,
	MCR_CLACT_RESNUM
};
#define MCR_CLACT_LOADRESNUM	(16)// �ǂݍ��ރ��\�[�X�̎��
#define MCR_CLACT_OBJNUM	(96)// �Ǘ��I�u�W�F�N�g��

// �l���I�u�W�F�N�g�p
#define MCR_MOVEOBJNUM		(48)// ����I�u�W�F�N�g��

// �����_���[�n
#define MCR_CLACTSUBSURFACE_Y	(800*FX32_ONE)	// �T�u���[�ӂ��[��


// �O���ւ̖߂�l
enum{
	MCR_RET_NONE,		// �Ȃ�
	MCR_RET_CANCEL,		// �L�����Z��
	MCR_RET_SELECT,		// �����I��
	MCR_RET_MYSELECT,	// �����I��
};

//-------------------------------------
///	NPC����萔
//=====================================
typedef enum {
	MCR_NPC_MOVE_NORMAL,
	MCR_NPC_MOVE_JUMP,
	MCR_NPC_MOVE_NUM,
} MCR_NPC_MOVETYPE;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�l�����[�N
//=====================================
typedef struct _WIFI_MATCHROOM WIFI_MATCHROOM;

typedef struct _MCR_MOVEOBJ{

	WF2DMAP_OBJWK* p_obj;
	WF2DMAP_OBJDRAWWK* p_draw;

	s16	move_count;	// �G�t�F�N�g�p�J�E���^
	u8 accesFriend;// �A�N�Z�X���Ă���F�BNo	friendNo�ƈꏏ�Ȃ�N�Ƃ�acces���Ă��Ȃ�
	u8 moveID;			// NPC���݂̓���
	u16 move_st;	// ���̓���
	u16 move_st_tmp;// ��ԕۑ���
	
	// �R�}���h���N�G�X�g
	BOOL (*pMove)( WIFI_MATCHROOM* p_mcr, struct _MCR_MOVEOBJ* p_obj );
	BOOL (*pMoveTmp)( WIFI_MATCHROOM* p_mcr, struct _MCR_MOVEOBJ* p_obj );
	
	// ����\��
	void (*pDraw)( WIFI_MATCHROOM* p_mcr, struct _MCR_MOVEOBJ* p_obj );
} MCR_MOVEOBJ;

//-------------------------------------
///	�G�t�F�N�g���[�N
//=====================================
typedef struct {
	CLACT_U_RES_OBJ_PTR resobj[4];	// �G�t�F�N�g���\�[�X�o�b�t�@
	CLACT_HEADER header;
	CLACT_WORK_PTR exit_cursor;
	CLACT_WORK_PTR obj_waku;
} MCR_EFFECT;

//-------------------------------------
///	�Z���A�N�^�[���[�N
//=====================================
typedef struct {
	CLACT_SET_PTR 			clactSet;						// �Z���A�N�^�[�Z�b�g
	CLACT_U_EASYRENDER_DATA	renddata;						// �ȈՃ����_�[�f�[�^
	CLACT_U_RES_MANAGER_PTR	resMan[MCR_CLACT_RESNUM];		// �L�����E�p���b�g���\�[�X�}�l�[�W��
	MCR_EFFECT effect;	// �G�t�F�N�g
} MCR_CLACT; 


//-------------------------------------
///	�p�\�R���A�j��
//=====================================
typedef struct {
	void* p_plbuff;
	NNSG2dPaletteData* p_pltt;
	u8 all_pcbitmap;	// �ҋ@�A�j��������PC�̃r�b�g�}�b�v
	u8 all_seq;			// �ҋ@�A�j���V�[�P���X
	s16 all_count;		// �ҋ@�A�j���J�E���^
	u8 use_pc;			// �g�pPC
	u8 use_pc_seq;		// �g�pPC�A�j���V�[�P���X
	s16 use_pc_count;	// �g�pPC�A�j���J�E���^
	u8 use_pc_next;		// ���ɐi��łق����Ƃ��̃t���O
	u8 pad[3];
} MCR_PCANM;



//-------------------------------------
///	WIFI�}�b�`���O���[���\����
//=====================================
typedef struct _WIFI_MATCHROOM{
	BOOL	init;			// �����������p
	u32 use_heap;			// �g�p����q�[�v
	u32 seq;				// �V�[�P���X
	BOOL player_pause;		// �v���C���[�̈ړ����~������
	GF_BGL_INI*	p_bgl;		// BGL
	MCR_CLACT				clact;		// �A�N�^�[���
	MCR_MOVEOBJ				moveObj[MCR_MOVEOBJNUM];		// ����I�u�W�F�N�g
	MCR_MOVEOBJ*			p_player;	// ��l���͓��ʂɎ���Ă���		
	MCR_PCANM				pc_anm;		// pc�A�j���[�V����

	WF2DMAP_MAPSYS*		p_mapsys;
	WF2DMAP_OBJSYS*		p_objsys;
	WF2DMAP_OBJDRAWSYS* p_objdraw;
	WF2DMAP_SCROLL		scroll;
	WF2DMAP_SCRDRAW*	p_scrdraw;
	WF2DMAP_REQCMDQ*	p_reqcmdQ;	// ���N�G�X�g�R�}���h�L���[

} WIFI_MATCHROOM;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL void WIFI_MCR_Init( WIFI_MATCHROOM* p_mcr, u32 heapID, ARCHANDLE* p_handle, GF_BGL_INI* p_bgl, u32 hero_view, u32 friendNum );
GLOBAL void WIFI_MCR_Dest( WIFI_MATCHROOM* p_mcr );
GLOBAL BOOL WIFI_MCR_GetInitFlag( const WIFI_MATCHROOM* cp_mcr );

GLOBAL u32 WIFI_MCR_Main( WIFI_MATCHROOM* p_mcr );
GLOBAL void WIFI_MCR_Draw( WIFI_MATCHROOM* p_mcr );
GLOBAL u8 WIFI_MCR_PlayerSelect( const WIFI_MATCHROOM* cp_mcr );
GLOBAL u32 WIFI_MCR_GetPlayerOnMapParam( const WIFI_MATCHROOM* cp_mcr );
GLOBAL u32 WIFI_MCR_GetPlayerOnUnderMapParam( const WIFI_MATCHROOM* cp_mcr );
GLOBAL void WIFI_MCR_CursorOn( WIFI_MATCHROOM* p_mcr, const MCR_MOVEOBJ* cp_obj );
GLOBAL void WIFI_MCR_CursorOff( WIFI_MATCHROOM* p_mcr );
GLOBAL void WIFI_MCR_PlayerMovePause( WIFI_MATCHROOM* p_mcr, BOOL flag );
GLOBAL BOOL WIFI_MCR_PlayerMovePauseGet( const WIFI_MATCHROOM* cp_mcr );


// �I�u�W�F�N�g�֘A
GLOBAL MCR_MOVEOBJ* WIFI_MCR_SetPlayer( WIFI_MATCHROOM* p_mcr, u32 view );
GLOBAL MCR_MOVEOBJ* WIFI_MCR_SetNpc( WIFI_MATCHROOM* p_mcr, u32 view, u8 friendNo );
GLOBAL void WIFI_MCR_DelPeopleReq( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );
GLOBAL void WIFI_MCR_DelPeople( MCR_MOVEOBJ* p_obj );

GLOBAL u8	WIFI_MCR_GetFriendNo( const MCR_MOVEOBJ* cp_obj );
GLOBAL u32	WIFI_MCR_GetView( const MCR_MOVEOBJ* cp_obj );
GLOBAL WF2DMAP_WAY	WIFI_MCR_GetWay( const MCR_MOVEOBJ* cp_obj );
GLOBAL WF2DMAP_WAY	WIFI_MCR_GetRetWay( const MCR_MOVEOBJ* cp_obj );

// NPC����
GLOBAL void WIFI_MCR_NpcPauseOn( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, WF2DMAP_WAY way );
GLOBAL void WIFI_MCR_NpcPauseOff( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj );

// NPC����ݒ�
GLOBAL void WIFI_MCR_NpcMoveSet( WIFI_MATCHROOM* p_mcr, MCR_MOVEOBJ* p_obj, MCR_NPC_MOVETYPE moveID );

// PC�A�j���ݒ�
GLOBAL void WIFI_MCR_PCAnmStart( WIFI_MATCHROOM* p_mcr );
GLOBAL void WIFI_MCR_PCAnmOff( WIFI_MATCHROOM* p_mcr );
GLOBAL void WIFI_MCR_PCAnmMain( WIFI_MATCHROOM* p_mcr );

#undef	GLOBAL
#endif		// __WIFI_P2PMATCHROOM_H__

