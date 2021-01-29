/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - listenOnly
  File:     main.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2007/11/15 13:26:43  seiki_masashi
  �x���ɑΏ�

  Revision 1.3  2007/11/12 04:33:17  seiki_masashi
  ��ʕ\����ǉ�

  Revision 1.2  2007/11/09 14:24:41  seiki_masashi
  small fix

  Revision 1.1  2007/11/09 14:09:00  seiki_masashi
  initial upload


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WM���C�u��������M���胂�[�h�ŏ��������A�X�L�������s���P�@�\�T���v���ł��B
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>

#include    "font.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     BEACON_INFO_MAX     256    // �X�L�����Œ~�ς��� AP ���̍ő吔

#define     WIRELESS_DMA_NO     2      // GX �� DMA �ԍ��Ɣ��Ȃ��悤�ɒ���

#define     KEY_REPEAT_START    25     // �L�[���s�[�g�J�n�܂ł̃t���[����
#define     KEY_REPEAT_SPAN     10     // �L�[���s�[�g�̊Ԋu�t���[����

enum {
    STATE_NONE = 0,
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_SCANNING,
    STATE_TERMINATING_SCAN,
    STATE_FATAL
};

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
// �L�[���͏��
typedef struct KeyInfo
{
    u16     cnt;                       // �����H���͒l
    u16     trg;                       // �����g���K����
    u16     up;                        // �����g���K����
    u16     rep;                       // �����ێ����s�[�g����

}
KeyInfo;

// AP ���
typedef struct BeaconInfo
{
    u32 timeStamp_found;
    u32 timeStamp_updated;
    BOOL isDS;
    u8 bssid[WM_SIZE_BSSID];
    u8 ssid[WM_SIZE_SSID];
    u8 padding[2];
    u32 channel;
}
BeaconInfo;

/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);          // V�u�����N�����݃n���h��

static void SetState(u32 state);
static u32 GetState(void);

static BOOL InitializeWMForListeningAndStartScan(void);
static void InitializeCb(void *arg);
static BOOL StartScan(void);
static void StartScanExCb(void *arg);
static void EndScanCb(void* arg);

// �ėp�T�u���[�`��
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u32 sState = STATE_NONE;
static BeaconInfo sBeaconInfo[BEACON_INFO_MAX];

/* WM �p�V�X�e�����[�N�̈�o�b�t�@ */
static u8 sWmBuffer[WM_SYSTEM_BUF_SIZE] ATTRIBUTE_ALIGN(32);
static u8 sScanBuf[WM_SIZE_SCAN_EX_BUF] ATTRIBUTE_ALIGN(32);
static WMScanExParam sScanExParam ATTRIBUTE_ALIGN(32);

static u16 sScreen[32 * 32];           // ���z�X�N���[��
static KeyInfo sKey;                   // �L�[����


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �������y�у��C�����[�v�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // �e�평����
    OS_Init();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // �\���ݒ菉����
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // ������\���p�ɂQ�c�\���ݒ�
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      // SCR �x�[�X�u���b�N 31
                     GX_BG_CHARBASE_0x00000,    // CHR �x�[�X�u���b�N 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    MI_CpuFillFast((void *)sScreen, 0, sizeof(sScreen));
    DC_FlushRange(sScreen, sizeof(sScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(sScreen, 0, sizeof(sScreen));

    // �����ݐݒ�
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // ����������
    InitializeAllocateSystem();

    MI_CpuClear8(sBeaconInfo, sizeof(sBeaconInfo));

    // ����������
    if (! InitializeWMForListeningAndStartScan() )
    {
        OS_Panic("Cannot initialize WM");
    }

    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O������o��
    OS_Printf("ARM9: WM listenOnly demo started.\n");

    // �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�)
    KeyRead(&sKey);

    // ���C�����[�v
    while (GetState() != STATE_FATAL)
    {
        // �L�[���͏��擾
        KeyRead(&sKey);

        // �X�N���[���N���A
        ClearString();

        // ���݂̃X�e�[�g�̕\��
        switch (GetState())
        {
        case STATE_IDLE:
            PrintString( 14, 0, 6, "IDLE" );
            break;

        case STATE_SCANNING:
            PrintString( 14, 0, 5, "SCAN" );
            break;

        case STATE_FATAL:
            PrintString( 14, 0, 1, "ERROR" );
            break;

        default:
            PrintString( 13, 0, 7, "Busy..." );
            break;

        }

        // �e�`�����l���̏󋵕\��
        {
            int i;
            u32 beaconCountAP[13];
            u32 beaconCountDS[13];
            BOOL isNewAP[13];
            BOOL isNewDS[13];
            u32 currentVCount = OS_GetVBlankCount();
            OSIntrMode e;

            MI_CpuClear8(beaconCountAP, sizeof(beaconCountAP));
            MI_CpuClear8(beaconCountDS, sizeof(beaconCountDS));
            MI_CpuClear8(isNewAP, sizeof(isNewAP));
            MI_CpuClear8(isNewDS, sizeof(isNewDS));

            // ���荞�݂ōX�V�����f�[�^�ɐG��̂Ŕr������
            e = OS_DisableInterrupts();
            for (i = 0; i < BEACON_INFO_MAX; i++)
            {
                if (sBeaconInfo[i].timeStamp_found != 0)
                {
                    if (1 <= sBeaconInfo[i].channel && sBeaconInfo[i].channel <= 13)
                    {
                        if ((u32)(currentVCount - sBeaconInfo[i].timeStamp_updated) > 60 * 60)
                        {
                            // 1���X�V�̖��������G���g���͍폜
                            MI_CpuClear8(&sBeaconInfo[i], sizeof(sBeaconInfo[i]));
                        }
                        else
                        {
                            if (sBeaconInfo[i].isDS)
                            {
                                beaconCountDS[sBeaconInfo[i].channel-1]++;
                            }
                            else
                            {
                                beaconCountAP[sBeaconInfo[i].channel-1]++;
                            }
                            // 1�b�ȓ��Ɍ����������̂�����΁A�\������������
                            if ((u32)(currentVCount - sBeaconInfo[i].timeStamp_found) < 60 * 1)
                            {
                                if (sBeaconInfo[i].isDS)
                                {
                                    isNewDS[sBeaconInfo[i].channel-1] = TRUE;
                                }
                                else
                                {
                                    isNewAP[sBeaconInfo[i].channel-1] = TRUE;
                                }
                            }
                        }
                    }
                }
            }
            (void)OS_RestoreInterrupts(e);

            for (i = 0; i < 13; i++)
            {
                s16 y = (s16)(5 + i);
                u8 color;
                PrintString( 5, y, 4, "Ch%2d: ", i+1 );
                color = (u8)((beaconCountAP[i] > 0) ? ((isNewAP[i]) ? 1 : 15) : 13);
                PrintString( 5 + 7, y, color, "%3d APs", beaconCountAP[i]);
                color = (u8)((beaconCountDS[i] > 0) ? ((isNewDS[i]) ? 1 : 15) : 13);
                PrintString( 5 + 7 + 8, y, color, "%3d DSs", beaconCountDS[i]);
            }
        }

        // �u�u�����N�҂�
        OS_WaitVBlankIntr();
    }

    OS_Terminate();
}

static void SetState(u32 state)
{
    if (sState != STATE_FATAL)
    {
        sState = state;
    }
}

static inline u32 GetState(void)
{
    return sState;
}

static BOOL FindBeaconInfo(const u8* bssid, BeaconInfo** beaconInfo)
{
    int i, j;
    BeaconInfo* emptyEntry = NULL;

    for (i = 0; i < BEACON_INFO_MAX; i++)
    {
        if (sBeaconInfo[i].timeStamp_found != 0)
        {
            for (j = 0; j < WM_SIZE_BSSID; j++)
            {
                if (bssid[j] != sBeaconInfo[i].bssid[j])
                {
                    break;
                }
            }
            if (j == WM_SIZE_BSSID)
            {
                if (beaconInfo != NULL)
                {
                    *beaconInfo = &sBeaconInfo[i];
                }
                return TRUE;
            }
        }
        else
        {
            emptyEntry = &sBeaconInfo[i];
        }
    }
    if (beaconInfo != NULL)
    {
        *beaconInfo = emptyEntry;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         InitalizeWMForListeningAndStartScan

  Description:  WM ����M��p���[�h�ŏ��������A�X�L�������J�n����

  Arguments:    None.

  Returns:      �������ɐ��������� TRUE
 *---------------------------------------------------------------------------*/
static BOOL InitializeWMForListeningAndStartScan(void)
{
    WMErrCode result;

    SetState( STATE_INITIALIZING );

    result = WM_InitializeForListening(&sWmBuffer, InitializeCb, WIRELESS_DMA_NO,
                                       FALSE /* LED ��_�ł����Ȃ� */);
    if (result != WM_ERRCODE_OPERATING)
    {
        OS_TPrintf("WM_Initialize failed: %d\n", result);
        SetState( STATE_FATAL );
        return FALSE;
    }

    return TRUE;
}

static void InitializeCb(void *arg)
{
    // �d���������Ԃł��B
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("WM_Initialize failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    SetState( STATE_IDLE );

    (void)StartScan();

    return;
}

static BOOL StartScan(void)
{
    WMErrCode result;
    u16     chanpat;
    static u32 channel = 0;

    chanpat = WM_GetAllowedChannel();

    // �������g�p�\���ǂ������`�F�b�N����
    if (chanpat == 0x8000)
    {
        // 0x8000 ���Ԃ��Ă����ꍇ�́A����������������Ă��Ȃ��Ȃ�
        // �������C�u�����̏�Ԉُ��\���Ă���̂ŃG���[�ɂ��܂��B
        OS_TPrintf("WM_GetAllowedChannel returns %08x\n", chanpat);
        SetState( STATE_FATAL );
        return FALSE;
    }
    if (chanpat == 0)
    {
        // �������g���Ȃ���ԁB
        OS_TPrintf("WM_GetAllowedChannel returns %08x\n", chanpat);
        SetState( STATE_FATAL );
        return FALSE;
    }

    {
        /* ���݂̎w�肩�珸���ɁA�\�ȃ`�����l�����������܂� */
        while (TRUE)
        {
            channel++;
            if (channel > 16)
            {
                channel = 1;
            }

            if (chanpat & (0x0001 << (channel - 1)))
            {
                break;
            }
        }
        sScanExParam.channelList = (u16)(1 << (channel - 1));
    }

    sScanExParam.maxChannelTime = WM_GetDispersionScanPeriod();
    sScanExParam.scanBuf = (WMbssDesc *)sScanBuf;
    sScanExParam.scanBufSize = WM_SIZE_SCAN_EX_BUF;
    sScanExParam.scanType = WM_SCANTYPE_PASSIVE;
    sScanExParam.ssidLength = 0;
    MI_CpuFill8(sScanExParam.ssid, 0xFF, WM_SIZE_SSID);
    MI_CpuFill8(sScanExParam.bssid, 0xff, WM_SIZE_BSSID);

    SetState( STATE_SCANNING );

    result = WM_StartScanEx(StartScanExCb, &sScanExParam);

    if (result != WM_ERRCODE_OPERATING)
    {
        OS_TPrintf("WM_StartScanEx failed: %d\n", result);
        SetState( STATE_FATAL );
        return FALSE;
    }

    return TRUE;
}

static void StartScanExCb(void *arg)
{
    WMStartScanExCallback *cb = (WMStartScanExCallback *)arg;
    int i;
    WMErrCode result;

    // �X�L�����R�}���h�Ɏ��s�����ꍇ
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("WM_StartScanEx failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    if (GetState() != STATE_SCANNING)
    {
        // ��Ԃ��ύX����Ă���΃X�L�����I��
        SetState( STATE_TERMINATING_SCAN );
        result = WM_EndScan(EndScanCb);
                OS_TPrintf("WM_EndScan returns %d\n", result);
        if (result != WM_ERRCODE_OPERATING)
        {
            OS_TPrintf("WM_EndScan failed: %d\n", result);
            SetState( STATE_FATAL );
        }
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // �e�@�����������ꍇ

        OS_TPrintf("found %d beacons\n", cb->bssDescCount);

        if ( cb->bssDescCount > 0 )
        {
            // BssDesc�̏��ARM7�����珑�����܂�Ă��邽��
            // �o�b�t�@�ɐݒ肳�ꂽBssDesc�̃L���b�V����j��
            DC_InvalidateRange(&sScanBuf, sizeof(sScanBuf));
        }

        for ( i = 0; i < cb->bssDescCount; i++ )
        {
            WMBssDesc* bd = cb->bssDesc[i];
            BOOL isDS;

            OS_TPrintf(" Ch%2d %02x:%02x:%02x:%02x:%02x:%02x ",
                       bd->channel,
                       bd->bssid[0], bd->bssid[1], bd->bssid[2],
                       bd->bssid[3], bd->bssid[4], bd->bssid[5]);

            // �܂��AWMBssDesc.gameInfoLength ���m�F���A
            // ggid �ɗL���Ȓl�������Ă��邱�Ƃ��璲�ׂ�K�v������܂��B
            if (WM_IsValidGameInfo(&bd->gameInfo, bd->gameInfoLength))
            {
                // DS �e�@
                OS_TPrintf("GGID=%08x TGID=%04x\n", bd->gameInfo.ggid, bd->gameInfo.tgid);
                isDS = TRUE;
            }
            else
            {
                // AP
                char str[WM_SIZE_SSID+1];
                u32 ssidLength = bd->ssidLength;
                // ���C�u�������t�B���^���Ă���Ă��邩�킩��Ȃ����ɂ́A
                // ��{�I�ɂ̓l�b�g���[�N���痈���l�͐M�p���Ȃ�
                if (ssidLength > WM_SIZE_SSID)
                {
                    ssidLength = WM_SIZE_SSID;
                }
                MI_CpuCopy8(bd->ssid, str, ssidLength);
                str[ssidLength] = 0;
                OS_TPrintf("SSID=%s\n", str);
                isDS = FALSE;
            }

            // �������r�[�R���̏����L�^
            {
                BOOL found;
                BeaconInfo* beaconInfo;
                // ���荞�݃R�[���o�b�N���Ȃ̂ŏd�����鏈���͂��Ȃ��悤�ɒ���
                found = FindBeaconInfo(bd->bssid, &beaconInfo);
                if (beaconInfo != NULL)
                {
                    u32 timeStamp = OS_GetVBlankCount();
                    if ( timeStamp == 0 ) { timeStamp = (u32)-1; }
                    if ( !found )
                    {
                        // �V�����G���g��
                        beaconInfo->timeStamp_found = timeStamp;
                    }
                    beaconInfo->timeStamp_updated = timeStamp;
                    beaconInfo->isDS      = isDS;
                    beaconInfo->channel   = bd->channel;
                    MI_CpuCopy8(bd->bssid, beaconInfo->bssid, WM_SIZE_BSSID);
                    MI_CpuCopy8(bd->ssid,  beaconInfo->ssid,  WM_SIZE_SSID);
                }
                else
                {
                    OS_TPrintf("Too many beacons; cannot record beacon info.\n");
                }
            }
        }

        break;
    }

    // �`�����l����ύX���čăX�L�������J�n���܂��B
    (void)StartScan();
}


static void EndScanCb(void* arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("EndScanCb failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    SetState( STATE_IDLE );
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  �u�u�����N�����݃x�N�g���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // ���z�X�N���[����VRAM�ɔ��f
    DC_FlushRange(sScreen, sizeof(sScreen));
    /* DMA�����IO���W�X�^�փA�N�Z�X����̂ŃL���b�V���� Wait �͕s�v */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(sScreen, 0, sizeof(sScreen));

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  �L�[���͏���ҏW����B
                �����g���K�A�����g���K�A�����p�����s�[�g�����o�B

  Arguments:    pKey  - �ҏW����L�[���͏��\���́B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInfo * pKey)
{
    static u16 repeat_count[12];
    int     i;
    u16     r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     // �����g���K
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); // �����p�����s�[�g
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      // �����g���K
            }
        }
    }
    pKey->cnt = r;                     // �����H�L�[����
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  ���z�X�N���[�����N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast((void *)sScreen, sizeof(sScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[���ɕ������z�u����B�������32�����܂ŁB

  Arguments:    x       - ������̐擪��z�u���� x ���W( �~ 8 �h�b�g )�B
                y       - ������̐擪��z�u���� y ���W( �~ 8 �h�b�g )�B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B
                text    - �z�u���镶����B�I�[������NULL�B
                ...     - ���z�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        sScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɔz�u����������̐F��ύX����B

  Arguments:    x       - �F�ύX���J�n���� x ���W( �~ 8 �h�b�g )�B
                y       - �F�ύX���J�n���� y ���W( �~ 8 �h�b�g )�B
                length  - �A�����ĐF�ύX���镶�����B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColorString(s16 x, s16 y, s16 length, u8 palette)
{
    s32     i;
    u16     temp;
    s32     index;

    if (length < 0)
        return;

    for (i = 0; i < length; i++)
    {
        index = ((y * 32) + x + i) % (32 * 32);
        temp = sScreen[index];
        temp &= 0x0fff;
        temp |= (palette << 12);
        sScreen[index] = temp;
    }
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
  End of file
 *---------------------------------------------------------------------------*/
