/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2di_BGManipulator.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/
#ifndef G2DI_BGMANIPULATOR_H_
#define G2DI_BGMANIPULATOR_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_Screen.h>
#include "g2di_Dma.h"

#ifdef __cplusplus
extern "C" {
#endif




/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dBGGetCharSize

  Description:  �Ώ� BG �ʂ̃L�����N�^�P�ʂł̃T�C�Y���擾���܂��B

  Arguments:    pWidth:     BG �ʂ̕����i�[����o�b�t�@�ւ̃|�C���^�B
                pHeight:    BG �ʂ̍������i�[����o�b�t�@�ւ̃|�C���^�B
                n:          �ΏۂƂ��� BG �ʁB

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dBGGetCharSize(int* pWidth, int* pHeight, NNSG2dBGSelect n);



/*---------------------------------------------------------------------------*
  Name:         GetBGNo

  Description:  �Ώ�BG�ʂ� BG �ԍ����擾���܂��B

  Arguments:    n:          �Ώۂ� BG �ʁB

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int GetBGNo(NNSG2dBGSelect n)
{
    NNS_G2D_BG_ASSERT( n );
    return n & 3;
}

/*---------------------------------------------------------------------------*
  Name:         GetBGnCNT

  Description:  �Ώۂ� BG �ʂ𐧌䂷�� BGnCNT ���W�X�^�ւ̃|�C���^���擾���܂��B

  Arguments:    n:          �ΏۂƂ��� BG �ʁB

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE REGType16v* GetBGnCNT(NNSG2dBGSelect n)
{
    extern REGType16v* const NNSiG2dBGCNTTable[];

    NNS_G2D_BG_ASSERT( n );
    return NNSiG2dBGCNTTable[n];
}

/*---------------------------------------------------------------------------*
  Name:         IsBG03D

  Description:  ���C����� BG0 ��3D�Ɋ��蓖�Ă��Ă��邩�ǂ������ׂ܂��B

  Arguments:    �Ȃ��B

  Returns:      3D�Ɋ��蓖�Ă��Ă���Ȃ�TRUE�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL IsBG03D( void )
{
    return (reg_GX_DISPCNT & REG_GX_DISPCNT_BG02D3D_MASK) != 0;
}

/*---------------------------------------------------------------------------*
  Name:         IsMainBG

  Description:  NNSG2dBGSelect �^�̒l�� ���C����� BG ��\���Ă��邩
                ���肵�܂��B

  Arguments:    bg: ����Ώ�

  Returns:      ���C����� BG ��\���Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL IsMainBG(NNSG2dBGSelect bg)
{
    NNS_G2D_BG_ASSERT( bg );
    return ( bg <= NNS_G2D_BGSELECT_MAIN3 );
}

/*---------------------------------------------------------------------------*
  Name:         GetBGMode*

  Description:  ���݂�BG���[�h���擾���܂��B

  Arguments:    �Ȃ��B

  Returns:      ���݂�BG���[�h�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE GXBGMode GetBGModeMain( void )
{
    return (GXBGMode)((reg_GX_DISPCNT & REG_GX_DISPCNT_BGMODE_MASK) >> REG_GX_DISPCNT_BGMODE_SHIFT);
}

NNS_G2D_INLINE GXBGMode GetBGModeSub( void )
{
    return (GXBGMode)((reg_GXS_DB_DISPCNT & REG_GXS_DB_DISPCNT_BGMODE_MASK) >> REG_GXS_DB_DISPCNT_BGMODE_SHIFT);
}

/*---------------------------------------------------------------------------*
  Name:         IsBGUseExtPltt*

  Description:  �Ώۂ� BG �ʂ�S������O���t�B�b�N�G���W����
                �g���p���b�g���g�p���邩�ǂ������肵�܂��B

  Arguments:    bg: ����Ώۂ� BG �ʁB

  Returns:      �g���p���b�g���L���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL IsBGUseExtPlttMain( void )
{
    return (reg_GX_DISPCNT & REG_GX_DISPCNT_BG_MASK) != 0;
}

NNS_G2D_INLINE BOOL IsBGUseExtPlttSub( void )
{
    return (reg_GXS_DB_DISPCNT & REG_GXS_DB_DISPCNT_BG_MASK) != 0;
}

NNS_G2D_INLINE BOOL IsBGUseExtPltt(NNSG2dBGSelect bg)
{
    return IsMainBG(bg) ? IsBGUseExtPlttMain(): IsBGUseExtPlttSub();
}

/*---------------------------------------------------------------------------*
  Name:         IsSubBGExtPlttAvailable

  Description:  �T�u��ʂ̊g���p���b�g�� VRAM �����蓖�Ă��Ă��邩
                ���肵�܂��B
                �T�u��ʂł�0123�S�X���b�g�Ɋ��蓖�Ă邩�A�S�����蓖�ĂȂ�����
                �ǂ��炩���� VRAM �̊��蓖�Ă��s���܂���B

  Arguments:    �Ȃ��B

  Returns:      VRAM �����蓖�Ă��Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL IsSubBGExtPlttAvailable( void )
{
    return GX_GetBankForSubBGExtPltt() != GX_VRAM_SUB_BGEXTPLTT_NONE;
}

/*---------------------------------------------------------------------------*
  Name:         IsMainBGExtPltt01Available

  Description:  ���C����ʂ̊g���p���b�g�X���b�g01�� VRAM �����蓖�Ă���
                ���邩���肵�܂��B

  Arguments:    �Ȃ��B

  Returns:      VRAM �����蓖�Ă��Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL IsMainBGExtPltt01Available( void )
{
    GXVRamBGExtPltt pltt = GX_GetBankForBGExtPltt();

    return (pltt == GX_VRAM_BGEXTPLTT_01_F)
            || (pltt == GX_VRAM_BGEXTPLTT_0123_E)
            || (pltt == GX_VRAM_BGEXTPLTT_0123_FG);
}

/*---------------------------------------------------------------------------*
  Name:         IsMainBGExtPltt23Available

  Description:  ���C����ʂ̊g���p���b�g�X���b�g23�� VRAM �����蓖�Ă���
                ���邩���肵�܂��B

  Arguments:    �Ȃ��B

  Returns:      VRAM �����蓖�Ă��Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
static BOOL IsMainBGExtPltt23Available( void )
{
    GXVRamBGExtPltt pltt = GX_GetBankForBGExtPltt();

    return (pltt == GX_VRAM_BGEXTPLTT_23_G)
            || (pltt == GX_VRAM_BGEXTPLTT_0123_E)
            || (pltt == GX_VRAM_BGEXTPLTT_0123_FG);
}

/*---------------------------------------------------------------------------*
  Name:         MakeBGnCNTVal*

  Description:  BGnCNT ���W�X�^�ɐݒ肷�邽�߂̒l���쐬���܂��B

  Arguments:    screenSize: BG�ʂ̃T�C�Y
                areaOver:   �G���A�I�[�o�[����
                colorMode:  �J���[���[�h
                screenBase: �X�N���[���x�[�X�I�t�Z�b�g
                charBase:   �L�����N�^�x�[�X�I�t�Z�b�g
                bgExtPltt:  �g���p���b�g�X���b�g

  Returns:     �쐬�����l�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 MakeBGnCNTValText(
    GXBGScrSizeText screenSize,
    GXBGColorMode colorMode,
    GXBGScrBase screenBase,
    GXBGCharBase charBase,
    GXBGExtPltt bgExtPltt
)
{
    GX_BG_SCRSIZE_TEXT_ASSERT(screenSize);
    GX_BG_COLORMODE_ASSERT(colorMode);
    GX_BG_SCRBASE_ASSERT(screenBase);
    GX_BG_CHARBASE_ASSERT(charBase);
    GX_BG_EXTPLTT_ASSERT(bgExtPltt);

    return (u16)(
        (screenSize   << REG_G2_BG0CNT_SCREENSIZE_SHIFT)
        | (screenBase << REG_G2_BG0CNT_SCREENBASE_SHIFT)
        | (charBase   << REG_G2_BG0CNT_CHARBASE_SHIFT)
        | (bgExtPltt  << REG_G2_BG0CNT_BGPLTTSLOT_SHIFT)
        | (colorMode  << REG_G2_BG0CNT_COLORMODE_SHIFT)
    );
}
NNS_G2D_INLINE u16 MakeBGnCNTValAffine(
    GXBGScrSizeAffine screenSize,
    GXBGAreaOver areaOver,
    GXBGScrBase screenBase,
    GXBGCharBase charBase
)
{
    GX_BG_SCRSIZE_AFFINE_ASSERT(screenSize);
    GX_BG_AREAOVER_ASSERT(areaOver);
    GX_BG_SCRBASE_ASSERT(screenBase);
    GX_BG_CHARBASE_ASSERT(charBase);

    return (u16)(
        (screenSize   << REG_G2_BG2CNT_SCREENSIZE_SHIFT)
        | (screenBase << REG_G2_BG2CNT_SCREENBASE_SHIFT)
        | (charBase   << REG_G2_BG2CNT_CHARBASE_SHIFT)
        | (areaOver   << REG_G2_BG2CNT_AREAOVER_SHIFT)
    );
}
NNS_G2D_INLINE u16 MakeBGnCNTVal256x16Pltt(
    GXBGScrSize256x16Pltt screenSize,
    GXBGAreaOver areaOver,
    GXBGScrBase screenBase,
    GXBGCharBase charBase
)
{
    GX_BG_SCRSIZE_256x16PLTT_ASSERT(screenSize);
    GX_BG_AREAOVER_ASSERT(areaOver);
    GX_BG_SCRBASE_ASSERT(screenBase);
    GX_BG_CHARBASE_ASSERT(charBase);

    return (u16)(
        (screenSize   << REG_G2_BG2CNT_SCREENSIZE_SHIFT)
        | (screenBase << REG_G2_BG2CNT_SCREENBASE_SHIFT)
        | (charBase   << REG_G2_BG2CNT_CHARBASE_SHIFT)
        | (areaOver   << REG_G2_BG2CNT_AREAOVER_SHIFT)
        | GX_BG_EXTMODE_256x16PLTT
    );
}

/*---------------------------------------------------------------------------*
  Name:         GetBG*Offset

  Description:  ���C����ʂ̃X�N���[��or�L�����N�^�x�[�X�I�t�Z�b�g���擾���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int GetBGCharOffset(void)
{
    return (int)(0x10000 * ((reg_GX_DISPCNT & REG_GX_DISPCNT_BGCHAROFFSET_MASK) >>
                            REG_GX_DISPCNT_BGCHAROFFSET_SHIFT));
}

NNS_G2D_INLINE int GetBGScrOffset(void)
{
    return (int)(0x10000 * ((reg_GX_DISPCNT & REG_GX_DISPCNT_BGSCREENOFFSET_MASK) >>
                            REG_GX_DISPCNT_BGSCREENOFFSET_SHIFT));
}

/*---------------------------------------------------------------------------*
  Name:         GetBGn*Ptr

  Description:  �Ώ� BG �ʂ̃X�N���[��or�L�����N�^�x�[�X�ւ̃|�C���^��
                �擾���܂��B

  Arguments:    n:      �ΏۂƂȂ� BG �ʁB

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void* GetBGnCharPtr(NNSG2dBGSelect n)
{
    const int baseBlock = 0x4000 * ((*GetBGnCNT(n) & REG_G2_BG0CNT_CHARBASE_MASK) >>
                             REG_G2_BG0CNT_CHARBASE_SHIFT);

    return (void *)((IsMainBG(n) ? (HW_BG_VRAM + GetBGCharOffset()): HW_DB_BG_VRAM) + baseBlock);
}

NNS_G2D_INLINE void* GetBGnScrPtr(NNSG2dBGSelect n)
{
    const int baseBlock = 0x800 * ((*GetBGnCNT(n) & REG_G2_BG0CNT_SCREENBASE_MASK) >>
                             REG_G2_BG0CNT_SCREENBASE_SHIFT);

    return (void *)((IsMainBG(n) ? (HW_BG_VRAM + GetBGScrOffset()): HW_DB_BG_VRAM) + baseBlock);
}

/*---------------------------------------------------------------------------*
  Name:         LoadBGnChar

  Description:  �Ώ� BG �ʂ̃L�����N�^�f�[�^�����[�h���܂��B

  Arguments:    n:      �ΏۂƂȂ� BG �ʁB
                pSrc:   ���[�h����f�[�^�ւ̃|�C���^�B
                offset: ���[�h��̃L�����N�^�x�[�X����̃I�t�Z�b�g
                szByte: ���[�h����f�[�^�̃T�C�Y�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void LoadBGnChar(NNSG2dBGSelect n, const void *pSrc, u32 offset, u32 szByte)
{
    u32 ptr;

    NNS_G2D_POINTER_ASSERT( pSrc );
    ptr = (u32)GetBGnCharPtr(n);

    NNSi_G2dDmaCopy16(GXi_DmaId, pSrc, (void *)(ptr + offset), szByte);
}

/*---------------------------------------------------------------------------*
  Name:         SetBGnControl*

  Description:  �Ώ� BG �ʂ� BGControl ���s���܂��B

  Arguments:    n:          �Ώۂ� BG �ʎw��B
                screenSize: �X�N���[���T�C�Y�B
                colorMode:  �J���[���[�h�B
                areaOver:   �G���A�I�[�o�[�����B
                screenBase: �X�N���[���x�[�X�u���b�N�B
                charBase:   �L�����N�^�x�[�X�u���b�N�B
                bgExtPltt:  �g���p���b�g�X���b�g�I���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void SetBGnControlText(
    NNSG2dBGSelect n,
    GXBGScrSizeText screenSize,
    GXBGColorMode colorMode,
    GXBGScrBase screenBase,
    GXBGCharBase charBase,
    GXBGExtPltt bgExtPltt
)
{
    *GetBGnCNT(n) = (u16)(
        (*GetBGnCNT(n) & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BG0CNT_MOSAIC_MASK))
        | MakeBGnCNTValText(screenSize, colorMode, screenBase, charBase, bgExtPltt)
    );
}
NNS_G2D_INLINE void SetBGnControlAffine(
    NNSG2dBGSelect n,
    GXBGScrSizeAffine screenSize,
    GXBGAreaOver areaOver,
    GXBGScrBase screenBase,
    GXBGCharBase charBase
)
{
    *GetBGnCNT(n) = (u16)(
        (*GetBGnCNT(n) & (REG_G2_BG2CNT_PRIORITY_MASK | REG_G2_BG2CNT_MOSAIC_MASK))
        | MakeBGnCNTValAffine(screenSize, areaOver, screenBase, charBase)
    );
}
NNS_G2D_INLINE void SetBGnControl256x16Pltt(
    NNSG2dBGSelect n,
    GXBGScrSize256x16Pltt screenSize,
    GXBGAreaOver areaOver,
    GXBGScrBase screenBase,
    GXBGCharBase charBase
)
{
    *GetBGnCNT(n) = (u16)(
        (*GetBGnCNT(n) & (REG_G2_BG2CNT_PRIORITY_MASK | REG_G2_BG2CNT_MOSAIC_MASK))
        | MakeBGnCNTVal256x16Pltt(screenSize, areaOver, screenBase, charBase)
    );
}








#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2DI_BGMANIPULATOR_H_

