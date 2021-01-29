//==============================================================================================
/**
 * @file	factory_3d.c
 * @brief	�u�o�g���t�@�N�g���[�v3D
 * @author	Satoshi Nohara
 * @date	2007.03.20
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/main.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "system/softsprite.h"

#include "factory_sys.h"


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void Factory_SimpleSetUp( void );
void Factory_3DMain( SOFT_SPRITE_MANAGER* ssm_p );
POKEMON_PARAM* Factory_PokeMake( u16 monsno, u8 level, u16 itemno, int place_id, int ground_id );
SOFT_SPRITE* Factory_SoftSpritePokeAdd( SOFT_SPRITE_MANAGER *ssm_p, int poly_id, POKEMON_PARAM* pp, int x, int y, int z );
void Factory_PokeHFlip( SOFT_SPRITE* ss, int flag );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief   3D�V���v���Z�b�g�A�b�v
 *
 * @param	none
 *
 * @retval  none
 */
//--------------------------------------------------------------
void Factory_SimpleSetUp( void )
{
	//�R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority( 0 );

	//�e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );			//�A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );				//�A���t�@�u�����h�@�I��
	G3X_EdgeMarking( FALSE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	//�N���A�J���[�̐ݒ�
    G3X_SetClearColor( GX_RGB(0,0,0), 0, 0x7fff, 63, FALSE);	//color,alpha,depth,polygonID,fog

	//�r���[�|�[�g�̐ݒ�
    G3_ViewPort( 0, 0, 255, 191 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	3D���C������
 *
 * @param   ssm_p	�\�t�g�E�F�A�X�v���C�g�}�l�[�W���ւ̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Factory_3DMain( SOFT_SPRITE_MANAGER* ssm_p )
{
	GF_G3X_Reset();
	//GFC_CameraLookAt();

	G3_MtxMode( GX_MTXMODE_PROJECTION );
	G3_Identity();
	G3_MtxMode( GX_MTXMODE_POSITION_VECTOR );
	G3_Identity();

	NNS_G3dGlbFlush();

	NNS_G2dSetupSoftwareSpriteCamera();

	//���C�����[�v�ŌĂяo��
	SoftSpriteMain( ssm_p );

	G3_SwapBuffers( GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z );

	return;
}

//------------------------------------------------------------------
/**
 * @brief	�|�P������������
 *
 * @param	monsno
 * @param	level
 * @param	itemno
 * @param	place_id
 * @param	ground_id
 *
 * @retval	"�|�P�����̃|�C���^"
 */
//------------------------------------------------------------------
POKEMON_PARAM* Factory_PokeMake( u16 monsno, u8 level, u16 itemno, int place_id, int ground_id )
{
	POKEMON_PARAM* poke_param;
	u32 buf;

	poke_param = PokemonParam_AllocWork( HEAPID_FACTORY );
	PokeParaInit( poke_param );
	PokeParaSet( poke_param, monsno, level, POW_RND, RND_NO_SET, 0, ID_NO_SET, 0 );

	//PokeParaGetInfoSet( poke_param, SaveData_GetMyStatus(sv), 
	//						ITEM_MONSUTAABOORU, place_id, ground_id, heapID );
	
	//buf = itemno;
	//PokeParaPut( poke_param, ID_PARA_item, &buf );
	
	return poke_param;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����\�t�g�E�F�A�X�v���C�g�����֐�
 *
 * @param   ssm_p		�\�t�g�E�F�A�X�v���C�g�}�l�[�W���ւ̃|�C���^
 * @param   poly_id		�|���S��ID
 * @param   pp			POKEMON_PARAM�ւ̃|�C���^
 * @param   dir			�|�P�����̌���(PARA_FRONT or PARA_BACK)
 * @param   heap_id		�q�[�vID(�ꎞ�g�p)
 * @param   x			�\�����WX
 * @param   y			�\�����WY
 * @param   z			�\�����WZ
 *
 * @retval  �\�t�g�E�F�A�X�v���C�g�̃|�C���^
 */
//--------------------------------------------------------------
SOFT_SPRITE* Factory_SoftSpritePokeAdd( SOFT_SPRITE_MANAGER *ssm_p, int poly_id, POKEMON_PARAM* pp, int x, int y, int z )
{
	SOFT_SPRITE_ARC ssa;
	SOFT_SPRITE	*ss;
	int height;
	u32 monsno;
	
	PokeGraArcDataGetPP( &ssa, pp, PARA_FRONT );

	//���������킹�鏈��
	//height = PokeParaHeightGet( pp, PARA_FRONT );			//�������擾
	height = 0;

	ss = SoftSpriteAdd( ssm_p, &ssa, x, (y + height), z, poly_id, NULL, NULL );
	return ss;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�������]
 *
 * @param	ss		SOFT_SPRITE�^�̃|�C���^
 * @param	flag	���E���]�����邩
 *
 * @retval  none
 */
//--------------------------------------------------------------
void Factory_PokeHFlip( SOFT_SPRITE* ss, int flag )
{
	SoftSpriteParaSet( ss, SS_PARA_H_FLIP, flag );	//���E���]�t���O
	return;
}	


