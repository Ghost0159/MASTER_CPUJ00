/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - SeparateTex
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// �قȂ�t�@�C���Ɋi�[����Ă��郂�f���ƃe�N�X�`�����o�C���h���ĕ\������T���v��
// 
// ���f�����\�[�X�ƃe�N�X�`�����\�[�X�̓R���o�[�g���ɕ������Ă������Ƃ��\�ł��B
// ���̂悤�ȏꍇ�ANNS_G3dResDefaultSetup�ł̓��f���ƃe�N�X�`�����o�C���h���邱�Ƃ�
// �ł��Ȃ��̂ŁA���[�U�[�������I�ɍs���K�v������܂��B
//
// �T���v���̓���
// human_run_t.nsbmd��g3dcvtr��-emdl�I�v�V�����ō쐬���ꂽ�e�N�X�`���u���b�N��
// �����Ȃ�.nsbmd�t�@�C���ł��B�܂��Ahuman_run_t.nsbtx��-etex�I�v�V�����ō쐬���ꂽ
// �e�N�X�`���u���b�N�݂̂�.nsbtx�t�@�C���ł��B�ȏ�2�̕ʌ̃t�@�C�������s���ɂ�����
// �o�C���h���ĕ\�����܂��B
//
// HOWTO:
// �ENNS_G3dBindMdlSet���g�p���ă��f�����\�[�X�Z�b�g�ƃe�N�X�`�����\�[�X���o�C���h���܂��B
//   (�T���v����NNS_G3dResDefaultSetup��.nsbtx�t�@�C���ɓK�p�����Ƃ��Ƀe�N�X�`����
//    VRAM�Ƀ��[�h����Ă���)
// �ENNS_G3dBindMdlTex���g�p����ƌX�̃��f�����\�[�X���e�N�X�`�����\�[�X�Ƀo�C���h
//   �ł��܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* mdlPath,const char* texPath);
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
    NNSG3dRenderObj object;
    NNSG3dResMdl*   model;
    NNSG3dResTex*   tex;
    NNSG3dResFileHeader* resNsbmd;
    NNSG3dResFileHeader* resNsbtx;

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
        // �Ⴄ���\�[�X�t�@�C�����ɑ��݂��郂�f���ƃe�N�X�`�����o�C���h����B
        //

        // .nsbmd�t�@�C����.nsbtx�t�@�C����ǂݍ���
        resNsbmd = G3DDemo_LoadFile("data/human_run_t.nsbmd");
        SDK_ASSERTMSG(resNsbmd, "load failed");

        resNsbtx = G3DDemo_LoadFile("data/human_run_t.nsbtx");
        SDK_ASSERTMSG(resNsbtx, "load failed");

        // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
        // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
        // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
        // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
        DC_StoreRange(resNsbmd, resNsbmd->fileSize);
        DC_StoreRange(resNsbtx, resNsbtx->fileSize);

        (void)NNS_G3dResDefaultSetup(resNsbmd);
        (void)NNS_G3dResDefaultSetup(resNsbtx); // ���̒��Ńe�N�X�`����VRAM�Ƀ��[�h����Ă���B

        // ���f���Z�b�g���̍ŏ��̃��f�����擾
        model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(resNsbmd), 0);
        SDK_NULL_ASSERT(model);

        // .nsbtx�̃e�N�X�`���u���b�N���擾
        tex = NNS_G3dGetTex(resNsbtx);
        SDK_NULL_ASSERT(tex);

        // resNsbmd���̃��f����resNsbtx���̃e�N�X�`���E�p���b�g���o�C���h����B
        // ���̍s���R�����g�A�E�g����ƁA�Y�{���̃e�N�X�`�����͂��Ă��Ȃ��摜�ɂȂ�B
        (void)NNS_G3dBindMdlSet(NNS_G3dGetMdlSet(resNsbmd), tex);
    }
    
    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&object, model);

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "SeparateTex");

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
            // �\���I�u�W�F�N�g�ɃX�P�[�����O��������
            //
            VecFx32 scale = {FX32_ONE >> 2, FX32_ONE >> 2, FX32_ONE >> 2};
            NNS_G3dGlbSetBaseScale(&scale);

            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
                //
                NNS_G3dDraw(&object);
            }
            time = OS_GetTick() - time;
        }

        {
            // �O���E���h�`��p�ɃX�P�[����߂�
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);
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
