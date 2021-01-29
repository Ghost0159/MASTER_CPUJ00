/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - libraries
  File:     cpsif.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: cpsif.c,v $
  Revision 1.7.4.2  2007/09/18 09:23:11  okubata_ryoma
  Copyright fix

  Revision 1.7.4.1  2007/09/18 09:18:05  okubata_ryoma
  HEAD�ƃ}�[�W

  Revision 1.7  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/09/01 23:45:02  terui
  WcmCpsifKACallback�֐��ɂĈ������g�p�x�����������Ȃ��悤�ɉ���

  Revision 1.5  2005/09/01 13:05:06  terui
  Keep Alive �p NULL �p�P�b�g���M�@�\��ǉ�
  �f�[�^���M���� Keep Alive �p�P�b�g���M�A���[�������Z�b�g����@�\��ǉ�

  Revision 1.4  2005/08/10 10:39:24  adachi_hiroaki
  �G���[���b�Z�[�W�̈������C��

  Revision 1.3  2005/08/08 11:15:49  terui
  WCM_GetApEssid �֐���ǉ�

  Revision 1.2  2005/07/11 11:42:04  yasu
  �p�f�B���O�x�����Ȃ������߂Ƀp�f�B���O�ǉ�

  Revision 1.1  2005/07/07 10:45:52  terui
  �V�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "wcm_private.h"
#include "wcm_cpsif.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#if WCM_DEBUG

// �x�����p�e�L�X�g���`
static const char   cpsifWarningText_NotInit[] = { "WCM library is not initialized yet.\n" };
static const char   cpsifWarningText_IllegalParam[] = { "Illegal parameter ( %s )\n" };
static const char   cpsifWarningText_InIrqMode[] = { "Processor mode is IRQ mode now.\n" };

// �񍐕��p�e�L�X�g���`
static const char   cpsifReportText_WmSyncError[] = { "%s failed syncronously. ( ERRCODE: 0x%02x )\n" };
static const char   cpsifReportText_WmAsyncError[] = { "%s failed asyncronously. ( ERRCODE: 0x%02x )\n" };
#endif

#define WCMCPSIF_DUMMY_IRQ_THREAD   ((OSThread*)OS_IrqHandler)

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/

// ������ԊǗ��p�\����
typedef struct WCMCpsifWork
{
    u8  initialized;                // ���[�N�ϐ��������m�F�p
    u8  reserved[3];                // �p�f�B���O  (OSThreadQueue ��4byte�ȏ��)
    OSThreadQueue   sendQ;          // DCF ���M�����҂��u���b�N�p Thread Queue
    OSMutex         sendMutex;      // �����X���b�h�ɂ�鑗�M�̔r������p Mutex
    WMErrCode       sendResult;     // DCF ���M�̔񓯊��I�ȏ������ʑޔ��G���A
    WCMRecvInd      recvCallback;   // DCF ��M�R�[���o�b�N��ޔ��G���A
} WCMCpsifWork;

/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/

/* 
 * ThreadQueue �� Mutex �ȂǁAWCM���C�u�������I�������ۂɉ������Ă��܂���
 * ����ϐ��Q�� static �ȕϐ��Ƃ���B�����̕ϐ��͈�x�����������ƂQ�x��
 * ������ꂽ��ď��������ꂽ�肵�Ȃ��B
 */
static WCMCpsifWork wcmCpsifw;

/*---------------------------------------------------------------------------*
    �����֐��v���g�^�C�v
 *---------------------------------------------------------------------------*/
static void         WcmCpsifWmCallback(void* arg);
static void         WcmCpsifKACallback(void* arg);
static BOOL         WcmCpsifTryLockMutexInIRQ(OSMutex* mutex);
static void         WcmCpsifUnlockMutexInIRQ(OSMutex* mutex);

/*---------------------------------------------------------------------------*
  Name:         WCMi_InitCpsif

  Description:  CPS ���C�u�����Ƃ̃C���^�[�t�F�[�X�ƂȂ镔���ɂ��āA���[�N
                �ϐ��Q������������B��x������������ď������͂��Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCMi_InitCpsif(void)
{
    if (wcmCpsifw.initialized == 0)
    {
        wcmCpsifw.initialized = 1;
        wcmCpsifw.sendResult = WM_ERRCODE_SUCCESS;

        /* wcmCpsifw.recvCallback �͕ύX����Ă���\��������̂ŏ��������Ȃ� */
        OS_InitThreadQueue(&(wcmCpsifw.sendQ));
        OS_InitMutex(&(wcmCpsifw.sendMutex));
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_CpsifRecvCallback

  Description:  �����h���C�o�� DCF �t���[������M�����ۂɌĂяo�����֐��B
                ���荞�݃T�[�r�X���ŌĂяo����ACPS �̎�M�R�[���o�b�N�֐���
                �Ăяo���B

  Arguments:    recv    -   ��M���� DCF �t���[���\���̂ւ̃|�C���^���n�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCMi_CpsifRecvCallback(WMDcfRecvBuf* recv)
{
    if (wcmCpsifw.recvCallback != NULL)
    {
        wcmCpsifw.recvCallback((u8 *) (recv->srcAdrs), (u8 *) (recv->destAdrs), (u8 *) (recv->data), (s32) (recv->length));
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_CpsifSendNullPacket

  Description:  �L�[�v�A���C�u NULL �p�P�b�g �𑗐M����B
                ���� DCF �p�P�b�g�𑗐M���̏ꍇ�͉����s��Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCMi_CpsifSendNullPacket(void)
{
    volatile WCMWork*   w = WCMi_GetSystemWork();

    // WCM ���C�u�����̓�����Ԃ��m�F
    if (w == NULL)
    {
        return;
    }

    if ((w->phase != WCM_PHASE_DCF) || (w->resetting == WCM_RESETTING_ON))
    {
        return;
    }

    // �r�����b�N
    if (FALSE == WcmCpsifTryLockMutexInIRQ(&(wcmCpsifw.sendMutex)))
    {
        return;
    }

    // ���M�v�����s
    if (WM_ERRCODE_OPERATING != WM_SetDCFData(WcmCpsifKACallback, (const u8*)w->bssDesc.bssid, (const u16*)(w->sendBuf),
        0))
    {
        WcmCpsifUnlockMutexInIRQ(&(wcmCpsifw.sendMutex));
    }
}

/*---------------------------------------------------------------------------*
  Name:         WCM_GetApMacAddress

  Description:  �����h���C�o���ڑ����m�����Ă��� AP �� MAC �A�h���X���擾����B
    NOTICE:     ���̊֐��ɂ���Ď擾�����|�C���^�������o�b�t�@�̓��e�́A���荞��
                ���ɂ���ԕω��ɔ����ĕύX���ꂽ��A�o�b�t�@���g��������ꂽ��
                ����\��������_�ɒ��ӁB���荞�݋֎~������ԂŌĂяo���A���e��
                �ʃo�b�t�@�ɑޔ����邱�Ƃ𐄏�����B

  Arguments:    None.

  Returns:      u8*     -   �ڑ����m�����Ă��� AP �� MAC �A�h���X�ւ̃|�C���^��
                            �Ԃ��B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
u8* WCM_GetApMacAddress(void)
{
    u8*         mac = NULL;
    WCMWork*    w = WCMi_GetSystemWork();
    OSIntrMode  e = OS_DisableInterrupts();

    // �������m�F
    if (w != NULL)
    {
        // WCM ���C�u�����̏�Ԋm�F
        if ((w->phase == WCM_PHASE_DCF) && (w->resetting == WCM_RESETTING_OFF))
        {
            /* �����h���C�o�ɂ����ăR�l�N�V�������m������Ă����� */
            mac = w->bssDesc.bssid;
        }
    }

    (void)OS_RestoreInterrupts(e);
    return mac;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_GetApEssid

  Description:  �����h���C�o���ڑ����m�����Ă��� AP �� ESS-ID ���擾����B
    NOTICE:     ���̊֐��ɂ���Ď擾�����|�C���^�������o�b�t�@�̓��e�́A���荞��
                ���ɂ���ԕω��ɔ����ĕύX���ꂽ��A�o�b�t�@���g��������ꂽ��
                ����\��������_�ɒ��ӁB���荞�݋֎~������ԂŌĂяo���A���e��
                �ʃo�b�t�@�ɑޔ����邱�Ƃ𐄏�����B

  Arguments:    length  -   ESS-ID �̒������擾���� u16 �^�ϐ��ւ̃|�C���^���w��
                            ����B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� 0 ���i�[�����B
                            �������擾����K�v���Ȃ��ꍇ�ɂ́A���̈����� NULL ��
                            �w�肵�Ă��悢�B

  Returns:      u8*     -   �ڑ����m�����Ă��� AP �� ESS-ID �f�[�^��ւ̃|�C���^
                            ��Ԃ��B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
u8* WCM_GetApEssid(u16* length)
{
    u8*         essid = NULL;
    u16         len = 0;
    WCMWork*    w = WCMi_GetSystemWork();
    OSIntrMode  e = OS_DisableInterrupts();

    // �������m�F
    if (w != NULL)
    {
        // WCM ���C�u�����̏�Ԋm�F
        if ((w->phase == WCM_PHASE_DCF) && (w->resetting == WCM_RESETTING_OFF))
        {
            /* �����h���C�o�ɂ����ăR�l�N�V�������m������Ă����� */
            essid = &(w->bssDesc.ssid[0]);
            len = w->bssDesc.ssidLength;
        }
    }

    (void)OS_RestoreInterrupts(e);
    if (length != NULL)
    {
        *length = len;
    }

    return essid;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_SetRecvDCFCallback

  Description:  CPS �̎�M�R�[���o�b�N�֐���o�^����B
                �����h���C�o�� DCF �t���[������M����x�ɂ����œo�^������M
                �R�[���o�b�N�֐����Ăяo�����B
    NOTICE:     �����œo�^�����R�[���o�b�N�֐����I�[�o�[���C���ɂ���ꍇ�ɂ́A
                ��ѐ悪�������ɑ��݂��Ȃ��Ȃ�O�ɓo�^���Ă���R�[���o�b�N�֐���
                NULL �ŏ㏑���N���A����K�v������_�ɒ��ӁB

  Arguments:    callback    -   DCF ��M�R�[���o�b�N�֐����w�肷��B
                                NULL ���w�肷��Ǝ�M�ʒm����Ȃ��Ȃ�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCM_SetRecvDCFCallback(WCMRecvInd callback)
{
    OSIntrMode  e = OS_DisableInterrupts();

    wcmCpsifw.recvCallback = callback;

    (void)OS_RestoreInterrupts(e);
}

/*---------------------------------------------------------------------------*
  Name:         WCM_SendDCFData

  Description:  �����h���C�o�� DCF �t���[�����M���w������BDCF �t���[���̑��M
                �����A�������͎��s����܂œ����Ńu���b�N���đ҂B�܂��A������
                �ڑ���Ԃ� DCF �t���[���𑗐M�ł��Ȃ���Ԃ̏ꍇ�ɂ͎��s����B
    NOTICE:     �����̃X���b�h����񓯊��ɌĂяo����邱�Ƃ�z�肵�Ă��邪�A
                ���荞�݃T�[�r�X���ŌĂяo����邱�Ƃ͑z�肵�Ă��Ȃ��_�ɒ��ӁB

  Arguments:    dstAddr -   DCF �t���[���̂��Đ�ƂȂ� MAC �A�h���X���w�肷��B
                buf     -   ���M����f�[�^�ւ̃|�C���^���w�肷��B
                            ���֐�����߂�܂ł͎w�肵���o�b�t�@�̓��e�͕ێ�
                            ���Ă����K�v������_�ɒ��ӁB
                len     -   ���M����f�[�^�̒������o�C�g�P�ʂŎw�肷��B

  Returns:      s32     -   ���M�ɐ��������ꍇ�ɑ��M���ꂽ�f�[�^����Ԃ��B
                            ���s�����ꍇ�ɂ͕��l��Ԃ��B
 *---------------------------------------------------------------------------*/
s32 WCM_SendDCFData(const u8* dstAddr, const u8* buf, s32 len)
{
    OSIntrMode          e = OS_DisableInterrupts();
    volatile WCMWork*   w = WCMi_GetSystemWork();

    // �������ς݂��m�F
    if (w == NULL)
    {
        WCMi_Warning(cpsifWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_BEFORE_INIT;        // WCM ���C�u����������������Ă��Ȃ�
    }

#if WCM_DEBUG
    // �p�����[�^�m�F
    if ((dstAddr == NULL) || (buf == NULL))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "dstAddr or buf");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // �|�C���^�� NULL
    }

    if (len <= 0)
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "len");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // len �� 0 �ȉ�
    }

    if (len > WCM_ROUNDUP32(WM_DCF_MAX_SIZE))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "len");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // len �� �T�C�Y����
    }

    if ((u32) buf & 0x01)
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "buf");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // buf ����A�h���X
    }

    // ���荞�݃T�[�r�X�����ǂ������m�F
    if (OS_GetProcMode() == HW_PSR_IRQ_MODE)
    {
        WCMi_Warning(cpsifWarningText_InIrqMode);
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_IN_IRQ_MODE;        // IRQ ���[�h�ł̌Ăяo���͋֎~
    }
#endif

    // �ʃX���b�h���r�����M�u���b�N���̏ꍇ�͉��������̂�҂�
    OS_LockMutex(&(wcmCpsifw.sendMutex));

    /*
     * �u���b�N��Ԃ��畜�A�������_�ŁAWCM���C�u�����̏�Ԃ͕s���B
     * AP �Ɛڑ�����Ă����ԂłȂ��Ƒ��M�͂ł��Ȃ����߁A������ WCM ���C�u������
     * ��Ԃ��m�F����K�v������B
     */
    w = WCMi_GetSystemWork();
    if (w == NULL)
    {
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_BEFORE_INIT;        // �Q�Ă���Ԃ� WCM ���C�u�������I�����Ă��܂����ꍇ
    }

    if ((w->phase != WCM_PHASE_DCF) || (w->resetting == WCM_RESETTING_ON))
    {
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_NO_CONNECTION;      // ���M���悤�Ƃ������� AP �Ƃ̐ڑ����m�����Ă��Ȃ��ꍇ
    }

    /*
     * ���M�f�[�^���L���b�V���Z�[�t�ȃo�b�t�@�Ɉ�U�R�s�[����B
     * �����̃X���b�h����̑��M�v���͔r���I�ɏ��������̂ŁA�R�s�[��̃L���b�V���Z�[�t��
     * �o�b�t�@�͈��̑��M����������Ηǂ��͂��B
     */
    MI_CpuCopy8(buf, (void*)(w->sendBuf), (u32) len);

    // ���M�v�����s
    {
        WMErrCode   wmResult;

        wmResult = WM_SetDCFData(WcmCpsifWmCallback, (const u8*)dstAddr, (const u16*)(w->sendBuf), (u16) len);
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            break;

        case WM_ERRCODE_ILLEGAL_STATE:
        case WM_ERRCODE_INVALID_PARAM:
        case WM_ERRCODE_FIFO_ERROR:
        default:
            // ���M�v���Ɏ��s�����ꍇ
            WCMi_Printf(cpsifReportText_WmSyncError, "WM_SetDCFData", wmResult);
            OS_UnlockMutex(&(wcmCpsifw.sendMutex));
            (void)OS_RestoreInterrupts(e);
            return WCM_CPSIF_RESULT_SEND_FAILURE;   // DCF ���M�Ɏ��s
        }
    }

    // ���M�����҂��u���b�N
    OS_SleepThread(&(wcmCpsifw.sendQ));

    /*
     * �u���b�N��Ԃ��畜�A�������_�ŁAWCM���C�u�����̏�Ԃ͕s���B
     * WCM ���C�u�������I������Ă��Ă��A���M���ʂ͕ێ�����Ă���̂�
     * WCM ���C�u�����̏�Ԃ��m�F����K�v�͂Ȃ��B
     */
    switch (wcmCpsifw.sendResult)
    {
    case WM_ERRCODE_SUCCESS:
        break;

    case WM_ERRCODE_FAILED:
    default:
        // ���M�Ɏ��s�����ꍇ
        WCMi_Printf(cpsifReportText_WmAsyncError, "WCM_SendDCFData", wcmCpsifw.sendResult);
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_SEND_FAILURE;       // DCF ���M�Ɏ��s
    }

    // ����I��
    OS_UnlockMutex(&(wcmCpsifw.sendMutex));
    (void)OS_RestoreInterrupts(e);
    return len;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_SendDCFDataEx

  Description:  �����h���C�o�� DCF �t���[�����M���w������BDCF �t���[���̑��M
                �����A�������͎��s����܂œ����Ńu���b�N���đ҂B�܂��A������
                �ڑ���Ԃ� DCF �t���[���𑗐M�ł��Ȃ���Ԃ̏ꍇ�ɂ͎��s����B
    NOTICE:     �����̃X���b�h����񓯊��ɌĂяo����邱�Ƃ�z�肵�Ă��邪�A
                ���荞�݃T�[�r�X���ŌĂяo����邱�Ƃ͑z�肵�Ă��Ȃ��_�ɒ��ӁB

  Arguments:    dstAddr     -   DCF �t���[���̂��Đ�ƂȂ� MAC �A�h���X���w�肷��B
                header      -   ���M����f�[�^�̃w�b�_�����ւ̃|�C���^���w�肷��B
                headerLen   -   ���M����f�[�^�̃w�b�_�̒������o�C�g�P�ʂŎw�肷��B
                body        -   ���M����f�[�^�̃{�f�B�����ւ̃|�C���^���w�肷��B
                bodyLen     -   ���M����f�[�^�̃{�f�B�����̒������o�C�g�P�ʂŎw�肷��B
                
                * header�Abody �Ɋւ��ẮA���֐�����߂�܂ł͎w�肵���o�b�t�@�̓��e��
                  �ێ����Ă����K�v������_�ɒ��ӁB

  Returns:      s32     -   ���M�ɐ��������ꍇ�ɑ��M���ꂽ�f�[�^����Ԃ��B
                            ���s�����ꍇ�ɂ͕��l��Ԃ��B
 *---------------------------------------------------------------------------*/
s32 WCM_SendDCFDataEx(const u8* dstAddr, const u8* header, s32 headerLen,
                      const u8* body, s32 bodyLen)
{
    OSIntrMode          e = OS_DisableInterrupts();
    volatile WCMWork*   w = WCMi_GetSystemWork();

    // �������ς݂��m�F
    if (w == NULL)
    {
        WCMi_Warning(cpsifWarningText_NotInit);
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_BEFORE_INIT;        // WCM ���C�u����������������Ă��Ȃ�
    }

#if WCM_DEBUG
    // �p�����[�^�m�F
    if ((dstAddr == NULL) || (header == NULL))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "dstAddr or header");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // header �̃|�C���^�� NULL
    }

    if (headerLen <= 0 || (body != NULL && bodyLen < 0) || (body == NULL && bodyLen != 0))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "headerLen or bodyLen");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // headerLen �� 0 �ȉ��A�܂��� bodyLen �� 0 ����
    }

    if (headerLen + bodyLen > WCM_ROUNDUP32(WM_DCF_MAX_SIZE))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "headerLen + bodyLen");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // (headerLen + bodyLen) �� �T�C�Y����
    }

    if ((u32) header & 0x01 || ((body != NULL) && ((u32) body & 0x01)))
    {
        WCMi_Warning(cpsifWarningText_IllegalParam, "header or body");
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_ILLEGAL_PARAM;      // header �܂��� body �̃o�b�t�@����A�h���X
    }

    // ���荞�݃T�[�r�X�����ǂ������m�F
    if (OS_GetProcMode() == HW_PSR_IRQ_MODE)
    {
        WCMi_Warning(cpsifWarningText_InIrqMode);
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_IN_IRQ_MODE;        // IRQ ���[�h�ł̌Ăяo���͋֎~
    }
#endif

    // �ʃX���b�h���r�����M�u���b�N���̏ꍇ�͉��������̂�҂�
    OS_LockMutex(&(wcmCpsifw.sendMutex));

    /*
     * �u���b�N��Ԃ��畜�A�������_�ŁAWCM���C�u�����̏�Ԃ͕s���B
     * AP �Ɛڑ�����Ă����ԂłȂ��Ƒ��M�͂ł��Ȃ����߁A������ WCM ���C�u������
     * ��Ԃ��m�F����K�v������B
     */
    w = WCMi_GetSystemWork();
    if (w == NULL)
    {
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_BEFORE_INIT;        // �Q�Ă���Ԃ� WCM ���C�u�������I�����Ă��܂����ꍇ
    }

    if ((w->phase != WCM_PHASE_DCF) || (w->resetting == WCM_RESETTING_ON))
    {
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_NO_CONNECTION;      // ���M���悤�Ƃ������� AP �Ƃ̐ڑ����m�����Ă��Ȃ��ꍇ
    }

    /*
     * ���M�f�[�^���L���b�V���Z�[�t�ȃo�b�t�@�Ɉ�U�R�s�[����B
     * �����̃X���b�h����̑��M�v���͔r���I�ɏ��������̂ŁA�R�s�[��̃L���b�V���Z�[�t��
     * �o�b�t�@�͈��̑��M����������Ηǂ��͂��B
     */

    /*
     * �w�b�_�����ƃ{�f�B�����ɕ�����Ă����f�[�^���AWCMWork �� SendBuf �ɂ܂Ƃ߂ăR�s�[����
     */
    MI_CpuCopy8(header, (void*)(w->sendBuf), (u32) headerLen);
    if (bodyLen > 0)
    {
        MI_CpuCopy8(body, (void*)(((u8*)w->sendBuf) + headerLen), (u32) bodyLen);
    }

    // ���M�v�����s
    {
        WMErrCode   wmResult;

        wmResult = WM_SetDCFData(WcmCpsifWmCallback, (const u8*)dstAddr, (const u16*)(w->sendBuf), (u16) (headerLen + bodyLen));
        switch (wmResult)
        {
        case WM_ERRCODE_OPERATING:
            break;

        case WM_ERRCODE_ILLEGAL_STATE:
        case WM_ERRCODE_INVALID_PARAM:
        case WM_ERRCODE_FIFO_ERROR:
        default:
            // ���M�v���Ɏ��s�����ꍇ
            WCMi_Printf(cpsifReportText_WmSyncError, "WM_SetDCFData", wmResult);
            OS_UnlockMutex(&(wcmCpsifw.sendMutex));
            (void)OS_RestoreInterrupts(e);
            return WCM_CPSIF_RESULT_SEND_FAILURE;   // DCF ���M�Ɏ��s
        }
    }

    // ���M�����҂��u���b�N
    OS_SleepThread(&(wcmCpsifw.sendQ));

    /*
     * �u���b�N��Ԃ��畜�A�������_�ŁAWCM���C�u�����̏�Ԃ͕s���B
     * WCM ���C�u�������I������Ă��Ă��A���M���ʂ͕ێ�����Ă���̂�
     * WCM ���C�u�����̏�Ԃ��m�F����K�v�͂Ȃ��B
     */
    switch (wcmCpsifw.sendResult)
    {
    case WM_ERRCODE_SUCCESS:
        break;

    case WM_ERRCODE_FAILED:
    default:
        // ���M�Ɏ��s�����ꍇ
        WCMi_Printf(cpsifReportText_WmAsyncError, "WCM_SendDCFData", wcmCpsifw.sendResult);
        OS_UnlockMutex(&(wcmCpsifw.sendMutex));
        (void)OS_RestoreInterrupts(e);
        return WCM_CPSIF_RESULT_SEND_FAILURE;       // DCF ���M�Ɏ��s
    }

    // ����I��
    OS_UnlockMutex(&(wcmCpsifw.sendMutex));
    (void)OS_RestoreInterrupts(e);
    return headerLen + bodyLen;
}

/*---------------------------------------------------------------------------*
  Name:         WcmCpsifWmCallback

  Description:  DCF �t���[�����M�v���ɑ΂��閳���h���C�o����̔񓯊��I�ȉ�����
                �󂯎��R�[���o�b�N�֐��B

  Arguments:    arg -   �����h���C�o����̉����p�����[�^���i�[����Ă���o�b�t�@
                        �̃A�h���X���n�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmCpsifWmCallback(void* arg)
{
    WMCallback*     cb = (WMCallback*)arg;

    switch (cb->apiid)
    {
    case WM_APIID_SET_DCF_DATA:
        wcmCpsifw.sendResult = (WMErrCode) (cb->errcode);
        if (cb->errcode == WM_ERRCODE_SUCCESS)
        {
            WCMi_ResetKeepAliveAlarm();
        }

        OS_WakeupThread(&(wcmCpsifw.sendQ));
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcmCpsifKACallback

  Description:  DCF �t���[�����M�v���ɑ΂��閳���h���C�o����̔񓯊��I�ȉ�����
                �󂯎��R�[���o�b�N�֐��B�L�[�v�A���C�u NULL �p�P�b�g���M��
                ��p�B�ʏ�̃p�P�b�g���M�Ƃ͔r���I�ɑ��M������s�����ƂŁA
                ������o�^�ł��Ȃ��R�[���o�b�N�֐���؂�ւ��Ďg�����Ƃ��\�B

  Arguments:    arg -   �����h���C�o����̉����p�����[�^���i�[����Ă���o�b�t�@
                        �̃A�h���X���n�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmCpsifKACallback(void* arg)
{
#if WCM_DEBUG
    if (((WMCallback*)arg)->errcode == WM_ERRCODE_SUCCESS)
    {
        WCMi_Printf("Send NULL packet for KeepAlive.\n");
    }

#else
#pragma unused(arg)
#endif

    // �����̐��ۂɊւ�炸�r�����b�N����������
    WcmCpsifUnlockMutexInIRQ(&(wcmCpsifw.sendMutex));
}

/*---------------------------------------------------------------------------*
  Name:         WcmCpsifTryLockMutexInIRQ

  Description:  IRQ ���荞�݃T�[�r�X���ɂ����Ĕr������p Mutex �̃��b�N�����݂�B
                ���̊֐��Ń��b�N���� Mutex �� WcmCpsifUnlockMutexInIRQ �֐���
                �̂݃A�����b�N�\�B

  Arguments:    mutex   -   �r������p OSMutex �ϐ��ւ̃|�C���^���w��B

  Returns:      BOOL    -   ���b�N�ɐ��������ꍇ�� TRUE ��Ԃ��B���ɂȂ�炩��
                            �X���b�h���烍�b�N����Ă����ꍇ�ɂ� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL WcmCpsifTryLockMutexInIRQ(OSMutex* mutex)
{
#if WCM_DEBUG
    if ((mutex == NULL) || (OS_GetProcMode() != HW_PSR_IRQ_MODE))
    {
        return FALSE;
    }
#endif
    if (mutex->thread == NULL)
    {
        mutex->thread = WCMCPSIF_DUMMY_IRQ_THREAD;
        mutex->count++;
        return TRUE;
    }
    else if (mutex->thread == WCMCPSIF_DUMMY_IRQ_THREAD)
    {
        mutex->count++;
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WcmCpsifUnlockMutexInIRQ

  Description:  IRQ ���荞�݃T�[�r�X���ɂ����Ĕr������p Mutex �̃A�����b�N��
                ���݂�BWcmCpsifTryLockMutexInIRQ �֐��Ń��b�N���� Mutex �ɑ�
                ���Ă̂ݗL���B
                �A�����b�N�ł��Ȃ������ꍇ�ɂ́A�����m���i�͂Ȃ��B

  Arguments:    mutex   -   �r������p OSMutex �ϐ��ւ̃|�C���^���w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcmCpsifUnlockMutexInIRQ(OSMutex* mutex)
{
#if WCM_DEBUG
    if ((mutex == NULL) || (OS_GetProcMode() != HW_PSR_IRQ_MODE))
    {
        return;
    }
#endif
    if (mutex->thread == WCMCPSIF_DUMMY_IRQ_THREAD)
    {
        if (--(mutex->count) == 0)
        {
            mutex->thread = NULL;
            OS_WakeupThread(&(mutex->queue));
        }
    }
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
