//==============================================================================================
/**
 * @file	scratch_clact.c
 * @brief	�u�X�N���b�`�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	06.12.11
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "poketool/pokeicon.h"

#include "scratch_sys.h"
#include "scratch_clact.h"
#include "scratch.naix"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�L�����N�^�[�}�l�[�W���[
//#define CHAR_CONT_NUM						(4)		//�L�����N�^���䐔
#define CHAR_CONT_NUM						(5)		//�L�����N�^���䐔(08.02.09)

#if 0
//.ncg�̃T�C�Y(poke.ncg = 160x40 = 6400 / 2 = ��3200 )
//.ncg�̃T�C�Y(1���.ncg = 256x192 = 49152 / 2 = ��24576 )
//#define CHAR_VRAMTRANS_MAIN_SIZE			(68224)				//poke + card + l_oam02�T�C�Y
//#define CHAR_VRAMTRANS_SUB_SIZE			(4096)				//poke �T�C�Y
#else
//.ncg�̃T�C�Y(poke.ncg = 200x40 = 8000 / 2 = ��4000 )
//.ncg�̃T�C�Y(card.ncg = 384x256 = 34048 / 2 = ��17024 )
//.ncg�̃T�C�Y(l_oam02.ncg = 224x152 = 98304 / 2 = ��49152 )
#endif

//#define CHAR_VRAMTRANS_MAIN_SIZE			(128*1024)
//#define CHAR_VRAMTRANS_SUB_SIZE			(16*1024)

//#define CHAR_VRAMTRANS_MAIN_SIZE			(43000)				//poke + card + l_oam02�T�C�Y
#define CHAR_VRAMTRANS_MAIN_SIZE			(3000)				//poke + card + l_oam02�T�C�Y
#define CHAR_VRAMTRANS_SUB_SIZE				(5120)				//poke �T�C�Y

///�L�����}�l�[�W���F���C����ʃT�C�Y(byte�P��)
//#define FMAP_CHAR_VRAMSIZE_MAIN			(0x10000)	//(1024 * 0x40)	//64K
///�L�����}�l�[�W���F�T�u��ʃT�C�Y(byte�P��)
//#define FMAP_CHAR_VRAMSIZE_SUB			(512 * 0x20)	//32K

enum{
	DISP_MAIN_OBJ_PAL	= 5,				//poke
	DISP_SUB_CARD_PAL	= 4,				//l_oam00
	DISP_SUB2_BUTTON_PAL= 4,				//l_oam01
	DISP_SUB3_ATARI_PAL = 1,				//l_oam02
	
	//DISP_PAL_NUM		= 15,
	DISP_PAL_NUM		= 14,
};
//�p���b�g�̎g���Ă���1�{�����̏ꏊ�ɁA
//���̃p���b�g1�{�����㏑���ł悢���A
//���\�[�X�̃p���b�g���I�t�Z�b�g�w�肪�ǂ̊֐�����������Ȃ������̂ŁA
//�ʃ��\�[�X�ɕ����āA���\�[�X�؂�ւ��ɂ���


//==============================================================================================
//
//	�f�[�^
//
//==============================================================================================
//���\�[�X�^�C�v�񋓂ɍ��킹��(include/system/clact_util_res.h)
//���\�[�X�}�l�[�W���[�o�^���e�[�u��
static const u8 ResEntryNumTbl[SCRATCH_RESOURCE_NUM] = {
#if 0
	2,			//�L�������\�[�X
	3,			//�p���b�g���\�[�X
	2,			//�Z�����\�[�X
	2,			//�Z���A�j�����\�[�X
#else
	SCRATCH_RES_OBJ_MAX,		//�L�������\�[�X
	SCRATCH_RES_OBJ_MAX,		//�p���b�g���\�[�X
	SCRATCH_RES_OBJ_MAX,		//�Z�����\�[�X
	SCRATCH_RES_OBJ_MAX,		//�Z���A�j�����\�[�X
#endif
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//ScratchClact�֐�
void			ScratchClact_InitCellActor( SCRATCH_CLACT* wk );
CLACT_WORK_PTR ScratchClact_SetActor( SCRATCH_CLACT* wk, u32 data_no, u32 anm_no, u32 bg_pri, u32 pri, u8 disp );
void			ScratchClact_DeleteCellObject( SCRATCH_CLACT* wk );
void			ScratchClact_ResButtonChg( SCRATCH_CLACT* wk );
void			ScratchClact_ButtonResourcePalChg( SCRATCH_CLACT* wk, int no );

//static
static void		InitCharPlttManager( void );
static void		ScratchClact_ResPokeSet( SCRATCH_CLACT* wk, int res_id, int obj_id, int vram );
static void		ScratchClact_ResCardSet( SCRATCH_CLACT* wk, int vram );
static void		ScratchClact_ResButtonSet( SCRATCH_CLACT* wk, int vram );
static void		ScratchClact_ResAtariSet( SCRATCH_CLACT* wk, int vram );
static void		ScratchClact_DeleteCellObjectOne( SCRATCH_CLACT* wk, int res_id );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[������
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchClact_InitCellActor( SCRATCH_CLACT* wk )
{
	int i;
	ARCHANDLE* p_handle;

	initVramTransferManagerHeap( 32, HEAPID_SCRATCH );

	//�L�����N�^�[�E�p���b�g�}�l�[�W���[������
	InitCharPlttManager();

	//REND_OAM_UtilOamRamClear_Main( HEAPID_SCRATCH );
	//REND_OAM_UtilOamRamClear_Sub( HEAPID_SCRATCH );	

	//OAM�}�l�[�W���[�̏�����
	NNS_G2dInitOamManagerModule();

	//���LOAM�}�l�[�W���쐬
	//�����_���pOAM�}�l�[�W���쐬
	//�����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
	REND_OAMInit(	0, 128,				//���C�����OAM�Ǘ��̈�
					0, 32,				//���C����ʃA�t�B���Ǘ��̈�
					0, 128,				//�T�u���OAM�Ǘ��̈�
					0, 32,				//�T�u��ʃA�t�B���Ǘ��̈�
					HEAPID_SCRATCH);
	
	//�Z���A�N�^�[�Z�b�g�̊ȒP������(�쐬���ꂽ�Z���A�N�^�[�Z�b�g���Ԃ�)
	wk->ClactSet = CLACT_U_SetEasyInit( SCRATCH_CLACT_OBJ_MAX, &wk->RendData, HEAPID_SCRATCH );
	
	//CLACT�Œ�`���Ă���Z�����傫�����ăT�u��ʂɉe�����łĂ��܂��̂ŗ����Ă݂�
	CLACT_U_SetSubSurfaceMatrix( &wk->RendData, 0, SCRATCH_SUB_ACTOR_DISTANCE );

	//���\�[�X�}�l�[�W���[������
	for( i=0; i < SCRATCH_RESOURCE_NUM ;i++ ){		//���\�[�X�}�l�[�W���[�쐬
		wk->ResMan[i] = CLACT_U_ResManagerInit( ResEntryNumTbl[i], i, HEAPID_SCRATCH );
	}

	/***************/
	//	�����
	/***************/
	ScratchClact_ResCardSet( wk, NNS_G2D_VRAM_TYPE_2DMAIN );					//�J�[�h
	ScratchClact_ResPokeSet( wk, SCRATCH_RES_OBJ_D_POKE, 
							 SCRATCH_ID_OBJ_D_POKE, NNS_G2D_VRAM_TYPE_2DMAIN );	//�|�P�����G��
	ScratchClact_ResButtonSet( wk, NNS_G2D_VRAM_TYPE_2DMAIN );					//�{�^��
	ScratchClact_ResAtariSet( wk, NNS_G2D_VRAM_TYPE_2DMAIN );					//������
	
	/***************/
	//	����
	/***************/
	ScratchClact_ResPokeSet( wk, SCRATCH_RES_OBJ_U_POKE, 
							 SCRATCH_ID_OBJ_U_POKE, NNS_G2D_VRAM_TYPE_2DSUB );	//�|�P�����G��

	//���\�[�X�}�l�[�W���[����]��
	for( i=0; i < SCRATCH_RES_OBJ_MAX ;i++ ){
		CLACT_U_CharManagerSet( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );	//Char�]��
		CLACT_U_PlttManagerSet( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );	//�p���b�g�]��
	}

	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );			//SUB DISP OBJ ON
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );			//MAIN DISP OBJ ON

	//ArchiveDataHandleClose( p_handle );	
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�F�J�[�h�Z�b�g
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchClact_ResCardSet( SCRATCH_CLACT* wk, int vram )
{
	//chara�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_CARD][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_SCRATCH, NARC_scratch_l_oam00_NCGR,
					FALSE, SCRATCH_ID_OBJ_D_CARD, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_SCRATCH);

	//pal�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_CARD][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_SCRATCH, NARC_scratch_l_oam00_NCLR,
					FALSE, SCRATCH_ID_OBJ_D_CARD, NNS_G2D_VRAM_TYPE_2DMAIN, 
					DISP_SUB_CARD_PAL, HEAPID_SCRATCH);

	//cell�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_CARD][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_SCRATCH, NARC_scratch_l_oam00_NCER,
					FALSE, SCRATCH_ID_OBJ_D_CARD, CLACT_U_CELL_RES, HEAPID_SCRATCH);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_CARD][CLACT_U_CELLANM_RES]=CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_SCRATCH, NARC_scratch_l_oam00_NANR,
					FALSE, SCRATCH_ID_OBJ_D_CARD, CLACT_U_CELLANM_RES, HEAPID_SCRATCH);

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�F�|�P�����G���Z�b�g
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchClact_ResPokeSet( SCRATCH_CLACT* wk, int res_id, int obj_id, int vram )
{
	//chara�ǂݍ���
	wk->ResObjTbl[res_id][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_SCRATCH, NARC_scratch_poke_NCGR,
					0, obj_id, vram, HEAPID_SCRATCH);

	//pal�ǂݍ���
	wk->ResObjTbl[res_id][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_SCRATCH, NARC_scratch_poke_NCLR,
					FALSE, obj_id, vram, 
					DISP_MAIN_OBJ_PAL,HEAPID_SCRATCH);

	//cell�ǂݍ���
	wk->ResObjTbl[res_id][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_SCRATCH, NARC_scratch_poke_NCER,
					0, obj_id, CLACT_U_CELL_RES, HEAPID_SCRATCH);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[res_id][CLACT_U_CELLANM_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_SCRATCH, NARC_scratch_poke_NANR,
					0, obj_id, CLACT_U_CELLANM_RES, HEAPID_SCRATCH);

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�؂�ւ�(�J�[�h���{�^��)
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchClact_ResButtonChg( SCRATCH_CLACT* wk )
{
	ScratchClact_DeleteCellObjectOne( wk, SCRATCH_RES_OBJ_D_CARD );	//�J�[�h���\�[�X�폜
	ScratchClact_ResButtonSet( wk, NNS_G2D_VRAM_TYPE_2DMAIN );		//�{�^�����\�[�X�ǉ�

	CLACT_U_CharManagerSet( wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_CHAR_RES] );//Char�]��
	CLACT_U_PlttManagerSet( wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_PLTT_RES] );//pltt�]��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�F�{�^���Z�b�g
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchClact_ResButtonSet( SCRATCH_CLACT* wk, int vram )
{
	//chara�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_SCRATCH, NARC_scratch_l_oam01_NCGR,
					FALSE, SCRATCH_ID_OBJ_D_BUTTON, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_SCRATCH);

	//pal�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_SCRATCH, NARC_scratch_l_oam01_NCLR,
					FALSE, SCRATCH_ID_OBJ_D_BUTTON, NNS_G2D_VRAM_TYPE_2DMAIN, 
					DISP_SUB2_BUTTON_PAL, HEAPID_SCRATCH);

	//cell�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_SCRATCH, NARC_scratch_l_oam01_NCER,
					FALSE, SCRATCH_ID_OBJ_D_BUTTON, CLACT_U_CELL_RES, HEAPID_SCRATCH);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_BUTTON][CLACT_U_CELLANM_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_SCRATCH, NARC_scratch_l_oam01_NANR,
					FALSE, SCRATCH_ID_OBJ_D_BUTTON, CLACT_U_CELLANM_RES, HEAPID_SCRATCH);

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�F������Z�b�g
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchClact_ResAtariSet( SCRATCH_CLACT* wk, int vram )
{
	//chara�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_ATARI][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_SCRATCH, NARC_scratch_l_oam02_NCGR,
					FALSE, SCRATCH_ID_OBJ_D_ATARI, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_SCRATCH);

	//pal�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_ATARI][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_SCRATCH, NARC_scratch_l_oam02_NCLR,
					FALSE, SCRATCH_ID_OBJ_D_ATARI, NNS_G2D_VRAM_TYPE_2DMAIN, 
					DISP_SUB3_ATARI_PAL, HEAPID_SCRATCH);

	//cell�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_ATARI][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_SCRATCH, NARC_scratch_l_oam02_NCER,
					FALSE, SCRATCH_ID_OBJ_D_ATARI, CLACT_U_CELL_RES, HEAPID_SCRATCH);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[SCRATCH_RES_OBJ_D_ATARI][CLACT_U_CELLANM_RES]=CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_SCRATCH, NARC_scratch_l_oam02_NANR,
					FALSE, SCRATCH_ID_OBJ_D_ATARI, CLACT_U_CELLANM_RES, HEAPID_SCRATCH);

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[���Z�b�g
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
CLACT_WORK_PTR ScratchClact_SetActor( SCRATCH_CLACT* wk, u32 data_no, u32 anm_no, u32 bg_pri, u32 pri, u8 disp )
{
	int i;
	CLACT_HEADER cl_act_header;
	CLACT_WORK_PTR act;
	
	//�Z���A�N�^�[�w�b�_�쐬	
	CLACT_U_MakeHeader(	&cl_act_header,
						//�o�^�������\�[�X��ID(ResObjTbl[id])
						data_no, data_no, data_no, data_no,
						CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
						//0, 0,							//VRAM�]�����t���O�ABG�Ƃ̗D�揇��
						0, bg_pri,						//VRAM�]�����t���O�ABG�Ƃ̗D�揇��(2)
						wk->ResMan[CLACT_U_CHAR_RES],
						wk->ResMan[CLACT_U_PLTT_RES],
						wk->ResMan[CLACT_U_CELL_RES],
						wk->ResMan[CLACT_U_CELLANM_RES],
						NULL,NULL);

	{
		//�o�^���i�[
		CLACT_ADD add;

		add.ClActSet	= wk->ClactSet;
		add.ClActHeader	= &cl_act_header;

		//add.mat.x		= 0;//FX32_CONST(32) ;
		//add.mat.y		= 0;//FX32_CONST(96) ;		//��ʂ͏㉺�A�����Ă���iMAIN����ASUB�����j
		add.mat.x		= 0;
		add.mat.y		= 0;						//��ʂ͏㉺�A�����Ă���iMAIN����ASUB�����j
		add.mat.z		= 0;
		add.sca.x		= FX32_ONE;
		add.sca.y		= FX32_ONE;
		add.sca.z		= FX32_ONE;
		add.rot			= 0;
		add.pri			= pri;
		add.heap		= HEAPID_SCRATCH;

		if( disp == DISP_MAIN ){
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DMAIN;
		}else{
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DSUB;
		}

		//if( disp == DISP_SUB ){
		//	add.mat.y += SUB_SURFACE_Y;				//���W��␳
		//	add.mat.y += SCRATCH_SUB_ACTOR_DISTANCE;//���W��␳
		//}

		//�Z���A�N�^�[�\���J�n
		act = CLACT_Add( &add );
		//if( act == NULL ){
		//	OS_Printf( "CLACT_Add���s\n" );
		//	GF_ASSERT(0);
		//}

#if 0
		CLACT_SetAnmFlag( act, 1 );					//�I�[�g�A�j��
#else
		CLACT_SetAnmFlag( act, 0 );					//��A�j��
#endif

		CLACT_SetAnmFrame( act, FX32_ONE );			//�I�[�g�A�j���[�V�����t���[����ݒ�
		CLACT_AnmChg( act, anm_no );				//�A�j���[�V�����̃V�[�P���X���`�F���W����
	}	

	return act;
}

//--------------------------------------------------------------
/**
 * @brief	2D�Z���I�u�W�F�N�g���
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void ScratchClact_DeleteCellObject(SCRATCH_CLACT* wk)
{
	u8 i;

	//�Z���A�N�^�[���\�[�X���
	for( i=0; i < SCRATCH_RES_OBJ_MAX ;i++ ){
		CLACT_U_CharManagerDelete( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );//char�]���}�l�[�W���[�j��
		CLACT_U_PlttManagerDelete( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );//pltt�]���}�l�[�W���[�j��
	}
		
	//�L�����E�p���b�g�E�Z���E�Z���A�j���̃��\�[�X�}�l�[�W���[�j��
	for( i=0; i < SCRATCH_RESOURCE_NUM ;i++ ){
		CLACT_U_ResManagerDelete( wk->ResMan[i] );
	}

	//�Z���A�N�^�[�Z�b�g�j��
	CLACT_DestSet(wk->ClactSet);

	//OAM�����_���[�j��
	REND_OAM_Delete();

	DeleteCharManager();
	DeletePlttManager();

	return;
}


//==============================================================================================
//
//	static
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�L�����N�^�[�E�p���b�g�}�l�[�W���[������
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void InitCharPlttManager(void)
{
	//�L�����N�^�}�l�[�W���[������
	{
		CHAR_MANAGER_MAKE cm = {
			CHAR_CONT_NUM,						//�L�����N�^���䐔
			CHAR_VRAMTRANS_MAIN_SIZE,			//���C����ʂ�Vram�]���p�ɗp�ӂ���Vram�T�C�Y
			CHAR_VRAMTRANS_SUB_SIZE,			//�T�u��ʂ�Vram�]���p�ɗp�ӂ���Vram�T�C�Y
			HEAPID_SCRATCH						//�g�p����q�[�v
		};
		//InitCharManager( &cm );				//OBJ�}�b�s���O���[�h�ݒ�
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K );
		InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K );
	}

	//�p���b�g�}�l�[�W���[������
#if 0
	InitPlttManager( PLTT_CONT_NUM, HEAPID_SCRATCH );
#else
	InitPlttManager( DISP_PAL_NUM, HEAPID_SCRATCH );
#endif

	//�ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�g�ύX
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
//���N�̃p���b�g���\�[�X�̃C���f�b�N�X
static u8 waku_pal_index_tbl[WAKU_PAL_NUM] = {
	NARC_scratch_l_oam_waku01_NCLR,
	NARC_scratch_l_oam_waku02_NCLR,
	NARC_scratch_l_oam_waku03_NCLR,
	NARC_scratch_l_oam_waku04_NCLR,
	NARC_scratch_l_oam_waku05_NCLR,
	NARC_scratch_l_oam_waku06_NCLR,
	NARC_scratch_l_oam_waku07_NCLR,
	NARC_scratch_l_oam_waku08_NCLR,
};

#if 1
void ScratchClact_ButtonResourcePalChg( SCRATCH_CLACT* wk, int no )
{
	CLACT_U_RES_OBJ_PTR	obj;

	if( no >= WAKU_PAL_NUM ){
		GF_ASSERT(0);
	}
	
	obj = CLACT_U_ResManagerGetIDResObjPtr(wk->ResMan[CLACT_U_PLTT_RES], SCRATCH_RES_OBJ_D_BUTTON);

	//�p���b�g�f�[�^�̃��\�[�X��ύX
	CLACT_U_ResManagerResChgArcPltt(wk->ResMan[CLACT_U_PLTT_RES],
									obj,
									ARC_SCRATCH, waku_pal_index_tbl[no],
									FALSE, HEAPID_SCRATCH );

	//���̊֐�����ACharDataChg��AddVramTransferManager���Ă΂��(gflib/vram_transfer_manager.c)
	CLACT_U_PlttManagerReTrans( obj );
	return;
}
#endif

//--------------------------------------------------------------
/**
 * @brief	���\�[�X�폜
 *
 * @param	wk		SCRATCH_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchClact_DeleteCellObjectOne( SCRATCH_CLACT* wk, int res_id )
{
	u8 i;

	//�Z���A�N�^�[���\�[�X���
	CLACT_U_CharManagerDelete( wk->ResObjTbl[res_id][CLACT_U_CHAR_RES] );//char�]���}�l�[�W���[�j��
	CLACT_U_PlttManagerDelete( wk->ResObjTbl[res_id][CLACT_U_PLTT_RES] );//pltt�]���}�l�[�W���[�j��
		
	CLACT_U_ResManagerResDelete( wk->ResMan[res_id], wk->ResObjTbl[res_id][CLACT_U_CHAR_RES] );
	CLACT_U_ResManagerResDelete( wk->ResMan[res_id], wk->ResObjTbl[res_id][CLACT_U_PLTT_RES] );
	CLACT_U_ResManagerResDelete( wk->ResMan[res_id], wk->ResObjTbl[res_id][CLACT_U_CELL_RES] );
	CLACT_U_ResManagerResDelete( wk->ResMan[res_id], wk->ResObjTbl[res_id][CLACT_U_CELLANM_RES] );
	return;
}


