//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_objdraw.c
 *	@brief		�I�u�W�F�N�g�f�[�^�\�����W���[��
 *	@author		tomoya takahashi
 *	@data		2007.03.16
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#define __WF2DMAP_OBJDRAW_H_GLOBAL
#include "application/wifi_2dmap/wf2dmap_objdraw.h"

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
// �\���D�揇�ʌv�Z�}�N��
#define WF2DMAP_DRAWPRI_STATR	(20000)	// �\���D�揇�ʊJ�n�ʒu
#define WF2DMAP_DRAWPRI_MAX		(20000)	//���Ă�10000�h�b�g�T�C�Y�̕����͂Ȃ����낤
#define WF2DMAP_DRAWPRI_HERO_GET(y)	( WF2DMAP_DRAWPRI_MAX - (y*2) + WF2DMAP_DRAWPRI_STATR )	// ��l���̕\���D�揇��
#define WF2DMAP_DRAWPRI_GET(y)	( WF2DMAP_DRAWPRI_HERO_GET(y)+1 )	// NPC�̕\���D�揇��
#define WF2DMAP_DRAWPRI_SHADOW	( 40001 )

// �o�^�f�t�H���g�ݒ�
#define WF2DMAP_BGPRI_DEF	(2)	// ��{�a�f�D�揇��

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�\�����[�N
//=====================================
typedef struct _WF2DMAP_OBJDRAWWK{
	const WF2DMAP_OBJWK* cp_objwk;	// �����N���Ă���I�u�W�F�N�g���[�N
	WF_2DCWK* p_drawwk;	// �\�����[�N
	u16 status;	// ���̏��
	u16 way;	// ����
	u16 playid;	// �v���C���[�h�c
	u16 lastframe;	// 1�O�̏I���t���[��
	u16 lastanm;	// 1�O�̃A�j��
	u8 hero;	// ��l���t���O
	u8 updata;	// �A�b�v�f�[�g�t���O
}WF2DMAP_OBJDRAWWK;


//-------------------------------------
///	�\���V�X�e�����[�N
//=====================================
typedef struct _WF2DMAP_OBJDRAWSYS{
	WF_2DCSYS* p_drawsys;
	WF2DMAP_OBJDRAWWK* p_wk;
	u16 objnum;
	u8	bg_pri;
	u8	draw_type;
}WF2DMAP_OBJDRAWSYS;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJDrawWkCleanCheck( const WF2DMAP_OBJDRAWWK* cp_wk );
static WF2DMAP_OBJDRAWWK* WF2DMAP_OBJDrawSysCleanWkGet( WF2DMAP_OBJDRAWSYS* p_sys );
static WF_2DC_ANMTYPE WF2DMAP_OBJDrawWF2DMAP_OBJSTtoWF_2DC_ANMTYPE( const WF_2DCWK* p_drawwk, WF2DMAP_OBJST status );
static void WF2DMAP_OBJDrawWkFramePosGet( const WF2DMAP_OBJDRAWWK* cp_wk, s16* p_x, s16* p_y );

static void WF2DMAP_OBJDrawWkAnmSet( WF2DMAP_OBJDRAWWK* p_wk, WF_2DC_ANMTYPE anmtype, WF_COMMON_WAY anmway );

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�`��V�X�e��������
 *	
 *	@param	p_clset			�Z���A�N�^�[�|�C���^
 *	@param	p_pfd			�p���b�g�t�F�[�h�|�C���^
 *	@param	objnum			�I�u�W�F�N�g��
 *	@param	draw_type		�S�l���̕\����
 *	@param	heapID			�q�[�v�h�c
 *
 *	@return	�V�X�e�����[�N
 *
 *
 *	draw_type
	main�ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAIN
	sub�ɓo�^ �FNNS_G2D_VRAM_TYPE_2DSUB
	�����ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAX
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJDRAWSYS* WF2DMAP_OBJDrawSysInit( CLACT_SET_PTR p_clset, PALETTE_FADE_PTR p_pfd, u32 objnum, u32 draw_type, u32 heapID )
{
	WF2DMAP_OBJDRAWSYS* p_sys;

	// �V�X�e�����[�N�쐬
	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJDRAWSYS) );

	// �\���V�X�e���쐬
	p_sys->p_drawsys = WF_2DC_SysInit( p_clset, p_pfd, objnum, heapID );

	// �I�u�W�F�N�g���[�N�쐬
	p_sys->objnum = objnum;
	p_sys->p_wk = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJDRAWWK)*p_sys->objnum );
	memset( p_sys->p_wk, 0, sizeof(WF2DMAP_OBJDRAWWK)*p_sys->objnum );

	// ��{�ݒ�
	p_sys->bg_pri		= WF2DMAP_BGPRI_DEF;
	p_sys->draw_type	= draw_type;

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�`��V�X�e��������		�A����
 *	
 *	@param	p_clset			�Z���A�N�^�[�|�C���^
 *	@param	p_pfd			�p���b�g�t�F�[�h�|�C���^
 *	@param	objnum			�I�u�W�F�N�g��
 *	@param	hero_charid		��l���L�����N�^ID
 *	@param	hero_movetype	��l���̓���
 *	@param	draw_type		�S�l���̕\����
 *	@param	heapID			�q�[�v�h�c
 *
 *	@return	�V�X�e�����[�N
 *
 *
 *	draw_type
	main�ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAIN
	sub�ɓo�^ �FNNS_G2D_VRAM_TYPE_2DSUB
	�����ɓo�^�FNNS_G2D_VRAM_TYPE_2DMAX
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJDRAWSYS* WF2DMAP_OBJDrawSysInit_Shadow( CLACT_SET_PTR p_clset, PALETTE_FADE_PTR p_pfd, u32 objnum, u32 hero_charid, WF_2DC_MOVETYPE hero_movetype,  u32 draw_type,u32 heapID )
{
	WF2DMAP_OBJDRAWSYS* p_sys;

	// �V�X�e�����[�N�쐬
	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJDRAWSYS) );

	// �\���V�X�e���쐬
	p_sys->p_drawsys = WF_2DC_SysInit( p_clset, p_pfd, objnum, heapID );

	// �I�u�W�F�N�g���[�N�쐬
	p_sys->objnum = objnum;
	p_sys->p_wk = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJDRAWWK)*p_sys->objnum );
	memset( p_sys->p_wk, 0, sizeof(WF2DMAP_OBJDRAWWK)*p_sys->objnum );

	// ��{�ݒ�
	p_sys->bg_pri		= WF2DMAP_BGPRI_DEF;
	p_sys->draw_type	= draw_type;

	// ��l���̃��\�[�X��ǂݍ���
	WF_2DC_ResSet( p_sys->p_drawsys, hero_charid, p_sys->draw_type, hero_movetype, heapID );

	// �A�̃��\�[�X�ǂݍ���
	WF_2DC_ShadowResSet( p_sys->p_drawsys, p_sys->draw_type, WF2DMAP_DRAWPRI_SHADOW, heapID );

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�`��V�X�e���j��
 *
 *	@param	p_sys 
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysExit( WF2DMAP_OBJDRAWSYS* p_sys )
{
	int i;
	
	// �I�u�W�F�N�g���[�N�j��
	for( i=0; i<p_sys->objnum; i++ ){
		if( WF2DMAP_OBJDrawWkCleanCheck( &p_sys->p_wk[i] ) == FALSE ){
			WF2DMAP_OBJDrawWkDel( &p_sys->p_wk[i] );
		}
	}


	// �A�̃��\�[�X�j��
	WF_2DC_ShadowResDel( p_sys->p_drawsys );

	// ���\�[�X�͂�
	WF2DMAP_OBJDrawSysAllResDel( p_sys );

	// �\���V�X�e���͂�
	WF_2DC_SysExit( p_sys->p_drawsys );
	
	// ���[�N�o�b�t�@�j��
	sys_FreeMemoryEz( p_sys->p_wk );
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�D�揇�ʂ̐ݒ�
 *
 *	@param	p_sys	�I�u�W�F�N�g�\���V�X�e�����[�N
 *	@param	bg_pri	�a�f�D�揇��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysDefBgPriSet( WF2DMAP_OBJDRAWSYS* p_sys, u32 bg_pri )
{
	p_sys->bg_pri = bg_pri;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�a�f�D�揇�ʂ̎擾
 *
 *	@param	cp_sys	�I�u�W�F�N�g�\���V�X�e�����[�N
 *
 *	@return	�a�f�D�揇��
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_OBJDrawSysDefBgPriGet( const WF2DMAP_OBJDRAWSYS* cp_sys )
{
	return cp_sys->bg_pri;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^���\�[�X�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N	
 *	@param	charaid		�L�����N�^�h�c
 *	@param	movetype	����^�C�v
 *	@param	heapID		�q�[�v�h�c

	movetype
	WF_2DC_MOVERUN,		// �����{�U������{����A�j���i��l���̂݁j
	WF_2DC_MOVENORMAL,	// �����{�U������̂݃A�j��
	WF_2DC_MOVETURN,	// �U������̂݃A�j��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysResSet( WF2DMAP_OBJDRAWSYS* p_sys, u32 charaid, WF_2DC_MOVETYPE movetype, u32 heapID )
{
	// ���d�ǂݍ��݉��
	if( WF_2DC_ResCheck( p_sys->p_drawsys, charaid ) == FALSE ){
		WF_2DC_ResSet( p_sys->p_drawsys, charaid, p_sys->draw_type, movetype, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�[���\�[�X�̔j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	charid		�L�����N�^�[�^�C�v
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysResDel( WF2DMAP_OBJDRAWSYS* p_sys, u32 charaid )
{
	WF_2DC_ResDel( p_sys->p_drawsys, charaid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�[���\�[�X���ǂݍ��܂�Ă��邩�`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	charid		�L�����N�^�h�c
 *
 *	@retval	�ǂݍ���ł���
 *	@retval	�ǂݍ���łȂ�
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_OBJDrawSysResCheck( const WF2DMAP_OBJDRAWSYS* cp_sys, u32 charid )
{
	return WF_2DC_ResCheck( cp_sys->p_drawsys, charid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�S���\�[�X�̔j��
 *	
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysAllResDel( WF2DMAP_OBJDRAWSYS* p_sys )
{
	WF_2DC_AllResDel( p_sys->p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^���\�[�X�o�^
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	movetype	����^�C�v
 *	@param	heap		�q�[�vID
 *

	movetype
	WF_2DC_MOVERUN,		// �����{�U������{����A�j���i��l���̂݁j
	WF_2DC_MOVENORMAL,	// �����{�U������̂݃A�j��
	WF_2DC_MOVETURN,	// �U������̂݃A�j��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysUniResSet( WF2DMAP_OBJDRAWSYS* p_sys, WF_2DC_MOVETYPE movetype, u32 heap )
{
	WF_2DC_UnionResSet( p_sys->p_drawsys, p_sys->draw_type, movetype, heap );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�I���L�����N�^���\�[�X�j��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysUniResDel( WF2DMAP_OBJDRAWSYS* p_sys )
{
	WF_2DC_UnionResDel( p_sys->p_drawsys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���I�u�W�F�N�g�̐���
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_objwk	�I�u�W�F�N�g���[�N
 *	@param	hero		��l���t���O	�i���̃I�u�W�F�N�g������:TRUE�@���l:FALSE�j
 *	@param	heapID		�q�[�v�h�c
 *
 *	@return	�\���I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJDRAWWK* WF2DMAP_OBJDrawWkNew( WF2DMAP_OBJDRAWSYS* p_sys, const WF2DMAP_OBJWK* cp_objwk, BOOL hero, u32 heapID )
{
	WF2DMAP_OBJDRAWWK* p_wk;
	WF_2DC_WKDATA add;
	s32 charid;

	p_wk = WF2DMAP_OBJDrawSysCleanWkGet( p_sys );
	
	p_wk->status = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_ST );
	p_wk->way	 = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_WAY );
	p_wk->playid = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_PLID );
	p_wk->cp_objwk = cp_objwk;	// �I�u�W�F�N�g���[�N�ۑ�
	p_wk->hero = hero;
	p_wk->updata = TRUE;
	p_wk->lastframe = 0;
	p_wk->lastanm = 0;

	// �Z���A�N�^�[�f�[�^�쐬
	add.x = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_X );
	add.y = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_Y );
	add.pri = WF2DMAP_OBJDrawWkDrawPriCalc( add.y, p_wk->hero );
	add.bgpri = p_sys->bg_pri;

	// �A�N�^�[�o�^
	charid = WF2DMAP_OBJWkDataGet( cp_objwk, WF2DMAP_OBJPM_CHARA );
	p_wk->p_drawwk = WF_2DC_WkAdd( p_sys->p_drawsys, &add, charid, heapID );

	// ��xAPDATA
	WF2DMAP_OBJDrawWkUpdata( p_wk );

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���I�u�W�F�N�g�̔j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkDel( WF2DMAP_OBJDRAWWK* p_wk )
{
	// ���[�N�j��
	WF_2DC_WkDel( p_wk->p_drawwk );
	memset( p_wk, 0, sizeof(WF2DMAP_OBJDRAWWK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�X�V
 *
 *	@param	p_sys		�I�u�W�F�N�g�\���Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawSysUpdata( WF2DMAP_OBJDRAWSYS* p_sys )
{
	int i;

	for( i=0; i<p_sys->objnum; i++ ){
		// �f�[�^�������Ă�����X�V
		if( WF2DMAP_OBJDrawWkCleanCheck(&p_sys->p_wk[i]) == FALSE ){
			WF2DMAP_OBJDrawWkUpdata( &p_sys->p_wk[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�X�V	�X��
 *
 *	@param	p_wk		�I�u�W�F�N�g�\�����[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkUpdata( WF2DMAP_OBJDRAWWK* p_wk )
{
	s32 status;
	s32 way;
	s16 set_x, set_y;
	s16 frame;
	u16 pri;
	WF_2DC_ANMTYPE anmtype;

	// �X�V�t���O�`�F�b�N
	if( p_wk->updata == FALSE ){
		return;
	}

	status = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_ST );
	way = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_WAY );
	frame = WF2DMAP_OBJWkDataGet( p_wk->cp_objwk, WF2DMAP_OBJPM_FRAME );
	
	// �f�[�^�X�V
	if( (p_wk->status != status) || (p_wk->way != way) || (frame == 0) ){
		p_wk->status = status;
		p_wk->way = way;
		// �A�j���f�[�^�ύX
		anmtype = WF2DMAP_OBJDrawWF2DMAP_OBJSTtoWF_2DC_ANMTYPE( p_wk->p_drawwk, status );
		WF2DMAP_OBJDrawWkAnmSet( p_wk, anmtype, way );
	}

	// ���W�����߂�
	WF2DMAP_OBJDrawWkFramePosGet( p_wk, &set_x, &set_y );
	WF_2DC_WkMatrixSet( p_wk->p_drawwk, set_x, set_y );
	pri = WF2DMAP_OBJDrawWkDrawPriCalc( set_y, p_wk->hero );
	WF_2DC_WkDrawPriSet( p_wk->p_drawwk, pri );
	
	// �A�j���[�V������i�߂�
	WF_2DC_WkPatAnmAddFrame( p_wk->p_drawwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�b�v�f�[�g�t���O�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		ON/OFF�t���O
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkUpdataFlagSet( WF2DMAP_OBJDRAWWK* p_wk, BOOL flag )
{
	p_wk->updata = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�b�v�f�[�g�t���O�̎擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�t���O
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_OBJDrawWkUpdataFlagGet( const WF2DMAP_OBJDRAWWK* cp_wk )
{ 
	return cp_wk->updata;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�A�j���̊J�n
 *
 *	@param	p_wk	���[�N
 *
 *	����]�A�j���I�������s����܂ŉ�葱����
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkKuruAnimeStart( WF2DMAP_OBJDRAWWK* p_wk )
{
	WF2DMAP_OBJDrawWkAnmSet( p_wk, WF_2DC_ANMROTA, p_wk->way );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�A�j�����C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkKuruAnimeMain( WF2DMAP_OBJDRAWWK* p_wk )
{
	// �A�j���[�V������i�߂�
	WF_2DC_WkPatAnmAddFrame( p_wk->p_drawwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��]�A�j���I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkKuruAnimeEnd( WF2DMAP_OBJDRAWWK* p_wk )
{
	WF_2DC_ANMTYPE anmtype;

	// �A�j���f�[�^�ύX
	anmtype = WF2DMAP_OBJDrawWF2DMAP_OBJSTtoWF_2DC_ANMTYPE( p_wk->p_drawwk, p_wk->status );
	WF2DMAP_OBJDrawWkAnmSet( p_wk, anmtype, p_wk->way );

	// �P��A�b�v�f�[�g
	WF2DMAP_OBJDrawWkUpdata( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`����W�̎擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	���W
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WF2DMAP_OBJDrawWkMatrixGet( const WF2DMAP_OBJDRAWWK* cp_wk )
{
	WF2DMAP_POS pos;

	pos.x = WF_2DC_WkMatrixGet( cp_wk->p_drawwk, WF_2DC_GET_X );
	pos.y = WF_2DC_WkMatrixGet( cp_wk->p_drawwk, WF_2DC_GET_Y );

	return pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`����W�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	pos			���W
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkMatrixSet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_POS pos )
{
	WF_2DC_WkMatrixSet( p_wk->p_drawwk, pos.x, pos.y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\����\�����擾
 *
 *	@param	cp_wk	���[�N
 *	
 *	@retval	TRUE	�\��
 *	@retval	FALSE	��\��
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_OBJDrawWkDrawFlagGet( const WF2DMAP_OBJDRAWWK* cp_wk )
{
	return WF_2DC_WkDrawFlagGet( cp_wk->p_drawwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\����\���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkDrawFlagSet( WF2DMAP_OBJDRAWWK* p_wk, BOOL flag )
{
	WF_2DC_WkDrawFlagSet( p_wk->p_drawwk, flag );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���D�揇�ʂ̎擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�\���D�揇��
 */
//-----------------------------------------------------------------------------
u16 WF2DMAP_OBJDrawWkDrawPriGet( const WF2DMAP_OBJDRAWWK* cp_wk )
{
	return WF_2DC_WkDrawPriGet( cp_wk->p_drawwk );
}

//----------------------------------------------------------------------------
/**
 *	@brieif	��~��Ԃ̊G�̕����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	way			����
 *
 *	*�����Ă����肵�Ă��Ƃ܂��Ă���G�̕������o���܂��B
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkWaySet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_WAY way )
{
	WF2DMAP_OBJDrawWkAnmSet( p_wk, WF_2DC_ANMWAY, way );
	WF_2DC_WkPatAnmAddFrame( p_wk->p_drawwk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���D�揇�ʎ擾
 *
 *	@param	y		�����W
 *	@param	hero	��l���@��
 *
 *	@return	�\���D�揇��
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_OBJDrawWkDrawPriCalc( s16 y, BOOL hero )
{
	if( hero == TRUE ){
		return WF2DMAP_DRAWPRI_HERO_GET(y);
	}
	return WF2DMAP_DRAWPRI_GET(y);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�ƃp���b�g�i���o�[���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	���Q�Ƃ��Ă���p���b�g�ʒu
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_OBJDrawWkPaletteNoGet( const WF2DMAP_OBJDRAWWK* cp_wk )
{
	return CLACT_PaletteOffsetGet( WF_2DC_WkConstClWkGet( cp_wk->p_drawwk ) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�̈ʒu��ݒ肷��
 *	
 *	@param	p_wk	���[�N
 *	@param	pos		�A�ʒu
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJDrawWkShadowMatrixSet( WF2DMAP_OBJDRAWWK* p_wk, WF2DMAP_POS pos )
{
	WF_2DC_WkShadowMatrixSet( p_wk->p_drawwk, pos.x, pos.y );
}



//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F���[�N���󂢂Ă��邩�`�F�b�N
 *	
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�󂢂Ă���
 *	@retval	FALSE	�󂢂Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJDrawWkCleanCheck( const WF2DMAP_OBJDRAWWK* cp_wk )
{
	if( cp_wk->p_drawwk == NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��̃I�u�W�F�N�g���[�N���擾
 *
 *	@param	p_sys		�I�u�W�F�N�g�\���V�X�e��
 *
 *	@return	��̃I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static WF2DMAP_OBJDRAWWK* WF2DMAP_OBJDrawSysCleanWkGet( WF2DMAP_OBJDRAWSYS* p_sys )
{
	int i;

	for( i=0; i<p_sys->objnum; i++ ){
		if( WF2DMAP_OBJDrawWkCleanCheck( &p_sys->p_wk[i] ) == TRUE ){
			return &p_sys->p_wk[i];
		}
	}

	GF_ASSERT(0);
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�e�[�^�X���A�j���[�V�����^�C�v�ɃR���o�[�g����
 *
 *	@param	p_drawwk	wifi_2dchar
 *	@param	status		���
 *
 *	@return	�A�j���[�V�����^�C�v
 */
//-----------------------------------------------------------------------------
static WF_2DC_ANMTYPE WF2DMAP_OBJDrawWF2DMAP_OBJSTtoWF_2DC_ANMTYPE( const WF_2DCWK* p_drawwk, WF2DMAP_OBJST status )
{
	static const u8 WF2DMAP_OBJSTtoWF_2DC_ANMTYPE[ WF2DMAP_OBJST_NUM ] = {
		WF_2DC_ANMWAY,
		WF_2DC_ANMTURN,
		WF_2DC_ANMWALK,
		WF_2DC_ANMRUN,
		WF_2DC_ANMWAY,
		WF_2DC_ANMWALLWALK,
		WF_2DC_ANMSLOWWALK,
		WF_2DC_ANMHIGHWALK4,
		WF_2DC_ANMHIGHWALK2,
		WF_2DC_ANMHIGHWALK4,
		WF_2DC_ANMWALK,
		WF_2DC_ANMSLOWWALK,
	};
	WF_2DC_MOVETYPE movetype;

	// ����^�C�v�擾
	movetype = WF_2DC_WkMoveTypeGet( p_drawwk );
	
	// ����^�C�v���U������݂̂Ȃ�ǂ�ȂƂ��ł��U����������ł��Ȃ�
	if( movetype == WF_2DC_MOVETURN ){
		return WF_2DC_ANMWAY;
	}

	return WF2DMAP_OBJSTtoWF_2DC_ANMTYPE[ status ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[����������W���擾����
 *
 *	@param	cp_wk		���[�N
 *	@param	p_x			���W�ݒ��
 *	@param	p_y			���W�ݒ��
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawWkFramePosGet( const WF2DMAP_OBJDRAWWK* cp_wk, s16* p_x, s16* p_y )
{
	WF2DMAP_POS pos;

	pos = WF2DMAP_OBJWkFrameMatrixGet( cp_wk->cp_objwk );

	*p_x = pos.x;
	*p_y = pos.y;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���[�V������ݒ�
 *
 *	@param	p_wk		���[�N		
 *	@param	anmtype		�A�j���^�C�v
 *	@param	anmway		�A�j������
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawWkAnmSet( WF2DMAP_OBJDRAWWK* p_wk, WF_2DC_ANMTYPE anmtype, WF_COMMON_WAY anmway )
{
	u16 frame;
	u16 lastanmtype;
	static const u8 WF2DMAP_OBJFRAME_NEXT[ WF_2DC_ANMNUM ] = {
		FALSE,		// �����ς�
		FALSE,		// ��]
		TRUE,		// ����
		FALSE,		// �U�����
		TRUE,		// ����
		TRUE,		// �Ǖ���
		TRUE,		// ����������
		TRUE,		// ��������2�t���[��
		TRUE,		// ��������3�t���[��
		FALSE,		// �����Ȃ�
	};

	// �`��t���[�����擾
	frame = WF_2DC_WkAnmFrameGet( p_wk->p_drawwk ); 
	lastanmtype = WF_2DC_WkAnmTypeGet( p_wk->p_drawwk );

	// �A�j���[�V�����ύX
	WF_2DC_WkPatAnmStart( p_wk->p_drawwk, anmtype, anmway );

	// ���̃A�j���Ɛݒ肵���A�j�����Ⴄ�Ƃ�
	// ���������A����Ȃ�ȑO�̃t���[������J�n������
	if( lastanmtype != anmtype ){
		
		// �A�j���[�V�����t���[�����p���Ȃ̂��`�F�b�N
		if( WF2DMAP_OBJFRAME_NEXT[ anmtype ] == TRUE ){
			// �P�O�A�j���^�C�v�Ɠ����Ȃ�t���[�������ꂩ��ɂ���
			if( p_wk->lastanm == anmtype ){
//				TOMOYA_PRINT( "anmtype %d anmframe %d\n", anmtype, p_wk->lastframe );
				WF_2DC_WkAnmFrameSet( p_wk->p_drawwk, p_wk->lastframe );
			}
		}

		// �ۑ��f�[�^�X�V
		if( WF2DMAP_OBJFRAME_NEXT[ lastanmtype ] == TRUE ){
			p_wk->lastanm = lastanmtype;
			p_wk->lastframe = frame;
		}
	}
}
