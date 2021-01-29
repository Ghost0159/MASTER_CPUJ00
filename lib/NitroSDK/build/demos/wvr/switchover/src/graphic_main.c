/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - demos - switchover
  File:     graphic_main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: graphic_main.c,v $
  Revision 1.5  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 11:08:57  kitase_hirotake
  OS_PrintServer �̍폜
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.3  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.2  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.1  2005/02/16 12:47:54  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
 * WVR ���C�u�����g�p�̗L���𓮓I�ɐ؂�ւ���T���v���ł�.
 * �{�^���ɂ�藼��� 3D �\�����[�h�ƃ��C�����X�ʐM���[�h��؂�ւ��܂�.
 *
 * ���̃T���v���͈ȉ���2�̃T���v���𑫂����킹�������̂��̂ł�.
 * �X�̃��[�h�ɂ����铮��̏ڍׂ͊e�X�̃\�[�X�R�[�h���Q�Ƃ�������.
 *    /build/demos/wvr/simple
 *    /build/demos/gx/UnitTours/Sub_Double3D
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include    "common.h"


/*****************************************************************************/
/* constant */

/* �����̂̒��_���W */
static const s16 gCubeGeometry[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

/* �����̂̒��_�J���[ */
static const GXRgb gCubeColor[8] = {
    GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 0),
    GX_RGB(31, 0, 31),
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 31),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0)
};


/*****************************************************************************/
/* variable */

/* �L���v�`��������ʂ��r�b�g�}�b�v OBJ �Ƃ��ĕ\�����邽�߂� OAM ��� */
static GXOamAttr sOamBak[128];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         GetPadTrigger

  Description:  �L�[���͏����X�V��, �ŐV�̉����g���K�r�b�g��Ԃ��B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static int GetPadTrigger(void)
{
    static u16 pad_bak = 0;
    const u16 pad_cur = PAD_Read();
    const int trig = (u16)(~pad_bak & pad_cur);
    return (pad_bak = pad_cur), trig;
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V �u�����N���荞�݃n���h�� (DEMO ���C�u�������ݒ�)

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         Color

  Description:  �����̂̎w�蒸�_�J���[��ݒ�

  Arguments:    idx             ���_�C���f�b�N�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void Color(int idx)
{
    G3_Color(gCubeColor[idx]);
}

/*---------------------------------------------------------------------------*
  Name:         Vtx

  Description:  �����̂̎w�蒸�_���W��ݒ�

  Arguments:    idx             ���_�C���f�b�N�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void Vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

/*---------------------------------------------------------------------------*
  Name:         ColVtxQuad

  Description:  �w��̒��_�ɂ����W����ѐF��A���ݒ肵�ċ�`�𐶐�

  Arguments:    idx0            0 �Ԗڂ̒��_�C���f�b�N�X
                idx1            1 �Ԗڂ̒��_�C���f�b�N�X
                idx2            2 �Ԗڂ̒��_�C���f�b�N�X
                idx3            3 �Ԗڂ̒��_�C���f�b�N�X
                bOwnIndexColor  TRUE �Ȃ�e�C���f�b�N�X�̐F���g�p,
                                FALSE �Ȃ�S�� idx0 �̐F���g�p.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColVtxQuad(int idx0, int idx1, int idx2, int idx3, BOOL bOwnIndexColor)
{
    if (bOwnIndexColor)
    {
        Color(idx0);
        Vtx(idx0);
        Color(idx1);
        Vtx(idx1);
        Color(idx2);
        Vtx(idx2);
        Color(idx3);
        Vtx(idx3);
    }
    else
    {
        Color(idx0);
        Vtx(idx0);
        Vtx(idx1);
        Vtx(idx2);
        Vtx(idx3);
    }
}

/*---------------------------------------------------------------------------*
  Name:         drawCube

  Description:  ��ʍ����ɗ����̂�`��

  Arguments:    Rotate          ��]�p�x
                bIsRight        �E�Ȃ� TRUE, ���Ȃ� FALSE

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void drawCube(u16 Rotate, BOOL bIsRight)
{
    G3_PushMtx();
    {
        const int pos = bIsRight ? +3 : -3;

        /* ���s�ړ�����щ�]�ړ� */
        G3_Translate(pos << (FX32_SHIFT - 1), 0, 0);
        {
            const fx16 s = FX_SinIdx(Rotate);
            const fx16 c = FX_CosIdx(Rotate);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /*
         * �ȉ��̐ݒ�Ń}�e���A������у|���S���������w��.
         *   �g�U���ːF  = GX_RGB(31, 31, 31) (���_�F�Ƃ��Ďg�p���Ȃ�)
         *   �����ːF  = GX_RGB(16, 16, 16)
         *   ���ʔ��ːF  = GX_RGB(16, 16, 16) (�e�[�u�����g�p���Ȃ�)
         *   ���ˌ��F    = GX_RGB(0, 0, 0)
         *   ���C�g      = �S����
         *   ���[�h      = ���W�����[�V����
         *   �J�����O    = ���ʔ�\��
         *   �|���S�� ID = 0
         *   �s�����x    = 31
         *   ���̑�      = ����
         */
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(16, 16, 16), FALSE);
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_BACK,
                       0, 31, GX_POLYGON_ATTR_MISC_NONE);

        /* �����̂�`�� */
        G3_Begin(GX_BEGIN_QUADS);
        {
            ColVtxQuad(2, 0, 4, 6, bIsRight);
            ColVtxQuad(7, 5, 1, 3, bIsRight);
            ColVtxQuad(6, 4, 5, 7, bIsRight);
            ColVtxQuad(3, 1, 0, 2, bIsRight);
            ColVtxQuad(5, 4, 0, 1, bIsRight);
            ColVtxQuad(6, 7, 3, 2, bIsRight);
        }
        G3_End();

    }
    G3_PopMtx(1);
}

/*---------------------------------------------------------------------------*
  Name:         setupFrame

  Description:  �t���[���̕`��ݒ��������

  Arguments:    bIsRight        �E�Ȃ� TRUE, ���Ȃ� FALSE

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void setupFrame(BOOL bIsRight)
{
    /*
     * 2 �̉�ʂ� 1 �s�N�`���t���[�����ƂɌ��݂� 3D �G���W�����g�p����.
     * 3D �G���W�����g�p�ł��Ȃ��t���[���ł͑O�t���[���̃L���v�`�����ʂ��g�p����.
     * VRAM-C �ɂ͍���藧���̂�, VRAM-D �ɂ͉E��藧���̂��L���v�`������邪,
     * ���������ꂼ���VRAM �� BG �܂��� OBJ �����ɂ����g�p�ł��Ȃ�����,
     * VRAM-C �ł̓_�C���N�g�r�b�g�}�b�v��, VRAM-D �ł̓e�N�X�`�� OBJ �ŕ\������.
     */

    /*
     * �ȉ��̐ݒ�� LCD ��������.
     *   ���C�� BG ���[�h : 0
     *     BG-0 : 3D (�D��x 0)
     *     BG-1 : -
     *     BG-2 : -
     *     BG-3 : -
     *     OBJ  : -
     */
    (void)GX_ResetBankForSubOBJ();
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);
    GXS_SetGraphicsMode(GX_BGMODE_5);

    if (bIsRight)
    {
        /*
         *   ��Ƀ��C��, ���ɃT�u LCD ��\��
         *   �T�u BG ���[�h : 5
         *     BG-0 : -
         *     BG-1 : -
         *     BG-2 : �_�C���N�g�r�b�g�}�b�v
         *     BG-3 : -
         *     OBJ  : -
         *   �L���v�`�����[�h : (256, 192), 3D �̂� 100%
         *   VRAM-A : -
         *   VRAM-B : -
         *   VRAM-C : �T�u OBJ
         *   VRAM-D : LCDC
         */
        (void)GX_DisableBankForLCDC();
        (void)GX_DisableBankForSubOBJ();
        GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
        GX_SetBankForLCDC(GX_VRAM_LCDC_D);
        GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                      GX_CAPTURE_MODE_A, GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0,
                      GX_CAPTURE_DEST_VRAM_D_0x00000, 16, 0);
        GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
        G2S_SetBG2Priority(0);
        G2S_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256,
                               GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
        G2S_BG2Mosaic(FALSE);
    }
    else
    {
        /*
         *   ���Ƀ��C��, ��ɃT�u LCD ��\��
         *   �T�u BG ���[�h : 5
         *     BG-0 : -
         *     BG-1 : -
         *     BG-2 : -
         *     BG-3 : -
         *     OBJ  : ON
         *   �L���v�`�����[�h : (256, 192), 3D �̂� 100%
         *   VRAM-A : -
         *   VRAM-B : -
         *   VRAM-C : LCDC
         *   VRAM-D : �T�u OBJ
         */
        (void)GX_DisableBankForLCDC();
        (void)GX_DisableBankForSubBG();
        GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                      GX_CAPTURE_MODE_A, GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0,
                      GX_CAPTURE_DEST_VRAM_C_0x00000, 16, 0);
        GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);
    }

    G3X_Reset();

    /* �J�����s��ݒ� */
    {
        const VecFx32 Eye = { 0, 0, FX32_ONE * 5 };
        const VecFx32 at = { 0, 0, 0 };
        const VecFx32 vUp = { 0, FX32_ONE, 0 };
        G3_LookAt(&Eye, &vUp, &at, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         setupSubOAM

  Description:  VRAM-D �̃T�u��ʕ\���p�� OAM ���i�q��ɔz�u�ݒ�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void setupSubOAM(void)
{
    int     i;
    int     x, y;
    int     idx = 0;

    GXS_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_2D_W256);

    for (i = 0; i < 128; ++i)
    {
        sOamBak[i].attr01 = 0;
        sOamBak[i].attr23 = 0;
    }

    for (y = 0; y < 192; y += 64)
    {
        for (x = 0; x < 256; x += 64, idx++)
        {
            G2_SetOBJAttr(&sOamBak[idx], x, y, 0,
                          GX_OAM_MODE_BITMAPOBJ, FALSE,
                          GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16,
                          (y / 8) * 32 + (x / 8), 15, 0);
        }
    }
    DC_FlushRange(&sOamBak[0], sizeof(sOamBak));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(&sOamBak[0], 0, sizeof(sOamBak));
}

/*---------------------------------------------------------------------------*
  Name:         GraphicMain

  Description:  ����� 3D �\�����[�h�̏������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void GraphicMain(void)
{
    GX_DispOff();
    GXS_DispOff();

    /* VRAM, OAM, �p���b�gRAM �̑S�N���A */
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    /* V �u�����N���荞�݃n���h���ݒ� */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /* GX ������ */
    G3X_Init();
    G3X_InitTable();
    G3X_InitMtxStack();

    /* �N�����̂ݕK�v�ȕ`��ݒ�̏����� */
    G3X_AntiAlias(TRUE);
    setupSubOAM();
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);

    /* �v���W�F�N�V�����s��̏����� */
    {
        const fx32 right = FX32_ONE;
        const fx32 top = FX32_ONE * 3 / 4;
        const fx32 near = FX32_ONE;
        const fx32 far = FX32_ONE * 400;
        G3_Perspective(FX32_SIN30, FX32_COS30, FX32_ONE * 4 / 3, near, far, NULL);
        G3_StoreMtx(0);
    }

    /* �`�惋�[�v�̊J�n */
    OS_WaitVBlankIntr();
    GX_DispOn();
    GXS_DispOn();
    {
        u16     Rotate = 0;
        BOOL    bIsRight;

        (void)GetPadTrigger();
        for (bIsRight = TRUE;; bIsRight = !bIsRight)
        {
            if (GetPadTrigger() & PAD_BUTTON_START)
            {
                break;
            }
            /* �t���[���`�� */
            setupFrame(bIsRight);
            drawCube(Rotate, bIsRight);

            Rotate += 256;

            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
            OS_WaitVBlankIntr();
            /*
             * �`�挋�ʂ̓o�b�t�@�X���b�v��� V �u�����N���甽�f�����̂�
             * �Ӑ}����\���� LCD �ݒ�͂��̎��_�Őݒ肷��.
             * �E���̗����̂�����ʂɏo���悤�w�肷��.
             */
            GX_SetDispSelect(bIsRight ? GX_DISP_SELECT_SUB_MAIN : GX_DISP_SELECT_MAIN_SUB);

        }
    }
}
