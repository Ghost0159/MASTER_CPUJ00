/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - CustomizeSbcOp
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
// SBC�R�}���h�̏����֐������[�U�[���J�X�^�}�C�Y�������̂ɕύX����T���v��
//
// �T���v���̓���
// 3���̐����`���\������Ă��āA���ꂼ��A�r���{�[�h�Ȃ��E�r���{�[�h����E
// Y���r���{�[�h����ŕ\������Ă��܂��B
// A�{�^�����������Ƃɂ��A�r���{�[�h�����֐������̃T���v�����Œ�`�������̂ɒu��������
// ���Ƃ��ł��܂��BB�{�^�����������Ƃɂ��A�r���{�[�h�ɂ�����X�P�[�����Q�{�ɂȂ�܂��B
// �Ȃ��A���̃T���v�����Œ�`����Ă���r���{�[�h�����֐��̓��C�u�����̂��̂�荂���ł����A
// �X�P�[�����������Ă���ꍇ�͐��m�ɕ\���ł��܂���B
//
// HOWTO:
// �ENNS_G3dFuncSbcTable�z��̃J�X�^�}�C�Y�������R�}���h�ɑΉ������ꏊ��
//   ���[�U�[����`�����֐��̃|�C���^��������
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


static void MySbcBBSimple(NNSG3dRS* rs, u32 opt);
static void MySbcBBYSimple(NNSG3dRS* rs, u32 opt);


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
    BOOL            useCustomizedBB = FALSE;
    BOOL            isScaled = FALSE;

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

    G3DDemo_InitCamera(&gCamera, 0 * FX32_ONE, 4 * FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5 * FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "CustomizeSbcOp");
    G3DDemo_Print(0,2, G3DDEMO_COLOR_WHITE, "change:A");
    G3DDemo_Print(0,4, G3DDEMO_COLOR_WHITE, "change:B");

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
            VecFx32 scale;

            //
            // B�{�^���Ń��f���ɂ�����X�P�[�����ύX����Ă���
            //
            if (isScaled)
            {
                scale.x = scale.y = scale.z = 2 * FX32_ONE;
            }
            else
            {
                scale.x = scale.y = scale.z = FX32_ONE;
            }

            NNS_G3dGlbSetBaseScale(&scale);

            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            //  ���ԑ���J�n
            time = OS_GetTick();
            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
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
            NNS_G3dGlbFlushP();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        /* �W�I���g���������_�����O�G���W���֘A�������̃X���b�v */
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // A�{�^����SBC�̃r���{�[�h�R�}���h�̃n���h����ύX
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (useCustomizedBB)
            {
                useCustomizedBB = FALSE;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BB]  = &NNSi_G3dFuncSbc_BB;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BBY] = &NNSi_G3dFuncSbc_BBY;
            }
            else
            {
                useCustomizedBB = TRUE;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BB]  = &MySbcBBSimple;
                NNS_G3dFuncSbcTable[NNS_G3D_SBC_BBY] = &MySbcBBYSimple;
            }
        }

        // B�{�^���Ń��f���ɂ�����X�P�[����ύX
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            if (isScaled)
                isScaled = FALSE;
            else
                isScaled = TRUE;
        }

        if (useCustomizedBB)
        {
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "Use simplified BB Functions");
        }
        else
        {
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "Use default BB Functions   ");
        }

        if (isScaled)
        {
            G3DDemo_Printf(0,5, G3DDEMO_COLOR_GREEN, "Billboards scaled    ");
        }
        else
        {
            G3DDemo_Printf(0,5, G3DDEMO_COLOR_GREEN, "Billboards not scaled");
        }

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));
    }
}


/*---------------------------------------------------------------------------*
    MySbcBBSimple
    
    �P�������ꂽ�r���{�[�h�����֐��B�ȉ��̂悤�Ȑ������������ɍ����ɂȂ��Ă���
    �E�R�[���o�b�N�|�C���g�����݂��Ȃ��B
    �Eopt��NNS_G3D_SBCFLG_000�łȂ��Ă͂Ȃ�Ȃ�
    �ENNS_G3dGlbFlushP�ȊO�ɂ͑Ή����Ă��Ȃ�
    �ENNS_G3D_RSFLAG_OPT_NOGECMD�ɂ͑Ή����Ă��Ȃ�
    �E�X�P�[�����������Ă���ꍇ�A�������\������Ȃ�
 *---------------------------------------------------------------------------*/
static void
MySbcBBSimple(NNSG3dRS* rs, u32 opt)
{
    //
    // �u�J�����̎ˉe���ʁv�ɕ��s�ȃr���{�[�h�̕\��
    //
    static u32 bbcmd1[] =
    {
        ((G3OP_MTX_POP << 0)       |
         (G3OP_MTX_MODE << 8)      |
         (G3OP_MTX_LOAD_4x3 << 16)),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, 
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0   // �����͉�(Trans)
    };
    
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    MtxFx44 m;

    NNS_G3D_NULL_ASSERT(rs);

    //
    // �R�[���o�b�N�|�C���g�͑��݂��Ȃ�
    //

    //
    // �s��X�^�b�N�Ƃ̏o������͂ł��Ȃ�
    //
#pragma unused(opt)
    NNS_G3D_ASSERT(opt == NNS_G3D_SBCFLG_000);

    //
    // NNS_G3dGlbFlushAlt�͋֎~���Ă��܂�
    //
    NNS_G3D_ASSERT(!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_ALT));

    //
    // NNS_G3D_RSFLAG_OPT_NOGECMD���֎~
    //
    NNS_G3D_ASSERT(!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD));

    // �o�b�t�@�̃t���b�V��
    NNS_G3dGeFlushBuffer();

    // �R�}���h�]��:
    // PROJ���[�h�ɕύX
    // �ˉe�s���ޔ�
    // �P�ʍs����Z�b�g
    reg_G3X_GXFIFO = ((G3OP_MTX_MODE << 0) |
                      (G3OP_MTX_PUSH << 8) |
                      (G3OP_MTX_IDENTITY << 16));
    reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;

    // �X�ɃW�I���g���G���W���̒�~��҂�
    // �J�����g�s��̎擾
    while (G3X_GetClipMtx(&m))
        ;

    // �r���{�[�h�s��̌v�Z
    trans->x = m._30;
    trans->y = m._31;
    trans->z = m._32;

    // �ˉe�s���POP
    // POS_VEC�ɕ��A
    // �J�����g�s��Ɋi�[
    MI_CpuSend32(&bbcmd1[0],
                 &reg_G3X_GXFIFO,
                 15 * sizeof(u32));

    rs->c += 2;
}


/*---------------------------------------------------------------------------*
    MySbcBBYSimple
    
    �P�������ꂽY���r���{�[�h�����֐��B�ȉ��̂悤�Ȑ������������ɍ����ɂȂ��Ă���
    �E�R�[���o�b�N�|�C���g�����݂��Ȃ��B
    �Eopt��NNS_G3D_SBCFLG_000�łȂ��Ă͂Ȃ�Ȃ�
    �ENNS_G3dGlbFlushP�ȊO�ɂ͑Ή����Ă��Ȃ�
    �ENNS_G3D_RSFLAG_OPT_NOGECMD�ɂ͑Ή����Ă��Ȃ�
    �E�X�P�[�����������Ă���ꍇ�A�������\������Ȃ�
 *---------------------------------------------------------------------------*/
static void
MySbcBBYSimple(NNSG3dRS* rs, u32 opt)
{
    MtxFx44 m;
    VecFx32 lz;

    static u32 bbcmd1[] =
    {
        ((G3OP_MTX_POP << 0)       |
         (G3OP_MTX_MODE << 8)      |
         (G3OP_MTX_LOAD_4x3 << 16)),
        1,
        GX_MTXMODE_POSITION_VECTOR,
        FX32_ONE, 0, 0, // �����͉�(4x3Mtx)
        0, FX32_ONE, 0,
        0, 0, FX32_ONE,
        0, 0, 0
    };
    VecFx32* trans = (VecFx32*)&bbcmd1[12];
    MtxFx43* mtx   = (MtxFx43*)&bbcmd1[3];

    NNS_G3D_NULL_ASSERT(rs);
    
    //
    // �R�[���o�b�N�|�C���g�͑��݂��Ȃ�
    //

    //
    // �s��X�^�b�N�Ƃ̏o������͂ł��Ȃ�
    //
#pragma unused(opt)
    NNS_G3D_ASSERT(opt == NNS_G3D_SBCFLG_000);

    //
    // NNS_G3dGlbFlushAlt�͋֎~���Ă��܂�
    //
    NNS_G3D_ASSERT(!(NNS_G3dGlb.flag & NNS_G3D_GLB_FLAG_FLUSH_ALT));

    //
    // NNS_G3D_RSFLAG_OPT_NOGECMD���֎~
    //
    NNS_G3D_ASSERT(!(rs->flag & NNS_G3D_RSFLAG_OPT_NOGECMD));

    // �o�b�t�@�̃t���b�V��
    NNS_G3dGeFlushBuffer();

    // �R�}���h�]��:
    // PROJ���[�h�ɕύX
    // �ˉe�s���ޔ�
    // �P�ʍs����Z�b�g
    reg_G3X_GXFIFO = ((G3OP_MTX_MODE << 0) |
                      (G3OP_MTX_PUSH << 8) |
                      (G3OP_MTX_IDENTITY << 16));
    reg_G3X_GXFIFO = (u32)GX_MTXMODE_PROJECTION;

    // �X�ɃW�I���g���G���W���̒�~��҂�
    // �J�����g�s��(�N���b�v�s��)�̎擾
    while (G3X_GetClipMtx(&m))
        ;

    // �r���{�[�h�s��̌v�Z
    trans->x = m._30;
    trans->y = m._31;
    trans->z = m._32;

    // mtx��_00, _01, _02�͕s��
    VEC_Normalize((VecFx32*)&m._10, (VecFx32*)&mtx->_10);

    lz.x = 0;
    lz.y = -m._12;
    lz.z = m._11;
    VEC_Normalize(&lz, (VecFx32*)&mtx->_20);

    // �ˉe�s���POP
    // POS_VEC�ɕ��A
    // �J�����g�s��Ɋi�[
    MI_CpuSend32(&bbcmd1[0],
                 &reg_G3X_GXFIFO,
                 15 * sizeof(u32));

    rs->c += 2;
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
