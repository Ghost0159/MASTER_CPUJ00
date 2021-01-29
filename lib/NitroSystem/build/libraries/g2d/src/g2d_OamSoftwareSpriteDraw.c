/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_OamSoftwareSpriteDraw.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.25 $
 *---------------------------------------------------------------------------*/
 
#include <nnsys/g2d/g2d_OAM.h>
#include <nnsys/g2d/g2d_Softsprite.h>

#include <nnsys/g2d/g2d_OamSoftwareSpriteDraw.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>


#include "g2d_Internal.h"

#define NNSI_G2D_SHIFT_SIZEOF_256PLTT   9   // means ( sizeof( u16 ) * 256 )
#define NNSI_G2D_SHIFT_SIZEOF_16PLTT    5   // means ( sizeof( u16 ) * 16 )

typedef struct SoftwareSpriteParamCache
{
    fx32       u0;
    fx32       v0;
    fx32       u1;
    fx32       v1;
    
}SoftwareSpriteParamCache;

typedef struct SpriteParams
{
    fx32   u0;
    fx32   v0;
    fx32   u1;
    fx32   v1;
    
    int    sx;
    int    sy;
    
}SpriteParams;



//
// �p�����[�^�L���b�V��
// �܂����������e�N�X�`���ݒ�̃X�v���C�g���ʂɕ`�悷��ۂɎg�p���܂��B
static SoftwareSpriteParamCache         softwareSpreiteParamCache_ =
{
    0,
    0,  
    0,
    0
};




// Oam�\�t�g�E�G�A�X�v���C�g�G�~�����[�V���� �Ŏg�p����� UV�l�␳�֐��|�C���^
static NNS_G2dOamSoftEmuUVFlipCorrectFunc        s_pUVFlipCorrectFunc   = NULL;



// GXTexFmt �L�����N�^�[�T�C�Y�e�[�u��
static const u32 characterShiftSize_[] = 
{
    0,// GX_TEXFMT_NONE   
    0,// GX_TEXFMT_A3I5   
    0,// GX_TEXFMT_PLTT4  
    5,// GX_TEXFMT_PLTT16 
    6,// GX_TEXFMT_PLTT256
    0,// GX_TEXFMT_COMP4x4
    0,// GX_TEXFMT_A5I3  
    0 // GX_TEXFMT_DIRECT
};
// GXOamShape ���� GXTexSizeS �� �e�[�u���������܂�
static const GXTexSizeS gxTexSizeSTbl [3][4] = 
{
           {
           GX_TEXSIZE_S8,
           GX_TEXSIZE_S16,
           GX_TEXSIZE_S32,
           GX_TEXSIZE_S64
           },
           {
           GX_TEXSIZE_S16,
           GX_TEXSIZE_S32,
           GX_TEXSIZE_S32,
           GX_TEXSIZE_S64
           },
           {
           GX_TEXSIZE_S8,
           GX_TEXSIZE_S8,
           GX_TEXSIZE_S16,
           GX_TEXSIZE_S32
           }
};    

// GXOamShape ���� GXTexSizeT �� �e�[�u���������܂�
static const GXTexSizeT gxTexSizeTTbl [3][4] = 
{
           {
           GX_TEXSIZE_T8,
           GX_TEXSIZE_T16,
           GX_TEXSIZE_T32,
           GX_TEXSIZE_T64
           },
           {
           GX_TEXSIZE_T8,
           GX_TEXSIZE_T8,
           GX_TEXSIZE_T16,
           GX_TEXSIZE_T32
           },
           {
           GX_TEXSIZE_T16,
           GX_TEXSIZE_T32,
           GX_TEXSIZE_T32,
           GX_TEXSIZE_T64        
           }
};

static BOOL         bAutoZOffsetAdd_    = FALSE;    // �X�v���C�g��`�悷�邽�т�Z�I�t�Z�b�g���������Z���邩�ǂ���
static fx32         zOffset_            = 0;        // �X�v���C�g�ɗ^������AZ�I�t�Z�b�g
static fx32         zOffsetStep_        = -FX32_ONE; // �X�v���C�g�ɗ^������AZ�I�t�Z�b�g�̑�����






//------------------------------------------------------------------------------
// GXTexSizeS����e�N�X�`���T�C�Y �� u32 �l�� �擾���܂�
static NNS_G2D_INLINE u32 GetNumTexChar_( GXTexSizeS texSize )
{
    static const u32 texSize_ [] = 
    {
        1,
        2,
        4,
        8,
        16,
        32,
        64,
        128   
    };
    
    GX_TEXSIZE_S_ASSERT( texSize );
    
    return texSize_[ texSize ];
}

//------------------------------------------------------------------------------
// ����Z�I�t�Z�b�g�̃I�t�Z�b�g�l���X�V����
static NNS_G2D_INLINE void IncreaseAutoZOffset_()
{
    if( bAutoZOffsetAdd_ )
    {
        zOffset_ += zOffsetStep_;
    }
}

//------------------------------------------------------------------------------
// fx32 �^ �� Z �l ���擾����A����Z�I�t�Z�b�g���l�����Ă���
static NNS_G2D_INLINE fx32 GetFx32DepthValue_( int z )
{
    if( bAutoZOffsetAdd_ )
    {
        return ( z << FX32_SHIFT ) + zOffset_;
    }else{
        return ( z << FX32_SHIFT );
    }
}

//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�UV�l�v�Z���s���܂�
static NNS_G2D_INLINE void DoFlip_
(
    BOOL bFlipH, BOOL bFlipV,
    fx32*   pRetU0,  fx32* pRetU1,
    fx32*   pRetV0,  fx32* pRetV1
)
{
    NNS_G2D_NULL_ASSERT( pRetU0 );
    NNS_G2D_NULL_ASSERT( pRetU1 );
    NNS_G2D_NULL_ASSERT( pRetV0 );
    NNS_G2D_NULL_ASSERT( pRetV1 );
    
    // UV�t���b�v����
    {
        fx32 temp;
        if( bFlipH )
        {
            temp = *pRetU0;
            *pRetU0 = *pRetU1;
            *pRetU1 = temp;
        }
        
        if( bFlipV )
        {
            temp = *pRetV0;
            *pRetV0 = *pRetV1;
            *pRetV1 = temp;
        }
       
        // UV �l�␳�֐����ݒ肳��Ă���Ȃ�ΌĂяo���܂��B
        if( s_pUVFlipCorrectFunc )
        {
            (*s_pUVFlipCorrectFunc)( pRetU0, pRetV0, pRetU1, pRetV1, bFlipH, bFlipV );
        }
    }
}

//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�UV�l�v�Z���s���܂�
static NNS_G2D_INLINE void CalcUVFor3DDirect2DMap_
( 
    const NNSG2dImageAttr*          pTexImageAttr, 
    u16                             charName,
    fx32*   pRetU0,  
    fx32*   pRetV0
)
{
    NNS_G2D_NULL_ASSERT( pRetU0 );
    NNS_G2D_NULL_ASSERT( pRetV0 );
    
    GX_OBJVRAMMODE_CHAR_ASSERT( pTexImageAttr->mappingType );
    
    //
    // ���ӁF2D �O���t�B�b�N�G���W���̎d�l�ł��B
    // ���� �� 8*8 �e�N�Z���̃L�����N�^���� charName/2 �ł��B
    //
    if( pTexImageAttr->fmt == GX_TEXFMT_PLTT256 )
    {
        // charName /= 2;
        charName >>= 1;
    }
    
    {
        // GX_OBJVRAMMODE_CHAR_2D�̏ꍇ
        {
            const u32 numCharPerOneLine = GetNumTexChar_( pTexImageAttr->sizeS );
            
            //U = ((charName) % numCharPerOneLine ) * 8;
        	*pRetU0 = (fx32)( (( charName & ( numCharPerOneLine - 1 ) ) << 3) << FX32_SHIFT ); 
            
            //
            // ���ӁF�������̂��߁B�iGXTexSizeS�̒l�Ɉˑ����Ă��܂��B�j
            //
            //V = ((charName) / numCharPerOneLine ) * 8;
            *pRetV0 = (( charName >> pTexImageAttr->sizeS ) << 3) << FX32_SHIFT;
        }
    }
}

//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�UV�l�v�Z���s���܂�
static NNS_G2D_INLINE void CalcUVFor3DDirect1DMap_
( 
    fx32*   pRetU0,  
    fx32*   pRetV0
)
{
    NNS_G2D_NULL_ASSERT( pRetU0 );
    NNS_G2D_NULL_ASSERT( pRetV0 );
    
    
    {
        // GX_OBJVRAMMODE_CHAR_1D_32K:
        // GX_OBJVRAMMODE_CHAR_1D_64K:
        // GX_OBJVRAMMODE_CHAR_1D_128K:
        // GX_OBJVRAMMODE_CHAR_1D_256K:�̏ꍇ
        //
        // baseOffset �� �Ή�����
        // NNS_G2dDrawOneOam3DDirect() �� �Q�Ƃ��ꂽ��
        //
        *pRetU0 = 0; 
        *pRetV0 = 0;
    }
}


                                          


//------------------------------------------------------------------------------
static NNS_G2D_INLINE GXTexSizeS GetTexS_( GXOamShape shape )
{
    GX_OAM_SHAPE_ASSERT( shape );
    {

    
    const u16 shapeBit = (u16)(( shape & GX_OAM_ATTR01_SHAPE_MASK ) >> GX_OAM_ATTR01_SHAPE_SHIFT);
    const u16 sizeBit = (u16)(( shape & GX_OAM_ATTR01_SIZE_MASK ) >> GX_OAM_ATTR01_SIZE_SHIFT);
                              
    return gxTexSizeSTbl[shapeBit][sizeBit];
    }
}

//------------------------------------------------------------------------------
// GXOamShape ���� GXTexSizeT �� �e�[�u���������܂�
static NNS_G2D_INLINE GXTexSizeT GetTexT_( GXOamShape shape )
{
    GX_OAM_SHAPE_ASSERT( shape );
    {

    const u16 shapeBit = (u16)(( shape & GX_OAM_ATTR01_SHAPE_MASK ) >> GX_OAM_ATTR01_SHAPE_SHIFT);
    const u16 sizeBit = (u16)(( shape & GX_OAM_ATTR01_SIZE_MASK ) >> GX_OAM_ATTR01_SIZE_SHIFT);
                              
    return gxTexSizeTTbl[shapeBit][sizeBit];
    }
}    



//------------------------------------------------------------------------------
// �p���b�g�ԍ����������̃I�t�Z�b�g�o�C�g�����v�Z���܂��B
static NNS_G2D_INLINE u32 GetOffsetByteSizeOfPlt_( GXTexFmt pltFmt, BOOL bExtendedPlt, u16 pltNo )
{
    NNS_G2D_ASSERT( pltFmt == GX_TEXFMT_PLTT16 || pltFmt == GX_TEXFMT_PLTT256 );
    
    if( bExtendedPlt )
    {
        // �g���p���b�g 256 * 16 
        NNS_G2D_ASSERT( pltFmt == GX_TEXFMT_PLTT256 );
        
        //return pltNo * (sizeof( u16 ) * 256 );
    	return (u32)( pltNo << NNSI_G2D_SHIFT_SIZEOF_256PLTT );
    }else{
        
        if( pltFmt == GX_TEXFMT_PLTT256 )
        {
            // GX_TEXFMT_PLTT256 �ł� �p���b�g�ԍ��͖�������K�v������܂��B
            //return pltNo * 0;
            return 0;
        }else{
            // return pltNo * (sizeof( u16 ) * 16);
        	return (u32)( pltNo << NNSI_G2D_SHIFT_SIZEOF_16PLTT );
        }
    }
    
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE int GetCharacterNameShiftBit_( GXOBJVRamModeChar objMappingMode )
{
    
    /*
    switch( objMappingMode)
    {
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        return 1;
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        return 2;
    case GX_OBJVRAMMODE_CHAR_1D_256K:
        return 3;
    
    default:
    case GX_OBJVRAMMODE_CHAR_2D:
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        return 0;    
    }
    */
    //
    // ���ӁFenum ��`�Ɉˑ����鏈���ł��B
    //
    /*
    GX_OBJVRAMMODE_CHAR_2D      = (0 << REG_GX_DISPCNT_OBJMAP_SHIFT) | (0 << REG_GX_DISPCNT_EXOBJ_SHIFT),
    GX_OBJVRAMMODE_CHAR_1D_32K  = (1 << REG_GX_DISPCNT_OBJMAP_SHIFT) | (0 << REG_GX_DISPCNT_EXOBJ_SHIFT),
    GX_OBJVRAMMODE_CHAR_1D_64K  = (1 << REG_GX_DISPCNT_OBJMAP_SHIFT) | (1 << REG_GX_DISPCNT_EXOBJ_SHIFT),
    GX_OBJVRAMMODE_CHAR_1D_128K = (1 << REG_GX_DISPCNT_OBJMAP_SHIFT) | (2 << REG_GX_DISPCNT_EXOBJ_SHIFT),
    GX_OBJVRAMMODE_CHAR_1D_256K = (1 << REG_GX_DISPCNT_OBJMAP_SHIFT) | (3 << REG_GX_DISPCNT_EXOBJ_SHIFT)
    */
    return (objMappingMode & REG_GX_DISPCNT_EXOBJ_MASK ) >> REG_GX_DISPCNT_EXOBJ_SHIFT;
}


//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�Texture�ݒ���s���܂�
static NNS_G2D_INLINE void SetTextureParamsFor3DDirect1DMap_
(
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    GXOamShape                      shape,
    u16                             charName
)
{
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    
    // 
    // �e�N�X�`���T�C�Y��ݒ肷��
    // 
    {
        const u16 shapeBit   = (u16)(( shape & GX_OAM_ATTR01_SHAPE_MASK ) >> GX_OAM_ATTR01_SHAPE_SHIFT);
        const u16 sizeBit    = (u16)(( shape & GX_OAM_ATTR01_SIZE_MASK ) >> GX_OAM_ATTR01_SIZE_SHIFT);
        //
        // �t�H�[�}�b�g�ɉ����āA�{���̃L�����N�^�u���b�N�����V�t�g�v�Z���ċ��߂�
        //
        // 5 �� * 32(8*8�e�N�Z���̃o�C�g�T�C�Y) �̈�
        // �{���� pTexImageAttr->fmt == GX_TEXFMT_PLTT256 �̏ꍇ��
        //     ( (charName / 2) * ( 32 * 2 ) ) << shiftBit �Ƃ��ׂ��ł����A
        //     ����́A�܂Ƃ߂�� ���� charName * 32 << shiftBit �ƂȂ�܂��B
        //
        const int shiftBit   = ( 5 + GetCharacterNameShiftBit_( pTexImageAttr->mappingType ) );
                          
        NNS_G2D_ASSERT( pTexImageAttr->mappingType != GX_OBJVRAMMODE_CHAR_2D );

        
        // 1D �}�b�s���O�Ȃ�A�e�N�X�`���T�C�Y == OBJ �T�C�Y�ł��� 
        //
        // G3_TexImageParam ���g�p����� �R���p�C����
        // SetTextureParamsFor3DDirect_�̃C�����C���W�J�Ɏ��s����̂�...        
        reg_G3_TEXIMAGE_PARAM 
           = GX_PACK_TEXIMAGE_PARAM( pTexImageAttr->fmt,   
                                     GX_TEXGEN_TEXCOORD, 
                                     gxTexSizeSTbl[shapeBit][sizeBit],         
                                     gxTexSizeTTbl[shapeBit][sizeBit],         
                                     GX_TEXREPEAT_NONE,  
                                     GX_TEXFLIP_NONE,    
                                     pTexImageAttr->plttUse, 
                                     texBaseAddr + ( charName << shiftBit ) ); 
        
    }
}

//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�Texture�ݒ���s���܂�
static NNS_G2D_INLINE void SetTextureParamsFor3DDirect2DMap_
(
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr
)
{
    NNS_G2D_NULL_ASSERT( pTexImageAttr );        
    {        
        // 2D �}�b�s���O�Ȃ�A�e�N�X�`���T�C�Y �� �L�����N�^�f�[�^�̃T�C�Y
        reg_G3_TEXIMAGE_PARAM 
           = GX_PACK_TEXIMAGE_PARAM( pTexImageAttr->fmt,   
                                     GX_TEXGEN_TEXCOORD, 
                                     pTexImageAttr->sizeS,         
                                     pTexImageAttr->sizeT,         
                                     GX_TEXREPEAT_NONE,  
                                     GX_TEXFLIP_NONE,    
                                     pTexImageAttr->plttUse, 
                                     texBaseAddr );
    }
}


//------------------------------------------------------------------------------
// NNS_G2dDrawOneOam3DDirect() ���ł�Texture�ݒ���s���܂�
static NNS_G2D_INLINE void SetPaletteParamsFor3DDirect_
(
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             pltBaseAddr,
    const GXOamAttr*                pOam
)
{
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    NNS_G2D_NULL_ASSERT( pOam );
    
    //
    // �p���b�g�ݒ���s��
    //
    {
        // const GXTexFmt pltFmt = ( pOam->colorMode ) ? GX_TEXFMT_PLTT256 : GX_TEXFMT_PLTT16;
        const static GXTexFmt pltFmtTbl[] = 
        {
           GX_TEXFMT_PLTT16,
           GX_TEXFMT_PLTT256
        };
        const GXTexFmt pltFmt = pltFmtTbl[pOam->colorMode];
        
        const u32 pltOffs 
               = GetOffsetByteSizeOfPlt_( pltFmt, 
                                          pTexImageAttr->bExtendedPlt, 
                                          (u16)pOam->cParam );
                                        
        G3_TexPlttBase( pltBaseAddr + pltOffs, pltFmt );
    }    
}


//------------------------------------------------------------------------------
// �X�v���C�g�̕��s�ړ��l��ݒ肵�܂�
// �֐����ŁA�{�p�A�t�B��OBJ���ǂ����𔻒肵�A���̌��ʂɂ���ĕ`��ʒu��
// �ω������Ă��܂��B
static NNS_G2D_INLINE void SetQuadTranslation_
( 
    const GXOamAttr* pOam, 
    const int posX, 
    const int posY, 
    const int posZ 
)
{
    #pragma inline_max_size(20000)
    // �{�p�A�t�B��OBJ�����肵�܂�
    if( G2_GetOBJEffect( pOam ) == GX_OAM_EFFECT_AFFINE_DOUBLE )
    {        
        const GXOamShape     oamShape = NNS_G2dGetOAMSize( pOam );
        const int           halfW = NNS_G2dGetOamSizeX( &oamShape ) >> 1; // - 1 �� / 2 �̈�
        const int           halfH = NNS_G2dGetOamSizeY( &oamShape ) >> 1; 
           
        G3_Translate
        ( 
           (posX + halfW )<< FX32_SHIFT, 
           (posY + halfH )<< FX32_SHIFT, 
           GetFx32DepthValue_( posZ ) 
        );    
    }else{
        G3_Translate
        ( 
           posX << FX32_SHIFT, 
           posY << FX32_SHIFT, 
           GetFx32DepthValue_( posZ ) 
        );    
    }
}

//------------------------------------------------------------------------------
// �X�v���C�g�̃p�����[�^���v�Z����B
static void CalcSpriteParams_
( 
    const GXOamAttr*                pOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr,
    SpriteParams                    *pResult
)
{
    
    const GXOamShape  shapeOam = NNS_G2dGetOAMSize( pOam );
    const u16 charName = (u16)pOam->charNo;
    
    pResult->sx       = NNS_G2dGetOamSizeX( &shapeOam );
    pResult->sy       = NNS_G2dGetOamSizeY( &shapeOam );
   
    if( pTexImageAttr->mappingType == GX_OBJVRAMMODE_CHAR_2D )
    {
        // �e�N�X�`��
        SetTextureParamsFor3DDirect2DMap_( pTexImageAttr, texBaseAddr );
        // UV 0 �l���Z�o����
        CalcUVFor3DDirect2DMap_( pTexImageAttr, charName, &pResult->u0, &pResult->v0 );          
    }else{
        // �e�N�X�`��
        SetTextureParamsFor3DDirect1DMap_( pTexImageAttr, texBaseAddr, shapeOam, charName );
        // UV 0 �l���Z�o����
        CalcUVFor3DDirect1DMap_( &pResult->u0, &pResult->v0 );
    }
       
    // UV 1 �l���Z�o����
    pResult->u1 = pResult->u0 + ( pResult->sx << FX32_SHIFT );
    pResult->v1 = pResult->v0 + ( pResult->sy << FX32_SHIFT );
       
    DoFlip_( NNSi_G2dGetOamFlipH( pOam ), 
             NNSi_G2dGetOamFlipV( pOam ), 
             &pResult->u0, &pResult->u1 , 
             &pResult->v0, &pResult->v1 );
   
    // �p���b�g
    SetPaletteParamsFor3DDirect_( pTexImageAttr, pltBaseAddr, pOam );
}

//------------------------------------------------------------------------------
// ����Z�l�I�t�Z�b�g�@�\��L���ɂ���t���O��ݒ肵�܂��B
void NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( BOOL flag )
{
    bAutoZOffsetAdd_ = flag;
}

//------------------------------------------------------------------------------
fx32 NNSi_G2dGetOamSoftEmuAutoZOffset( void )
{
    return zOffset_;
}

//------------------------------------------------------------------------------
void NNSi_G2dResetOamSoftEmuAutoZOffset( void )
{
    zOffset_ = 0;
}

//------------------------------------------------------------------------------
void NNSi_G2dSetOamSoftEmuAutoZOffsetStep( fx32 step )
{
    NNS_G2D_WARNING( step <=  0, "AutoZOffsetStep should be smaller than zero." );
    zOffsetStep_ = step;
}

//------------------------------------------------------------------------------
fx32 NNSi_G2dGetOamSoftEmuAutoZOffsetStep( void )
{
    return zOffsetStep_;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawOneOam3DDirectFast

  Description:  Oam�A�g���r���[�g�̓��e�𒼐� 3D Graphics Engine �� �g�p���ĕ`�悵�܂��B
                
  Arguments:    pOam          :     [IN]  OAM�iGXOamAttr�j
                pTexImageAttr :     [IN]  �e�N�X�`������ 
                texBaseAddr   :     [IN]  �e�N�X�`���x�[�X�A�h���X 
                pltBaseAddr   :     [IN]  �p���b�g�x�[�X�A�h���X 

  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawOneOam3DDirectFast
( 
    const GXOamAttr*                pOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr
)
{
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    
    {
        SpriteParams             spriteParams;
        const s16 posX = NNSi_G2dRepeatXinScreenArea( NNSi_G2dGetOamX( pOam ) );
        const s16 posY = NNSi_G2dRepeatYinScreenArea( NNSi_G2dGetOamY( pOam ) );
        
        CalcSpriteParams_( pOam, pTexImageAttr, texBaseAddr, pltBaseAddr, &spriteParams );
        
        NNSi_G2dDrawSpriteFast   ( posX, posY, GetFx32DepthValue_( -1 ), 
                               spriteParams.sx, spriteParams.sy,       
                               spriteParams.u0, spriteParams.v0, 
                               spriteParams.u1, spriteParams.v1 );
        //
        // ����Z�I�t�Z�b�g�l�̍X�V
        //
        IncreaseAutoZOffset_();   
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawOneOam3DDirectWithPosFast

  Description:  Oam�A�g���r���[�g�̓��e���ʒu���w�肵�� ���� 3D Graphics Engine �� �g�p���ĕ`�悵�܂��B
                
  Arguments:    
                posX          :        [IN]  �ʒu X
                posY          :        [IN]  �ʒu Y
                posZ          :        [IN]  �ʒu Z
                pOam          :        [IN]  OAM�iGXOamAttr�j
                pTexImageAttr :        [IN]  �e�N�X�`������
                texBaseAddr   :        [IN]  VRAM �x�[�X�A�h���X
                pltBaseAddr   :        [IN]  �p���b�g �x�[�X�A�h���X
                
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dDrawOneOam3DDirectWithPosFast
( 
    s16                             posX,
    s16                             posY,
    s16                             posZ,
    const GXOamAttr*                pOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr
)
{
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    
    {
        SpriteParams             spriteParams;
        CalcSpriteParams_( pOam, pTexImageAttr, texBaseAddr, pltBaseAddr, &spriteParams );
        //
        // �`��
        //
        //
        // �����_���`��̍œK���̂��߁A�\�t�g�E�F�A�X�v���C�g���W���[���̏�����W�J����B 
        //
        // T
        SetQuadTranslation_( pOam, posX, posY, posZ );
        // S
        G3_Scale( spriteParams.sx << FX32_SHIFT, spriteParams.sy << FX32_SHIFT, FX32_ONE );        
        {
           const fx32      size = FX32_ONE;
           //
           // Draw a quad polygon.
           //
           G3_Begin( GX_BEGIN_QUADS );
                
               G3_TexCoord( spriteParams.u0, spriteParams.v1 );
               G3_Vtx10( 0 ,size, 0 ); 
                 
               G3_TexCoord( spriteParams.u1, spriteParams.v1 );
               G3_Vtx10( size, size, 0 ); 
                
               G3_TexCoord( spriteParams.u1, spriteParams.v0 );
               G3_Vtx10( size, 0 , 0 ); 
               
               G3_TexCoord( spriteParams.u0, spriteParams.v0 );
               G3_Vtx10( 0 , 0, 0 );
               
                
           G3_End( );
        }
        //
        // ����Z�I�t�Z�b�g�l�̍X�V
        //
        IncreaseAutoZOffset_();   
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawOneOam3DDirectWithPosAffine

  Description:  Oam�A�g���r���[�g�̓��e���ʒu���w�肵�� ���� 3D Graphics Engine �� �g�p���ĕ`�悵�܂��B
                
  Arguments:    
                posX          :        [IN]  �ʒu X
                posY          :        [IN]  �ʒu Y
                posZ          :        [IN]  �ʒu Z
                pOam          :        [IN]  OAM�iGXOamAttr�j
                pTexImageAttr :        [IN]  �e�N�X�`������
                texBaseAddr   :        [IN]  VRAM �x�[�X�A�h���X
                pltBaseAddr   :        [IN]  �p���b�g �x�[�X�A�h���X
                pMtx          :        [IN]  �A�t�B���ϊ��s��
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dDrawOneOam3DDirectWithPosAffineFast
( 
    s16                             posX,
    s16                             posY,
    s16                             posZ,
    const GXOamAttr*                pOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr,
    const MtxFx22*                  pMtx
)
{
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    
    {
        SpriteParams             spriteParams;
        CalcSpriteParams_( pOam, pTexImageAttr, texBaseAddr, pltBaseAddr, &spriteParams );
        
        //
        // �`��
        //
        if( G2_GetOBJEffect( pOam ) == GX_OAM_EFFECT_AFFINE )
        {
            NNSi_G2dDrawSpriteWithMtxFast               
                ( posX, posY, GetFx32DepthValue_( posZ ), 
                  spriteParams.sx, spriteParams.sy, pMtx, 
                  spriteParams.u0, spriteParams.v0, 
                  spriteParams.u1, spriteParams.v1 );
        }else{
            NNSi_G2dDrawSpriteWithMtxDoubleAffineFast   
                ( posX, posY, GetFx32DepthValue_( posZ ), 
                spriteParams.sx, spriteParams.sy, pMtx, 
                spriteParams.u0, spriteParams.v0, 
                spriteParams.u1, spriteParams.v1 );
        }
        
        //
        // ����Z�I�t�Z�b�g�l�̍X�V
        //
        IncreaseAutoZOffset_();   
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetOamSoftEmuSpriteParamCache

  Description:  �\�t�g�E�G�A�X�v���C�g�`��Ɏg�p����A
                �p�����[�^�̃L���b�V����ݒ肵�܂��B
                
                �L���b�V���𗘗p�����`���
                NNS_G2dDrawOneOam3DDirectUsingParamCacheFast()�֐��ōs���܂��B
                
                �p�����[�^�Ƃ�UV�p�����[�^�̂��ƂȂ̂ŁA����̃e�N�X�`�����Q�Ƃ���
                �X�v���C�g���ʂɏ����ۂɁAUV�l�̌v�Z�A�ݒ����x�ɂ܂Ƃ� 
                �����悭�������s�����Ƃ��\�ƂȂ�܂��B
                
                
                NNS_G2dDrawOneOam3DDirectUsingParamCache()�̕`��
                  
                  ���� = UV�p�����[�^�ݒ� x 1 + �`�� x N
                  
                �ʏ�̕`��
                  ���� = ( UV�p�����[�^�ݒ� + �`�� ) x N
                  
                
                
  Arguments:    
                pOam          :        [IN]  OBJ�A�g���r���[�g
                pTexImageAttr :        [IN]  �e�N�X�`������
                texBaseAddr   :        [IN]  VRAM �x�[�X�A�h���X
                pltBaseAddr   :        [IN]  �p���b�g �x�[�X�A�h���X
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dSetOamSoftEmuSpriteParamCache
( 
    const GXOamAttr*                pOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr
)
{
    {
        SpriteParams             spriteParams;
        CalcSpriteParams_( pOam, pTexImageAttr, texBaseAddr, pltBaseAddr, &spriteParams );
    
        //
        // �L���b�V���Ɋi�[
        //
        softwareSpreiteParamCache_.u0 = spriteParams.u0;
        softwareSpreiteParamCache_.v0 = spriteParams.v0;
        softwareSpreiteParamCache_.u1 = spriteParams.u1;
        softwareSpreiteParamCache_.v1 = spriteParams.v1;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawOneOam3DDirectUsingParamCacheFast

  Description:  �p�����[�^�̃L���b�V�����g�p����OAM���X�v���C�g�`�悵�܂��B
                ���s�O�ɃL���b�V���p�����[�^�̐ݒ���s���Ă��������B
                �L���b�V���p�����[�^�̐ݒ��
                NNS_G2dSetOamSoftEmuSpriteParamCache()�֐��ōs���܂��B
                
                �֐��Ăяo���̑O��ŁA3D �O���t�B�b�N�X�G���W���̃J�����g�s��
                �͕ۑ�����܂���B
                
                
                �p�����[�^�Ƃ�UV�p�����[�^�̂��ƂȂ̂ŁA����̃e�N�X�`�����Q�Ƃ���
                �X�v���C�g���ʂɏ����ۂɁAUV�l�̌v�Z�A�ݒ����x�ɂ܂Ƃ� 
                �����悭�������s�����Ƃ��\�ƂȂ�܂��B
                
                
                �N���[���`�掞�̃t���b�v�t���O�ύX�̓p�t�H�[�}���X�ւ̉e�����傫�����߁A
                �T�|�[�g���Ȃ����Ƃɂ��܂����B
                ���������āA�{�֐��ł́AOAM�̃t���b�v�t���O�����������_�ɂ����ӂ��������B
                (UV�p�����[�^��NNS_G2dSetOamSoftEmuSpriteParamCache()�֐����s����
                ���肳��Ă��܂��܂��B)
                �t���b�v�����X�v���C�g��`�悷��ꍇ�A
                NNS_G2dSetOamSoftEmuSpriteParamCache()�̈����Ƃ��ăt���b�v�ݒ肵��
                OAM�iGXOamAttr�j��n���A�L���b�V���p�����[�^���X�V����K�v������܂��B
                
                
                NNS_G2dDrawOneOam3DDirectUsingParamCache()�̕`��
                  
                  ���� = UV�p�����[�^�ݒ� x 1 + �`�� x N
                  
                �ʏ�̕`��
                  ���� = ( UV�p�����[�^�ݒ� + �`�� ) x N
                
  Arguments:    
                posX          :        [IN]  �ʒu X
                posY          :        [IN]  �ʒu Y
                posZ          :        [IN]  �ʒu Z
                pOam          :        [IN]  OAM�iGXOamAttr�j
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dDrawOneOam3DDirectUsingParamCacheFast
( 
    s16                             posX,
    s16                             posY,
    s16                             posZ,
    const GXOamAttr*                pOam
)
{
    {
        const GXOamShape  shapeOam = NNS_G2dGetOAMSize( pOam );
        const int sx       = NNS_G2dGetOamSizeX( &shapeOam );
        const int sy       = NNS_G2dGetOamSizeY( &shapeOam );        
        
        //
        // �p�����[�^�L���b�V���̒l���g�p����B
        //
        const fx32   u0 = softwareSpreiteParamCache_.u0, 
                     u1 = softwareSpreiteParamCache_.u1, 
                     v0 = softwareSpreiteParamCache_.v0, 
                     v1 = softwareSpreiteParamCache_.v1;
        //
        // �`��
        //
        //
        // �`��̍œK���̂��߁A�\�t�g�E�F�A�X�v���C�g���W���[���̏�����W�J����B 
        
        //
        // T
        //G3_Translate( posX << FX32_SHIFT, posY << FX32_SHIFT, GetFx32DepthValue_( posZ ) );    
        SetQuadTranslation_( pOam, posX, posY, posZ );
        
        // S
        G3_Scale( sx << FX32_SHIFT, sy << FX32_SHIFT, FX32_ONE );        
        {
           const fx32      size = FX32_ONE;
           //
           // Draw a quad polygon.
           //
           G3_Begin( GX_BEGIN_QUADS );
               
               G3_TexCoord( u0, v1 );
               G3_Vtx10( 0 ,size, 0 ); 
               
               
               G3_TexCoord( u1, v1 );
               G3_Vtx10( size, size, 0 ); 
               
               G3_TexCoord( u1, v0 );
               G3_Vtx10( size, 0 , 0 ); 
               
               G3_TexCoord( u0, v0 );
               G3_Vtx10( 0 , 0, 0 );
               
                    
           G3_End( );
        }
        //
        // ����Z�I�t�Z�b�g�l�̍X�V
        //
        IncreaseAutoZOffset_();
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetOamSoftEmuUVFlipCorrectFunc

  Description:  Oam�\�t�g�E�G�A�X�v���C�g�G�~�����[�V���� 
                �Ŏg�p����� UV�l�␳�֐��|�C���^��ݒ肵�܂��B
                
  Arguments:    pFunc          :        [IN]  UV�l�␳�֐��|�C���^
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dSetOamSoftEmuUVFlipCorrectFunc( NNS_G2dOamSoftEmuUVFlipCorrectFunc pFunc )
{
    NNS_G2D_NULL_ASSERT( pFunc );
    s_pUVFlipCorrectFunc = pFunc;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetOamSoftEmuUVFlipCorrectFunc

  Description:  Oam�\�t�g�E�G�A�X�v���C�g�G�~�����[�V���� 
                �Ŏg�p����� UV�l�␳�֐��|�C���^��ݒ肵�܂��B
                
  Arguments:    �Ȃ�
                       
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void    NNS_G2dResetOamSoftEmuUVFlipCorrectFunc()
{
    s_pUVFlipCorrectFunc = NULL;
}


