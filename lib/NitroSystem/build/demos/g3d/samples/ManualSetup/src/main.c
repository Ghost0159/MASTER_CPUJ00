/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ManualSetup
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// NNS_G3dResDefaultSetup���g�p�����Ƀ��f�����\�[�X���Z�b�g�A�b�v����T���v��
//
// ���f���̃o�C�i���f�[�^�����C����������Ƀ��[�hNNS_G3dResDefaultSetup���g�p
// �����ɃZ�b�g�A�b�v���s�Ȃ��A���f������ʂɕ\�����܂��B
//
// �T���v���̓���
// ���f����\�����܂��B
// 
// HOWTO:
// �R�[�h��̐������Q�Ƃ��Ă��������B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void           InitG3DLib(void);
static NNSG3dResMdl*  LoadG3DModel(const char* path);
static void           SetCamera(G3DDemoCamera* camera);


static void
ManualSetup(void* pResFile /* pResFile�̓��[�h���ꂽ.nsbmd�t�@�C���ւ̃|�C���^ */)
{
    //
    // NNS_G3dResDefaultSetup���g�p�����Ɏ����ŃZ�b�g�A�b�v����
    // ���̃R�[�h�ł́A.nsbmd���̃C���f�b�N�X���O�̃��f���݂̂��Z�b�g�A�b�v���Ă���B
    //
    NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet(pResFile);
    NNSG3dResTex* texture = NNS_G3dGetTex(pResFile); // �e�N�X�`���f�[�^���Ȃ��ꍇ��NULL��Ԃ��B

    //
    // ���f���f�[�^�̏������ɂ͈ȉ��̂悤�ȃX�e�b�v���K�v�ł��B
    // ���ۂɂ̓e�N�X�`���̂Ȃ����f���̏����������͕K�v����܂���B
    //
    // 1: �K�v�ȃe�N�X�`���E�p���b�g�̈�̃T�C�Y���擾���܂�
    // 2: �e�N�X�`����p���b�g�p�Ɏg�p����VRAM�̈�̂��߂̃L�[(gfd���C�u�����Ɠ����t�H�[�}�b�g)���쐬���܂�
    // 3: �L�[�����f���f�[�^�̃e�N�X�`���u���b�N�ɐݒ肵�܂�
    // 4: �e�N�X�`����p���b�g��VRAM�ɓ]�����܂�
    // 5: ���f�����\�[�X���̃}�e���A���ƃe�N�X�`�����o�C���h(�֘A�t��)���܂�
    //
    // 4��5�͏��Ԃ����ւ��Ă���肠��܂���B
    //
    // �Ȃ��ANNS_G3dResDefaultSetup�Ɠ��l�A�e�N�X�`����p���b�g�̓]���ɂ�
    // DMA���g�p���邽�߁A���̃Z�b�g�A�b�v�֐����ĂԑO�ɂ����̃f�[�^��
    // �L���b�V�����烁�C���������Ƀ��C�g�o�b�N���Ă����K�v������܂��B
    //
    if (texture)
    {
        //
        // 1: �K�v�ȃe�N�X�`���E�p���b�g�̈�̃T�C�Y���擾���܂��B
        //
        u32 szTex     = NNS_G3dTexGetRequiredSize(texture);
        u32 szTex4x4  = NNS_G3dTex4x4GetRequiredSize(texture);
        u32 szPltt    = NNS_G3dPlttGetRequiredSize(texture);

        //
        // 2: �e�N�X�`����p���b�g�p�Ɏg�p����VRAM�̈�̂��߂̃L�[
        //    (gfd���C�u�����Ɠ����t�H�[�}�b�g)���쐬���܂��B
        //
        // �L�[�ɐݒ肷��̈�̃T�C�Y�́A�K�v�ȃT�C�Y�ȏ�ł���K�v������܂��B
        // �܂��A�����ȃL�[�Ȃ�΁A���[�U�[���Ǝ��ɍ쐬���Ă����܂��܂���B
        // �L�[�́ANNS_GfdMakeTexKey, NNS_GfdMakePlttKey���g�p���邱�Ƃɂ��
        // ���ڍ쐬���邱�Ƃ��ł��܂��B
        //
        u32 keyTex    = NNS_GfdAllocTexVram(szTex, FALSE, 0);
        u32 keyTex4x4 = NNS_GfdAllocTexVram(szTex4x4, TRUE, 0);
        u32 keyPltt   = NNS_GfdAllocPlttVram(szPltt, FALSE, TRUE);

        OS_Printf("keyTex    = 0x%08x addr = 0x%08x size = 0x%08x\n", keyTex   , NNS_GfdGetTexKeyAddr(keyTex   ), NNS_GfdGetTexKeySize(keyTex   ));
        OS_Printf("keyTex4x4 = 0x%08x addr = 0x%08x size = 0x%08x\n", keyTex4x4, NNS_GfdGetTexKeyAddr(keyTex4x4), NNS_GfdGetTexKeySize(keyTex4x4));
        OS_Printf("keyPltt   = 0x%08x addr = 0x%08x size = 0x%08x\n", keyPltt  , NNS_GfdGetPlttKeyAddr(keyPltt ), NNS_GfdGetPlttKeySize(keyPltt ));

        //
        // 3: �쐬�����L�[�����f���f�[�^�̃e�N�X�`���u���b�N�ɐݒ肵�܂��B
        // 
        NNS_G3dTexSetTexKey(texture, keyTex, keyTex4x4);
        NNS_G3dPlttSetPlttKey(texture, keyPltt);

        //
        // 4: �e�N�X�`����p���b�g��VRAM�ɓ]�����܂��B
        //
        // �ݒ肵���L�[�̏��ɏ]���ăe�N�X�`���ƃp���b�g��VRAM�ɓ]�����܂��B
        // 2�Ԗڂ̈�����TRUE��ݒ肷��Ɩ���BeginLoadTex, EndLoadTex������ōs��
        // FALSE���ƂȂɂ����܂���i�O����BeginLoadTex,EndLoadTex������K�v������j
        //
        NNS_G3dTexLoad(texture, TRUE);
        NNS_G3dPlttLoad(texture, TRUE);

        //
        // 5: ���f�����\�[�X���̃}�e���A���ƃe�N�X�`�����o�C���h(�֘A�t��)���܂�
        //
        // NNS_G3dBindMdlSet���g�p���āA���f�����\�[�X�̃}�e���A������
        // �e�N�X�`���֘A�f�[�^(TexImageParam TexPlttBase)���Z�b�g�A�b�v���܂��B
        // 
        // �֐���BOOL�^�̕Ԃ�l�������A�Ԃ�l��TRUE�̏ꍇ�̓��f�����̃e�N�X�`�������}�e���A��
        // �S�Ă��Ή�����e�N�X�`���Ƀo�C���h����Ă��܂��B
        // FALSE�̏ꍇ�́A�e�N�X�`���Ƀo�C���h����Ă��Ȃ��}�e���A�����܂��c���Ă��܂��B
        //
        // �Ȃ��A���Ƀo�C���h����Ă���}�e���A����Release���Ȃ�����V�����e�N�X�`���Ƀo�C���h����
        // ���Ƃ��ł��܂���(�v����ɓ����e�N�X�`�������������̃e�N�X�`���u���b�N���o�C���h
        // �����ꍇ�A��Ƀo�C���h���ꂽ�e�N�X�`�����L���ɂȂ�܂�)�B
        //
        {
            BOOL result;
            result = NNS_G3dBindMdlSet(mdlSet, texture);
            SDK_ASSERT(result);
        }
    }
}


static void
ManualRelease(void* pResFile /* pResFile�̓��[�h���ꂽ.nsbmd�t�@�C���ւ̃|�C���^ */)
{
    //
    // NNS_G3dResDefaultRelease���g�p�����Ɏ����Ń����[�X����B
    //

    NNSG3dResTex* texture = NNS_G3dGetTex(pResFile); // �e�N�X�`���f�[�^���Ȃ��ꍇ��NULL��Ԃ��B

    //
    // ���f���f�[�^�̉�������ɂ͈ȉ��̂悤�ȃX�e�b�v���K�v�ł��B
    // ���ۂɂ̓e�N�X�`���̂Ȃ����f���̉�������͕K�v����܂���B
    //
    // 1: ���f�����\�[�X���ɐݒ肳��Ă���e�N�X�`�����\�[�X�ւ̊֘A�t�����N���A����B
    // 2: �e�N�X�`�����\�[�X����L�[���Ƃ�O���B
    // 3: (���p���Ă���ꍇ��)VRAM�}�l�[�W����p���ăL�[�ɑΉ�����VRAM�̈���������B
    //
    if (texture)
    {
        NNSG3dPlttKey plttKey;
        NNSG3dTexKey  texKey, tex4x4Key;
        int status;
        NNSG3dResMdlSet* mdlSet = NNS_G3dGetMdlSet(pResFile);

        //
        // 1: ���f�����\�[�X���ɐݒ肳��Ă���e�N�X�`�����\�[�X�ւ̊֘A�t�����N���A����B
        //
        NNS_G3dReleaseMdlSet(mdlSet);

        // 2: �e�N�X�`�����\�[�X����L�[���Ƃ�O���B
        plttKey = NNS_G3dPlttReleasePlttKey(texture);
        NNS_G3dTexReleaseTexKey(texture, &texKey, &tex4x4Key);

        //
        // 3: (���p���Ă���ꍇ��)VRAM�}�l�[�W����p���ăL�[�ɑΉ�����VRAM�̈���������B
        //
        // VRAM�}�l�[�W�����g�p���Ă���ꍇ�́AVRAM�}�l�[�W���𗘗p����
        // VRAM�̈��������Ă����܂��B
        // �������A���̃T���v���Ŏg�p���Ă���t���[�������̃}�l�[�W���̏ꍇ�́A
        // �̈���ʂɉ���ł��Ȃ��̂ŁA�����̈���ė��p���邱�Ƃ͂ł��܂���B
        // ���[�U�[���Ǝ��ɊǗ�����ꍇ�́A�ȉ��̃R�[�h�����[�U�[�Ǝ��̂��̂�
        // ����K�v������܂��B
        //
        if (plttKey > 0)
        {
            // �e�N�X�`���p���b�g�X���b�g���Ɋm�ۂ���Ă���Ή��
            status = NNS_GfdFreePlttVram(plttKey);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreePlttVram failed");
        }

        if (tex4x4Key > 0)
        {
            // �e�N�X�`���C���[�W�X���b�g���Ɋm�ۂ���Ă���Ή��
            status = NNS_GfdFreeTexVram(tex4x4Key);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
        }

        if (texKey > 0)
        {
            // �e�N�X�`���C���[�W�X���b�g���Ɋm�ۂ���Ă���Ή��
            status = NNS_GfdFreeTexVram(texKey);
            NNS_G3D_ASSERTMSG(!status, "NNS_GfdFreeTexVram failed");
        }
    }
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
    NNSG3dResMdl*   pModel;
    void*           pResFile;
    BOOL            isSetup = TRUE;

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
        pResFile = G3DDemo_LoadFile("data/human_run_t.nsbmd");
        SDK_ASSERTMSG(pResFile, "load failed");

        // ManualSetup��NNS_G3dDraw�̒���DMA�]�����g�p���邽�߁A
        // �����̊֐����ĂԑO�Ƀ��f�����\�[�X�S�̂��������փX�g�A���܂��B
        // ���\�[�X�̃T�C�Y��������x�傫���ꍇ��DC_StoreAll���Ă񂾕��������ł��B
        // DC_StoreRange��DC_StoreAll�ɂ��Ă�NITRO-SDK�֐����t�@�����X�}�j���A�����Q�Ƃ��ĉ������B
        DC_StoreRange(pResFile, ((NNSG3dResFileHeader*)pResFile)->fileSize);

        ManualSetup(pResFile);

        //
        // ���f�����\�[�X���̃��f���Z�b�g����C���f�b�N�X#0�̃��f�����擾
        //
        pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
        SDK_ASSERTMSG(pModel, "model not found");

        //
        // G3D: NNSG3dRenderObj�̏�����
        //
        // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
        // �ێ�����\���̂ł��B
        // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
        // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
        //
        NNS_G3dRenderObjInit(&object, pModel);
    }

    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ManualSetup");

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
            if (isSetup)
            {
                ManualRelease(pResFile);
                pModel = NULL;
                isSetup = FALSE;
            }
            else
            {
                ManualSetup(pResFile);
                pModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(pResFile), 0);
                SDK_ASSERTMSG(pModel, "model not found");
                NNS_G3dRenderObjInit(&object, pModel);

                isSetup = TRUE;
            }
        }

        if (isSetup)
        {
            // �X�P�[���������ď��������Ă݂�
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

        if (isSetup)
        {
            G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Press A Button to Release");
        }
        else
        {
            G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:           ");
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Press A Button to Setup  ");
        }
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
