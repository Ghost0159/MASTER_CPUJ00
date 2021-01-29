#include    <nitro.h>
#include    <nitro/cht.h>
#include    "wc.h"


/*---------------------------------------------------------------------------*
    �f�o�b�O�p�R���p�C���X�C�b�`
 *---------------------------------------------------------------------------*/
#define     WC_DEBUG_PRINT             // �f�o�b�O�o�͐؂�ւ��X�C�b�`


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void WcCreateParentParameter(void);      // �e�@���ҏW
static void WcCreateScanParameter(void);        // �X�L�����ݒ�ҏW
static void WcSetNextScanCannel(void);

// WM����̃R�[���o�b�N�֐�
static void WcCb_Enable(void *arg);
static void WcCb_Disable(void *arg);
static void WcCb_PowerOn(void *arg);
static void WcCb_PowerOff(void *arg);
static void WcCb_Indicate(void *arg);
static void WcCb_Reset(void *arg);
static void WcCb_SetParentParameter(void *arg);
static void WcCb_StartParent(void *arg);
static void WcCb_StartMP(void *arg);
static void WcCb_StartScan(void *arg);
static void WcCb_EndScan(void *arg);
static void WcCb_StartConnect(void *arg);
static void WcCb_SetMPData(void *arg);
static void WcCb_ReceiveData(void *arg);

// MP�ʐM�p�R�[���o�b�N
static void WM_ResetCallback(void* param);
static void WM_EndCallback(void* param);

// WM�Ăяo���P�@�\�֐�
static BOOL Wc_Enable(void);
static BOOL Wc_Disable(void);
static BOOL Wc_PowerOn(void);
static BOOL Wc_PowerOff(void);
static BOOL Wc_SetIndCallback(void);
static BOOL Wc_Reset(void);
static BOOL Wc_SetParentParameter(void);
static BOOL Wc_StartParent(void);
static BOOL Wc_StartScan(void);
static BOOL Wc_EndScan(void);
static BOOL Wc_StartConnect(void);
static BOOL Wc_StartMP(void);
static BOOL Wc_StartDataSharing(void);
static BOOL Wc_SetMPData(const void *buf, u16 size);

// �v�����g�o�͗}���_�~�[�֐�
#ifdef  WC_DEBUG_PRINT
#define     WcDebugPrintf       OS_TPrintf
#else
#define     WcDebugPrintf(...)  ((void) 0)
#endif


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static WCStatus wcStatus;              // ���݂̏�Ԃ��Ǘ�
static WCStatus wcTarget;              // ��ԕύX�̖ڕW���Ǘ�
static u16 wcAid;                      // AID(�q�@��)
static u16 wcFramePeriod;              // �Q�[���t���[���Ԋu
static BOOL wcDataShareFlag;           // �f�[�^�V�F�A / �ʏ�MP �؂�ւ��t���O
static BOOL wcBlockTransFlag;          // �u���b�N�]�� / �ʏ�MP �؂�ւ��t���O
static BOOL wcSendFlag;                // ���M�����t���O(�ʏ�MP���[�h��)

static WMParentParam *wcDefaultParameter;
static WCCallbackFunc wcMpRecvCallback;
static WCBlockTransferCallbackFunc wcBtCallback;

static void *wcWmBuffer;               // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static WMParentParam *wcParentParameter;        // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static WMScanParam *wcScanParameter;   // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static WMBssDesc *wcWmBssDesc;         // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static s32 wcSendBufferSize;           // 32�̔{��
static void *wcSendBuffer;             // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static s32 wcRecvBufferSize;           // 32�̔{��
static void *wcRecvBuffer;             // ���̂̓L���b�V���Z�[�t�ȃo�b�t�@
static WMDataSharingInfo *wcDataSharingInfo;
static WMDataSet *wcDataSet;

static BOOL wcPictoCatchStatus;
static WCPictoCatchCallbackFunc wcPictoCatchCallback;

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WcInit

  Description:  �����ʐM������������

  Arguments:    pp          -   �����ʐM�ɂ�����e�@�Ƃ��Ẵp�����[�^���|�C���^
                                �Ŏw�肵�܂��BWC���W���[�����쓮���ɓ��e��ύX
                                �����肵�Ȃ��ŉ������B
                dsFlag      -   �f�[�^�V�F�A�t���O�ł��B
                                TRUE���w�肷��ƃf�[�^�V�F�A���[�h�ɂȂ�܂��B
                                FALSE���w�肷���MP�𒼐ڑ��삷�郂�[�h�ɂȂ�܂��B
                btFlag      -   �u���b�N�]���t���O�ł��B
                                TRUE���w�肷��ƃu���b�N�]�����[�h�ɂȂ�܂��B
                framePeriod -   �Q�[���t���[�����Ƃ̃s�N�`���[�t���[�����B
                                �f�[�^�V�F�A���[�h�̏ꍇ�̂݊��p����܂��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod)
{
    // �X�e�[�g������
    wcStatus = WC_STATUS_READY;
    wcAid = 0;
    wcFramePeriod = framePeriod;
    wcDataShareFlag = dsFlag;
    wcBlockTransFlag = btFlag;
    wcDefaultParameter = (WMParentParam *)pp;
    // ���M�t���O������
    if (!wcDataShareFlag)
    {
        wcSendFlag = FALSE;
    }
    // ���[�U�[�R�[���o�b�N������
    wcMpRecvCallback = NULL;
    wcBtCallback = NULL;

    // WM�p�������m��
    wcWmBuffer = OS_Alloc(WM_SYSTEM_BUF_SIZE);
    wcParentParameter = (WMParentParam *)OS_Alloc(sizeof(WMParentParam));
    wcScanParameter = (WMScanParam *)OS_Alloc(sizeof(WMScanParam));
    wcWmBssDesc = (WMBssDesc *)OS_Alloc(sizeof(WMBssDesc));
    if (wcDataShareFlag)
    {
        wcDataSharingInfo = (WMDataSharingInfo *)OS_Alloc(sizeof(WMDataSharingInfo));
        wcDataSet = (WMDataSet *)OS_Alloc(sizeof(WMDataSet));
        MI_CpuClear8(wcDataSharingInfo, sizeof(WMDataSharingInfo));
        MI_CpuClear8(wcDataSet, sizeof(WMDataSet));
    }
    wcPictoCatchStatus = 0;

    // ���M�o�b�t�@�p�������m��
    {
        s32 parentSize;
        s32 childSize;

        parentSize = WM_SIZE_MP_PARENT_SEND_BUFFER(pp->parentMaxSize, FALSE);
        childSize = WM_SIZE_MP_CHILD_SEND_BUFFER(pp->childMaxSize, FALSE);
        wcSendBufferSize = (parentSize > childSize) ? parentSize : childSize;
    }
    wcSendBuffer = OS_Alloc((u32)wcSendBufferSize);

    // ��M�o�b�t�@�p�������m��
    {
        s32 parentSize;
        s32 childSize;

        parentSize = (s32)WM_SIZE_MP_PARENT_RECEIVE_BUFFER(pp->childMaxSize, pp->maxEntry, FALSE);
        childSize = (s32)WM_SIZE_MP_CHILD_RECEIVE_BUFFER(pp->parentMaxSize, FALSE);
        wcRecvBufferSize = (parentSize > childSize) ? parentSize : childSize;
    }
    wcRecvBuffer = OS_Alloc((u32)wcRecvBufferSize);

    // WM���C�u����������
    (void)WM_Init(wcWmBuffer, WC_DMA_NO);
}

/*---------------------------------------------------------------------------*
  Name:         WcFinish

  Description:  �����ʐM���������O�̏�Ԃɖ߂��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcFinish(void)
{
    WMErrCode wmResult;
    int req;
    
    // �|�[�g��M�R�[���o�b�N�N���A
    if (!wcDataShareFlag)
    {
        WcDebugPrintf("> WM_SetPortCallback start.\n");
        wmResult = WM_SetPortCallback(WC_DEFAULT_PORT, NULL, NULL);
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_SetPortCallback success.\n");
        }
        else
        {
            WcDebugPrintf("< WM_SetPortCallback failed. ERRCODE: %x\n", wmResult);
        }
    }
/*
    switch (wcStatus)
    {
    case WC_STATUS_READY:
    case WC_STATUS_ERROR:
        // WM���C�u������~
        WcDebugPrintf("> WM_Finish start.\n");
        wmResult = WM_Finish();
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_Finish success.\n");
            break;
        }
        WcDebugPrintf("< WM_Finish failed. ERRCODE: %x\n", wmResult);
    default:
        WcDebugPrintf("* Could not finish Wireless library\n");
        return;
    }
*/
    req = WM_Reset( WM_ResetCallback );
    req = WM_Finish();

    // ���������
    OS_Free(wcRecvBuffer);
    OS_Free(wcSendBuffer);
    if (wcDataShareFlag)
    {
        OS_Free(wcDataSet);
        OS_Free(wcDataSharingInfo);
    }
    OS_Free(wcWmBssDesc);
    OS_Free(wcScanParameter);
    OS_Free(wcParentParameter);
    OS_Free(wcWmBuffer);
}

/*---------------------------------------------------------------------------*
  Name:         WcStartParent

  Description:  �e�@�Ƃ��Ė����ʐM���J�n����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcStartParent(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // �e�@�Ƃ��ď������J�n
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_PARENT;   // �ڕW��Ԃ�"�e�@�ڑ����"��
        (void)Wc_Enable();
        break;
    case WC_STATUS_BUSY:
        // �ڕW��Ԃ��X�V
        wcTarget = WC_STATUS_PARENT;   // �ڕW��Ԃ�"�e�@�ڑ����"��
        break;
    case WC_STATUS_PARENT:
        // ���ɐe�@�Ȃ̂ŏ����Ȃ�
        break;
    case WC_STATUS_CHILD:
        // ��U���Z�b�g����
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_PARENT;   // �ڕW��Ԃ�"�e�@�ڑ����"��
        (void)Wc_Reset();
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcStartChild

  Description:  �q�@�Ƃ��Ė����ʐM���J�n����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcStartChild(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // �q�@�Ƃ��ď������J�n
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_CHILD;    // �ڕW��Ԃ�"�q�@�ڑ����"��
        (void)Wc_Enable();
        break;
    case WC_STATUS_BUSY:
        // �ڕW��Ԃ��X�V
        wcTarget = WC_STATUS_CHILD;    // �ڕW��Ԃ�"�q�@�ڑ����"��
        break;
    case WC_STATUS_PARENT:
        // ��U���Z�b�g����
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_CHILD;    // �ڕW��Ԃ�"�q�@�ڑ����"��
        (void)Wc_Reset();
        break;
    case WC_STATUS_CHILD:
        // ���Ɏq�@�Ȃ̂ŏ����Ȃ�
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcEnd

  Description:  �ʐM�O�̏�Ԃɖ߂�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcEnd(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // ����READY��ԂȂ̂ŏ����Ȃ�
        break;
    case WC_STATUS_BUSY:
        // �ڕW��Ԃ��X�V
        wcTarget = WC_STATUS_READY;    // �ڕW��Ԃ�"READY"��
        break;
    case WC_STATUS_PARENT:
    case WC_STATUS_CHILD:
        // ���Z�b�g���邱�ƂŐڑ���ؒf����
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_READY;    // �ڕW��Ԃ�"READY"��
        (void)Wc_Reset();
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcGetStatus

  Description:  �����ʐM�̌��݂̃X�e�[�g���擾����B

  Arguments:    None.

  Returns:      ���݂̃X�e�[�g��Ԃ��B
 *---------------------------------------------------------------------------*/
WCStatus WcGetStatus(void)
{
    return wcStatus;
}

/*---------------------------------------------------------------------------*
  Name:         WcGetAid

  Description:  �����ʐM�O���[�v�ɂ����Ċ��蓖�Ă�ꂽAID���擾����B

  Arguments:    None.

  Returns:      ���蓖�Ă�ꂽAID��Ԃ��B
 *---------------------------------------------------------------------------*/
u16 WcGetAid(void)
{
    return wcAid;
}

/*---------------------------------------------------------------------------*
  Name:         WcStepDataSharing

  Description:  �f�[�^�V�F�A�̓������P�i�߂�B

  Arguments:    send   - ���L����f�[�^�ւ̃|�C���^�B�f�[�^����WC_SHARED_DATA_SIZE
                         �ɌŒ�ƂȂ�B32-Byte Align ����Ă���K�v������B
                bitmap - �ʐM�O���[�v�Ɋ܂܂��[���������r�b�g�}�b�v���擾����
                         �ϐ��ւ̃|�C���^�B

  Returns:      BOOL   - �����ɐ������A�V�F�A���ꂽ�f�[�^���擾�ł����ꍇ��
                         TRUE ��Ԃ��B
                         �擾�����f�[�^��WcGetSharedDataAddress�֐��Ŋe�[�������
                         �f�[�^�ւ̃A�h���X���擾����B
                         �Ȃ�炩�̗��R�Ŏ��s�����ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WcStepDataSharing(const void *send, u16 *bitmap)
{
    WMErrCode wmResult;

    // �f�[�^�V�F�A���[�h�łȂ����͎��s�s��
    if (!wcDataShareFlag)
    {
        return FALSE;
    }

    // �f�[�^�V�F�A�̓������P�i�߂�
//    WcDebugPrintf("> WM_StepDataSharing start.\n");
    wmResult = WM_StepDataSharing(wcDataSharingInfo, send, wcDataSet);

    // �������ʂ𔻒�
    if (wmResult != WM_ERRCODE_SUCCESS && wmResult != WM_ERRCODE_NO_KEYSET)
    {
        WcDebugPrintf("< WM_StepDataSharing failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }

    // �ڑ��ςݒ[���̃r�b�gmap���i�[
    *bitmap = wcDataSet->receivedBitmap;

//    WcDebugPrintf("< WM_StepDataSharing success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WcGetSharedDataAddress

  Description:  �f�[�^�V�F�A�ɂĎ擾�����f�[�^����A�w��AID�̒[������̃f�[�^
                �ւ̃A�h���X���擾����B

  Arguments:    aid - �f�[�^���擾����[����AID���w�肷��B�e�@��"0"�B

  Returns:      u8* - �w��[������̃f�[�^�ւ̃A�h���X��Ԃ��B�f�[�^����
                      WC_SHARED_DATA_SIZE�ɌŒ�ƂȂ�B
                      �w��[������̃f�[�^�����݂��Ȃ��ꍇ�ȂǁA�����Ɏ��s����
                      �ꍇ�ɂ�NULL��Ԃ��B
 *---------------------------------------------------------------------------*/
u8 *WcGetSharedDataAddress(u16 aid)
{
    // �f�[�^�V�F�A���[�h�łȂ����͎��s�s��
    if (!wcDataShareFlag)
    {
        return NULL;
    }

    return (u8 *)WM_GetSharedDataAddress(wcDataSharingInfo, wcDataSet, aid);
}

/*---------------------------------------------------------------------------*
  Name:         WcSetParentData

  Description:  �e�@�Ƃ��đ��M����f�[�^��ݒ肷��B

  Arguments:    buf  - ���M����f�[�^�ւ̃|�C���^�B
                       (32-Byte Align ����Ă���K�v������)
                size - ���M����f�[�^�̃T�C�Y�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetParentData(const void *buf, u16 size)
{
    // �f�[�^�V�F�A���[�h�̎��͎��s�s��
    if (wcDataShareFlag)
    {
        return;
    }

    // ���M�f�[�^�T�C�Y�m�F
    if (size > wcParentParameter->parentMaxSize)
    {
        return;
    }
    // �O�̑��M�̊������m�F
    if (wcSendFlag)
    {
        wcSendFlag = FALSE;

        (void)Wc_SetMPData(buf, size);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcSetChildData

  Description:  �q�@�Ƃ��đ��M����f�[�^��ݒ肷��B

  Arguments:    buf  - ���M����f�[�^�ւ̃|�C���^�B
                       (32-Byte Align ����Ă���K�v������)
                size - ���M����f�[�^�̃T�C�Y�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetChildData(const void *buf, u16 size)
{
    // �f�[�^�V�F�A���[�h�̎��͎��s�s��
    if (wcDataShareFlag)
    {
        return;
    }

    // ���M�f�[�^�T�C�Y�m�F
    if (size > wcDefaultParameter->childMaxSize)
    {
        return;
    }
    // �O�̑��M�̊������m�F
    if (wcSendFlag)
    {
        wcSendFlag = FALSE;

        (void)Wc_SetMPData(buf, size);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcSetMpReceiveCallback

  Description:  MP�ʐM�ɂăf�[�^����M�������̃R�[���o�b�N�֐���ݒ肷��B

  Arguments:    cb  -   �R�[���o�b�N�֐����w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetMpReceiveCallback(WCCallbackFunc cb)
{
    wcMpRecvCallback = cb;
}

/*---------------------------------------------------------------------------*
  Name:         WcSetBlockTransferCallback

  Description:  MP�ʐM�ɂăf�[�^����M�������̃R�[���o�b�N�֐���ݒ肷��B

  Arguments:    cb  -   �R�[���o�b�N�֐����w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb)
{
    wcBtCallback = cb;
}

/*---------------------------------------------------------------------------*
  Name:         WcTurnOnPictoCatch

  Description:  �s�N�g�`���b�g�T�[�`�@�\��L���ɂ���B

  Arguments:    func    -   �s�N�g�`���b�g�̃r�[�R�����𔭌���������
                            �Ăяo�����R�[���o�b�N�֐����w�肷��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func)
{
    wcPictoCatchStatus = 1;
    wcPictoCatchCallback = func;
}

/*---------------------------------------------------------------------------*
  Name:         WcTurnOffPictoCatch

  Description:  �s�N�g�`���b�g�T�[�`�@�\�𖳌��ɂ���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcTurnOffPictoCatch(void)
{
    wcPictoCatchStatus = 0;
    wcPictoCatchCallback = NULL;
}

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WcCreateParentParameter

  Description:  �e�@����ҏW����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCreateParentParameter(void)
{
    static u16 tempID;

    // �e�@�����L���b�V��Store���Ă����Ȃ��o�b�t�@�ɃR�s�[
    MI_CpuCopy32(wcDefaultParameter, wcParentParameter, sizeof(WMParentParam));
    // �r�[�R���Ԋu��K�x�Ȓl�ɐݒ�
    wcParentParameter->beaconPeriod = WM_GetDispersionBeaconPeriod();
    // tempID���C���N�������g
    wcParentParameter->tgid = ++tempID;
}

/*---------------------------------------------------------------------------*
  Name:         WcCreateScanParameter

  Description:  �X�L�����ݒ����ҏW����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCreateScanParameter(void)
{
    wcScanParameter->scanBuf = wcWmBssDesc;
    wcScanParameter->channel = wcDefaultParameter->channel;
    // �T�����x���Ԃ�K�x�Ȓl�ɐݒ�
    wcScanParameter->maxChannelTime = WM_GetDispersionScanPeriod();
    // �u���[�h�L���X�g�A�h���X���w�肵�A�S�e�@��T������悤�ݒ�
    wcScanParameter->bssid[0] = 0xff;
    wcScanParameter->bssid[1] = 0xff;
    wcScanParameter->bssid[2] = 0xff;
    wcScanParameter->bssid[3] = 0xff;
    wcScanParameter->bssid[4] = 0xff;
    wcScanParameter->bssid[5] = 0xff;
}

/*---------------------------------------------------------------------------*
  Name:         WcSetNextScanCannel

  Description:  �X�L�����ݒ���̃`�����l���w������̃`�����l���ɍX�V����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcSetNextScanCannel(void)
{
    u16 allowedChannel;
    s32 i;

    allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0)
        return;
    for (i = 0; i < 16; i++)
    {
        if (allowedChannel & (0x0001 << ((wcScanParameter->channel + i) % 16)))
        {
            wcScanParameter->channel = (u16)(((wcScanParameter->channel + i) % 16) + 1);
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Enable

  Description:  WM_Enable�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Enable(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Enable success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Disable();
            break;
        case WC_STATUS_PARENT:
        case WC_STATUS_CHILD:
            (void)Wc_PowerOn();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Enable failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Disable

  Description:  WM_Disable�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Disable(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Disable success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            wcStatus = WC_STATUS_READY; // �����READY��ԂɑJ�ڊ���
            break;
        case WC_STATUS_PARENT:
            wcStatus = WC_STATUS_READY;
            WcStartParent();
            break;
        case WC_STATUS_CHILD:
            wcStatus = WC_STATUS_READY;
            WcStartChild();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Disable failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_PowerOn

  Description:  WM_PowerOn�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_PowerOn(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_PowerOn success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            if (Wc_SetIndCallback())
            {
                WcCreateParentParameter();
                if (!Wc_SetParentParameter())
                {
                    wcTarget = WC_STATUS_READY; // �ڕW��Ԃ�"READY"��
                    (void)Wc_Reset();
                }
            }
            break;
        case WC_STATUS_CHILD:
            if (TRUE == Wc_SetIndCallback())
            {
                WcCreateScanParameter();
                if (!Wc_StartScan())
                {
                    wcTarget = WC_STATUS_READY; // �ڕW��Ԃ�"READY"��
                    (void)Wc_Reset();
                }
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_PowerOn failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_PowerOff

  Description:  WM_PowerOff�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_PowerOff(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_PowerOff success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Disable();
            break;
        case WC_STATUS_PARENT:
        case WC_STATUS_CHILD:
            (void)Wc_PowerOn();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_PowerOff failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Indicate

  Description:  Indicate�������ɌĂяo�����R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Indicate(void *arg)
{
    WMindCallback *cb = (WMindCallback *)arg;

//    WcDebugPrintf("- Indication. STATECODE: %x\n", cb->state);
    if (cb->errcode == WM_ERRCODE_FIFO_ERROR)
    {
        // �����s�\�ȃG���[
        wcTarget = WC_STATUS_ERROR;
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Reset

  Description:  WM_Reset�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Reset(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Reset success.\n");
        wcAid = 0;
        if (wcBtCallback != NULL)
            (*wcBtCallback) (arg);
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            WcCreateParentParameter();
            if (!Wc_SetParentParameter())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            WcCreateScanParameter();
            if (!Wc_StartScan())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Reset failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_SetParentParameter

  Description:  WM_SetParentParameter�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_SetParentParameter(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetParentParameter success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Reset();
            break;
        case WC_STATUS_PARENT:
            if (!Wc_StartParent())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            wcTarget = WC_STATUS_CHILD;
            // �q�@���[�h�Ɉڍs����ׂɈ�U���Z�b�g����
            (void)Wc_Reset();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_SetParentParameter failed. ERRCODE: %x %x %x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartParent

  Description:  WM_StartParent�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartParent(void *arg)
{
    WMstartParentCallback *cb = (WMstartParentCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        switch (cb->state)
        {
        case WM_STATECODE_PARENT_START:
            WcDebugPrintf("< WM_StartParent success.\n");
            wcAid = 0;
            switch (wcTarget)
            {
            case WC_STATUS_READY:
                (void)Wc_Reset();
                break;
            case WC_STATUS_PARENT:
                if (!Wc_StartMP())
                {
                    wcTarget = WC_STATUS_READY;
                    (void)Wc_Reset();
                }
                break;
            case WC_STATUS_CHILD:
                wcTarget = WC_STATUS_CHILD;
                // �q�@���[�h�Ɉڍs����ׂɈ�U���Z�b�g����
                (void)Wc_Reset();
                break;
            }
            break;
        case WM_STATECODE_CONNECTED:
            WcDebugPrintf("- Child Connected.\n");
            if (wcBtCallback != NULL)
                (*wcBtCallback) (arg);
            break;
        case WM_STATECODE_BEACON_SENT:
//            WcDebugPrintf("- Beacon sent.\n");
            break;
        case WM_STATECODE_DISCONNECTED:
            WcDebugPrintf("- Child disconnected.\n");
            if (wcBtCallback != NULL)
                (*wcBtCallback) (arg);
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartParent failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartScan

  Description:  WM_StartScan�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartScan(void *arg)
{
    WMstartScanCallback *cb = (WMstartScanCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_StartScan success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Reset();
            break;
        case WC_STATUS_PARENT:
            // �e�@���[�h�Ɉڍs����ׂɈ�UIDLE�ɖ߂�
            if (!Wc_EndScan())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            switch (cb->state)
            {
            case WM_STATECODE_PARENT_FOUND:
                // �e�@���i�[�o�b�t�@�̃L���b�V����j��
                DC_InvalidateRange(wcWmBssDesc, sizeof(WMbssDesc));
                // �s�N�g�`���b�g�̃r�[�R���ł��邩���m�F
                if (wcPictoCatchStatus)
                {
                    if ((CHT_IsPictochatParent(wcWmBssDesc)) && (wcPictoCatchCallback != NULL))
                    {
                        wcPictoCatchCallback(wcWmBssDesc);
                    }
                }
                else
                {
                    // GameGroupID����v���邩���m�F
                    if ((wcWmBssDesc->gameInfoLength >= 8) &&
                        (wcWmBssDesc->gameInfo.ggid == wcDefaultParameter->ggid))
                    {
                        // �����A�v���P�[�V�����̐e�@�Ɣ���
                        if (!Wc_EndScan())
                        {
                            wcTarget = WC_STATUS_READY;
                            (void)Wc_Reset();
                        }
                        break;
                    }
                }
                // Don't break here.
                // �����Ă������X�L���������s
            case WM_STATECODE_PARENT_NOT_FOUND:
                // �������X�L���������s����
                WcSetNextScanCannel();
                if (!Wc_StartScan())
                {
                    wcTarget = WC_STATUS_READY;
                    (void)Wc_Reset();
                }
                break;
            default:
                WcDebugPrintf("- Invalid state code. STATECODE: %x\n", cb->state);
                wcStatus = WC_STATUS_BUSY;
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartScan failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_EndScan

  Description:  WM_EndScan�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_EndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_EndScan success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            WcCreateParentParameter();
            if (!Wc_SetParentParameter())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            // �q�@�Ƃ��ăR�l�N�g�J�n
            if (!Wc_StartConnect())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_EndScan failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartConnect

  Description:  WM_StartConnect�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartConnect(void *arg)
{
    WMstartConnectCallback *cb = (WMstartConnectCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        switch (wcTarget)
        {
        case WC_STATUS_READY:
        case WC_STATUS_PARENT:
            WcDebugPrintf("< WM_StartConnect success.\n");
            // �ڕW��Ԃ�"�q�@"�łȂ��Ȃ����̂ŁA��U���Z�b�g����
            // �R�[���o�b�N�̃`�F�[�����̏ꍇ�͂����ł�Wc_Reset���Ă΂Ȃ��B
            if (wcStatus != WC_STATUS_BUSY)
            {
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            switch (cb->state)
            {
            case WM_STATECODE_CONNECT_START:
                // CONNECTED�ɂȂ�̂�҂̂ŁA�����Ȃ�
                break;
            case WM_STATECODE_CONNECTED:
                // ���蓖�Ă�ꂽAID��ޔ�
                wcAid = cb->aid;
                // ���Ɏq�@�̏ꍇ�͉������Ȃ�
                if (wcStatus == WC_STATUS_CHILD)
                {
                    WcDebugPrintf("- Connected , but already child mode.\n");
                }
                else
                {
                    // MP���J�n
                    WcDebugPrintf("< WM_StartConnect success.\n");
                    if (!Wc_StartMP())
                    {
                        wcTarget = WC_STATUS_READY;
                        (void)Wc_Reset();
                    }
                }
                break;
            case WM_STATECODE_BEACON_LOST:
                WcDebugPrintf("- Beacon from parent lost.\n");
                // �r�[�R���������������ł̓��Z�b�g���Ȃ�
                break;
            case WM_STATECODE_DISCONNECTED:
                WcDebugPrintf("- DisConnected from parent.\n");
                wcTarget = WC_STATUS_READY;
                // �R�[���o�b�N�̃`�F�[�����̏ꍇ�͂����ł�Wc_Reset���Ă΂Ȃ��B
                if (wcStatus != WC_STATUS_BUSY)
                {
                    (void)Wc_Reset();
                }
                break;
            default:
                WcDebugPrintf("< WM_StartConnect failed. Invalid state code. STATECODE: %x\n",
                              cb->state);
                wcStatus = WC_STATUS_BUSY;
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartConnect failed. ERRCODE: %x %x %x\n", cb->errcode, cb->wlCmdID,
                      cb->wlResult);
        if (cb->errcode == WM_ERRCODE_NO_ENTRY)
        {
            WcDebugPrintf(" ! Parent doesn't accept entry now.\n");
        }
        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY)
        {
            WcDebugPrintf(" ! Over max entry of BSS group.\n");
        }
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartMP

  Description:  WM_StartMP�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartMP(void *arg)
{
    WMstartMPCallback *cb = (WMstartMPCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        switch (cb->state)
        {
        case WM_STATECODE_MP_START:
            WcDebugPrintf("< WM_StartMP success.\n");
            switch (wcTarget)
            {
            case WC_STATUS_READY:
                (void)Wc_Reset();
                break;
            case WC_STATUS_PARENT:
            case WC_STATUS_CHILD:
                if (wcDataShareFlag)
                {
                    if (!Wc_StartDataSharing())
                    {
                        wcTarget = WC_STATUS_READY;
                        (void)Wc_Reset();
                    }
                    else
                    {
                        wcStatus = wcTarget;    // �e(�q)�@�Ƃ��Ă̐ڑ�������Ɋ���
                    }
                }
                else
                {
                    wcSendFlag = TRUE;
                    wcStatus = wcTarget;        // �e(�q)�@�Ƃ��Ă̐ڑ�������Ɋ���
                    if (wcBtCallback != NULL)
                        (*wcBtCallback) (arg);
                }
                break;
            }
            break;
        case WM_STATECODE_MPEND_IND:
            // �e�@�Ƃ��Ă̑���M��������indicate
            break;
        case WM_STATECODE_MP_IND:
            // �q�@�Ƃ��Ă̎�M��������indicate
            break;
        }
    }
    else
    {
        switch (cb->errcode)
        {
        case WM_ERRCODE_SEND_FAILED:
        case WM_ERRCODE_TIMEOUT:
            // ����M���������Ȃ������ꍇ��indicate
            // �Ȃɂ������ɍđ������̂�҂�
            break;
        case WM_ERRCODE_INVALID_POLLBITMAP:
            // �������ł͂Ȃ� MP �ʐM���󂯎�����ꍇ�� indicate
            break;
        default:
            WcDebugPrintf("< WM_StartMP failed. ERRCODE: %x %x\n", cb->errcode, cb->state);
            wcStatus = WC_STATUS_BUSY;
            wcTarget = WC_STATUS_READY;
            (void)Wc_Reset();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_SetMPData

  Description:  WM_SetMPData�ɑ΂���R�[���o�b�N�֐��B

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_SetMPData(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    wcSendFlag = TRUE;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetMPData success.\n");
    }
    else
    {
        WMPortSendCallback *cb_PortSend = (WMPortSendCallback *)cb;
        WcDebugPrintf("< WM_SetMPData failed. ERRCODE: %x %x %x restBitmap: %x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult, cb_PortSend->restBitmap);
    }
    if (wcBtCallback != NULL)
        (*wcBtCallback) (arg);
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_ReceiveData

  Description:  port �ɑ΂����M�ʒm

  Arguments:    arg - �R�[���o�b�N�\���̂ւ̃|�C���^�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_ReceiveData(void *arg)
{
    WMPortRecvCallback *cb = (WMPortRecvCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< Receive Data success.\n");
        switch (cb->state)
        {
        case WM_STATECODE_PORT_RECV:
            // �f�[�^��M
            switch (wcStatus)
            {
            case WC_STATUS_PARENT:
                // main���Ɏ�M��ʒm
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(cb->aid, cb->data, cb->length);
                }
                break;
            case WC_STATUS_CHILD:
                // main���Ɏ�M��ʒm
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(0, cb->data, cb->length);
                }
                break;
            }
            break;
        case WM_STATECODE_CONNECTED:
            // �ڑ��ʒm
            break;
        case WM_STATECODE_DISCONNECTED:
            // �ؒf�ʒm
            switch (wcStatus)
            {
            case WC_STATUS_PARENT:
                // main���ɐؒf��ʒm
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(cb->aid, NULL, 0);
                }
                break;
            case WC_STATUS_CHILD:
                // main���ɐؒf��ʒm
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(0, NULL, 0);
                }
                break;
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< Receive Data failed. ERRCODE: %x\n", cb->errcode);
    }
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Enable

  Description:  WM_Enable�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_Enable�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_Enable(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_Enable start.\n");
    wmResult = WM_Enable(WcCb_Enable);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Enable failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Disable

  Description:  WM_Disable�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_Disable�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_Disable(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_Disable start.\n");
    wmResult = WM_Disable(WcCb_Disable);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Disable failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_PowerOn

  Description:  WM_PowerOn�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_PowerOn�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_PowerOn(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_PowerOn start.\n");
    wmResult = WM_PowerOn(WcCb_PowerOn);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_PowerOn failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_PowerOff

  Description:  WM_PowerOff�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_PowerOff�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_PowerOff(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_PowerOff start.\n");
    wmResult = WM_PowerOff(WcCb_PowerOff);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_PowerOff failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetIndCallback

  Description:  WM_SetIndCallback�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_SetIndCallback�֐������������ꍇ��TRUE��Ԃ��B
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetIndCallback(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetIndCallback start.\n");
    wmResult = WM_SetIndCallback(WcCb_Indicate);
    if (wmResult != WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetIndCallback failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    WcDebugPrintf("< WM_SetIndCallback success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Reset

  Description:  WM_Reset�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_Reset�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_Reset(void)
{
    WMErrCode wmResult;

    if ((wcStatus != WC_STATUS_BUSY) && (wcStatus != WC_STATUS_ERROR))
    {
        wcStatus = WC_STATUS_BUSY;
    }
    WcDebugPrintf("> WM_Reset start.\n");
    wmResult = WM_Reset(WcCb_Reset);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Reset failed. ERRCODE: %x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetParentParameter

  Description:  WM_SetParentParameter�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_SetParentParameter�ɂ��񓯊�����������ɊJ�n�ł���
                       �ꍇ��TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetParentParameter(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetParentParameter start.\n");
    wmResult = WM_SetParentParameter(WcCb_SetParentParameter, wcParentParameter);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_SetParentParameter failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartParent

  Description:  WM_StartParent�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_StartParent�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartParent(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartParent start.\n");
    wmResult = WM_StartParent(WcCb_StartParent);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartParent failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartScan

  Description:  WM_StartScan�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_StartScan�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartScan(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartScan start.\n");
    wmResult = WM_StartScan(WcCb_StartScan, wcScanParameter);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartScan failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_EndScan

  Description:  WM_EndScan�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_EndScan�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_EndScan(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_EndScan start.\n");
    wmResult = WM_EndScan(WcCb_EndScan);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_EndScan failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartConnect

  Description:  WM_StartConnect�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_StartConnect�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartConnect(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartConnect start.\n");
    wmResult = WM_StartConnect(WcCb_StartConnect, wcWmBssDesc, NULL);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartConnect failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartMP

  Description:  WM_StartMP�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_StartMP�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartMP(void)
{
    WMErrCode wmResult;

    if (!wcDataShareFlag)
    {
        WcDebugPrintf("> WM_SetPortCallback start.\n");
        wmResult = WM_SetPortCallback(WC_DEFAULT_PORT, WcCb_ReceiveData, NULL);
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_SetPortCallback success.\n");
        }
        else
        {
            WcDebugPrintf("< WM_SetPortCallback failed. ERRCODE: %x\n", wmResult);
            return FALSE;
        }
    }

    WcDebugPrintf("> WM_StartMP start.\n");
    wmResult = WM_StartMP(WcCb_StartMP,
                          wcRecvBuffer,
                          (u16)wcRecvBufferSize,
                          wcSendBuffer,
                          (u16)wcSendBufferSize, (u16)(wcParentParameter->CS_Flag ? 0 : 1));
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartMP failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartDataSharing

  Description:  WM_StartDataSharing�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_StartDataSharing������Ɋ��������ꍇ��TRUE��Ԃ��B
                       ���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartDataSharing(void)
{
    WMErrCode wmResult;
    u16 aidBitmap;

    aidBitmap = (u16)((1 << (wcDefaultParameter->maxEntry + 1)) - 1);   // ���� WC_NUM_MAX_CHILD+1 �r�b�g��1�� bitmap
    WcDebugPrintf("> WM_StartDataSharing start.\n");
    wmResult = WM_StartDataSharing(wcDataSharingInfo, WC_DS_PORT_NUMBER,        // �|�[�g
                                   aidBitmap,   // �ڑ�����[���̃r�b�g�}�b�v
                                   wcDefaultParameter->childMaxSize,    // �[����䕪�̋��L�f�[�^�̃T�C�Y
                                   (wcFramePeriod > 1 ? FALSE : TRUE));
    if (wmResult != WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_StartDataSharing failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    WcDebugPrintf("< WM_StartDataSharing success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetMPData

  Description:  WM_SetMPData�֐����Ăяo���B

  Arguments:    None.

  Returns:      BOOL - WM_SetMPData�ɂ��񓯊�����������ɊJ�n�ł����ꍇ��
                       TRUE��Ԃ��B���s�����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetMPData(const void *buf, u16 size)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetMPData start. data: %08x\n", *(u32 *)buf);
    wmResult = WM_SetMPDataToPort(WcCb_SetMPData,
                                  buf, size, 0xffff, WC_DEFAULT_PORT, WC_DEFAULT_PORT_PRIO);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        wcSendFlag = TRUE;
        WcDebugPrintf("< WM_SetMPData failed. ERRCODE: %x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}


/*---------------------------------------------------------------------------*
  MP�ʐM�p�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void WM_EndCallback(void* param){
    WMCallback wmcallback;
    
    wmcallback = *(WMCallback*)param;
    if(wmcallback.errcode == WM_ERRCODE_SUCCESS){
        OS_TPrintf("WM end success\n");
    }
    else{
        OS_TPrintf("WM end failed\n");
    }
}

static void WM_ResetCallback(void* param){
    WMCallback wmcallback;

    wmcallback = *(WMCallback*)param;
    if(wmcallback.errcode == WM_ERRCODE_SUCCESS){
        OS_TPrintf("WM reset success\n");
    }
    else{
        OS_TPrintf("WM reset failed\n");
    }

    WM_End( WM_EndCallback );
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
