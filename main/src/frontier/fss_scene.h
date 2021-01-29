//==============================================================================
/**
 * @file	fss_scene.h
 * @brief	
 * @author	matsuda
 * @date	2007.04.03(��)
 */
//==============================================================================
#ifndef __FSS_SCENE_H__
#define __FSS_SCENE_H__


//==============================================================================
//	�萔��`
//==============================================================================

//--------------------------------------------------------------
//	�V�[��ID��`(��SceneDataTbl�ƕ��т𓯂��ɂ��邱�ƁI)
//--------------------------------------------------------------
#define FSS_SCENEID_TESTSCR		(0)
#define FSS_SCENEID_TESTSCR2	(FSS_SCENEID_TESTSCR + 1)
#define FSS_SCENEID_TESTSCR3	(FSS_SCENEID_TESTSCR2 + 1)
//�t�@�N�g���[
#define FSS_SCENEID_FACTORY		(FSS_SCENEID_TESTSCR3 + 1)
#define FSS_SCENEID_FACTORY_BTL	(FSS_SCENEID_FACTORY + 1)
//�^���[
#define FSS_SCENEID_TOWER_START			(FSS_SCENEID_FACTORY_BTL + 1)		//�^���[�X�^�[�g
#define FSS_SCENEID_TOWER_SINGLE_WAY	(FSS_SCENEID_TOWER_START + 0)		//r203
#define FSS_SCENEID_TOWER_MULTI_WAY		(FSS_SCENEID_TOWER_START + 1)		//r204
#define FSS_SCENEID_TOWER_SINGLE_BTL	(FSS_SCENEID_TOWER_START + 2)		//r205
#define FSS_SCENEID_TOWER_MULTI_BTL		(FSS_SCENEID_TOWER_START + 3)		//r206
//�X�e�[�W
#define FSS_SCENEID_STAGE_START			(FSS_SCENEID_TOWER_MULTI_BTL + 1)	//�X�e�[�W�X�^�[�g
#define FSS_SCENEID_STAGE_WAY			(FSS_SCENEID_STAGE_START + 0)		//
#define FSS_SCENEID_STAGE_BTL			(FSS_SCENEID_STAGE_START + 1)		//
//�L���b�X��
#define FSS_SCENEID_CASTLE_START		(FSS_SCENEID_STAGE_BTL + 1)			//�L���b�X���X�^�[�g
#define FSS_SCENEID_CASTLE_ROOM			(FSS_SCENEID_CASTLE_START + 0)		//
#define FSS_SCENEID_CASTLE_BTL			(FSS_SCENEID_CASTLE_START + 1)		//
#define FSS_SCENEID_CASTLE_BYPATH		(FSS_SCENEID_CASTLE_START + 2)		//
//WiFi��t
#define FSS_SCENEID_WIFI_COUNTER		(FSS_SCENEID_CASTLE_BYPATH + 1)		//
//���[���b�g
#define FSS_SCENEID_ROULETTE_START		(FSS_SCENEID_WIFI_COUNTER + 1)		//���[���b�g�X�^�[�g
#define FSS_SCENEID_ROULETTE_WAY		(FSS_SCENEID_ROULETTE_START + 0)	//
#define FSS_SCENEID_ROULETTE_BTL		(FSS_SCENEID_ROULETTE_START + 1)	//

#define FSS_SCENEID_END			(0xfffe)
#define FSS_SCENEID_DEFAULT		(0xffff)	//�f�t�H���gID


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�V�[���f�[�^�ɑ��d�ʂ��g�p���Ȃ��ꍇ�Ɏw��
#define MULTI_BG_NULL		(0xffff)


//==============================================================================
//
//	
//
//==============================================================================
#ifndef	__ASM_NO_DEF_	// ������ȍ~�́A�A�Z���u���ł͖���

//--------------------------------------------------------------
//	�V�[���f�[�^�擾�R�[�h
//--------------------------------------------------------------
enum{
	FSS_SCENE_DATA_BG_MODE,		//��check �ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������
	FSS_SCENE_DATA_SCRID,
	FSS_SCENE_DATA_MSGID,
	FSS_SCENE_DATA_BGMID,
	FSS_SCENE_DATA_SCREEN_SIZE,
	FSS_SCENE_DATA_MAP_ARCID,
	FSS_SCENE_DATA_MAP_SCREENID,
	FSS_SCENE_DATA_MAP_CHARID,
	FSS_SCENE_DATA_MAP_PLTTID,
	FSS_SCENE_DATA_MULTI_MAP_SCREENID,
	FSS_SCENE_DATA_MULTI_MAP_CHARID,
	FSS_SCENE_DATA_MULTI_MAP_PLTTID,
	FSS_SCENE_DATA_SCROLL_MODE,
	FSS_SCENE_DATA_MULTI_SCROLL_MODE,
};

///�}�b�v�̃X�N���[�����샂�[�h
enum{
	SCROLL_MODE_NORMAL,		///<�ʏ�X�N���[������(WF2DMAP�V�X�e���g�p)
	SCROLL_MODE_EASY,		///<BG���W�̃X�N���[�������̂�(�X�N���[���]���A�j�����g�p����ꍇ)
};

//==============================================================================
//	�O���֐��錾
//==============================================================================
extern int FSS_SceneParamGet(int scene_id, int param_code);
extern void FSS_SceneInitFuncCall(FMAP_PTR fmap, void **ptr_work, int scene_id);
extern void FSS_SceneEndFuncCall(FMAP_PTR fmap, void **ptr_work, int scene_id);

#endif	//__ASM_NO_DEF_


#endif	//__FSS_SCENE_H__

