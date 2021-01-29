//==============================================================================
/**
 * @file	pl_bugfix.h
 * @brief	�v���`�i�F�o�O�Ώ��p��`
 * @author	matsuda
 * @date	2008.07.06(��)
 *
 * 7��7���̃m�[�^�b�`�f�o�b�O�ȍ~�ŏC�����ꂽ�o�O�̗L���E������`
 * ��`�̗L���E�����łȂ���`�l���O�̏ꍇ�͖����A�P�̏ꍇ�͗L���Ƃ��Ē�`���邱��
 */
//==============================================================================
#ifndef	__PL_BUGFIX_H__
#define	__PL_BUGFIX_H__


//--------------------------------------------------------------
//	�o�g��		PL_B0000_080707_FIX		BTS�ԍ��F�C���������t
//	�V�i���I	PL_S0000_080707_FIX		BTS�ԍ��F�C���������t
//	�ʐM		PL_T0000_080707_FIX		BTS�ԍ��F�C���������t
//	�Г�		PL_G0000_080707_FIX		BTS�ԍ��F�C���������t
//--------------------------------------------------------------

/* �s�v�c�ȑ��蕨�ŗ]�v�ȃ��b�Z�[�W�҂�������(3sec) */
#define	PL_T0852_080709_FIX		( 0 )	///< comm_mystery_gift.c


/*
 * T0849	���U���g��ʂŃ^�b�`�y���^�̊e�v���C���[�̕\���������
 * �ꏊ	src/application/wifi_lobby/minigame_tool.c
 */
#define PL_T0849_080709_FIX	(1)


/*
 * T0834	�^�b�`�g�C���삪���v���C���[�̃^�b�`�g�C����ɉe����^����
 * �ꏊ	src/application/wifi_lobby/wflby_ev_def.c
 */
#define PL_T0834_080709_FIX	(0)


/*
 * S0789	�����|�P���������炤���O�ɃZ�[�u���s���A��x�d���𗎂Ƃ�����ɂ�
 *			�炨���Ƃ���Ƃ��̍ۂɕ\�������e�L�X�g��������ł��܂���
 * �ꏊ	src/demo/ev_poke_select.c
 */
#define PL_S0789_080710_FIX	(0)


/*
 * T0855	DL�o�g���^���[�Ő��т����f����Ȃ�
 * �ꏊ	src/frontier/fssc_tower.c
 */
#define PL_T0855_080710_FIX	(0)

/*
 * T0856	�Q���l����RingBuff�̐l�������v���Ă��Ȃ��ꍇ�̑Ώ��Ƃ���Ring�������݂�NULL������ǉ�
 * �ꏊ	src/communication/comm_ring_buff.c
 */
#define PL_T0856_080710_FIX  (0)

/*
 * G0197 	DP����PL�Ɂu��������v��\�����ނƂƂ܂�
 *�@BTS_T0856�ƁABTS_G0197�́A
 *	��M�o�b�t�@�� DWC_SetRecvBuffer�łc�v�b���C�u�����ɐݒ肷��^�C�~���O��
 *	���Ŕ�������o�O�ł��B
 *
 *	���������s�����Ă��邽�߁A DWCMatchedSCCallback�Ń}�b�`���O�������������Ƃ��m�F������ɁA
 *	��M�o�b�t�@�̃��������m�ۂ��A���C�u�����ɐݒ肷��悤�ɏ�����g��ł���܂��B
 *	���̕s�����������^�C�~���O�ł́A�c�v�b���C�u�������f�[�^��M���̏�ԂŁA�o�b�t�@��ݒ肵�悤�Ƃ��āA
 *	�o�b�t�@��ݒ�ł����A�c�v�b���C�u�������ȑO�̎�M�o�b�t�@�̂܂܁A���̌�����삵�Ă��܂��܂��B
 *	�܂��A�}�b�`���O�����܂łɂ����C�u�������ł̓R�}���h����M���Ă��邽�߁A���̕����ł��������j�󂪂����Ă���\��������܂��B
 *
 *	���̖�����������ɂ́A�}�b�`���O���J�n����O�ɁA��M�o�b�t�@��ݒ肷��悤�ɏC�����܂����B
 *	���̈׎擾���郁�����T�C�Y��0x1000byte�����Ă��܂��B
 *
 * �ꏊ	src/wifi/dwc_rap.c
 */
#define PL_G0197_080710_FIX	(0)

/*
 * T0857	DP�̕�W�ɑ΂��A�\�����݂��s�������ɃA�C�R���͕�W���e�̂��̂��\������Ă���̂�
 *			�{�C�X�`���b�g���n�܂��Ă��܂�
 *
 * T0858	DP�Ƃ̐ڑ����A��W���iPL�j������̃^�C�~���O�Ń{�C�X�`���b�g��
 *			ON/OFF��؂�ւ���Ǝ��ۂ̐ݒ�ƈقȂ�`�ł̋����ɂȂ��Ă��܂�
 *
 *	�ꏊ src/application/wifi_p2pmatch/wifi_p2pmatch.c
 */
#define PL_T0857_080711_FIX	(0)


/*
 * S0793	���܂��݂w�����b�Z�[�W�ŁA�]�v�ȑ���J�[�\�����\������Ă���
 * �ꏊ	src/fielddata/script/d13r0101.ev
 */
#define PL_S0793_080711_FIX	(0)

/*
 * S0792	
 * �J���i�M�^�E������210�΂�ǂ���Ɉړ���������Ɂu����΂炢�v��
 * �g���Ƃɂ񂶂Ⴒ�����P���L�`�̉B��Ă���ꏊ�̃O���t�B�b�N���Ȃ��Ȃ��Ă��܂�
 * �ꏊ src/field/fldeff_hide.c
 */
#define PL_S0792_080711_FIX (0)

/*
 * G0198
 * �}�ӂɓo�^����Ă��Ȃ��^�}�S�������āu��Ԃꂽ�������v�ɓ���ƁA�z�����Ă��Ȃɂ��ւ�炸
 * �}�ӓo�^����Ă��܂��B
 * �ꏊ src/field/scrcmd.c
 */
#define PL_G0198_080711_FIX (1)


/*
 * S0791	�A�N�Z�T���[�������܂łɁA�w�i�����炦�Ă��܂�
 * �ꏊ	src/fielddata/script/c01r0601.ev
 */
#define PL_S0791_080712_FIX	(1)


/*
 * S0798	�h��Ă���~�c�̖؂𒲂׃G���J�E���g���悤�Ƃ����Ƃ���łƂ܂邱
�Ƃ�����
 *			�Ƃ܂�̂́A�V�C�̕������������ɔj���������������邽��
 * 
 * �ꏊ src/field/weather_sys.c
 */
#define PL_S0798_080712_FIX (1)


/*
 * T0861	�o�^�m�F�e�L�X�g���^�b�`����ł���������ł��Ȃ�
 * 
 * �ꏊ src/application/wifi_note/pl_wifi_note.c
 */
#define PL_T0861_080712_FIX (0)


/*
 * T0862	�b������������̃v���t�B�[�����\������Ȃ����Ƃ�����
 * 
 * �ꏊ src/application/wifi_lobby/wflby_room.c
 */
#define PL_T0862_080712_FIX (0)


/*
 * T0864	�g���[�i�[�����L���O�ɏo���肵����A
 *			�W�I�l�b�g�ɓ���ƁA���b�Z�[�W�𑁑���ł��Ȃ�
 * 
 * �ꏊ src/application/wifi_earth/wifi_earth.c
 */
#define PL_T0864_080714_FIX (0)



/*
 * S0800	���̍s�����s������A�l�V���Ɛ키�ƃJ�b�g�C�����Ɏ��L�������S��
�f���Ă����J�������������Ɉړ����Ă��܂�����������
 * 
 * �ꏊ src/field/encount_effect.c
 */
#define PL_S0800_080714_FIX (0)


/*
 * T0866	��荞�ݒ��ŃL�����Z�����ꂽ����ɍēx�b�������Ď~�܂�
 * 
 * �ꏊ src/application/wifi_lobby/wflby_ev_talk.c
 */
#define PL_T0866_080715_FIX (1)

/*
 * T0867	���L�����Ƒ��̃v���C���[���d�Ȃ�����ԂŁu������ׂ�v���s�Ȃ��
��
 * 
 * �ꏊ src/application/wifi_lobby/wflby_ev_def.c
 */
#define PL_T0867_080716_FIX (0)

/*
 * T0394	�������A���őI�����Ă��Ȃ��Ƃ��ɂ������������Ƃ��ɑ��̏�Ԃ��������Ă��܂�
 *			�퓬�^��ɉe�����Ă��܂��̂ŁA�v���`�i�Ŕ��f����Ȃ��ꍇ�́A�����Ɣ��f���Ȃ��ł�������
 * 
 * �ꏊ src/battle/server.c
 */
#define PL_T0394_080716_FIX (0)

/*
 * S0802	�}�i�t�B�[�C�x���g�Ő}�ӂɓo�^����Ă���̂ɁA�}�ӂɓo�^���ꂽ���b�Z�[�W���\�������
 * 
 * �ꏊ src/fielddata/script/r212ar0103.ev
 *		src/fielddata/script/scr_seq_def.h
 *		src/field/scrcmd.c
 *		src/field/scr_field.c
 *		src/field/scr_field.h
 */
#define PL_S0802_080716_FIX (0)

/*
 * G0200	���l�̃v���t�B�[����\�����悤�Ƃ����t���[�����ŁA
���̐l�̑ގ��R�[���o�b�N���Ă΂��ƁA�m�t�k�k�Q�ƂłƂ܂�B 
 * 
 * �ꏊ src/application/wifi_lobby/wflby_room.c
 */
#define PL_G0200_080716_FIX (0)


/*
 * T0868	��������������M���鏈���ŁA�e�@���q�@�̌����������t���O���`�F�b�N���Ă��Ȃ����߁A
 *			��������[�N�A�󂯎�郏�[�N���q�@�̒l�ŏ㏑������Ă��܂�
 * 
 * �ꏊ src/frontier/factory/factory.c
 */
#define PL_T0868_080717_FIX (1)


#endif	//__PL_BUGFIX_H__
