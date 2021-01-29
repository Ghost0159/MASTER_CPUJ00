//==============================================================================
/**
 * @file	connect_anm.h
 * @brief	Wifi�ڑ���ʂ�BG�A�j����inline�֐�
 * @author	matsuda
 * @date	2007.12.26(��)
 *
 * Wifi�ڑ�BG��ʂ��e�A�v���P�[�V�����X�ō쐬����Ă���̂ŁA�A�j���[�V����������ׂ̊֐���
 * �����ɑS��inline�֐��Ƃ��ėp�ӁB(�����̃I�[�o�[���C�ɂ܂������Ă����)
 */
//==============================================================================
#ifndef __CONNECT_ANM_H__
#define __CONNECT_ANM_H__

#include "application/connect_anm_types.h"

static inline void ConnectBGPalAnm_TblCreate(CONNECT_BG_PALANM *cbp);
static inline void ConnectBGPalAnm_IntrTCB(TCB_PTR tcb, void *work);

//==============================================================================
//
//	inline�֐�
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F������
 *
 * @param   cbp				Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 * @param   p_hdl			BG�p���b�g�������Ă���A�[�J�C�u�̃n���h��
 * @param   pal_index		BG�p���b�g��Index
 * @param   heap_id			�q�[�vID(���̏����̃e���|�����Ŏg�p)
 *
 * ���[�N�̊m�ۂ�TCB�̐������͍s���Ă��Ȃ��ׁA�I���֐��͂���܂���B
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_Init(CONNECT_BG_PALANM *cbp, ARCHANDLE *p_hdl, int pal_index, int heap_id)
{
	NNSG2dPaletteData *palData;
	void *p_work;
	
	MI_CpuClear8(cbp, sizeof(CONNECT_BG_PALANM));

	p_work = ArcUtil_HDL_PalDataGet(p_hdl, pal_index, &palData, heap_id);
	
	//�o�b�t�@�Ƀp���b�g�f�[�^���R�s�[
	MI_CpuCopy16(&((u16*)(palData->pRawData))[CBP_PAL_START_NUMBER * 16], 
		cbp->src_color, CBP_PAL_NUM * 0x20);
	MI_CpuCopy16(&((u16*)(palData->pRawData))[CBP_PAL_START_NUMBER * 16], 
		cbp->dest_color, CBP_PAL_NUM * 0x20);
	
	sys_FreeMemoryEz(p_work);

	ConnectBGPalAnm_TblCreate(cbp);
	
	//�L���t���OON
	cbp->occ = TRUE;
	//VIntroTCB����
#ifdef PM_DEBUG
	GF_ASSERT(DebugTCB_VintrFuncCheck(ConnectBGPalAnm_IntrTCB) == FALSE);
#endif
	cbp->tcb = VIntrTCB_Add(ConnectBGPalAnm_IntrTCB, cbp, 20);
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F���C������
 *
 * @param   cbp		Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_End(CONNECT_BG_PALANM *cbp)
{
	if(cbp->tcb != NULL){
		TCB_Delete(cbp->tcb);
		cbp->tcb = NULL;
		cbp->occ = FALSE;
	}
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F���C������
 *
 * @param   cbp		Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_Main(CONNECT_BG_PALANM *cbp)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F�]��
 *
 * @param   cbp		Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 *
 * VBlank���ɌĂяo���悤�ɂ��Ă�������
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_VBlank(CONNECT_BG_PALANM *cbp)
{
#if 0
	if(cbp->occ == FALSE){
		return;
	}

	GX_LoadBGPltt((const void *)cbp->dest_color[cbp->trans_pos], 
		CBP_PAL_START_NUMBER * 0x20, CBP_TRANS_PAL_NUM * 0x20);
	GXS_LoadBGPltt((const void *)cbp->dest_color[cbp->trans_pos], 
		CBP_PAL_START_NUMBER * 0x20, CBP_TRANS_PAL_NUM * 0x20);
	if(cbp->trans_dir == 0){
		cbp->trans_pos++;
		if(cbp->trans_pos >= CBP_EVY_TBL_ALL){
			cbp->trans_pos = CBP_EVY_TBL_ALL - 2;
			cbp->trans_dir ^= 1;
		}
	}
	else{
		cbp->trans_pos--;
		if(cbp->trans_pos < 0){
			cbp->trans_pos = 1;
			cbp->trans_dir ^= 1;
		}
	}
#endif
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F�L���A�����t���O���Z�b�g
 *
 * @param   cbp		Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 * @param   occ		TRUE:�L�����B�@FALSE�F������
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_OccSet(CONNECT_BG_PALANM *cbp, BOOL occ)
{
	cbp->occ = occ;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ�BG�A�j���F���C������
 *
 * @param   cbp		Wifi�ڑ�BG�̃p���b�g�A�j������\���̂ւ̃|�C���^
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_TblCreate(CONNECT_BG_PALANM *cbp)
{
	int src_pos, dest_pos, next_color_pos, evy, s, next_break;
	
	dest_pos = 0;
	for(src_pos = 0; src_pos < CBP_PAL_NUM-1; src_pos++){
		next_color_pos = src_pos + 1;
		evy = 0;
		next_break = 0;
		do{
			GF_ASSERT(dest_pos < CBP_EVY_TBL_ALL);
			for(s = CBP_PAL_COLOR_START; s < CBP_PAL_COLOR_START + CBP_PAL_COLOR_NUM; s++){
				SoftFade(&cbp->src_color[src_pos][s], &cbp->dest_color[dest_pos][s],
					1, evy >> 8, cbp->src_color[next_color_pos][s]);
			}
			dest_pos++;
			if(next_break == TRUE){
				break;
			}
			evy += CBP_ADD_EVY;
			if(evy >= (16<<8)){
				evy = (16<<8);
				next_break = TRUE;
			}
		}while(1);
	}
	DC_FlushRange(cbp->dest_color, CBP_EVY_TBL_ALL * 16 * sizeof(u16));
}

//--------------------------------------------------------------
/**
 * @brief   VIntr�Ńp���b�g�]��
 *
 * @param   tcb		
 * @param   work		
 */
//--------------------------------------------------------------
static inline void ConnectBGPalAnm_IntrTCB(TCB_PTR tcb, void *work)
{
	CONNECT_BG_PALANM *cbp = work;
	
	if(cbp->occ == FALSE){
		return;
	}

	cbp->intr_count ^= 1;
	if(cbp->intr_count & 1){
		return;
	}
	
	GX_LoadBGPltt((const void *)cbp->dest_color[cbp->trans_pos], 
		CBP_PAL_START_NUMBER * 0x20, CBP_TRANS_PAL_NUM * 0x20);
	GXS_LoadBGPltt((const void *)cbp->dest_color[cbp->trans_pos], 
		CBP_PAL_START_NUMBER * 0x20, CBP_TRANS_PAL_NUM * 0x20);
	if(cbp->trans_dir == 0){
		cbp->trans_pos++;
		if(cbp->trans_pos >= CBP_EVY_TBL_ALL){
			cbp->trans_pos = CBP_EVY_TBL_ALL - 2;
			cbp->trans_dir ^= 1;
		}
	}
	else{
		cbp->trans_pos--;
		if(cbp->trans_pos < 0){
			cbp->trans_pos = 1;
			cbp->trans_dir ^= 1;
		}
	}
}

#endif	//__CONNECT_ANM_H__
