/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - 1mat1shp
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// ���ɒP���ȃ��f���������ɕ\������T���v��
//
// NNS_G3dDraw1Mat1Shp���g�p���邱�Ƃɂ��A�w�肳�ꂽ�}�e���A��ID�ƃV�F�C�vID
// �̑g�������ɕ`�悵�܂��B
//
// �T���v���̓���
// A�{�^�����������Ƃɂ��ANNS_G3dDraw1MatShp�ɂ��`���NNS_G3dDraw�ɂ��`��
// ��؂�ւ��܂��B
//
// HOWTO:
// 1: ���f���̃��\�[�X�t�@�C���i.nsbmd�t�@�C���j�����C���������Ƀ��[�h���܂��B
// 2: NNS_G3dResDefaultSetup�֐��Ń��[�h�������\�[�X�ɑ΂��ăf�t�H���g�̃Z�b�g�A�b�v����
//    ���s���܂��B
// 3: ���f�����\�[�X����g�p���������f�����ANNS_G3dGetMdlSet�֐���NNS_G3dGetMdlByIdx�֐���
//    �g�p���Ď擾���܂��B
// 4: NNS_G3dGlbXXXX�֐����g�p���āA���C�g�̌�����F�A�ˉe�s��A�J�����s��A���f����
//    �x�[�XSRT����ݒ肵�܂��B
// 5: NNS_G3dGlbFlush�֐����g�p����5�Őݒ肵���l���W�I���g���G���W���ɂ܂Ƃ߂đ��M���܂��B
// 6: NNS_G3dDraw1Mat1Shp�֐����g�p���āA�P���ȃ��f���������ɕ`�悵�܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void           InitG3DLib(void);
static NNSG3dResMdl*  LoadG3DModel(const char* path);
static void           SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    NNSG3dRenderObj object;
    NNSG3dResMdl*   pModel;
    void*           pResFile;
    BOOL            is1Mat1Shp = TRUE;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

    // �}�l�[�W�����e�N�X�`���C���[�W�X���b�g��4�X���b�g���Ǘ��ł���悤�ɂ���
    // �f�t�H���g�}�l�[�W���ɂ���B
    NNS_GfdInitFrmTexVramManager(4, TRUE);

    // �}�l�[�W�����p���b�g��32KB���Ǘ��ł���悤�ɂ���
    // �f�t�H���g�}�l�[�W���ɂ���B
    NNS_GfdInitFrmPlttVramManager(0x8000, TRUE);

    {
        //
        // ���f�����\�[�X�̃��[�h
        //
        pResFile = G3DDemo_LoadFile("data/cube_n_t.nsbmd");
        SDK_ASSERTMSG(pResFile, "load failed");

        // NNS_G3dResDefaultSetup��NNS_G3dDraw*�̒���DMA�]�����g�p���邽�߁A
        // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
        // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
        // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
        DC_StoreRange(pResFile, ((NNSG3dResFileHeader*)pResFile)->fileSize);

        //
        // �f�t�H���g�̏������֐����R�[�����ăZ�b�g�A�b�v����
        //
        if (!NNS_G3dResDefaultSetup(pResFile))
        {
            NNS_G3D_ASSERTMSG(0, "NNS_G3dResDefaultSetup failed");
        }

        //
        // ���f�����\�[�X���̃��f���Z�b�g����C���f�b�N�X#0�̃��f�����擾
        //
        pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
        SDK_ASSERTMSG(pModel, "model not found");
    }

    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNS_G3dDraw1Mat1Shp�ɂ͕K�v����܂��񂪁A�ʏ�̕`����@�Ɣ�r���邽�߂Ɏg���܂��B
    //
    NNS_G3dRenderObjInit(&object, pModel);

    G3DDemo_InitCamera(&gCamera, 0*FX32_ONE, 120*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "1MAT1SHP");

    for(;;)
    {
        OSTick time;

        SVC_WaitVBlankIntr();
        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);

        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            is1Mat1Shp = is1Mat1Shp ? FALSE : TRUE;
        }
        if (is1Mat1Shp)
        {
            G3DDemo_Printf(0,1, G3DDEMO_COLOR_GREEN, "NNS_G3dDraw1Mat1Shp");
        }
        else
        {
            G3DDemo_Printf(0,1, G3DDEMO_COLOR_GREEN, "NNS_G3dDraw        ");
        }

        {
            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlush();

            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw1Mat1Shp
                //
                // �}�e���A��ID�ƃV�F�C�vID�����[�U�[���w�肵�ĕ`�悵�܂��B
                // �A�j���[�V�����̓T�|�[�g����Ă��܂���B
                // �Ō�̈�����FALSE�ɂ���ƃ}�e���A���̑��M���ȗ����܂��B
                // (�������A���f�����\�[�X�̃}�e���A���𑀍삷�邱�Ƃɂ����
                //  �}�e���A���A�j���[�V�����̌��ʂ𓾂邱�Ƃ��ł��܂��B
                //  model.h�ƃT���v����ModifyNsbmd���Q�Ƃ��Ă�������)
                //
                int i, j;
                BOOL sendMaterial = TRUE;
                VecFx32 trans = {0, 0, 0};

                //
                // �e�N�X�`���t�{�b�N�X��200���ׂ�
                //
                for (i = 0; i < 20; ++i)
                {
                    trans.z = (i - 10) * FX32_ONE * 7;
                    for (j = 0; j < 10; ++j)
                    {
                        trans.x = (j - 5) * FX32_ONE * 7;

                        // �P���ȃI�u�W�F�N�g�Ȃ̂ŁANNS_G3dGlbFlush�͎g�킸�ɁA
                        // �J�����g�s�񂾂����Z�b�g���Ă��܂��Ă��悢�B
                        NNS_G3dGeLoadMtx43(&NNS_G3dGlb.cameraMtx);
                        NNS_G3dGeTranslateVec(&trans);

                        if (is1Mat1Shp)
                        {
                            // 2�ڈȍ~�̓}�e���A�������W�I���g���G���W���ɑ��M���Ȃ��B
                            NNS_G3dDraw1Mat1Shp(pModel, 0, 0, sendMaterial);
                            sendMaterial = FALSE;
                        }
                        else
                        {
                            NNS_G3dDraw(&object);
                        }
                    }
                }
            }
            time = OS_GetTick() - time;
        }

        {
            VecFx32 trans = {0, 0, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            //
            // G3D: �ʏ��G3_XXX���ĂԂƂ��́A���̑O��NNS_G3dGeComFlushBuffer()
            //      ���R�[�����ē������Ȃ��Ă͂Ȃ�Ȃ�
            //
            NNS_G3dGlbFlush();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        /* �W�I���g���������_�����O�G���W���֘A�������̃X���b�v */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
    }
}


/* -------------------------------------------------------------------------
  Name:         InitG3DLib

  Description:  G3D���C�u���������������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
static void
InitG3DLib(void)
{
    //
    // G3D:
    // �f�t�H���g�̏��������s��
    // ------------------------------------------------------------------------
    NNS_G3dInit();
}


/* -------------------------------------------------------------------------
  Name:         SetCamera

  Description:  G3D�̃O���[�o����ԂɃJ�����s��Ǝˉe�s���ݒ肵�܂��B

  Arguments:    camera:     G3DDemoCamera�\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
static void
SetCamera(G3DDemoCamera* camera)
{
    G3DDemoLookAt* lookat = &camera->lookat;
    G3DDemoPersp*  persp  = &camera->persp;

    NNS_G3dGlbPerspective(persp->fovySin, persp->fovyCos, persp->aspect, persp->nearClip, persp->farClip);
    NNS_G3dGlbLookAt(&lookat->camPos, &lookat->camUp, &lookat->target);
}
