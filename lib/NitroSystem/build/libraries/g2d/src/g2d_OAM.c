/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_OAM.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.44 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_OAM.h>
#include <nnsys/g2d/g2d_Softsprite.h>

#include <nnsys/g2d/g2d_OamSoftwareSpriteDraw.h>

#include <nnsys/g2d/fmt/g2d_Cell_data.h>

#include "g2d_Internal.h"
#include "g2di_Dma.h"


//
// g2d_Oam_data.h �Ő錾����Ă���A�e�[�u���̎��̂��`���܂��B
// �ꏊ�͂������K���ł͂Ȃ���������܂���B
// ���Â�ׂ̃t�@�C���ւƈړ������\��������܂��B
//
NNS_G2D_DEFINE_NNSI_OBJSIZEWTBL;
NNS_G2D_DEFINE_NNSI_OBJSIZEHTBL;



//------------------------------------------------------------------------------
#define OAM_SETTING_INVISIBLE   192     // OAM Attribute �s���ݒ�


#define NUM_HW_OAM_ATTR         128     // OAM Attribute ��
#define NUM_HW_OAM_AFFINE       32      // Affine paramater ��
#define NUM_OAM_TYPES           3       // OAM �̎��

// OAM �̈悪�N�ɂ��g�p����Ă��Ȃ�(�g�p�\��e�[�u���̋K��l)
#define OAM_NOT_USED            0xFFFF  

#define GX_AFFINE_SIZE          sizeof( GXOamAffine )
#define GX_OAMATTR_SIZE         sizeof( GXOamAttr )
#define OAMATTR_SIZE            sizeof( u16 ) * 3

//
// OBJ ���[�h���Ɏg�p�����֐�
//
typedef void (OBJLoadFunction)( const void *pSrc, u32 offset, u32 szByte );






//------------------------------------------------------------------------------
// OAM�}�l�[�W�� �� OAM �ւ̑���̃L���b�V��
// 
typedef struct OamAttributeCache
{
    u16             reservationTable[NUM_HW_OAM_ATTR]; // �g�p�\��e�[�u��
    GXOamAttr       oamBuffer       [NUM_HW_OAM_ATTR]; // OAM Attribute �̃o�b�t�@
    
}OamAttributeCache;

//------------------------------------------------------------------------------
// Affine paramaters �ւ̑���L���V��
// OamAttributeCache �� oamBuffer �� ���L���܂��B
// 
typedef struct OamAffineCache
{
    u16             reservationTable[NUM_HW_OAM_AFFINE]; // AffineParamater �g�p�\��e�[�u��
    
}OamAffineCache;

//------------------------------------------------------------------------------
// OAM�}�l�[�W�� �� OAM �ւ̑���̃L���b�V��
// 
// �}�l�[�W�� �� OAM �ɑ΂��鑀��͖{�\���ɂ���Ĉ�U�L���b�V������܂��B
// ���[�U �� NNS_G2dApplyToHWXXX() �̌Ăяo���ɂ���āA�L���b�V���̓��e��
// HW �ɔ��f����K�v������܂��B
// 
typedef struct OamCache
{
    OamAttributeCache           attributeCache; // OamAttr 
    OamAffineCache              affineCache;    // Affine params
    
}OamCache;




//------------------------------------------------------------------------------
// �B��� OamCache ���� 
// NNS_G2dInitOamManagerModule() �ɂ���ď���������܂��B
// 
static OamCache                        oamCache_[NUM_OAM_TYPES];
static u16                             numRegisterdInstance_ = 0x0;











//------------------------------------------------------------------------------
// �Ǘ��̈悪�L�������肵�܂�
static NNS_G2D_INLINE BOOL IsManageAreaValid_( const NNSG2dOAMManageArea* pArea )
{
    NNS_G2D_NULL_ASSERT( pArea );
    
    return (BOOL)( ( pArea->currentIdx <= pArea->toIdx + 1 ) &&
                   ( pArea->fromIdx <= pArea->toIdx ) );
}

//------------------------------------------------------------------------------
// �w��^�C�v�� OamCache �ւ̃|�C���^���擾���܂��B
// 
static NNS_G2D_INLINE OamCache* GetOamCachePtr_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    return &oamCache_[type];
}



//------------------------------------------------------------------------------
// �w��^�C�v�� �Ǘ��̈�\��e�[�u���ւ̃|�C���^���擾���܂��B( OamAttr �p)
static NNS_G2D_INLINE u16* GetOamReservationTblPtr_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    {        
        OamCache* pOamCache = GetOamCachePtr_( type );
        NNS_G2D_NULL_ASSERT( pOamCache );
    
        return pOamCache->attributeCache.reservationTable;
    }
}
//------------------------------------------------------------------------------
// �w��^�C�v�� �Ǘ��̈�\��e�[�u���ւ̃|�C���^���擾���܂��B( affine Paramater �p)
static NNS_G2D_INLINE u16* GetAffineReservationTblPtr_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    {
        OamCache* pOamCache = GetOamCachePtr_( type );
        NNS_G2D_NULL_ASSERT( pOamCache );
        
        return pOamCache->affineCache.reservationTable;
    }
}

//------------------------------------------------------------------------------
// OamAttr �o�b�t�@�� index �Ԗ� �ւ̃|�C���^ ���擾���܂��B
static NNS_G2D_INLINE GXOamAttr* GetOamBufferPtr_( NNSG2dOamType type, u16 index )
{
    ASSERT_OAMTYPE_VALID( type );
    {
        OamCache* pOamCache = GetOamCachePtr_( type );
        NNS_G2D_NULL_ASSERT( pOamCache );
        
        return pOamCache->attributeCache.oamBuffer + index;
    }
}

//------------------------------------------------------------------------------
// Oam Affine paramater �o�b�t�@�� index �Ԗ� �ւ̃|�C���^ ���擾���܂��B
// ������ GetOamBufferPtr_() �� �Ăяo���Ă��܂�.
static NNS_G2D_INLINE GXOamAffine* GetAffineBufferPtr_( NNSG2dOamType type, u16 index )
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_MINMAX_ASSERT   ( index, 0 , NUM_HW_OAM_AFFINE );
    
    {
        GXOamAffine* pAff =  (GXOamAffine*)GetOamBufferPtr_( type, 0 );
        NNS_G2D_NULL_ASSERT( pAff );
        
        return pAff + index;
    }
}

//------------------------------------------------------------------------------
// affine paramater �� �擾���܂�
static NNS_G2D_INLINE void GetAffineParams_( NNSG2dOamType type, u16 idx, MtxFx22* pMtx )
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_MINMAX_ASSERT( idx, 0, NUM_HW_OAM_AFFINE );
    NNS_G2D_NULL_ASSERT( pMtx );
    
    {    
        GXOamAffine* pAff = GetAffineBufferPtr_( type, idx );
        NNS_G2D_NULL_ASSERT( pAff );
        
        // Affine transform parmeters(PA,PB,PC,PD) are in s7.8 format.
        // Since fx32 is in s19.12 format, shift them rightward by 4 bits.
        pMtx->_00 = (s16)(pAff->PA << 4);
        pMtx->_01 = (s16)(pAff->PB << 4);
        pMtx->_10 = (s16)(pAff->PC << 4);
        pMtx->_11 = (s16)(pAff->PD << 4);
    }
}

//------------------------------------------------------------------------------
// Oam�Ǘ��̈悪���̎��̂ɂ���Ďg�p����Ă��Ȃ����������܂��B
static NNS_G2D_INLINE BOOL IsOamNotUsed_( u16* pResvTblHead, u16 from, u16 to )
{
    const u16*    pCursor   = pResvTblHead + from;
    const u16*    pEnd      = pResvTblHead + to;
    
    while( pCursor <= pEnd )
    {
        if( *pCursor != OAM_NOT_USED )
        {
            return FALSE;
        }
        pCursor++;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
// �V����GUID���擾���܂�.
static NNS_G2D_INLINE u16 GetNewGUID_()
{
    return numRegisterdInstance_++;
}

//------------------------------------------------------------------------------
// �̈�� ownerGUID �̊Ǘ�����̈�Ƃ��āA�\�񂵂܂�.
// 
// �ΏۂƂȂ�̈� �� ���̃}�l�[�W���ɂ���Ďg�p����Ă��Ȃ��K�v������܂��B
// IsOamNotUsed_() ���g�p���Ē��ׂĂ�������.
// �����𖞂����Ȃ��ꍇ �A�T�[�g�Ɏ��s���܂�.
// 
static NNS_G2D_INLINE void DoReserveArea_( u16* pResvTblHead, u16 from, u16 to, u16 ownerGUID )
{
    NNS_G2D_NULL_ASSERT( pResvTblHead );
    NNS_G2D_ASSERT( from <= to );// MUST BE !    
    // Check if they aren't going to Reserve others area.
    NNS_G2D_ASSERT( IsOamNotUsed_( pResvTblHead, from, to ) );
    
    
    
    
    NNSI_G2D_DEBUGMSG0( "Oam Reservation occur... from %d to %d by GUID %d \n", 
                         from, 
                         to, 
                         ownerGUID );
               
               
    
    MI_CpuFill16( pResvTblHead + from, ownerGUID, sizeof(u16)*(u32)(to - from + 1) );
}

//------------------------------------------------------------------------------
// �\��̈�������I�ɖ��g�p��ԂւƐݒ肵�܂��B
static NNS_G2D_INLINE void DoRestoreArea_( u16* pResvTblHead, u16 from, u16 to )
{
    NNS_G2D_NULL_ASSERT( pResvTblHead );
    NNS_G2D_ASSERT( from <= to );// MUST BE !    
    
    NNSI_G2D_DEBUGMSG0( "Oam Restoration occur... from %d to %d \n" , from, to );
    
    MI_CpuFill16( pResvTblHead + from, OAM_NOT_USED, sizeof(u16)*(u32)(to - from + 1) );
}
    
//------------------------------------------------------------------------------
// �Ǘ��̈�̗]�����擾���܂��D
static NNS_G2D_INLINE u16 GetCapacity_( const NNSG2dOAMManageArea* pArea )
{
    NNS_G2D_NULL_ASSERT( pArea );
//  NNS_G2D_ASSERT( IsManageAreaValid_( pArea ) );
    
    if( IsManageAreaValid_( pArea ) )
    {
        return (u16)( (int)pArea->toIdx - pArea->currentIdx + 1);
    }else{ 
        return 0;
    }
}

//------------------------------------------------------------------------------
// �Ǘ��̈�̎g�p�����擾���܂��D
static NNS_G2D_INLINE u16 GetNumOamUsed_( const NNSG2dOAMManageArea* pArea )
{
    NNS_G2D_NULL_ASSERT( pArea );
//  NNS_G2D_ASSERT( IsManageAreaValid_( pArea ) );

    if( IsManageAreaValid_( pArea ) )
    {
        return (u16)( (int)pArea->currentIdx - pArea->fromIdx);
    }else{ 
        return 0;
    }
}

//------------------------------------------------------------------------------
// �Ǘ��̈悪�\���ȗ]���������������܂��D
static NNS_G2D_INLINE BOOL HasEnoughCapacity_( const NNSG2dOAMManageArea* pArea, u16 num )
{
    NNS_G2D_NULL_ASSERT( pArea );
    NNS_G2D_ASSERT( num != 0 );
    
    return (BOOL)( GetCapacity_( pArea ) >= num );
}


//------------------------------------------------------------------------------
// Oam �o�b�t�@�̌��݂̈ʒu�ւ̃|�C���^���擾���܂�.
static NNS_G2D_INLINE GXOamAttr* GetOamCurrentPtr_( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERT( IsManageAreaValid_( &pMan->managedAttrArea ) );
    NNS_G2D_ASSERT( pMan->managedAttrArea.toIdx < NUM_HW_OAM_ATTR );
    {            
            
        GXOamAttr* pret = GetOamBufferPtr_( pMan->type, 
                                            pMan->managedAttrArea.currentIdx );
                                            
        NNS_G2D_NULL_ASSERT( pret );
        
        return pret;
    }
}

//------------------------------------------------------------------------------
// Oam �o�b�t�@�̊Ǘ��̈�擪�̈ʒu�ւ̃|�C���^���擾���܂�.
static NNS_G2D_INLINE GXOamAttr* GetOamFromPtr_( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    {
        GXOamAttr* pFrom = GetOamBufferPtr_( pMan->type, 
                                             pMan->managedAttrArea.fromIdx );
        NNS_G2D_NULL_ASSERT( pFrom );
        
        
        return pFrom;
    }
}

//------------------------------------------------------------------------------
// �Ǘ��̈�̃o�C�g�T�C�Y���������܂��D
static NNS_G2D_INLINE u32 GetSizeOfManageArea_( const NNSG2dOAMManageArea* pArea )
{
    NNS_G2D_NULL_ASSERT( pArea );
//  NNS_G2D_ASSERT( IsManageAreaValid_( pArea ) );
    
    if( IsManageAreaValid_( pArea ) )
    {
        return (u32)(GX_OAMATTR_SIZE * (u16)(pArea->toIdx - pArea->fromIdx + 1));
    }else{
        return 0;
    }
        
}

//------------------------------------------------------------------------------
// affine paramater �� �ݒ肵�܂�
static NNS_G2D_INLINE void SetAffineParams_( NNSG2dOamType type, const MtxFx22* mtx, u16 idx )
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_NULL_ASSERT( mtx );
    NNS_G2D_MINMAX_ASSERT( idx, 0, NUM_HW_OAM_AFFINE );
    
    
    {    
        GXOamAffine* pAff = GetAffineBufferPtr_( type, idx );
        NNS_G2D_NULL_ASSERT( pAff );
        
        G2_SetOBJAffine( pAff, mtx );
    }
    
}



//------------------------------------------------------------------------------
// GXOamAttr �� 3DGraphicsEngine_�𗘗p���ĕ`�悵�܂��B
// 
// Affine Transform �𗘗p����ꍇ�AScale �p�����[�^�� �����ɂ��Ē��ӂ��K�v�ł��B
// 
static void DrawBy3DGraphicsEngine_
( 
    const GXOamAttr*                pOam, 
    u16                             numOam, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr
)
{
    MtxFx22                 mtx;
    u16                     affineIdx;
    s16 posX;
    s16 posY;
    s16 posZ;
    
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    NNS_G2D_MINMAX_ASSERT( numOam, 0, NUM_HW_OAM_ATTR );
    
    G3_PushMtx();
    {
        int i = 0; 
        for( i = 0; i < numOam; i++ )
        {
            posX = NNSi_G2dRepeatXinScreenArea( NNSi_G2dGetOamX( &pOam[i] ) );
            posY = NNSi_G2dRepeatYinScreenArea( NNSi_G2dGetOamY( &pOam[i] ) );
            posZ = -1;
            
            // 
            // ����:
            // �\�t�g�E�F�A�X�v���C�g�`��֐��̓J�����g�s��̉e�����󂯂�
            // �܂��A�`���̃J�����g�s��͕ύX�����i�ۑ�����Ȃ��j�B
            // 
            G3_Identity();
            
            if( NNSi_G2dIsRSEnable( &pOam[i] ) )
            {
                //
                // affine �p�����[�^���擾����
                // 
                affineIdx = NNSi_G2dGetAffineIdx( &pOam[i] );
                NNS_G2D_MINMAX_ASSERT( affineIdx, 0, NUM_HW_OAM_AFFINE );
                GetAffineParams_( NNS_G2D_OAMTYPE_SOFTWAREEMULATION, affineIdx, &mtx );
                
                NNS_G2dDrawOneOam3DDirectWithPosAffineFast( posX, posY, posZ, &pOam[i], pTexImageAttr, texBaseAddr, pltBaseAddr, &mtx );
            }else{
                NNS_G2dDrawOneOam3DDirectWithPosFast( posX, posY, posZ, &pOam[i], pTexImageAttr, texBaseAddr, pltBaseAddr );
            }
        }
    }    
    G3_PopMtx( 1 );
}
    


//------------------------------------------------------------------------------
// Oam �o�b�t�@���f�t�H���g�l�ŏ��������܂��B
static NNS_G2D_INLINE void ClearOamByDefaultValue_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    
    MI_CpuFill16( GetOamBufferPtr_( type, 0 ),                        
                  OAM_SETTING_INVISIBLE, 
                  GX_OAMATTR_SIZE * NUM_HW_OAM_ATTR );
}

//------------------------------------------------------------------------------
// Oam �Ǘ��̈�\��e�[�u���𖢎g�p�Ƃ��ď��������܂��B
static NNS_G2D_INLINE void SetOamReservationTblNotUsed_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    
    DoRestoreArea_( GetOamReservationTblPtr_( type ),             
                    0, 
                    NUM_HW_OAM_ATTR - 1 );
}

//------------------------------------------------------------------------------
// Affine paramater �Ǘ��̈�\��e�[�u���𖢎g�p�Ƃ��ď��������܂��B
static NNS_G2D_INLINE void SetAffineReservationTblNotUsed_( NNSG2dOamType type )
{
    ASSERT_OAMTYPE_VALID( type );
    
    DoRestoreArea_( GetAffineReservationTblPtr_( type ),             
                    0, 
                    NUM_HW_OAM_AFFINE - 1);
}
        
//------------------------------------------------------------------------------
// CpuCopy �ɂ�� �n�`�l�A�g���r���[�g�̓]��( for 2D Graphics Engine Main )
// �����ȃu���b�N�ŕ����]�����s���K�v�������������ߍ쐬
static NNS_G2D_INLINE void CpuLoadOAMMain_(
    const void *pSrc,
    u32 offset,
    u32 szByte)
{
    NNS_G2D_NULL_ASSERT(pSrc);
    NNS_G2D_ASSERT(offset + szByte <= HW_OAM_SIZE);
    
    MI_CpuCopy16( pSrc, (void *)(HW_OAM + offset), szByte);
}

//------------------------------------------------------------------------------
// CpuCopy �ɂ�� �n�`�l�A�g���r���[�g�̓]��( for 2D Graphics Engine Sub )
// �����ȃu���b�N�ŕ����]�����s���K�v�������������ߍ쐬
static NNS_G2D_INLINE void CpuLoadOAMSub_(
    const void *pSrc,
    u32 offset,
    u32 szByte)
{
    NNS_G2D_NULL_ASSERT(pSrc);
    NNS_G2D_ASSERT(offset + szByte <= HW_OAM_SIZE);

    MI_CpuCopy16( pSrc, (void *)(HW_DB_OAM + offset), szByte);
}

//------------------------------------------------------------------------------
// Oam �̎�ނ���K�؂� �]���֐��|�C���^���擾���܂�
static NNS_G2D_INLINE OBJLoadFunction* GetOBJLoadFunction_( NNSG2dOamType type )
{
    static OBJLoadFunction*       funcTbl[] =
    {
        CpuLoadOAMMain_,    // for NNS_G2D_OAMTYPE_MAIN
        CpuLoadOAMSub_,     // for NNS_G2D_OAMTYPE_SUB
        NULL,               // for NNS_G2D_OAMTYPE_SOFTWAREEMULATION
        NULL,               // for NNS_G2D_OAMTYPE_INVALID
        NULL,               // for NNS_G2D_OAMTYPE_MAX
    };
    
    return funcTbl[type];
}

//------------------------------------------------------------------------------
// affine �p�����[�^ �� 2D Graphics Engine OAM  �Ƀ��[�h���܂�
// affine �p�����[�^ �݂̂𕡐��̓]���ɕ������ē]�����܂�
// 
static NNS_G2D_INLINE void LoadOneAffine_( const GXOamAffine* pAff, u32 offset, OBJLoadFunction* pOBJLoadFunc )
{
    offset += OAMATTR_SIZE;
    
    (*pOBJLoadFunc)( &pAff->PA, offset + GX_OAMATTR_SIZE*0, sizeof(u16) );
    (*pOBJLoadFunc)( &pAff->PB, offset + GX_OAMATTR_SIZE*1, sizeof(u16) );
    (*pOBJLoadFunc)( &pAff->PC, offset + GX_OAMATTR_SIZE*2, sizeof(u16) );
    (*pOBJLoadFunc)( &pAff->PD, offset + GX_OAMATTR_SIZE*3, sizeof(u16) );
}

//------------------------------------------------------------------------------
// �o�b�t�@�̓��e��DMA���g�p���č����ɃO���t�B�b�N�X�G���W���ɓ]�����܂�
static NNS_G2D_INLINE void LoadOamAndAffineFast_( NNSG2dOamType type, u16 fromIdx, u16 toIdx )
{
    GXOamAttr* pFrom    = GetOamBufferPtr_( type, fromIdx );
    const u16  szByte   = (u16)(GX_OAMATTR_SIZE * (toIdx - fromIdx + 1) );
    u16        offset   = (u16)(GX_OAMATTR_SIZE * fromIdx);
    
    // �L���b�V���t���b�V��
    DC_FlushRange( pFrom, szByte );

    // DMA �]��
    switch( type )
    {
    case NNS_G2D_OAMTYPE_MAIN:
        GX_LoadOAM( pFrom, offset, szByte );
        break;
    case NNS_G2D_OAMTYPE_SUB:
        GXS_LoadOAM( pFrom, offset, szByte );
        break;
    default:
        NNS_G2D_ASSERT( FALSE );
        break;
    }
}

//------------------------------------------------------------------------------
// OAM �A�g���r���[�g�� 2D Graphics Engine OAM �Ƀ��[�h���܂�
static NNS_G2D_INLINE void LoadOam_( NNSG2dOamType type, u16 fromIdx, u16 toIdx )
{
    GXOamAttr* pFrom    = GetOamBufferPtr_( type, fromIdx );
    const u16  numArea  = (u16)(toIdx - fromIdx + 1);
    u16        offset   = (u16)(GX_OAMATTR_SIZE * fromIdx);
    u16         i;        
    OBJLoadFunction* pOBJLoadFunc = GetOBJLoadFunction_( type );
    
    
    for( i = 0; i < numArea; i++ ) 
    {    
        //
        // OAM �A�g���r���[�g�����݂̂�]�����܂�
        // 
        (*pOBJLoadFunc)( pFrom, offset, OAMATTR_SIZE );
        
        offset += GX_OAMATTR_SIZE;
        pFrom++;
    }
}

//------------------------------------------------------------------------------
// affine �p�����[�^ �� 2D Graphics Engine OAM �Ƀ��[�h���܂�
static NNS_G2D_INLINE void LoadAffine_( NNSG2dOamType type, u16 fromIdx, u16 toIdx )
{

    GXOamAffine* pFrom      = GetAffineBufferPtr_( type, fromIdx );
    const u16  numArea      = (u16)(toIdx - fromIdx + 1);
    u16        offset       = (u16)(GX_AFFINE_SIZE * fromIdx);
    u16         i;        
    OBJLoadFunction* pOBJLoadFunc = GetOBJLoadFunction_( type );
    
    for( i = 0; i < numArea; i++ ) 
    {    
        //
        // �A�t�B���p�����[�^�����݂̂�]�����܂�
        // 
        LoadOneAffine_( pFrom, offset, pOBJLoadFunc );
       
        offset += GX_AFFINE_SIZE;
        pFrom++;
    }
}
        
//------------------------------------------------------------------------------
// OAM �A�g���r���[�g�o�b�t�@�������l�Ń��Z�b�g���܂�
static NNS_G2D_INLINE void ResetOam_( NNSG2dOamType type, u16 fromIdx, u16 toIdx )
{
    GXOamAttr* pFrom    = GetOamBufferPtr_( type, fromIdx );
    const u16  numArea  = (u16)(toIdx - fromIdx + 1);
    u16         i;        
    
    for( i = 0; i < numArea; i++ ) 
    {    
        //
        // OAM �A�g���r���[�g�����݂̂��f�t�H���g�l�Ńt�B�����܂�
        //
        //MI_CpuFill16( pFrom, OAM_SETTING_INVISIBLE, sizeof( u16 ) );
        *((u16*)pFrom) = OAM_SETTING_INVISIBLE;
        pFrom++;
    }
}

//------------------------------------------------------------------------------
// affine �p�����[�^ �o�b�t�@�������l�Ń��Z�b�g���܂�
static NNS_G2D_INLINE void ResetAffine_( NNSG2dOamType type, u16 fromIdx, u16 toIdx )
{

    GXOamAffine* pFrom      = GetAffineBufferPtr_( type, fromIdx );
    const u16  numArea      = (u16)(toIdx - fromIdx + 1);
    u16         i;        
    
    for( i = 0; i < numArea; i++ ) 
    {    
        //
        // �A�t�B���p�����[�^�����݂̂��f�t�H���g�l�Ńt�B�����܂�
        // 
        pFrom->PA = OAM_SETTING_INVISIBLE;
        pFrom->PB = OAM_SETTING_INVISIBLE;
        pFrom->PC = OAM_SETTING_INVISIBLE;
        pFrom->PD = OAM_SETTING_INVISIBLE;
        pFrom++;
    }
}

//------------------------------------------------------------------------------
// ���C�u�����������J
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* NNSi_G2dGetOamManagerInternalBufferForDebug( NNSG2dOamType type )
{
    return (void*)GetOamBufferPtr_( type, 0 ); 
}




//------------------------------------------------------------------------------
// �O�����J
//------------------------------------------------------------------------------



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitOamManagerModule

  Description:  OamManager ���W���[�� �� ����������
                ���ׂĂ� OamManager ���W���[�� ���\�b�h�̎��s����ɌĂяo���Ă��������B
                
                ������
                HW Oam �̃������t�B��
                Oam�\��e�[�u���̏��������s���܂�
                
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitOamManagerModule()
{  
    // Oam�o�b�t�@
    ClearOamByDefaultValue_( NNS_G2D_OAMTYPE_MAIN );
    ClearOamByDefaultValue_( NNS_G2D_OAMTYPE_SUB );
    ClearOamByDefaultValue_( NNS_G2D_OAMTYPE_SOFTWAREEMULATION );
    
    NNSI_G2D_DEBUGMSG0("Initialize OamBuffer ... done.\n");
    
    // OamAttr�\��e�[�u��
    SetOamReservationTblNotUsed_( NNS_G2D_OAMTYPE_MAIN );
    SetOamReservationTblNotUsed_( NNS_G2D_OAMTYPE_SUB );
    SetOamReservationTblNotUsed_( NNS_G2D_OAMTYPE_SOFTWAREEMULATION );
    
    NNSI_G2D_DEBUGMSG0("Initialize Oam reservation table ... done.\n");
    
    // affine�p�����[�^�\��e�[�u��
    SetAffineReservationTblNotUsed_( NNS_G2D_OAMTYPE_MAIN );
    SetAffineReservationTblNotUsed_( NNS_G2D_OAMTYPE_SUB );
    SetAffineReservationTblNotUsed_( NNS_G2D_OAMTYPE_SOFTWAREEMULATION );
    
    NNSI_G2D_DEBUGMSG0("Initialize Oam affine reservation table ... done.\n");
        
    
    NNSI_G2D_DEBUGMSG0("Initialize OamManager ... done.\n");
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetNewManagerInstance

  Description:  OamManager �� ���̂�����������
                
                �v���̈悪���łɑ��̎��̂ɂ���Ďg�p����Ă���ꍇ�́A
                �������Ɏ��s��FALSE��Ԃ��܂��B
                
  
  
  Arguments:    pMan:       [OUT] �}�l�[�W������
                from:       [IN]  �g�p����Oam�i�J�n�ԍ��j
                to:         [IN]  �g�p����Oam�i�I���ԍ��j
                type:       [IN]  Oam�̎��
                
                from <= to �𖞂����K�v������܂��B
                
  Returns:      �������̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dGetNewManagerInstance
( 
    NNSG2dOamManagerInstance*   pMan, 
    u16                         from, 
    u16                         to, 
    NNSG2dOamType               type 
)
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_ASSERT( from <= to );// MUST BE !    
    NNS_G2D_ASSERT( to < NUM_HW_OAM_ATTR );
    NNS_G2D_NULL_ASSERT( pMan );
    
    
    {
        u16*        pReserveTbl = GetOamReservationTblPtr_( type );
        NNS_G2D_NULL_ASSERT( pReserveTbl );
        
        //
        // �w�肳�ꂽ�̈悪���g�p�ł����...
        //
        if( IsOamNotUsed_( pReserveTbl, from, to ) )
        {
            //
            // ���̂ւ̃p�����[�^�̑�����s��
            //
            pMan->GUID                          = GetNewGUID_();
            pMan->managedAttrArea.fromIdx       = from;
            pMan->managedAttrArea.toIdx         = to;
            pMan->managedAttrArea.currentIdx    = from;
            //
            // �A�t�B���p�����[�^���Ǘ����Ȃ��l�ɐݒ�
            //
            pMan->managedAffineArea.fromIdx    = NUM_HW_OAM_AFFINE;
            pMan->managedAffineArea.toIdx      = 0;
            
            
            pMan->type                          = type;
            pMan->bFastTransferEnable           = FALSE;
            
            DoReserveArea_( pReserveTbl, from, to, pMan->GUID );
            
            return TRUE;
        }else{
            
            OS_Warning("Failure in NNS_G2dGetNewManagerInstance().\n The manageArea that you specified has been used by someone.");
            return FALSE;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitManagerInstanceAffine

  Description:  OamManager �� ���� Affine �Ǘ����� ������������
                
                �v���̈悪���łɑ��̎��̂ɂ���Ďg�p����Ă���ꍇ�́A
                �������Ɏ��s��FALSE��Ԃ��܂��B
                
  
  
  Arguments:    pMan:       [OUT] �}�l�[�W������
                from:       [IN]  �g�p����Affine�i�J�n�ԍ��j
                to:         [IN]  �g�p����Affine�i�I���ԍ��j
                
                from <= to �𖞂����K�v������܂��B
                
  Returns:      �������̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dInitManagerInstanceAffine
( 
    NNSG2dOamManagerInstance*   pMan, 
    u16                         from, 
    u16                         to 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERT( from <= to );
	NNS_G2D_ASSERT( to < NUM_HW_OAM_AFFINE );
    // �������ς݂��H
    
    {
        u16*        pReserveTbl = GetAffineReservationTblPtr_( pMan->type );
        NNS_G2D_NULL_ASSERT( pReserveTbl );
        
        //
        // �w�肳�ꂽ�̈悪���g�p�ł����...
        //
        if( IsOamNotUsed_( pReserveTbl, from, to ) )
        {
            //
            // ���̂ւ̃p�����[�^�̑�����s��
            //
            pMan->managedAffineArea.fromIdx    = from;
            pMan->managedAffineArea.toIdx      = to;
            pMan->managedAffineArea.currentIdx = from;
            pMan->bFastTransferEnable          = FALSE;
            
            DoReserveArea_( pReserveTbl, from, to, pMan->GUID );
            
            return TRUE;
        }else{
            OS_Warning("Failure in NNS_G2dInitManagerInstanceAffine().\n The manageArea that you specified has been used by someone.");
            return FALSE;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetNewOamManagerInstance

  Description:  OamManager �� ���̂����������܂�
                
                OAM�}�l�[�W���̏�����������V����API�ł��B
                
  Arguments:    pMan:           [OUT] �}�l�[�W������
                fromOBJ:        [IN]  �g�p����Oam�i�J�n�ԍ��j
                numOBJ:         [IN]  �g�p����Oam��(��[���l)
                fromAffine:     [IN]  �g�p����Affine�i�J�n�ԍ��j
                numAffine:      [IN]  �g�p����A�t�B����(�[������)
                type:           [IN]  Oam�̎��
                
                
  Returns:      �������̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dGetNewOamManagerInstance
( 
    NNSG2dOamManagerInstance*   pMan, 
    u16                         fromOBJ, 
    u16                         numOBJ, 
    u16                         fromAffine, 
    u16                         numAffine, 
    NNSG2dOamType               type 
)
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_NON_ZERO_ASSERT( numOBJ );// MUST BE !    
    NNS_G2D_ASSERT( numOBJ <= NUM_HW_OAM_ATTR );
	NNS_G2D_ASSERT( numAffine <= NUM_HW_OAM_AFFINE );
    NNS_G2D_NULL_ASSERT( pMan );
    //
    // OBJ �ɂ���
    //
    {
        u16*        pReserveTbl = GetOamReservationTblPtr_( type );
        const u16   toOBJ       = (u16)(fromOBJ + (numOBJ - 1));
        
        NNS_G2D_NULL_ASSERT( pReserveTbl );
        //
        // �w�肳�ꂽ�̈悪���g�p�ł����...
        //
        if( IsOamNotUsed_( pReserveTbl, fromOBJ, toOBJ ) )
        {
            //
            // ���̂ւ̃p�����[�^�̑�����s��
            //
            pMan->GUID                          = GetNewGUID_();
            pMan->managedAttrArea.fromIdx       = fromOBJ;
            pMan->managedAttrArea.toIdx         = toOBJ;
            pMan->managedAttrArea.currentIdx    = fromOBJ;
            
            DoReserveArea_( pReserveTbl, fromOBJ, toOBJ, pMan->GUID );
            
        }else{
            
            OS_Warning("Failure in NNS_G2dGetNewManagerInstance().\n The manageArea that you specified has been used by someone.");
            return FALSE;
        }
    }
    
    //
    // �A�t�B���p�����[�^�ɂ���
    //
    {
        u16*        pReserveTbl = GetAffineReservationTblPtr_( type );
        NNS_G2D_NULL_ASSERT( pReserveTbl );
        
        
        if( numAffine == 0 )
        {
            //
            // �A�t�B���p�����[�^ ���g�p���Ȃ��ꍇ�A�s���ȃf�[�^��}������
            // �s���ȃf�[�^�̑}���͏d�v�ł���B�i���W���[�������ł͕s���ȊǗ��̈�ݒ�𔻒肵�āA�������s���B�j
            //
            pMan->managedAffineArea.fromIdx    = NUM_HW_OAM_AFFINE;
            pMan->managedAffineArea.toIdx      = 0;
            pMan->managedAffineArea.currentIdx = pMan->managedAffineArea.fromIdx;
            
        }else{
            //
            // �A�t�B���p�����[�^ ���g�p����ꍇ
            //
            const u16   toAffine       = (u16)(fromAffine + (numAffine - 1));
            //
            // �w�肳�ꂽ�̈悪���g�p�ł����...
            //
            if( IsOamNotUsed_( pReserveTbl, fromAffine, toAffine ) )
            {
                pMan->managedAffineArea.fromIdx    = fromAffine;
                pMan->managedAffineArea.toIdx      = toAffine;
                pMan->managedAffineArea.currentIdx = fromAffine;
                
                DoReserveArea_( pReserveTbl, fromAffine, toAffine, pMan->GUID );
                
            }else{
                OS_Warning("Failure in NNS_G2dGetNewManagerInstanceNew().\n The manageArea that you specified has been used by someone.");
                return FALSE;
            }
        }
    }
    
    pMan->bFastTransferEnable           = FALSE;
    pMan->type                          = type;
    
    //
    // �؂�Ȃ�����������������
    //
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetNewOamManagerInstanceAsFastTransferMode

  Description:  OamManager �� ���̂����������܂�
                
                OAM�}�l�[�W���̏�����������V����API�ł��B
                
                �m�ۂ���̈�ɐ��������݂��܂��B
                �܂��A�Ǘ��A�t�B���p�����[�^�̈�͊Ǘ�OAM�A�g���r���[�g�̈���Ɋ܂܂��A��������
                ���蓖�Ă��܂��B(��FOAM 0 - 32 �� => �A�t�B���p�����[�^ 0 - 8 �� )
                
                �o�b�t�@����O���t�B�b�N�X�G���W���ւ̃f�[�^�]���̍ۂ�
                �����]�����\�ƂȂ��Ă��܂��B
                
  Arguments:    pMan:           [OUT] �}�l�[�W������
                fromOBJ:        [IN]  �g�p����Oam�i�J�n�ԍ��F4�̔{���ł���K�v������܂��j
                numOBJ:         [IN]  �g�p����Oam��(��[���l�F�F4�̔{���ł���K�v������܂�)
                type:           [IN]  Oam�̎��
                
                
  Returns:      �������̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dGetNewOamManagerInstanceAsFastTransferMode
( 
    NNSG2dOamManagerInstance*   pMan, 
    u16                         fromOBJ, 
    u16                         numOBJ,
    NNSG2dOamType               type 
)
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_NON_ZERO_ASSERT( numOBJ );// MUST BE !    
    NNS_G2D_ASSERT( numOBJ <= NUM_HW_OAM_ATTR );
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERT( fromOBJ % 4 == 0 );
    NNS_G2D_ASSERT( numOBJ % 4 == 0 );
    //
    // OBJ �ɂ���
    //
    {
        u16*        pReserveTbl = GetOamReservationTblPtr_( type );
        const u16   toOBJ       = (u16)(fromOBJ + (numOBJ - 1));
        
        NNS_G2D_NULL_ASSERT( pReserveTbl );
        //
        // �w�肳�ꂽ�̈悪���g�p�ł����...
        //
        if( IsOamNotUsed_( pReserveTbl, fromOBJ, toOBJ ) )
        {
            //
            // ���̂ւ̃p�����[�^�̑�����s��
            //
            pMan->GUID                          = GetNewGUID_();
            pMan->managedAttrArea.fromIdx       = fromOBJ;
            pMan->managedAttrArea.toIdx         = toOBJ;
            pMan->managedAttrArea.currentIdx    = fromOBJ;
            
            DoReserveArea_( pReserveTbl, fromOBJ, toOBJ, pMan->GUID );
            
        }else{
            
            OS_Warning("Failure in NNS_G2dGetNewOamManagerInstanceAsFastTransferMode().\n The manageArea that you specified has been used by someone.");
            return FALSE;
        }
    }
    
    //
    // �A�t�B���p�����[�^�ɂ���
    //
    {
        const u16   fromAffine     = (u16)(fromOBJ / 4);
        const u16   numAffine      = (u16)(numOBJ / 4);
        const u16   toAffine       = (u16)(fromAffine + (numAffine - 1));
        
        u16*        pReserveTbl = GetAffineReservationTblPtr_( type );
        NNS_G2D_NULL_ASSERT( pReserveTbl );        
        //
        // �w�肳�ꂽ�̈悪���g�p�ł����...
        //
        if( IsOamNotUsed_( pReserveTbl, fromAffine, toAffine ) )
        {
            pMan->managedAffineArea.fromIdx    = fromAffine;
            pMan->managedAffineArea.toIdx      = toAffine;
            pMan->managedAffineArea.currentIdx = fromAffine;
            
            DoReserveArea_( pReserveTbl, fromAffine, toAffine, pMan->GUID );
            
        }else{
            OS_Warning("Failure in NNS_G2dGetNewManagerInstanceNew().\n The manageArea that you specified has been used by someone.");
            return FALSE;
        }
    }
    
    //
    // �����]���\�t���O���Z�b�g����
    //
    pMan->bFastTransferEnable           = TRUE;
    pMan->type                          = type;
    
    //
    // �؂�Ȃ�����������������
    //
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManagerOam

  Description:  Oam �� �o�^���܂��B
                �v�����ꂽ�o�^�̂��߂̏\����Oam�������Ȃ��ꍇ�́A
                ��������FALSE��Ԃ��܂��B
                
  Arguments:    pMan:       [OUT] �}�l�[�W������
                pOam:       [IN]  �o�^ ���� Oam �̐擪�|�C���^
                num:        [IN]  �o�^ ���� Oam ��
                
  Returns:      �o�^�̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dEntryOamManagerOam
( 
    NNSG2dOamManagerInstance*   pMan, 
    const GXOamAttr*            pOam, 
    u16                         num 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_ASSERT( num != 0 );
    
    //
    // �\���ȗe�ʂ������...
    //
    if( HasEnoughCapacity_( &pMan->managedAttrArea, num ) )
    {
        //
        // �f�[�^���o�b�t�@�ɃR�s�[����
        // affine paramater �� �㏑�����Ȃ��悤�ɁA�������R�s�[ �� �������čs���B
        //     
        int i = 0;
        GXOamAttr* pOamAttr = GetOamCurrentPtr_( pMan );
        for( i = 0; i < num; i ++ )
        {    
            //MI_CpuCopy16( pOam, pOamAttr, OAMATTR_SIZE );
            pOamAttr[i].attr0 = pOam->attr0;
            pOamAttr[i].attr1 = pOam->attr1;
            pOamAttr[i].attr2 = pOam->attr2;
            
            pMan->managedAttrArea.currentIdx++;
            pOam++;
        }
        
        //
        // �f�o�b�N�o��
        //
        NNSI_G2D_DEBUGMSG1( "New Oam entry occur...  from %d to %d for GUID %d\n",
                             pMan->currentPos, 
                             pMan->currentPos + num, 
                             pMan->GUID );
        
        return TRUE;
    }else{
        NNSI_G2D_DEBUGMSG0("The OamBuffer has no capacity enough to store new Oam.");
        return FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManagerOamWithAffineIdx

  Description:  OAM �A�g���r���[�g�p�����[�^ �� affine index ���w�肵�� �o�^���܂��B
                �v�����ꂽ�o�^�̂��߂̏\���ȗe�ʂ������Ȃ��ꍇ�́A
                ��������FALSE��Ԃ��܂��B
                
  Arguments:    pMan:       [OUT] �}�l�[�W������
                pOam:       [IN]  �o�^ ���� Affine Matrix 
                affineIdx:  [IN]  OBJ �� �Q�Ƃ��� Affine index 
                
  Returns:      �o�^�̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dEntryOamManagerOamWithAffineIdx
( 
    NNSG2dOamManagerInstance*   pMan, 
    const GXOamAttr*            pOam, 
    u16                         affineIdx 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pOam );
   
    
    //
    // �\���ȗe�ʂ������...
    //
    if( HasEnoughCapacity_( &pMan->managedAttrArea, 1 ) )
    {
        GXOamAttr* pOamAttr = GetOamCurrentPtr_( pMan );
        
        
        // MI_CpuCopy16( pOam, pOamAttr, OAMATTR_SIZE );
        pOamAttr->attr0 = pOam->attr0;
        pOamAttr->attr1 = pOam->attr1;
        pOamAttr->attr2 = pOam->attr2;
        
        //
        // affine Index ���w�肳��Ă�����...
        //
        if( NNS_G2D_OAM_AFFINE_IDX_NONE != affineIdx )
        {
            
            // G2_SetOBJEffect( pOamAttr, GX_OAM_EFFECT_AFFINE_DOUBLE, affineIdx );
            {
                
                // ��]�g��k���C�l�[�u���t���O�̃`�F�b�N
                // �A�t�B���ϊ��L���t���O���L���ɐݒ肳��Ă��Ȃ��ꍇ
                // oamAffinIdx ��ݒ肵�܂��� 
                // �ȑO�̓A�T�[�g�ŋ����I�����Ă��܂����B
                //
                // NNS_G2D_ASSERT( pOamAttr->rsMode & 0x1 );
                //
                if( pOamAttr->rsMode & 0x1 )                
                {
                    pOamAttr->rsParam = affineIdx;
                }
            }
        }
        
        pMan->managedAttrArea.currentIdx++;
        return TRUE;
    }else{
        NNSI_G2D_DEBUGMSG0("The OamBuffer has no capacity enough to store new Oam.");
        return FALSE;
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetOamManagerAffine

  Description:  Oam Affine �p�����[�^ �� index ���w�肵�� �o�^���܂��B
                �s����OAM��ށA�C���f�b�N�X���w�肵���ꍇ�̓A�T�[�g�Ɏ��s���܂��B
                
                �A�t�B���p�����[�^ �Ǘ��̈� �ւ̏������݂��������ꍇ�́A�x����\�����܂��B
                
  Arguments:    type:      [OUT] OAM���
                mtx:       [IN]  �o�^ ���� Affine Matrix 
                idx:       [IN]  Affine �� �o�^ ���� index 
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetOamManagerAffine
( 
    NNSG2dOamType               type, 
    const MtxFx22*              mtx, 
    u16                         idx 
)
{
    ASSERT_OAMTYPE_VALID( type );
    NNS_G2D_NULL_ASSERT( mtx );
    NNS_G2D_MINMAX_ASSERT( idx, 0, NUM_HW_OAM_AFFINE );
    
    
    SDK_WARNING( IsOamNotUsed_( GetAffineReservationTblPtr_( type ), idx, idx ),
                "An invalid affine param setting to the managed area is detected. index = %d", idx );

    
    SetAffineParams_( type, mtx, idx );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManagerAffine

  Description:  Oam Affine �p�����[�^ �o�^���܂��B
                �o�^�� Affine�p�����[�^Index��Ԃ��܂��B
                �v�����ꂽ�o�^�̂��߂̏\���ȗe�ʂ������Ȃ��ꍇ�́A
                �������� NNS_G2D_OAM_AFFINE_IDX_NONE ��Ԃ��܂��B
                
                ���������ɁA�A�t�B���p�����[�^�����蓖�ĂĂ��Ȃ��ꍇ�̓A�T�[�g�Ɏ��s���܂��B
                
  Arguments:    pMan:      [OUT] �}�l�[�W������
                mtx:       [IN]  �o�^ ���� Affine Matrix 
                
  Returns:      Affine�p�����[�^Index
  
 *---------------------------------------------------------------------------*/
u16 NNS_G2dEntryOamManagerAffine( NNSG2dOamManagerInstance* pMan, const MtxFx22* mtx )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( mtx );
    NNS_G2D_ASSERT( IsManageAreaValid_( &pMan->managedAffineArea ) );
    
    //
    // �\���ȗe�ʂ������...
    //
    if( HasEnoughCapacity_( &pMan->managedAffineArea, 1 ) )
    {
        const u16 currentAffineIdx = pMan->managedAffineArea.currentIdx;

        SetAffineParams_( pMan->type, mtx, currentAffineIdx );
        
        pMan->managedAffineArea.currentIdx++;
        
        return currentAffineIdx;
    }else{
        NNSI_G2D_DEBUGMSG0("The OamBuffer has no capacity enough to store new Affine Paramater.");
        return NNS_G2D_OAM_AFFINE_IDX_NONE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dApplyOamManagerToHW

  Description:  OamManager ����buffer �� HW OAM �ɓK�p���܂�
                �`�撆�̉�ʂɉe����^�����ɔ��f���������邽�߂ɂ́A
                �`��u�����N�Ɏ��s����K�v������܂��B
                
  Arguments:    pMan:                [OUT] �}�l�[�W������
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/     
void NNS_G2dApplyOamManagerToHW( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERTMSG( pMan->type != NNS_G2D_OAMTYPE_SOFTWAREEMULATION, 
                    " For the NNS_G2D_OAMTYPE_SOFTWAREEMULATION type Manager, Use NNS_G2dApplyOamManagerToHWSprite() instead." );
    {
        //
        // �����]��(OAMAttr�ƃA�t�B���p�����[�^�̈ꊇ�]��)���\���H
        //
        if( pMan->bFastTransferEnable )
        {
            LoadOamAndAffineFast_( pMan->type,
                                   pMan->managedAttrArea.fromIdx, 
                                   pMan->managedAttrArea.toIdx );
        }else{
            // OAM Attr
            LoadOam_    ( pMan->type, 
                          pMan->managedAttrArea.fromIdx, 
                          pMan->managedAttrArea.toIdx );
            // affine params
            if( IsManageAreaValid_( &pMan->managedAffineArea ) )
            {    
                LoadAffine_ ( pMan->type, 
                              pMan->managedAffineArea.fromIdx, 
                              pMan->managedAffineArea.toIdx );
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dApplyOamManagerToHWSprite 

  Description:  3D Graphics Engine �� �g�p���� �}�l�[�W���̓��e��`�悵�܂�
                
  Arguments:    pMan          :      [OUT] �}�l�[�W������
                pTexImageAttr :      [IN]  VRAM �e�N�X�`���C���[�W�̑���
                texBaseAddr   :      [IN]  VRAM �x�[�X�A�h���X
                pltBaseAddr   :      [IN]  �p���b�g �x�[�X�A�h���X
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/     
void NNS_G2dApplyOamManagerToHWSprite
( 
    NNSG2dOamManagerInstance*       pMan, 
    const NNSG2dImageAttr*          pTexImageAttr,
    u32                             texBaseAddr,
    u32                             pltBaseAddr
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pTexImageAttr );
    
    
    NNS_G2D_ASSERTMSG( pMan->type == NNS_G2D_OAMTYPE_SOFTWAREEMULATION, 
        " For the NNS_G2D_OAMTYPE_MAIN SUB type Manager, Use NNS_G2dApplyOamManagerToHW() instead." );
    
    if( pMan->spriteZoffsetStep != 0 )
    {
        fx32 step = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( TRUE );
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( pMan->spriteZoffsetStep );
        
        {
            void*       pFrom   = GetOamFromPtr_( pMan );
            const u16   numOam  = GetNumOamUsed_( &pMan->managedAttrArea );
            NNS_G2D_NULL_ASSERT( pFrom );
            
            
            DrawBy3DGraphicsEngine_( pFrom, 
                                     numOam, 
                                     pTexImageAttr,
                                     texBaseAddr,
                                     pltBaseAddr );
        }
        
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( step );
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( FALSE );
        NNSi_G2dResetOamSoftEmuAutoZOffset();
    }else{
        {
           void*       pFrom   = GetOamFromPtr_( pMan );
           const u16   numOam  = GetNumOamUsed_( &pMan->managedAttrArea );
           NNS_G2D_NULL_ASSERT( pFrom );
           
           
           DrawBy3DGraphicsEngine_( pFrom, 
                                    numOam, 
                                    pTexImageAttr,
                                    texBaseAddr,
                                    pltBaseAddr );
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetOamManagerBuffer

  Description:  OamManager ���� buffer �� ���Z�b�g���܂��B
                
  Arguments:    pMan:                [OUT] �}�l�[�W������
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/ 
void NNS_G2dResetOamManagerBuffer( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );

    // �f�t�H���g�l�Ń��Z�b�g
    {
        //
        // �����]���\��ԂȂ��
        //
        if( pMan->bFastTransferEnable )
        {    
            //
            // DMA �ɂ���Ĉ�C�Ƀ������t�B�����s��
            //            
            const u32 szByte = GetSizeOfManageArea_( &pMan->managedAttrArea );
            void* pData = GetOamFromPtr_( pMan );
            NNS_G2D_ASSERT( szByte != 0 );
            
            DC_InvalidateRange( pData, szByte );
            NNSi_G2dDmaFill32( NNS_G2D_DMA_NO, pData, OAM_SETTING_INVISIBLE, szByte );
            
        }else{    
            //
            // CPU �R�s�[�ɂ���čא؂�Ƀ������t�B�����s��
            //
            // OAM Attr
            ResetOam_( pMan->type, 
                       pMan->managedAttrArea.fromIdx, 
                       pMan->managedAttrArea.toIdx );
            
            //
            // �A�t�B���p�����[�^�̃��Z�b�g�͕K�v�Ȃ��͂��Ȃ̂ŁA�s��Ȃ��l�ɕύX���܂����B
            //
            /*
            // affine params
            if( IsManageAreaValid_( &pMan->managedAffineArea ) )
            {
                ResetAffine_( pMan->type, 
                              pMan->managedAffineArea.fromIdx, 
                              pMan->managedAffineArea.toIdx );
            }
            */
        }
    }
    
    // �J�E���^�[�����Z�b�g����
    {
        pMan->managedAttrArea.currentIdx    = pMan->managedAttrArea.fromIdx;
        pMan->managedAffineArea.currentIdx  = pMan->managedAffineArea.fromIdx;
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dApplyAndResetOamManagerBuffer

  Description:  OamManager ���� buffer �� HW OAM �ɓK�p������A
                OamManager ���� buffer �� ���Z�b�g���܂��B
                
  Arguments:    pMan:                [OUT] �}�l�[�W������
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/    
void NNS_G2dApplyAndResetOamManagerBuffer( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    
    NNS_G2dApplyOamManagerToHW( pMan );
    NNS_G2dResetOamManagerBuffer( pMan );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetOamManagerOamCapacity, NNS_G2dGetOamManagerAffineCapacity

  Description:  ���p�\�ȃ��\�[�X�̌����擾���܂�
                
  Arguments:    pMan:                [IN]  �}�l�[�W������
                
                
  Returns:      ���p�\�ȃ��\�[�X�̌�
  
 *---------------------------------------------------------------------------*/
u16     NNS_G2dGetOamManagerOamCapacity( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    return GetCapacity_( &pMan->managedAttrArea );
}

//------------------------------------------------------------------------------
u16     NNS_G2dGetOamManagerAffineCapacity( NNSG2dOamManagerInstance* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    return GetCapacity_( &pMan->managedAffineArea );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetOamBuffer

  Description:  OAM�}�l�[�W�����W���[�������o�b�t�@�̎Q�Ƃ��擾���܂��B
                
  Arguments:    type:                [IN]  OAM�o�b�t�@�̎��
                
                
  Returns:      �����o�b�t�@�ւ̃|�C���^
  
 *---------------------------------------------------------------------------*/
GXOamAttr* NNS_G2dGetOamBuffer( NNSG2dOamType type )
{
    return GetOamBufferPtr_( type, 0 );    
}



