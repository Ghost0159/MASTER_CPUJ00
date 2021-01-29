#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "userdata.h"
#include "utility.h"
#include "screen.h"
#include "font.h"
#include "dbs.h"
#include "wc.h"

#include "mp.h"

/** --------------------------------------------------------------------
  define
  ----------------------------------------------------------------------*/
#define     PICTURE_FRAME_PER_GAME_FRAME    1

#define     DEFAULT_GGID            0x003fff11
#define     DEFAULT_CHAN            1
#define     NUM_MAX_CHILD           15

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static u16* s_SendBuf;
static DWCFriendData* s_FriendList;
//�f�[�^��M�p�t���O
static BOOL s_RecvFlag[NUM_MAX_CHILD];

/** --------------------------------------------------------------------
  static function
  ----------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------*
  Name:         MP_SetData

  Description:  ���M�p�f�[�^�̏��������s���B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void MP_SetData(u16* sendbuf, DWCFriendData* friendlist)
{
    s_SendBuf = sendbuf;
    s_FriendList = friendlist;

    // ��M�t���O�̏�����
    MI_CpuClear8( s_RecvFlag, sizeof(s_RecvFlag) );

    // WM������
    WcInit(&defaultParameter, TRUE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);

}

/*---------------------------------------------------------------------------*
  Name:         MP_Match

  Description:  MP�Ńf�[�^�̑���M���s���B

  Arguments:    NONE.

  Returns:      �ʐM���I���A�܂��̓L�����Z�����ꂽ�ꍇTRUE
                �ʐM�r���̏ꍇFALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL MP_Match()
{
    static u8 child_flag = 0;//�ʐM����p�t���O

    // �ʐM��Ԃɂ�菈����U�蕪��
    switch (WcGetStatus())
    {
    case WC_STATUS_READY:// �e�@�A�q�@�̑I��
        if(child_flag){// �e����ؒf���ꂽ�q�@�̒ʐM�I��
            child_flag = 0;
            WcEnd();
            return TRUE;
            break;
        }
        ModeSelect();
        break;
    case WC_STATUS_ERROR:
        ModeError();
        break;
    case WC_STATUS_BUSY:// ���s��
        ModeWorking();
        break;
    case WC_STATUS_PARENT:// �e�@�̏ꍇ
        ModeParent(s_SendBuf, s_FriendList);
        break;
    case WC_STATUS_CHILD:// �q�@�̏ꍇ
        ModeChild(s_SendBuf, s_FriendList);
        child_flag = 1;// �e����ؒf������Ready��Ԃɖ߂��Ă��܂����߂̑Ώ�
        break;
    }
    if (g_KeyCtrl.trg & PAD_BUTTON_B){//�ʐM�I��
        child_flag = 0;
        WcEnd();
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         MP_Finish

  Description:  MP�ʐM���I��������B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void MP_Finish()
{
    // WM�̏I��
    WcFinish();
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
    // �����\��
    DispMessage( "MP MODE SELECT", NULL, "MP Cancel", "Parent Mode", "Child Mode", NULL);

    if (g_KeyCtrl.trg & PAD_BUTTON_X)
    {
        // �e�@�ʐM�J�n
        WcStartParent();
        return 0;
    }
    else if (g_KeyCtrl.trg & PAD_BUTTON_Y)
    {
        // �q�@�ڑ��J�n
        WcStartChild();
        return 0;
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
    // �����\��
    DispMessage( "ERROR!\n Fatal error occured.\n Please reboot program.", NULL, NULL, NULL, NULL, NULL);
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
    // �����\��
    DispMessage( "NOW WORKING...", NULL, "MP Cancel", NULL, NULL, NULL);

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

    // �����\��
    DispMessage( "PARENT MODE", NULL, "MP Cancel", NULL, NULL, NULL);
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

    // �����\��
    DispMessage( "CHILD MODE", NULL, "MP Cancel", NULL, NULL, NULL);
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
    u8 friend_number = 0;


    // �e�@ + �q�@�ő吔�������[�v
    for (i = 0; i < (1 + WM_NUM_MAX_CHILD); i++)
    {
        if (bitmap & (0x0001 << i))
        {
            // ID��"i"�̒[������̃f�[�^�擾
            p = WcGetSharedDataAddress(i);

            if (p != NULL)
            {
                friend_number++;
                if(!s_RecvFlag[i] && WcGetAid() != i){// �����������t�����h�f�[�^���擾
                    friendIdx = DTUD_GetAvailableFriendListIndex();
                    s_RecvFlag[i] = 1;
                    MI_CpuCopy8(p, &friendlist[friendIdx], sizeof(DWCFriendData));
                }
//                PrintString(2, (s16)(15 + i), 0x4, "Friend %d MP Data Received!!", i);
            }
        }
        else{
            s_RecvFlag[i] = 0;
        }
    }
    PrintString(2, 15, 0x4, "Changed Number of Friend %d", friend_number-1);
    // ���@�̃f�[�^�����F��ύX
//    ColorString(2, (s16)(15 + WcGetAid()), 25, 0x1);
}
