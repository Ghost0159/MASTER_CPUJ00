/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ModifyNsbmd
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
// ���f�����\�[�X���̃f�[�^������������T���v��
//
// G3D�̃��C�u�������ł͏��������͏I�����������ă��f�����\�[�X���̃f�[�^��
// ���������邱�Ƃ͂���܂��񂪁A���[�U�[�������I�ɃR�[�h�������Β��ڏ���������
// ���Ƃ͉\�ł��B�{�T���v���ł́AG3D��model.h�Ő錾����Ă���֐���p����
// �}�e���A�����̊e�����ɃA�N�Z�X���Ă��܂��B
//
// �T���v���̓���
// ���f�����\�[�X���̃}�e���A���f�[�^�͖��t���[���ύX����Ă��܂��B
// B,X�{�^���Ń��j���[(�}�e���A���̊e����)���㉺�Ɉړ����܂��B
// A�{�^���Ń}�e���A���̃f�[�^��NNSG3dGlbXXXX�֐��Őݒ肳�ꂽ�O���[�o������
// ���f�����\�[�X���̃}�e���A���f�[�^�̑���Ɏg�p���邩�ǂ����ݒ肵�܂��B�{�^����
// �������ƂŐ؂�ւ��܂��B
//
// HOWTO:
// �ENNS_G3dMdlUseMdlXXXX�Ń��f�����\�[�X���̃}�e���A���f�[�^��`��ɔ��f�����܂�(�f�t�H���g)�B
// �ENNS_G3dMdlUseGlbXXXX��NSG3dGlbXXXX�֐��Őݒ肳�ꂽ�}�e���A���f�[�^��`��ɔ��f�����܂��B
// �ENNS_G3dMdlSetMdlXXXX�Ń��f�����\�[�X���̃}�e���A���f�[�^�����������܂��B
// �ENNS_G3dMdlGetMdlXXXX�Ń��f�����\�[�X���̃}�e���A���f�[�^���擾���܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);

//  ���f���̂���}�e���A�������ԕω������邽�߂̃^�C�}�[
static int mdlTimer = 0;
static void changeMdlMat(NNSG3dResMdl* pMdl);
static GXRgb calcNextColor(GXRgb rgb);

#define	NUM_ITEM	9


/* -------------------------------------------------------------------------
  Name:         NitroMain

  Description:  �T���v���̃��C���B
                X,B�ō��ڑI��,A��Glb,Mdl�̐؂�ւ�

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
NitroMain(void)
{
    //  ���ڊ֌W�̕ϐ���`
//  const int NumItem = 9;

    int itemNo = 0;
    const char* ItemName[NUM_ITEM][2] = {
        {"MdlDiff","GlbDiff"},
        {"MdlAmb","GlbAmb"},
        {"MdlSpec","GlbSpec"},
        {"MdlEmi","GlbEmi"},
        {"MdlLightFlag","GlbLightFlag"},
        {"MdlPolyMode","GlbPolyMode"},
        {"MdlCullMode","GlbCullMode"},
        {"MdlPolyID","GlbPolyID"},
        {"MdlAlpha","GlbAlpha"}
    };
    int itemFlag[NUM_ITEM] = {
        0,0,0,0,0,0,0,0,0
    };
    

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
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ModifyNsBmd");

    for(;;)
    {
        OSTick time;

        SVC_WaitVBlankIntr();

        G3DDemo_PrintApplyToHW();
        G3DDemo_ReadGamePad();
        
        //  �O���[�o���ȃp�����[�^���g����,���f���̃p�����[�^���g������ݒ肷��
        //  model.h�Ő錾����Ă���֐����g�p
        if (itemFlag[0] == 0)
            NNS_G3dMdlUseMdlDiff(model);
        else
            NNS_G3dMdlUseGlbDiff(model);

        if (itemFlag[1] == 0)
            NNS_G3dMdlUseMdlAmb(model);
        else
            NNS_G3dMdlUseGlbAmb(model);

        if (itemFlag[2] == 0)
            NNS_G3dMdlUseMdlSpec(model);
        else
            NNS_G3dMdlUseGlbSpec(model);

        if (itemFlag[3] == 0)
            NNS_G3dMdlUseMdlEmi(model);
        else
            NNS_G3dMdlUseGlbEmi(model);

        if (itemFlag[4] == 0)
            NNS_G3dMdlUseMdlLightEnableFlag(model);
        else
            NNS_G3dMdlUseGlbLightEnableFlag(model);

        if (itemFlag[5] == 0)
            NNS_G3dMdlUseMdlPolygonMode(model);
        else
            NNS_G3dMdlUseGlbPolygonMode(model);

        if (itemFlag[6] == 0)
            NNS_G3dMdlUseMdlCullMode(model);
        else
            NNS_G3dMdlUseGlbCullMode(model);

        if (itemFlag[7] == 0)
            NNS_G3dMdlUseMdlPolygonID(model);
        else
            NNS_G3dMdlUseGlbPolygonID(model);

        if (itemFlag[8] == 0)
            NNS_G3dMdlUseMdlAlpha(model);
        else
            NNS_G3dMdlUseGlbAlpha(model);

        //  ���f���̂���}�e���A���̃p�����[�^�����ԕω�������
        changeMdlMat(model);

        G3DDemo_MoveCamera(&gCamera);
        G3DDemo_CalcCamera(&gCamera);

        G3X_Reset();
        G3X_ResetMtxStack();

        SetCamera(&gCamera);

        {
            VecFx32 scale = {FX32_ONE >> 2, FX32_ONE >> 2, FX32_ONE >> 2};
            VecFx32 trans = {FX32_ONE * 4, 0, 0};

            //
            // ���f���̃x�[�X�X�P�[���ƃx�[�X�g�����X���[�V������ݒ�
            //
            NNS_G3dGlbSetBaseScale(&scale);
            NNS_G3dGlbSetBaseTrans(&trans);

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
            // ���ԑ��茋�ʉ�ʕ\���B
            time = OS_GetTick() - time;
        }

        // �X�P�[�������ɖ߂�
        {
            VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
            NNS_G3dGlbSetBaseScale(&scale);
        }
        //  �g�����X�����ɖ߂�
        {
            VecFx32 trans = {0, 0, 0};
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
        
        
        //  X,B�őI�����ڐ؂�ւ�,�{�^���ŕύX
        {
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_X))
            {
                itemNo --;
                if(itemNo < 0)
                {
                    itemNo = NUM_ITEM - 1;
                }
            }
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_B))
            {
                itemNo ++;
                if(itemNo >= NUM_ITEM)
                {
                    itemNo = 0;
                }
            }
            if(G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
            {
                itemFlag[itemNo] = 1 - itemFlag[itemNo];
            }
        }
        
        
        //  ���ڂ̌���ԕ\��
        {
            int item;
            G3DDemo_Printf(0,2, G3DDEMO_COLOR_GREEN, "select:X,B");
            G3DDemo_Printf(0,3, G3DDEMO_COLOR_GREEN, "change:A");
            for(item = 0;item < NUM_ITEM;item ++)
            {
                int color = G3DDEMO_COLOR_WHITE;
                if(item == itemNo)
                {
                    color = G3DDEMO_COLOR_RED;
                }
                G3DDemo_Printf(0,5 + item, color, "%s", ItemName[item][itemFlag[item]]);
            }
        }

        G3DDemo_Printf(16,0, G3DDEMO_COLOR_YELLOW, "TIME:%06d usec", OS_TicksToMicroSeconds(time));
    }
}


/* -------------------------------------------------------------------------
  Name:         changeMdlMat

  Description:  ���f���̂���}�e���A���̐ݒ�����ԕω�������

  Arguments:    ���f���ւ̃|�C���^:NNSG3dResMdl* pMdl

  Returns:      �Ȃ��B
   ------------------------------------------------------------------------- */
void
changeMdlMat(NNSG3dResMdl* pMdl)
{
    u32 MatID;

    mdlTimer ++;
    if(mdlTimer >= 32 * 32 * 32)
    {
        mdlTimer = 0;
    }

    for (MatID = 0; MatID < pMdl->info.numMat; ++MatID)
    {
        //
        // �t���[�����Ƀ��f���̃}�e���A���J���[��ω������܂��B
        //
        NNS_G3dMdlSetMdlDiff(pMdl,
                             MatID,
                             calcNextColor(NNS_G3dMdlGetMdlDiff(pMdl, MatID)));

        NNS_G3dMdlSetMdlAmb(pMdl,
                            MatID,
                            calcNextColor(NNS_G3dMdlGetMdlAmb(pMdl, MatID)));

        NNS_G3dMdlSetMdlSpec(pMdl,
                             MatID,
                             calcNextColor(NNS_G3dMdlGetMdlSpec(pMdl, MatID)));

        NNS_G3dMdlSetMdlEmi(pMdl,
                            MatID,
                            calcNextColor(NNS_G3dMdlGetMdlEmi(pMdl, MatID)));

        //
        // ���f���ɂ����郉�C�g��ON/OFF��ω������܂��B
        //
        NNS_G3dMdlSetMdlLightEnableFlag(pMdl,
                                        MatID,
                                        mdlTimer / 55 % 16);

        //
        // ���f���̃|���S�����[�h��ω������܂��B
        //
        NNS_G3dMdlSetMdlPolygonMode(pMdl,
                                    MatID,
                                    (GXPolygonMode)(mdlTimer / 60 % 2));

        //
        // ���f���̃J�����[�h��ω������܂��B
        //
        NNS_G3dMdlSetMdlCullMode(pMdl,
                                 MatID,
                                 (GXCull)(mdlTimer / 73 % 2 + 1));

        //
        // ���f���̃|���S��ID��ݒ肵�܂��B
        //
        NNS_G3dMdlSetMdlPolygonID(pMdl,
                                  MatID,
                                  31);

        //
        // ���f���̃���ω������܂��B
        //
        NNS_G3dMdlSetMdlAlpha(pMdl,
                              MatID,
                              mdlTimer / 7 % 32);
    }
}


/* -------------------------------------------------------------------------
  Name:         calcNextColor

  Description:  �}�e���A���̐F��ω������邽��,���̐F���玟�̐F���v�Z����

  Arguments:    ���̐F:GXRgb rgb

  Returns:      ���̐F:GXRgb
   ------------------------------------------------------------------------- */
GXRgb
calcNextColor(GXRgb rgb) {
    //  �e�����̒��o
    u16 R = (u16)((rgb & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT);
    u16 G = (u16)((rgb & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT);
    u16 B = (u16)((rgb & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT);
    
    const int ChangeSpeed = 2;
    
    R += ChangeSpeed;
    if(R > 31)
    {
        R -= 31;
        G += ChangeSpeed;
        if(G > 31)
        {
            G -= 31;
            B += ChangeSpeed;
            if(B > 31)
            {
                B -= ChangeSpeed;
            }
        }
    }

    return GX_RGB(R, G, B);
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

    //
    // G3D:
    // �O���[�o�����C�g�̐ݒ�
    // ------------------------------------------------------------------------
    NNS_G3dGlbLightVector(GX_LIGHTID_0, -FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
    NNS_G3dGlbLightVector(GX_LIGHTID_1,  FX16_SQRT1_3,  FX16_SQRT1_3,  FX16_SQRT1_3);
    NNS_G3dGlbLightVector(GX_LIGHTID_2,             0,   -FX16_ONE  ,             0);
    NNS_G3dGlbLightVector(GX_LIGHTID_3,             0,    FX16_ONE-1,             0);

    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    NNS_G3dGlbLightColor(GX_LIGHTID_1, GX_RGB(10, 10, 10));
    NNS_G3dGlbLightColor(GX_LIGHTID_2, GX_RGB(16,  0,  0));
    NNS_G3dGlbLightColor(GX_LIGHTID_3, GX_RGB(16, 16,  0));

    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(20, 10, 10), GX_RGB(6, 6, 6), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(10, 16, 0), GX_RGB(6, 6, 14), FALSE);
    
    //  �O���[�o���|���S�������̐ݒ�
    NNS_G3dGlbPolygonAttr(
            0x000f,                                   // ���C�g�͑S��ON
            GX_POLYGONMODE_MODULATE,
            GX_CULL_BACK,
            0,
            28,
            GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE
    );
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
    BOOL status;
    NNSG3dResMdl* model = NULL;
    NNSG3dResTex* texture = NULL;
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
