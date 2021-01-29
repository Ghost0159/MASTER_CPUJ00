/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ShadowVolume
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
// �V���h�E�{�����[���\���T���v��
//
// G3D���g�p���ăV���h�E�{�����[����`�悷����@��Ꭶ���܂��B
//
// �T���v���̓���
// �r�W�r���e�B�A�j���[�V�������J��Ԃ��Đ�����Ă���I�u�W�F�N�g�ɁA
// �V���h�E�{�����[����p���ĉe���f���܂��B
//
// HOWTO:
// 1: G3_SwapBuffers�Ń}�j���A���\�[�g���[�h���w�肵�Ă��邱�Ƃ��m�F���܂��B
// 2: �������f�����A�}�X�N�p�V���h�E�|���S���̃}�e���A���ݒ�ɂ��ĕ`�悵�A
//    ���̌�A�`��p�V���h�E�|���S���̃}�e���A���ݒ�ɂ��ĕ`�悵�܂��B
//    �ڂ����̓T���v������DrawShadowVolume�֐����̃R�����g���Q�Ƃ��Ă��������B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


/* -------------------------------------------------------------------------
    DrawShadowVolume

    �V���h�E�{�����[����`�悷��B
 ------------------------------------------------------------------------- */
static void
DrawShadowVolume(NNSG3dResMdl* p, BOOL isShadow)
{
    //
    // �V���h�E�{�����[���̕`��
    //
    // �܂��A�P�ł��V���h�E�{�����[����`�悷��ꍇ�ɂ́A�K���}�j���A���\�[�g���[�h
    // �ɂ��Ă��������B
    // �܂�A
    // G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W)��
    // G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z)���g�p
    //
    // �P�̃��f�����A�}�X�N�p�V���h�E�|���S���̃}�e���A���ݒ�ɂ��ĕ`�悵�A
    // ���ɓ������f����`��p�V���h�E�|���S���̃}�e���A���ݒ�ɂ��ĕ`�悷��ƁA
    // ���̂ɉe�𗎂Ƃ����Ƃ��ł��܂��B
    // �Ȃ��A�V���h�E�{�����[���p�̃��f���́A�e���f�����̂��ђʂ��Ă���K�v������܂��B
    //
    {
        // �ʒu����
        VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
        VecFx32 trans = {0, 5 * FX32_ONE, 0};
        NNS_G3dGlbSetBaseScale(&scale);
        NNS_G3dGlbSetBaseTrans(&trans);
        NNS_G3dGlbFlushP();
        trans.y = 0;
        NNS_G3dGlbSetBaseTrans(&trans);
    }

    if (isShadow)
    {
        {
            //
            // �}�X�N�p�V���h�E�|���S����`�悵�܂��B
            // �|���S��ID��0, ����1-30�C�|���S�����[�h��GX_POLYGONMODE_SHADOW
            // �ɐݒ肵�āA���ʂ���(GX_CULL_FRONT)��`�悵�܂��B
            //
            NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
            NNS_G3dMdlSetMdlPolygonID( p, 0, 0 );                       // �|���S��ID��0�ɐݒ�
            NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_FRONT );            // �w�ʂ݂̂�`��
            NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
            NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_SHADOW ); // �V���h�E�|���S���g�p

            // matID=0, shpID=0��`��(�}�e���A���̑��M�͏ȗ����Ȃ�)
            NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
        }

        {
            //
            // �����ꏊ�ɕ`��p�V���h�E�|���S����`�悵�܂��B
            // �|���S��ID��1-63�C����1-30�C�|���S�����[�h��GX_POLYGONMODE_SHADOW
            // �ɐݒ肵�āA����(GX_CULL_NONE)��`�悵�܂��B
            //
            // GX_CULL_BACK�̐ݒ肾�ƁA�}�X�N�p�V���h�E�|���S�����͂ݏo���P�[�X�����݂�
            // �������`�悳��Ȃ��ꍇ������܂��B
            //
            // �|���S��ID�́A�e���\�������I�u�W�F�N�g�Ƃ͈Ⴄ���̂ɂ��Ă��������B
            // �|���S��ID�������ꍇ�́A�e�͕`�悳��܂���B�e�𗎂Ƃ��I�u�W�F�N�g���g��
            // �e��\�����������Ȃ��ꍇ�́A�e�𗎂Ƃ��I�u�W�F�N�g�ƕ`��p�V���h�E�|���S����
            // �|���S��ID�𓯂��ɂ��Ă��������B
            //
            NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
            NNS_G3dMdlSetMdlPolygonID( p, 0, 1 );                       // �|���S��ID��1-63�ɐݒ�
            NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_NONE );             // ���ʕ`��
            NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
            NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_SHADOW ); // �V���h�E�|���S���g�p

            // matID=0, shpID=0��`��(�}�e���A���̑��M�͏ȗ����Ȃ�)
            NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
        }
    }
    else
    {
        //
        // �V���h�E�{�����[���Ƃ��Ăł͂Ȃ����ʂ̔������|���S���Ƃ��ĕ`�悵�܂��B
        // ���̃T���v���Ŏg�p���Ă��郂�f���͉~���`�ł��B
        // 
        NNS_G3dMdlSetMdlLightEnableFlag( p, 0, 0 );
        NNS_G3dMdlSetMdlPolygonID( p, 0, 0 );
        NNS_G3dMdlSetMdlCullMode( p, 0, GX_CULL_BACK ); 
        NNS_G3dMdlSetMdlAlpha( p, 0, 10 );
        NNS_G3dMdlSetMdlPolygonMode( p, 0, GX_POLYGONMODE_MODULATE );

        // matID=0, shpID=0��`��(�}�e���A���̑��M�͏ȗ����Ȃ�)
        NNS_G3dDraw1Mat1Shp(p, 0, 0, TRUE);
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
    NNSFndAllocator allocator;
    
    NNSG3dRenderObj object;
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSG3dAnmObj*   pAnmObj;
    BOOL isShadow = TRUE;

    //
    // �V���h�E�{�����[���͕��ʂP�}�e���A���P�V�F�C�v�ō\�������̂�
    // NNS_G3dDraw1Mat1Shp�ŕ`��ł��܂��B�]���āANNSG3dRenderObj�͕s�v�Ȃ̂�
    // NNSG3dResMdl�������Z�b�g�A�b�v���邾���ŏ\���ł��B
    //
    NNSG3dResMdl*   pShadow;

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

    pModel = LoadG3DModel("data/brother.nsbmd");
    SDK_ASSERTMSG(pModel, "load failed");

    pShadow = LoadG3DModel("data/shadow.nsbmd");
    SDK_ASSERTMSG(pShadow, "load failed");

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
    // �r�W�r���e�B�A�j����ǂݍ���Ŋ֘A�t������B
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/brother.nsbva");
        SDK_ASSERTMSG( pFile, "anm load failed" );
        {
            //
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

            // �K�v�ʂ̃��������A���P�[�g����B�C�j�V�����C�Y�͕ʓr�K�v�ɂȂ�B
            pAnmObj = NNS_G3dAllocAnmObj(&allocator, // �g�p����A���P�[�^���w��
                                         pAnmRes,    // �A�j���[�V�������\�[�X���w��
                                         pModel);    // ���f�����\�[�X���w��
            SDK_NULL_ASSERT(pAnmObj);

            //
            // AnmObj������������B
            //
            NNS_G3dAnmObjInit(pAnmObj, // �A�j���[�V�����I�u�W�F�N�g�ւ̃|�C���^
                              pAnmRes, // �A�j���[�V�������\�[�X�ւ̃|�C���^
                              pModel,  // NNSG3dResMdl�ւ̃|�C���^
                              NULL );  // NNSG3dResTex�ւ̃|�C���^(�e�N�X�`���p�^�[���A�j���ȊO�Ȃ��NULL�ł��悢)

            //
            // RenderObj �� AnmObj ��ǉ�����B
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );
        }
    }
    
    //
    // �f�����ʂ̏�����
    //
    G3DDemo_InitCamera(&gCamera, 2*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ShadowVolume");

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

        // �A�j���t���[�����X�V����
        pAnmObj->frame += FX32_ONE;

        if (pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            isShadow = isShadow ? FALSE : TRUE;
        }

        time = OS_GetTick();
        {
            {
                // �X�P�[���������đ傫������
                VecFx32 scale = {FX32_ONE << 1, FX32_ONE << 1 , FX32_ONE << 1};
                NNS_G3dGlbSetBaseScale(&scale);

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
                NNS_G3dDraw(&object);
            }

            DrawShadowVolume(pShadow, isShadow);
        }
        time = OS_GetTick() - time;

        {
            //
            // G3D: �ʏ��G3_XXX���ĂԂƂ��́A���̑O��NNS_G3dGeComFlushBuffer()
            //      ���R�[�����ē������Ȃ��Ă͂Ȃ�Ȃ�
            //
            NNS_G3dGlbFlushP();
            NNS_G3dGeFlushBuffer();
            G3DDemo_DrawGround(&gGround);
        }

        //
        // NOTICE:
        // �V���h�E�|���S�����g�p����ꍇ�͕K���}�j���A���\�[�g���[�h�ɂ��Ă�������
        // �I�[�g�\�[�g�ɂ����ꍇ�A�������|���S���̕`�揇�������[�U�[�����肷�邱�Ƃ�
        // �ł��Ȃ��̂ŁA�V���h�E�{�����[���𐳂����`�悷�邱�Ƃ��ł��܂���B
        //
        G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);

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
