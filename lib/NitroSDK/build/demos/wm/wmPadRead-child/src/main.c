/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmPadRead-child
  File:     main.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2007/10/29 00:12:20  seiki_masashi
  small fix

  Revision 1.3  2007/10/27 14:54:14  seiki_masashi
  WH ���g�p����悤�ɕύX
  �}���`�u�[�g�����ۂɃ_�E�����[�h���̐e�@�ɐڑ��ɍs���悤�ɏC��
  �R�[�h�̐���

  Revision 1.2  2007/10/27 11:20:04  seiki_masashi
  �f�[�^�V�F�A�����O���g�p����悤�ɕύX

  Revision 1.1  2007/04/18 09:27:37  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//
// Wii �� SDK �Ɏ��^����Ă���f���ƒʐM���A�p�b�h���͂𑗐M����f���ł��B
//

#include <nitro.h>
#include "wh.h"

#include "data.h"
#include "tpdata.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define PICTURE_FRAME_PER_GAME_FRAME    1

/* ���̃f���Ŏg�p���� GGID */
#define MY_GGID SDK_MAKEGGID_SYSTEM(0x20)

#define DMA_NO  3

#define SAMPLING_BUFFER_SIZE ( 1024 * 1024 ) // 1M

#define FFT_NSHIFT      9
#define FFT_N           (1 << FFT_NSHIFT)

/*---------------------------------------------------------------------------*
    �ϐ���`
 *---------------------------------------------------------------------------*/

static WMBssDesc mbParentBssDesc ATTRIBUTE_ALIGN(32);
static BOOL isMultiBooted;

static GXOamAttr oamBak[128];

typedef struct MyPadData {
    u16 keyData;
    u8 touchPanel_x;
    u8 touchPanel_y;
    u8 mic;
    u8 touch;
    u8 padding[2];
} MyPadData;

// �\���p����M�o�b�t�@
static MyPadData gRecvData[1 + WH_CHILD_MAX]; // �e�@���z�M����S�v���C���[�̃^�b�`���
static BOOL gRecvFlag[1 + WH_CHILD_MAX];

u16     keyData;

static GXOamAttr gOam[128];

TPData  raw_point;
TPData  disp_point;
TPCalibrateParam calibrate;

static BOOL input_mic;
static MICAutoParam gMicAutoParam;
static u8 *gMicData;


// FFT �p�̃o�b�t�@�Q
static fx16 sinTable[FFT_N - FFT_N / 4];
#ifdef USE_FFTREAL
static fx16 sinTable2[(FFT_N - FFT_N / 4) / 2];
static fx32 data[FFT_N];
#else
static fx32 data[FFT_N * 2];
#endif
static s32 power[FFT_N/2+1];
static s32 smoothedPower[FFT_N/2+1];

/*---------------------------------------------------------------------------*
    �֐��錾
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void);
static void SetPoint16x16(int objNo, u16 pos_x, u16 pos_y, int charNo, int paletteNo);
static void Initialize(void);
static BOOL DoConnect(void);
static void DoPadSharing(void);
static BOOL CheckMicData(void *address);
static void ObjSet(int objNo, int x, int y, int charNo, int paletteNo);
static void VBlankIntr(void);


//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    Initialize();

    // ���L�f�[�^�̏�����
    MI_CpuClear8( gRecvData, sizeof(gRecvData) );

    // �������}���`�u�[�g����N�������q�@�ł��邩�ǂ������`�F�b�N���܂��B
    isMultiBooted = MB_IsMultiBootChild();

    if (isMultiBooted)
    {
        //--------------------------------------------------------------
        // �}���`�u�[�g�ŋN�������ꍇ�A��U���Z�b�g����ʐM���ؒf����܂��B
        // �u�[�g����u�[�g�����e�@��BssDesc��ێ����Ă��邽�߁A���̏����g����
        // �e�@�֍Đڑ����Ă��������B
        // ���̎��ABssDesc����MAC�A�h���X�݂̂����o����MAC�A�h���X�w���
        // �e�̃X�L��������ڑ����s�Ȃ��ꍇ�͓��ɖ�肠��܂��񂪁A�ێ�����Ă���
        // BssDesc���g���Ē��ɐe�@�ɐڑ����s�Ȃ��ꍇ�ɂ́A�ʐM�T�C�Y��]�����[�h��
        // ���炩���ߐe�q�̊Ԃō��킹�Đݒ肵�Ă����K�v������܂��B
        //--------------------------------------------------------------

        /* 
         * �e�@�ƍĐڑ����邽�߂ɐe�@�̏����擾���܂��B                   
         * �ڑ��ɗ��p����WMBssDesc��32�o�C�g�ɃA���C������Ă���K�v������܂��B
         * �e�@��MAC�A�h���X�ōăX�L�������邱�ƂȂ��ɍĐڑ�������ꍇ�́A
         * �e�@/�q�@�̍ő呗�M�T�C�Y�AKS/CS�t���O�͂��炩���ߍ��킹�Ă����Ă��������B
         * �ăX�L�������s�Ȃ��Ă���ڑ�����ꍇ�͂����̒l�͂��ׂ�0�ō\���܂���B
         */
        MB_ReadMultiBootParentBssDesc(&mbParentBssDesc,
                                      WH_PARENT_MAX_SIZE, // �e�@�ő呗�M�T�C�Y
                                      WH_CHILD_MAX_SIZE,  // �q�@�ő呗�M�T�C�Y
                                      0,   // �\��̈�(���0)
                                      0);  // �\��̈�(���0)
    }

    while (TRUE)
    {
        int connectedFlag = FALSE;

        // ����������
        (void)WH_Initialize();

        connectedFlag = DoConnect();
        // �����܂łŎq�@��ԂɈڍs����

        if (connectedFlag)
        {
            DoPadSharing();
            // �ʐM���[�h��������܂Ŗ߂��Ă��܂���
        }

        WH_Finalize();
    }
}

void Initialize(void)
{
    //================ ������
    //---- OS ������
    OS_Init();

    //---- TP ������
    TP_Init();

    // get CalibrateParameter from FlashMemory
    if (!TP_GetUserInfo(&calibrate))
    {
        OS_Panic("FATAL ERROR: can't read UserOwnerInfo\n");
    }
    else
    {
        OS_Printf("Get Calibration Parameter from NVRAM\n");
    }

    TP_SetCalibrateParam(&calibrate);

    //---- GX ������
    GX_Init();

    //================ �����ݒ�
    //---- �d���S�ăI��
    GX_SetPower(GX_POWER_ALL);

    //----  V�u�����N��������
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- V�u�����N�����ݒ�
    (void)GX_VBlankIntr(TRUE);

    //---- VRAM �N���A
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    //---- OAM�ƃp���b�g�N���A
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    //---- �o���N�`���n�a�i��
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);

    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);

    //---- �O���t�B�b�N�X�\�����[�h�ɂ���
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJ�̕\���̂�ON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);
    
    //---- 32K�o�C�g��OBJ��2D�}�b�v���[�h�Ŏg�p
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    //---- �f�[�^���[�h
    GX_LoadOBJ( sampleCharData, 0, sizeof(sampleCharData) );
    GX_LoadOBJPltt( samplePlttData, 0, sizeof(samplePlttData) );

    GXS_SetGraphicsMode(GX_BGMODE_0);    // BGMODE 0

    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GXS_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    /* Load character bitmap data */
    GXS_LoadOBJ( sampleCharData, 0, sizeof(sampleCharData) );
    GXS_LoadOBJPltt( samplePlttData, 0, sizeof(samplePlttData) );

    //---- ��\��OBJ�͉�ʊO�ֈړ�
    MI_DmaFill32(DMA_NO, oamBak, 0xc0, sizeof(oamBak));

    //================ ���̑�������

    /////////////////////
    // ����������
    InitializeAllocateSystem();

    /////////////////////
    // �}�C�N�֌W������

    //---- MIC ������
    MIC_Init();

    //---- PMIC ������
    PM_Init();
    // AMP�I��
    (void)PM_SetAmp(PM_AMP_ON);
    // AMP�Q�C������
    (void)PM_SetAmpGain(PM_AMPGAIN_80);

    // �I�[�g�T���v�����O�̐ݒ�
    // OS_Alloc�Ŋm�ۂ�����������32�o�C�gALIGN�ɂȂ��Ă���̂ŁA
    // �L���b�V�����삵�Ă����̃��������󂳂Ȃ��B
    gMicData = (u8 *)OS_Alloc(SAMPLING_BUFFER_SIZE);
    // �t�[���G�ϊ��̐��x���グ�邽�߂� 12bit �ŃT���v�����O���s��
    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
    gMicAutoParam.buffer = (void *)gMicData;
    gMicAutoParam.size = SAMPLING_BUFFER_SIZE;
    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
    gMicAutoParam.loop_enable = TRUE;
    gMicAutoParam.full_callback = NULL;
    (void)MIC_StartAutoSampling(&gMicAutoParam);

    /////////////////////
    // FFT �֘A

    MATH_MakeFFTSinTable(sinTable, FFT_NSHIFT);
#ifdef USE_FFTREAL
    MATH_MakeFFTSinTable(sinTable2, FFT_NSHIFT - 1);
#endif

    //================ �\���J�n
    OS_WaitVBlankIntr();
    GX_DispOn();

    GXS_DispOn();
}

BOOL DoConnect(void)
{
    s32 retry = 0;
    enum
    {
        MAX_RETRY = 5
    };

    ObjSet(0, 100, 80, 'C', 4);
    ObjSet(1, 110, 80, 'O', 4);
    ObjSet(2, 120, 80, 'N', 4);
    ObjSet(3, 130, 80, 'N', 4);
    ObjSet(4, 140, 80, 'E', 4);
    ObjSet(5, 150, 80, 'C', 4);
    ObjSet(6, 160, 80, 'T', 4);
    MI_CpuClear8( (GXOamAttr *)&oamBak[7], sizeof(GXOamAttr) * 12);

    //---- V�u�����N�����I���҂�
    OS_WaitVBlankIntr();

    // �}���`�u�[�g�̏ꍇ�͐e�@���\�����Ă��� GGID �𗘗p���܂��B
    //   �ʏ�́AMY_GGID �ƈ�v���Ă���͂��ł��B
    WH_SetGgid(isMultiBooted ? mbParentBssDesc.gameInfo.ggid : MY_GGID);
    retry = 0;

    while (TRUE)
    {
        // �ʐM��Ԃɂ�菈����U�蕪��
        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_CONNECT_FAIL:
            {
                // WM_StartConnect()�Ɏ��s�����ꍇ�ɂ�WM�����̃X�e�[�g���s���ɂȂ��Ă����
                // ��xWM_Reset��IDLE�X�e�[�g�Ƀ��Z�b�g����K�v������܂��B
                WH_Reset();
            }
            break;
        case WH_SYSSTATE_IDLE:
            {
                if (retry < MAX_RETRY)
                {
                    // �e�@���X�L�������Đڑ����܂��B
                    //   �}���`�u�[�g����Ă����ꍇ�́A�o���Ă����e�@�� BSSID �𗘗p����
                    //   �}���`�u�[�g���̐e�@�����Ɍq���ɍs���悤�ɂ��܂��B
                    (void)WH_ChildConnectAuto(WH_CONNECTMODE_DS_CHILD, isMultiBooted ? mbParentBssDesc.bssid : NULL, 0);
                    retry++;
                    break;
                }
                // MAX_RETRY �Őe�@�ɐڑ��ł��Ȃ����ERROR�\��
            }
        case WH_SYSSTATE_ERROR:
            return FALSE;

        case WH_SYSSTATE_DATASHARING:
            return TRUE;

        case WH_SYSSTATE_BUSY:
        case WH_SYSSTATE_SCANNING:
        case WH_SYSSTATE_CONNECTED:
        default:
            break;
        }

        OS_WaitVBlankIntr();
    }

    // can't reach here
    return FALSE;
}

void DoPadSharing(void)
{
    int myAid;
    myAid = WM_GetAID();

    //================ ���C�����[�v
    while (TRUE)
    {
        // Draw Marker by calibrated point.
        while (TP_RequestRawSampling(&raw_point) != 0)
        {
        };
        TP_GetCalibratedPoint(&disp_point, &raw_point);

        if (disp_point.touch)
        {
            SetPoint16x16(0, disp_point.x, disp_point.y, myAid + '0', 1);

            switch (disp_point.validity)
            {
            case TP_VALIDITY_VALID:
                OS_TPrintf("( %d, %d ) -> ( %d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_X:
                OS_TPrintf("( *%d, %d ) -> ( *%d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_Y:
                OS_TPrintf("( %d, *%d ) -> ( %d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_XY:
                OS_TPrintf("( *%d, *%d ) -> ( *%d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            }
        }
        //---- V�u�����N�����I���҂�
        OS_WaitVBlankIntr();

        //---- �p�b�h�f�[�^�ǂݍ���
        keyData = PAD_Read();

        {
            static MyPadData sendData ATTRIBUTE_ALIGN(32);
            int i;

            // ���M�f�[�^������
            MI_CpuClear8(&sendData, sizeof(sendData));
            sendData.keyData = (u16)(keyData | (PAD_DetectFold() ? PAD_DETECT_FOLD_MASK : 0));
            sendData.touch        = (u8)disp_point.touch;
            sendData.touchPanel_x = (u8)disp_point.x;
            sendData.touchPanel_y = (u8)disp_point.y;
            sendData.mic = (u8)input_mic;

            if ( WH_StepDS((void*)&sendData) )
            {
                for (i = 0; i < (1 + WH_CHILD_MAX); i++)
                {
                    u8* data;

                    data = (u8 *)WH_GetSharedDataAdr((u16)i);

                    if (data != NULL)
                    {
                        gRecvFlag[i] = TRUE;
                        MI_CpuCopy8(data, &gRecvData[i], sizeof(gRecvData[0]));
                    }
                    else
                    {
                        gRecvFlag[i] = FALSE;
                    }
                }
            }
        }

        //---- �p�b�h���� OBJ �ŕ\��
        ObjSet(0, 200, 90, 'A', (keyData & PAD_BUTTON_A) ? 1 : 2);
        ObjSet(1, 180, 95, 'B', (keyData & PAD_BUTTON_B) ? 1 : 2);

        ObjSet(2, 60, 50, 'L', (keyData & PAD_BUTTON_L) ? 1 : 2);
        ObjSet(3, 180, 50, 'R', (keyData & PAD_BUTTON_R) ? 1 : 2);

        ObjSet(4, 60, 80, 'U', (keyData & PAD_KEY_UP) ? 1 : 2);
        ObjSet(5, 60, 100, 'D', (keyData & PAD_KEY_DOWN) ? 1 : 2);
        ObjSet(6, 50, 90, 'L', (keyData & PAD_KEY_LEFT) ? 1 : 2);
        ObjSet(7, 70, 90, 'R', (keyData & PAD_KEY_RIGHT) ? 1 : 2);

        ObjSet(8, 130, 95, 'S', (keyData & PAD_BUTTON_START) ? 1 : 2);
        ObjSet(9, 110, 95, 'S', (keyData & PAD_BUTTON_SELECT) ? 1 : 2);

        ObjSet(10, 200, 75, 'X', (keyData & PAD_BUTTON_X) ? 1 : 2);
        ObjSet(11, 180, 80, 'Y', (keyData & PAD_BUTTON_Y) ? 1 : 2);

        //---- �܂��݌��m��Ԃ� OBJ �ŕ\��
        ObjSet(12, 120, 30, 'F', (PAD_DetectFold())? 1 : 2);

        ObjSet(13, 100, 5, 'A', 4);
        ObjSet(14, 110, 5, 'I', 4);
        ObjSet(15, 120, 5, 'D', 4);
        ObjSet(16, 130, 5, (myAid / 10) ? (myAid / 10) + '0' : ' ', 4);
        ObjSet(17, 140, 5, (myAid % 10) + '0', 4);

        // �}�C�N���̓`�F�b�N
        input_mic = CheckMicData(MIC_GetLastSamplingAddress());
        ObjSet(18, 120, 120, 'M', input_mic ? 1 : 2);

        if( WH_GetSystemState() != WH_SYSSTATE_DATASHARING )
        {
            break;
        }

        // ��M��񂩂�S�v���C���[�̃^�b�`����`��
        {
            int i;

            for(i = 0; i < WH_CHILD_MAX + 1; i++)
            {
                if( i != myAid && gRecvData[i].touch )
                {
                    SetPoint16x16(i+1, gRecvData[i].touchPanel_x, gRecvData[i].touchPanel_y, '0'+i, 2);
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  OBJ �Z�b�g
//
void ObjSet(int objNo, int x, int y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&oamBak[objNo],
                  x,
                  y,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}


//--------------------------------------------------------------------------------
//    �u�u�����N���荞�ݏ���
//
void VBlankIntr(void)
{
    //---- �n�`�l�̍X�V
    DC_FlushRange(oamBak, sizeof(oamBak));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    MI_DmaCopy32(DMA_NO, oamBak, (void *)HW_OAM, sizeof(oamBak));

    /* flush cache of OAM buffers to main memory */
    DC_FlushRange(gOam, sizeof(gOam));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(gOam, 0, sizeof(gOam));
    MI_DmaFill32(3, gOam, 192, sizeof(gOam));       // Clear OAM buffer

    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  ���C����������̃A���[�i�ɂă����������ăV�X�e��������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Init�͌Ă΂�Ă���Ƃ����O��
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         SetPoint16x16

  Description:  Display a 16x16 OBJ on indicated point.

  Arguments:    x - position X.
                y - position Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetPoint16x16(int objNo, u16 pos_x, u16 pos_y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&gOam[objNo],
                  pos_x - 8,
                  pos_y - 8,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}

/*---------------------------------------------------------------------------*
  Name:         CheckMicData

  Arguments:    address - mic sampling data.

  Returns:      Whether mic input exists.
 *---------------------------------------------------------------------------*/
static BOOL CheckMicData(void *address)
{
    s32     i;

    u16    *p;

    // �T���v�����O�������s���Ă��Ȃ��ꍇ�͉��������ɏI��
    // (�}�C�N�Ɩ��֌W�̃������̃L���b�V����j�����Ă��܂���)
    if ((address < gMicData) || (address >= (gMicData + SAMPLING_BUFFER_SIZE)))
    {
        return FALSE;
    }

    // ���߂� FFT_N �̃T���v�����O�l�ɑ΂��� FFT ��K�p
    // 12bit �T���v�����O�͊e�l��2�o�C�g�ƂȂ�
    p = (u16 *)((u32)address - (FFT_N-1)*2);
    if ((u32)p < (u32)gMicData)
    {
        p = (u16 *)((u32)p + SAMPLING_BUFFER_SIZE);
    }
    DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
    for (i = 0; i < FFT_N; i++)
    {
#ifdef USE_FFTREAL
        data[i] = ((*p) << (FX32_SHIFT - (16 - 12)));
#else
        data[i * 2] = ((*p) << (FX32_SHIFT - (16 - 12)));
        data[i * 2 + 1] = 0; // �����ɂ� 0 ����
#endif
        p++;
        if ((u32)p >= (u32)(gMicData + SAMPLING_BUFFER_SIZE))
        {
            p = (u16 *)((u32)p - SAMPLING_BUFFER_SIZE);
        }
        if (((u32)p % 32) == 0)
        {
            DC_InvalidateRange(p, 32);
        }
    }

#ifdef USE_FFTREAL
    MATH_FFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
#else
    MATH_FFT(data, FFT_NSHIFT, sinTable);
#endif

    // FFT_N/2 ����͂���ȉ����������Ĕ��]�������l���������Ȃ��̂Ōv�Z���Ȃ�
    for (i = 0; i <= FFT_N/2; i++)
    {
        fx32 real;
        fx32 imm;

#ifdef USE_FFTREAL
        if (0 < i  && i < FFT_N/2)
        {
            real = data[i * 2];
            imm  = data[i * 2 + 1];
        }
        else
        {
            if (i == 0)
            {
                real = data[0];
            }
            else // i == FFT_N/2
            {
                real = data[1];
            }
            imm  = 0;
        }
#else
        real = data[i * 2];
        imm  = data[i * 2 + 1];
#endif

        // �e���g���̃p���[���v�Z����
        // �l�̑召�݂̂��K�v�ȏꍇ�� Sqrt ���ȗ����邱�Ƃō��������\ 
        power[i] = FX_Whole(FX_Sqrt(FX_MUL(real, real) + FX_MUL(imm, imm)));

        // �e���g���̃p���[�̎��ԗݐϒl���L�^����
        // ���������������Ă���
        smoothedPower[i] += power[i] - (smoothedPower[i]/8);
    }

    // �}�C�N���͂��`�F�b�N
    {
        s32 totalPower = 0;

#define FILTER_LOW 12    // ((8000Hz/2)/(FFT_N/2)) * 12 = 187.5Hz
#define FILTER_HIGH 64   // ((8000Hz/2)/(FFT_N/2)) * 64 = 1000Hz
        for (i = FILTER_LOW; i < FILTER_HIGH; i++)
        {
            totalPower += smoothedPower[i];
        }
        //OS_TPrintf("totalPower = %d, ave = %d\n", totalPower, totalPower/(FILTER_HIGH-FILTER_LOW));
        // ���ʈȏ�̓��͒l�����邩�`�F�b�N
        if (totalPower > (FILTER_HIGH-FILTER_LOW)*64)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*====== End of main.c ======*/
