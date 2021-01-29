/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - DisposeTex
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.18 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// ���C����������̃e�N�X�`���^�p���b�g�̈��VRAM�]����ɔj������T���v��
//
// ��l���L�������̃��f���̃e�N�X�`���^�p���b�g��VRAM�̈�ɏ풓������ꍇ�A
// ���C����������ɂ��̃��f���̃e�N�X�`���^�p���b�g��ێ����Ă����K�v�͂���܂���B
// ���̂悤�ȏꍇ�́A���\�[�X�t�@�C���̍Ō���ɂ���e�N�X�`���^�p���b�g�̈���������
// ��������ߖ񂷂邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// �T���v������CollapseResourceMemory�֐��ɂ����āANNSG3dResTex����e�N�X�`���^�p���b�g
// �̊J�n�A�h���X���擾���A���̃A�h���X�ȍ~�̗̈���q�[�v�ɕԋp���Ă��܂��B
//
// HOWTO
// tex��NNSG3dResTex�\���̂ւ̃|�C���^�Ƃ���ƁA
// (u8*)tex + tex->texInfo.ofsTex���e�N�X�`���^�p���b�g�f�[�^�̊J�n�A�h���X�ɂȂ�܂��B
// ����ȍ~�̔Ԓn�̃������͏��������Ă��\���܂���B
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void SetCamera(G3DDemoCamera* camera);
static void CollapseResourceMemory(void* memBlock, NNSG3dResTex* texture);
static void TextureZeroClear(NNSG3dResTex* pTex);

/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B
                TextureZeroClear�Ńe�N�X�`���̈���[���N���A���܂��B
                Ram�]����Ƀe�N�X�`���̈���[���N���A���Ă����Ȃ����삷�邱�Ƃ������B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    NNSG3dRenderObj object;
    NNSG3dResMdl*   model;

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

    //
    // LoadG3DModel�����Ńf�[�^�����[�h�A�e�N�X�`����VRAM�ɐݒ肷�铙��
    // ���������s������A�e�N�X�`���f�[�^�����̃��������[���N���A���A
    // �q�[�v���k�����ăe�N�X�`���f�[�^�����݂̂�������܂��B
    //
    model = LoadG3DModel("data/human_run_t.nsbmd");
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

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "DisposeTex");

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
            VecFx32 scale = {FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2};
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
            // �X�P�[�������ɖ߂�
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);

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

  Returns:      ���f���ւ̃|�C���^�B
   ------------------------------------------------------------------------- */
static NNSG3dResMdl*
LoadG3DModel(const char* path)
{
    NNSG3dResMdl* model   = NULL;
    NNSG3dResTex* texture = NULL;
    NNSG3dResFileHeader* resFile = G3DDemo_LoadFile(path);
    BOOL status;

    SDK_NULL_ASSERT(resFile);

    // NNS_G3dResDefaultSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
    // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
    // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
    // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
    DC_StoreRange(resFile, resFile->fileSize);

    status = NNS_G3dResDefaultSetup(resFile);
    NNS_G3D_ASSERTMSG(status, "NNS_G3dResDefaultSetup failed");

    //
    // �e�N�X�`���u���b�N�̃e�N�X�`���f�[�^�������[���N���A���܂��B
    //
    TextureZeroClear(NNS_G3dGetTex(resFile));

    //
    // �q�[�v���k�����āA�e�N�X�`���f�[�^������������܂��B
    //
    CollapseResourceMemory(resFile, NNS_G3dGetTex(resFile));

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


/* -------------------------------------------------------------------------
  Name:         CollapseResourceMemory

  Description:  �e�N�X�`���C���[�W�̃T�C�Y���������u���b�N���k�����܂��B

                �������u���b�N�̖����̓e�N�X�`���C���[�W�݂̂ł��邱�Ƃ�
                �O��Ƃ��Ă��܂��B

  Arguments:    memBlock:  �k�����郁�����u���b�N�B
                texture:   �e�N�X�`���f�[�^�̃|�C���^�B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
static void
CollapseResourceMemory(void* memBlock, NNSG3dResTex* texture)
{
    u8* texImgStartAddr;
    u32 newSize;

    // �e�N�X�`���^�p���b�g�́A4x4COMP�ȊO�̃e�N�X�`���E4x4COMP�̃e�N�X�`���E�p���b�g
    // �̏��ԂŊi�[����Ă��܂��B�����4x4COMP�ȊO�̃e�N�X�`���̊J�n�A�h���X�ȍ~��
    // �s�v�ɂȂ�܂��B�Ȃ��A4x4COMP�t�H�[�}�b�g�ȊO�̃e�N�X�`���C���[�W�����݂��Ȃ�
    // �ꍇ�ł�texture->texInfo.ofsTex�ɂ͓K�؂Ȓl�������Ă��܂��B
    SDK_ASSERT(texture->texInfo.ofsTex != 0);
    texImgStartAddr = (u8*)texture + texture->texInfo.ofsTex;

    // �q�[�v�̐擪����e�N�X�`���C���[�W�܂ł̃T�C�Y
    newSize = (u32)(texImgStartAddr - (u8*)memBlock);

    // �������u���b�N���k�����܂��B
    // ���̌��ʁA�e�N�X�`���C���[�W���̃������̓q�[�v�ɕԋp����܂��B
    (void)NNS_FndResizeForMBlockExpHeap(G3DDemo_AppHeap, memBlock, newSize);

}


/*-------------------------------------------------------------------------
    TextureZeroClear

    �e�N�X�`���u���b�N�̃e�N�X�`���f�[�^�������[���N���A���܂��B
    �������̏��͎c��̂ŁA�o�C���h�⃊���[�X�͉\�ł��B
  ------------------------------------------------------------------------- */
static void
TextureZeroClear(NNSG3dResTex* pTex)
{
    MI_CpuClearFast((u8*)pTex + pTex->texInfo.ofsTex,
                    pTex->header.size - pTex->texInfo.ofsTex);
}
