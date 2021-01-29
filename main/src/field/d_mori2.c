#ifdef PM_DEBUG
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_mori2.c
 *	@brief		�X����̃f�o�b�N���j���[�풓����
 *	@author		tomoya takahashi
 *	@data		2007.04.17
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "gflib/strbuf.h"
#include "gflib/strbuf_family.h"
#include "system/lib_pack.h"
#include "system/bmp_list.h"
#include "fieldmap.h"
#include "worldmap.h"
#include "fieldsys.h"
#include "field_event.h"
#include "system/fontproc.h"
#include "system/arc_util.h"
#include "system/window.h"
#include "fld_bmp.h"
#include "system/pm_str.h"
#include "fld_debug.h"
#include "talk_msg.h"
#include "encount_effect.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "msgdata/msg.naix"
#include "system/snd_play.h"
#include "system/snd_tool.h"
#include "savedata/savedata.h"
#include "savedata/wifilist.h"
#include "savedata/config.h"
#include "savedata/perapvoice.h"
#include "savedata/sodateyadata.h"
#include "sodateya.h"
#include "itemtool/item.h"

#include "msgdata/msg_debug_mori.h"
#include "application/namein.h"
#include "application/tradelist.h"
#include "communication/communication.h"

#include "communication/wpb.h"
#include "communication/wpbpokemon.h"
#include "communication/wpbutil.h"
#include  "communication/wm_icon.h"

#define __D_MORI2_H_GLOBAL
#include "d_mori2.h"

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


//-----------------------------------------------------------------------------
/**
 *		���O���͌Ăяo���e�X�g
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief ���O���͌Ăяo���e�X�g
 *
 *	@param	TCB_PTR tcb
 *	@param	work 
 *
 *	@return
 *
 *
 */
//-----------------------------------------------------------------------------
void TestNameIn_Call( TCB_PTR tcb, void* work ) 
{
	TEST_PROC_WORK* wk = work;
	
	switch( wk->seq ){
	case 0:		// �t�F�[�h�A�E�g
		FieldSystemProc_SeqHoldEnd();		
		wk->seq++;
		break;

	case 1:		// �t�B�[���h�j��
		if(wk->sub == 0){
			wk->sub++;

			// ���O���͗p�������[�N�쐬
			wk->NameInParam = NameIn_ParamAllocMake(HEAPID_WORLD, NAMEIN_MYNAME 
								/* NAMEIN_FRIENDNAME */ /*NAMEIN_FRIENDCODE*/ /*NAMEIN_POKEMON*/ , 
								2, NAMEIN_PERSON_LENGTH, SaveData_GetConfig(wk->fsys->savedata) );
//			wk->NameInParam = NameIn_ParamAllocMake(HEAPID_WORLD, NAMEIN_MYNAME, PM_MALE, 5);
//			PM_strcpy(wk->NameInParam->str,test_name);
			wk->NameInParam->info = MyStatus_GetMySex(SaveData_GetMyStatus(wk->fsys->savedata));
			wk->NameInParam->get_msg_id = 0;
			wk->NameInParam->sex = MyStatus_GetMySex(SaveData_GetMyStatus(wk->fsys->savedata));
			wk->NameInParam->boxdata = SaveData_GetBoxData(wk->fsys->savedata);
			OS_Printf("BOXDATA Addr = %08x\n",wk->NameInParam->boxdata);
			// ���O����PROC�쐬
			NameInput_SetProc( wk->fsys, wk->NameInParam );

			GameSystem_FinishFieldProc( wk->fsys );

		}else{

			// �I���������������`�F�b�N
			if( FieldEvent_Cmd_WaitMapProcStart( wk->fsys ) == FALSE ){
				wk->sub = 0;
				wk->seq++;
			}
		}
		break;

	case 2:		// ���O���͏�����
		wk->seq++;
		break;
	case 3:		// ���O���͏I���҂�
		if( GameSystem_CheckSubProcExists( wk->fsys ) == FALSE ){
			{
				int i;
				OS_Printf("���������O�� ");
				for(i=0;i<5;i++){
					OS_Printf("%x",wk->NameInParam->str[i]);
				}
				OS_Printf("\n");
				OS_Printf("�L�����Z���H = %d\n",wk->NameInParam->cancel);
			}
//			sys_FreeMemory( HEAPID_WORLD, wk->NameInParam );

			// ���������O��MYSTATUS�ɔ��f������
			if(!wk->NameInParam->cancel){
				MyStatus_SetMyName(SaveData_GetMyStatus(wk->fsys->savedata),wk->NameInParam->str);
			}
			NameIn_ParamDelete( wk->NameInParam );
			// �t�B�[���h���A
			GameSystem_CreateFieldProc( wk->fsys );
			wk->seq++;

		}
		break;
	case 4:
		if( GameSystem_CheckFieldProcExists(wk->fsys) ){
			FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKIN );
			// �^�X�N�I��
			TCB_Delete( tcb );
			sys_FreeMemoryEz( work );
		}
		break;
	}
}


//------------------------------------------------------------------
/**
 * �������X�g�Ăяo���e�X�g
 *
 * @param   tcb		
 * @param   work		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
void TestTradeList_Call( TCB_PTR tcb, void* work ) 
{
	TEST_PROC_WORK* wk = work;
	
	switch( wk->seq ){
	case 0:		// �t�F�[�h�A�E�g
		FieldSystemProc_SeqHoldEnd();		
		wk->seq++;
		break;

	case 1:		// �t�B�[���h�j��
		if(wk->sub == 0){
			wk->sub++;

			// �������X�gPROC�쐬
//			TradeList_SetProc( wk->fsys );
//			OekakiBoard_SetProc( wk->fsys );
			Field_MySign_SetProc( wk->fsys );

			GameSystem_FinishFieldProc( wk->fsys );

		}else{

			// �I���������������`�F�b�N
			if( FieldEvent_Cmd_WaitMapProcStart( wk->fsys ) == FALSE ){
				wk->sub = 0;
				wk->seq++;
			}
		}
		break;
	case 2:		// ���O���͏I���҂�
		if( GameSystem_CheckSubProcExists( wk->fsys ) == FALSE ){
			// �t�B�[���h���A
			GameSystem_CreateFieldProc( wk->fsys );
			wk->seq++;

		}
		break;
	case 3:
		if( GameSystem_CheckFieldProcExists(wk->fsys) ){
			FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKIN );
			// �^�X�N�I��
			TCB_Delete( tcb );
			sys_FreeMemoryEz( work );
		}
		break;
	}
}

//------------------------------------------------------------------
/**
 * ���E�����Ăяo���e�X�g
 *
 * @param   tcb		
 * @param   work		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
void TestWorldTrade_Call( TCB_PTR tcb, void* work ) 
{
	TEST_PROC_WORK* wk = work;
	
	switch( wk->seq ){
	case 0:		// �t�F�[�h�A�E�g
		FieldSystemProc_SeqHoldEnd();		
		wk->seq++;
		break;

	case 1:		// �t�B�[���h�j��
		if(wk->sub == 0){
			wk->sub++;

			// ���E����PROC�쐬
			Field_WorldTrade_SetProc( wk->fsys, 0 );

			GameSystem_FinishFieldProc( wk->fsys );

		}else{

			// �I���������������`�F�b�N
			if( FieldEvent_Cmd_WaitMapProcStart( wk->fsys ) == FALSE ){
				wk->sub = 0;
				wk->seq++;
			}
		}
		break;
	case 2:		// ���E�����I���҂�
		if( GameSystem_CheckSubProcExists( wk->fsys ) == FALSE ){
			// �t�B�[���h���A
			GameSystem_CreateFieldProc( wk->fsys );
			wk->seq++;

		}
		break;
	case 3:
		if( GameSystem_CheckFieldProcExists(wk->fsys) ){
			FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKIN );
			// �^�X�N�I��
			TCB_Delete( tcb );
			sys_FreeMemoryEz( work );
		}
		break;
	}
}
#endif // PM_DEBUG
