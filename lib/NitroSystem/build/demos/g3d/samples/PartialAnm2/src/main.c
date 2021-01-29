/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - PartialAnm2
  File:     main.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//
// �W���C���g�A�j���[�V�����𕔕��I�ɍĐ�����T���v���Q
//
// �C�j�V�����C�Y���ꂽ�A�j���[�V�����I�u�W�F�N�g��K�؂ɑ��삷�邱�Ƃɂ���āA
// �Ⴆ�Θr�����𓮂����Ƃ������A�W���C���g�A�j���[�V�����̈ꕔ�Đ����\�ł��B
//
// �T���v���̓���
// �l�������Ă��܂��B�{�^���������ƁA�l���̃A�j���[�V������Ɨ��ɂn�m�^�n�e�e�ł��܂��B
// �`�{�^���������ƁA���r�̃A�j���[�V�������n�m�^�n�e�e���܂��B
// �a�{�^���������ƁA�E�r�̃A�j���[�V�������n�m�^�n�e�e���܂��B
// �w�{�^���������ƁA���r�̃A�j���[�V�������n�m�^�n�e�e���܂��B
// �x�{�^���������ƁA�E�r�̃A�j���[�V�������n�m�^�n�e�e���܂��B
//
// HOWTO:
// 1: JointAnm�T���v���Ɠ��l�ɃA�j���[�V�����I�u�W�F�N�g���Z�b�g�A�b�v���܂��B
// 2: �E�r�E���r�E�E�r�E���r���ꂼ�ꂾ���������A�j���[�V�����I�u�W�F�N�g��
//    NNS_G3dAnmObjDisableID��p���āA�쐬���܂��B
// 3: ���ꂼ��̃A�j���[�V�����I�u�W�F�N�g��Ɨ��ɃR���g���[�����邱�Ƃɂ��A
//    �E�r�E���r�E�E�r�E���r������������~�߂���ł��܂��B
//
//---------------------------------------------------------------------------
#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
    anmObjArmL

    �A�j���[�V�����I�u�W�F�N�g�𑀍삵�č��r�����𓮂����悤�ɐݒ肷��B
   ------------------------------------------------------------------------- */
static void anmObjArmL(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int arm_l1, arm_l2, wrist_l, wrist_l_end;
    int i;

    // ���r�����̃m�[�hID���擾
    NNS_G3D_GET_JNTID(m, &arm_l1, "arm_l1");
    NNS_G3D_GET_JNTID(m, &arm_l2, "arm_l2");
    NNS_G3D_GET_JNTID(m, &wrist_l, "wrist_l");
    NNS_G3D_GET_JNTID(m, &wrist_l_end, "wrist_l_end");

    SDK_ASSERT(arm_l1 >= 0 && arm_l2 >= 0 && wrist_l >=0 && wrist_l_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != arm_l1  &&
            i != arm_l2  &&
            i != wrist_l &&
            i != wrist_l_end)
        {
            //
            // ���r�ȊO�̃A�j���[�V������Disable�ɂ���B
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjArmR

    �A�j���[�V�����I�u�W�F�N�g�𑀍삵�ĉE�r�����𓮂����悤�ɐݒ肷��B
   ------------------------------------------------------------------------- */
static void anmObjArmR(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int arm_r1, arm_r2, wrist_r, wrist_r_end;
    int i;

    // ���r�����̃m�[�hID���擾
    NNS_G3D_GET_JNTID(m, &arm_r1, "arm_r1");
    NNS_G3D_GET_JNTID(m, &arm_r2, "arm_r2");
    NNS_G3D_GET_JNTID(m, &wrist_r, "wrist_r");
    NNS_G3D_GET_JNTID(m, &wrist_r_end, "wrist_r_end");

    SDK_ASSERT(arm_r1 >= 0 && arm_r2 >= 0 && wrist_r >=0 && wrist_r_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != arm_r1  &&
            i != arm_r2  &&
            i != wrist_r &&
            i != wrist_r_end)
        {
            //
            // ���r�ȊO�̃A�j���[�V������Disable�ɂ���B
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegL

    �A�j���[�V�����I�u�W�F�N�g�𑀍삵�č��r�����𓮂����悤�ɐݒ肷��B
   ------------------------------------------------------------------------- */
static void anmObjLegL(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int leg_l1, leg_l2, foot_l, foot_l_end;
    int i;

    // ���r�����̃m�[�hID���擾
    NNS_G3D_GET_JNTID(m, &leg_l1, "leg_l1");
    NNS_G3D_GET_JNTID(m, &leg_l2, "leg_l2");
    NNS_G3D_GET_JNTID(m, &foot_l, "foot_l");
    NNS_G3D_GET_JNTID(m, &foot_l_end, "foot_l_end");

    SDK_ASSERT(leg_l1 >= 0 && leg_l2 >= 0 && foot_l >= 0 && foot_l_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != leg_l1 &&
            i != leg_l2 &&
            i != foot_l &&
            i != foot_l_end)
        {
            //
            // ���r�ȊO�̃A�j���[�V������Disable�ɂ���B
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegR

    �A�j���[�V�����I�u�W�F�N�g�𑀍삵�ĉE�r�����𓮂����悤�ɐݒ肷��B
   ------------------------------------------------------------------------- */
static void anmObjLegR(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int leg_r1, leg_r2, foot_r, foot_r_end;
    int i;

    // ���r�����̃m�[�hID���擾
    NNS_G3D_GET_JNTID(m, &leg_r1, "leg_r1");
    NNS_G3D_GET_JNTID(m, &leg_r2, "leg_r2");
    NNS_G3D_GET_JNTID(m, &foot_r, "foot_r");
    NNS_G3D_GET_JNTID(m, &foot_r_end, "foot_r_end");

    SDK_ASSERT(leg_r1 >= 0 && leg_r2 >= 0 && foot_r >= 0 && foot_r_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != leg_r1 &&
            i != leg_r2 &&
            i != foot_r &&
            i != foot_r_end)
        {
            //
            // ���r�ȊO�̃A�j���[�V������Disable�ɂ���B
            //
            NNS_G3dAnmObjDisableID(p, i);
        }
    }
}


/* -------------------------------------------------------------------------
    anmObjLegR

    �A�j���[�V�����I�u�W�F�N�g�𑀍삵�č��r�E�E�r�E���r�E�E�r�ȊO�̕���������
    �������悤�ɐݒ肷��B
   ------------------------------------------------------------------------- */
static void anmObjMisc(NNSG3dAnmObj* p, const NNSG3dResMdl* m)
{
    int human_all, body_model, skl_root, hip, spin1, spin2, head, head_end;
    int i;

    // ���r�E�E�r�E���r�E�E�r�ȊO�̕����̃m�[�hID���擾
    NNS_G3D_GET_JNTID(m, &human_all, "human_all");
    NNS_G3D_GET_JNTID(m, &body_model, "body_model");
    NNS_G3D_GET_JNTID(m, &skl_root, "skl_root");
    NNS_G3D_GET_JNTID(m, &hip, "hip");
    NNS_G3D_GET_JNTID(m, &spin1, "spin1");
    NNS_G3D_GET_JNTID(m, &spin2, "spin2");
    NNS_G3D_GET_JNTID(m, &head, "head");
    NNS_G3D_GET_JNTID(m, &head_end, "head_end");

    SDK_ASSERT(human_all >= 0 && body_model >= 0 && skl_root >= 0 && hip >= 0 &&
               spin1 >= 0 && spin2 >= 0 && head >= 0 && head_end >= 0);

    for (i = 0; i < m->info.numNode; ++i)
    {
        if (i != human_all && i != body_model &&
            i != skl_root && i != hip &&
            i != spin1 && i != spin2 &&
            i != head && i != head_end)
        {
            //
            // ���r�E�E�r�E���r�E�E�r�̃A�j���[�V������Disable�ɂ���B
            //
            NNS_G3dAnmObjDisableID(p, i);
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
    void*           pAnmRes;
    NNSFndAllocator allocator;
    
    NNSG3dAnmObj*   pAnmObjArmL;
    NNSG3dAnmObj*   pAnmObjArmR;
    NNSG3dAnmObj*   pAnmObjLegL;
    NNSG3dAnmObj*   pAnmObjLegR;
    NNSG3dAnmObj*   pAnmObjMisc;

    BOOL armL = TRUE;
    BOOL armR = TRUE;
    BOOL legL = TRUE;
    BOOL legR = TRUE;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

    //
    // 4�o�C�g�A���C�������g�ŃA���P�[�^���쐬
    //
    NNS_FndInitAllocatorForExpHeap(&allocator, G3DDemo_AppHeap, 4);

    // �}�l�[�W�����e�N�X�`���C���[�W�X���b�g��4�X���b�g���Ǘ��ł���悤�ɂ���
    // �f�t�H���g�}�l�[�W���ɂ���B
    NNS_GfdInitFrmTexVramManager(4, TRUE);

    // �}�l�[�W�����p���b�g��32KB���Ǘ��ł���悤�ɂ���
    // �f�t�H���g�}�l�[�W���ɂ���B
    NNS_GfdInitFrmPlttVramManager(0x8000, TRUE);

    pModel = LoadG3DModel("data/human_run_t.nsbmd");
    SDK_ASSERTMSG(pModel, "load failed");

    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&object, pModel);
    
    //
    // �W���C���g�A�j����ǂݍ���Ŋ֘A�t�����Ă݂�
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/human_run_t.nsbca");
        SDK_ASSERTMSG( pFile, "anm load failed" );
        {
            // �A�j���[�V�����̏������A�����_�����O�I�u�W�F�N�g�ւ̒ǉ�
            // 
            // �ǂݍ��񂾃A�j���[�V�����t�@�C����NNSG3dRenderObj�̒��������
            // �A�j���[�V�����I�u�W�F�N�g(NNSG3dAnmObj)���m�ہE����������B
            // �S�ẴA�j���[�V���������l�̃R�[�h�ŃZ�b�g�A�b�v�����B
            //

            //
            // �A�j���[�V�����I�u�W�F�N�g�ɕK�v�ȃ�������allocator���犄�蓖�Ă�B
            // ���f�����\�[�X�ƃA�j���[�V�������\�[�X���w�肵�Ċ��蓖�Ă�K�v������B
            //

            // �C���f�b�N�X���O�̃A�j���[�V�������w��
            pAnmRes = NNS_G3dGetAnmByIdx(pFile, 0);
            SDK_NULL_ASSERT(pAnmRes);

            {
                // �g�p����A�j���[�V�����I�u�W�F�N�g���A���P�[�g
                pAnmObjArmL = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjArmL);
                pAnmObjArmR = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjArmR);
                pAnmObjLegL = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjLegL);
                pAnmObjLegR = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjLegR);
                pAnmObjMisc = NNS_G3dAllocAnmObj(&allocator, pAnmRes, pModel);
                SDK_NULL_ASSERT(pAnmObjMisc);
            }

            {
                // �g�p����A�j���[�V�����I�u�W�F�N�g������������
                NNS_G3dAnmObjInit(pAnmObjArmL, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjArmR, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjLegL, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjLegR, pAnmRes, pModel, NULL);
                NNS_G3dAnmObjInit(pAnmObjMisc, pAnmRes, pModel, NULL);
            }

            {
                // �A�j���[�V�����I�u�W�F�N�g��ݒ肵�đ̂̈ꕔ�����𓮂����悤�ɂ���
                anmObjArmL(pAnmObjArmL, pModel);
                anmObjArmR(pAnmObjArmR, pModel);
                anmObjLegL(pAnmObjLegL, pModel);
                anmObjLegR(pAnmObjLegR, pModel);
                anmObjMisc(pAnmObjMisc, pModel);
            }

            {
                // RenderObj�ɊeAnmObj��ǉ�����B
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmL );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmR );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegL );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegR );
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjMisc );
            }
        }
    }
    
    //
    // �f�����ʂ̏�����
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "PartialAnm2");

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

        //
        // �A�j���[�V�����t���[�����X�V����B
        // ���ꂼ��̃I�u�W�F�N�g�̏����t���[�����o���o���Ȃ̂ŁA
        // �̂̓o���o���̓���������B
        //
        pAnmObjArmL->frame += FX32_ONE;
        if( pAnmObjArmL->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjArmL))
            pAnmObjArmL->frame = 0;

        pAnmObjArmR->frame += FX32_ONE;
        if( pAnmObjArmR->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjArmR))
            pAnmObjArmR->frame = 0;

        pAnmObjLegL->frame += FX32_ONE;
        if( pAnmObjLegL->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjLegL))
            pAnmObjLegL->frame = 0;

        pAnmObjLegR->frame += FX32_ONE;
        if( pAnmObjLegR->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjLegR))
            pAnmObjLegR->frame = 0;

        pAnmObjMisc->frame += FX32_ONE;
        if( pAnmObjMisc->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObjMisc))
            pAnmObjMisc->frame = 0;

        // �`�{�^���͍��r�̂n�m�^�n�e�e
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (legL)
            {
                legL = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjLegL );
            }
            else
            {
                legL = TRUE;
                pAnmObjLegL->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegL );
            }
        }

        // �a�{�^���͉E�r�̂n�m�^�n�e�e
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            if (legR)
            {
                legR = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjLegR );
            }
            else
            {
                legR = TRUE;
                pAnmObjLegR->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjLegR );
            }
        }

        // �w�{�^���͍��r�̂n�m�^�n�e�e
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_X))
        {
            if (armL)
            {
                armL = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjArmL );
            }
            else
            {
                armL = TRUE;
                pAnmObjArmL->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmL );
            }
        }

        // �x�{�^���͉E�r�̂n�m�^�n�e�e
        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_Y))
        {
            if (armR)
            {
                armR = FALSE;
                NNS_G3dRenderObjRemoveAnmObj( &object, pAnmObjArmR );
            }
            else
            {
                armR = TRUE;
                pAnmObjArmR->frame = 0;
                NNS_G3dRenderObjAddAnmObj( &object, pAnmObjArmR );
            }
        }

        {
            VecFx32 scale = { FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2 };
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
                //
                NNS_G3dDraw(&object);
            }
            // ���ԑ��茋��
            time = OS_GetTick() - time;
        }

        {
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

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06ld usec", OS_TicksToMicroSeconds(time));

        // �l���̃A�j���[�V�����̂n�m�^�n�e�e��\��
        if (armL)
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Left  Arm(X): ON ");
        else
            G3DDemo_Printf(0, 1, G3DDEMO_COLOR_GREEN, "Left  Arm(X): OFF");

        if (armR)
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "Right Arm(Y): ON ");
        else
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "Right Arm(Y): OFF");

        if (legL)
            G3DDemo_Printf(0, 3, G3DDEMO_COLOR_GREEN, "Left  Leg(A): ON ");
        else
            G3DDemo_Printf(0, 3, G3DDEMO_COLOR_GREEN, "Left  Leg(A): OFF");

        if (legR)
            G3DDemo_Printf(0, 4, G3DDEMO_COLOR_GREEN, "Right Leg(B): ON ");
        else
            G3DDemo_Printf(0, 4, G3DDEMO_COLOR_GREEN, "Right Leg(B): OFF");

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
