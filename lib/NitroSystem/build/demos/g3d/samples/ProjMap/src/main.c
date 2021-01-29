/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ProjMap
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// ���e�}�b�v���ꂽ�I�u�W�F�N�g��\������T���v��
//
// .imd�t�@�C����<material>�v�f��tex_gen_mode��pos�ɐݒ肳��Ă���ꍇ�A
// ���e�}�b�v���g�p���ăe�N�X�`�����}�b�s���O���܂��B
//
// �T���v���̓���
// �T���v���ł́A�e�N�X�`����xy���W��(-1.0, -1.0)����(1.0, 1.0)�̍��W��
// ���ˉe�����悤�ɂȂ��Ă��܂�(effect_mtx���P�ʍs��̏ꍇ)�B
// �I�u�W�F�N�g��X���W��-1.0����1.0�܂ł̊Ԃ���Ɉړ����Ă��܂��B
//
// A�{�^���������� --> �`��O�̎ˉe�s��̐ݒ���@���؂�ւ��܂��B
//                     �ǂ̕��@�ł������悤�Ƀe�N�X�`�����}�b�s���O����܂��B
// X�{�^���������� --> X��(�r���[���W�n��)�𒆐S�Ɋ��}�b�v����]���܂��B
// Y�{�^���������� --> Y��(�r���[���W�n��)�𒆐S�Ɋ��}�b�v����]���܂��B
// B�{�^���������� --> Z��(�r���[���W�n��)�𒆐S�Ɋ��}�b�v����]���܂��B
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void             InitG3DLib(void);
static NNSG3dResMdl*    LoadG3DModel(const char* path);
static void             SetCamera(G3DDemoCamera* camera);


/*-------------------------------------------------------------------------
    rotEnvMap

    ���}�b�v�̕�����ς��鏈�����s���R�[���o�b�N�֐�
  -------------------------------------------------------------------------*/
static void
rotProjMap(NNSG3dRS* rs)
{
    static MtxFx33 mRot = {
        FX32_ONE, 0, 0,
        0, FX32_ONE, 0,
        0, 0, FX32_ONE
    };
    MtxFx33 m;

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_B))
    {
        MTX_RotZ33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_Y))
    {
        MTX_RotY33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_X))
    {
        MTX_RotX33(&m, FX_SinIdx(256), FX_CosIdx(256));
        MTX_Concat33(&m, &mRot, &mRot);
    }

    // effect_mtx�������ō쐬���������邱�Ƃɂ��A���e�����e�N�X�`���̕�����ς��邱�Ƃ��ł���B
    NNS_G3dGeMultMtx33(&mRot);

    // effect_mtx�̍쐬������u��������̂Ńt���O�𗧂ĂĂ����B
    // ��������ƁATiming B����Timing C�܂ł̊Ԃɂ��郉�C�u�������̏������X�L�b�v�ł���B
    rs->flag |= NNS_G3D_RSFLAG_SKIP;
}


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
    NNSG3dResMdl*    model;
    int state = 0;
    u16 sincos = 0;
    fx32 X = -FX32_ONE;

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

    model = LoadG3DModel("data/sphere_proj.nsbmd");
    SDK_ASSERTMSG(model, "load failed");

    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&object, model);

    G3DDemo_InitCamera(&gCamera, 0, 5*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ProjMap");

    //
    // effect_mtx�̐ݒ���J�X�^�}�C�Y����ꍇ�́A
    // NNS_G3D_SBC_ENVMAP�Ƀ^�C�~���O�a��
    // �R�[���o�b�N����������B
    //
    NNS_G3dRenderObjSetCallBack(&object,
                                &rotProjMap,
                                NULL,
                                NNS_G3D_SBC_PRJMAP,
                                NNS_G3D_SBC_CALLBACK_TIMING_B);

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

        {
            //
            // �I�u�W�F�N�g�͏�ɉ�]���Ă���
            //
            MtxFx33 mRot;
            VecFx32 mTrans;
            sincos += 256;

            mTrans.x = X;
            mTrans.y = 0;
            mTrans.z = 0;
            
            MTX_RotX33(&mRot, FX_SinIdx(sincos), FX_CosIdx(sincos));
            NNS_G3dGlbSetBaseRot(&mRot);
            NNS_G3dGlbSetBaseTrans(&mTrans);

            X += FX32_ONE >> 8;
            if (X >= FX32_ONE)
                X = -FX32_ONE;
            
            //
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
            {
                state = (state + 1) % 3;
            }

            //
            // A�{�^�����������Ƃɂ��ˉe�s��̃Z�b�g�A�b�v���@���؂�ւ��
            // (�o�Ă���G�͓���)�B
            //
            switch(state)
            {
            case 0:
                NNS_G3dGlbFlushP();
                break;
            case 1:
                NNS_G3dGlbFlushVP();
                break;
            case 2:
                NNS_G3dGlbFlushWVP();
                break;
            };

            time = OS_GetTick();

            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
                //
                NNS_G3dDraw(&object);
            }

            // ���ԑ���B
            time = OS_GetTick() - time;
        }

        {
            // ��]�����ɖ߂�
            MtxFx33 mRot;
            VecFx32 mTrans = {0,0,0};
            MTX_Identity33(&mRot);
            NNS_G3dGlbSetBaseRot(&mRot);
            NNS_G3dGlbSetBaseTrans(&mTrans);

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

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
        switch(state)
        {
        case 0:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushP  ");
            break;
        case 1:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushVP ");
            break;
        case 2:
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_YELLOW, "NNS_G3dGlbFlushWVP");
            break;
        };
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
  Name:         LoadG3DModel

  Description:  �t�@�C�����烂�f���f�[�^��ǂݍ��݂܂��B

  Arguments:    path: ���f���f�[�^�̃p�X���B

  Returns:      ���f���ւ̃|�C���^�B
   ------------------------------------------------------------------------- */
static NNSG3dResMdl*
LoadG3DModel(const char* path)
{
    NNSG3dResMdl*        model   = NULL;
    NNSG3dResTex*        texture = NULL;
    NNSG3dResFileHeader* resFile = G3DDemo_LoadFile(path);
    BOOL status;
    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
    // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
    // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
    // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
    DC_StoreRange(resFile, resFile->fileSize);

    // �f�t�H���g�̏������֐����R�[�����ăZ�b�g�A�b�v����
    status = NNS_G3dResDefaultSetup(resFile);
    NNS_G3D_ASSERTMSG(status, "NNS_G3dResDefaultSetup failed");

    //
    // G3D: ���f���̎擾
    // nsbmd�̓��f���𕡐��܂ނ��Ƃ��ł���̂ŁA�C���f�b�N�X(���f�����P�̏ꍇ��0)
    // ���w�肵�ĂP�̃��f���ւ̃|�C���^���擾����B
    // --------------------------------------------------------------------
    model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 0);

    return model;
}


/* -------------------------------------------------------------------------
  Name:         SetCamera

  Description:  G3D�̃O���[�o����ԂɃJ�����s��Ǝˉe�s���ݒ肵�܂��B

  Arguments:    camera:		G3DDemoCamera�\���̂ւ̃|�C���^�B

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
