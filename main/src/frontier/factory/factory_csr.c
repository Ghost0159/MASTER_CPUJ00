//==============================================================================================
/**
 * @file	factory_csr.c
 * @brief	�u�o�g���t�@�N�g���[�v�J�[�\��
 * @author	Satoshi Nohara
 * @date	2007.03.20
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"

#include "factory_sys.h"
#include "factory_clact.h"
#include "factory_csr.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//�J�[�\��OBJ
struct _FACTORY_CSR{
	u8	sel_max;					//�I���̍ő吔
	u8	mode;						//���I�����A�c�I����
	u8	csr_pos;					//�J�[�\���ʒu
	u8	pause;						//�|�[�Y�t���O

	//POS* pos_tbl;					//�J�[�\�����ړ�������W�e�[�u���̃|�C���^
	const FACTORY_POS* pos_tbl;		//�J�[�\�����ړ�������W�e�[�u���̃|�C���^
	const u8* anm_tbl;				//�J�[�\���A�j���e�[�u���̃|�C���^
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^

	u8 sel_h_max;					//���I���̍ő吔(sel_max�̒��̂��������I����)
};

//�J�[�\���̃p���b�g
enum{
	//PAL_CSR_MOVE = 0,
	//PAL_CSR_STOP,
	PAL_CSR_MOVE = 6,
	PAL_CSR_STOP = 6,
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
FACTORY_CSR* FactoryCsr_Create( FACTORY_CLACT* factory_clact, u8 sel_max, u8 sel_h_max, u8 mode, u8 csr_pos, const FACTORY_POS* pos_tbl, const u8* anm_tbl );
void* FactoryCsr_Delete( FACTORY_CSR* wk );
void FactoryCsr_Vanish( FACTORY_CSR* wk, int flag );
void FactoryCsr_Move( FACTORY_CSR* wk );
u8 FactoryCsr_GetCsrPos( FACTORY_CSR* wk );
void FactoryCsr_Pause( FACTORY_CSR* wk, int flag );
void FactoryCsr_SetCsrPos( FACTORY_CSR* wk, u8 csr_pos );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ�쐬
 *
 * @param	factory_clact	FACTORY_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"FACTORY_CSR���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
FACTORY_CSR* FactoryCsr_Create( FACTORY_CLACT* factory_clact, u8 sel_max, u8 sel_h_max, u8 mode, u8 csr_pos, const FACTORY_POS* pos_tbl, const u8* anm_tbl )
{
	FACTORY_CSR* wk;
	VecFx32	vec;

	wk = sys_AllocMemory( HEAPID_FACTORY, sizeof(FACTORY_CSR) );		//�������m��
	memset( wk, 0, sizeof(FACTORY_CSR) );

	wk->sel_max		= sel_max;				//�I���ő吔
	wk->sel_h_max	= sel_h_max;			//���I���̍ő吔
	wk->mode		= mode;					//���I�����A�c�I����
	wk->csr_pos		= csr_pos;				//�J�[�\���ʒu
	wk->pos_tbl		= pos_tbl;				//�J�[�\�����ړ�������W�e�[�u���̃|�C���^
	wk->anm_tbl		= anm_tbl;				//�J�[�\���̃A�j���e�[�u���̃|�C���^

	//�A�j���e�[�u���̎w�肪���鎞
	if( anm_tbl != NULL ){
		wk->p_clact = FactoryClact_SetActor(	factory_clact, 0, 
												wk->anm_tbl[csr_pos], 0, 0, DISP_MAIN );
	}else{
		//����A�^�C�v�͌Œ�(data_no,anm_no,pri)
		if( mode == CSR_H_MODE ){
			wk->p_clact = FactoryClact_SetActor( factory_clact, 0, ANM_BALL_CSR, 0, 0, DISP_MAIN );
		}else{
			wk->p_clact = FactoryClact_SetActor( factory_clact, 0, ANM_MENU_CSR, 0, 0, DISP_MAIN );
		}
	}

	//�����J�[�\�����W���Z�b�g
	vec.x = (wk->pos_tbl[wk->csr_pos].x * FX32_ONE);
	vec.y = (wk->pos_tbl[wk->csr_pos].y * FX32_ONE);
	CLACT_SetMatrix( wk->p_clact, &vec );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ���[�N�폜
 *
 * @param	wk		FACTORY_CSR���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* FactoryCsr_Delete( FACTORY_CSR* wk )
{
	CLACT_Delete( wk->p_clact );

	//FACTORY_CSR�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		FACTORY_CSR�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryCsr_Vanish( FACTORY_CSR* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�ړ�
 *
 * @param	wk		FACTORY_CSR�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryCsr_Move( FACTORY_CSR* wk )
{
	VecFx32	vec;
	u32	check_key1,check_key2;

	//�|�[�Y���͉������Ȃ�
	if( wk->pause == 1 ){
		return;
	}

	if( sys.trg == 0 ){
		return;
	}

	//HV�ɑΉ����������ȃ��[�h
	if( wk->mode == CSR_HV_MODE ){

		if( sys.trg & PAD_KEY_LEFT ){
			if( wk->csr_pos == 0 ){
				wk->csr_pos = (wk->sel_max-1);
			}else{
				wk->csr_pos--;
			}
		}else if( sys.trg & PAD_KEY_RIGHT ){
			if( wk->csr_pos == (wk->sel_max-1) ){
				wk->csr_pos = 0;
			}else{
				wk->csr_pos++;
			}
		}else if( sys.trg & PAD_KEY_DOWN ){
			if( wk->csr_pos < (wk->sel_h_max) ){		//���ɃJ�[�\���ړ�
				wk->csr_pos = wk->sel_h_max;
			}else if( wk->csr_pos == (wk->sel_max-1) ){	//���łɉ��ɃJ�[�\�������鎞�́A���ɖ߂�
				wk->csr_pos = 0;
			}else{
				wk->csr_pos++;
			}
		}else if( sys.trg & PAD_KEY_UP ){
			if( wk->csr_pos < (wk->sel_h_max) ){		//��ɃJ�[�\���ړ�
				wk->csr_pos = (wk->sel_max-1);
			}else{										//���ɃJ�[�\���ړ�
				wk->csr_pos--;
			}
		}

	//------------------------------------------------------------------------------------------
	}else{

		//�`�F�b�N����L�[
		if( wk->mode == CSR_H_MODE ){
			check_key1 = PAD_KEY_RIGHT;
			check_key2 = PAD_KEY_LEFT;
		}else{
			check_key1 = PAD_KEY_DOWN;
			check_key2 = PAD_KEY_UP;
		}

		//�L�[�Ή����鏈��
		if( sys.trg & check_key1 ){
			wk->csr_pos++;
			if( wk->csr_pos >= wk->sel_max ){
				wk->csr_pos = 0;
			}
		}else if( sys.trg & check_key2 ){
			if( wk->csr_pos == 0 ){
			wk->csr_pos = wk->sel_max;
				}
			wk->csr_pos--;
		}
	}

	//�A�j����؂�ւ���
	if( wk->anm_tbl != NULL ){
		CLACT_AnmChgCheck( wk->p_clact, wk->anm_tbl[wk->csr_pos] );
	}

	//���W���擾
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x = (wk->pos_tbl[wk->csr_pos].x * FX32_ONE);
	vec.y = (wk->pos_tbl[wk->csr_pos].y * FX32_ONE);

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���擾
 *
 * @param	wk		FACTORY_CSR�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
u8 FactoryCsr_GetCsrPos( FACTORY_CSR* wk )
{
	return wk->csr_pos;
}

//--------------------------------------------------------------
/**
 * @brief	�|�[�Y
 *
 * @param	wk		FACTORY_CSR�^�̃|�C���^
 * @param	flag	0:�|�[�Y���� 1:�|�[�Y
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryCsr_Pause( FACTORY_CSR* wk, int flag )
{
	wk->pause = flag;

	if( flag == 0 ){
		CLACT_AnmChgCheck( wk->p_clact, ANM_BALL_CSR );
		//CLACT_PaletteNoChg( wk->p_clact, PAL_CSR_MOVE );		//p_csr�ȊO�n���ƃ_���B�B�B
	}else{
		CLACT_AnmChgCheck( wk->p_clact, ANM_BALL_CSR_STOP );
		//CLACT_PaletteNoChg( wk->p_clact, PAL_CSR_STOP );		//p_csr�ȊO�n���ƃ_���B�B�B
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���Z�b�g
 *
 * @param	wk		FACTORY_CSR�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryCsr_SetCsrPos( FACTORY_CSR* wk, u8 csr_pos )
{
	VecFx32	vec;

	wk->csr_pos = csr_pos;

	//�A�j����؂�ւ���
	if( wk->anm_tbl != NULL ){
		CLACT_AnmChgCheck( wk->p_clact, wk->anm_tbl[wk->csr_pos] );
	}

	//���W���擾
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x = (wk->pos_tbl[wk->csr_pos].x * FX32_ONE);
	vec.y = (wk->pos_tbl[wk->csr_pos].y * FX32_ONE);

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

