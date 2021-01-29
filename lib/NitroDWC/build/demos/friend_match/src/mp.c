#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "screen.h"
#include "font.h"
#include "dbs.h"
#include "wc.h"

#include "mp.h"

#define     PICTURE_FRAME_PER_GAME_FRAME    1

#define     DEFAULT_GGID            0x003fff11
#define     DEFAULT_CHAN            1
#define     NUM_MAX_CHILD           15

extern KeyControl stKeyCnt;    // �L�[���͐���\����

//MP�ʐM�p���[�h
static u8 ModeSelect(void);             // �e�@/�q�@ �I��
static void ModeError(void);            // �G���[�\��
static u8 ModeWorking(void);            // �r�W�[
static void ModeParent(u16* gSendBuf, DWCFriendData* friendlist);  // �e�@ �ʐM
static void ModeChild(u16* gSendBuf, DWCFriendData* friendlist);   // �q�@ �ʐM

static void PrintSharedData(u16 bitmap, DWCFriendData* friendlist);// �V�F�A���ꂽ�f�[�^�̕\��

//MP�ʐM�p�e�@�̏����p�����[�^
static WMParentParam defaultParameter ATTRIBUTE_ALIGN(32) = {
    NULL, 0, 0,
    DEFAULT_GGID,                             // �Q�[���O���[�vID
    0x0000,                                   // temporary ID (����+1�����)
    1,                                        // �G���g���[���t���O
    NUM_MAX_CHILD,                            // �ő�ڑ��q�@��
    0,                                        // �}���`�u�[�g�t���O
    0,                                        // �L�[�V�F�A�����O
    0,                                        // �A�����M�t���O
    100,                                      // beacon �Ԋu
    {0, 0, 0, 0},                             // ���[�U�[��
    {0, 0, 0, 0, 0, 0, 0, 0},                 // �Q�[����
    DEFAULT_CHAN,                             // �ڑ��`�����l��
    sizeof(DWCFriendData)*(1+NUM_MAX_CHILD)+4,// �e�@���M�f�[�^�T�C�Y
    sizeof(DWCFriendData)                     // �q�@���M�f�[�^�T�C�Y
};

//�f�[�^��M�p�t���O
static BOOL gRecvFlag[NUM_MAX_CHILD];

void mp_match(u16* gSendBuf, DWCFriendData* friendlist){
    u8 child_flag = 0, mp_flag = 0;//�ʐM����p�t���O

    MI_CpuClear8( gRecvFlag, sizeof(gRecvFlag) );

    // WM������
    WcInit(&defaultParameter, TRUE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);
    // �ʐM��Ԃɂ�菈����U�蕪��
    while(!mp_flag){
        switch (WcGetStatus())
        {
        case WC_STATUS_READY:// �e�@�A�q�@�̑I��
            if(child_flag){
                mp_flag = 1;
                break;
            }
            mp_flag = ModeSelect();
            break;
        case WC_STATUS_ERROR:
            ModeError();
             break;
        case WC_STATUS_BUSY:// ���s��
            mp_flag = ModeWorking();
            break;
        case WC_STATUS_PARENT:// �e�@�̏ꍇ
            ModeParent(gSendBuf, friendlist);
            break;
        case WC_STATUS_CHILD:// �q�@�̏ꍇ
            ModeChild(gSendBuf, friendlist);
            child_flag = 1;//�e����ؒf������Ready��Ԃɖ߂��Ă��܂����߂̑Ώ�
            break;
        }
        ReadKeyData();  // �L�[�f�[�^�擾
        if (stKeyCnt.trg & PAD_BUTTON_B){//�ʐM�I��
        	WcEnd();
            mp_flag = 1;
        }

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // �X�^�b�N���`�F�b�N
        OS_CheckStack(OS_GetCurrentThread());
    }
    // WM�̏I��
    WcFinish();
    ClearScreen();
}

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  �e�@/�q�@ �I����ʂł̏����B

  Arguments:    None.

  Returns:      0: ���[�h����
                1: �L�����Z��
 *---------------------------------------------------------------------------*/
static u8 ModeSelect(void)
{
    // �X�N���[���N���A
    ClearScreen();
    // �����\��
    PrintString(3, 8, 0xf,  "Push L to connect as PARENT");
    PrintString(3, 10, 0xf, "Push R to connect as CHILD");
    PrintString(3, 16, 0xf, "Push B to STOP");

    dbs_DemoUpdate();
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
    dbs_DemoLoad();
    // �X�^�b�N���`�F�b�N
    OS_CheckStack(OS_GetCurrentThread());
    
    while(1){
        ReadKeyData();  // �L�[�f�[�^�擾
        if (stKeyCnt.trg & PAD_BUTTON_L)
        {
            // �e�@�ʐM�J�n
            WcStartParent();
            return 0;
        }
        else if (stKeyCnt.trg & PAD_BUTTON_R)
        {
            // �q�@�ڑ��J�n
            WcStartChild();
            return 0;
        }
        else if (stKeyCnt.trg & PAD_BUTTON_B)
        {
            return 1;
        }
    }
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�\����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    // �X�N���[���N���A
    ClearScreen();
    // �����\��
    PrintString(5, 10, 0x1, "======= ERROR! =======");
    PrintString(5, 13, 0xf, " Fatal error occured.");
    PrintString(5, 14, 0xf, "Please reboot program.");
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  �r�W�[��ʂł̏����B

  Arguments:    None.

  Returns:      0: ���s��
                1: �ʐM�I��
 *---------------------------------------------------------------------------*/
static u8 ModeWorking(void)
{
    // �X�N���[���N���A
    ClearScreen();
    // �����\��
    PrintString(9, 9, 0xf, "Now working...");
    PrintString(3, 16, 0xf, "Push B to STOP");

    ReadKeyData();  // �L�[�f�[�^�擾
    if (stKeyCnt.trg & PAD_BUTTON_B)
    {
        // �ʐM�I��
        WcEnd();
        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  �e�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeParent(u16* gSendBuf, DWCFriendData* friendlist)
{
    BOOL result;
    u16 bitmap;

    // �f�[�^�V�F�A����
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);

    // �X�N���[���N���A
    ClearScreen();
    // �����\��
    PrintString(8, 1, 0x2, "Parent mode");
    // �V�F�A���ꂽ�f�[�^��\��
    if (result)
    {
        PrintSharedData(bitmap, friendlist);
    }
    else
    {
        // ���̃t���[���� StepDataSharing �Ƀg���C
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  �q�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChild(u16* gSendBuf, DWCFriendData* friendlist)
{
    BOOL result;
    u16 bitmap;

    // �f�[�^�V�F�A����
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);

    // �X�N���[���N���A
    ClearScreen();
    // �����\��
    PrintString(8, 1, 0x2, "Child mode");
    // �V�F�A���ꂽ�f�[�^��\��
    if (result)
    {
        PrintSharedData(bitmap, friendlist);
    }
    else
    {
        // ���̃t���[���� StepDataSharing �Ƀg���C
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintSharedData

  Description:  ���L���ꂽ�e�[������̃f�[�^��\������B

  Arguments:    bitmap - �f�[�^����M�����[��������ID�r�b�g�}�b�v���w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintSharedData(u16 bitmap, DWCFriendData* friendlist)
{
    u16 i;
    u8 *p;
    int friendIdx;


    // �e�@ + �q�@�ő吔�������[�v
    for (i = 0; i < (1 + WM_NUM_MAX_CHILD); i++)
    {
        if (bitmap & (0x0001 << i))
        {
            // ID��"i"�̒[������̃f�[�^�擾
            p = WcGetSharedDataAddress(i);

            if (p != NULL)
            {
                if(!gRecvFlag[i] && WcGetAid() != i){// �����������t�����h�f�[�^���擾
                    friendIdx = GetAvailableFriendListIndex();
                    gRecvFlag[i] = 1;
                    MI_CpuCopy8(p, &friendlist[friendIdx], sizeof(DWCFriendData));
                }
                PrintString(2, (s16)(3 + i), 0x4, "Number%d Receive Success!!", i);
            }
            else
            {
                PrintString(2, (s16)(3 + i), 0x1, "xxxxxxxxxxxxxxxx");
            }
        }
        else{
            gRecvFlag[i] = 0;
        }
    }
    PrintString(3, 20, 0xf, "Push B to STOP");
    // ���@�̃f�[�^�����F��ύX
    ColorString(2, (s16)(3 + WcGetAid()), 25, 0x1);
}
