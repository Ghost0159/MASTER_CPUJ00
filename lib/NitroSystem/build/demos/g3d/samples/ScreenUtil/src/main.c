/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - samples - ScreenUtil
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
// G3D�̍��W�ϊ����[�e�B���e�BAPI���g�����T���v��
//
// BG�X�N���[�����W�Ƃ̍��W�ϊ����s�����ƂŁA���[���h���W��BG�X�N���[�����
// �ǂ̓_�ɑ�������̂��ABG�X�N���[�����W��̓_�����[���h���W�̂ǂ̐��ɑ���
// ����̂������߂邱�Ƃ��ł��܂��B
//
// �T���v���̓���
// ���[���h���W�n�̌��_�ɑΉ�����BG�X�N���[����̍��W��\�����A
// ����l�̊e�W���C���g�̈ʒu�ɑΉ�����BG�X�N���[����̍��W��\�����܂��B
// �^�b�`�p�l����G��ƁA�G�����ʒu��BG�X�N���[����̍��W�ƁA
// ����ɑ�������Near�v���[����̓_��Far�v���[����̓_��
// ���[���h���W�n�ŕ\�����܂��B
//
// HOWTO:
// 1: NNS_G3dWorldPosToScrPos�֐����g�p����ƃ��[���h���W�n�̓_��BG�X�N���[��
//    ���W�n�̓_�ɕϊ����邱�Ƃ��ł��܂��B
// 2: NNS_G3dLocalOriginToScrPos�֐����g�p����ƃ��[�J�����W�n�̌��_��
//    BG�X�N���[�����W�n�̓_�ɕϊ����邱�Ƃ��ł��܂��B
// 3: NNS_G3dScrPosToWorldLine�֐����g�p�����BG�X�N���[�����W�n�̓_��
//    ���[���h���W�n�̐��ɕϊ����邱�Ƃ��ł��܂��B����2�_�Ƃ��ĕԂ���A
//    ���ꂼ��ANEAR�N���b�v��, FAR�N���b�v�ʏ�̓_�ɂȂ�܂��B
// 
//---------------------------------------------------------------------------

#include "g3d_demolib.h"

G3DDemoCamera gCamera;        // �J�����\���́B
G3DDemoGround gGround;        // �O���E���h�\���́B

static void          InitG3DLib(void);
static NNSG3dResMdl* LoadG3DModel(const char* path);
static void          SetCamera(G3DDemoCamera* camera);


#define NUMJNT 24
struct
{
    BOOL isOnScreen;
    int  x;
    int  y;
}
ScrPosJnt[NUMJNT];

//
// �e�W���C���g��BG�X�N���[����̈ʒu���擾
//
static void
getScrPos(NNSG3dRS* rs)
{
    int jntID;
    int x, y, r;

    jntID = NNS_G3dRSGetCurrentNodeDescID(rs);
    r = NNS_G3dLocalOriginToScrPos(&x, &y);
    ScrPosJnt[jntID].isOnScreen = (!r);
    ScrPosJnt[jntID].x = x;
    ScrPosJnt[jntID].y = y;
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
    NNSG3dAnmObj*   pAnmObj;

    G3DDemo_InitSystem();
    G3DDemo_InitMemory();
    G3DDemo_InitVRAM();

    InitG3DLib();
    G3DDemo_InitDisplay();

    //
    // ���̃T���v���ł̓^�b�`�p�l�����g�p���邽�߃^�b�`�p�l���̏��������s��
    //
    {
        BOOL result;
        TPCalibrateParam calibrate;

        // �^�b�`�p�l��API�̃C�j�V�����C�Y
        TP_Init();

        // �L�����u���[�V�����̃p�����[�^���t���b�V���������������Ă���
        result = TP_GetUserInfo( &calibrate );

        if (!result)
        {
            // �L���Ȓl�������Ă��Ȃ������烆�[�U�[�ɃL�����u���[�g��
            // ���肢���ăv���O�������I��
            OS_Printf("Calibration please\n");
            return;
        }

        // �L�����u���[�V�����̃p�����[�^���^�b�`�p�l��API�ɃZ�b�g
        TP_SetCalibrateParam( &calibrate );
    }

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

            //
            // �R�[���o�b�N��ݒ肷��
            //
            NNS_G3dRenderObjSetCallBack(&object,
                                        &getScrPos,
                                        NULL,
                                        NNS_G3D_SBC_NODEDESC,
                                        NNS_G3D_SBC_CALLBACK_TIMING_C);
        }
    }
    
    //
    // �f�����ʂ̏�����
    //
    G3DDemo_InitCamera(&gCamera, 10*FX32_ONE, 16*FX32_ONE);
    G3DDemo_InitGround(&gGround, (fx32)(1.5*FX32_ONE));

    G3DDemo_InitConsole();
    G3DDemo_Print(0,0, G3DDEMO_COLOR_YELLOW, "ScreenUtil");

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

        {
            int x, y, r;
            VecFx32 worldOrig = {0, 0, 0};
            r = NNS_G3dWorldPosToScrPos(&worldOrig, &x, &y);
            if (!r)
            {
                G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "World Origin -> (%3d,%3d)", x, y);
            }
            else
            {
                G3DDemo_Printf(0, 2, G3DDEMO_COLOR_GREEN, "World Origin -> ---------");
            }
        }

        {
            TPData tp;
            if (!TP_RequestCalibratedSampling(&tp) &&
                tp.touch                           &&
                tp.validity == TP_VALIDITY_VALID)
            {
                VecFx32 near, far;
                int x, y;
                x = tp.x;
                y = tp.y;
                G3DDemo_Printf(0, 5, G3DDEMO_COLOR_GREEN, "Touch = (%3d, %3d)", x, y);

                (void)NNS_G3dScrPosToWorldLine(x, y, &near, &far);
                G3DDemo_Printf(0, 6, G3DDEMO_COLOR_GREEN, "Point on near plane(World)");
                G3DDemo_Printf(0, 7, G3DDEMO_COLOR_GREEN, "  (%7.2f, %7.2f, %7.2f)", near.x / 4096.f, near.y / 4096.f, near.z / 4096.f);
                
                G3DDemo_Printf(0, 8, G3DDEMO_COLOR_GREEN, "Point on far plane(World)");
                G3DDemo_Printf(0, 9, G3DDEMO_COLOR_GREEN, "  (%7.2f, %7.2f, %7.2f)", far.x / 4096.f, far.y / 4096.f, far.z / 4096.f);
            }
            else
            {
                G3DDemo_Printf(0, 5, G3DDEMO_COLOR_GREEN, "No touch          ");
                G3DDemo_Printf(0, 6, G3DDEMO_COLOR_GREEN, "                          ");
                G3DDemo_Printf(0, 7, G3DDEMO_COLOR_GREEN, "                                ");
                G3DDemo_Printf(0, 8, G3DDEMO_COLOR_GREEN, "                          ");
                G3DDemo_Printf(0, 9, G3DDEMO_COLOR_GREEN, "                                ");
            }
        }

        {
            //
            // �e�W���C���g�̃X�N���[����̈ʒu��\��
            //
            int i;
            for (i = 0; i < NUMJNT / 2; i++)
            {
                if (ScrPosJnt[2*i].isOnScreen)
                    G3DDemo_Printf( 0, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d (%3d,%3d)", 2*i, ScrPosJnt[2*i].x, ScrPosJnt[2*i].y);
                else
                    G3DDemo_Printf( 0, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d ---------", 2*i);

                if (ScrPosJnt[2*i+1].isOnScreen)
                    G3DDemo_Printf(16, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d (%3d,%3d)", 2*i+1, ScrPosJnt[2*i+1].x, ScrPosJnt[2*i+1].y);
                else
                    G3DDemo_Printf(16, i + 11, G3DDEMO_COLOR_GREEN, "Jnt%02d ---------", 2*i+1);
            }
        }

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
