//==============================================================================
/**
 * @file	footprint_tool.c
 * @brief	���Ճ{�[�h�̃c�[����
 * @author	matsuda
 * @date	2008.01.22(��)
 */
//==============================================================================
#include "common.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata\system_data.h"
#include "system/bmp_menu.h"
#include "system/procsys.h"
#include "system/wipe.h"
#include "system/window.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/fontproc.h"
#include "gflib/strbuf_family.h"
#include "msgdata/msg_wflby_footprint.h"
#include "system\font_arc.h"
#include "savedata/config.h"
#include "communication/wm_icon.h"
#include "system/fontoam.h"
#include "system/d3dobj.h"
#include "system/clact_tool.h"
#include "system/bmp_list.h"
#include "system/bmp_menu.h"
#include "gflib/strbuf_family.h"
#include "gflib/touchpanel.h"

#include "application/wifi_lobby/wflby_system_def.h"
#include "application/footprint_main.h"
#include "footprint_common.h"
#include "graphic/footprint_board.naix"
#include "footprint_stamp.h"
#include "footprint_comm.h"
#include "footprint_tool.h"
#include "footprint_snd_def.h"

//==============================================================================
//	�f�[�^
//==============================================================================
#include "footprint_control.dat"
#include "footprint_foot.dat"

///�C���N�p���b�g�^�b�`�p�l���̈�ݒ�
static const RECT_HIT_TBL InkPaletteTouchData[] = {
	//TOP BOTTOM LEFT RIGHT
	{160, 192, 32*0, 32*1},
	{160, 192, 32*1, 32*2},
	{160, 192, 32*2, 32*3},
	{160, 192, 32*3, 32*4},
	{160, 192, 32*4, 32*5},
	{160, 192, 32*5, 32*6},
	{160, 192, 32*6, 255},		//��߂�
	{ RECT_HIT_END, 0, 0, 0 }
};


//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[����`��
 *
 * @param   msgman			���b�Z�[�W�}�l�[�W���ւ̃|�C���^
 * @param   wordset			���[�h�Z�b�g�ւ̃|�C���^
 * @param   win[]			BMPWIN�ւ̃|�C���^
 * @param   wflby_sys		���C�t�@�C���r�[�V�X�e���ւ̃|�C���^
 * @param   user_index		�T�u�`�����l���̃��[�U�[ID
 */
//--------------------------------------------------------------
void FootPrintTool_NameDraw(MSGDATA_MANAGER *msgman, WORDSET *wordset, GF_BGL_BMPWIN win[], WFLBY_SYSTEM *wflby_sys, s32 user_id)
{
	const WFLBY_USER_PROFILE* wup;
	MYSTATUS *my_status;
	STRBUF *message_src;
	STRBUF *expand_src;
	STRBUF *name_src;
	int expand_len = 64;	//���[�h�Z�b�g�W�J�o�b�t�@�̃T�C�Y�B�K���ɑ傫��
	int sex;
	u32 user_index, main_idx;
	GF_PRINTCOLOR print_color;
	
	//�T�u�`�����l���̃��[�U�[ID���烁�C���̃��[�U�[Index���擾����(���̂�)
	user_index = DWC_LOBBY_SUBCHAN_GetUserIDIdx(user_id);
	if(user_index == DWC_LOBBY_USERIDTBL_IDX_NONE){
		return;
	}
	main_idx = DWC_LOBBY_GetUserIDIdx(user_id);
	if(main_idx == DWC_LOBBY_USERIDTBL_IDX_NONE){
		return;
	}
	
	OS_TPrintf("user_index = %d, main_idx = %d, user_id = %d\n", user_index, main_idx, user_id);
	//���C���̃��[�U�[Index����v���t�B�[�����擾
	wup = WFLBY_SYSTEM_GetUserProfile(wflby_sys, main_idx);
	if(wup == NULL){
		return;
	}
	
	if(user_index >= FOOTPRINT_BMPWIN_NAME_MAX){
		GF_ASSERT(0);
		return;
	}
	
	my_status = MyStatus_AllocWork(HEAPID_FOOTPRINT);
	WFLBY_SYSTEM_GetProfileMyStatus(wup, my_status, HEAPID_FOOTPRINT);
	name_src = MyStatus_CreateNameString(my_status, HEAPID_FOOTPRINT);
	sex = MyStatus_GetMySex(my_status);
	
	WORDSET_RegisterWord(wordset, 0, name_src, sex, TRUE, PM_LANG);
	message_src = MSGMAN_AllocString(msgman, msg_footprint_name);
	expand_src = STRBUF_Create(expand_len, HEAPID_FOOTPRINT);
	WORDSET_ExpandStr(wordset, expand_src, message_src);

	GF_BGL_BmpWinDataFill(&win[user_index], 0x00);
	if(WFLBY_SYSTEM_GetUserVipFlag(wflby_sys, main_idx) == TRUE){
		print_color = GF_PRINTCOLOR_MAKE(7,8,0);
	}
	else{
		print_color = GF_PRINTCOLOR_MAKE(1,2,0);
	}
	GF_STR_PrintColor(&win[user_index], FONT_SYSTEM, expand_src, 0, 0, 
		MSG_ALLPUT, print_color, NULL);
	GF_BGL_BmpWinOn(&win[user_index]);
	
	STRBUF_Delete(name_src);
	STRBUF_Delete(message_src);
	STRBUF_Delete(expand_src);
	
	sys_FreeMemoryEz(my_status);
}

//--------------------------------------------------------------
/**
 * @brief   �Q���Җ�����������
 *
 * @param   win[]			BMPWIN�ւ̃|�C���^
 * @param   user_index		���[�U�[index
 */
//--------------------------------------------------------------
void FootPrintTool_NameErase(GF_BGL_BMPWIN win[], u32 user_index)
{
	if(user_index >= FOOTPRINT_BMPWIN_NAME_MAX){
		GF_ASSERT(0);
		return;
	}
	GF_BGL_BmpWinOff(&win[user_index]);
}

//--------------------------------------------------------------
/**
 * @brief   �e��ID����A�X�^���v�̐F���擾����
 *
 * @param   board_type	�{�[�h�̎��(FOOTPRINT_BOARD_TYPE_???)
 * @param   oya_id		�e��ID
 *
 * @retval  �J���[�R�[�h
 */
//--------------------------------------------------------------
u16 FootprintTool_StampColorGet(int board_type, u32 oya_id)
{
	return StampColorTbl[oya_id % NELEMS(StampColorTbl)][board_type];
}

//--------------------------------------------------------------
/**
 * @brief   �C���N�p���b�g���^�b�`����Ă��邩�̍X�V����
 *
 * @param   my_stamp_array[]		�����̃X�^���v�p�����[�^�ւ̃|�C���^
 *
 * @retval  0�`5:�^�b�`���ꂽ�C���N�p���b�g or FOOT_TOUCH_RET_EXIT or FOOT_TOUCH_RET_NULL
 */
//--------------------------------------------------------------
int FootprintTool_InkPalTouchUpdate(STAMP_PARAM *my_stamp_array, int now_select_no)
{
	int hit;
	int ret = FOOT_TOUCH_RET_NULL;
	
	hit = GF_TP_RectHitTrg(InkPaletteTouchData);
	if(hit < 6){		//�C���N�p���b�g��������
		if(now_select_no != hit 
				&& my_stamp_array[hit].monsno != 0 && my_stamp_array[hit].monsno <= MONSNO_END){
			ret = hit;
		}
	}
	else if(hit == 6){	//�u��߂�v�{�^����������
		ret = FOOT_TOUCH_RET_EXIT;
	}
	else{
		return FOOT_TOUCH_RET_NULL;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ���Ղ��L��|�P�������`�F�b�N����
 *
 * @param   monsno		�|�P�����ԍ�
 * @param   form_no		�t�H�����ԍ�
 * @param   arceus_flg	�A���Z�E�X�t���O(TRUE:���JOK)
 *
 * @retval  TRUE:���Ղ�����
 * @retval  FALSE:���Ղ��Ȃ�
 */
//--------------------------------------------------------------
BOOL FootprintTool_FootDispCheck(int monsno, int form_no, BOOL arceus_flg)
{
	if(monsno == MONSNO_KIMAIRAN && form_no > 0){
		return FALSE;	//�t�H�����`�F���W�����M���e�B�i�����ʂɔ���
	}
	if(monsno == MONSNO_AUSU && arceus_flg == FALSE){
		return FALSE;	//���JOK�ɂȂ��Ă��Ȃ����̓A���Z�E�X�͑��Ֆ��������Ŕ���
	}
	
	return PokeFootConvTbl[monsno].disp;
}

//--------------------------------------------------------------
/**
 * @brief   ���Ղ��L��|�P�������`�F�b�N����
 *
 * @param   monsno		�|�P�����ԍ�
 * @param   form_no		�t�H�����ԍ�
 *
 * @retval  TRUE:���Ղ�����
 * @retval  FALSE:���Ղ��Ȃ�
 */
//--------------------------------------------------------------
BOOL FootprintTool_FootHitSizeGet(int monsno, int form_no)
{
	if(monsno == MONSNO_KIMAIRAN && form_no > 0){
		return 2;	//�t�H�����`�F���W�����M���e�B�i�����ʂɔ���
	}
	
	return PokeFootConvTbl[monsno].hit_size;
}
