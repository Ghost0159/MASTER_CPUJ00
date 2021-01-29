/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - CameraMtx
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// Projection�s���Position/Vector�s��̃Z�b�g�A�b�v���@�ɂ��ẴT���v��
//
// G3D�ł́ANNS_G3dDraw�Ń��f����`�悷��O�ɁANNS_G3dGlbFlushP, NNS_G3dGlbFlushVP
// NNS_G3dGlbFlushWVP�ɂ���āAProjection�s��APosition/Vector�s��̃J�����g�s��
// ���Z�b�g�A�b�v����̂����ʂł��B
// ��L�R�̊֐��̈Ⴂ�́AProjection�s��APosition/Vector�s��ɉ����Z�b�g���邩�ɂȂ�܂��B
// NNS_G3dGlbFlushP�֐��ł́AProjection�s��Ɏˉe�ϊ��s�񂪁APosition/Vector�s���
// �J�����s��ƃI�u�W�F�N�g��BaseSRT�s�񂪍������ꂽ���̂��Z�b�g����܂��B
// NNS_G3dGlbFlushVP�֐��ł́AProjection�s��Ɏˉe�ϊ��s��ƃJ�����s�񂪍������ꂽ���̂��A
// Position/Vector�s��ɃI�u�W�F�N�g��BaseSRT�s�񂪃Z�b�g����܂��B
// NNS_G3dGlbFlushWVP�֐��ł́AProjection�s��Ɏˉe�ϊ��s��ƃJ�����s���
// �I�u�W�F�N�g��BaseSRT�s�񂪍������ꂽ���̂��APosition/Vector�s��ɂ�
// �P�ʍs�񂪃Z�b�g����܂��B
//
// ���̂悤�ɕ����̍s��Z�b�g�A�b�v���@���T�|�[�g���Ă���͈̂ȉ��̂Q�̗��R����ł��B
// 1: Position/Vector�s������o���ĉ��炩�̉��H���s�������ꍇ�A�J�������
//    �ł̍��W���~�����ꍇ�A���[���h���W���~�����ꍇ�A���f��(�W���C���g)��
//    ���[�J�����W���~�����ꍇ������܂��B���炩���ߓK�؂Ȋ֐��ŃZ�b�g�A�b�v��
//    �s���Ă����΁A�s��̏�Z�Ȃ��ɂ����̒l�𓾂邱�Ƃ��\�ł��B
// 2: ���f��(�W���C���g)�̃��[�J�����W�����炩���ߌv�Z���Ă���ꍇ�A
//    Position/Vector�s��̃X�^�b�N�Ɍv�Z�ς݂̍s����Z�b�g���Ă����΁A
//    Projection�s���ύX���邾���ŁA�I�u�W�F�N�g�S�̂̈ʒu��J������
//    �ʒu��ύX���邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// A�{�^�����������ƂŁA�I�u�W�F�N�g�̕`��O�ɌĂԊ֐����A
// NNS_G3dGlbFlushP / NNS_G3dGlbFlushVP / NNS_G3dGlbFlushWVP
// �̒�����I�����܂��B�Ȃ��A
// P: Projection
// V: View
// W: World
// �̗��ł��B
//
// HOWTO:
// �I�u�W�F�N�g�̕`��O��NNS_G3dGlbFlushP / NNS_G3dGlbFlushVP / NNS_G3dGlbFlushWVP�֐�
// ���R�[�����܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    enum
    {
        Enum_NNS_G3dGlbFlushP   = 0,
        Enum_NNS_G3dGlbFlushVP  = 1,
        Enum_NNS_G3dGlbFlushWVP = 2
    };

    //  �ǂ����̊֐����g���Ă��邩
    int cameraFlag = Enum_NNS_G3dGlbFlushP;

    NNSG3dRenderObj object;
    NNSG3dResMdl*    model;

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

    model = LoadG3DModel("data/billboard.nsbmd");
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

    G3DDemo_InitCamera(&gCamera, 4 * FX32_ONE, 16 * FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5 * FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "CameraMtx");
    G3DDemo_Print(0,2, G3DDEMO_COLOR_WHITE, "change:A");

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
            VecFx32 scale = {FX32_ONE << 2, FX32_ONE << 2, FX32_ONE << 2};
            NNS_G3dGlbSetBaseScale(&scale);

            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //

            // NNS_G3dGlbFlushAlt��p�����ꍇ�A�J�����gProjection�s��Ɏˉe�ϊ��s��E�J�����s��E�x�[�XSRT
            // ����������悤�ɂȂ�BPosition/Vector�s��(�X�^�b�N)�ɂ́A���[�J���ϊ��̍s��݂̂��i�[����邱�ƂɂȂ�B
            // ���̂��߁A�J�����gProjection�s���ύX���邱�ƂŁAPosition/Vector�s��(�X�^�b�N)���v�Z���Ȃ����Ȃ��Ă�
            // �����|�[�Y���Ƃ������f���𕡐��`�悷�邱�Ƃ��ł���(�s��X�^�b�N���Ɍv�Z���ʂ��c���Ă���K�v�͂���)�B
            switch(cameraFlag) {
            case Enum_NNS_G3dGlbFlushP:
                //  Projection�s��ɃJ�����s��͂������Ȃ�
                NNS_G3dGlbFlushP();
                break;
            case Enum_NNS_G3dGlbFlushVP:
                //
                NNS_G3dGlbFlushVP();
                break;
            case Enum_NNS_G3dGlbFlushWVP:
                //  Projection�s��ɃJ�����s�񂪂������܂�
                NNS_G3dGlbFlushWVP();
                break;
            }

            //  ���ԑ���J�n
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
                // NNS_G3dGlbFlush(),NNS_G3dGlbFlushAlt()�̂ǂ�����g���Ă��o�Ă���G�͓���
                NNS_G3dDraw(&object);
            }
            // ���ԑ��茋��
            time = OS_GetTick() - time;
        }

        {
            // �X�P�[�������ɖ߂�
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);
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

        //  �`�{�^�����������т�cameraFlag��؂�ւ���
        if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A)) {
            cameraFlag++;
            cameraFlag = cameraFlag % 3;
        }
        
        //   ��,�ǂ̊֐����Ă�ł��邩��\������
        switch(cameraFlag)
        {
        case Enum_NNS_G3dGlbFlushP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushP  ");
            break;
        case Enum_NNS_G3dGlbFlushVP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushVP ");
            break;
        case Enum_NNS_G3dGlbFlushWVP:
            G3DDemo_Printf(0,4, G3DDEMO_COLOR_GREEN, "G3dGlbFlushWVP");
            break;
        }
        
        //   ���̃J�����gProjection�s��ƃJ�����gPos/Vector�s��̒��g��������\������
        switch(cameraFlag)
        {
        case Enum_NNS_G3dGlbFlushP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj                   ");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "Camera * BaseSRT ");
            break;
        case Enum_NNS_G3dGlbFlushVP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj * Camera          ");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "BaseSRT              ");
            break;
        case Enum_NNS_G3dGlbFlushWVP:
            G3DDemo_Printf(0,6, G3DDEMO_COLOR_WHITE, "Projection Mtx:");
            G3DDemo_Printf(2,7, G3DDEMO_COLOR_MAGENTA, "Pj * Camera * BaseSRT");
            G3DDemo_Printf(0,8, G3DDEMO_COLOR_WHITE, "Pos/Vector Mtx:");
            G3DDemo_Printf(2,9, G3DDEMO_COLOR_MAGENTA, "Identity Mtx         ");
            break;
        }
        
        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
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
    NNS_G3D_NULL_ASSERT(resFile);

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
