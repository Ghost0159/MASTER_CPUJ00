/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - RecordJoint
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
//
// �W���C���g�̌v�Z���ʂ�O�v�Z���Ă����T���v��
//
// �W���C���g�̌v�Z���ʂ��o�b�t�@���Ă������ƂŁA�W���C���g���s�ς̏ꍇ��CPU��
// �v�Z�R�X�g��ߖ񂷂邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// �r�W�r���e�B�A�j���[�V�������J��Ԃ��Đ�����܂��B
// A�{�^���������ƃo�b�t�@���ꂽ�W���C���g�v�Z���ʂ��g�p���邩�A
// �`�斈�Ɍv�Z���Ȃ��������؂�ւ��܂��B
//
// HOWTO:
// 1: NNS_G3dAllocRecBufferJnt�Ńo�b�t�@�̗̈���m�ۂ��܂��B
// 2: NNS_G3dRenderObjSetJntAnmBuffer�Ńo�b�t�@�������_�����O�I�u�W�F�N�g��
//    �A�^�b�`���܂��B
// 3: NNS_G3dRenderObjSetFlag��NNS_G3D_RENDEROBJ_FLAG_RECORD�𗧂Ă܂��B
//    ���̃t���O�𗧂Ă�NNS_G3dDraw���R�[�������ꍇ�A�o�b�t�@�Ɍv�Z���ʂ��i�[����܂��B
// 4: �܂��ANNS_G3D_RENDEROBJ_FLAG_NOGECMD�𗧂ĂĂ�����NNS_G3dDraw�̎��s����
//    �W�I���g���G���W���Ƀf�[�^�����M����܂���B
// 5: NNS_G3dDraw�����s���āA�o�b�t�@�Ƀf�[�^���i�[���܂��B
// 6: NNS_G3dRenderObjResetFlag��NNS_G3D_RENDEROBJ_FLAG_RECORD��(NNS_G3D_RENDEROBJ_FLAG_NOGECMD)
//    �����Z�b�g���܂��B
// 7: �����NNS_G3dDraw����W���C���g�f�[�^���v�Z�������Ƀo�b�t�@�̃f�[�^���Q�Ƃ��ĕ`�悵�܂��B
// 8: NNS_G3dRenderObjReleaseJntAnmBuffer�Ńo�b�t�@�����O�����Ƃ��ł��܂��B
//
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
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
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;
    NNSG3dJntAnmResult* pBufJntAnmResult;
    BOOL useRecord = TRUE;

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
    // �r�W�r���e�B�A�j����ǂݍ���Ŋ֘A�t�����Ă݂�
    //
    {
        const u8* pFile = G3DDemo_LoadFile("data/brother.nsbva");
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

            // �K�v�ʂ̃��������A���P�[�g����B�C�j�V�����C�Y�͕ʓr�K�v�ɂȂ�B
            pAnmObj = NNS_G3dAllocAnmObj(&allocator, // �g�p����A���P�[�^���w��
                                         pAnmRes,    // �A�j���[�V�������\�[�X���w��
                                         pModel);    // ���f�����\�[�X���w��
            SDK_NULL_ASSERT(pAnmObj);

            //
            // AnmObj ������������B
            //
            NNS_G3dAnmObjInit(pAnmObj, // �A�j���[�V�����I�u�W�F�N�g�ւ̃|�C���^
                              pAnmRes, // �A�j�����\�[�X�ւ̃|�C���^
                              pModel,  // NNSG3dResMdl�ւ̃|�C���^
                              NULL );  // NNSG3dResTex�ւ̃|�C���^(�e�N�X�`���p�^�[���A�j���ȊO�Ȃ��NULL�ł��悢)

            //
            // RenderObj �� AnmObj ��ǉ�����B
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );
        }

        {
            //
            // �W���C���g�v�Z���ʂ̃o�b�t�@���m�ۂ��A�����_�����O�I�u�W�F�N�g�ɂ���
            //
            pBufJntAnmResult = NNS_G3dAllocRecBufferJnt(&allocator, pModel);
            NNS_G3dRenderObjSetJntAnmBuffer(&object, pBufJntAnmResult);

            //
            // �W���C���g�̌v�Z���ʂ��o�b�t�@�ɋL�^����B
            //
            NNS_G3dRenderObjSetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_NOGECMD); // �W�I���g���G���W���ɑ��M���Ȃ�
            NNS_G3dRenderObjSetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_RECORD);  // pBufJntAnmResult�Ƀf�[�^���L�^

            NNS_G3dDraw(&object);

            NNS_G3dRenderObjResetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_NOGECMD); // �W�I���g���G���W���ɑ��M����
            NNS_G3dRenderObjResetFlag(&object, NNS_G3D_RENDEROBJ_FLAG_RECORD);  // pBufJntAnmResult�̃f�[�^���g�p
        }

    }
    
    //
    // �f�����ʂ̏�����
    //
    G3DDemo_InitCamera(&gCamera, 0, 5*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "RecordJoint");

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
        // �A�j���t���[�����X�V����
        //
        pAnmObj->frame += FX32_ONE;
        if( pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        if (G3DDEMO_IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            //
            // A�{�^���������Ɛ؂�ւ��
            //
            if (useRecord)
            {
                useRecord = FALSE;
                (void)NNS_G3dRenderObjReleaseJntAnmBuffer(&object);
            }
            else
            {
                useRecord = TRUE;
                NNS_G3dRenderObjSetJntAnmBuffer(&object, pBufJntAnmResult);
            }
        }

        if (useRecord)
        {
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "use recorded joint data    ");
        }
        else
        {
            G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "calc joint data every frame");
        }

        {
            VecFx32 scale = { FX32_ONE>>1, FX32_ONE>>1, FX32_ONE>>1 };
            NNS_G3dGlbSetBaseScale(&scale);
            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            //  ���ԑ���J�n
            time = OS_GetTick();
            //
            // G3D: NNS_G3dDraw
            // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
            // ������ꍇ�̕`����s��
            //
            NNS_G3dDraw(&object);

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

        //
        // ���ԑ��茋�ʉ�ʕ\���B
        //
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
