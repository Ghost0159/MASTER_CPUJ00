/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g3d - src - binres
  File:     res_struct_accessor_anm.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.36 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#include <nnsys/g3d/binres/res_struct_accessor.h>

#include "res_print_internal.h"




#define NNS_G3D_SIZE_NNSG3DRESJNTANM    sizeof( NNSG3dResJntAnm )
#define NNS_G3D_SIZE_ROTMTX3            ( 3 * sizeof( u16 ) )
#define NNS_G3D_SIZE_ROTMTX5            ( 5 * sizeof( u16 ) )


BOOL NNSi_G3dIsValidAnmRes(const void* pRes)
{
    NNS_G3D_NULL_ASSERT(pRes);

    if (pRes)
    {
        const NNSG3dResAnmCommon* p = (const NNSG3dResAnmCommon*) pRes;

        if (p->anmHeader.category0 == 'M' ||
            p->anmHeader.category0 == 'J' ||
            p->anmHeader.category0 == 'V')
        {
            if (p->anmHeader.category1 == 'CA' ||
                p->anmHeader.category1 == 'VA' ||
                p->anmHeader.category1 == 'MA' ||
                p->anmHeader.category1 == 'TP' ||
                p->anmHeader.category1 == 'TA')
                return TRUE;
        }
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGetAnmByIdx

    �A�j���[�V�������\�[�X�t�@�C������idx�Ԗڂ̃A�j���[�V�������\�[�X���擾
    �A�j���[�V�����̎�ނ͖��Ȃ�
 *---------------------------------------------------------------------------*/
void*
NNS_G3dGetAnmByIdx(const void* pRes, u32 idx)
{
    NNS_G3D_NULL_ASSERT(pRes);

    if (pRes)
    {
        const NNSG3dResFileHeader* header;
        const NNSG3dResAnmSet* anmSet;
        const NNSG3dResDictAnmSetData* anmSetData;
        u32* blks;

        header = (const NNSG3dResFileHeader*) pRes;
        blks = (u32*)((u8*)header + header->headerSize);

        anmSet = (const NNSG3dResAnmSet*)((u8*)header + blks[0]);
        anmSetData = (const NNSG3dResDictAnmSetData*)NNS_G3dGetResDataByIdx(&anmSet->dict, idx);

        if (anmSetData)
        {
            return (void*)((u8*)anmSet + anmSetData->offset);
        }
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
    NNS_G3dGetAnmByName

    �A�j���[�V�������\�[�X�t�@�C������*pName�̖��O�ŎQ�Ƃł���A�j���[�V�������\�[�X���擾
    �A�j���[�V�����̎�ނ͖��Ȃ�
 *---------------------------------------------------------------------------*/
void*
NNS_G3dGetAnmByName(const void* pRes, const NNSG3dResName* pName)
{
    NNS_G3D_NULL_ASSERT(pRes);
    
    if (pRes)
    {
        const NNSG3dResFileHeader* header;
        const NNSG3dResAnmSet* anmSet;
        const NNSG3dResDictAnmSetData* anmSetData;
        u32* blks;
        header = (const NNSG3dResFileHeader*) pRes;
        blks = (u32*)((u8*)header + header->headerSize);

        anmSet = (const NNSG3dResAnmSet*)((u8*)header + blks[0]);
        anmSetData = (const NNSG3dResDictAnmSetData*)NNS_G3dGetResDataByName(&anmSet->dict, pName);

        if (anmSetData)
        {
            return (void*)((u8*)anmSet + anmSetData->offset);
        }
    }

    return NULL;
}

//---------------------------------------------------------------------
// �f�[�^�z��ɃA�N�Z�X����֐���\���|�C���^
typedef fx32  (*FrameIdxValueAccessFuncPtr)( const void* pDataHead, u32 frameIdx, u32 step, u32 offset );


/*---------------------------------------------------------------------------*
     �A�j���[�V�����֘A ����
 *---------------------------------------------------------------------------*/

const void* 
NNSi_G3dGetBinaryBlockFromFile
( 
    const u8*  pFileHead, 
    u32        fileSignature, 
    u32        blockSignature 
)
{
    NNS_G3D_NULL_ASSERT( pFileHead );
    if (pFileHead)
    {
        u32 i = 0;
        const NNSG3dResFileHeader*   pHeader
           = (const NNSG3dResFileHeader*)&pFileHead[0];

        if( pHeader->sigVal == fileSignature )
        {        
            //
            // ���ׂẴo�C�i���u���b�N�ɂ���...
            //
            for( i = 0 ; i < pHeader->dataBlocks; i++ )
            {
               const NNSG3dResDataBlockHeader* blk 
                   = NNS_G3dGetDataBlockHeaderByIdx( pHeader, i );
               NNS_G3D_NULL_ASSERT( blk );

               if( blk->kind == blockSignature )
               {
                   return (const void*)blk;
               }
            }
        }
    }
    //
    // �����ł��Ȃ�����
    //
    return NULL;
}


//------------------------------------------------------------------------------
// �A�j���w�b�_���w�肵���J�e�S�����������������܂��B
static BOOL IsValidAnimHeader( const NNSG3dResAnmHeader* pAnmHeader, u8 cat0, u16 cat1 )
{
    NNS_G3D_NULL_ASSERT( pAnmHeader );
    return (BOOL)( pAnmHeader                    &&
                   pAnmHeader->category0 == cat0 && 
                   pAnmHeader->category1 == cat1 );
}





/*---------------------------------------------------------------------------*
    Visibility �A�j���[�V�����֘A
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetVisAnmByIdx

  Description:  �r�W�r���e�B�A�j���[�V�����Z�b�g���� �w�肵���ԍ��� 
                �r�W�r���e�B�A�j���[�V�������擾���܂��B
                �s���Ȕԍ��̓��͂��������ꍇ�́AASSERT�Ɏ��s���܂��B
                
  Arguments:    pAnmSet   :        �r�W�r���e�B�A�j���[�V�����Z�b�g
                idx       :        �t�@�C���ԍ�
                
  Returns:      �r�W�r���e�B�A�j���[�V����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResVisAnm*
NNS_G3dGetVisAnmByIdx( const NNSG3dResVisAnmSet* pAnmSet, u8 idx  )
{
    NNS_G3D_NULL_ASSERT( pAnmSet );
    NNS_G3D_ASSERT( idx < pAnmSet->dict.numEntry );
    
    if (pAnmSet)
    {
        const NNSG3dResDictVisAnmSetData* pSetData =
            (const NNSG3dResDictVisAnmSetData*)NNS_G3dGetResDataByIdx( &pAnmSet->dict, idx );
        
        if (pSetData)
            return (const NNSG3dResVisAnm*)((u8*)pAnmSet + pSetData->offset);
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetVisAnmSet

  Description:  �r�W�r���e�B�A�j���[�V�����Z�b�g���擾���܂��B
                
  Arguments:    pFileHead   :        nsbva�̐擪�������|�C���^
                
                
  Returns:      �r�W�r���e�B�A�j���[�V�����Z�b�g
  
 *---------------------------------------------------------------------------*/ 
const NNSG3dResVisAnmSet*
NNS_G3dGetVisAnmSet( const u8* pFileHead )
{
    const void *pVisBlk 
        = NNSi_G3dGetBinaryBlockFromFile( pFileHead,
                                          NNS_G3D_SIGNATURE_NSBVA,
                                          NNS_G3D_DATABLK_VIS_ANM );
    NNS_G3D_NULL_ASSERT( pVisBlk );
    return (const NNSG3dResVisAnmSet*)pVisBlk;
}





/*---------------------------------------------------------------------------*
    Texture �p�^�[�� �A�j���[�V�����֘A
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         NNSi_G3dGetTexNameByIdx

  Description:      �e�N�X�`�������擾���܂�
                
  Arguments:    pAnm      :      �e�N�X�`���p�^�[���A�j���[�V����
                idx       :      �w��e�N�X�`���ԍ�(�A�j������ NNSG3dResTexPatAnmFV.idTex )
                
                
  Returns:      �e�N�X�`����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResName* 
NNSi_G3dGetTexPatAnmTexNameByIdx( const NNSG3dResTexPatAnm* pPatAnm, u8 texIdx )
{
    NNS_G3D_NULL_ASSERT( pPatAnm );
    NNS_G3D_ASSERT( texIdx < pPatAnm->numTex );
    
    if (pPatAnm && texIdx < pPatAnm->numTex)
    {
        const NNSG3dResName* pNameArray 
            = (const NNSG3dResName*)((const u8*)pPatAnm + pPatAnm->ofsTexName);
        
        return &pNameArray[texIdx];
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G3dGetPlttNameByIdx

  Description:      �p���b�g�����擾���܂�
                
  Arguments:    pAnm      :      �e�N�X�`���p�^�[���A�j���[�V����
                idx       :      �w��e�N�X�`���ԍ�(�A�j������ NNSG3dResTexPatAnmFV.idPltt )
                
                
  Returns:      �p���b�g��
  
 *---------------------------------------------------------------------------*/
const NNSG3dResName* 
NNSi_G3dGetTexPatAnmPlttNameByIdx( const NNSG3dResTexPatAnm* pPatAnm, u8 plttIdx )
{
    NNS_G3D_NULL_ASSERT( pPatAnm );
    NNS_G3D_ASSERT( plttIdx < pPatAnm->numPltt );
    
    if (pPatAnm && plttIdx < pPatAnm->numPltt)
    {
        const NNSG3dResName* pNameArray 
            = (const NNSG3dResName*)((const u8*)pPatAnm + pPatAnm->ofsPlttName);
        
        return &pNameArray[plttIdx];
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G3dGetTexPatAnmFVByFVIndex

  Description:  �e�N�X�`���p�^�[���A�j���[�V������ 
                �w��ԍ��̎w��FV�ԍ��ɂ�����A�j���[�V�������ʂ��擾���܂��B
                
  Arguments:    pAnm      :      �e�N�X�`���p�^�[���A�j���[�V����
                idx       :      �w��}�e���A���ԍ�
                fvIdx     :      FV�ԍ�    
                
  Returns:      �e�N�X�`���p�^�[���A�j���[�V�����A�A�j���[�V��������
  
 *---------------------------------------------------------------------------*/
const NNSG3dResTexPatAnmFV* 
NNSi_G3dGetTexPatAnmFVByFVIndex( const NNSG3dResTexPatAnm* pPatAnm, u32 idx, u32 fvIdx )
{
    NNS_G3D_NULL_ASSERT( pPatAnm );
    NNS_G3D_ASSERT( IsValidAnimHeader( &pPatAnm->anmHeader, 'M', 'TP' ) );
    
    
    {
        // �e�N�X�`���p�^�[���A�j���[�V�������擾����
        const NNSG3dResDictTexPatAnmData* pAnmData = 
           NNSi_G3dGetTexPatAnmDataByIdx( pPatAnm, idx );
        
        NNS_G3D_ASSERT( fvIdx < pAnmData->numFV );
        {
           // �t���[��->(�e�N�X�`��, �p���b�g)�f�[�^�z��擪�𓾂�
           const NNSG3dResTexPatAnmFV* pfvArray 
               = (const NNSG3dResTexPatAnmFV*)((u8*)pPatAnm + pAnmData->offset);

           return &pfvArray[fvIdx];
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_G3dGetTexPatAnmFV

  Description:  �e�N�X�`���p�^�[���A�j���[�V������ 
                �w��ԍ��̎w��t���[���ɂ�����A�j���[�V�������ʂ��擾���܂��B
                
  Arguments:    pAnm      :      �e�N�X�`���p�^�[���A�j���[�V����
                idx       :      �w��}�e���A���ԍ�
                frame     :      �w��t���[��    
                
  Returns:      �e�N�X�`���p�^�[���A�j���[�V�����A�A�j���[�V��������
  
 *---------------------------------------------------------------------------*/
const NNSG3dResTexPatAnmFV* 
NNSi_G3dGetTexPatAnmFV( const NNSG3dResTexPatAnm* pPatAnm, u32 idx, u32 frame )
{
    NNS_G3D_ASSERT( IsValidAnimHeader( &pPatAnm->anmHeader, 'M', 'TP' ) );
    {
        // �e�N�X�`���p�^�[���A�j���[�V�������擾����
        const NNSG3dResDictTexPatAnmData* pAnmData = 
           NNSi_G3dGetTexPatAnmDataByIdx( pPatAnm, idx );
        {
            //
            // �t���[��->(�e�N�X�`��, �p���b�g)�f�[�^�z��擪�𓾂�
            //
            const NNSG3dResTexPatAnmFV* pfvArray 
                = (const NNSG3dResTexPatAnmFV*)((u8*)pPatAnm + pAnmData->offset);
           
            //
            // FV�C���f�b�N�X����������
            // �q���g�����ƂɃC���f�b�N�X�̌��������l�����߂�
            // ���������l���猟�����X�^�[�g���āA�{���̃C���f�b�N�X�l���擾����
            //                
            const u32 fvIdx = (u32)( (fx32)pAnmData->ratioDataFrame * frame >> FX32_SHIFT);
            NNS_G3D_ASSERT( fvIdx < pAnmData->numFV );
            //
            // ���������l���猟�����X�^�[�g���āA�{���̃C���f�b�N�X�l���擾����
            //
            {
                u32 realFvIdx = fvIdx;
                
                // NNSG3dResTexPatAnmFV�̂��t���[���l�����݂̃t���[����菬�����Ȃ�܂�
                while( realFvIdx > 0 && pfvArray[ realFvIdx ].idxFrame >= frame )
                {
                    realFvIdx--;
                }
                
                // ���� �� NNSG3dResTexPatAnmFV �̎��� FV ���t���[�������݂̃t���[�����傫���Ȃ�܂�
                while( realFvIdx + 1 < pAnmData->numFV && pfvArray[ realFvIdx + 1 ].idxFrame <= frame )
                {
                    realFvIdx++;
                }    
                NNS_G3D_ASSERT( realFvIdx < pAnmData->numFV );

                
                return &pfvArray[realFvIdx];
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_G3dGetTexPatAnmDataByIdx

  Description:  �e�N�X�`���p�^�[���A�j���[�V�����f�[�^�̎w��ԍ��}�e���A���� 
                �e�N�X�`���p�^�[���A�j���[�V�����\���̂��擾���܂��B
                
  Arguments:    pPatAnm   :      �e�N�X�`���p�^�[���A�j���[�V����
                idx       :      �w��}�e���A���ԍ�

                
  Returns:      �e�N�X�`���p�^�[���A�j���[�V�����f�[�^
  
 *---------------------------------------------------------------------------*/
const NNSG3dResDictTexPatAnmData* 
NNSi_G3dGetTexPatAnmDataByIdx( const NNSG3dResTexPatAnm* pPatAnm, u32 idx )
{
    NNS_G3D_ASSERT( IsValidAnimHeader( &pPatAnm->anmHeader, 'M', 'TP' ) );
    return (const NNSG3dResDictTexPatAnmData*)NNS_G3dGetResDataByIdx( &pPatAnm->dict, idx );
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetTexPatAnmByIdx

  Description:  �e�N�X�`���p�^�[���A�j���[�V�����Z�b�g���� �w�肵���ԍ��� 
                �e�N�X�`���p�^�[���A�j���[�V�������擾���܂��B
                �s���Ȕԍ��̓��͂��������ꍇ�́AASSERT�Ɏ��s���܂��B
                
  Arguments:    pAnmSet   :        �e�N�X�`���p�^�[���A�j���[�V�����Z�b�g
                idx       :        �t�@�C���ԍ�
                
  Returns:      �e�N�X�`���p�^�[���A�j���[�V����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResTexPatAnm*
NNS_G3dGetTexPatAnmByIdx( const NNSG3dResTexPatAnmSet* pAnmSet, u8 idx )
{
    NNS_G3D_NULL_ASSERT( pAnmSet );
    NNS_G3D_ASSERT( idx < pAnmSet->dict.numEntry );

    if (pAnmSet)
    {
        const NNSG3dResDictTexPatAnmSetData* pSetData =
            (const NNSG3dResDictTexPatAnmSetData*)NNS_G3dGetResDataByIdx( &pAnmSet->dict, idx );

        if (pSetData)
            return (const NNSG3dResTexPatAnm*)((u8*)pAnmSet + pSetData->offset);
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetTexPatAnmSet

  Description:  �e�N�X�`���p�^�[���A�j���[�V�����Z�b�g���擾���܂��B
                
  Arguments:    pFileHead   :        nsbtp�̐擪�������|�C���^
                
                
  Returns:      �e�N�X�`���p�^�[���A�j���[�V�����Z�b�g
  
 *---------------------------------------------------------------------------*/ 
const NNSG3dResTexPatAnmSet*
NNS_G3dGetTexPatAnmSet( const u8* pFileHead )
{
    const void *pVisBlk 
        = NNSi_G3dGetBinaryBlockFromFile( pFileHead,
                                          NNS_G3D_SIGNATURE_NSBTP,
                                          NNS_G3D_DATABLK_TEXPAT_ANM );
    NNS_G3D_NULL_ASSERT( pVisBlk );
    return (const NNSG3dResTexPatAnmSet*)pVisBlk;
}



/*---------------------------------------------------------------------------*
    Texture SRT �A�j���[�V�����֘A
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetTexSRTAnmByIdx

  Description:  �e�N�X�`��SRT�A�j���[�V�����Z�b�g���� �w�肵���ԍ��� �e�N�X�`��SRT�A�j���[�V�������擾���܂��B
                �s���Ȕԍ��̓��͂��������ꍇ�́AASSERT�Ɏ��s���܂��B
                
  Arguments:    pTexSRTSet :        �e�N�X�`��SRT�A�j���[�V�����Z�b�g
                idx        :        �t�@�C���ԍ�
                
  Returns:      �e�N�X�`��SRT�A�j���[�V����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResTexSRTAnm*
NNS_G3dGetTexSRTAnmByIdx( const NNSG3dResTexSRTAnmSet* pTexSRTSet, u8 idx )
{
    NNS_G3D_NULL_ASSERT( pTexSRTSet );
    NNS_G3D_ASSERT( idx < pTexSRTSet->dict.numEntry );

    if (pTexSRTSet)
    {
        const NNSG3dResDictTexSRTAnmSetData* pSetData = 
            (const NNSG3dResDictTexSRTAnmSetData*)NNS_G3dGetResDataByIdx( &pTexSRTSet->dict, idx );
        
        if (pSetData)
            return (const NNSG3dResTexSRTAnm*)((u8*)pTexSRTSet + pSetData->offset );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetTexSRTAnmSet

  Description:  �e�N�X�`��SRT�A�j���[�V�����Z�b�g������킷�o�C�i���u���b�N���擾���܂��B
                
  Arguments:    pFileHead   :        nsbta�̐擪�������|�C���^
                
                
  Returns:      �e�N�X�`��SRT�A�j���[�V�����Z�b�g
  
 *---------------------------------------------------------------------------*/ 
const NNSG3dResTexSRTAnmSet*
NNS_G3dGetTexSRTAnmSet( const u8* pFileHead )
{
    const void *pVisBlk 
        = NNSi_G3dGetBinaryBlockFromFile( pFileHead,
                                          NNS_G3D_SIGNATURE_NSBTA,
                                          NNS_G3D_DATABLK_TEXSRT_ANM );
    NNS_G3D_NULL_ASSERT( pVisBlk );
    return (const NNSG3dResTexSRTAnmSet*)pVisBlk;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetMatCAnmByIdx

  Description:  �}�e���A���J���[�A�j���[�V�����Z�b�g���� �w�肵���ԍ��� �}�e���A���J���[�A�j���[�V�������擾���܂��B
                �s���Ȕԍ��̓��͂��������ꍇ�́AASSERT�Ɏ��s���܂��B
                
  Arguments:    pAnmSet   :        �}�e���A���J���[�A�j���[�V�����Z�b�g
                idx       :        �t�@�C���ԍ�
                
  Returns:      �}�e���A���J���[�A�j���[�V����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResMatCAnm*
NNS_G3dGetMatCAnmByIdx( const NNSG3dResMatCAnmSet* pAnmSet, u8 idx )
{
    NNS_G3D_NULL_ASSERT( pAnmSet );
    NNS_G3D_ASSERT( idx < pAnmSet->dict.numEntry );
    
    if (pAnmSet)
    {
        const NNSG3dResDictMatCAnmSetData* pSetData = 
            (const NNSG3dResDictMatCAnmSetData*)NNS_G3dGetResDataByIdx( &pAnmSet->dict, idx );
        
        if (pSetData)
            return (const NNSG3dResMatCAnm*)((u8*)pAnmSet + pSetData->offset );
    }
    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetMatCAnmSet

  Description:  �}�e���A���J���[�A�j���[�V�����Z�b�g������킷�o�C�i���u���b�N���擾���܂��B
                
  Arguments:    pFileHead   :        nsbma�̐擪�������|�C���^
                
                
  Returns:      �}�e���A���J���[�A�j���[�V�����Z�b�g
  
 *---------------------------------------------------------------------------*/ 
const NNSG3dResMatCAnmSet*
NNS_G3dGetMatCAnmSet(const u8* pFileHead)
{
    const void *pVisBlk 
        = NNSi_G3dGetBinaryBlockFromFile( pFileHead,
                                          NNS_G3D_SIGNATURE_NSBMA,
                                          NNS_G3D_DATABLK_MATC_ANM );
    NNS_G3D_NULL_ASSERT( pVisBlk );
    return (const NNSG3dResMatCAnmSet*)pVisBlk;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetJntAnmSRTTag

  Description:  �w��m�[�h�ԍ��� 
                NNSG3dResJntAnmSRTTag�iSRT�f�[�^�̐������L�q����Ă���j���擾���܂�
                
                
  Arguments:    pJntAnm   :           �W���C���g�A�j���[�V����
                nodeIdx   :           �m�[�h�ԍ�
                
                  
  Returns:      �w��m�[�h�ԍ���NNSG3dResJntAnmSRTTag 
  
 *---------------------------------------------------------------------------*/
const NNSG3dResJntAnmSRTTag*
NNS_G3dGetJntAnmSRTTag(const NNSG3dResJntAnm* pJntAnm, 
                       int                    nodeIdx)
{
    NNS_G3D_NULL_ASSERT( pJntAnm );
    NNS_G3D_ASSERT( nodeIdx < pJntAnm->numNode );
    {
        const u16*    pTagOffset 
            = (const u16*)((u8*)pJntAnm + NNS_G3D_SIZE_NNSG3DRESJNTANM);
        const u16     tagOffset  
            = pTagOffset[nodeIdx];

        return (const NNSG3dResJntAnmSRTTag*)((u8*)pJntAnm + tagOffset );
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetJntAnmByIdx

  Description:  �W���C���g�A�j���[�V�����Z�b�g���� �w�肵���ԍ��� �W���C���g�A�j���[�V���� ���擾���܂��B
                �s���Ȕԍ��̓��͂��������ꍇ�́AASSERT�Ɏ��s���܂��B
                
  Arguments:    pJntAnmSet:           �W���C���g�A�j���[�V�����Z�b�g
                idx       :           �W���C���g�A�j���[�V�����ԍ�
                
  Returns:      �W���C���g�A�j���[�V����
  
 *---------------------------------------------------------------------------*/
const NNSG3dResJntAnm*
NNS_G3dGetJntAnmByIdx
( 
    const NNSG3dResJntAnmSet*    pJntAnmSet, 
    int                          idx 
)
{
    NNS_G3D_NULL_ASSERT( pJntAnmSet );
    NNS_G3D_ASSERT( idx < pJntAnmSet->dict.numEntry );

    if (pJntAnmSet)
    {
        const NNSG3dResDictJntAnmSetData* pJntAnmData = 
            (const NNSG3dResDictJntAnmSetData*)NNS_G3dGetResDataByIdx( &pJntAnmSet->dict, (u32)idx );

        if (pJntAnmData)
            return (const NNSG3dResJntAnm*)((u8*)pJntAnmSet + pJntAnmData->offset);
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G3dGetJntAnmSet

  Description:  �W���C���g�A�j���[�V�����Z�b�g������킷�o�C�i���u���b�N���擾���܂��B
                
  Arguments:    pFileHead   :        nsbca�̐擪�������|�C���^
                
                
  Returns:      �W���C���g�A�j���[�V�����Z�b�g
  
 *---------------------------------------------------------------------------*/ 
const NNSG3dResJntAnmSet*
NNS_G3dGetJntAnmSet( const u8* pFileHead )
{
    const void *pVisBlk 
        = NNSi_G3dGetBinaryBlockFromFile( pFileHead,
                                          NNS_G3D_SIGNATURE_NSBCA,
                                          NNS_G3D_DATABLK_JNT_ANM );
    NNS_G3D_NULL_ASSERT( pVisBlk );
    return (const NNSG3dResJntAnmSet*)pVisBlk;
}
