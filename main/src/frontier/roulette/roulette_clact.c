//==============================================================================================
/**
 * @file	roulette_clact.c
 * @brief	�u�o�g�����[���b�g�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	06.09.05
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "poketool/pokeicon.h"
#include "poketool/pokeparty.h"
#include "itemtool/item.h"
#include "system/procsys.h"
#include "application/pokelist.h"

#include "../roulette_def.h"
#include "roulette_sys.h"
#include "roulette_clact.h"
#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�L�����N�^�[�}�l�[�W���[
#define CHAR_CONT_NUM						(32)//(8)//(6)//(16)//(3)		//�L�����N�^���䐔
#if 0
#define CHAR_VRAMTRANS_MAIN_SIZE			(2048)
#define CHAR_VRAMTRANS_SUB_SIZE				(2048)
#else
#define CHAR_VRAMTRANS_MAIN_SIZE			(1024)
#define CHAR_VRAMTRANS_SUB_SIZE				(1024)
//#define CHAR_VRAMTRANS_MAIN_SIZE			(4096)
//#define CHAR_VRAMTRANS_SUB_SIZE				(4096)
#endif

//�p���b�g�}�l�[�W���[
#define PLTT_ITEMKEEP_NUM					(1)			//�A�C�e�������Ă���A�C�R���p���b�g

enum{
	DISP_MAIN_OBJ_PAL = 8,								//
	DISP_SUB_OBJ_PAL = 2,								//
};


//==============================================================================================
//
//	�f�[�^
//
//==============================================================================================
//���\�[�X�^�C�v�񋓂ɍ��킹��(include/system/clact_util_res.h)
//���\�[�X�}�l�[�W���[�o�^���e�[�u��
static const u8 ResEntryNumTbl[ROULETTE_RESOURCE_NUM] = {
#if 0
	2,			//�L�������\�[�X
	3,			//�p���b�g���\�[�X
	2,			//�Z�����\�[�X
	2,			//�Z���A�j�����\�[�X
#else
	ROULETTE_RES_OBJ_MAX,		//�L�������\�[�X
	ROULETTE_RES_OBJ_MAX,		//�p���b�g���\�[�X
	ROULETTE_RES_OBJ_MAX,		//�Z�����\�[�X
	ROULETTE_RES_OBJ_MAX,		//�Z���A�j�����\�[�X
#endif
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//RouletteClact�֐�
//void			RouletteClact_InitCellActor( ROULETTE_CLACT* wk );
void			RouletteClact_InitCellActor( ROULETTE_CLACT* wk, POKEPARTY* m_party, POKEPARTY* e_pary, u8 type );
CLACT_WORK_PTR	RouletteClact_SetActor( ROULETTE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
void			RouletteClact_DeleteCellObject( ROULETTE_CLACT* wk );

//static
static void		InitCharPlttManager( void );
static void		RouletteClact_Load( ROULETTE_CLACT* wk );
static void		RouletteClact_ItemKeepLoad( ROULETTE_CLACT* wk );
static void		RouletteClact_IconLoad( ROULETTE_CLACT* wk, POKEPARTY* m_party, POKEPARTY* e_party, u8 type );
static void		RouletteClact_ButtonLoad( ROULETTE_CLACT* wk );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[������
 *
 * @param	wk		ROULETTE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
//void RouletteClact_InitCellActor( ROULETTE_CLACT* wk )
void RouletteClact_InitCellActor( ROULETTE_CLACT* wk, POKEPARTY* m_party, POKEPARTY* e_party, u8 type )
{
	int i;

	initVramTransferManagerHeap( 32, HEAPID_ROULETTE );

	//�L�����N�^�[�E�p���b�g�}�l�[�W���[������
	InitCharPlttManager();

	//OAM�}�l�[�W���[�̏�����
	NNS_G2dInitOamManagerModule();

	//���LOAM�}�l�[�W���쐬
	//�����_���pOAM�}�l�[�W���쐬
	//�����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
	REND_OAMInit(	0, 128,				//���C�����OAM�Ǘ��̈�
					0, 32,				//���C����ʃA�t�B���Ǘ��̈�
					0, 128,				//�T�u���OAM�Ǘ��̈�
					0, 32,				//�T�u��ʃA�t�B���Ǘ��̈�
					HEAPID_ROULETTE);
	
	//�Z���A�N�^�[�Z�b�g�̊ȒP������(�쐬���ꂽ�Z���A�N�^�[�Z�b�g���Ԃ�)
	wk->ClactSet = CLACT_U_SetEasyInit( ROULETTE_CLACT_OBJ_MAX, &wk->RendData, HEAPID_ROULETTE );
	
	//���\�[�X�}�l�[�W���[������
	for( i=0; i < ROULETTE_RESOURCE_NUM ;i++ ){		//���\�[�X�}�l�[�W���[�쐬
		wk->ResMan[i] = CLACT_U_ResManagerInit( ResEntryNumTbl[i], i, HEAPID_ROULETTE );
	}

	/***************/
	//	�����
	/***************/
	RouletteClact_ButtonLoad( wk );

	/***************/
	//	����
	/***************/
	RouletteClact_Load( wk );

	/******************************/
	//	����(�A�C�e�������Ă��邩�A�C�R��)
	/******************************/
	RouletteClact_ItemKeepLoad( wk );

	/******************************/
	//	����(�|�P�����A�C�R��)
	/******************************/
	RouletteClact_IconLoad( wk, m_party, e_party, type );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	
	//���\�[�X�}�l�[�W���[����]��
	//for( i=0; i < ROULETTE_RES_OBJ_MAX ;i++ ){
	for( i=0; i < ROULETTE_ID_CHAR_MAX ;i++ ){
		CLACT_U_CharManagerSet( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );	//Char�]��
		//CLACT_U_PlttManagerSet( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );	//�p���b�g�]��
	}

	//���\�[�X�}�l�[�W���[����]��
	//for( i=0; i < ROULETTE_RES_OBJ_MAX ;i++ ){
	for( i=0; i < ROULETTE_ID_PLTT_MAX ;i++ ){
		CLACT_U_PlttManagerSet( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );	//�p���b�g�]��
	}


	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );			//SUB DISP OBJ ON
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );			//MAIN DISP OBJ ON

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[���Z�b�g
 *
 * @param	wk		ROULETTE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
CLACT_WORK_PTR	RouletteClact_SetActor( ROULETTE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp )
{
	int i;
	CLACT_HEADER cl_act_header;
	CLACT_WORK_PTR act;
	
	//OS_Printf( "cell_no = %d\n", cell_no );

	//�Z���A�N�^�[�w�b�_�쐬	
	CLACT_U_MakeHeader(	&cl_act_header,
						//�o�^�������\�[�X��ID(ResObjTbl[id])
						//data_no, data_no, data_no, data_no,
						char_no, pltt_no, cell_no, cell_no,
						CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
						0, bg_pri,						//VRAM�]�����t���O�ABG�Ƃ̗D�揇��
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
		add.heap		= HEAPID_ROULETTE;

		if( disp == DISP_MAIN ){
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DMAIN;
		}else{
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DSUB;
			add.mat.y += SUB_SURFACE_Y;				//���W��␳
		}

		//�Z���A�N�^�[�\���J�n
		act = CLACT_Add(&add);
		//CLACT_SetAnmFlag( act, 0 );				//��A�j��
		CLACT_SetAnmFlag( act, 1 );					//�I�[�g�A�j��
		CLACT_SetAnmFrame( act, FX32_ONE );			//�I�[�g�A�j���[�V�����t���[����ݒ�
		CLACT_AnmChg( act, anm_no );				//�A�j���[�V�����̃V�[�P���X���`�F���W����
	}	

	return act;
}

//--------------------------------------------------------------
/**
 * @brief	2D�Z���I�u�W�F�N�g���
 *
 * @param	wk		ROULETTE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteClact_DeleteCellObject(ROULETTE_CLACT* wk)
{
	u8 i;

	//�Z���A�N�^�[���\�[�X���
	//for( i=0; i < ROULETTE_RES_OBJ_MAX ;i++ ){
	for( i=0; i < ROULETTE_ID_CHAR_MAX ;i++ ){
		CLACT_U_CharManagerDelete( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );//char�]���}�l�[�W���[�j��
	}

	//for( i=0; i < ROULETTE_RES_OBJ_MAX ;i++ ){
	for( i=0; i < ROULETTE_ID_PLTT_MAX ;i++ ){
		CLACT_U_PlttManagerDelete( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );//pltt�]���}�l�[�W���[�j��
	}
		
	//�L�����E�p���b�g�E�Z���E�Z���A�j���̃��\�[�X�}�l�[�W���[�j��
	for( i=0; i < ROULETTE_RESOURCE_NUM ;i++ ){
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
			HEAPID_ROULETTE						//�g�p����q�[�v
		};
		InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
	}

	//�p���b�g�}�l�[�W���[������
	//InitPlttManager((DISP_MAIN_OBJ_PAL + PLTT_ITEMKEEP_NUM + POKEICON_PAL_MAX), HEAPID_ROULETTE);
	InitPlttManager( (DISP_MAIN_OBJ_PAL + DISP_SUB_OBJ_PAL + 
						PLTT_ITEMKEEP_NUM + POKEICON_PAL_MAX), HEAPID_ROULETTE );

	//�ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���[���b�g�f�[�^�̃��[�h
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void RouletteClact_Load( ROULETTE_CLACT* wk )
{
	//chara�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_CSR][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_FRONTIER_OBJ, BR_OBJ_NCGR_BIN,
					1, ROULETTE_ID_OBJ_CSR, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_ROULETTE);

	//pal�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_CSR][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_FRONTIER_OBJ, BR_OBJ_NCLR,
					FALSE, ROULETTE_ID_OBJ_CSR, NNS_G2D_VRAM_TYPE_2DMAIN, 
					DISP_MAIN_OBJ_PAL,HEAPID_ROULETTE);

	//cell�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_CSR][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_FRONTIER_OBJ, BR_OBJ_NCER_BIN,
					1, ROULETTE_ID_OBJ_CSR, CLACT_U_CELL_RES, HEAPID_ROULETTE);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_CSR][CLACT_U_CELLANM_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_FRONTIER_OBJ, BR_OBJ_NANR_BIN,
					1, ROULETTE_ID_OBJ_CSR, CLACT_U_CELLANM_RES, HEAPID_ROULETTE);

	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e�������Ă���A�C�R���̃��[�h
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void RouletteClact_ItemKeepLoad( ROULETTE_CLACT* wk )
{
	ARCHANDLE* p_handle;
	u32	i;
	
	p_handle = ArchiveDataHandleOpen( ARC_PLIST_GRA,  HEAPID_ROULETTE ); 
	
	//chara�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ITEMKEEP][CLACT_U_CHAR_RES] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
		wk->ResMan[CLACT_U_CHAR_RES], p_handle, Pokelist_ItemIconCgxArcGet(),
		0, ROULETTE_ID_OBJ_ITEMKEEP, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_ROULETTE );

	//pal�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ITEMKEEP][CLACT_U_PLTT_RES] = 
		CLACT_U_ResManagerResAddArcPltt(
		wk->ResMan[CLACT_U_PLTT_RES], ARC_PLIST_GRA, Pokelist_ItemIconPalArcGet(),
		FALSE, ROULETTE_ID_OBJ_ITEMKEEP, NNS_G2D_VRAM_TYPE_2DMAIN,POKEICON_PAL_MAX,HEAPID_ROULETTE );

	//cell�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ITEMKEEP][CLACT_U_CELL_RES] =
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELL_RES], p_handle, Pokelist_ItemIconCellArcGet(),
		0, ROULETTE_ID_OBJ_ITEMKEEP, CLACT_U_CELL_RES, HEAPID_ROULETTE );

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ITEMKEEP][CLACT_U_CELLANM_RES] = 
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELLANM_RES], p_handle, Pokelist_ItemIconCAnmArcGet(),
		0, ROULETTE_ID_OBJ_ITEMKEEP, CLACT_U_CELLANM_RES, HEAPID_ROULETTE );

	ArchiveDataHandleClose( p_handle );	
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �|�P�����A�C�R���̃��[�h
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void RouletteClact_IconLoad( ROULETTE_CLACT* wk, POKEPARTY* m_party, POKEPARTY* e_party, u8 type )
{
	POKEMON_PARAM* m_poke;
	POKEMON_PARAM* e_poke;
	ARCHANDLE* p_handle;
	u32	i;

	//chara�ǂݍ���
	p_handle = ArchiveDataHandleOpen( ARC_POKEICON, HEAPID_ROULETTE );	

	//pal�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ICON1][CLACT_U_PLTT_RES] = 
			CLACT_U_ResManagerResAddArcPltt(
				wk->ResMan[CLACT_U_PLTT_RES],
				ARC_POKEICON, PokeIconPalArcIndexGet(),
				FALSE, ROULETTE_ID_PLTT_ICON, NNS_G2D_VRAM_TYPE_2DMAIN,
				POKEICON_PAL_MAX,HEAPID_ROULETTE);

	//cell�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ICON1][CLACT_U_CELL_RES] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				wk->ResMan[CLACT_U_CELL_RES],
				//p_handle, PokeIcon64kCellArcIndexGet(),
				//p_handle, PokeIconCellArcIndexGet(),
				p_handle, PokeIconAnmCellArcIndexGet(),
				0, ROULETTE_ID_CELL_ICON, CLACT_U_CELL_RES, HEAPID_ROULETTE);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_ICON1][CLACT_U_CELLANM_RES] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				wk->ResMan[CLACT_U_CELLANM_RES],
				//p_handle, PokeIcon64kCellAnmArcIndexGet(),
				//p_handle, PokeIconCellAnmArcIndexGet(),
				p_handle, PokeIconAnmCellAnmArcIndexGet(),
				0, ROULETTE_ID_CELLANM_ICON, CLACT_U_CELLANM_RES, HEAPID_ROULETTE);

	//�ʐM�ΐ펞�̖���2+2 �G2+2�̍ő�8�C
	for( i=0; i < ROULETTE_COMM_POKE_TOTAL_NUM ;i++ ){

		if( i == 3 ){
			//�ʐM�^�C�v�łȂ���
			if( type == FALSE ){
				m_poke = PokeParty_GetMemberPointer( m_party, 0 );	//�g�p���Ȃ�������݂̂��Ă���
				e_poke = PokeParty_GetMemberPointer( e_party, 0 );	//�g�p���Ȃ�������݂̂��Ă���
			}else{
				m_poke = PokeParty_GetMemberPointer( m_party, i );
				e_poke = PokeParty_GetMemberPointer( e_party, i );
			}
		}else{
			m_poke = PokeParty_GetMemberPointer( m_party, i );
			e_poke = PokeParty_GetMemberPointer( e_party, i );
		}

		wk->ResObjTbl[ROULETTE_RES_OBJ_ICON1+i][CLACT_U_CHAR_RES] = 
					CLACT_U_ResManagerResAddArcChar_ArcHandle(
					wk->ResMan[CLACT_U_CHAR_RES],
					p_handle, PokeIconCgxArcIndexGetByPP(m_poke),
					0, ROULETTE_ID_CHAR_ICON1+i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_ROULETTE );

		wk->ResObjTbl[ROULETTE_RES_OBJ_ICON5+i][CLACT_U_CHAR_RES] = 
					CLACT_U_ResManagerResAddArcChar_ArcHandle(
					wk->ResMan[CLACT_U_CHAR_RES],
					p_handle, PokeIconCgxArcIndexGetByPP(e_poke),
					0, ROULETTE_ID_CHAR_ICON5+i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_ROULETTE );
	}

	ArchiveDataHandleClose( p_handle );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���[���b�g��~�{�^���f�[�^�̃��[�h
 *
 * @param	wk		���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void RouletteClact_ButtonLoad( ROULETTE_CLACT* wk )
{
	//chara�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_BUTTON][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_FRONTIER_OBJ, BR_BUTTON_NCGR_BIN,
					1, ROULETTE_ID_OBJ_BUTTON, NNS_G2D_VRAM_TYPE_2DSUB, HEAPID_ROULETTE);

	//pal�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_BUTTON][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_FRONTIER_OBJ, BR_BUTTON_NCLR,
					FALSE, ROULETTE_ID_OBJ_BUTTON, NNS_G2D_VRAM_TYPE_2DSUB, 
					DISP_SUB_OBJ_PAL,HEAPID_ROULETTE);

	//cell�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_BUTTON][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_FRONTIER_OBJ, BR_BUTTON_NCER_BIN,
					1, ROULETTE_ID_OBJ_BUTTON, CLACT_U_CELL_RES, HEAPID_ROULETTE);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[ROULETTE_RES_OBJ_BUTTON][CLACT_U_CELLANM_RES]=CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_FRONTIER_OBJ, BR_BUTTON_NANR_BIN,
					1, ROULETTE_ID_OBJ_BUTTON, CLACT_U_CELLANM_RES, HEAPID_ROULETTE);

	return;
}


