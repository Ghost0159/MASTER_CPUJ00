//==============================================================================================
/**
 * @file	factory_clact.c
 * @brief	�u�o�g���t�@�N�g���[�v�Z���A�N�^�[
 * @author	Satoshi Nohara
 * @date	2007.03.14
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"

#include "factory_sys.h"
#include "factory_clact.h"
#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�L�����N�^�[�}�l�[�W���[
#define CHAR_CONT_NUM						(32)//(3)		//�L�����N�^���䐔
#define CHAR_VRAMTRANS_MAIN_SIZE			(2048)
#define CHAR_VRAMTRANS_SUB_SIZE				(2048)
//#define CHAR_VRAMTRANS_MAIN_SIZE			(1024)
//#define CHAR_VRAMTRANS_SUB_SIZE				(1024)
//#define CHAR_VRAMTRANS_MAIN_SIZE			(3096)
//#define CHAR_VRAMTRANS_SUB_SIZE				(3096)

#define DISP_MAIN_OBJ_PAL					(8)

//�p���b�g�}�l�[�W���[
#define PLTT_CONT_NUM						(DISP_MAIN_OBJ_PAL)		//�Ǘ��p���b�g��


//==============================================================================================
//
//	�f�[�^
//
//==============================================================================================
//���\�[�X�^�C�v�񋓂ɍ��킹��(include/system/clact_util_res.h)
//���\�[�X�}�l�[�W���[�o�^���e�[�u��
static const u8 ResEntryNumTbl[FACTORY_RESOURCE_NUM] = {
#if 0
	2,			//�L�������\�[�X
	3,			//�p���b�g���\�[�X
	2,			//�Z�����\�[�X
	2,			//�Z���A�j�����\�[�X
#else
	FACTORY_RES_OBJ_MAX,		//�L�������\�[�X
	FACTORY_RES_OBJ_MAX,		//�p���b�g���\�[�X
	FACTORY_RES_OBJ_MAX,		//�Z�����\�[�X
	FACTORY_RES_OBJ_MAX,		//�Z���A�j�����\�[�X
#endif
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//FactoryClact�֐�
void			FactoryClact_InitCellActor( FACTORY_CLACT* wk );
CLACT_WORK_PTR	FactoryClact_SetActor( FACTORY_CLACT* wk, u32 data_no, u32 anm_no, u32 pri, int bg_pri, u8 disp );
void			FactoryClact_DeleteCellObject( FACTORY_CLACT* wk );

//static
static void		InitCharPlttManager( void );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Z���A�N�^�[������
 *
 * @param	wk		FACTORY_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryClact_InitCellActor( FACTORY_CLACT* wk )
{
	int i;

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
					HEAPID_FACTORY);
	
	//�Z���A�N�^�[�Z�b�g�̊ȒP������(�쐬���ꂽ�Z���A�N�^�[�Z�b�g���Ԃ�)
	wk->ClactSet = CLACT_U_SetEasyInit( FACTORY_CLACT_OBJ_MAX, &wk->RendData, HEAPID_FACTORY );
	
	//���\�[�X�}�l�[�W���[������
	for( i=0; i < FACTORY_RESOURCE_NUM ;i++ ){		//���\�[�X�}�l�[�W���[�쐬
		wk->ResMan[i] = CLACT_U_ResManagerInit( ResEntryNumTbl[i], i, HEAPID_FACTORY );
	}

	/***************/
	//	�����
	/***************/

	/***************/
	//	����
	/***************/
	//chara�ǂݍ���
	wk->ResObjTbl[FACTORY_RES_OBJ_BALL][CLACT_U_CHAR_RES] = CLACT_U_ResManagerResAddArcChar(
					wk->ResMan[CLACT_U_CHAR_RES],
					ARC_FRONTIER_OBJ, BF_RENTAL_OAM_NCGR_BIN,
					1, FACTORY_ID_OBJ_BALL, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_FACTORY);

	//pal�ǂݍ���
	wk->ResObjTbl[FACTORY_RES_OBJ_BALL][CLACT_U_PLTT_RES] = CLACT_U_ResManagerResAddArcPltt(
			wk->ResMan[CLACT_U_PLTT_RES],
			ARC_FRONTIER_OBJ, BF_RENTAL_OAM_NCLR,
			FALSE, FACTORY_ID_OBJ_BALL, NNS_G2D_VRAM_TYPE_2DMAIN, DISP_MAIN_OBJ_PAL,HEAPID_FACTORY);

	//cell�ǂݍ���
	wk->ResObjTbl[FACTORY_RES_OBJ_BALL][CLACT_U_CELL_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELL_RES],
					ARC_FRONTIER_OBJ, BF_RENTAL_OAM_NCER_BIN,
					1, FACTORY_ID_OBJ_BALL, CLACT_U_CELL_RES, HEAPID_FACTORY);

	//�����֐���anim�ǂݍ���
	wk->ResObjTbl[FACTORY_RES_OBJ_BALL][CLACT_U_CELLANM_RES] = CLACT_U_ResManagerResAddArcKindCell(
					wk->ResMan[CLACT_U_CELLANM_RES],
					ARC_FRONTIER_OBJ, BF_RENTAL_OAM_NANR_BIN,
					1, FACTORY_ID_OBJ_BALL, CLACT_U_CELLANM_RES, HEAPID_FACTORY);

	//���\�[�X�}�l�[�W���[����]��
	for( i=0; i < FACTORY_RES_OBJ_MAX ;i++ ){
		CLACT_U_CharManagerSet( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );	//Char�]��
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
 * @param	wk		FACTORY_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
CLACT_WORK_PTR FactoryClact_SetActor( FACTORY_CLACT* wk, u32 data_no, u32 anm_no, u32 pri, int bg_pri, u8 disp )
{
	int i;
	CLACT_HEADER cl_act_header;
	CLACT_WORK_PTR act;
	
	//�Z���A�N�^�[�w�b�_�쐬	
	CLACT_U_MakeHeader(	&cl_act_header,
						//�o�^�������\�[�X��ID(ResObjTbl[id])
						data_no, data_no, data_no, data_no,
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

		add.heap		= HEAPID_FACTORY;

		if( disp == DISP_SUB ){						//���W��␳
			add.mat.y += SUB_SURFACE_Y;
		}

		//�Z���A�N�^�[�\���J�n
		act = CLACT_Add(&add);
		//CLACT_SetAnmFlag( act, 0 );				//��A�j��
		CLACT_SetAnmFlag( act, 1 );					//�I�[�g�A�j��
		CLACT_AnmChg( act, anm_no );
	}	

	return act;
}

//--------------------------------------------------------------
/**
 * @brief	2D�Z���I�u�W�F�N�g���
 *
 * @param	wk		FACTORY_CLACT�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void FactoryClact_DeleteCellObject(FACTORY_CLACT* wk)
{
	u8 i;

	//�Z���A�N�^�[���\�[�X���
	for( i=0; i < FACTORY_RES_OBJ_MAX ;i++ ){
		CLACT_U_CharManagerDelete( wk->ResObjTbl[i][CLACT_U_CHAR_RES] );//char�]���}�l�[�W���[�j��
		CLACT_U_PlttManagerDelete( wk->ResObjTbl[i][CLACT_U_PLTT_RES] );//pltt�]���}�l�[�W���[�j��
	}
		
	//�L�����E�p���b�g�E�Z���E�Z���A�j���̃��\�[�X�}�l�[�W���[�j��
	for( i=0; i < FACTORY_RESOURCE_NUM ;i++ ){
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
			HEAPID_FACTORY						//�g�p����q�[�v
		};
		//InitCharManager(&cm);					//OBJ�}�b�s���O���[�h�ݒ�
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K );
		InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K );
		//InitCharManagerReg( &cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K );
	}

	//�p���b�g�}�l�[�W���[������
	InitPlttManager( PLTT_CONT_NUM, HEAPID_FACTORY );

	//�ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

	return;
}


