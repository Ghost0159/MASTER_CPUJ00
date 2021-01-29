/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib
  File:     txt.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include "g2d_textdemolib.h"


static NNSFndAllocator sAllocator;

#define GRAY(x) GX_RGB(x, x, x)

// �f�����ʂ̃J���[�p���b�g
GXRgb TXTColorPalette[TXT_NUM_CPALEETE * 16] =
{
    GX_RGB( 0,  0,  0), GX_RGB(31, 31, 31), GX_RGB( 0,  0,  0), GX_RGB(31,  0,  0),
    GX_RGB( 0, 31,  0), GX_RGB( 0,  0, 31), GX_RGB( 0, 31, 31), GX_RGB(31,  0, 31),
    GX_RGB(31, 31,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0),
    GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0),

    GX_RGB( 0,  0,  0), GX_RGB(12, 16, 19), GX_RGB(23,  9,  0), GX_RGB(31,  0,  3),
    GX_RGB(31, 17, 31), GX_RGB(31, 18,  0), GX_RGB(30, 28,  0), GX_RGB(21, 31,  0),
    GX_RGB( 0, 20,  7), GX_RGB( 9, 27, 17), GX_RGB( 6, 23, 30), GX_RGB( 0, 11, 30),
    GX_RGB( 0,  0, 18), GX_RGB(17,  0, 26), GX_RGB(26,  0, 29), GX_RGB(31,  0, 18),

    GRAY(31),           GRAY(29),           GRAY(27),           GRAY(25),
    GRAY(23),           GRAY(21),           GRAY(19),           GRAY(17),
    GRAY(15),           GRAY(14),           GRAY(12),           GRAY(10),
    GRAY( 8),           GRAY( 6),           GRAY( 3),           GRAY( 0),
};

//****************************************************************************
//
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         AssignVramBanks

  Description:  VRAM�̊��蓖�Ă��s���܂��B

                        A B C D E F G H I
                2DA BG      o o           256 KB    �f�����w�i�p
                    OBJ o o               256 KB    �f���p
                2DB BG                o    32 KB    ���\���p
                    OBJ
                ���g�p          o o o   o 112 KB

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void AssignVramBanks( void )
{
    GX_SetBankForBG(GX_VRAM_BG_256_CD);
    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݂��������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}


//****************************************************************************
//
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         TXT_Init

  Description:  �T���v�����ʂ̏������B
                    VRAM������
                    VRAM���蓖��
                    �������A���P�[�^������
                    �t�@�C���V�X�e��������
                    �O���t�B�b�N�X���[�h�ݒ�
                    ���ʐݒ�
                    �D��x�ݒ�
                    BG�������I�t�Z�b�g�ݒ�

                ���C����� BG
                    BG���[�h4
                        BG0 �e�L�X�g        �w�i�p  ��
                        BG1 �e�L�X�g        �f���p  �s��
                        BG2 �A�t�B��        �f���p  �s��
                        BG3 �A�t�B���g��    �f���p  �s��
                        OBJ                 �f���p  �s��
                    BG VRAM 256 KB
                        00000-  192 KB  �f���p�L�����N�^
                        30000-   30 KB  �f���p�X�N���[��
                        37800-    2 KB  �w�i�p�X�N���[��
                        38000-   32 KB  �w�i�p�L�����N�^
                    OBJ VRAM 256 KB
                        00000-  256 KB  �f���p�L�����N�^

                �T�u���
                    BG���[�h0
                        BG0 �e�L�X�g        ���o�͗p  ��
                        BG1 ���g�p
                        BG2 ���g�p
                        BG3 ���g�p


  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_Init(void)
{
    // Common initialization.
    {
        OS_Init();
        FX_Init();
        GX_Init();

        GX_DispOff();
        GXS_DispOff();
    }

    {
        CMN_InitInterrupt();
        CMN_BeginVBlankIntr(VBlankIntr);
        CMN_ClearVram();
        CMN_InitAllocator( &sAllocator );
        CMN_InitFileSystem( &sAllocator );
    }

    AssignVramBanks();

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_4, GX_BG0_AS_2D);
    GX_SetBGScrOffset(GX_BGSCROFFSET_0x30000);
    GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);

    DTX_Init();
}



/*---------------------------------------------------------------------------*
  Name:         TXT_SetupBackground

  Description:  ���C�����BG0�ɔw�i�摜�����[�h���\�����܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_SetupBackground( void )
{
    void*                   pPltFile;
    void*                   pChrFile;
    void*                   pScnFile;
    NNSG2dPaletteData*      pPltData;
    NNSG2dCharacterData*    pChrData;
    NNSG2dScreenData*       pScnData;

    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(3);

    pPltFile = LoadNCLR( &pPltData, "/data/BG.NCLR", &sAllocator );
    SDK_NULL_ASSERT( pPltFile );

    pChrFile = LoadNCGR( &pChrData, "/data/BG.NCGR", &sAllocator );
    SDK_NULL_ASSERT( pChrFile );

    pScnFile = LoadNSCR( &pScnData, "/data/BG.NSCR", &sAllocator );
    SDK_NULL_ASSERT( pScnFile );

    NNS_G2dBGSetup(
        NNS_G2D_BGSELECT_MAIN0,
        pScnData,
        pChrData,
        pPltData,
        GX_BG_SCRBASE_0x7800,
        GX_BG_CHARBASE_0x38000
    );

    NNS_FndFreeToAllocator( &sAllocator, pPltFile );
    NNS_FndFreeToAllocator( &sAllocator, pChrFile );
    NNS_FndFreeToAllocator( &sAllocator, pScnFile );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_Alloc

  Description:  ���������m�ۂ��܂��B

  Arguments:    size:   �m�ۂ��郁�����̃T�C�Y

  Returns:      �m�ۂ����������̈�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
void* TXT_Alloc(u32 size)
{
    return NNS_FndAllocFromAllocator( &sAllocator, size );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_Free

  Description:  TXT_Alloc() �Ŋm�ۂ�����������������܂��B

  Arguments:    ptr:    �J�����郁�����̈�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_Free(void* ptr)
{
    NNS_FndFreeToAllocator( &sAllocator, ptr );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFont

  Description:  �t�H���g���t�@�C�����烁�����Ƀ��[�h���܂��B

  Arguments:    pFname: �t�H���g���\�[�X�̃p�X�B

  Returns:      ���[�h�����t�H���g�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TXT_LoadFont( NNSG2dFont* pFont, const char* pFname )
{
    void* pBuf;

    pBuf = LoadNFTR( pFont, pFname, &sAllocator );
    if( pBuf == NULL )
    {
        OS_Panic("Fail to load font file(%s).", pFname);
    }

    return;
}



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFile

  Description:  �t�@�C�����������Ƀ��[�h���܂��B

  Arguments:    ppFile: �t�@�C�������[�h�����������A�h���X���󂯎��
                        �o�b�t�@�ւ̃|�C���^�B
                fpath:  ���[�h����t�@�C���̃p�X

  Returns:      ���[�h�����t�@�C���̃t�@�C���T�C�Y��Ԃ��܂��B
                0 �̏ꍇ�̓t�@�C�����[�h�Ɏ��s��������\���܂��B
                ���̏ꍇ *ppFile �̒l�͖����ł��B
 *---------------------------------------------------------------------------*/
u32 TXT_LoadFile(void** ppFile, const char* fpath)
{
    return CMN_LoadFile(ppFile, fpath, &sAllocator);
}



/*---------------------------------------------------------------------------*
  Name:         TXT_SetCharCanvasOBJAttrs

  Description:  �A������OAM��NNS_G2dArrangeOBJ* �Őݒ肳��Ȃ��p�����[�^��
                �܂Ƃ߂Đݒ肵�܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_SetCharCanvasOBJAttrs(
    GXOamAttr * oam,
    int num,
    int priority,
    GXOamMode mode,
    BOOL mosaic,
    GXOamEffect effect,
    int cParam,
    int rsParam
)
{
    int i;

    for( i = 0; i < num; i++ )
    {
        G2_SetOBJPriority(oam, priority);
        G2_SetOBJMode(oam, mode, cParam);
        G2_SetOBJEffect(oam, effect, rsParam);
        G2_OBJMosaic(oam, mosaic);

        oam++;
    }
}

