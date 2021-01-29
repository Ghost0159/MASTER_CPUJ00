/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_Character_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_CHARACTER_DATA_H_
#define NNS_G2D_CHARACTER_DATA_H_

#include <nitro/types.h>

#ifndef SDK_ARM9
#define SDK_ARM9
    #include <nitro_win32.h>
#undef  SDK_ARM9
#endif //SDK_ARM9

#include <nnsys/g2d/fmt/g2d_Common_data.h>
#include <nnsys/g2d/g2d_config.h>

#ifdef __cplusplus
extern "C" {
#endif

// Binary file signature
#define NNS_G2D_BINFILE_SIG_CHARACTERDATA          (u32)'NCGR'

#define NNS_G2D_BINFILE_SIG_PALETTEDATA            (u32)'NCLR'
#define NNS_G2D_BINFILE_SIG_PALETTEDATA_OLD        (u32)'NCPR'

// Binary block signature
#define NNS_G2D_BINBLK_SIG_CHARACTERDATA          (u32)'CHAR'
#define NNS_G2D_BINBLK_SIG_CHAR_POSITION          (u32)'CPOS'

#define NNS_G2D_BINBLK_SIG_PALETTEDATA            (u32)'PLTT'
#define NNS_G2D_BINBLK_SIG_PALETTECOMPINFO        (u32)'PCMP'



#define NNS_G2D_BINFILE_EXT_CHARACTERDATA          "NCGR"
#define NNS_G2D_BINFILE_EXT_CHARACTERDATA_BMP      "NCBR"
#define NNS_G2D_BINFILE_EXT_PALETTEDATA            "NCLR"

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
// 1.1         NNSG2dCharacterPosInfoBlock �̒ǉ�
//
#define NNS_G2D_NCGR_MAJOR_VER             (u8)1
#define NNS_G2D_NCGR_MINOR_VER             (u8)1

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
//
#define NNS_G2D_NCLR_MAJOR_VER             (u8)1
#define NNS_G2D_NCLR_MINOR_VER             (u8)0

#define NNS_G2D_1D_MAPPING_CHAR_SIZE      (u16)0xFFFF

#define NNS_G2D_CHARACTER_FMT_MASK              0xFF    // 8 bit
#define NNS_G2D_VRAMTRANSFERDATA_FLAG_MASK      0x1     // 1 bit
#define NNS_G2D_CHARACTER_FMT_SHIFT             0       
#define NNS_G2D_VRAMTRANSFERDATA_FLAG_SHIFT     8

//------------------------------------------------------------------------------
typedef enum NNSG2dCharacterPixelFmt
{
    NNS_G2D_CHARACTER_PIXELFMT_16,
    NNS_G2D_CHARACTER_PIXELFMT_256,
    NNS_G2D_CHARACTER_PIXELFMT_MAX
    
}NNSG2dCharacterPixelFmt;

// �L�����N�^�f�[�^�̃f�[�^�������
typedef enum NNSG2dCharacterFmt
{
    NNS_G2D_CHARACTER_FMT_CHAR,  // 8*8 �e�N�Z�����Ƃɐ��񂵂Ă���
    NNS_G2D_CHARACTER_FMT_BMP,   // ��ʓI�ȃr�b�g�}�b�v����
    NNS_G2D_CHARACTER_FMT_MAX

}NNSG2dCharacterFmt;

//------------------------------------------------------------------------------
typedef enum NNSG2dCharacterDataMapingType
{
    NNS_G2D_CHARACTERMAPING_1D_32,
    NNS_G2D_CHARACTERMAPING_1D_64,
    NNS_G2D_CHARACTERMAPING_1D_128,
    NNS_G2D_CHARACTERMAPING_1D_256,
    NNS_G2D_CHARACTERMAPING_2D,
    NNS_G2D_CHARACTERMAPING_MAX
 
}NNSG2dCharacterDataMapingType;

//------------------------------------------------------------------------------
typedef enum NNSG2dPaletteFmt
{
    NNS_G2D_NNSG2DPALETTEFMT_16X16,
    NNS_G2D_NNSG2DPALETTEFMT_256X1,
    NNS_G2D_NNSG2DPALETTEFMT_256X16,
    NNS_G2D_NNSG2DPALETTEFMT_MAX
    
}NNSG2dPaletteFmt;

//------------------------------------------------------------------------------
typedef struct NNSG2dPaletteCompressInfo
{
    u16               numPalette;
    u16               pad16;
    void*             pPlttIdxTbl;        // offset addr.
    
}NNSG2dPaletteCompressInfo;

//------------------------------------------------------------------------------
typedef struct NNSG2dPaletteData
{
    
    GXTexFmt            fmt;
    BOOL                bExtendedPlt;
    
    u32                 szByte;
    void*               pRawData;          // offset addr of the content.
    
}NNSG2dPaletteData;

//------------------------------------------------------------------------------
typedef struct NNSG2dPaletteCompressDataBlock
{
    
    NNSG2dBinaryBlockHeader             blockHeader;
    NNSG2dPaletteCompressInfo           plttCmpInfo;
    
}NNSG2dPaletteCompressDataBlock;

//------------------------------------------------------------------------------
typedef struct NNSG2dPaletteDataBlock
{
    
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dPaletteData           paletteData;
    
}NNSG2dPaletteDataBlock;

//------------------------------------------------------------------------------
typedef struct NNSG2dCharacterData
{
    
    u16                             H;           
    u16                             W;           
    
    GXTexFmt                        pixelFmt;    // GXTexFmt
    
    GXOBJVRamModeChar               mapingType;  // GXOBJVRamModeChar
    u32                             characterFmt;// 31 ..... 10 ...........9 8 ................ 0
                                                 //     �\��   VramTransfer   NNSG2dCharacterFmt
    u32                             szByte;
    void*                           pRawData;  // offset addr of the content.
    
}NNSG2dCharacterData;


//------------------------------------------------------------------------------
typedef struct NNSG2dCharacterDataBlock
{
    
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dCharacterData         characterData;
    
}NNSG2dCharacterDataBlock;


//------------------------------------------------------------------------------
typedef struct NNSG2dCharacterPosInfo
{
    
    u16                   srcPosX;  
    u16                   srcPosY;
    u16                   srcW;
    u16                   srcH;
    
}NNSG2dCharacterPosInfo;

//------------------------------------------------------------------------------
typedef struct NNSG2dCharacterPosInfoBlock
{
    
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dCharacterPosInfo         posInfo;
    
}NNSG2dCharacterPosInfoBlock;


//------------------------------------------------------------------------------
// �R���o�[�^���g�p����֐��ł�
NNS_G2D_INLINE u32 NNSi_G2dPackCharacterFmtData
( 
    NNSG2dCharacterFmt          charFmt,
    BOOL                        bVramTransferData 
)
{
    return (u32)( (charFmt << NNS_G2D_CHARACTER_FMT_SHIFT) | 
                  ( bVramTransferData << NNS_G2D_VRAMTRANSFERDATA_FLAG_SHIFT ) );
}

//------------------------------------------------------------------------------
// �L�����N�^�̐���������擾���܂�
NNS_G2D_INLINE NNSG2dCharacterFmt NNSi_G2dGetCharacterFmtType( u32 characterFmt )
{
    return (NNSG2dCharacterFmt)
    (NNS_G2D_CHARACTER_FMT_MASK & (characterFmt >> NNS_G2D_CHARACTER_FMT_SHIFT));
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE BOOL NNSi_G2dIsCharacterVramTransfered( u32 characterFmt )
{
    return (BOOL)
    (NNS_G2D_VRAMTRANSFERDATA_FLAG_MASK & ( characterFmt >> NNS_G2D_VRAMTRANSFERDATA_FLAG_SHIFT ) );
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_CHARACTER_DATA_H_

