//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_taya2.c
 *	@brief		�c�J����@�f�o�b�N���j���[	�풓����
 *	@author		tomoya takahashi
 *	@data		2007.04.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


#include "common.h"
#include "system/lib_pack.h"
#include "system/bmp_list.h"
#include "system/wipe.h"
#include "fieldmap.h"
#include "fieldsys.h"
#include "system/fontproc.h"
#include "fld_bmp.h"
#include "system/pm_str.h"
#include "system/pm_overlay.h"
#include "field_event.h"
#include "system\palanm.h"
#include "application\pms_input.h"
#include "system\pm_rtc.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_debug_taya.h"

#define __D_TAYA2_H_GLOBAL
#include "d_taya2.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


BOOL GMEVENT_ChangeScheneDebug(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK* fsys;
	DEBUG_CHANGESCHENE_WORK* wk;

//	if( PaletteFadeCheck() != 0 ){
//		return FALSE;
//	}

	fsys = FieldEvent_GetFieldSysWork(event);
	wk = FieldEvent_GetSpecialWork(event);

	switch( wk->seq ){
	case 0:
		FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKOUT );
		wk->seq++;
		/* fallthru */
	case 1:
		if( WIPE_SYS_EndCheck() )
		{
			wk->start_func( fsys, wk->gen_wk );
			wk->seq++;
		}
		break;
	case 2:
		if (FieldEvent_Cmd_WaitSubProcEnd(fsys) == FALSE) {
			FieldEvent_Cmd_SetMapProc(fsys);
			wk->seq++;
		}
		break;
	case 3:
		if (FieldEvent_Cmd_WaitMapProcStart(fsys) == FALSE) {
			FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKIN );
			wk->seq++;
		}
		break;
	case 4:
		if( WIPE_SYS_EndCheck() )
		{
			if( wk->end_func )
			{
				wk->end_func( fsys, wk->gen_wk );
			}
			sys_FreeMemoryEz(wk);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

