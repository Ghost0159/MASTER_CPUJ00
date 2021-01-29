/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - RecordMtx
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
// callback2�T���v���Ɠ��l�̓�����R�[���o�b�N���g�p�����Ɏ�������T���v��
//
// g3dcvtr��-s�I�v�V�������g�p����ƁA�v�Z�����s���K���s��X�^�b�N�Ɋi�[����
// �悤�ɂ��邱�Ƃ��ł��܂��B���̂悤�ɂ���ƁA�`���ɃX�^�b�N�Ɋi�[�����
// ����s����J�����g�s��Ƀ��X�g�A���邱�Ƃɂ��A�K�v�ȃW���C���g�̍s���
// ���邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// �l���������{�b�g������Ŏ����đ����Ă��܂��Bcallback2�T���v���Ɠ��l�̓�������܂��B
//
// HOWTO:
// 1: .imd�t�@�C����-s�I�v�V���������ăR���o�[�g���܂��B
// 2: NNS_G3dDraw�ő���l��`�悵�܂��B
// 3: NNS_G3dGetResultMtx�ŕK�v�ȃW���C���g��jntID���w�肵�āA
//    �s����J�����g�s��Ƀ��X�g�A���܂��B
// 4: NNS_G3dDraw�֐��Ń��{�b�g��`�悷��ƁA����l�̍��肪���{�b�g�������Ă���
//    (���{�b�g���������Ă���j�悤�ɂȂ�܂��B
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
    NNSG3dRenderObj object2;
    NNSG3dResMdl*   pModel;
    void*           pAnmRes;
    NNSG3dResMdl*   pModel2;
    void*           pAnmRes2;
    NNSFndAllocator allocator;
    NNSG3dAnmObj*   pAnmObj;
    NNSG3dAnmObj*   pAnmObj2;

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

    pModel2 = LoadG3DModel("data/robot_t.nsbmd");
    SDK_ASSERTMSG(pModel2, "load failed");

    //
    // G3D: NNSG3dRenderObj�̏�����
    //
    // NNSG3dRenderObj�͂P�̃��f���Ƃ��̃��f���ɓK�p�����A�j���Ɋւ���e�����
    // �ێ�����\���̂ł��B
    // �����̃����o�|�C���^�Ŏw�肳���̈�̓��[�U�[���g�Ŏ擾�E�������K�v������܂��B
    // ������mdl��NULL�ł����Ă����܂��܂���(NNS_G3dDraw���ɂ͐ݒ肳��Ă���K�v������)�B
    //
    NNS_G3dRenderObjInit(&object, pModel);
    NNS_G3dRenderObjInit(&object2, pModel2);
    
    //
    // �W���C���g�A�j����ǂݍ���Ŋ֘A�t�����Ă݂�
    //
    {
        //
        // ����l
        //
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

            // �K�v�ʂ̃��������A���P�[�g����B�C�j�V�����C�Y�͕ʓr�K�v�ɂȂ�B
            pAnmObj = NNS_G3dAllocAnmObj(&allocator, // �g�p����A���P�[�^���w��
                                         pAnmRes,    // �A�j���[�V�������\�[�X���w��
                                         pModel);    // ���f�����\�[�X���w��
            SDK_NULL_ASSERT(pAnmObj);

            //
            // AnmObj ������������B�W���C���g�A�j���[�V�����ȊO�����l
            //
            NNS_G3dAnmObjInit(pAnmObj, // �A�j���[�V�����I�u�W�F�N�g�ւ̃|�C���^
                              pAnmRes, // �A�j�����\�[�X�ւ̃|�C���^
                              pModel,  // NNSG3dResMdl�ւ̃|�C���^
                              NULL );  // NNSG3dResTex�ւ̃|�C���^(�e�N�X�`���p�^�[���A�j���ȊO�Ȃ��NULL�ł��悢)

            //
            // RenderObj �� AnmObj ��ǉ�����B�W���C���g�A�j���[�V�����ȊO�����l
            //
            NNS_G3dRenderObjAddAnmObj( &object, pAnmObj );
        }
    }

    {
        //
        // �������{�b�g
        //
        const u8* pFile = G3DDemo_LoadFile("data/robot_t.nsbca");
        SDK_ASSERTMSG( pFile, "anm load failed" );

        pAnmRes2 = NNS_G3dGetAnmByIdx(pFile, 0);
        SDK_NULL_ASSERT(pAnmRes2);

        pAnmObj2 = NNS_G3dAllocAnmObj(&allocator,
                                      pAnmRes2,
                                      pModel2);
        SDK_NULL_ASSERT(pAnmObj2);

        NNS_G3dAnmObjInit(pAnmObj2,
                          pAnmRes2,
                          pModel2,
                          NULL);

        NNS_G3dRenderObjAddAnmObj(&object2, pAnmObj2);
    }
    
    //
    // �f�����ʂ̏�����
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "RecordMtx");

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

        // ���{�b�g�����₷�����邽�߂ɐl�̃A�j���[�V������x�����Ă���
        pAnmObj->frame += (FX32_ONE >> 1);
        if( pAnmObj->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj))
        {
            pAnmObj->frame = 0;
        }

        // ���{�b�g�������Ă���̂�������悤�Ƀ��{�b�g�̃A�j���[�V�����𑬂����Ă���
        pAnmObj2->frame += FX32_ONE * 2;
        if ( pAnmObj2->frame >= NNS_G3dAnmObjGetNumFrame(pAnmObj2))
        {
            pAnmObj2->frame = 0;
        }

        //  ���ԑ���J�n
        time = OS_GetTick();

        {
            //
            // ����l�̕`��
            //

            VecFx32 scale = { FX32_ONE>>2, FX32_ONE>>2, FX32_ONE>>2 };
            NNS_G3dGlbSetBaseScale(&scale);
            //
            // G3D:
            // G3dGlbXXXX�Őݒ肵����Ԃ��܂Ƃ߂ē]�����܂��B
            // �J������ˉe�s�񓙂��ݒ肳��Ă��܂��B
            //
            NNS_G3dGlbFlushP();

            {
                //
                // G3D: NNS_G3dDraw
                // �Z�b�g�A�b�v���ꂽNNSG3dRenderObj��n�����Ƃɂ��
                // ������ꍇ�̕`����s��
                //
                NNS_G3dDraw(&object);
            }
        }
 
        {
            //
            // �l�̍���Ɏ�����ĕ������{�b�g�̕`��
            //

            // ���̏ꍇ�ANNS_G3dGlbFlushP�͌Ă΂Ȃ��Ă��悢

            //
            // �l��.imd�t�@�C����ϊ�����ۂ�-s�I�v�V���������Ă���̂�
            // �`�掞�Ɍv�Z���ꂽ�s��͑S�čs��X�^�b�N�Ɋi�[����Ă��܂�
            // (���̂��߃m�[�h�̐���30�ȉ��łȂ��Ă̓R���o�[�g�ł��Ȃ�)�B
            // callback2�T���v���Ƃ͈Ⴂ�A�`�撆�ɍs����擾����K�v�͂Ȃ��A
            // ���̂��߃R�[���o�b�N���g���K�v������܂���B
            //
            int jntID;
            BOOL result;

            // ����̃W���C���gID���擾����B
            NNS_G3D_GET_JNTID(object.resMdl, &jntID, "wrist_l_end");

            //
            // �����_�����O�I�u�W�F�N�g��jntID�m�[�h�ɑΉ�����s����J�����g�s��Ɏ����Ă���
            // �Q�Ԗڂ̈�����MtxFx43�ւ̃|�C���^���w�肷��ƈʒu���W�s�񂪁A
            // �R�Ԗڂ̈�����MtxFx33�ւ̃|�C���^���w�肷��ƕ����x�N�g���s�񂪓�����B
            //
            result = NNS_G3dGetResultMtx(&object, NULL, NULL, (u32)jntID);

            // jntID�m�[�h�ɑΉ�����s�񂪂Ȃ������ꍇ��FALSE���Ԃ��Ă���̂ŔO�̂��߃`�F�b�N
            SDK_ASSERT(result);

            //
            // �`�撼�O�ɂ���posScale�ł̃X�P�[�����O�͂�����Ȃ��̂ŁA
            // �����I�ɃX�P�[�����O���Ȃ��Ă͂Ȃ�Ȃ��B
            //
            NNS_G3dGeScale(object.resMdl->info.posScale, object.resMdl->info.posScale, object.resMdl->info.posScale);

            NNS_G3dDraw(&object2);
        }
        
        
        // ���ԑ��茋��
        time = OS_GetTick() - time;

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
