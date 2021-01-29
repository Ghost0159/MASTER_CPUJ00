/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - callback5
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
// �R�[���o�b�N���g�p����G3D�̕`�擮����J�X�^�}�C�Y����T���v���i���̂T�j
//
// NNS_G3dRenderObjSetInitFunc�֐���p����ƁA�`��O�Ƀ��[�U�[��`�̊֐���
// �Ăяo����A���̒��ŕ����̃R�[���o�b�N�֐���o�^���鏈�����L�q���邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// �����̃R�[���o�b�N�֐����g�p���ă��{�b�g�̕`����J�X�^�}�C�Y���܂��B
// A�{�^���������� --> callback1�T���v����A,X,Y�{�^�����������ꍇ�Ɏ��s����鏈����
//                     ��x�Ɏ��s����܂��B
//
// HOWTO:
// 1: NNS_G3dRenderObjSetInitFunc�֐���p���āA�`��O�Ɏ��s����֐���NNSG3dRenderObj��
//    �o�^���܂��B
// 2: �o�^���ꂽ�֐������ŁANNS_G3dRSSetCallBack�֐���p���āA�����̕K�v�ȃR�[���o�b�N
//    �֐���NNSG3dRS�\���̂ɓo�^���Ă����܂��B
//
//---------------------------------------------------------------------------


#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void             InitG3DLib(void);
static NNSG3dResMdl*    LoadG3DModel(const char* path);
static void             SetCamera(G3DDemoCamera* camera);




/* -------------------------------------------------------------------------
    eraseWaist

    SBC��NODE�R�}���h(NNSi_G3dFuncSbc_NODE�֐�)�ɓ���R�[���o�b�N�B
    NODE�R�}���h�ŁA�^�C�~���OA,�^�C�~���OB�ɂ����ăR�[���o�b�N��o�^�����
    �r�W�r���e�B�v�Z�̌��ʂ��쐬������ύX������ł���B
    rs->pVisAnmResult����Q�Ƃł���B

    �Ă΂ꂽ�Ƃ��̕`��m�[�h����chest�Ȃ�Ε\�����Ȃ�
 ------------------------------------------------------------------------- */
static void
eraseWaist(NNSG3dRS* rs)
{
    int jntID;
    NNS_G3D_GET_JNTID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &jntID,
                      "chest");
    SDK_ASSERT(jntID >= 0);

    if (NNS_G3dRSGetCurrentNodeID(rs) == jntID)
    {
        NNSG3dVisAnmResult* visResult = NNS_G3dRSGetVisAnmResult(rs);
        visResult->isVisible = FALSE;
        //
        // ����ȏ�R�[���o�b�N���g�p����K�v���Ȃ��ꍇ�̓��Z�b�g���Ă�����
        // �p�t�H�[�}���X�ւ̈��e�����኱�������Ȃ�\��������B
        //
        NNS_G3dRSResetCallBack(rs, NNS_G3D_SBC_NODE);
    }
}


/* -------------------------------------------------------------------------
    getTranslucent

    SBC��MAT�R�}���h(NNSi_G3dFuncSbc_MAT�֐�)�ɓ���R�[���o�b�N�B
    MAT�R�}���h�ŁA�^�C�~���OA,�^�C�~���OB�ɂ����ăR�[���o�b�N��o�^�����
    �}�e���A���v�Z�̌��ʂ��쐬������ύX������ł���B
    rs->pMatAnmResult����Q�Ƃł���B

    �Ă΂ꂽ�Ƃ��̕`��}�e���A������lambert2�Ȃ�Δ������ɁA
    ����ȊO�Ȃ�΃��C���[�t���[���\���ɂ���B
 ------------------------------------------------------------------------- */
static void
getTranslucent(NNSG3dRS* rs)
{
    int matID;
    NNSG3dMatAnmResult* matResult;
    NNS_G3D_GET_MATID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &matID,
                      "lambert2");
    SDK_ASSERT(matID >= 0);

    matResult = NNS_G3dRSGetMatAnmResult(rs);

    if (NNS_G3dRSGetCurrentMatID(rs) == matID)
    {
        matResult->prmPolygonAttr =
            (matResult->prmPolygonAttr & ~REG_G3_POLYGON_ATTR_ALPHA_MASK) |
            (16 << REG_G3_POLYGON_ATTR_ALPHA_SHIFT);
    }
    else
    {
        matResult->flag |= NNS_G3D_MATANM_RESULTFLAG_WIREFRAME;
    }
}


/* -------------------------------------------------------------------------
    scaleArm

    SBC��NODEDESC�R�}���h(NNSi_G3dFuncSbc_NODEDESC�֐�)�ɓ���R�[���o�b�N�B
    NODEDESC�R�}���h�ŁA�^�C�~���OA, �^�C�~���OB�ɂ����ăR�[���o�b�N��o�^����
    �ƁA�W���C���g�v�Z�̌��ʂ��쐬������ύX������ł���B
    rs->pJntAnmResult����Q�Ƃł���B

    NODEDESC�R�}���h�̍ŏ��̈�����NODEDESC�R�}���h�Ōv�Z����W���C���gID�ɂȂ�B
    left_upper_arm�̌v�Z�̍ۂɂ̂݁A�X�P�[����2�{�ɂ���B
 ------------------------------------------------------------------------- */
static void
scaleArm(NNSG3dRS* rs)
{
    int jntID;
    
    NNS_G3D_GET_JNTID(NNS_G3dRenderObjGetResMdl(NNS_G3dRSGetRenderObj(rs)),
                      &jntID,
                      "left_upper_arm");
    SDK_ASSERT(jntID >= 0);

    if (NNS_G3dRSGetCurrentNodeDescID(rs) == jntID)
    {
        NNSG3dJntAnmResult* jntResult;
        jntResult = NNS_G3dRSGetJntAnmResult(rs);

        jntResult->flag &= ~NNS_G3D_JNTANM_RESULTFLAG_SCALE_ONE;
        jntResult->scale.x = 2 * FX32_ONE;
        jntResult->scale.y = 2 * FX32_ONE;
        jntResult->scale.z = 2 * FX32_ONE;

        //
        // ����ȏ�R�[���o�b�N���g�p����K�v���Ȃ��ꍇ�̓��Z�b�g���Ă�����
        // �p�t�H�[�}���X�ւ̈��e�����኱�������Ȃ�\��������B
        //
        NNS_G3dRSResetCallBack(rs, NNS_G3D_SBC_NODEDESC);
    }
}


static void
setCallback(NNSG3dRS* rs)
{
    NNS_G3dRSSetCallBack(rs, &scaleArm, NNS_G3D_SBC_NODEDESC, NNS_G3D_SBC_CALLBACK_TIMING_B);
    NNS_G3dRSSetCallBack(rs, &getTranslucent, NNS_G3D_SBC_MAT, NNS_G3D_SBC_CALLBACK_TIMING_B);
    NNS_G3dRSSetCallBack(rs, &eraseWaist, NNS_G3D_SBC_NODE, NNS_G3D_SBC_CALLBACK_TIMING_B);
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

    model = LoadG3DModel("data/robot_t.nsbmd");
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
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "callback5");

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
            // �����{�^���ɂ���Đݒ肷��R�[���o�b�N�֐�,�R�[���o�b�N������SBC�R�}���h,
            // �R�}���h���^�C�~���O��ݒ肷��B
            //
            if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_A))
            {
                NNS_G3dRenderObjSetInitFunc(&object, &setCallback);
            }
            else
            {
                // ���������Ȃ��ꍇ�̓R�[���o�b�N�ݒ�����Z�b�g
                NNS_G3dRenderObjSetInitFunc(&object, NULL);
            }

            //
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

            // ���ԑ���B
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
