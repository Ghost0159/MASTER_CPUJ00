//============================================================================================
/**
 * @file	plist_bmp.h
 * @brief	�|�P�������X�g���BMP�֘A
 * @author	Hiroyuki Nakamura
 * @date	05.09.29
 */
//============================================================================================
#ifndef	PLIST_BMP_H
#define	PLIST_BMP_H
#undef	GLOBAL
#ifdef	PLIST_BMP_H_GLOBAL
#define	GLOBAL /*	*/
#else
#define	GLOBAL extern
#endif


//============================================================================================
//	�萔��`
//============================================================================================
#define	PL_MSG_COMP_EXPAND	( 0xffffffff )	// �\�����郁�b�Z�[�W�����łɓW�J�ς�


//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * BMP�ǉ�
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListBmpAdd( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * BMP�E�B���h�E�j��
 *
 * @param	wk		�|�P�������X�g���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListBmpWinExit( PLIST_WORK * wk );


//--------------------------------------------------------------------------------------------
/**
 * ���O�쐬
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	pp		POKEMON_PARAM
 * @param	pos		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListNameMake( PLIST_WORK * wk, POKEMON_PARAM * pp, u32 pos );

//--------------------------------------------------------------------------------------------
/**
 * ���O�\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListNamePut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * HP�\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListHPPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * HP�\���N���A
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListHPClear( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * HP�Q�[�W�\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListHPBerPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * ���x���\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListLvPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �p�����[�^�S�̂�\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListParamPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �p�����[�^�S�̂�BMP��CGX��]��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListParamBmpCgxOn( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �p�����[�^�S�̂��\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListParamBmpWinOff( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �i���`�F�b�N�R�����g��\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_PanelShinkaCommPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �Z�o���`�F�b�N�R�����g��\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_PanelWazaOboeCommPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �R���e�X�g�Q���`�F�b�N�R�����g��\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_PanelContestCommPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * ���O�����x���\���i�퓬�Q���I��p�j
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_PanelBattleInNamePut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * �퓬�Q���`�F�b�N�R�����g��\��
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	num		���C�ڂ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_PanelBattleInCommPut( PLIST_WORK * wk, u8 num );

//--------------------------------------------------------------------------------------------
/**
 * ���j���[���ڕ�����擾
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListMenuStrMake( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * ���j���[�f�[�^�쐬
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	prm		�쐬�f�[�^
 * @param	siz		�T�C�Y
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListMenuDataMake( PLIST_WORK * wk, const u8 * prm, u8 siz );

//--------------------------------------------------------------------------------------------
/**
 * ���j���[���b�Z�[�W�쐬
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListMenuMsgMake( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �Z���j���[�̕�����擾
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 * @param	waza	�Z�ԍ�
 * @param	pos		�ʒu
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_WazaMenuStrGet( PLIST_WORK * wk, u16 waza, u8 pos );


//--------------------------------------------------------------------------------------------
/**
 * �u�����Ă��v�u���ǂ�v�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 * @param	flg		�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListSelectMsgPut( PLIST_WORK * wk, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * �P�s���b�Z�[�W�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 ` @param	id		���b�Z�[�WID
 * @param	flg		�E�B���h�E�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListSmallMsgPut( PLIST_WORK * wk, u32 id, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * �Q�s���b�Z�[�W�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 * @param	id		���b�Z�[�WID
 * @param	flg		�E�B���h�E�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListMiddleMsgPut( PLIST_WORK * wk, u32 id, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * ��b�i���j���b�Z�[�W�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 * @param	id		���b�Z�[�WID
 * @param	flg		�E�B���h�E�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeListLargeMsgPut( PLIST_WORK * wk, u32 id, u8 flg );

//--------------------------------------------------------------------------------------------
/**
 * ��b�i���j���b�Z�[�W�X�^�[�g
 *
 * @param	wk		�|�P�������X�g���[�N
 * @param	id		���b�Z�[�WID
 * @param	flg		�E�B���h�E�\���t���O
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_TalkMsgStart( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * �͂��E�������\��
 *
 * @param	wk		�|�P�������X�g���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_YesNoSelectInit( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * ���x���A�b�v�E�B���h�E�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_LvWinPut( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * ���x���A�b�v��̃p�����[�^�\��
 *
 * @param	wk		�|�P�������X�g���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_LvUpParamPut( PLIST_WORK * wk );

//--------------------------------------------------------------------------------------------
/**
 * ���x���A�b�v�E�B���h�E�폜
 *
 * @param	wk		�|�P�������X�g���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
GLOBAL void PokeList_LvUpWinExit( PLIST_WORK * wk );



#undef GLOBAL
#endif