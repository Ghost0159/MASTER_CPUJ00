//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dobj.c
 *	@brief		�RD�r���{�[�h�A�N�^�[�I�u�W�F�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.01
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/blact.h"
#include "gflib/res_manager.h"
#include "gflib/texanm_sys.h"
#include "gflib/bounding_box.h"

#include "system/arc_util.h"
#include "system/d3dobj.h"

#include "field/fieldobj_code.h"

#include "data/mmodel/mmodel.naix"

#include "graphic/wifi_lobby_other.naix"

#include "wflby_def.h"
#include "wflby_3dmatrix.h"
#include "wflby_def.h"

#include "wflby_3dobj.h"

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

//-------------------------------------
///	��l���̕\���ʒu�̌덷
//=====================================
#define WFLBY_3DOBJ_OFS_Z	( FX32_CONST(16) )


//-------------------------------------
///	�U���������A�P���O����`��t���[��
//=====================================
#define WFLBY_3DOBJ_TRUN_FIRST_FRAME	(4)



//-------------------------------------
///	����A�j��	��]
//=====================================
#define WFLBY_3DOBJ_ANM_ROTA_1SYNC	( 4 )	// 1�y���������Ă���V���N��
static const u8	sc_WFLBY_3DOBJ_ANM_WAY[ WF2DMAP_WAY_NUM ] = {
	WF2DMAP_WAY_DOWN,
	WF2DMAP_WAY_LEFT,
	WF2DMAP_WAY_UP,
	WF2DMAP_WAY_RIGHT,
};

//-------------------------------------
///	����A�j��	���o�^�o�^
//=====================================
#define WFLBY_3DOBJ_ANM_BATA_FRAME	( 4 )


//-------------------------------------
///	�e�ɂ���
//	Y���W�͏�ɏ��̈ʒu
//	���̑��͓��삷��
//=====================================
#define WFLBY_3DOBJ_SHADOW_OFS_X		(0)
#define WFLBY_3DOBJ_SHADOW_MAT_Y	(WFLBY_3DMATRIX_FLOOR_MAT+FX32_CONST(2))
#define WFLBY_3DOBJ_SHADOW_OFS_Z	(FX32_CONST(-8))
#define WFLBY_3DOBJ_SHADOW_OBJID	( 20 )


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�l���RD�I�u�W�F�N�g�f�[�^
//=====================================
typedef struct {
	u16	objid;			// �Ή�����l��ID
	u16 tex:15;			// �e�N�X�`���t�@�C���i���o�[
	u16 anm:1;			// Vram�]���A�j���t���O
} WFLBY_3DMDL_DATA;


//-------------------------------------
///	�A�N�^�[�ŕ\���ł��Ȃ��RD�I�u�W�F���\�[�X
//=====================================
typedef struct {
	D3DOBJ_MDL			mdlres;
	u32					alpha;
} WFLBY_3DMDLRES;



//-------------------------------------
///	���[�N�\����
//=====================================
typedef struct _WFLBY_3DOBJWK{
	u8						updata : 4;	// �X�V�t���O
	u8						culling : 2;// �J�����O
	u8						objdraw : 2;// �A�N�^�[�\��
	u8						anmflag : 1;// ����A�j���̗L��
	u8						anm : 7;	// ����A�j��
	u8						anmframe;	// �A�j���t���[��
	u8						anmspeed;	// �A�j���X�s�[�h
	
	const WF2DMAP_OBJWK*	cp_objwk;	// �Q�ƃI�u�W�F�N�g�f�[�^
	BLACT_WORK_PTR			p_act;		// �`��A�N�^�[
	D3DOBJ					shadow;		// �e���f��

	u8	lastst;			// 1�O�̏��
	u8	lastanm;		// 1�O�̕ۑ��A�j��(BLACT �A�j���I�t�Z�b�g)
	u16 lastfrm;		// 1�O�̃t���[��
	fx32 lastframe;		// 1�O�̕ۑ��A�j���I���t���[��(BLACT frame)

	int	set_light_msk;	// �ݒ肵�����C�g�t���O
}WFLBY_3DOBJWK;

//-------------------------------------
///	�V�X�e���\����
//=====================================
typedef struct _WFLBY_3DOBJSYS{

	// ���\�[�X�}�l�[�W��
	RES_MANAGER_PTR		p_mdlresman;
	RES_MANAGER_PTR		p_anmresman;
	TEXRES_MANAGER_PTR	p_texresman;

	// �r���{�[�h�A�N�^�[�V�X�e��
	BLACT_SET_PTR		p_blact;

	// �`��I�u�W�F�N�g�e�[�u��
	WFLBY_3DOBJWK*		p_obj;
	u32					objnum;

	// �e���f�����\�[�X�f�[�^
	WFLBY_3DMDLRES		shadowres;	

	// ���C�g�}�X�N
	int					mdl_light_msk;

}WFLBY_3DOBJSYS;



//-----------------------------------------------------------------------------
/**
 *				���f���f�[�^
 */
//-----------------------------------------------------------------------------
#define WFLBY_3DMDL_MDLDATA_NUM	(1)
#define WFLBY_3DMDL_MDLDATA_IDX	(NARC_wifi_lobby_other_wifi_hero_nsbmd)

//-----------------------------------------------------------------------------
/**
 *				�A�j���f�[�^
 */
//-----------------------------------------------------------------------------
enum{
	WFLBY_3DMDL_ANMDATA_NORMAL,
	WFLBY_3DMDL_ANMDATA_TRANS,
	WFLBY_3DMDL_ANMDATA_NUM
};
static const u16 sc_WFLBY_ANMDATA[ WFLBY_3DMDL_ANMDATA_NUM ] = {
	NARC_mmodel_charaobj_itpcv_dat,		// �ʏ�A�j���p
	NARC_mmodel_hero_itpcv_dat			// �]���A�j���p
};

//-------------------------------------
///		�A�j���t���[���f�[�^
//=====================================
enum{
	// �����A�j��
	WFLBY_3DOBJ_ANM_TYPE_UP,
	WFLBY_3DOBJ_ANM_TYPE_DOWN,
	WFLBY_3DOBJ_ANM_TYPE_LEFT,
	WFLBY_3DOBJ_ANM_TYPE_RIGHT,

	// ����A�j��
	WFLBY_3DOBJ_ANM_TYPE_RUN_UP,
	WFLBY_3DOBJ_ANM_TYPE_RUN_DOWN,
	WFLBY_3DOBJ_ANM_TYPE_RUN_LEFT,
	WFLBY_3DOBJ_ANM_TYPE_RUN_RIGHT,
};
#define WFLBY_3DOBJ_ONEANM_FRAME		( 16 )
#define WFLBY_3DOBJ_ONEANM_HFRAME		( 8 )	// ����
#define WFLBY_3DOBJ_ONEANM_RUNFRAME		( 4 )	// ����
static const BLACT_ANIME_TBL WFLBY_3DOBJ_ANM_TR_FRAME_DATA[] =
{
	{ 0, 15, BLACT_ANIM_LOOP },
	{ 16, 31, BLACT_ANIM_LOOP },
	{ 32, 47, BLACT_ANIM_LOOP },
	{ 48, 63, BLACT_ANIM_LOOP },
	
	{ 64, 79, BLACT_ANIM_LOOP },	//�_�b�V��
	{ 80, 95, BLACT_ANIM_LOOP },
	{ 96, 111, BLACT_ANIM_LOOP },
	{ 112, 127, BLACT_ANIM_LOOP },
	
	{ 0, 0, BLACT_ANIM_CMD_MAX },
};



//-----------------------------------------------------------------------------
/**
 *				�l���t�@�C���f�[�^
 */
//-----------------------------------------------------------------------------
#define WFLBY_3DMDL_DATA_NUM		( 20 )	// ���j�I��16+��l��2+���˂�����+���ɂ�����
#define WFLBY_3DMDL_DATA_HERO_NUM	( 2 )
static const WFLBY_3DMDL_DATA	sc_WFLBY_3DMDL_DATA[ WFLBY_3DMDL_DATA_NUM ] = {
	// ��l��2
	{
		HERO,
		NARC_mmodel_hero_nsbtx,
		TRUE
	},
	{
		HEROINE,
		NARC_mmodel_heroine_nsbtx,
		TRUE
	},

	// ���j�I���L�����N�^16
	{
		BOY1,
		NARC_mmodel_boy1_nsbtx,
		FALSE
	},
	{
		BOY3,
		NARC_mmodel_boy3_nsbtx,
		FALSE
	},
	{
		MAN3,
		NARC_mmodel_man3_nsbtx,
		FALSE
	},
	{
		BADMAN,
		NARC_mmodel_badman_nsbtx,
		FALSE
	},
	{
		EXPLORE,
		NARC_mmodel_explore_nsbtx,
		FALSE
	},
	{
		FIGHTER,
		NARC_mmodel_fighter_nsbtx,
		FALSE
	},
	{
		GORGGEOUSM,
		NARC_mmodel_gorggeousm_nsbtx,
		FALSE
	},
	{
		MYSTERY,
		NARC_mmodel_mystery_nsbtx,
		FALSE
	},
	{
		GIRL1,
		NARC_mmodel_girl1_nsbtx,
		FALSE
	},
	{
		GIRL2,
		NARC_mmodel_girl2_nsbtx,
		FALSE
	},
	{
		WOMAN2,
		NARC_mmodel_woman2_nsbtx,
		FALSE
	},
	{
		WOMAN3,
		NARC_mmodel_woman3_nsbtx,
		FALSE
	},
	{
		IDOL,
		NARC_mmodel_idol_nsbtx,
		FALSE
	},
	{
		LADY,	
		NARC_mmodel_lady_nsbtx,
		FALSE
	},
	{
		COWGIRL,
		NARC_mmodel_cowgirl_nsbtx,
		FALSE
	},
	{
		GORGGEOUSW,
		NARC_mmodel_gorggeousw_nsbtx,
		FALSE
	},
	{
		WIFISW,
		NARC_mmodel_wifisw_nsbtx,
		FALSE
	},
	{
		WIFISM,
		NARC_mmodel_wifism_nsbtx,
		FALSE
	},
};

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�@�l���t�@�C���f�[�^
//=====================================
static const WFLBY_3DMDL_DATA* WFLBY_3DOBJ_GetMdlData( u32 objid );


//-------------------------------------
///		�A�j���f�[�^�̎擾
//=====================================
static u32 WFLBY_3DMDL_ANM_GetAnmOffs( BOOL walk, u32 way );
static fx32 WFLBY_3DMDL_ANM_GetAnmFrame( u16 frame_max, u16 now_frame, u32 anm_frame_max );


//-------------------------------------
///	�A�N�^�[�ŕ\���ł��Ȃ��RD�I�u�W�F���\�[�X
//=====================================
static void WFLBY_3DMDL_RES_Load( WFLBY_3DMDLRES* p_wk, ARCHANDLE* p_handle, u32 dataidx, u32 gheapID );
static void WFLBY_3DMDL_RES_Delete( WFLBY_3DMDLRES* p_wk );
static void WFLBY_3DMDL_RES_SetAlpha( WFLBY_3DMDLRES* p_wk, u32 alpha );
static u32 WFLBY_3DMDL_RES_GetAlpha( const WFLBY_3DMDLRES* cp_wk );
static void WFLBY_3DMDL_RES_InitD3DOBJ( WFLBY_3DMDLRES* p_wk, D3DOBJ* p_obj );


//-------------------------------------
///	�RD�I�u�W�F�N�g���[�N
//=====================================
static WFLBY_3DOBJWK* WFLBY_3DOBJSYS_GetCleanWk( WFLBY_3DOBJSYS* p_sys );
static BOOL WFLBY_3DOBJWK_CheckMove( const WFLBY_3DOBJWK* cp_wk );
static void WFLBY_3DOBJWK_CleanWk( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Draw( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Updata( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Updata_Normal( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Updata_Walk( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Updata_Turn( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Updata_Run( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_AnmUpdata( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Anm_Rota( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Anm_Jump( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_Anm_BataBata( WFLBY_3DOBJWK* p_wk );
static BOOL WFLBY_3DOBJWK_GetAnmSave( u32 st );
static void WFLBY_3DOBJWK_CheckCulling( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_ContBlactDrawFlag( WFLBY_3DOBJWK* p_wk );
static void WFLBY_3DOBJWK_CallBack_BlactDraw( BLACT_WORK_PTR p_act ,void* p_work );
static BOOL WFLBY_3DOBJWK_CheckCullingBlact( BLACT_WORK_PTR p_act );

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��		�쐬
 *
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	hero_sex	��l���̐���
 *	@param	heapID		�q�[�vID
 *	@param	gheapID		�O���t�B�b�N�q�[�vID
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DOBJSYS* WFLBY_3DOBJSYS_Init( u32 objnum, u32 hero_sex, u32 heapID, u32 gheapID )
{
	WFLBY_3DOBJSYS* p_sys;


	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_3DOBJSYS) );
	memset( p_sys, 0, sizeof(WFLBY_3DOBJSYS) );

	// �I�u�W�F�N�g�e�[�u���쐬
	{
		int i;
		
		p_sys->p_obj	= sys_AllocMemory( heapID, sizeof(WFLBY_3DOBJWK)*objnum );
		p_sys->objnum	= objnum;
	
		for( i=0; i<p_sys->objnum; i++ ){
			WFLBY_3DOBJWK_CleanWk( &p_sys->p_obj[i] );
		}
	}

	// ���\�[�X�I�u�W�F���쐬
	p_sys->p_mdlresman = RESM_Init( WFLBY_3DMDL_MDLDATA_NUM, heapID );
	p_sys->p_anmresman = RESM_Init( WFLBY_3DMDL_ANMDATA_NUM, heapID );
	p_sys->p_texresman = TEXRESM_Init( WFLBY_3DMDL_DATA_NUM, heapID );

	// �r���{�[�h�A�N�^�[�쐬
	{
		BLACT_SETDATA	setdata;

		BLACT_InitSys( 1, heapID );
		
		setdata.WorkNum = objnum;
		setdata.heap	= heapID;
		p_sys->p_blact = BLACT_InitSet( &setdata );
	}

	// �ǂݍ��ݏ���
	{
		ARCHANDLE* p_handle;
		ARCHANDLE* p_handle_other;
		void* p_file;
		int i;
		
		p_handle		= ArchiveDataHandleOpen( ARC_MMODEL, heapID );
		p_handle_other	= ArchiveDataHandleOpen( ARC_WIFILOBBY_OTHER_GRA, heapID );

		// ���f��
		{
			p_file = ArcUtil_HDL_Load( p_handle_other, WFLBY_3DMDL_MDLDATA_IDX, FALSE, gheapID, ALLOC_TOP );
			RESM_AddResNormal( p_sys->p_mdlresman, p_file, WFLBY_3DMDL_MDLDATA_IDX );
			WFLBY_LIGHT_SetUpMdl( p_file );

			// ���f���f�[�^���烉�C�g�f�[�^���擾����
			{
				NNSG3dResMdlSet* p_mdlset;
				NNSG3dResMdl* p_mdlres;
	
				p_mdlset = NNS_G3dGetMdlSet( p_file );
				p_mdlres = NNS_G3dGetMdlByIdx( p_mdlset, 0 );
				p_sys->mdl_light_msk = NNS_G3dMdlGetMdlLightEnableFlag( p_mdlres, 0 );
			}

		}

		// �A�j���f�[�^
		{
			for( i=0; i<WFLBY_3DMDL_ANMDATA_NUM; i++ ){
				p_file = ArcUtil_HDL_Load( p_handle, sc_WFLBY_ANMDATA[i], FALSE, gheapID, ALLOC_TOP );
				RESM_AddResNormal( p_sys->p_anmresman, p_file, sc_WFLBY_ANMDATA[i] );
			}
		}

		// �e�N�X�`���f�[�^
		{
			TEXRES_OBJ_PTR p_tex;
			int				skip;
			BOOL			tex_cut;

			// ��l���̓o�^���X�L�b�v
			if( hero_sex == PM_MALE ){
				skip = 1;	// ���̃��\�[�X���X�L�b�v
			}else{
				skip = 0;	// �j�̃��\�[�X���X�L�b�v
			}

			for( i=0; i<WFLBY_3DMDL_DATA_NUM; i++ ){
				if( skip == i ){
					continue;
				}

				if( sc_WFLBY_3DMDL_DATA[i].anm == FALSE ){
					tex_cut = TRUE;
				}else{
					tex_cut = FALSE;
				}

				p_file = ArcUtil_HDL_Load( p_handle, sc_WFLBY_3DMDL_DATA[i].tex, FALSE, gheapID, ALLOC_TOP );
				p_tex = TEXRESM_AddResNormal( p_sys->p_texresman, p_file, sc_WFLBY_3DMDL_DATA[i].tex, tex_cut, gheapID );
				// �]���ƃe�N�X�`���̈���J�b�g
				if( tex_cut == TRUE ){	
					TEXRESM_AllocVramKeyPTR( p_tex );
					TEXRESM_TexLoadPTR( p_tex );
					TEXRESM_CutTexPTR( p_tex );
				}
			}
		}


		// �e
		WFLBY_3DMDL_RES_Load( &p_sys->shadowres, p_handle_other, NARC_wifi_lobby_other_kage_nsbmd, gheapID );

		ArchiveDataHandleClose( p_handle );
		ArchiveDataHandleClose( p_handle_other );
	}
	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��	�j��
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJSYS_Exit( WFLBY_3DOBJSYS* p_sys )
{
	// �ǂݍ��񂾃��\�[�X��j������
	{
		WFLBY_3DMDL_RES_Delete( &p_sys->shadowres );
		TEXRESM_DeleteAllRes( p_sys->p_texresman );
		RESM_DeleteAllRes( p_sys->p_mdlresman );
		RESM_DeleteAllRes( p_sys->p_anmresman );
	}

	// �r���{�[�h�A�N�^�[�j��
	{
		BLACT_DestSet( p_sys->p_blact );
		BLACT_DestSys();
	}

	// ���\�[�X�}�l�[�W���̔j��
	TEXRESM_Delete( p_sys->p_texresman );
	RESM_Delete( p_sys->p_mdlresman );
	RESM_Delete( p_sys->p_anmresman );

	// ���f���f�[�^�j��
	sys_FreeMemoryEz( p_sys->p_obj );

	// �V�X�e�����̂�j��
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��	�X�V
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJSYS_Updata( WFLBY_3DOBJSYS* p_sys )
{
	int i;
	
	// �\���f�[�^�̍X�V
	for( i=0; i<p_sys->objnum; i++ ){
		// p_obj->updata��TRUE�Ȃ�WF2DMAP_OBJ�̃f�[�^�����f����܂��B
		// p_obj->updata��FALSE��p_obj->anmflag��TRUE�Ȃ����A�j���̃f�[�^�����f����܂��B
		WFLBY_3DOBJWK_Updata( &p_sys->p_obj[i] );		// �ʏ�A�b�v�f�[�g
		WFLBY_3DOBJWK_AnmUpdata( &p_sys->p_obj[i] );	// ����A�j���A�b�v�f�[�g
		WFLBY_3DOBJWK_CheckCulling( &p_sys->p_obj[i] );	// �J�����O�`�F�b�N
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��		�`��
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJSYS_Draw( WFLBY_3DOBJSYS* p_sys )
{
	int i;

	
	// �r���{�[�h�A�N�^�[�\��
	BLACT_DrawSys();


	// �e�̕`��
	// �e�̃J���[���O�Ȃ�\�����Ȃ�
	if( WFLBY_3DMDL_RES_GetAlpha( &p_sys->shadowres ) > 0 ){
		for( i=0; i<p_sys->objnum; i++ ){
			if( WFLBY_3DOBJWK_CheckMove( &p_sys->p_obj[i] ) == TRUE ){
				WFLBY_3DOBJWK_Draw( &p_sys->p_obj[i] );
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��	VBlank����
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJSYS_VBlank( WFLBY_3DOBJSYS* p_sys )
{

	// �r���{�[�h�A�N�^�[
	BLACT_VBlankFunc( p_sys->p_blact );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o����g���[�i�^�C�v���`�F�b�N
 *
 *	@param	trtype	�g���[�i�^�C�v
 *
 *	@retval	TRUE	�o����
 *	@retval	FALSE	�o���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJSYS_CheckTrType( u32 trtype )
{
	const WFLBY_3DMDL_DATA* cp_data;

	cp_data = WFLBY_3DOBJ_GetMdlData( trtype );
	if( cp_data == NULL ){
		return FALSE;
	}
	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�e�̃A���t�@�l��ݒ肷��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	alpha		�A���t�@�l
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJSYS_SetShadowAlpha( WFLBY_3DOBJSYS* p_sys, u32 alpha )
{
	WFLBY_3DMDL_RES_SetAlpha( &p_sys->shadowres, alpha );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�̃A���t�@�l���擾����
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *
 *	@return
 */
//-----------------------------------------------------------------------------
u32 WFLBY_3DOBJSYS_GetShadowAlpha( const WFLBY_3DOBJSYS* cp_sys )
{
	return WFLBY_3DMDL_RES_GetAlpha( &cp_sys->shadowres );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��	�`��I�u�W�F�쐬
 *
 *	@param	p_sys		�V�X�e��
 *	@param	cp_objwk	�I�u�W�F�N�g�f�[�^���[�N
 *
 *	@return	�`��I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
WFLBY_3DOBJWK* WFLBY_3DOBJWK_New( WFLBY_3DOBJSYS* p_sys, const WF2DMAP_OBJWK* cp_objwk )
{
	WFLBY_3DOBJWK* p_wk;

	// �󂢂Ă��郏�[�N���擾
	p_wk = WFLBY_3DOBJSYS_GetCleanWk( p_sys );

	// �Q�Ɛ�I�u�W�F�N�g�|�C���^�ۑ�
	p_wk->cp_objwk = cp_objwk;

	// �A�N�^�[����
	{
		const WFLBY_3DMDL_DATA* cp_data;
		BLACT_HEADER	header;
		BLACT_ADD		add;
		RES_OBJ_PTR		p_resobj;
		TEXRES_OBJ_PTR	p_texresobj;
		u32				objid;
		void*			p_imd;
		const NNSG3dResTex* cp_itx;
		TEXANM_DATATBL texanm;
		NNSGfdTexKey	texkey;
		NNSGfdTexKey	tex4x4key;
		NNSGfdPlttKey	plttkey;
		VecFx32			matrix = {0,0,0};
		VecFx32			scale = {FX32_ONE,FX32_ONE,FX32_ONE};

		// �I�u�W�F�N�g�\���f�[�^�擾
		objid	= WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_CHARA );
		cp_data = WFLBY_3DOBJ_GetMdlData( objid );

		// ���f���f�[�^�擾
		{
			p_resobj = RESM_GetResObj( p_sys->p_mdlresman, WFLBY_3DMDL_MDLDATA_IDX );
			p_imd	 = RESM_GetRes( p_resobj );
		}

		// �e�N�X�`���f�[�^�擾
		{
			p_texresobj = TEXRESM_GetResObj( p_sys->p_texresman, cp_data->tex );
			cp_itx		= TEXRESM_GetResPTR( p_texresobj );
		}

		// �e�N�X�`���A�j���f�[�^
		{
			u32 anm_id;
			const void* cp_buff;
			if( cp_data->anm == TRUE ){
				anm_id = sc_WFLBY_ANMDATA[WFLBY_3DMDL_ANMDATA_TRANS];
			}else{
				anm_id = sc_WFLBY_ANMDATA[WFLBY_3DMDL_ANMDATA_NORMAL];
			}
			p_resobj	= RESM_GetResObj( p_sys->p_anmresman, anm_id );
			cp_buff		= RESM_GetRes( p_resobj );

			TEXANM_UnPackLoadFile( cp_buff, &texanm );
		}
		
		if( cp_data->anm == TRUE ){	
			// �]���A�j���Őݒ�
			BLACT_MakeHeaderVramAnm( &header, p_imd, cp_itx, WFLBY_3DOBJ_ANM_TR_FRAME_DATA, &texanm );
		}else{
			// �ʏ�A�j���Őݒ�
			texkey		= TEXRESM_GetTexKeyPTR( p_texresobj );
			tex4x4key	= TEXRESM_GetTex4x4KeyPTR( p_texresobj );
			plttkey		= TEXRESM_GetPlttKeyPTR( p_texresobj );
			BLACT_MakeHeaderNormalAnm( &header, p_imd, cp_itx, WFLBY_3DOBJ_ANM_TR_FRAME_DATA, &texanm, texkey, tex4x4key, plttkey  );
		}

		// ���W��ݒ�
		{
			WF2DMAP_POS pos;
			pos = WF2DMAP_OBJWkMatrixGet( cp_objwk );
			WFLBY_3DMATRIX_GetPosVec( &pos, &matrix );
			matrix.z += WFLBY_3DOBJ_OFS_Z;
		}

		// ���[�N���쐬
		add.blact_s = p_sys->p_blact;
		add.pHeader	= &header;
		add.matrix	= matrix;
		add.scale	= scale;
		p_wk->p_act = BLACT_Add( &add );

		// �R�[���o�b�N�̐ݒ�
		BLACT_DrawBeforeProcSet( p_wk->p_act, WFLBY_3DOBJWK_CallBack_BlactDraw, p_wk );


		// �e���쐬
		WFLBY_3DMDL_RES_InitD3DOBJ( &p_sys->shadowres, &p_wk->shadow );

		// ���W�����킹��
		D3DOBJ_SetMatrix( &p_wk->shadow, 
				matrix.x + WFLBY_3DOBJ_SHADOW_OFS_X,
				WFLBY_3DOBJ_SHADOW_MAT_Y,
				matrix.z + WFLBY_3DOBJ_SHADOW_OFS_Z );
	}

	


	// �X�V�J�n
	p_wk->updata = TRUE;

	// �\���ƃJ�����O�t���O
	p_wk->objdraw = TRUE;
	p_wk->culling = FALSE;

	// ���C�g�}�X�N�ݒ�
	p_wk->set_light_msk = p_sys->mdl_light_msk;

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F�N�g�`��V�X�e��	�`��I�u�W�F�j��
 *
 *	@param	p_wk		�`��I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_Del( WFLBY_3DOBJWK* p_wk )
{
	BLACT_Delete( p_wk->p_act );
	memset( p_wk, 0, sizeof(WFLBY_3DOBJWK) );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̍X�V�t���O��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	updata		�A�b�v�f�[�g�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetUpdata( WFLBY_3DOBJWK* p_wk, BOOL updata )
{
	p_wk->updata = updata;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̍X�V�t���O���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE		�X�V��
 *	@retval	FALSE		�X�V���Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJWK_GetUpdata( const WFLBY_3DOBJWK* cp_wk )
{
	return cp_wk->updata;
}


//----------------------------------------------------------------------------
/**
 *	@brief	light_msk��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	light_msk	���C�g�}�X�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetLight( WFLBY_3DOBJWK* p_wk, u32 light_msk )
{
	p_wk->set_light_msk = light_msk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����O�t���O���擾��
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�J�����O�`�F�b�N�Ɉ����������Ĕ�\����
 *	@retval	FALSE	�J�����O�Ɉ��������炸�ɕ\����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJWK_GetCullingFlag( const WFLBY_3DOBJWK* cp_wk )
{
	return cp_wk->culling;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̍��W��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_vec		���W
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetMatrix( WFLBY_3DOBJWK* p_wk, const WF2DMAP_POS* cp_pos )
{
	VecFx32 vec;
	WFLBY_3DMATRIX_GetPosVec( cp_pos, &vec );
	vec.z += WFLBY_3DOBJ_OFS_Z;


	WFLBY_3DOBJWK_Set3DMatrix( p_wk, &vec );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̍��W���擾
 *
 *	@param	cp_wk		���[�N
 *	@param	p_vec		���W
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_GetMatrix( const WFLBY_3DOBJWK* cp_wk, WF2DMAP_POS* p_pos )
{
	const VecFx32* cp_vec;
	cp_vec = BLACT_MatrixGet( cp_wk->p_act );
	WFLBY_3DMATRIX_GetVecPos( cp_vec, p_pos );
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D���W��ݒ肷��
 *
 *	@param	p_wk	�l�����[�N
 *	@param	cp_vec	�x�N�g��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_Set3DMatrix( WFLBY_3DOBJWK* p_wk, const VecFx32* cp_vec )
{
	BLACT_MatrixSet( p_wk->p_act, cp_vec );

	// �e�ɔ��f�������A�e�͂�Y�ʒu�͏�ɒ�ʒu
	D3DOBJ_SetMatrix( &p_wk->shadow, cp_vec->x + WFLBY_3DOBJ_SHADOW_OFS_X,
			WFLBY_3DOBJ_SHADOW_MAT_Y, cp_vec->z + WFLBY_3DOBJ_SHADOW_OFS_Z );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���W���擾����
 *
 *	@param	cp_wk	�l�����[�N
 *	@param	p_vec	�x�N�g��
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_Get3DMatrix( const WFLBY_3DOBJWK* cp_wk, VecFx32* p_vec )
{
	const VecFx32* cp_vec;
	
	cp_vec = BLACT_MatrixGet( cp_wk->p_act );
	*p_vec = *cp_vec;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����Ă��������ύX
 *
 *	@param	p_wk	�l�����[�N
 *	@param	way		����
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetWay( WFLBY_3DOBJWK* p_wk, WF2DMAP_WAY way )
{
	u32 anm;
	anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );
	BLACT_AnmOffsChg( p_wk->p_act, anm );
	BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���o�^	�i�X�V�t���O��OFF�ɂ�����̓���j
 *
 *	@param	p_wk	���[�N
 *	@param	anm		�A�j���i���o�[
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_StartAnm( WFLBY_3DOBJWK* p_wk, WFLBY_3DOBJ_ANMTYPE anm )
{
	p_wk->anmflag	= TRUE;
	p_wk->anm		= anm;
	p_wk->anmframe	= 0;
	p_wk->anmspeed	= 1;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_EndAnm( WFLBY_3DOBJWK* p_wk )
{
	p_wk->anmflag = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���X�s�[�h��ݒ肷��
 *
 *	@param	p_wk		���[�N	
 *	@param	speed		�X�s�[�h
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetAnmSpeed( WFLBY_3DOBJWK* p_wk, u8 speed )
{
	p_wk->anmspeed	= speed;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̕`��t���O��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�`��t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_3DOBJWK_SetDrawFlag( WFLBY_3DOBJWK* p_wk, BOOL flag )
{
	p_wk->objdraw = flag;
	WFLBY_3DOBJWK_ContBlactDrawFlag( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��I�u�W�F�N�g�̕`��t���O���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�`�撆
 *	@retval	FALSE	��\����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_3DOBJWK_GetDrawFlag( const WFLBY_3DOBJWK* cp_wk )
{
	return cp_wk->objdraw;
}




//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	objid�ɑΉ������l���f�[�^���擾����
 *
 *	@param	objid		�I�u�W�F�N�gID
 *
 *	@return	�l���f�[�^
 */
//-----------------------------------------------------------------------------
static const WFLBY_3DMDL_DATA* WFLBY_3DOBJ_GetMdlData( u32 objid )
{
	int i;

	for( i=0; i<WFLBY_3DMDL_DATA_NUM; i++ ){
		if( sc_WFLBY_3DMDL_DATA[i].objid == objid ){
			return &sc_WFLBY_3DMDL_DATA[i];
		}
	}
	GF_ASSERT(0);
	return NULL;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����I�t�Z�b�g���擾����
 *
 *	@param	walk		�����A�j����	TRUE�F����	FALSE�F����
 *	@param	way			����
 *
 *	@return	�A�j���[�V�����I�t�Z�b�g
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_3DMDL_ANM_GetAnmOffs( BOOL walk, u32 way )
{
	if( walk ){
		return WFLBY_3DOBJ_ANM_TYPE_UP + way;
	}
	return WFLBY_3DOBJ_ANM_TYPE_RUN_UP + way;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V�����t���[�����擾
 *
 *	@param	frame_max		�ő�t���[��
 *	@param	now_frame		���̃t���[��
 */
//-----------------------------------------------------------------------------
static fx32 WFLBY_3DMDL_ANM_GetAnmFrame( u16 frame_max, u16 now_frame, u32 anm_frame_max )
{
	fx32 frame;

	// OBJ�V�X�e���ŃA�N�V�����R�}���h��ݒ肵�Ă���A�b�v�f�[�g����̂ŁA
	// frame_max�ɂȂ邱�Ƃ͂Ȃ�
	now_frame ++;
	
	frame = ( now_frame * anm_frame_max ) / frame_max;
	frame *= FX32_ONE;
	return frame;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f�����\�[�X��ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	p_handle	�n���h��
 *	@param	dataidx		�f�[�^�C���f�b�N�X
 *	@param	gheapID		�O���t�B�b�N�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMDL_RES_Load( WFLBY_3DMDLRES* p_wk, ARCHANDLE* p_handle, u32 dataidx, u32 gheapID )
{
	void* p_mdl;
	
	//  ���f���f�[�^�ǂݍ��݁��e�N�X�`���]�����e�N�X�`�����f�[�^�j��
	WFLBY_3DMAPOBJ_TEX_LoatCutTex( &p_mdl, p_handle, dataidx, gheapID );

	// ���f���̃��\�[�X��D3DOBJ_MDL�̌`�Ɍ`��
	p_wk->mdlres.pResMdl	= p_mdl;
	p_wk->mdlres.pModelSet	= NNS_G3dGetMdlSet( p_wk->mdlres.pResMdl );
	p_wk->mdlres.pModel		= NNS_G3dGetMdlByIdx( p_wk->mdlres.pModelSet, 0 );
	p_wk->mdlres.pMdlTex	= NNS_G3dGetTex( p_wk->mdlres.pResMdl );

	// ���f���Ƀo�C���h����
	BindTexture( p_wk->mdlres.pResMdl, p_wk->mdlres.pMdlTex );

	// �ۂ育��ID ��ݒ�
	NNS_G3dMdlSetMdlPolygonIDAll( p_wk->mdlres.pModel, WFLBY_3DOBJ_SHADOW_OBJID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f�����\�[�X��j������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMDL_RES_Delete( WFLBY_3DMDLRES* p_wk )
{
	D3DOBJ_MdlDelete( &p_wk->mdlres );
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D���f�����\�[�X�ɃA���t�@�l��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	alpha		�A���t�@�l
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMDL_RES_SetAlpha( WFLBY_3DMDLRES* p_wk, u32 alpha )
{
	p_wk->alpha = alpha;
	NNS_G3dMdlSetMdlAlphaAll( p_wk->mdlres.pModel, p_wk->alpha );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f�����\�[�X����A���t�@�l���擾
 *
 *	@param	cp_wk		���[�N
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_3DMDL_RES_GetAlpha( const WFLBY_3DMDLRES* cp_wk )
{
	return cp_wk->alpha;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD���f�����\�[�X���g�����\���I�u�W�F���[�N��������
 *
 *	@param	p_wk		���[�N
 *	@param	p_obj		�\���I�u�W�F�N�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DMDL_RES_InitD3DOBJ( WFLBY_3DMDLRES* p_wk, D3DOBJ* p_obj )
{
	D3DOBJ_Init( p_obj, &p_wk->mdlres );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�󂢂Ă��郏�[�N���擾����
 *
 *	@param	p_sys	�V�X�e�����[�N
 *
 *	@return	�󂢂Ă��郏�[�N
 */
//-----------------------------------------------------------------------------
static WFLBY_3DOBJWK* WFLBY_3DOBJSYS_GetCleanWk( WFLBY_3DOBJSYS* p_sys )
{
	int i;


	for( i=0; i<p_sys->objnum; i++ ){
		if( WFLBY_3DOBJWK_CheckMove( &p_sys->p_obj[i] ) == FALSE ) {
			return &p_sys->p_obj[i];
		}
	}

	GF_ASSERT( 0 );
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�����쒆���`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���쒆
 *	@retval	FALSE	���삵�ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJWK_CheckMove( const WFLBY_3DOBJWK* cp_wk )
{
	if( cp_wk->cp_objwk == NULL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N����ɂ���
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_CleanWk( WFLBY_3DOBJWK* p_wk )
{
	memset( p_wk, 0, sizeof(WFLBY_3DOBJWK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�RD�I�u�W�F���[�N	�l���ȊO�̕`�揈��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Draw( WFLBY_3DOBJWK* p_wk )
{
	// �e�̕`��
	D3DOBJ_Draw( &p_wk->shadow );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N	�X�V
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Updata( WFLBY_3DOBJWK* p_wk )
{
	u32 st;
	u32 frame;
	WF2DMAP_POS pos;
	static void (*pUpdata[WF2DMAP_OBJST_NUM])( WFLBY_3DOBJWK* p_wk ) = {
		WFLBY_3DOBJWK_Updata_Normal,
		WFLBY_3DOBJWK_Updata_Turn,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Run,
		WFLBY_3DOBJWK_Updata_Normal,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Walk,
		WFLBY_3DOBJWK_Updata_Walk,
	};
	
	// �X�V�t���O�������Ă��Ȃ���΍X�V���Ȃ�
	if( p_wk->updata == FALSE ){
		return ;
	}

	// �I�u�W�F�N�g�̃t���[�����ɑΉ������A�j���𗬂�
	st		= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_ST );
	frame	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_FRAME );

	// �ȑO�̃t���[������荡�̃t���[�������������Ƃ�
	// ��Ԃ��ύX�����Ƃ��ɈȑO�̃A�j���ƃt���[������ۑ�����
	if( (p_wk->lastfrm > frame) || (p_wk->lastst != st) ){
		if( WFLBY_3DOBJWK_GetAnmSave( p_wk->lastst ) == TRUE ){
			// �A�j���t���[����ۑ�����
			p_wk->lastanm	= BLACT_AnmOffsGet( p_wk->p_act );
			p_wk->lastframe = BLACT_AnmFrameGetOffs( p_wk->p_act );
		}
		p_wk->lastst = st;
	}
	p_wk->lastfrm	= frame;
	
	// �X�V
	pUpdata[ st ]( p_wk );

	// ���W�𔽉f
	pos = WF2DMAP_OBJWkFrameMatrixGet( p_wk->cp_objwk );
	WFLBY_3DOBJWK_SetMatrix( p_wk, &pos );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�ҋ@�A�j��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Updata_Normal( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;

	// WAY�̕����������Ă��邾��
	way	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

	// �����̃A�j���I�t�Z�b�g�擾
	anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

	// �A�N�^�[�ɐݒ�
	BLACT_AnmOffsChg( p_wk->p_act, anm );
	BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����A�j��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Updata_Walk( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;
	fx32 frame;
	u16 frame_max;
	u16 now_frame;

	// WAY�̕����ɂ��邭
	way	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

	// �����̃A�j���I�t�Z�b�g�擾
	anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

	// �t���[�����擾
	frame_max = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_ENDFRAME );
	now_frame = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_FRAME );
	frame = WFLBY_3DMDL_ANM_GetAnmFrame( frame_max, now_frame, WFLBY_3DOBJ_ONEANM_HFRAME );

	// �A�N�^�[�ɐݒ�
	BLACT_AnmOffsChg( p_wk->p_act, anm );

	// �O�̃A�j���ƈꏏ�Ȃ�O�̃t���[�����瑱����
	if( p_wk->lastanm == anm ){
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
		BLACT_AnmFrameChg( p_wk->p_act, frame+p_wk->lastframe );
	}else{
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
		BLACT_AnmFrameChg( p_wk->p_act, frame );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�U������A�j��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Updata_Turn( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;
	u16 now_frame;

	// �t���[�����擾
	now_frame = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_FRAME );

	if( now_frame < WFLBY_3DOBJ_TRUN_FIRST_FRAME ){
		BLACT_AnmFrameSetOffs( p_wk->p_act, WFLBY_3DOBJ_TRUN_FIRST_FRAME*FX32_ONE );	// 1���O��
	}else{
		// WAY�̕����ɂ��邭
		way	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

		// �����̃A�j���I�t�Z�b�g�擾
		anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

		// �A�N�^�[�ɐݒ�
		BLACT_AnmOffsChg( p_wk->p_act, anm );
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	����A�j��
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Updata_Run( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;
	fx32 frame;
	u16 frame_max;
	u16 now_frame;

	// WAY�̕����ɂ��邭
	way	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

	// �����̃A�j���I�t�Z�b�g�擾
	anm = WFLBY_3DMDL_ANM_GetAnmOffs( FALSE, way );

	// �t���[�����擾
	frame_max = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_ENDFRAME );
	now_frame = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_FRAME );
	frame = WFLBY_3DMDL_ANM_GetAnmFrame( frame_max, now_frame, WFLBY_3DOBJ_ONEANM_RUNFRAME );

	// �A�N�^�[�ɐݒ�
	BLACT_AnmOffsChg( p_wk->p_act, anm );

	// �O�̃A�j���ƈꏏ�Ȃ�O�̃t���[�����瑱����
	if( p_wk->lastanm == anm ){
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
		BLACT_AnmFrameChg( p_wk->p_act, frame+p_wk->lastframe );
	}else{
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
		BLACT_AnmFrameChg( p_wk->p_act, frame );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	����A�j���A�b�v�f�[�g����
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_AnmUpdata( WFLBY_3DOBJWK* p_wk )
{
	static void (*pUpdata[WFLBY_3DOBJ_ANM_NUM])( WFLBY_3DOBJWK* p_wk ) = {
		WFLBY_3DOBJWK_Anm_Rota,
		WFLBY_3DOBJWK_Anm_Jump,
		WFLBY_3DOBJWK_Anm_BataBata,
	};

	// �X�V��~�{�A�j���Đ����Ȃ�ݒ�
	if( (p_wk->updata == FALSE) && (p_wk->anmflag == TRUE) ){
		pUpdata[ p_wk->anm ]( p_wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	����A�j��		��]
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Anm_Rota( WFLBY_3DOBJWK* p_wk )
{
	u32 anmidx;
	u32 blact_anm;
	
	if( (p_wk->anmframe % WFLBY_3DOBJ_ANM_ROTA_1SYNC) == 0 ){
		// �A�j���ύX
		anmidx = p_wk->anmframe / WFLBY_3DOBJ_ANM_ROTA_1SYNC;
		blact_anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, sc_WFLBY_3DOBJ_ANM_WAY[ anmidx ] );
		BLACT_AnmOffsChg( p_wk->p_act, blact_anm );
		BLACT_AnmFrameSetOffs( p_wk->p_act, 0 );
	}
	if( (p_wk->anmframe + p_wk->anmspeed) < (WFLBY_3DOBJ_ANM_ROTA_1SYNC*WF2DMAP_WAY_NUM) ){
		p_wk->anmframe += p_wk->anmspeed;
	}else{
		p_wk->anmframe = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�W�����v�A�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Anm_Jump( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;
	
	if( p_wk->anmframe == 0 ){
		// �����Ă�������ɕ����A�j���̂Q���ܖڂ�ݒ�
		way = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );


		// WAY�̕����������Ă��邾��
		way	= WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

		// �����̃A�j���I�t�Z�b�g�擾
		anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

		// �A�N�^�[�ɐݒ�
		BLACT_AnmOffsChg( p_wk->p_act, anm );
		BLACT_AnmFrameSetOffs( p_wk->p_act, WFLBY_3DOBJ_TRUN_FIRST_FRAME*FX32_ONE );

		p_wk->anmframe++;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����o�^�o�^�A�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_Anm_BataBata( WFLBY_3DOBJWK* p_wk )
{
	u32 way;
	u32 anm;
	
	if( p_wk->anmframe == 0 ){
		// �����Ă�������ɕ����A�j����1���ܖڂ�ݒ�
		way = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

		// �����̃A�j���I�t�Z�b�g�擾
		anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

		// �A�N�^�[�ɐݒ�
		BLACT_AnmOffsChg( p_wk->p_act, anm );
		BLACT_AnmFrameSetOffs( p_wk->p_act, WFLBY_3DOBJ_TRUN_FIRST_FRAME*FX32_ONE );

	}else if( p_wk->anmframe == WFLBY_3DOBJ_ANM_BATA_FRAME ){

		// �����Ă�������ɕ����A�j����3���ܖڂ�ݒ�
		way = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );

		// �����̃A�j���I�t�Z�b�g�擾
		anm = WFLBY_3DMDL_ANM_GetAnmOffs( TRUE, way );

		// �A�N�^�[�ɐݒ�
		BLACT_AnmOffsChg( p_wk->p_act, anm );
		BLACT_AnmFrameSetOffs( p_wk->p_act, (WFLBY_3DOBJ_TRUN_FIRST_FRAME*3)*FX32_ONE );
	}

	p_wk->anmframe = (p_wk->anmframe + 1) % (WFLBY_3DOBJ_ANM_BATA_FRAME*2);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j����ۑ������ԂȂ̂��`�F�b�N����
 *
 *	@param	st		���
 *
 *	@retval	TRUE	�Z�[�u����
 *	@retval	FALSE	�Z�[�u���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJWK_GetAnmSave( u32 st )
{
	switch( st ){
	case WF2DMAP_OBJST_WALK:
	case WF2DMAP_OBJST_RUN:
	case WF2DMAP_OBJST_WALLWALK:
	case WF2DMAP_OBJST_SLOWWALK:
	case WF2DMAP_OBJST_STAYWALK8:
	case WF2DMAP_OBJST_STAYWALK16:
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����O�`�F�b�N
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_CheckCulling( WFLBY_3DOBJWK* p_wk )
{
	BOOL result;


	if( WFLBY_3DOBJWK_CheckMove( p_wk ) ){
		
		result = WFLBY_3DOBJWK_CheckCullingBlact( p_wk->p_act );
		
		if( result == FALSE ){
			p_wk->culling = TRUE;
		}else{
			p_wk->culling = FALSE;
		}

		// �\���ݒ�
		WFLBY_3DOBJWK_ContBlactDrawFlag( p_wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��Ǘ�
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_ContBlactDrawFlag( WFLBY_3DOBJWK* p_wk )
{
	if( (p_wk->culling == FALSE) && (p_wk->objdraw == TRUE) ){
		// �\��ON
		BLACT_ObjDrawFlagSet( p_wk->p_act, TRUE );
		D3DOBJ_SetDraw( &p_wk->shadow, TRUE );
	}else{
		// �\��OFF
		BLACT_ObjDrawFlagSet( p_wk->p_act, FALSE );
		D3DOBJ_SetDraw( &p_wk->shadow, FALSE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���{�[�h�A�N�^�[	�`��O�R�[���o�b�N
 *
 *	@param	p_act		�A�N�^�[
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_3DOBJWK_CallBack_BlactDraw( BLACT_WORK_PTR p_act ,void* p_work )
{
	WFLBY_3DOBJWK* p_wk = p_work;
	NNSG3dResMdl*  p_mdl;

	p_mdl = BLACT_ResMdlGet( p_act );
	NNS_G3dMdlSetMdlLightEnableFlagAll( p_mdl, p_wk->set_light_msk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���{�[�h�A�N�^�[��p	�J�����O����
 *			�풓�𑝂₵�����Ȃ��̂�	�������Ȃ����ōs���܂�
 *
 *	@param	p_act	�A�N�^�[
 *		
 *	@retval	TRUE	�`�悷��
 *	@retval	FALSE	�`�悵�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_3DOBJWK_CheckCullingBlact( BLACT_WORK_PTR p_act )
{
	BOOL ret;
	VecFx32 matrix;
	NNSG3dResMdlInfo*	p_mdlinfo;	
	NNSG3dResMdl*		p_mdl;
	BOUNDING_BOX		box;
	MtxFx33				mtx;
	
	// ���\�[�X�擾
	p_mdl = BLACT_MdlResGet( p_act );
	p_mdlinfo = NNS_G3dGetMdlInfo( p_mdl );			// ���f�����\�[�X�C���t�H�f�[�^�擾

	// ���W
	matrix = *BLACT_MatrixGet(p_act);


	// �r���{�[�h�͔Ȃ̂Ń��f���Ƃ��Ă͉��s�����Ȃ����A
	// �J�����̕����������悤�ɉ�]����̂ŁA
	// ���̊p�x�ɂ���ẮA���s�����o�Ă���B
	// �r���{�[�h���ǂ�ȕ����ɉ�]���Ă����v�Ȃ悤��
	// ���s���ɍ����̑傫��������
	box.w		= p_mdlinfo->boxW;
	box.h		= p_mdlinfo->boxH;
	box.d		= p_mdlinfo->boxH;
	box.ScaleW	= p_mdlinfo->boxPosScale >> FX32_SHIFT;
	box.ScaleH	= p_mdlinfo->boxPosScale >> FX32_SHIFT;
	box.ScaleD	= p_mdlinfo->boxPosScale >> FX32_SHIFT;

	matrix.x	+= FX_Mul( p_mdlinfo->boxX, p_mdlinfo->boxPosScale );
	matrix.y	+= FX_Mul( p_mdlinfo->boxY, p_mdlinfo->boxPosScale );
	matrix.z	+= FX_Mul( p_mdlinfo->boxZ, p_mdlinfo->boxPosScale );
	matrix.z	-= FX_Mul( p_mdlinfo->boxH, p_mdlinfo->boxPosScale );	// ���s�������������₷�̂ŁAZ�̒l��H���ړ�������

	MTX_Identity33( &mtx );
	NNS_G3dGlbSetBaseRot( &mtx );
	
	// �X�P�[���ݒ�
	NNS_G3dGlbSetBaseScale( BLACT_ScaleGet(p_act) );

	ret = BB_CullingCheck3DModelNonResQuick( &matrix, &box );


	if( ret == 0 ){
		return FALSE;
	}
	return TRUE;
}

