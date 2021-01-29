//==============================================================================
/**
 * @file	stage_bg.c
 * @brief	�o�g���X�e�[�W�FBG�A�j��
 * @author	matsuda
 * @date	2007.08.07(��)
 */
//==============================================================================
#include "common.h"
#include "frontier_types.h"
#include "frontier_tcb_pri.h"
#include "stage_bganm.h"
#include "graphic/frontier_bg_def.h"
#include "system/arc_tool.h"
#include "frontier_map.h"


//==============================================================================
//	�萔��`
//==============================================================================
//--------------------------------------------------------------
//	BG�A�j��
//--------------------------------------------------------------
///BG�A�j���p�^�[����
#define STAGE_BG_ANIME_PATTERN		(2)
///�X�N���[���o�C�g�T�C�Y
#define STAGE_BG_SCREEN_SIZE		(0x1000)
///�L�����N�^�o�C�g�T�C�Y
#define STAGE_BG_CHAR_SIZE		(0x10000)
///BG�A�j���E�F�C�g
#define STAGE_BG_ANIME_WAIT			(12)

//--------------------------------------------------------------
//	�p���b�g�A�j��
//--------------------------------------------------------------
///�p���b�g�A�j���J�n�p���b�g�ԍ�
#define PALANM_START_PALETTE		(5)
///�p���b�g�A�j���̃p���b�g�{��
#define PALANM_PALETTE_NUM			(3)
///�p���b�g�A�j���E�F�C�g
#define PALANM_ANIME_WAIT			(8)
///�p���b�g�]���ʒu
#define PALANM_TRANS_POS			(6)	//�X�N���[����6�ԃp���b�g�ŕ`����Ă���̂�
///�p���b�g�A�j���J�n�ԍ�
#define PALANM_START_ANMNO			(2)	//�X�N���[����PALANM_START_PALETTE�Ŏn�܂��Ă��Ȃ��̂�

//--------------------------------------------------------------
//	���X�^�[
//--------------------------------------------------------------
///���X�^�[�J�n���C��(Y�h�b�g�ʒu)
#define LASTER_START_LINE	(0x13 * 8 - 1)
///���X�^�[�I�����C��(Y�h�b�g�ʒu)
#define LASTER_END_LINE		(0x1d * 8 + 1)

///���X�^�[���ZX�l(����8�r�b�g����)
#define LASTER_ADD_X		(0x0200)
///�ő�X�ʒu(�����܂ŗ�����܂�Ԃ�)
#define LASTER_MAX_X		(256 - 80)
///�܂�Ԃ��n�_�ɗ��Ă���ēx�����o���܂ł̃E�F�C�g
#define LASTER_RETURN_WAIT	(0)

//--------------------------------------------------------------
//	������
//--------------------------------------------------------------
///��������1�Ώۖ�
#define STAGE_BLD_PLANE_1		(GX_BLEND_PLANEMASK_BG2)
///��������2�Ώۖ�
#define STAGE_BLD_PLANE_2		(GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ)
///��1�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define STAGE_BLD_ALPHA_1		(8)
///��2�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define STAGE_BLD_ALPHA_2		(9)


//==============================================================================
//	�\���̒�`
//==============================================================================
///BG�A�j���[�V�����Ǘ����[�N
typedef struct _STAGE_BGANM_WORK{
	TCB_PTR tcb;				///<����^�X�N�ւ̃|�C���^
	GF_BGL_INI *bgl;
	u16 scrn[STAGE_BG_ANIME_PATTERN][STAGE_BG_SCREEN_SIZE / 2];	///<�]���X�N���[���f�[�^
	u8 charcter[STAGE_BG_ANIME_PATTERN][STAGE_BG_CHAR_SIZE];	///<�]���L�����N�^�f�[�^
	u8 wait;					///<�A�j���E�F�C�g
	u8 work_no;					///<�]������X�N���[���o�b�t�@�̔ԍ�
}STAGE_BGANM_WORK;

///�p���b�g�A�j���[�V�����Ǘ����[�N
typedef struct{
	TCB_PTR tcb;				///<����^�X�N�ւ̃|�C���^
	PALETTE_FADE_PTR pfd;		///<PFD�ւ̃|�C���^
	u16 palette[PALANM_PALETTE_NUM][16];	///<�p���b�g�o�b�t�@
	u8 anm_no;					///<�A�j���[�V�����ԍ�
	u8 wait;					///<�A�j���E�F�C�g
}STAGE_PALANM_WORK;

///���X�^�[�A�j���[�V�����Ǘ����[�N
typedef struct{
	TCB_PTR tcb;				///<����^�X�N�ւ̃|�C���^
	TCB_PTR v_tcb;				///<V�u�����N����^�X�N�ւ̃|�C���^
	GF_BGL_INI *bgl;			///<BGL�ւ̃|�C���^
	int default_x;				///<���X�^�[���Ȃ���Ԃ�X���W
	int calc_x;					///<���X�^�[X���W�v�Z�̈�
	int set_x;					///<���X�^�[�ŃZ�b�g����X���W
	u8 move_seq;				///<�V�[�P���X�ԍ�
	u8 wait;					///<�E�F�C�g
	u8 init;					///<1��ȏ�R���g���[���^�X�N�����s���ꂽ
}STAGE_LASTER_WORK;

//--------------------------------------------------------------
/**
 *	�o�g���X�e�[�W�A�j���[�V�����Ǘ����[�N
 */
//--------------------------------------------------------------
typedef struct _STAGE_ANIMATION_WORK{
	STAGE_BGANM_WORK *bganm;		///<BG�A�j���[�V�����Ǘ����[�N
	STAGE_PALANM_WORK *palanm;	///<�p���b�g�A�j���[�V�����Ǘ����[�N
	STAGE_LASTER_WORK *laster;	///<���X�^�[�A�j���[�V�����Ǘ����[�N
}STAGE_ANIMATION_WORK;


//==============================================================================
//	�f�[�^
//==============================================================================
///�X�N���[���]���A�j���̃X�N���[���A�[�J�C�uID
static const u16 AnimeScreenIndex[] = {
	BS_STAGE_01_A_NSCR_BIN,
	BS_STAGE_02_A_NSCR_BIN,
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static STAGE_BGANM_WORK * StageBGAnime_Init(GF_BGL_INI *bgl);
static void StageBGAnime_End(STAGE_BGANM_WORK *sbw);
static void StageBG_AnimeControl(TCB_PTR tcb, void *work);
static STAGE_PALANM_WORK * StagePaletteAnime_Init(PALETTE_FADE_PTR pfd);
static void StagePaletteAnime_End(STAGE_PALANM_WORK *pw);
static void StagePalette_AnimeControl(TCB_PTR tcb, void *work);
static STAGE_LASTER_WORK * StageLasterAnime_Init(GF_BGL_INI *bgl);
static void StageLasterAnime_End(STAGE_LASTER_WORK *laster);
static void StageLaster_AnimeControl(TCB_PTR tcb, void *work);
static void StageLaster_VIntrTCB(TCB_PTR tcb, void *work);
static void StageLaster_HBlank(void *work);




//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�F�A�j���[�V�����V�X�e���쐬
 *
 * @param   bgl		BGL�ւ̃|�C���^
 * @param   pfd		PFD�ւ̃|�C���^
 *
 * @retval  �X�e�[�W�A�j���[�V�����Ǘ����[�N�̃|�C���^
 */
//--------------------------------------------------------------
STAGE_ANIMATION_WORK_PTR StageAnimation_Init(GF_BGL_INI *bgl, PALETTE_FADE_PTR pfd)
{
	STAGE_ANIMATION_WORK *anmsys;
	
	anmsys = sys_AllocMemory(HEAPID_FRONTIERMAP, sizeof(STAGE_ANIMATION_WORK));
	MI_CpuClear8(anmsys, sizeof(STAGE_ANIMATION_WORK));
	
	anmsys->bganm = StageBGAnime_Init(bgl);
	anmsys->palanm = StagePaletteAnime_Init(pfd);
//	anmsys->laster = StageLasterAnime_Init(bgl);

	//�u�����h�ݒ�
	G2_SetBlendAlpha(STAGE_BLD_PLANE_1, STAGE_BLD_PLANE_2, STAGE_BLD_ALPHA_1, STAGE_BLD_ALPHA_2);
	
	return anmsys;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�F�A�j���[�V�����V�X�e���I��
 *
 * @param   anmsys		�X�e�[�W�A�j���[�V�����Ǘ����[�N�̃|�C���^
 */
//--------------------------------------------------------------
void StageAnimation_End(STAGE_ANIMATION_WORK_PTR anmsys)
{
	StageBGAnime_End(anmsys->bganm);
	StagePaletteAnime_End(anmsys->palanm);
//	StageLasterAnime_End(anmsys->laster);
	
	sys_FreeMemoryEz(anmsys);
}


//==============================================================================
//	BG�A�j��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�WBG�A�j���V�X�e���쐬
 *
 * @param   bgl				BGL�ւ̃|�C���^
 *
 * @retval  BG�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static STAGE_BGANM_WORK * StageBGAnime_Init(GF_BGL_INI *bgl)
{
	STAGE_BGANM_WORK *sbw;
	
	GF_ASSERT(STAGE_BG_ANIME_PATTERN == NELEMS(AnimeScreenIndex));
	
	sbw = sys_AllocMemory(HEAPID_FRONTIERMAP, sizeof(STAGE_BGANM_WORK));
	MI_CpuClear8(sbw, sizeof(STAGE_BGANM_WORK));
	sbw->bgl = bgl;
	
	//�X�N���[���]���A�j���̏���
	{
		ARCHANDLE* hdl_bg;
		void* arc_data;
		NNSG2dScreenData *scrnData;
		int i;
		
		hdl_bg  = ArchiveDataHandleOpen(ARC_FRONTIER_BG,  HEAPID_FRONTIERMAP);
		for(i = 0; i < STAGE_BG_ANIME_PATTERN; i++){
			arc_data = ArcUtil_HDL_ScrnDataGet(hdl_bg, AnimeScreenIndex[i], 1, 
				&scrnData, HEAPID_FRONTIERMAP);
			MI_CpuCopy32(scrnData->rawData, sbw->scrn[i], STAGE_BG_SCREEN_SIZE);
			sys_FreeMemoryEz(arc_data);
		}
		ArchiveDataHandleClose( hdl_bg );
	}

	//�L�����N�^�]���A�j���̏���
	{
		ARCHANDLE *hdl_bg;
		void *arc_data;
		NNSG2dCharacterData *charData;
		
		hdl_bg = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIERMAP );

		arc_data = ArcUtil_HDL_CharDataGet(
			hdl_bg, BS_STAGE_NCGR_BIN, 1, &charData, HEAPID_FRONTIERMAP);
		MI_CpuCopy32(charData->pRawData, sbw->charcter[0], charData->szByte);
		sys_FreeMemoryEz(arc_data);
		
		arc_data = ArcUtil_HDL_CharDataGet(
			hdl_bg, BS_STAGE02_NCGR_BIN, 1, &charData, HEAPID_FRONTIERMAP);
		MI_CpuCopy32(charData->pRawData, sbw->charcter[1], charData->szByte);
		sys_FreeMemoryEz(arc_data);

		ArchiveDataHandleClose(hdl_bg);
	}

	//�X�N���[���]���A�j���Ǘ��^�X�NAdd
	sbw->tcb = TCB_Add(StageBG_AnimeControl, sbw, TCBPRI_STAGE_BGANIME_CONTROL);
	
	return sbw;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�WBG�A�j���V�X�e���I��
 *
 * @param   sbw		BG�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void StageBGAnime_End(STAGE_BGANM_WORK *sbw)
{
	TCB_Delete(sbw->tcb);
	sys_FreeMemoryEz(sbw);
}

//--------------------------------------------------------------
/**
 * @brief   BG�A�j���R���g���[���^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		STAGE_BGANM_WORK�\����
 */
//--------------------------------------------------------------
static void StageBG_AnimeControl(TCB_PTR tcb, void *work)
{
	u32 idx;
	STAGE_BGANM_WORK *sbw = work;
	
	if(sbw->wait < STAGE_BG_ANIME_WAIT){
		sbw->wait++;
		return;
	}
	sbw->wait = 0;
	
	{
		GF_BGL_LoadCharacter(sbw->bgl, FRMAP_FRAME_MAP, 
			sbw->charcter[sbw->work_no], STAGE_BG_CHAR_SIZE, 0);
	}

	GF_BGL_ScreenBufSet(sbw->bgl, FRMAP_FRAME_MAP, sbw->scrn[sbw->work_no], STAGE_BG_SCREEN_SIZE);
	GF_BGL_LoadScreenV_Req(sbw->bgl, FRMAP_FRAME_MAP);
	sbw->work_no ^= 1;
}


//==============================================================================
//	�p���b�g�A�j��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�p���b�g�A�j���V�X�e���쐬
 *
 * @param   pfd				PFD�ւ̃|�C���^
 *
 * @retval  �p���b�g�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static STAGE_PALANM_WORK * StagePaletteAnime_Init(PALETTE_FADE_PTR pfd)
{
	STAGE_PALANM_WORK *pw;
	
	pw = sys_AllocMemory(HEAPID_FRONTIERMAP, sizeof(STAGE_PALANM_WORK));
	MI_CpuClear8(pw, sizeof(STAGE_PALANM_WORK));
	pw->pfd = pfd;
	pw->anm_no = PALANM_START_ANMNO;
	
	//�p���b�g���o�b�t�@�ɃR�s�[
	{
		u16 *src_pal;
		src_pal = PaletteWorkDefaultWorkGet(pfd, FADE_MAIN_BG);
		MI_CpuCopy16(&src_pal[16 * PALANM_START_PALETTE], 
			pw->palette, PALANM_PALETTE_NUM * 16 * sizeof(u16));
	}
	
	//�p���b�g�A�j���Ǘ��^�X�NAdd
	pw->tcb = TCB_Add(StagePalette_AnimeControl, pw, TCBPRI_STAGE_PALANIME_CONTROL);
	
	return pw;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�WBG�A�j���V�X�e���I��
 *
 * @param   pw		BG�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void StagePaletteAnime_End(STAGE_PALANM_WORK *pw)
{
	TCB_Delete(pw->tcb);
	sys_FreeMemoryEz(pw);
}

//--------------------------------------------------------------
/**
 * @brief   �p���b�g�A�j���R���g���[���^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		STAGE_PALANM_WORK�\����
 */
//--------------------------------------------------------------
static void StagePalette_AnimeControl(TCB_PTR tcb, void *work)
{
	STAGE_PALANM_WORK *pw = work;
	
	if(pw->wait < PALANM_ANIME_WAIT){
		pw->wait++;
		return;
	}
	pw->wait = 0;
	
	PaletteWorkSet(pw->pfd, pw->palette[pw->anm_no], FADE_MAIN_BG, PALANM_TRANS_POS * 16, 0x20);
	pw->anm_no++;
	if(pw->anm_no >= PALANM_PALETTE_NUM){
		pw->anm_no = 0;
	}
}


//==============================================================================
//	���X�^�[�A�j��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W���X�^�[�A�j���V�X�e���쐬
 *
 * @param   bgl				BGL�ւ̃|�C���^
 *
 * @retval  ���X�^�[�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static STAGE_LASTER_WORK * StageLasterAnime_Init(GF_BGL_INI *bgl)
{
	STAGE_LASTER_WORK *laster;
	BOOL ret;
	
	laster = sys_AllocMemory(HEAPID_FRONTIERMAP, sizeof(STAGE_LASTER_WORK));
	MI_CpuClear8(laster, sizeof(STAGE_LASTER_WORK));
	laster->bgl = bgl;
	
	laster->tcb = TCB_Add(StageLaster_AnimeControl, laster, TCBPRI_STAGE_LASTER_CONTROL);
	laster->v_tcb = VIntrTCB_Add(StageLaster_VIntrTCB, laster, 10);
	ret = sys_HBlankIntrSet(StageLaster_HBlank, laster);
	GF_ASSERT(ret == TRUE);
	
	return laster;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W���X�^�[�A�j���V�X�e���I��
 *
 * @param   laster		���X�^�[�A�j�����䃏�[�N�̃|�C���^
 */
//--------------------------------------------------------------
static void StageLasterAnime_End(STAGE_LASTER_WORK *laster)
{
	TCB_Delete(laster->tcb);
	TCB_Delete(laster->v_tcb);
	sys_FreeMemoryEz(laster);
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�F���X�^�[�A�j������^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		STAGE_LASTER_WORK�\����
 */
//--------------------------------------------------------------
static void StageLaster_AnimeControl(TCB_PTR tcb, void *work)
{
	STAGE_LASTER_WORK *laster = work;
	
	//���X�^�[X���W�v�Z
	{
		enum{
			SEQ_MOVE,
			SEQ_RETURN_WAIT,
			SEQ_BACK_MOVE,
			SEQ_BACK_RETURN_WAIT,
		};
		
		switch(laster->move_seq){
		case SEQ_MOVE:
			laster->calc_x += LASTER_ADD_X;
			if((laster->calc_x >> 8) > LASTER_MAX_X){
				laster->calc_x = (LASTER_MAX_X << 8) | 0xff;
				laster->move_seq++;
			}
			break;
		case SEQ_RETURN_WAIT:
		case SEQ_BACK_RETURN_WAIT:
			laster->wait++;
			if(laster->wait > LASTER_RETURN_WAIT){
				laster->wait = 0;
				if(laster->move_seq == SEQ_BACK_RETURN_WAIT){
					laster->move_seq = 0;
				}
				else{
					laster->move_seq++;
				}
			}
			break;
		case SEQ_BACK_MOVE:
			laster->calc_x -= LASTER_ADD_X;
			if(laster->calc_x < 0x100){
				laster->calc_x = 0;
				laster->move_seq++;
			}
			break;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���X�^�[�A�j��V�u�����N�^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		STAGE_LASTER_WORK�\����
 *
 * ��V�u�����N�^�X�N
 */
//--------------------------------------------------------------
static void StageLaster_VIntrTCB(TCB_PTR tcb, void *work)
{
	STAGE_LASTER_WORK *laster = work;
	int scr_y, start, end;
	
	laster->init = TRUE;
	laster->default_x = 0;//GF_BGL_ScrollGetX(laster->bgl, FRMAP_FRAME_EFF);
	laster->set_x = -(laster->calc_x >> 8);
	
	//���X�^�[�J�n���C������ʏ㕔�ɍs���Ă���ꍇ�̏���
	scr_y = GF_BGL_ScrollGetY(laster->bgl, FRMAP_FRAME_EFF);
	start = LASTER_START_LINE - scr_y;
	end = LASTER_END_LINE - scr_y;
	if(start <= 1 && end > 0){
		GF_BGL_ScrollSet(laster->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, laster->set_x);
	}
	else{
		GF_BGL_ScrollSet(laster->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, laster->default_x);
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���X�^�[�A�j���FH�u�����N�ł̃X�N���[�����W�ݒ�
 *
 * @param   work		STAGE_LASTER_WORK�\����
 */
//--------------------------------------------------------------
static void StageLaster_HBlank(void *work)
{
	STAGE_LASTER_WORK *laster = work;
	s32 v_count;
	int scr_y, start, end;
	
	if(laster->init == 0){
		return;	//�܂��R���g���[���^�X�N����x�����s����Ă��Ȃ�
	}
	
	v_count = GX_GetVCount();
	scr_y = GF_BGL_ScrollGetY(laster->bgl, FRMAP_FRAME_EFF);
	start = LASTER_START_LINE - scr_y;
	end = LASTER_END_LINE - scr_y;
	
	if(v_count == start){
		GF_BGL_ScrollSet(laster->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, laster->set_x);
	}
	else if(v_count == end){
		GF_BGL_ScrollSet(laster->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, laster->default_x);
	}
	
}
