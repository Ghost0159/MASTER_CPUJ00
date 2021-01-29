/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - MultiModel
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
// �����̃��f����1��.nsbmd�t�@�C���ɓ����Ă���ꍇ�̂��߂̃T���v��
//
// g3dcvtr�ŕ�����.imd�t�@�C������͂Ƃ��ė^����ƁA�����̃��f����1��.nsbmd�t�@�C��
// �ɂ܂Ƃ߂邱�Ƃ��ł��܂��B���̍ہA���O�E���e���ɓ���̃e�N�X�`����p���b�g��1��
// �܂Ƃ߂�ꃁ�����̎g�p�̈���팸���邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// NNS_G3dGetMdlByIdx�Ń��\�[�X���̃��f���Z�b�g����C���f�b�N�X���w�肵��
// �e���f�����\�[�X�̃f�[�^���擾���܂��B���ꂼ��̃��f�����\�[�X�̓e�N�X�`����
// ���L���Ă��܂�(g3dcvtr ./data/two_cubes.nsbmd�Ŋm�F�ł��܂�)�B
//
// HOWTO
// 1: �Ⴆ�΁Ag3dcvtr file1.imd file2.imd ... fileN.imd -o files.nsbmd�Ƃ���ƁA
//    file1����fileN�̃��f����files.nsbmd�Ƃ����P�̃t�@�C���ɂ܂Ƃ߂��܂��B
//    ���̍ہA���ꖼ�œ�����e�̃e�N�X�`���^�p���b�g�͂P�ɂ܂Ƃ߂��܂��B
// 2: NNS_G3dResDefaultSetup�Ńe�N�X�`���^�p���b�g��VRAM�ւ̃��[�h�Ɗe���f�����\�[�X
//    �ւ̃o�C���h���s���܂��B
// 3: NNS_G3dGetMdlByIdx�֐��ŃC���f�b�N�X���w�肵�Ċe���f�����\�[�X���擾�ł��܂��B
//    �܂��ANNS_G3D_GET_MDL�}�N��(util.h�Œ�`)���g�p�����.imd�t�@�C��������
//    �g���q����菜���������𕶎��񃊃e�����Ƃ��Ďw��ł��܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void InitG3DLib(void);
static void LoadG3DModel(const char* path,NNSG3dResMdl** pModel1,NNSG3dResMdl** pModel2);
static void SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    //  ��̃��f��������
    NNSG3dRenderObj object1,object2;
    NNSG3dResMdl*    model1 = NULL;
    NNSG3dResMdl*    model2 = NULL;

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

    //  ��̃o�C�i�������̃��f�������o��
    LoadG3DModel("data/two_cubes.nsbmd", &model1, &model2);
    SDK_ASSERTMSG(model1, "model1 load failed");
    SDK_ASSERTMSG(model2, "model2 load failed");

    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&object1, model1);
    NNS_G3dRenderObjInit(&object2, model2);

    G3DDemo_InitCamera(&gCamera, 2*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "MULTI MODEL");

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
            // object1�̐ݒ�,�`��
            //
            VecFx32 scale = {FX32_ONE << 2, FX32_ONE << 2, FX32_ONE << 2};
            VecFx32 trans = {FX32_ONE * 4, 0, 0};

            // �X�P�[���������đ傫������
            NNS_G3dGlbSetBaseScale(&scale);

            //  �����E�Ɉړ�
            NNS_G3dGlbSetBaseTrans(&trans);

            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            // ���ԑ���J�n
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
                //
                NNS_G3dDraw(&object1);
            }
            // ���ԑ��茋��
            time = OS_GetTick() - time;
        }

        {
            //
            // object2�̐ݒ�,�`��
            //
            VecFx32 trans = {-FX32_ONE * 4, 0, 0};
            // �X�P�[����object1�̕`�掞�̐ݒ�Ɠ����ɂ���
            
            //  �������Ɉړ�
            NNS_G3dGlbSetBaseTrans(&trans);

            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            //
            // G3D: NNS_G3dDraw
            // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
            // ������ꍇ�̕`����s��
            //
            NNS_G3dDraw(&object2);
        }

        {
            // �n�ʂ̕`��
            // �܂��A�X�P�[���ƃg�����X�����ɖ߂�
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            VecFx32 trans = {0, 0, 0};

            NNS_G3dGlbSetBaseScale(&scale);
            NNS_G3dGlbSetBaseTrans(&trans);
            //
            // G3D: �ʏ��G3_XXX���ĂԂƂ��́A���̑O��NNS_G3dGeComFlushBuffer()
            //      ���R�[�����ē������Ȃ��Ă͂Ȃ�Ȃ�
            //
            NNS_G3dGlbFlushP();
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
  Name:         LoadG3DModel

  Description:  �t�@�C�����烂�f���f�[�^��ǂݍ��݂܂��B

  Arguments:    path: ���f���f�[�^�̃p�X���B
                    pModel1,pModel2:���f�����w���|�C���^�̃A�h���X

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
static void
LoadG3DModel(const char* path, NNSG3dResMdl** pModel1, NNSG3dResMdl** pModel2)
{
    BOOL status;
    NNSG3dResTex*        texture = NULL;
    NNSG3dResFileHeader* resFile = G3DDemo_LoadFile(path);

    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
    // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
    // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
    // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
    DC_StoreRange(resFile, resFile->fileSize);

    // �f�t�H���g�̏������֐����R�[�����ăZ�b�g�A�b�v����ꍇ
    status = NNS_G3dResDefaultSetup(resFile);
    NNS_G3D_ASSERTMSG(status, "NNS_G3dResDefaultSetup failed");

    //
    // G3D: ���f���̎擾
    // nsbmd�̓��f���𕡐��܂ނ��Ƃ��ł���̂ŁA�C���f�b�N�X(���f�����P�̏ꍇ��0)
    // ���w�肵�ĂP�̃��f���ւ̃|�C���^���擾����B
    // --------------------------------------------------------------------
    //  ��̃o�C�i�������̃��f�������o��
    //  �T�C�R���̂P�̐��ƐԊۂ̕��������e�N�X�`�����Ⴄ
    //  ���͋��L���邱�ƂŃf�[�^�ʂ�}���Ă���
    *pModel1 = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 0);
    *pModel2 = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFile), 1);
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
