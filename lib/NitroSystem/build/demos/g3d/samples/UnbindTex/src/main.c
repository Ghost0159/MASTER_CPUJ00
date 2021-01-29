/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - UnbindTex
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
// �e�N�X�`���̎��O���A�����̃T���v��
//
// ���f������e�N�X�`�������O������A�Ⴄ���f���ƃe�N�X�`�������������肵�܂��B
//
// �T���v���̓���
// �����̂ƒ����̂Ƀe�N�X�`�����\���Ă��܂��B
// A�{�^��->�e�N�X�`�����������܂��B
// B�{�^��->�e�N�X�`�����Ƃ�͂����܂��B
//
// HOWTO:
// �ENNS_G3dBindMdlTex, NNS_G3dBindMdlPltt�Ń��f���ƃe�N�X�`�����֘A�t���܂��B
// �ENNS_G3dReleaseMdlTex, NNS_G3dReleaseMdlPltt�Ń��f���ƃe�N�X�`���̊֘A�t����
//   �����ł��܂��B
// �ENNS_G3dBindMdlTexEx, NNS_G3dBindMdlPlttEx�Ń��f�����̎w�肳�ꂽ���O��
//   �e�N�X�`�����֘A�t���܂��B
// �ENNS_G3dReleaseMdlTexEx, NNS_G3dReleaseMdlPlttEx�Ń��f�����̎w�肳�ꂽ���O��
//   �e�N�X�`���ւ̊֘A�t���������ł��܂��B
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResTex* LoadG3DTexture(NNSG3dResFileHeader* resFile);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B
                B�Ńe�N�X�`�����͂�����A�Ńe�N�X�`�����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    
    BOOL changeTexture = TRUE;
    
    //��̃��f���p
    NNSG3dResFileHeader* resFileCube = NULL;
    NNSG3dResFileHeader* resFileBox  = NULL;
    
    NNSG3dRenderObj cubeObject;
    NNSG3dResMdl*   cubeModel = NULL;
    NNSG3dResTex*   cubeTexture = NULL;
    
    NNSG3dRenderObj boxObject;
    NNSG3dResMdl*   boxModel = NULL;
    NNSG3dResTex*   boxTexture = NULL;
    
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
        // Box Model�����[�h���ď������B
        // ��Ŏg��NNSG3dResMdl*�ƁANNSG3dResTex*���擾���Ă����B
        //
        resFileBox = G3DDemo_LoadFile("data/TextureBox.nsbmd");

        // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
        // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
        // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
        // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
        DC_StoreRange(resFileBox, resFileBox->fileSize);

        (void)NNS_G3dResDefaultSetup(resFileBox);

        SDK_ASSERTMSG(resFileBox, "load failed");
        boxModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFileBox), 0);
        SDK_NULL_ASSERT(boxModel);

        boxTexture = NNS_G3dGetTex(resFileBox);
    }

    {
        //
        // Cube Model�����[�h���ď�����
        // ��Ŏg��NNSG3dResMdl*�ƁANNSG3dResTex*���擾���Ă����B
        //
        resFileCube = G3DDemo_LoadFile("data/TextureCube.nsbmd");

        // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
        // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
        // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
        // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
        DC_StoreRange(resFileCube, resFileCube->fileSize);

        (void)NNS_G3dResDefaultSetup(resFileCube);

        SDK_ASSERTMSG(resFileCube, "load failed");
        cubeModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resFileCube), 0);
        SDK_NULL_ASSERT(cubeModel);

        cubeTexture = NNS_G3dGetTex(resFileCube);
    }

    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&cubeObject, cubeModel);
    NNS_G3dRenderObjInit(&boxObject, boxModel);

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "UNBINDTEX SAMPLE");

    for(;;)
    {
        OSTick time;

        //A�Ńe�N�X�`���̐؂�ւ�
        //release��łȂ���bind�͗L���ɂȂ�܂���
        if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A)) 
        {
            // same_name�Ƃ������O�����e�N�X�`�������ւ���B
            // ���̃T���v���Ŏg�p���Ă��郂�f���̃e�N�X�`���̓_�C���N�g�e�N�X�`��
            // �Ȃ̂Ńp���b�g�����ւ���K�v�͂Ȃ�
            NNS_G3D_DEFRESNAME(texName, "same_name");
            (void)NNS_G3dReleaseMdlTexEx(boxModel, &texName.resName);
            (void)NNS_G3dReleaseMdlTexEx(cubeModel, &texName.resName);

            if (changeTexture)
            {
                //
                // ���胂�f���̃e�N�X�`�����o�C���h����B
                //
                (void)NNS_G3dBindMdlTexEx(boxModel, cubeTexture, &texName.resName);
                (void)NNS_G3dBindMdlTexEx(cubeModel, boxTexture, &texName.resName);
            }
            else
            {
                //
                // �����f���̃e�N�X�`�����o�C���h����B
                //
                (void)NNS_G3dBindMdlTexEx(boxModel, boxTexture, &texName.resName);
                (void)NNS_G3dBindMdlTexEx(cubeModel, cubeTexture, &texName.resName);
            }
            changeTexture = (changeTexture) ? FALSE : TRUE;
        }                            
        else if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B)) // B�{�^���Ńe�N�X�`�����O���܂�
        {
            NNS_G3dReleaseMdlTex(boxModel);
            NNS_G3dReleaseMdlPltt(boxModel); // �p���b�g�����݂��Ȃ��ꍇ�͉������Ȃ�
            
            NNS_G3dReleaseMdlTex(cubeModel);
            NNS_G3dReleaseMdlPltt(cubeModel); // �p���b�g�����݂��Ȃ��ꍇ�͉������Ȃ�
        }

        SVC_WaitVBlankIntr();
        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);



        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        time = OS_GetTick();
        {
            //boxModel�`��
            VecFx32 trans = {FX32_ONE<<2, FX32_ONE<<2, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            NNS_G3dGlbFlushP();
            NNS_G3dDraw(&boxObject);
        }

        {
            //cubeModel�`��
            VecFx32 trans = {-FX32_ONE<<2, FX32_ONE<<2, 0};
            NNS_G3dGlbSetBaseTrans(&trans);
            NNS_G3dGlbFlushP();
            NNS_G3dDraw(&cubeObject);
        }
        time = OS_GetTick() - time;

        {
            // �O���E���h��`�悷��O��Translation��ݒ肵�Ă���
            VecFx32 trans = {0,0,0};
            NNS_G3dGlbSetBaseTrans(&trans);
        }
        
        //
        // G3D: �ʏ��G3_XXX���ĂԂƂ��́A���̑O��NNS_G3dGeComFlushBuffer()
        //      ���R�[�����ē������Ȃ��Ă͂Ȃ�Ȃ�
        //
        NNS_G3dGlbFlushP();
        NNS_G3dGeFlushBuffer();
        G3DDemo_DrawGround(&gGround);

        /* �W�I���g���������_�����O�G���W���֘A�������̃X���b�v */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
        
        //���\��
        G3DDemo_Printf(0,2, G3DDEMO_COLOR_GREEN, "change Texture:A");
        G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "release Texture:B");
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
