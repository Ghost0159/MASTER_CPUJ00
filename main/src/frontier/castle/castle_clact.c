//==============================================================================================
/**
 * @file	castle_clact.c
 * @brief	�u�o�g���L���b�X���v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	06.07.05
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

#include "castle_sys.h"
#include "castle_clact.h"
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
	DISP_MAIN_OBJ_PAL = 4,
};


//==============================================================================================
//
//	�f�[�^
//
//==============================================================================================
//���\�[�X�^�C�v�񋓂ɍ��킹��(include/system/clact_util_res.h)
//���\�[�X�}�l�[�W���[�o�^���e�[�u��
static const u8 ResEntryNumTbl[CASTLE_RESOURCE_NUM] = {
#if 0
	2,			//�L�������\�[�X
	3,			//�p���b�g���\�[�X
	2,			//�Z�����\�[�X
	2,			//�Z���A�j�����\�[�X
#else
	RES_OBJ_MAX,		//�L�������\�[�X
	RES_OBJ_MAX,		//�p���b�g���\�[�X
	RES_OBJ_MAX,		//�Z�����\�[�X
	RES_OBJ_MAX,		//�Z���A�j�����\�[�X
#endif
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//CastleClact�֐�
//void			CastleClact_InitCellActor( CASTLE_CLACT* wk );
void			CastleClact_InitCellActor( CASTLE_CLACT* wk, POKEPARTY* party, u8 type );
CLACT_WORK_PTR	CastleClact_SetActor( CASTLE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
void			CastleClact_DeleteCellObject( CASTLE_CLACT* wk );

//�A�C�e���A�C�R��
void			CastleClact_ItemIconCharChange( CASTLE_CLACT* wk, u16 item );
void			CastleClact_ItemIconPlttChange( CASTLE_CLACT* wk, u16 item );

//static
static void		InitCharPlttManager( void );
static void		CastleClact_ItemKeepLoad( CASTLE_CLACT* wk );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[������
 *
 * @param	wk		CASTLE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
//void CastleClact_InitCellActor( CASTLE_CLACT* wk )
void CastleClact_InitCellActor( CASTLE_CLACT* wk, POKEPARTY* party, u8 type )
{
	int i;
	ARCHANDLE* p_handle;
	POKEMON_PARAM* poke;

	initVramTransferManagerHeap( 32, HEAPID_CASTLE );

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
					HEAPID_CASTLE);
	
	//�Z���A�N�^�[�Z�b�g�̊ȒP������(�쐬���ꂽ�Z���A�N�^�[�Z�b�g���Ԃ�)
	wk->ClactSet = CLACT_U_SetEasyInit( CLACT_OBJ_MAX, &wk->RendData, HEAPID_CASTLE );
	
	//���\�[�X�}�l�[�W���[������
	for( i=0; i < CASTLE_RESOURCE_NUM ;i++ ){		//���\�[�X�}�l�[�W���[�쐬
		wk->ResMan[i] = CLACT_U_ResManagerInit( ResEntryNumTbl[i], i, HEAPID_CASTLE );
	}

	/***************/
	//	�����
	/***************/

	/***************/
	//	����
	/***************/
	//chara�ǂݍ���
	wk->ResObjTbl[RES_OBJ_CSR][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_FRONTIER_OBJ, BC_OBJ_NCGR_BIN,
					1, ID_OBJ_CSR, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_CASTLE);

	//pal�ǂݍ���
	wk->ResObjTbl[RES_OBJ_CSR][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
					wk->ResMan[CLACT_U_PLTT_RES],
					ARC_FRONTIER_OBJ, BC_OBJ_NCLR,
					FALSE, ID_OBJ_CSR, NNS_G2D_VRAM_TYPE_2DMAIN, DISP_MAIN_OBJ_PAL,HEAPID_CASTLE);

	//cell�ǂݍ���
	wk->ResObjTbl[RES_OBJ_CSR][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_FRONTIER_OBJ, BC_OBJ_NCER_BIN,
					1, ID_OBJ_CSR, CLACT_U_CELL_RES, HEAPID_CASTLE);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[RES_OBJ_CSR][CLACT_U_CELLANM_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_FRONTIER_OBJ, BC_OBJ_NANR_BIN,
					1, ID_OBJ_CSR, CLACT_U_CELLANM_RES, HEAPID_CASTLE);

	/******************************/
	//	����(�A�C�e���A�C�R��)
	/******************************/
#if 1
	//chara�ǂݍ���
	p_handle = ArchiveDataHandleOpen( ARC_ITEMICON, HEAPID_CASTLE );	

	wk->ResObjTbl[RES_OBJ_ITEMICON][CLACT_U_CHAR_RES] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
		wk->ResMan[CLACT_U_CHAR_RES], p_handle, GetItemIndex(0,ITEM_GET_ICON_CGX),
		0, ID_OBJ_ITEMICON, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_CASTLE );

	//pal�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMICON][CLACT_U_PLTT_RES] = 
		CLACT_U_ResManagerResAddArcPltt(
		wk->ResMan[CLACT_U_PLTT_RES], ARC_ITEMICON, GetItemIndex(0,ITEM_GET_ICON_PAL),
		FALSE, ID_OBJ_ITEMICON, NNS_G2D_VRAM_TYPE_2DMAIN,POKEICON_PAL_MAX,HEAPID_CASTLE );

	//cell�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMICON][CLACT_U_CELL_RES] =
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELL_RES], p_handle, ItemIconCellGet(),
		0, ID_OBJ_ITEMICON, CLACT_U_CELL_RES, HEAPID_CASTLE );

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMICON][CLACT_U_CELLANM_RES] = 
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELLANM_RES], p_handle, ItemIconCAnmGet(),
		0, ID_OBJ_ITEMICON, CLACT_U_CELLANM_RES, HEAPID_CASTLE );

	ArchiveDataHandleClose( p_handle );	
#endif

	/******************************/
	//	����(�A�C�e�������Ă��邩�A�C�R��)
	/******************************/

	CastleClact_ItemKeepLoad( wk );

	/******************************/
	//	����(�|�P�����A�C�R��)
	/******************************/

	//chara�ǂݍ���
	p_handle = ArchiveDataHandleOpen( ARC_POKEICON, HEAPID_CASTLE );	

	//pal�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ICON1][CLACT_U_PLTT_RES] = 
			CLACT_U_ResManagerResAddArcPltt(
				wk->ResMan[CLACT_U_PLTT_RES],
				ARC_POKEICON, PokeIconPalArcIndexGet(),
				FALSE, ID_PLTT_ICON, NNS_G2D_VRAM_TYPE_2DMAIN,POKEICON_PAL_MAX,HEAPID_CASTLE);

	//cell�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ICON1][CLACT_U_CELL_RES] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				wk->ResMan[CLACT_U_CELL_RES],
				//p_handle, PokeIcon64kCellArcIndexGet(),
				//p_handle, PokeIconCellArcIndexGet(),
				p_handle, PokeIconAnmCellArcIndexGet(),
				0, ID_CELL_ICON, CLACT_U_CELL_RES, HEAPID_CASTLE);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ICON1][CLACT_U_CELLANM_RES] = 
			CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
				wk->ResMan[CLACT_U_CELLANM_RES],
				//p_handle, PokeIcon64kCellAnmArcIndexGet(),
				//p_handle, PokeIconCellAnmArcIndexGet(),
				p_handle, PokeIconAnmCellAnmArcIndexGet(),
				0, ID_CELLANM_ICON, CLACT_U_CELLANM_RES, HEAPID_CASTLE);

	for( i=0; i < 4 ;i++ ){

		if( i == 3 ){

			//�ʐM�^�C�v�łȂ���
			if( type == FALSE ){
				poke =  PokeParty_GetMemberPointer( party, 0 );		//�g�p���Ȃ�������݂̂��Ă���
			}else{
				poke =  PokeParty_GetMemberPointer( party, i );
			}
		}else{
			poke =  PokeParty_GetMemberPointer( party, i );
		}

		wk->ResObjTbl[RES_OBJ_ICON1+i][CLACT_U_CHAR_RES] = 
				CLACT_U_ResManagerResAddArcChar_ArcHandle(
					wk->ResMan[CLACT_U_CHAR_RES],
					p_handle, PokeIconCgxArcIndexGetByPP(poke),
					0, ID_CHAR_ICON1+i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_CASTLE );

	}

	ArchiveDataHandleClose( p_handle );

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	
	//���\�[�X�}�l�[�W���[����]��
	//for( i=0; i < RES_OBJ_MAX ;i++ ){
	for( i=0; i < ID_CHAR_MAX ;i++ ){
		CLACT_U_CharManagerSet( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );	//Char�]��
		//CLACT_U_PlttManagerSet( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );	//�p���b�g�]��
	}

	//���\�[�X�}�l�[�W���[����]��
	//for( i=0; i < RES_OBJ_MAX ;i++ ){
	for( i=0; i < ID_PLTT_MAX ;i++ ){
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
 * @param	wk		CASTLE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
CLACT_WORK_PTR	CastleClact_SetActor( CASTLE_CLACT* wk, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, u32 pri, int bg_pri, u8 disp )
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

		if( disp == DISP_MAIN ){
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DMAIN;
		}else{
			add.DrawArea= NNS_G2D_VRAM_TYPE_2DSUB;
		}

		add.heap		= HEAPID_CASTLE;

		if( disp == DISP_SUB ){						//���W��␳
			add.mat.y += SUB_SURFACE_Y;
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
 * @param	wk		CASTLE_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleClact_DeleteCellObject(CASTLE_CLACT* wk)
{
	u8 i;

	//�Z���A�N�^�[���\�[�X���
	//for( i=0; i < RES_OBJ_MAX ;i++ ){
	for( i=0; i < ID_CHAR_MAX ;i++ ){
		CLACT_U_CharManagerDelete( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );//char�]���}�l�[�W���[�j��
	}

	//for( i=0; i < RES_OBJ_MAX ;i++ ){
	for( i=0; i < ID_PLTT_MAX ;i++ ){
		CLACT_U_PlttManagerDelete( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );//pltt�]���}�l�[�W���[�j��
	}
		
	//�L�����E�p���b�g�E�Z���E�Z���A�j���̃��\�[�X�}�l�[�W���[�j��
	for( i=0; i < CASTLE_RESOURCE_NUM ;i++ ){
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
//	�A�C�e���A�C�R��
//
//==============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e���A�C�R���ύX(CHAR)
 *
 * @param	wk		CASTLE_CLACT�^�̃|�C���^
 * @param	item	�A�C�e���ԍ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void CastleClact_ItemIconCharChange( CASTLE_CLACT* wk, u16 item )
{
	CLACT_U_RES_OBJ_PTR	obj;
	
	obj = CLACT_U_ResManagerGetIDResObjPtr( wk->ResMan[CLACT_U_CHAR_RES], ID_OBJ_ITEMICON );

	CLACT_U_ResManagerResChgArcChar(	wk->ResMan[CLACT_U_CHAR_RES], obj, 
										ARC_ITEMICON, GetItemIndex(item,ITEM_GET_ICON_CGX), 
										0, HEAPID_CASTLE );

	//���̊֐�����ACharDataChg��AddVramTransferManager���Ă΂��(gflib/vram_transfer_manager.c)
	CLACT_U_CharManagerReTrans( obj );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e���A�C�R���ύX(PAL)
 *
 * @param	wk		CASTLE_CLACT�^�̃|�C���^
 * @param	item	�A�C�e���ԍ�
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void CastleClact_ItemIconPlttChange( CASTLE_CLACT* wk, u16 item )
{
	CLACT_U_RES_OBJ_PTR	obj;
	
	obj = CLACT_U_ResManagerGetIDResObjPtr( wk->ResMan[CLACT_U_PLTT_RES], ID_OBJ_ITEMICON );

	CLACT_U_ResManagerResChgArcPltt(	wk->ResMan[CLACT_U_PLTT_RES], obj, 
										ARC_ITEMICON, GetItemIndex(item,ITEM_GET_ICON_PAL), 
										0, HEAPID_CASTLE );

	CLACT_U_PlttManagerReTrans( obj );
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
			HEAPID_CASTLE						//�g�p����q�[�v
		};
#if 1
		//InitCharManager( &cm );				//OBJ�}�b�s���O���[�h�ݒ�
		InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K );
#else
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K );
		InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K );
#endif
	}

	//�p���b�g�}�l�[�W���[������
#if 0
	InitPlttManager( DISP_MAIN_OBJ_PAL, HEAPID_CASTLE );
#else
	InitPlttManager( (DISP_MAIN_OBJ_PAL + PLTT_ITEMKEEP_NUM + POKEICON_PAL_MAX), HEAPID_CASTLE );
#endif

	//�ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

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
static void CastleClact_ItemKeepLoad( CASTLE_CLACT* wk )
{
	ARCHANDLE* p_handle;
	u32	i;
	
	p_handle = ArchiveDataHandleOpen( ARC_PLIST_GRA,  HEAPID_CASTLE ); 
	
	//chara�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMKEEP][CLACT_U_CHAR_RES] = 
		CLACT_U_ResManagerResAddArcChar_ArcHandle(
		wk->ResMan[CLACT_U_CHAR_RES], p_handle, Pokelist_ItemIconCgxArcGet(),
		0, ID_OBJ_ITEMKEEP, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_CASTLE );

	//pal�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMKEEP][CLACT_U_PLTT_RES] = 
		CLACT_U_ResManagerResAddArcPltt(
		wk->ResMan[CLACT_U_PLTT_RES], ARC_PLIST_GRA, Pokelist_ItemIconPalArcGet(),
		FALSE, ID_OBJ_ITEMKEEP, NNS_G2D_VRAM_TYPE_2DMAIN,POKEICON_PAL_MAX,HEAPID_CASTLE );

	//cell�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMKEEP][CLACT_U_CELL_RES] =
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELL_RES], p_handle, Pokelist_ItemIconCellArcGet(),
		0, ID_OBJ_ITEMKEEP, CLACT_U_CELL_RES, HEAPID_CASTLE );

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[RES_OBJ_ITEMKEEP][CLACT_U_CELLANM_RES] = 
		CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
		wk->ResMan[CLACT_U_CELLANM_RES], p_handle, Pokelist_ItemIconCAnmArcGet(),
		0, ID_OBJ_ITEMKEEP, CLACT_U_CELLANM_RES, HEAPID_CASTLE );

	ArchiveDataHandleClose( p_handle );	
	return;
}


