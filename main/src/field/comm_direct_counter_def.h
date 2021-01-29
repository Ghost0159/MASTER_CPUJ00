//=============================================================================
/**
 * @file	comm_direct_counter_def.h
 * @bfief	�ʐM�_�C���N�g�R�[�i�[�̃J�E���^�[ �X�N���v�g�Ƌ��p�̒�`
 * @author	katsumi ohno
 * @date	05/08/04
 */
//=============================================================================

#ifndef __COMM_DIRECT_COUNTER_DEF_H__
#define __COMM_DIRECT_COUNTER_DEF_H__


#define DBC_TIM_BATTLE_MPDS (3)  // �ΐ핔��->�o�g��
#define DBC_TIM_BATTLE_DSOK (4)  // �o�g�����[�h�ɂȂ���
#define DBC_TIM_BATTLE_MPOK (5)  // �������[�h�ɂȂ���


#define DBC_TIM_BATTLE_BACK (30)    // �ΐ핔���ҋ@��Ԉʒu�ړ�����
#define DBC_TIM_BATTLE_PAUSE (92)    // �ΐ핔���ҋ@���
#define DBC_TIM_BATTLE_START (93)    // �ΐ핔���I�� > �o�g��
#define DBC_TIM_BATTLE_EXIT  (94)    // �ΐ핔���o�čs���Ƃ�
#define DBC_TIM_BATTLE_EXIT2  (91)    // �ΐ핔���o�čs���Ƃ�  �������

#define DBC_TIM_BATTLE_TR (95)    // �ΐ핔���g���[�i�[�J�[�h�����ҋ@
#define DBC_TIM_BATTLE_ROOMIN	(96) // �ΐ핔���ɓ���Ƃ�
#define DBC_TIM_BATTLE_DSCHANGE (97)  // �f�[�^�`���؂�ւ�
#define DBC_TIM_BATTLE_DSEND (98) // �؂�ւ�����
//-----------------------------------------------------------------------------
//�X�e�[�W
//-----------------------------------------------------------------------------
#define DBC_TIM_STAGE_POKETYPE_SEL	(100)	// �|�P�����^�C�v�I��O�̓���
#define DBC_TIM_STAGE_BATTLE		(101)	// �퓬�O�̓���
#define DBC_TIM_STAGE_MENU			(102)	// �Â���I�񂾌�̓���
#define DBC_TIM_STAGE_TR_ENEMY		(103)	// �g���[�i�[�ƃ|�P������ʐM�I���̓���
#define DBC_TIM_STAGE_END			(104)	// �^�C�v�I����ʏI���̓���
#define DBC_TIM_STAGE_LOSE_END		(105)	// �퓬�����I���̓���
#define DBC_TIM_STAGE_SAVE_AFTER	(106)	// �Z�[�u��̃}�b�v�J�ڑO�̓���
#define DBC_TIM_STAGE_GIVE			(107)	// ��߂�I�񂾌�̓���
//d32r0401.ev
#define STAGE_COMM_MULTI_POKE_SELECT	(108)
#define STAGE_COMM_MULTI_AUTO_SAVE		(109)
#define STAGE_COMM_MULTI_GO_SAVE		(110)
//
#define DBC_TIM_STAGE_INIT			(111)	// �ʐM�������̌�̓���
#define DBC_TIM_STAGE_TYPE_INIT		(112)	// �ʐM��������̃^�C�v�I����ʂ̓���
//-----------------------------------------------------------------------------
//�L���b�X��
//-----------------------------------------------------------------------------
#define DBC_TIM_CASTLE_RENTAL		(120)	// �����^����ʂ֍s���O�̓���
#define DBC_TIM_CASTLE_BATTLE		(121)	// �퓬�O�̓���
#define DBC_TIM_CASTLE_MENU			(122)	// �Â����I�񂾌�̓���
#define DBC_TIM_CASTLE_INIT			(123)	// �ʐM�������̌�̓���
#define DBC_TIM_CASTLE_GIVE			(124)	// ��߂��I�񂾌�̓���
#define DBC_TIM_CASTLE_POKE_DATA	(125)	// �|�P�����f�[�^���M�O�̓���
#define DBC_TIM_CASTLE_CHANGE		(126)	// �ʉ�ʂɍs���O�̓���
#define DBC_TIM_CASTLE_MENU_BEFORE	(127)	// ���j���[�\���O�̓���
#define DBC_TIM_CASTLE_LOSE_END		(128)	// �퓬�����I���̓���
#define DBC_TIM_CASTLE_SAVE_AFTER	(129)	// �Z�[�u��̃}�b�v�J�ڑO�̓���
//�莝��
#define DBC_TIM_CASTLE_MINE_UP		(130)	// �莝���F�����N�A�b�v��̃��b�Z�[�W�I���̓���
#define DBC_TIM_CASTLE_MINE_END		(131)	// �莝���F��ʏI���̓���
#define DBC_TIM_CASTLE_MINE_CANCEL	(132)	// �莝���F�u�߂�v�L�����Z���̓���
//�G�g���[�i�[
#define DBC_TIM_CASTLE_ENEMY_UP		(133)	// �G�F�����N�A�b�v��̃��b�Z�[�W�I���̓���
#define DBC_TIM_CASTLE_ENEMY_END	(134)	// �G�F��ʏI���̓���
#define DBC_TIM_CASTLE_ENEMY_CANCEL	(135)	// �G�F�u�߂�v�L�����Z���̓���
//d32r0501.ev
#define CASTLE_COMM_MULTI_POKE_SELECT	(136)
#define CASTLE_COMM_MULTI_AUTO_SAVE		(137)
#define CASTLE_COMM_MULTI_GO_SAVE		(138)
#define CASTLE_COMM_MULTI_SIO_END		(139)
//-----------------------------------------------------------------------------
//���[���b�g
//-----------------------------------------------------------------------------
#define DBC_TIM_ROULETTE_RENTAL			(140)	// �g���[�i�[�o��O�̓���
#define DBC_TIM_ROULETTE_BATTLE			(141)	// �퓬�O�̓���
#define DBC_TIM_ROULETTE_MENU			(142)	// �Â����I�񂾌�̓���
#define DBC_TIM_ROULETTE_CALL			(143)	// ���[���b�g��ʂ֍s���O�̓���
#define DBC_TIM_ROULETTE_GIVE			(144)	// ��߂��I�񂾌�̓���
#define DBC_TIM_ROULETTE_INIT			(145)	// �ʐM�������̌�̓���
#define DBC_TIM_ROULETTE_CHANGE			(146)	// �ʉ�ʂɍs���O�̓���
#define DBC_TIM_ROULETTE_BTL_WIN_PANEL	(147)	// �퓬�����p�l���̓���
#define DBC_TIM_ROULETTE_LOSE_END		(148)	// �퓬�����I���̓���
#define DBC_TIM_ROULETTE_PANEL_INFO		(149)	// �p�l���̐�����̓���
#define DBC_TIM_ROULETTE_SAVE_AFTER		(150)	// �Z�[�u��̃}�b�v�J�ڑO�̓���
//�����N
#define DBC_TIM_ROULETTE_UP				(151)	// �����N�F�����N�A�b�v��̃��b�Z�[�W�I���̓���
#define DBC_TIM_ROULETTE_END			(152)	// �����N�F��ʏI���̓���
#define DBC_TIM_ROULETTE_CANCEL			(153)	// �����N�F�u�߂�v�L�����Z���̓���
//d32r0601.ev
#define ROULETTE_COMM_MULTI_POKE_SELECT	(154)
#define ROULETTE_COMM_MULTI_AUTO_SAVE	(155)
#define ROULETTE_COMM_MULTI_GO_SAVE		(156)
#define ROULETTE_COMM_MULTI_SIO_END		(157)
//
#define DBC_TIM_ROULETTE_POKE_DATA		(158)	// �|�P�����f�[�^���M�O�̓���
//-----------------------------------------------------------------------------
//�t�@�N�g���[
//-----------------------------------------------------------------------------
#define DBC_TIM_FACTORY_INIT			(159)	// �ʐM�������̌�̓���
#define DBC_TIM_FACTORY_RENTAL			(160)	// �����^����ʂ֍s���O�̓���
#define DBC_TIM_FACTORY_BATTLE			(161)	// �퓬�O�̓���
#define DBC_TIM_FACTORY_MENU			(162)	// �Â���I�񂾌�̓���
#define DBC_TIM_FACTORY_TRADE			(163)	// ������ʂ֍s���O�̓���
#define DBC_TIM_FACTORY_RENTAL_END		(164)	// �����^����ʏI���̓���
#define DBC_TIM_FACTORY_TRADE_END		(165)	// ������ʏI���̓���
#define DBC_TIM_FACTORY_LOSE_END		(166)	// �퓬�����I���̓���
#define DBC_TIM_FACTORY_SAVE_AFTER		(167)	// �Z�[�u��̃}�b�v�J�ڑO�̓���
#define DBC_TIM_FACTORY_GIVE			(168)	// ��߂�I�񂾌�̓���
//d32r0301.ev
#define FACTORY_COMM_MULTI_AUTO_SAVE	(169)
//-----------------------------------------------------------------------------
//�t�����e�B�AWiFi��t
//-----------------------------------------------------------------------------
#define DBC_TIM_FR_WIFI_COUNTER_YAMERU	(170)	// �u��߂�v�I��������̓���
#define DBC_TIM_FR_WIFI_COUNTER_GO		(171)	// �e�{�݂֍s���O�̓���
#define DBC_TIM_FR_WIFI_COUNTER_END		(172)	// ���������V�΂Ȃ��ŏI�����鎞�̓���
#define DBC_TIM_FR_WIFI_COUNTER_GO2		(173)	// �e�{�݂֍s���O�̍ŏI����
#define DBC_TIM_FR_WIFI_COUNTER_TOWER	(174)	// �^���[�̃f�[�^�����O�̓���
#define DBC_TIM_FR_WIFI_COUNTER_SAVE	(175)	// �Z�[�u�O�̓���
#define DBC_TIM_FR_WIFI_COUNTER_INIT_1	(176)	// �ʐM�������̌�̓���
#define DBC_TIM_FR_WIFI_COUNTER_INIT_2	(177)	// �ʐM�������̌�̓���
#define DBC_TIM_FR_WIFI_COUNTER_END_2	(178)	// �N���u�ɖ߂�O�̓���
//-----------------------------------------------------------------------------
//�ʐM�R�}���h��������ɓ���
//-----------------------------------------------------------------------------
#define DBC_TIM_ROULETTE_INIT_1			(180)	// �ʐM�������̌�̓���
#define DBC_TIM_ROULETTE_INIT_2			(181)	// �ʐM�������̌�̓���
#define DBC_TIM_ROULETTE_INIT_3			(182)	// �ʐM�������̌�̓���
#define DBC_TIM_ROULETTE_INIT_4			(183)	// �ʐM�������̌�̓���
#define DBC_TIM_ROULETTE_INIT_5			(199)	// �ʐM�������̌�̓���(��p���)

#define DBC_TIM_STAGE_INIT_1			(185)	// �ʐM�������̌�̓���
#define DBC_TIM_STAGE_INIT_2			(186)	// �ʐM�������̌�̓���
#define DBC_TIM_STAGE_INIT_3			(187)	// �ʐM�������̌�̓���
#define DBC_TIM_STAGE_INIT_4			(188)	// �ʐM�������̌�̓���

#define DBC_TIM_CASTLE_INIT_1			(190)	// �ʐM�������̌�̓���
#define DBC_TIM_CASTLE_INIT_2			(191)	// �ʐM�������̌�̓���
#define DBC_TIM_CASTLE_INIT_3			(192)	// �ʐM�������̌�̓���
#define DBC_TIM_CASTLE_INIT_4			(193)	// �ʐM�������̌�̓���

#define DBC_TIM_FACTORY_INIT_1			(195)	// �ʐM�������̌�̓���
#define DBC_TIM_FACTORY_INIT_2			(196)	// �ʐM�������̌�̓���
#define DBC_TIM_FACTORY_INIT_3			(197)	// �ʐM�������̌�̓���
#define DBC_TIM_FACTORY_INIT_4			(198)	// �ʐM�������̌�̓���

#define DBC_TIM_CASTLE_MINE_CALL		(240)	// �����ĉ�ʂ֐i�ޑO�̓���
#define DBC_TIM_CASTLE_ENEMY_CALL		(241)	// �����ĉ�ʂ֐i�ޑO�̓���
//-----------------------------------------------------------------------------
//���[���b�g
//-----------------------------------------------------------------------------
#define DBC_TIM_ROULETTE_CALL_2			(200)	// ��p��ʂɍs���O�ɍēx�̓���
#define DBC_TIM_ROULETTE_PANEL			(201)	// �p�l���I�o��̓���
#define DBC_TIM_ROULETTE_BOTH_0			(202)	// ��ʃf�[�^off�O�̓���
#define DBC_TIM_ROULETTE_BASIC			(203)	// ��{�f�[�^���M�O�̓���
#define DBC_TIM_ROULETTE_TR				(204)	// �g���[�i�[�f�[�^���M�O�̓���
#define DBC_TIM_ROULETTE_ENEMY_BEFORE	(205)	// �G�g���[�i�[���M�O�̓���
#define DBC_TIM_ROULETTE_ENEMY_AFTER	(206)	// �G�g���[�i�[���M��̓���
//-----------------------------------------------------------------------------
//�L���b�X��
//-----------------------------------------------------------------------------
#define DBC_TIM_CASTLE_BOTH_0			(210)	// ��ʃf�[�^off�O�̓���
#define DBC_TIM_CASTLE_BASIC_AFTER		(211)	// ��{��񑗐M��̓���
#define DBC_TIM_CASTLE_TR_BEFORE		(212)	// �g���[�i�[�f�[�^���M�O�̓���
#define DBC_TIM_CASTLE_ENEMY_BEFORE		(213)	// �G�|�P���������O�̓���
#define DBC_TIM_CASTLE_ENEMY_AFTER		(214)	// �G�|�P����������̓���
#define DBC_TIM_CASTLE_MINE_INIT		(215)	// �ʐM��������̎莝����ʂ̓���
#define DBC_TIM_CASTLE_ENEMY_INIT		(216)	// �ʐM��������̂����ĉ�ʂ̓���
//-----------------------------------------------------------------------------
//�X�e�[�W
//-----------------------------------------------------------------------------
#define DBC_TIM_STAGE_BOTH_1			(220)	// ��ʃf�[�^on�O�̓���
#define DBC_TIM_STAGE_BOTH_0			(221)	// ��ʃf�[�^off�O�̓���
#define DBC_TIM_STAGE_TR_AFTER			(222)	// �g���[�i�[�f�[�^���M��̓���
#define DBC_TIM_STAGE_INIT_BEFORE		(223)	// �ʐM�������̑O�̓���
#define DBC_TIM_STAGE_TYPE_SEL_CALL		(224)	// �^�C�v�I����ʂ֐i�ޑO�̓���
//-----------------------------------------------------------------------------
//�t�@�N�g���[
//-----------------------------------------------------------------------------
#define DBC_TIM_FACTORY_BASIC_AFTER		(230)	// ��{��񑗐M��̓���
#define DBC_TIM_FACTORY_TR_AFTER		(231)	// �g���[�i�[�f�[�^���M��̓���
#define DBC_TIM_FACTORY_ENEMY_AFTER		(232)	// �G�|�P�������M��̓���
#define DBC_TIM_FACTORY_TRADE_AFTER		(233)	// �����I����̓���
#define DBC_TIM_FACTORY_RENTAL_AFTER	(234)	// �����^���I����̓���
#define DBC_TIM_FACTORY_RENTAL_CALL		(235)	// �����^����ʂ֐i�ޑO�̓���
#define DBC_TIM_FACTORY_TRADE_CALL		(236)	// �����^����ʂ֐i�ޑO�̓���
#define DBC_TIM_FACTORY_START_INIT		(237)	// �ʐM��������̃����^��������ʂ̓���
#define DBC_TIM_FACTORY_ENEMY_BEFORE	(238)	// �G�|�P�������M�O�̓���


#define REGULATION_RET_NONE  (3)   // ���M�����[�V��������Ȃ�
#define REGULATION_RET_OK  (1)   // ���M�����[�V��������
#define REGULATION_RET_END (2)   // ���M�����[�V������I�������ɏI��


#endif //__COMM_DIRECT_COUNTER_DEF_H__
