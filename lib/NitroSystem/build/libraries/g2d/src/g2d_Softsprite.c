/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Softsprite.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.31 $

 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nnsys/g2d/g2d_Softsprite.h>


static  NNSG2dExtendedSprite              defaultSpriteAttr_;
static  u32                         defaultAttrEnable_  = 0x0;
static  u16                         currentPolygonID_   = 0x0;



static void DrawSprite_( const NNSG2dSimpleSprite* pS, const NNSG2dBasicSprite* pB, const NNSG2dExtendedSprite* pE );

//---------------------------------------------------------------------------
// �J�����g�s���ۑ����Ȃ��悤�ɁA�d�l���ύX�ɂȂ�܂����B
// �{�֐��𗘗p����֐��͂��ׂē��l�̓����������ƂƂȂ�܂��B
static void DrawSprite_( const NNSG2dSimpleSprite* pS, const NNSG2dBasicSprite* pB, const NNSG2dExtendedSprite* pE )
{
    const fx32      size = FX32_HALF;
    
    fx32 U0 = 0; 
    fx32 U1 = pS->size.x << FX32_SHIFT; 
    
    fx32 V0 = 0; 
    fx32 V1 = pS->size.y << FX32_SHIFT; 
    
    // Overwrite the UV values.
    if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_UV ) )
    {
    
        U0 = pE->uvUL.x;
        U1 = pE->uvLR.x;
        V0 = pE->uvUL.y;
        V1 = pE->uvLR.y;
    }
    
    // Flip 
    if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_FLIP ) )
    {
        fx32    temp;
        // H
        if( pE->flipH )
        {
            temp = U0;
            U0 = U1;
            U1 = temp;
        }
        // V
        if( pE->flipV )
        {
            temp = V0;
            V0 = V1;
            V1 = temp;
        }
    }
    
    {
        //
        // SRT
        //
        G3_Translate( ( pS->pos.x << FX32_SHIFT ) + ((pS->size.x >> 1) << FX32_SHIFT ), 
                      ( pS->pos.y << FX32_SHIFT ) + ((pS->size.y >> 1) << FX32_SHIFT ), 
                      ( (int)pS->priority << FX32_SHIFT ) );
                      
        // RotO
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_ROTO ) )
        {
            G3_Translate( pE->rotO.x << FX32_SHIFT , 
                          pE->rotO.y << FX32_SHIFT , 
                          0 );            
        }
        // RotXY
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_ROTXY ) )
        {
            G3_RotX( FX_SinIdx( pE->rotX ), FX_CosIdx( pE->rotX ) );
            G3_RotY( FX_SinIdx( pE->rotY ), FX_CosIdx( pE->rotY ) );
        }
        G3_RotZ( FX_SinIdx( pS->rotZ ), FX_CosIdx( pS->rotZ ) );
        
        
        // RotO
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_ROTO ) )
        {
            G3_Translate( -pE->rotO.x << FX32_SHIFT , 
                          -pE->rotO.y << FX32_SHIFT , 
                          0 );
        }
        G3_Scale( pS->size.x << FX32_SHIFT, 
                  pS->size.y << FX32_SHIFT, 
                  FX32_ONE );
        
        
        //
        //
        //
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_TEXTURE ) )
        {
            // Texture setting.
            if( pB->pTextureAttr )
            {
                const NNSG2dImageAttr* pAttr = pB->pTextureAttr;
                
                G3_TexImageParam( pAttr->fmt,        // use alpha texture
                         GX_TEXGEN_TEXCOORD,         // use texcoord
                         pAttr->sizeS,               // 16 pixels
                         pAttr->sizeT,               // 16 pixels
                         GX_TEXREPEAT_NONE,          // no repeat
                         GX_TEXFLIP_NONE,            // no flip
                         pAttr->plttUse,             // use color 0 of the palette
                         pB->texAddr                 // the offset of the texture image
                         );
                
                //
                // �p���b�g�t�H�[�}�b�g�Ȃ��...
                //                         
                if( NNS_G2dIsPaletteImageFmt( pAttr ) )
                {
                    G3_TexPlttBase( (u32)(pB->plttAddr + 32 * pB->plttOffset), pAttr->fmt );                                
                }
                
            }else{
                OS_Warning("The Texture Attribute of software sprite must be specified.");
            }
        }
        
        //
        // alpha blending
        //
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA ) )
        {
            //
            // ���� : alpha = 0 �Ȃ�Ε`����s���܂��� 
            //
            if( pS->alpha == 0 )
            {
                return;
            }
            
            //
            // ���ӁFG2D�͓����� Graphics Engine �̃��W�X�^�ݒ������ɕύX���܂��B
            //       �܂��ݒ�̕������s���܂���B
            //
            G3_PolygonAttr(GX_LIGHTMASK_NONE,              // disable lights
                   GX_POLYGONMODE_MODULATE,                // modulation mode
                   GX_CULL_NONE,                           // cull back
                   currentPolygonID_,                      // polygon ID(0 - 63)
                   pS->alpha,                              // alpha(0 - 31)
                   0                                       // OR of GXPolygonAttrMisc's value
                   );
        }
        //
        // VertexColor
        //
        if( NNS_G2dIsSpriteAttrEnable( NNS_G2D_SPRITEATTR_COLOR ) )
        {
            G3_Color( pB->color );
        }
        
        //
        // Draw a quad polygon.(�`�敽�ʂ̕�����ύX���܂���)
        //
        G3_Begin( GX_BEGIN_QUADS );
            
            G3_TexCoord( U0, V1 );
            G3_Vtx10( 0 - size,0 + size, 0 ); 
            
            G3_TexCoord( U1, V1 );
            G3_Vtx10( 0 + size, 0 + size, 0 ); 
            
            G3_TexCoord( U1, V0 );
            G3_Vtx10( 0 + size, -size, 0 ); 
            
            G3_TexCoord( U0, V0 );
            G3_Vtx10( 0 - size, -size, 0 );
            
        G3_End( );
    }
}

//------------------------------------------------------------------------------
static void SetAffine43( MtxFx43* pDst, const MtxFx22* pSubMtx, fx32 x, fx32 y, fx32 z )
{
    NNS_G2D_NULL_ASSERT(pDst);
    NNS_G2D_NULL_ASSERT(pSubMtx);
    
    
    pDst->_00 = pSubMtx->_00;
    pDst->_01 = pSubMtx->_01;
    pDst->_02 = 0;
    pDst->_10 = pSubMtx->_10;
    pDst->_11 = pSubMtx->_11;
    pDst->_12 = 0;
    pDst->_20 = 0;
    pDst->_21 = 0;
    pDst->_22 = FX32_ONE;
    pDst->_30 = x;
    pDst->_31 = y;
    pDst->_32 = z;
}
//------------------------------------------------------------------------------
static void SetBaseMtx_( const MtxFx22* baseMtx )
{
    //
    // RTS
    //
    MtxFx43       mtx;
    SetAffine43( &mtx, baseMtx, 0, 0, 0 );
    
    // base
    G3_MultMtx43( &mtx );
    
}
//------------------------------------------------------------------------------
static void DrawOneQuad_( fx32 U0, fx32 U1, fx32 V0, fx32 V1 )
{
    const fx32      size = FX32_ONE;
            
    {
        //
        // Draw a quad polygon.
        //
        G3_Begin( GX_BEGIN_QUADS);
            
            G3_TexCoord( U0, V1 );
            G3_Vtx( 0 ,0 + size, 0 ); 
            
            G3_TexCoord( U1, V1 );
            G3_Vtx( 0 + size, 0 + size, 0 ); 
            
            G3_TexCoord( U1, V0 );
            G3_Vtx( 0 + size, 0 , 0 ); 
            
            G3_TexCoord( U0, V0 );
            G3_Vtx( 0 , 0, 0 );
            
            
        G3_End( );
    }
}

//------------------------------------------------------------------------------
// OAM�}�l�[�W���ɂ�����A2D �G�~�����[�V�������̕`��݂̂Ɏg�p����܂��B
// UV �� �ݒ�� �O���ōs������
static void DrawOneQuadCenter_( fx32 U0, fx32 U1, fx32 V0, fx32 V1 )
{
    const fx32      size = FX32_HALF;
            
    {
        //
        // Draw a quad polygon.
        //
        G3_Begin( GX_BEGIN_QUADS);
            
            G3_TexCoord( U0, V1 );
            G3_Vtx( 0 -size ,0 + size, 0 ); 
            
            G3_TexCoord( U1, V1 );
            G3_Vtx( 0 + size, 0 + size, 0 ); 
            
            G3_TexCoord( U1, V0 );
            G3_Vtx( 0 + size, -size , 0 ); 
            
            G3_TexCoord( U0, V0 );
            G3_Vtx( -size , -size, 0 );
            
        G3_End( );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetupSoftwareSpriteMaterial

  Description:  �X�v���C�g�`��̂��߂̂RD�O���t�B�b�N�X�G���W���̃}�e���A��
  �@�@�@�@�@�@�@�ݒ���s���܂��B
                
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetupSoftwareSpriteMaterial()
{
    // �W���I�ȃ}�e���A���ݒ�����܂��B
    {
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // diffuse
                                GX_RGB(16, 16, 16),        // ambient
                                TRUE                       // use diffuse as vtx color if TRUE
                                );

        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),        // specular
                                GX_RGB(0, 0, 0),           // emission
                                FALSE                      // use shininess table if TRUE
                                );

        G3_PolygonAttr(GX_LIGHTMASK_NONE,                  // no lights
                       GX_POLYGONMODE_MODULATE,            // modulation mode
                       GX_CULL_NONE,                       // cull back
                       0,                                  // polygon ID(0 - 63)
                       31,                                 // alpha(0 - 31)
                       0                                   // OR of GXPolygonAttrMisc's value
                       );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetupSoftwareSpriteCamera

  Description:  �X�v���C�g�`��̂��߂̂RD�O���t�B�b�N�X�G���W���̃J�����ݒ���s���܂��B
                
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetupSoftwareSpriteCamera()
{
    G3_ViewPort(0, 0, 255, 191);                           // Viewport
    {
        // ���e�s���ݒ肵�܂��B
        {
            G3_OrthoW( FX32_ONE * 0,
                      FX32_ONE * 192,
                      FX32_ONE * 0,
                      FX32_ONE * 256,
                      FX32_ONE * -1024,
                      FX32_ONE * 1024,
                      FX32_ONE * 1024,
                      NULL );

            G3_StoreMtx(0);
        }
        
        // �J�����s���ݒ肵�܂��B
        {
            // �P�ʍs��Ɠ���
            VecFx32 Eye    = { 0, 0, 0 };          // Eye position
            VecFx32 vUp    = { 0, FX32_ONE, 0 };  // Up
            VecFx32 at     = { 0, 0, -FX32_ONE }; // Viewpoint

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }

        // G3_LookAt�̎��s�ŁA�s�񃂁[�h��GX_MTXMODE_POSITION_VECTOR �ɕύX�����̂�
        // �s�񃂁[�h�����ɖ߂��܂�
        G3_MtxMode( GX_MTXMODE_POSITION );        
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetSpriteAttrEnable

  Description:  SoftwareSprite�̗L��������ݒ肵�܂�
                
                
  Arguments:    attr:         [IN]  NNSG2dSpriteAttr �� �e�l�������͂��̘_���a
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetSpriteAttrEnable( u32 attr )
{
    defaultAttrEnable_ = attr;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsSpriteAttrEnable

  Description:  SoftwareSprite�̑������L�������肵�܂�
                
                
  Arguments:    attr:         [IN]  NNSG2dSpriteAttr �� �e�l( ���ӁF�_���a�͂Ƃ�܂���)
                
  Returns:      �������L����
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsSpriteAttrEnable( NNSG2dSpriteAttr attr )
{
    return (BOOL)(defaultAttrEnable_ & (u32)attr);
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetSpriteCurrentPolygonID 

  Description:  SoftwareSprite�`��Ɏg�p�����|���S��ID���w�肵�܂�
                
                
  Arguments:    id:         [IN]  SoftwareSprite�`��Ɏg�p�����|���S��ID(0-63)
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetSpriteCurrentPolygonID( u16 id )
{
    NNS_G2D_MINMAX_ASSERT( id, 0, 63 ); 
    
    currentPolygonID_ = id; 
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetSpriteDefaultAttr

  Description:  SoftwareSprite�̃f�t�H���g�l��ݒ肵�܂�
                �s��������͂��̒l�ɂ���ĕ���܂��B
                
                
  Arguments:    pE:         [IN]  �X�v���C�g
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetSpriteDefaultAttr( const NNSG2dExtendedSprite* pE )
{
    defaultSpriteAttr_ = *pE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetSpriteDefaultAttr

  Description:  SoftwareSprite�̃f�t�H���g�l�̎Q�ƃ|�C���^�𓾂܂�
                
  Arguments:    �Ȃ�
                
  Returns:      SoftwareSprite�̃f�t�H���g�l�̎Q�ƃ|�C���^
  
 *---------------------------------------------------------------------------*/
const NNSG2dExtendedSprite* NNS_G2dGetSpriteDefaultAttr( )
{
    return &defaultSpriteAttr_;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawSpriteSimpleFast ... NNS_G2dDrawSpriteExtendedFast

  Description:  SoftwareSprite��`�悵�܂�
                �s��������̓f�t�H���g�l�ɂ���ĕ���܂��B
                �֐��Ăяo���̑O��ŃW�I���g���G���W���̃J�����g�s��̓��e��
                �ۑ�����Ȃ��_�ɂ����ӂ��������B
                
                �ۑ������o�[�W�����Ƃ��āA�֐�������Fast���t���Ȃ�
                NNS_G2dDrawSpriteSimple ... NNS_G2dDrawSpriteExtended
                ���p�ӂ���Ă��܂��B
                
                
  Arguments:    pS,pB,pE:         [IN]  �X�v���C�g
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawSpriteSimpleFast( const NNSG2dSimpleSprite* pS )
{
    DrawSprite_( pS, &defaultSpriteAttr_.basicParams, &defaultSpriteAttr_ ); 
}
//---------------------------------------------------------------------------
void NNS_G2dDrawSpriteBasicFast( const NNSG2dBasicSprite* pB )
{
    DrawSprite_( &pB->simpleParams, pB, &defaultSpriteAttr_ ); 
}
//---------------------------------------------------------------------------
void NNS_G2dDrawSpriteExtendedFast( const NNSG2dExtendedSprite* pE )
{
    DrawSprite_( &pE->basicParams.simpleParams, &pE->basicParams, pE ); 
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawSpriteFast

  Description:  SoftwareSprite�̕`��
                
                
  Arguments:    px:         [IN]  x
                py:         [IN]  y
                pz:         [IN]  z
                sx:         [IN]  size x
                sy:         [IN]  size y
                U0:         [IN]  UV�l(����U)
                V0:         [IN]  UV�l(����V)
                U1:         [IN]  UV�l(�E��U)
                V1:         [IN]  UV�l(�E��V)
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawSpriteFast
( 
    s16 px, s16 py, int pz, 
    int sx, int sy, 
    int U0, int V0, int U1, int V1 
)
{
    // T
    G3_Translate( FX32_ONE * px, FX32_ONE * py, FX32_ONE * pz );    
    // S
    G3_Scale( FX32_ONE * sx, FX32_ONE * sy, FX32_ONE );
       
    DrawOneQuad_( FX32_ONE * U0, FX32_ONE * U1, FX32_ONE * V0, FX32_ONE * V1 );    
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dDrawSpriteWithMtxFast

  Description:  �X�v���C�g��`�悵�܂�
                Z�l�AUV�l�̌^�� NNS_G2dDrawSpriteFast �ƈقȂ�܂��B
                �����_���R�A���W���[�����痘�p����Ă��܂��B             
                
                
  Arguments:    �ȗ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSi_G2dDrawSpriteFast
( 
    s16 px, s16 py, fx32 pz, 
    int sx, int sy, 
    fx32 U0, fx32 V0, fx32 U1, fx32 V1 
)
{
    // T
    G3_Translate( px << FX32_SHIFT, py << FX32_SHIFT, pz );    
    // S
    G3_Scale( sx << FX32_SHIFT, sy << FX32_SHIFT, FX32_ONE );
       
    {
       const fx32      size = FX32_ONE;
           
           
       //
       // Draw a quad polygon.
       //
       G3_Begin( GX_BEGIN_QUADS);
           
           G3_TexCoord( U0, V1 );
           G3_Vtx( 0 ,0 + size, 0 ); 
                       
           G3_TexCoord( U1, V1 );
           G3_Vtx( 0 + size, 0 + size, 0 ); 
                   
           G3_TexCoord( U1, V0 );
           G3_Vtx( 0 + size, 0 , 0 ); 
               
           G3_TexCoord( U0, V0 );
           G3_Vtx( 0 , 0, 0 );
          
       G3_End( );
    }
    
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dDrawSpriteWithMtxFast

  Description:  �X�v���C�g��`�悵�܂�
                �����ōs����w�肵�܂��B
                �{�p�A�t�B�����[�h�ł�OBJ�Ɠ����悤�ɃX�v���C�g�̕`����s���܂��B
                OAM�}�l�[�W���̃\�t�g�E�F�A�X�v���C�g�E�G�~�����[�V�����@�\�g�p����
                ���p�����֐��ł��B
                
                
  Arguments:    �ȗ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSi_G2dDrawSpriteWithMtxFast
( 
    s16 px, s16 py, fx32 pz, 
    int sx, int sy, 
    const MtxFx22* baseMtx, 
    fx32 U0, fx32 V0, fx32 U1, fx32 V1 
)
{
    // T
    G3_Translate( FX32_ONE * px + FX32_HALF * sx, FX32_ONE * py + FX32_HALF * sy, pz );    
    // R or S
    SetBaseMtx_( baseMtx );
    // S
    G3_Scale( FX32_ONE * sx, FX32_ONE * sy, FX32_ONE );
       
    DrawOneQuadCenter_( U0, U1, V0, V1 );

}




/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dDrawSpriteWithMtxDoubleAffineFast

  Description:  �X�v���C�g��`�悵�܂��B
                �����ōs����w�肵�܂��B
                �{�p�A�t�B�����[�h�ł�OBJ�Ɠ����悤�ɃX�v���C�g�̕`����s���܂��B
                OAM�}�l�[�W���̃\�t�g�E�F�A�X�v���C�g�E�G�~�����[�V�����@�\�g�p����
                ���p�����֐��ł��B
                
                
  Arguments:    �ȗ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSi_G2dDrawSpriteWithMtxDoubleAffineFast
( 
    s16 px, s16 py, fx32 pz, 
    int sx, int sy, 
    const MtxFx22* baseMtx, 
    fx32 U0, fx32 V0, fx32 U1, fx32 V1 
)
{
    // T
    // FX32_HALF �ł͂Ȃ� FX32_ONE �ł���_�ɒ��ӁF�{�p���[�h�̐U�镑��
    G3_Translate( FX32_ONE * px + FX32_ONE * sx, FX32_ONE * py + FX32_ONE * sy, pz );    
    // R or S
    SetBaseMtx_( baseMtx );
    // S
    G3_Scale( FX32_ONE * sx, FX32_ONE * sy, FX32_ONE );
       
    DrawOneQuadCenter_( U0, U1, V0, V1 );
}










