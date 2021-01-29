/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Screen.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.21 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_Screen.h>
#include <nnsys/g2d/g2d_config.h>
#include "g2di_Dma.h"
#include "g2di_BGManipulator.h"


#define BG_MODE_WARNING 8

// �g���p���b�g�X���b�g�̃T�C�Y
#define NNS_G2D_BGEXTPLTT_SLOTSIZE  0x2000

//---- �e�L�X�gBG�ł̒P�ʃv���[���̃T�C�Y�i�L�����N�^�P�ʁj
#define PLANE_WIDTH     32
#define PLANE_HEIGHT    32


// BG �g���p���b�g�X���b�g��\���񋓎q
typedef enum NNSG2dBGExtPlttSlot
{
    NNS_G2D_BGEXTPLTTSLOT_MAIN0,
    NNS_G2D_BGEXTPLTTSLOT_MAIN1,
    NNS_G2D_BGEXTPLTTSLOT_MAIN2,
    NNS_G2D_BGEXTPLTTSLOT_MAIN3,
    NNS_G2D_BGEXTPLTTSLOT_SUB0,
    NNS_G2D_BGEXTPLTTSLOT_SUB1,
    NNS_G2D_BGEXTPLTTSLOT_SUB2,
    NNS_G2D_BGEXTPLTTSLOT_SUB3

} NNSG2dBGExtPlttSlot;


//---- �X�N���[���T�C�Y���l�����W�X�^�l�ϊ��p
typedef struct ScreenSizeMap
{
    u16 width;
    u16 height;
    u16 scnSize;
}
ScreenSizeMap;


// �e�L�X�gBG�̃X�N���[���T�C�Y�e�[�u��
static const ScreenSizeMap sTextScnSize[4] =
{
    {256, 256, GX_BG_SCRSIZE_TEXT_256x256},
    {256, 512, GX_BG_SCRSIZE_TEXT_256x512},
    {512, 256, GX_BG_SCRSIZE_TEXT_512x256},
    {512, 512, GX_BG_SCRSIZE_TEXT_512x512},
};

// �A�t�B��BG�̃X�N���[���T�C�Y�e�[�u��
static const ScreenSizeMap sAffineScnSize[4] =
{
    {128, 128, GX_BG_SCRSIZE_AFFINE_128x128},
    {256, 256, GX_BG_SCRSIZE_AFFINE_256x256},
    {512, 512, GX_BG_SCRSIZE_AFFINE_512x512},
    {1024, 1024, GX_BG_SCRSIZE_AFFINE_1024x1024},
};

// �A�t�B���g��BG�̃X�N���[���T�C�Y�e�[�u��
static const ScreenSizeMap sAffineExtScnSize[4] =
{
    {128, 128, GX_BG_SCRSIZE_256x16PLTT_128x128},
    {256, 256, GX_BG_SCRSIZE_256x16PLTT_256x256},
    {512, 512, GX_BG_SCRSIZE_256x16PLTT_512x512},
    {1024, 1024, GX_BG_SCRSIZE_256x16PLTT_1024x1024},
};


static const u8 sBGTextModeTable[4][8] =
{
    {
        /* 0 -> */ GX_BGMODE_0,
        /* 1 -> */ GX_BGMODE_1,
        /* 2 -> */ GX_BGMODE_2,
        /* 3 -> */ GX_BGMODE_3,
        /* 4 -> */ GX_BGMODE_4,
        /* 5 -> */ GX_BGMODE_5,
        /* 6 -> */ GX_BGMODE_6,
        /* 7 -> */ BG_MODE_WARNING,                             // �ݒ�֎~
    },
    {
        /* 0 -> */ GX_BGMODE_0,
        /* 1 -> */ GX_BGMODE_1,
        /* 2 -> */ GX_BGMODE_2,
        /* 3 -> */ GX_BGMODE_3,
        /* 4 -> */ GX_BGMODE_4,
        /* 5 -> */ GX_BGMODE_5,
        /* 6 -> */ GX_BGMODE_6,
        /* 7 -> */ BG_MODE_WARNING,                             // �ݒ�֎~
    },
    {
        /* 0 -> */ GX_BGMODE_0,
        /* 1 -> */ GX_BGMODE_1,
        /* 2 -> */ GX_BGMODE_1,
        /* 3 -> */ GX_BGMODE_3,
        /* 4 -> */ GX_BGMODE_3,
        /* 5 -> */ GX_BGMODE_3,
        /* 6 -> */ GX_BGMODE_0,
        /* 7 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // �ݒ�֎~
    },
    {
        /* 0 -> */ GX_BGMODE_0,
        /* 1 -> */ GX_BGMODE_0,
        /* 2 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // BG2:�A�t�B��     -> �e�L�X�g
        /* 3 -> */ GX_BGMODE_0,
        /* 4 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // BG2:�A�t�B��     -> �e�L�X�g
        /* 5 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // BG2:�A�t�B���g�� -> �e�L�X�g
        /* 6 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // BG2:����BMP    -> �e�L�X�g
        /* 7 -> */ (GXBGMode)(GX_BGMODE_0 + BG_MODE_WARNING),   // �ݒ�֎~
    }
};


// BG2 ���A�t�B��BG�ɕύX���鎞��BG���[�h�J�ڃe�[�u��
static const u8 sBGAffineModeTable[2][8] =
{
    {
        /* 0 -> */ (GXBGMode)(GX_BGMODE_2 + BG_MODE_WARNING),   // BG3:�e�L�X�g->�A�t�B��
        /* 1 -> */ GX_BGMODE_2,
        /* 2 -> */ GX_BGMODE_2,
        /* 3 -> */ GX_BGMODE_4,
        /* 4 -> */ GX_BGMODE_4,
        /* 5 -> */ GX_BGMODE_4,
        /* 6 -> */ GX_BGMODE_4,
        /* 7 -> */ (GXBGMode)(GX_BGMODE_2 + BG_MODE_WARNING),   // �ݒ�֎~
    },
    {
        /* 0 -> */ GX_BGMODE_1,
        /* 1 -> */ GX_BGMODE_1,
        /* 2 -> */ GX_BGMODE_2,
        /* 3 -> */ GX_BGMODE_1,
        /* 4 -> */ GX_BGMODE_2,
        /* 5 -> */ (GXBGMode)(GX_BGMODE_1 + BG_MODE_WARNING),   // BG2:�A�t�B���g��->�e�L�X�g
        /* 6 -> */ (GXBGMode)(GX_BGMODE_1 + BG_MODE_WARNING),   // BG2:����->�e�L�X�g
        /* 7 -> */ (GXBGMode)(GX_BGMODE_1 + BG_MODE_WARNING),   // �ݒ�֎~
    }
};
static const u8 sBG256x16PlttModeTable[2][8] =
{
    {
        /* 0 -> */ (GXBGMode)(GX_BGMODE_5 + BG_MODE_WARNING),   // BG3:�e�L�X�g->�A�t�B���g��
        /* 1 -> */ (GXBGMode)(GX_BGMODE_5 + BG_MODE_WARNING),   // BG3:�A�t�B��->�A�t�B���g��
        /* 2 -> */ (GXBGMode)(GX_BGMODE_5 + BG_MODE_WARNING),   // BG3:�A�t�B��->�A�t�B���g��
        /* 3 -> */ GX_BGMODE_5,
        /* 4 -> */ GX_BGMODE_5,
        /* 5 -> */ GX_BGMODE_5,
        /* 6 -> */ GX_BGMODE_5,
        /* 7 -> */ (GXBGMode)(GX_BGMODE_5 + BG_MODE_WARNING),   // �ݒ�֎~
    },
    {
        /* 0 -> */ GX_BGMODE_3,
        /* 1 -> */ GX_BGMODE_3,
        /* 2 -> */ GX_BGMODE_4,
        /* 3 -> */ GX_BGMODE_3,
        /* 4 -> */ GX_BGMODE_4,
        /* 5 -> */ GX_BGMODE_5,
        /* 6 -> */ (GXBGMode)(GX_BGMODE_3 + BG_MODE_WARNING),   // BG2:����->�e�L�X�g
        /* 7 -> */ (GXBGMode)(GX_BGMODE_3 + BG_MODE_WARNING),   // �ݒ�֎~
    }
};



GXBGAreaOver NNSi_G2dBGAreaOver = GX_BG_AREAOVER_XLU;



//****************************************************************************
// static �֐�
//****************************************************************************

//----------------------------------------------------------------------------
// �C�����C���֐�
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         IsMainBGExtPlttSlot

  Description:  NNSG2dBGExtPlttSlot �^�̒l�����C����ʂ̊g���p���b�g�X���b�g��
                �\���Ă��邩���肵�܂��B

  Arguments:    slot:   ����Ώ�

  Returns:      ���C����ʂ̊g���p���b�g�X���b�g��\���Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
static NNS_G2D_INLINE BOOL IsMainBGExtPlttSlot(NNSG2dBGExtPlttSlot slot)
{
    return (slot <= NNS_G2D_BGEXTPLTTSLOT_MAIN3);
}



/*---------------------------------------------------------------------------*
  Name:         CalcTextScreenOffset

  Description:  �e�L�X�gBG�ʂ̎w�肳�ꂽ�ʒu�̃X�N���[���x�[�X�����
                �I�t�Z�b�g���擾���܂��B

  Arguments:    x:  �ʒu�w�� x �i�L�����N�^�P�ʁj
                y:  �ʒu�w�� y �i�L�����N�^�P�ʁj
                w:  �Ώ� BG �ʂ̕��i�L�����N�^�P�ʁj
                h:  �Ώ� BG �ʂ̍����i�L�����N�^�P�ʁj

  Returns:      �w�肳�ꂽ�ʒu�̃I�t�Z�b�g�i�L�����N�^�P�ʁj
 *---------------------------------------------------------------------------*/
static NNS_G2D_INLINE int CalcTextScreenOffset(int x, int y, int w, int h)
{
    const int x_blk  = x / PLANE_WIDTH;
    const int y_blk  = y / PLANE_HEIGHT;
    const int x_char = x % PLANE_WIDTH;
    const int y_char = y % PLANE_HEIGHT;
    const int w_blk  = w / PLANE_WIDTH;
    const int h_blk  = h / PLANE_WIDTH;
    const int blk_w  = (x_blk == w_blk) ? (w % PLANE_WIDTH):  PLANE_WIDTH;
    const int blk_h  = (y_blk == h_blk) ? (h % PLANE_HEIGHT): PLANE_HEIGHT;

    return
        w * PLANE_HEIGHT * y_blk
        + PLANE_WIDTH * blk_h * x_blk
        + blk_w * y_char
        + x_char;
}



/*---------------------------------------------------------------------------*
  Name:         GetCompressedPlttOriginalIndex

  Description:  �w�肳�ꂽ�p���b�g�̈��k�O�̃I�t�Z�b�g���擾���܂��B

  Arguments:    pCmpInfo:   �p���b�g���k���ւ̃|�C���^�B
                idx:        �p���b�g�ԍ��B

  Returns:      �p���b�g�̃I�t�Z�b�g�i�o�C�g�P�ʁj
 *---------------------------------------------------------------------------*/
static NNS_G2D_INLINE u32 GetCompressedPlttOriginalIndex(
    const NNSG2dPaletteCompressInfo* pCmpInfo,
    int idx
)
{
    NNS_G2D_POINTER_ASSERT( pCmpInfo );

    return ( (u16*)(pCmpInfo->pPlttIdxTbl) )[idx];
}



/*---------------------------------------------------------------------------*
  Name:         GetPlttSize

  Description:  �p���b�g�f�[�^��1�p���b�g������̃T�C�Y���擾���܂��B

  Arguments:    pPltData:   �p���b�g�f�[�^�ւ̃|�C���^�B

  Returns:      �p���b�g�̃T�C�Y�i�o�C�g�P�ʁj
 *---------------------------------------------------------------------------*/
static NNS_G2D_INLINE u32 GetPlttSize(const NNSG2dPaletteData* pPltData)
{
    NNS_G2D_POINTER_ASSERT( pPltData );

    switch( pPltData->fmt )
    {
    case GX_TEXFMT_PLTT16:     return sizeof(GXBGPltt16);
    case GX_TEXFMT_PLTT256:    return sizeof(GXBGPltt256);
    default:                    SDK_ASSERTMSG(FALSE, "invalid NNSG2dPaletteData");
    }

    return 0;
}






//----------------------------------------------------------------------------
// �ʏ�֐�
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         SelectScnSize

  Description:  �^����ꂽ�e�[�u�����Q�Ƃ��ăX�N���[���T�C�Y��ϊ����܂��B
                �w�肳�ꂽ�X�N���[���T�C�Y�ȏ�ōŏ���BG�T�C�Y��Ԃ��܂��B
                �w�肳�ꂽ�X�N���[���T�C�Y�ȏ��BG�����݂��Ȃ��ꍇ��
                �ő��BG�T�C�Y��Ԃ��܂��B

  Arguments:    tbl:    �X�N���[���T�C�Y�ϊ��e�[�u��
                w:      �s�N�Z���P�ʂ̃X�N���[����
                h:      �s�N�Z���P�ʂ̃X�N���[����

  Returns:      �Ή�����X�N���[���T�C�Y�f�[�^�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
static const ScreenSizeMap* SelectScnSize(const ScreenSizeMap tbl[4], int w, int h)
{
    int i;
    SDK_NULL_ASSERT(tbl);

    for( i = 0; i < 4; i++ )
    {
        if( w <= tbl[i].width && h <= tbl[i].height )
        {
            return &tbl[i];
        }
    }
    return &tbl[3];
}



/*---------------------------------------------------------------------------*
  Name:         ChangeBGModeByTable*

  Description:  �e�[�u���ɏ]����BG���[�h��؂�ւ��܂��B

  Arguments:    modeTable:  ���[�h�J�ڃe�[�u��

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ChangeBGModeByTableMain(const u8 modeTable[])
{
    GXBGMode mode = (GXBGMode)modeTable[GetBGModeMain()];
    GXBG0As bg0as = IsBG03D() ? GX_BG0_AS_3D: GX_BG0_AS_2D;
    SDK_NULL_ASSERT(modeTable);

    // ����BG�Ɏx�Ⴊ�o�郂�[�h�ύX���x��
    if( mode >= BG_MODE_WARNING )
    {
        mode -= BG_MODE_WARNING;
        OS_Warning("Dangerous Main BG mode change: %d => %d", GetBGModeMain(), mode);
    }

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, mode, bg0as);
}

static void ChangeBGModeByTableSub(const u8 modeTable[])
{
    GXBGMode mode = (GXBGMode)modeTable[GetBGModeSub()];
    SDK_NULL_ASSERT(modeTable);

    // ����BG�Ɏx�Ⴊ�o�郂�[�h�ύX���x��
    if( mode >= BG_MODE_WARNING )
    {
        mode -= BG_MODE_WARNING;
        OS_Warning("Dangerous Sub BG mode change: %d => %d", GetBGModeSub(), mode);
    }

    GXS_SetGraphicsMode(mode);
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGPlttToExtendedPltt

  Description:  �w�肳�ꂽ�g���p���b�g�X���b�g�Ƀp���b�g�f�[�^��ǂݍ��݂܂��B

  Arguments:    slot:       �p���b�g�f�[�^��ǂݍ��ފg���p���b�g�X���b�g
                pPltData:   �p���b�g�f�[�^�ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadBGPlttToExtendedPltt(
    NNSG2dBGExtPlttSlot slot,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dPaletteCompressInfo* pCmpInfo
)
{
    void (*prepairLoad)(void);
    void (*cleanupLoad)(void);
    void (*loader)(const void*, u32, u32);
    u32 offset = (u32)( slot * NNS_G2D_BGEXTPLTT_SLOTSIZE );
    SDK_NULL_ASSERT(pPltData);

    // �L���b�V���t���b�V��
    DC_FlushRange(pPltData->pRawData, pPltData->szByte);

    if( IsMainBGExtPlttSlot(slot) )
    {
        prepairLoad = GX_BeginLoadBGExtPltt;
        cleanupLoad = GX_EndLoadBGExtPltt;
        loader      = GX_LoadBGExtPltt;
    }
    else
    {
        offset -= NNS_G2D_BGEXTPLTT_SLOTSIZE * NNS_G2D_BGEXTPLTTSLOT_SUB0;

        prepairLoad = GXS_BeginLoadBGExtPltt;
        cleanupLoad = GXS_EndLoadBGExtPltt;
        loader      = GXS_LoadBGExtPltt;
    }

    if( pCmpInfo != NULL )
    {
        const u32 szOnePltt = GetPlttSize( pPltData );
        const int numIdx = pCmpInfo->numPalette;
        int i;

        for( i = 0; i < numIdx; i++ )
        {
        	const u32 offsetAddr = GetCompressedPlttOriginalIndex(pCmpInfo, i) * szOnePltt;
            const void* pSrc = (u8*)pPltData->pRawData + szOnePltt * i;

            prepairLoad();
            loader(pSrc, offset + offsetAddr, szOnePltt);
            cleanupLoad();
        }
    }
    else
    {
        prepairLoad();
        loader(pPltData->pRawData, offset, pPltData->szByte);
        cleanupLoad();
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGPlttToNormalPltt

  Description:  �W���p���b�g�Ƀp���b�g�f�[�^��ǂݍ��݂܂��B

  Arguments:    bMainDisplay:   �Ǎ���Ώۂ� BG �ʁB
                pPltData:       �p���b�g�f�[�^�ւ̃|�C���^

  Returns:
 *---------------------------------------------------------------------------*/
static void LoadBGPlttToNormalPltt(
    NNSG2dBGSelect bg,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dPaletteCompressInfo* pCmpInfo
)
{
    u8* pDst;
    SDK_NULL_ASSERT(pPltData);

    // �L���b�V���t���b�V��
    DC_FlushRange(pPltData->pRawData, pPltData->szByte);

    pDst = (u8*)( IsMainBG(bg) ? HW_BG_PLTT: HW_DB_BG_PLTT );

    if( pCmpInfo != NULL )
    {
        const u32 szOnePltt = GetPlttSize( pPltData );
        const int numIdx = pCmpInfo->numPalette;
        int i;

        for( i = 0; i < numIdx; i++ )
        {
        	const u32 offsetAddr = GetCompressedPlttOriginalIndex(pCmpInfo, i) * szOnePltt;
            const void* pSrc = (u8*)pPltData->pRawData + szOnePltt * i;

            NNSi_G2dDmaCopy16(GXi_DmaId, pSrc, pDst + offsetAddr, szOnePltt);
        }
    }
    else
    {
        NNSi_G2dDmaCopy16(GXi_DmaId, pPltData->pRawData, pDst, pPltData->szByte);
    }
}



/*---------------------------------------------------------------------------*
  Name:         GetBGExtPlttSlot

  Description:  �w�肳�ꂽ BG ���g�p����g���p���b�g�X���b�g���擾���܂��B

  Arguments:    bg: �g�p����g���p���b�g�X���b�g�ԍ����擾������ BG

  Returns:      bg ���g�p����g���p���b�g�X���b�g�ԍ�
 *---------------------------------------------------------------------------*/
static NNSG2dBGExtPlttSlot GetBGExtPlttSlot(NNSG2dBGSelect bg)
{
    // NNSG2dBGSelect => BGxCNT���W�X�^�I�t�Z�b�g �̕ϊ��e�[�u��
    static const u16 addrTable[] =
    {
        REG_BG0CNT_OFFSET,
        REG_BG1CNT_OFFSET,
        0,                      // �g�p����X���b�g�͌Œ�
        0,                      // �g�p����X���b�g�͌Œ�
        REG_DB_BG0CNT_OFFSET,
        REG_DB_BG1CNT_OFFSET,
        0,                      // �g�p����X���b�g�͌Œ�
        0                       // �g�p����X���b�g�͌Œ�
    };
    u32 addr;
    NNSG2dBGExtPlttSlot slot = (NNSG2dBGExtPlttSlot)bg;
        // ��{�� (int)slot == (int)bg

    NNS_G2D_BG_ASSERT(bg);
    addr = addrTable[bg];

    if( addr != 0 )
    {
        addr += HW_REG_BASE;

        if( (*(u16*)addr & REG_G2_BG0CNT_BGPLTTSLOT_MASK) != 0 )
        {
            // BGPLTTSLOT�r�b�g�������Ă���ꍇ��
            // (int)slot == (int)bg + 2
            slot += 2;
        }
    }

    return slot;
}



/*---------------------------------------------------------------------------*
  Name:         SetBGnControlTo*

  Description:  BGxCNT ���W�X�^�̐ݒ�� BG ���[�h�̕ύX���s���܂��B

  Arguments:    n:          �Ώۂ� BG �ʁB
                size:       �X�N���[���T�C�Y�B
                cmode:      �J���[���[�h�B
                areaOver:   �G���A�I�[�o�[�����B
                scnBase:    BG �̃X�N���[���x�[�X�u���b�N�B
                chrBase:    BG �̃L�����N�^�x�[�X�u���b�N�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetBGnControlToText(
    NNSG2dBGSelect n,
    GXBGScrSizeText size,
    GXBGColorMode cmode,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    const int bgNo = GetBGNo(n);
    GXBGExtPltt extPltt = GX_BG_EXTPLTT_01;
    if( IsMainBG(n) )
    {
        if( ! IsMainBGExtPltt01Available() ) extPltt = GX_BG_EXTPLTT_23;
        ChangeBGModeByTableMain(sBGTextModeTable[bgNo]);
    }
    else
    {
        ChangeBGModeByTableSub(sBGTextModeTable[bgNo]);
    }
    SetBGnControlText(n, size, cmode, scnBase, chrBase, extPltt);
}
static void SetBGnControlToAffine(
    NNSG2dBGSelect n,
    GXBGScrSizeAffine size,
    GXBGAreaOver areaOver,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    if( IsMainBG(n) )
    {
        ChangeBGModeByTableMain(sBGAffineModeTable[n-2]);
    }
    else
    {
        ChangeBGModeByTableSub(sBGAffineModeTable[n-6]);
    }
    SetBGnControlAffine(n, size, areaOver, scnBase, chrBase);
}
static void SetBGnControlTo256x16Pltt(
    NNSG2dBGSelect n,
    GXBGScrSize256x16Pltt size,
    GXBGAreaOver areaOver,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    if( IsMainBG(n) )
    {
        ChangeBGModeByTableMain(sBG256x16PlttModeTable[n-2]);
    }
    else
    {
        ChangeBGModeByTableSub(sBG256x16PlttModeTable[n-6]);
    }
    SetBGnControl256x16Pltt(n, size, areaOver, scnBase, chrBase);
}



/*---------------------------------------------------------------------------*
  Name:         BGAutoControlText

  Description:  �X�N���[���f�[�^�ɏ]���� BGxCNT ���W�X�^���e�L�X�gBG�Ƃ���
                �ݒ肵�܂��B������ BG ���[�h�̕ύX���s���܂��B

  Arguments:    bg:         �R���g���[���Ώۂ� BG�B
                pScnData:   �X�N���[���f�[�^�ւ̃|�C���^�B
                scnBase:    BG �̃X�N���[���x�[�X
                chrBase:    BG �̃L�����N�^�x�[�X

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetBGControlText(
    NNSG2dBGSelect bg,
    GXBGColorMode colorMode,
    int screenWidth,
    int screenHeight,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    const ScreenSizeMap* pSizeMap;

    NNS_G2D_BG_ASSERT(bg);

    pSizeMap = SelectScnSize(sTextScnSize, screenWidth, screenHeight);
    SDK_ASSERTMSG( pSizeMap != NULL,
        "Unsupported screen size(%dx%d) in input screen data",
        screenWidth,
        screenHeight
    );

    SetBGnControlToText(bg, (GXBGScrSizeText)pSizeMap->scnSize, colorMode, scnBase, chrBase);
}



/*---------------------------------------------------------------------------*
  Name:         BGAutoControlAffine

  Description:  �X�N���[���f�[�^�ɏ]���� BGxCNT ���W�X�^���A�t�B��BG�Ƃ���
                �ݒ肵�܂��B������ BG ���[�h�̕ύX���s���܂��B

  Arguments:    bg:         �R���g���[���Ώۂ� BG�B
                pScnData:   �X�N���[���f�[�^�ւ̃|�C���^�B
                scnBase:    BG �̃X�N���[���x�[�X
                chrBase:    BG �̃L�����N�^�x�[�X

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetBGControlAffine(
    NNSG2dBGSelect bg,
    int screenWidth,
    int screenHeight,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    const ScreenSizeMap* pSizeMap;

    NNS_G2D_BG_ASSERT(bg);
    SDK_ASSERTMSG( (bg % 4) >= 2,
        "You can not show affine BG on %s BG %d",
        (IsMainBG(bg) ? "Main": "Sub"),
        (IsMainBG(bg) ? bg: bg - NNS_G2D_BGSELECT_SUB0) );

    pSizeMap = SelectScnSize(sAffineScnSize, screenWidth, screenHeight);
    SDK_ASSERTMSG( pSizeMap != NULL,
        "Unsupported screen size(%dx%d) in input screen data",
        screenWidth,
        screenHeight
    );

    SetBGnControlToAffine(bg, (GXBGScrSizeAffine)pSizeMap->scnSize, NNSi_G2dBGAreaOver, scnBase, chrBase);
}



/*---------------------------------------------------------------------------*
  Name:         BGAutoControlAffineExt

  Description:  �X�N���[���f�[�^�ɏ]���� BGxCNT ���W�X�^���A�t�B���g��BG�Ƃ���
                �ݒ肵�܂��B������ BG ���[�h�̕ύX���s���܂��B

  Arguments:    bg:         �R���g���[���Ώۂ� BG�B
                pScnData:   �X�N���[���f�[�^�ւ̃|�C���^�B
                scnBase:    BG �̃X�N���[���x�[�X
                chrBase:    BG �̃L�����N�^�x�[�X

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetBGControl256x16Pltt(
    NNSG2dBGSelect bg,
    int screenWidth,
    int screenHeight,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    const ScreenSizeMap* pSizeMap;

    NNS_G2D_BG_ASSERT(bg);
    SDK_ASSERTMSG( (bg % 4) >= 2,
        "You can not show affine BG on %s BG %d",
        (IsMainBG(bg) ? "Main": "Sub"),
        (IsMainBG(bg) ? bg: bg - NNS_G2D_BGSELECT_SUB0) );

    pSizeMap = SelectScnSize(sAffineExtScnSize, screenWidth, screenHeight);
    SDK_ASSERTMSG( pSizeMap != NULL,
        "Unsupported screen size(%dx%d) in input screen data",
        screenWidth,
        screenHeight
    );

    SetBGnControlTo256x16Pltt(bg, (GXBGScrSize256x16Pltt)pSizeMap->scnSize, NNSi_G2dBGAreaOver, scnBase, chrBase);
}



/*---------------------------------------------------------------------------*
  Name:         IsBGExtPlttSlotAssigned

  Description:  �w�肳�ꂽ�g���p���b�g�X���b�g�� VRAM �����蓖�Ă��Ă��邩
                ���肵�܂��B

  Arguments:    slot:   ����Ώۂ̊g���p���b�g�X���b�g

  Returns:      VRAM �����蓖�Ă��Ă���Ȃ� TRUE�B
 *---------------------------------------------------------------------------*/
static BOOL IsBGExtPlttSlotAssigned(NNSG2dBGExtPlttSlot slot)
{
    if( IsMainBGExtPlttSlot(slot) )
    {
        if( slot <= NNS_G2D_BGEXTPLTTSLOT_MAIN1 )
        {
            return IsMainBGExtPltt01Available();
        }
        else
        {
            return IsMainBGExtPltt23Available();
        }
    }
    else
    {
        return IsSubBGExtPlttAvailable();
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGPaletteSelect

  Description:  �p���b�g�f�[�^�� VRAM �ɓǂݍ��݂܂��B
                �p���b�g���g�p���� BG �ƁA�g���p���b�g���g�����ǂ�����
                �ǂݍ��ݐ���w�肵�܂��B

  Arguments:    bg:         �ǂݍ��܂���p���b�g�f�[�^���g�p���� BG
                bToExtPltt: TRUE �Ȃ�g���p���b�g�ցAFALSE �Ȃ�W���p���b�g��
                            �ǂݍ��݂܂��B
                pPltData:   �p���b�g�f�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadBGPaletteSelect(
    NNSG2dBGSelect bg,
    BOOL bToExtPltt,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dPaletteCompressInfo* pCmpInfo
)
{
    NNS_G2D_BG_ASSERT(bg);
    SDK_NULL_ASSERT(pPltData);

    if( bToExtPltt )
    {
        NNSG2dBGExtPlttSlot slot = GetBGExtPlttSlot(bg);

        SDK_ASSERT( IsBGExtPlttSlotAssigned(slot) );
        LoadBGPlttToExtendedPltt(slot, pPltData, pCmpInfo);
    }
    else
    {
        LoadBGPlttToNormalPltt(bg, pPltData, pCmpInfo);
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGPalette

  Description:  �p���b�g�f�[�^�� VRAM �ɓǂݍ��݂܂��B
                �p���b�g���g�p���� BG �� G2D�X�N���[���f�[�^����
                �ǂݍ��ݐ���������肵�܂��B

  Arguments:    bg:         �ǂݍ��܂���p���b�g�f�[�^���g�p���� BG
                pPltData:   �p���b�g�f�[�^�ւ̃|�C���^�B
                pScnData:   �ǂݍ��܂���p���b�g�f�[�^���g�p����
                            �X�N���[���f�[�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadBGPalette(
    NNSG2dBGSelect bg,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dScreenData* pScnData,
    const NNSG2dPaletteCompressInfo* pCmpInfo
)
{
/*
    �g���p���b�g    |�L��           ����
      �Ή��X���b�g  |�L��   ����    -
    -------------------------------------
    text    16x16   |�W��   �W��    �W��
            256x1   |�g��   (�g��)  �W��
            256x16  |�g��   (�g��)  [�W��]
    affine  256x1   |�W��   �W��    �W��
    extend  256x16  |�g��   (�g��)  [�W��]

    () �t�͓��ߐF�ȊO�����ׂč��ɂȂ�
    [] �t�̓p���b�g�w�肪���������
    ���� ASSERT �Œ�~
*/
    // TODO: �g���p���b�g�g�p���ł��o�b�N�h���b�v�F�ɂ͕W���p���b�g���g����
    //*(GXRgb*)(HW_BG_PLTT) = pPltData->pRawData[0];

    NNS_G2D_BG_ASSERT(bg);
    SDK_NULL_ASSERT(pPltData);
    SDK_NULL_ASSERT(pScnData);

    if( (pScnData->screenFormat == NNS_G2D_SCREENFORMAT_TEXT)
        && (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_256x1) )
    {
        LoadBGPaletteSelect(bg, IsBGUseExtPltt(bg), pPltData, pCmpInfo);
    }
    else
    {
        BOOL bExtPlttData = ! (
            (pPltData->fmt == GX_TEXFMT_PLTT16)
            || (pScnData->screenFormat == NNS_G2D_SCREENFORMAT_AFFINE)
        );

        SDK_ASSERTMSG( ! bExtPlttData || IsBGUseExtPltt(bg),
            "Input screen data requires extended BG palette, but unavailable");
        LoadBGPaletteSelect(bg, bExtPlttData, pPltData, pCmpInfo);
    }
}




/*---------------------------------------------------------------------------*
  Name:         LoadBGCharacter

  Description:  �L�����N�^�f�[�^��Ώۂ�BG�p�Ƀ��[�h���܂��B

  Arguments:    bg:         �Ώۂ� BG �ʁB
                pChrData:   �L�����N�^�f�[�^�ւ̃|�C���^�B
                pPosInfo:   �L�����N�^�ʒu���ւ̃|�C���^�B
                            pChrData �������L�����N�^�łȂ��ꍇ�� NULL ���w��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadBGCharacter(
    NNSG2dBGSelect bg,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dCharacterPosInfo* pPosInfo
)
{
    u32 offset = 0;

    NNS_G2D_BG_ASSERT(bg);
    SDK_NULL_ASSERT(pChrData);
    SDK_ASSERT( pPosInfo == NULL || pPosInfo->srcPosX == 0 );

    //---- �����L�����N�^�̏ꍇ�̓I�t�Z�b�g�Z�o
    if( pPosInfo != NULL )
    {
        int offsetChars = pPosInfo->srcPosY * pPosInfo->srcW;
        u32 szChar = (pChrData->pixelFmt == GX_TEXFMT_PLTT256) ?
                        sizeof(GXCharFmt256): sizeof(GXCharFmt16);

        offset = offsetChars * szChar;
    }

    DC_FlushRange(pChrData->pRawData, pChrData->szByte);
    LoadBGnChar(bg, pChrData->pRawData, offset, pChrData->szByte);
}



/*---------------------------------------------------------------------------*
  Name:         LoadBGScreen

  Description:  �X�N���[���f�[�^��Ώۂ�BG�p�Ƀ��[�h���܂��B

  Arguments:    bg:         �Ώۂ� BG �ʁB
                pScnData:   ���[�h����X�N���[���f�[�^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadBGScreen(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData
)
{
    GXScrFmtText* pDstBase;
    int plane_cwidth;
    int plane_cheight;
    int load_cwidth;
    int load_cheight;

    NNS_G2D_BG_ASSERT(bg);
    SDK_NULL_ASSERT(pScnData);

    //---- �X�N���[���x�[�X�擾
    pDstBase = (GXScrFmtText*)GetBGnScrPtr(bg);

    //---- �]���\�ȃT�C�Y���Z�o
    {
        const int scn_cwidth  = pScnData->screenWidth / 8;
        const int scn_cheight = pScnData->screenHeight / 8;

        NNSi_G2dBGGetCharSize(&plane_cwidth, &plane_cheight, bg);
        load_cwidth = (plane_cwidth > scn_cwidth) ? scn_cwidth: plane_cwidth;
        load_cheight = (plane_cheight > scn_cheight) ? scn_cheight: plane_cheight;
    }


    //---- �]��
    DC_FlushRange( (void*)pScnData->rawData, pScnData->szByte );
    NNS_G2dBGLoadScreenRect(
        pDstBase,
        pScnData,
        0, 0,
        0, 0,
        plane_cwidth, plane_cheight,
        load_cwidth, load_cheight
    );
}






/*---------------------------------------------------------------------------*
  Name:         SetBGControlAuto

  Description:  �Ώۂ� BG �ʂ� BG �R���g���[�����s���܂��B

  Arguments:    bg:             �Ώۂ� BG �ʁB
                screenFormat:   BG�^�C�v�B
                colorMode:      �J���[���[�h
                screenWidth:    �X�N���[�����i�s�N�Z���P�ʁj
                screenHeight:   �X�N���[�����i�s�N�Z���P�ʁj
                schBase:        �X�N���[���x�[�X�u���b�N�B
                chrBase:        �L�����N�^�x�[�X�u���b�N�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetBGControlAuto(
    NNSG2dBGSelect bg,
    NNSG2dScreenFormat screenFormat,
    GXBGColorMode colorMode,
    int screenWidth,
    int screenHeight,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    // BG Control
    switch( screenFormat )
    {
    case NNS_G2D_SCREENFORMAT_TEXT:
        SetBGControlText(bg, colorMode, screenWidth, screenHeight, scnBase, chrBase);
        break;

    case NNS_G2D_SCREENFORMAT_AFFINE:
        SetBGControlAffine(bg, screenWidth, screenHeight, scnBase, chrBase);
        break;

    case NNS_G2D_SCREENFORMAT_AFFINEEXT:
        SetBGControl256x16Pltt(bg, screenWidth, screenHeight, scnBase, chrBase);
        break;

    default:
        SDK_ASSERTMSG(FALSE, "TEXT, AFFINE, and 256x16 format support only");
        break;
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadScreenPartText

  Description:  �X�N���[���f�[�^���̎w�肳�ꂽ��`���o�b�t�@�̎w�肳�ꂽ�ʒu��
                �R�s�[���܂��B

  Arguments:    pScreenDst: �]�����_�ւ̃|�C���^�B
                pScnData:   �]�����ƂȂ�X�N���[���f�[�^�ւ̃|�C���^�B
                srcX:       �]�����������X���W�B�i�L�����N�^�P�ʁj
                srcY:       �]�����������Y���W�B�i�L�����N�^�P�ʁj
                dstX:       �]���捶�����X���W�B�i�L�����N�^�P�ʁj
                dstY:       �]���捶�����Y���W�B�i�L�����N�^�P�ʁj
                dstW:       �]����̈�̕��B�i�L�����N�^�P�ʁj
                dstH:       �]����̈�̍����B�i�L�����N�^�P�ʁj
                width:      �]������̈�̕��B�i�L�����N�^�P�ʁj
                height:     �]������̈�̍����B�i�L�����N�^�P�ʁj

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadScreenPartText(
    void* pScreenDst,
    const NNSG2dScreenData* pScnData,
    int srcX,
    int srcY,
    int dstX,
    int dstY,
    int dstW,
    int dstH,
    int width,
    int height
)
{
    NNS_G2D_POINTER_ASSERT( pScreenDst );
    NNS_G2D_POINTER_ASSERT( pScnData );

    //---- 1�L�������Ƀ}�b�s���O
    {
        const int src_x_end             = srcX + width;
        const int src_y_end             = srcY + height;
        const int src_next_offset       = pScnData->screenWidth - width;
        const int dst_next_offset       = dstW - width;
        const u32 szLine                = sizeof(GXScrFmtText) * width;
        const int srcW                  = pScnData->screenWidth / 8;
        const int srcH                  = pScnData->screenHeight / 8;
        const GXScrFmtText* pSrcBase    = (const GXScrFmtText*)pScnData->rawData;
        GXScrFmtText* pDstBase          = (GXScrFmtText*)pScreenDst;
        int sx, sy;
        int dx, dy;

        for( sy = srcY, dy = dstY; sy < src_y_end; sy++, dy++ )
        {
            for( sx = srcX, dx = dstX; sx < src_x_end; sx++, dx++ )
            {
                const GXScrFmtText* pSrc = pSrcBase + CalcTextScreenOffset(sx, sy, srcW, srcH);
                GXScrFmtText* pDst = pDstBase + CalcTextScreenOffset(dx, dy, dstW, dstH);

                *pDst = *pSrc;
            }
        }
    }
}




/*---------------------------------------------------------------------------*
  Name:         LoadScreenPartAffine

  Description:  �X�N���[���f�[�^���̎w�肳�ꂽ��`���o�b�t�@�̎w�肳�ꂽ�ʒu��
                �R�s�[���܂��B

  Arguments:    pScreenDst: �]�����_�ւ̃|�C���^�B
                pScnData:   �]�����ƂȂ�X�N���[���f�[�^�ւ̃|�C���^�B
                srcX:       �]�����������X���W�B�i�L�����N�^�P�ʁj
                srcY:       �]�����������Y���W�B�i�L�����N�^�P�ʁj
                dstX:       �]���捶�����X���W�B�i�L�����N�^�P�ʁj
                dstY:       �]���捶�����Y���W�B�i�L�����N�^�P�ʁj
                dstW:       �]����̈�̕��B�i�L�����N�^�P�ʁj
                width:      �]������̈�̕��B�i�L�����N�^�P�ʁj
                height:     �]������̈�̍����B�i�L�����N�^�P�ʁj

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadScreenPartAffine(
    void* pScreenDst,
    const NNSG2dScreenData* pScnData,
    int srcX,
    int srcY,
    int dstX,
    int dstY,
    int dstW,
    int width,
    int height
)
{
    NNS_G2D_POINTER_ASSERT( pScreenDst );
    NNS_G2D_POINTER_ASSERT( pScnData );

    //---- �P���ɋ�`��]��
    {
        const int src_y_end        = srcY + height;
        const int srcW             = pScnData->screenWidth / 8;
        const u32 szLine           = sizeof(GXScrFmtAffine) * width;
        const GXScrFmtAffine* pSrc = (const GXScrFmtAffine*)pScnData->rawData;
        GXScrFmtAffine*       pDst = (GXScrFmtAffine*)pScreenDst;
        int y;

        pSrc += srcY * srcW + srcX;
        pDst += dstY * dstW + dstX;

        for( y = srcY; y < src_y_end; y++ )
        {
            MI_CpuCopy8(pSrc, pDst, szLine);
            pSrc += srcW;
            pDst += dstW;
        }
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadScreenPart256x16Pltt

  Description:  �X�N���[���f�[�^���̎w�肳�ꂽ��`���o�b�t�@�̎w�肳�ꂽ�ʒu��
                �R�s�[���܂��B

  Arguments:    pScreenDst: �]�����_�ւ̃|�C���^�B
                pScnData:   �]�����ƂȂ�X�N���[���f�[�^�ւ̃|�C���^�B
                srcX:       �]�����������X���W�B�i�L�����N�^�P�ʁj
                srcY:       �]�����������Y���W�B�i�L�����N�^�P�ʁj
                dstX:       �]���捶�����X���W�B�i�L�����N�^�P�ʁj
                dstY:       �]���捶�����Y���W�B�i�L�����N�^�P�ʁj
                dstW:       �]����̈�̕��B�i�L�����N�^�P�ʁj
                width:      �]������̈�̕��B�i�L�����N�^�P�ʁj
                height:     �]������̈�̍����B�i�L�����N�^�P�ʁj

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadScreenPart256x16Pltt(
    void* pScreenDst,
    const NNSG2dScreenData* pScnData,
    int srcX,
    int srcY,
    int dstX,
    int dstY,
    int dstW,
    int width,
    int height
)
{
    NNS_G2D_POINTER_ASSERT( pScreenDst );
    NNS_G2D_POINTER_ASSERT( pScnData );

    //---- �P���ɋ�`��]��
    {
        const int src_y_end      = srcY + height;
        const int srcW           = pScnData->screenWidth / 8;
        const u32 szLine         = sizeof(GXScrFmtText) * width;
        const GXScrFmtText* pSrc = (const GXScrFmtText*)pScnData->rawData;
        GXScrFmtText*       pDst = (GXScrFmtText*)pScreenDst;
        int y;


        pSrc += srcY * srcW + srcX;
        pDst += dstY * dstW + dstX;

        for( y = srcY; y < src_y_end; y++ )
        {
            MI_CpuCopy16(pSrc, pDst, szLine);
            pSrc += srcW;
            pDst += dstW;
        }
    }
}












//****************************************************************************
// ���J�֐�
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGLoadElements

  Description:  BG �ŗp����O���t�B�b�N�X�f�[�^��K�؂�VRAM�ɓǂݍ��݂܂��B
                �Ώ� BG �� BGControl ���K�؂ɐݒ肳��Ă���K�v������܂��B
                BG0-1�̊g���p���b�g�̓Ǎ���͊g���p���b�g�ւ� VRAM ��
                ���蓖�ď󋵂��玩���I�ɔ��f���܂��B
                Text BG �� 256x1 �p���b�g�͊g���p���b�g���L�����ǂ�����
                ���[�h���؂�ւ��܂��B

  Arguments:    bg:         ���[�h����f�[�^���g�p���� BG
                pScnData:   VRAM �Ƀ��[�h����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł��\���܂��񂪁ApPltData �� NULL �łȂ�
                            �ꍇ�� NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   VRAM �Ƀ��[�h����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   VRAM �Ƀ��[�h����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPosInfo:   �L�����N�^���o�̈���ւ̃|�C���^�B
                pCmpInfo:   �p���b�g���k���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGLoadElementsEx(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dCharacterPosInfo* pPosInfo,
    const NNSG2dPaletteCompressInfo* pCmpInfo
)
{
    NNS_G2D_BG_ASSERT(bg);
    NNS_G2D_ASSERT( pPltData == NULL || pScnData != NULL );
    NNS_G2D_ASSERT( pPosInfo == NULL || pChrData != NULL );
    NNS_G2D_ASSERT( pCmpInfo == NULL || pPltData != NULL );

    NNS_G2D_ASSERTMSG( pChrData == NULL || pScnData == NULL
                || ( (pChrData->pixelFmt == GX_TEXFMT_PLTT16)
                    == (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_16x16) ),
                "Color mode mismatch between character data and screen data" );
    NNS_G2D_ASSERTMSG( pPltData == NULL
                || ( (pPltData->fmt == GX_TEXFMT_PLTT16)
                    == (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_16x16) ),
                "Color mode mismatch between palette data and screen data" );
    NNS_G2D_ASSERT( pChrData == NULL
                || ( NNSi_G2dGetCharacterFmtType( pChrData->characterFmt )
                    == NNS_G2D_CHARACTER_FMT_CHAR ) );

    if( pPltData != NULL && pScnData != NULL )
    {
        LoadBGPalette(bg, pPltData, pScnData, pCmpInfo);
    }
    if( pChrData != NULL )
    {
        LoadBGCharacter(bg, pChrData, pPosInfo);
    }
    if( pScnData != NULL )
    {
        LoadBGScreen(bg, pScnData);
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGSetupEx

  Description:  BG�̐ݒ���s���ABG�f�[�^��VRAM�ɓǂݍ��݂܂��B

  Arguments:    bg:         �ΏۂƂ��� BG
                pScnData:   BG �Ŏg�p����X�N���[���f�[�^�ւ̃|�C���^�B
                            NULL �ł����Ă͂Ȃ�܂���B
                pChrData:   BG �Ŏg�p����L�����N�^�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPltData:   BG �Ŏg�p����p���b�g�f�[�^�ւ̃|�C���^�B
                            NULL �̏ꍇ��VRAM�ɓǂݍ��݂܂���B
                pPosInfo:   �L�����N�^���o�̈���ւ̃|�C���^�B
                pCmpInfo:   �p���b�g���k���ւ̃|�C���^�B
                scnBase:    BG �̃X�N���[���x�[�X�B
                chrBase:    BG �̃L�����N�^�x�[�X�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGSetupEx(
    NNSG2dBGSelect bg,
    const NNSG2dScreenData* pScnData,
    const NNSG2dCharacterData* pChrData,
    const NNSG2dPaletteData* pPltData,
    const NNSG2dCharacterPosInfo* pPosInfo,
    const NNSG2dPaletteCompressInfo* pCmpInfo,
    GXBGScrBase scnBase,
    GXBGCharBase chrBase
)
{
    NNS_G2D_BG_ASSERT(bg);
    NNS_G2D_NULL_ASSERT( pScnData );
    NNS_G2D_ASSERT( pPosInfo == NULL || pChrData != NULL );
    NNS_G2D_ASSERT( pCmpInfo == NULL || pPltData != NULL );

    NNS_G2D_ASSERTMSG( pChrData == NULL
                || ( (pChrData->pixelFmt == GX_TEXFMT_PLTT16)
                    == (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_16x16) ),
                "Color mode mismatch between character data and screen data" );
    NNS_G2D_ASSERTMSG( pPltData == NULL
                || ( (pPltData->fmt == GX_TEXFMT_PLTT16)
                    == (pScnData->colorMode == NNS_G2D_SCREENCOLORMODE_16x16) ),
                "Color mode mismatch between palette data and screen data" );
    NNS_G2D_ASSERT( pChrData == NULL
                || ( NNSi_G2dGetCharacterFmtType( pChrData->characterFmt )
                    == NNS_G2D_CHARACTER_FMT_CHAR ) );

    SetBGControlAuto(
        bg,
        NNSi_G2dBGGetScreenFormat(pScnData),
        NNSi_G2dBGGetScreenColorMode(pScnData),
        pScnData->screenWidth,
        pScnData->screenHeight,
        scnBase,
        chrBase );
    NNS_G2dBGLoadElementsEx(bg, pScnData, pChrData, pPltData, pPosInfo, pCmpInfo);
}




/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBGLoadScreenRect

  Description:  �X�N���[���f�[�^���̎w�肳�ꂽ��`���o�b�t�@�̎w�肳�ꂽ�ʒu��
                �R�s�[���܂��B

  Arguments:    pScreenDst: �]�����_�ւ̃|�C���^�B
                pScnData:   �]�����ƂȂ�X�N���[���f�[�^�ւ̃|�C���^�B
                srcX:       �]�����������X���W�B�i�L�����N�^�P�ʁj
                srcY:       �]�����������Y���W�B�i�L�����N�^�P�ʁj
                dstX:       �]���捶�����X���W�B�i�L�����N�^�P�ʁj
                dstY:       �]���捶�����Y���W�B�i�L�����N�^�P�ʁj
                dstW:       �]����̈�̕��B�i�L�����N�^�P�ʁj
                dstH:       �]����̈�̍����B�i�L�����N�^�P�ʁj
                width:      �]������̈�̕��B�i�L�����N�^�P�ʁj
                height:     �]������̈�̍����B�i�L�����N�^�P�ʁj

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dBGLoadScreenRect(
    void* pScreenDst,
    const NNSG2dScreenData* pScnData,
    int srcX,
    int srcY,
    int dstX,
    int dstY,
    int dstW,
    int dstH,
    int width,
    int height
)
{
    NNS_G2D_POINTER_ASSERT( pScreenDst );
    NNS_G2D_POINTER_ASSERT( pScnData );

    // ���͌�/�o�͐�͈̔͂𒴂����̈�͉������Ȃ��悤�Ƀp�����[�^�𒲐߂���
    if( dstX < 0 )
    {
        const int adj = - dstX;
        srcX  += adj;
        width -= adj;
        dstX   = 0;
    }
    if( dstY < 0 )
    {
        const int adj = - dstY;
        srcY   += adj;
        height -= adj;
        dstY    = 0;
    }
    if( dstX + width > dstW )
    {
        const int adj = (dstX + width) - dstW;
        width -= adj;
    }
    if( dstY + height > dstH )
    {
        const int adj = (dstY + height) - dstH;
        height -= adj;
    }
    if( srcX < 0 )
    {
        const int adj = - srcX;
        dstX  += adj;
        width -= adj;
        srcX   = 0;
    }
    if( srcY < 0 )
    {
        const int adj = - srcY;
        dstY   += adj;
        height -= adj;
        srcY    = 0;
    }
    if( srcX + width > pScnData->screenWidth / 8 )
    {
        const int adj = (srcX + width) - (pScnData->screenWidth / 8);
        width -= adj;
    }
    if( srcY + height > pScnData->screenHeight / 8 )
    {
        const int adj = (srcY + height) - (pScnData->screenHeight / 8);
        height -= adj;
    }

    if( width <= 0 || height <= 0 )
    {
        return;
    }

    switch( pScnData->screenFormat )
    {
    case NNS_G2D_SCREENFORMAT_TEXT:
        LoadScreenPartText(pScreenDst, pScnData, srcX, srcY, dstX, dstY, dstW, dstH, width, height);
        break;

    case NNS_G2D_SCREENFORMAT_AFFINE:
        LoadScreenPartAffine(pScreenDst, pScnData, srcX, srcY, dstX, dstY, dstW, width, height);
        break;

    case NNS_G2D_SCREENFORMAT_AFFINEEXT:
        LoadScreenPart256x16Pltt(pScreenDst, pScnData, srcX, srcY, dstX, dstY, dstW, width, height);
        break;

    default:
        NNS_G2D_ASSERTMSG(FALSE, "Unknown screen format(=%d)", pScnData->screenFormat );
    }
}


