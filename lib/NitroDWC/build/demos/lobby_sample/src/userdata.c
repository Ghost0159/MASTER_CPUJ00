#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "userdata.h"
#include "init.h"
#include "dbs.h"

/** --------------------------------------------------------------------
  typedef
  ----------------------------------------------------------------------*/
typedef struct stUDBackupInfo
{
    CARDBackupType type;
    const char *comment;
} DTUDBackupInfo;

typedef struct stUDPlayerInfo
{
    DWCUserData   userData;
    DWCFriendData friendList[FRIEND_LIST_LEN];
} DTUDPlayerInfo;

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static DTUDPlayerInfo s_PlayerInfo;
static u16 s_CardLockId;

/** --------------------------------------------------------------------
  �o�b�N�A�b�v�̓R���p�C���^�C���Ɍ��肵�܂��B
  ----------------------------------------------------------------------*/
#if !defined(DTUD_BACKUP_TYPE)
//#define DTUD_BACKUP_TYPE CARD_BACKUP_TYPE_EEPROM_4KBITS
#define DTUD_BACKUP_TYPE CARD_BACKUP_TYPE_EEPROM_64KBITS
//#define DTUD_BACKUP_TYPE CARD_BACKUP_TYPE_EEPROM_512KBITS
//#define DTUD_BACKUP_TYPE CARD_BACKUP_TYPE_FLASH_2MBITS
//#define DTUD_BACKUP_TYPE CARD_BACKUP_TYPE_FRAM_256KBITS
#endif

/*---------------------------------------------------------------------------*
  Name:         DTUDs_CheckBackup

  Description:  �J�[�h�̃o�b�N�A�b�v�̏�Ԃ��m�F����B

  Arguments:    None.

  Returns:      BOOL - �J�[�h�̃o�b�N�A�b�v�f�o�C�X���ǂݏo�����ԂȂ�TRUE�A
                       �ǂݏo���Ȃ���ԂȂ�FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL
DTUDs_CheckBackup( void )
{
    BOOL end = FALSE;
    s32 ret;

    ret = OS_GetLockID();
    if (ret == OS_LOCK_ID_ERROR)
    {
        return FALSE;
    }
    s_CardLockId = (u16)ret;

    CARD_LockBackup(s_CardLockId);
    end = CARD_IdentifyBackup( DTUD_BACKUP_TYPE );
    CARD_UnlockBackup(s_CardLockId);

    return end;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_LoadBackup

  Description:  �J�[�h�̃o�b�N�A�b�v����̃��[�h���s���B

  Arguments:    src  - �]�����̃A�h���X
                buf  - �]����̃������A�h���X
                size - �ǂݏo���f�[�^�̃o�C�g��

  Returns:      BOOL - �J�[�h����f�[�^�̓ǂݏo���ɐ���������TRUE�A
                       ���s�Ȃ�FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL
DTUD_LoadBackup( u32 src, void* buf, u32 size )
{
    int last_result;

    CARD_LockBackup(s_CardLockId);
    if (CARD_IsBackupEeprom())
    {
        CARD_ReadEeprom(src, buf, size );
    }
    else if (CARD_IsBackupFlash())
    {
        CARD_ReadFlash(src, buf, size );
    }
    else if (CARD_IsBackupFram())
    {
        CARD_ReadFram(src, buf, size );
    }
    CARD_UnlockBackup(s_CardLockId);

    last_result = CARD_GetResultCode();
    if (last_result != CARD_RESULT_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_SaveBackup

  Description:  �J�[�h�̃o�b�N�A�b�v�ւ̃Z�[�u���s���B

  Arguments:    dst  - �]����̃A�h���X
                buf  - �]�����̃������A�h���X
                size - �ǂݏo���f�[�^�̃o�C�g��

  Returns:      BOOL - �J�[�h�փf�[�^�̏������݂ɐ���������TRUE�A
                       ���s�Ȃ�FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL
DTUD_SaveBackup( u32 dst, const void* buf, u32 size )
{
    int last_result;
    
    CARD_LockBackup(s_CardLockId);
    if (CARD_IsBackupEeprom())
    {
        CARD_WriteAndVerifyEeprom(dst, buf, size );
    }
    else if (CARD_IsBackupFlash())
    {
        CARD_WriteAndVerifyFlash(dst, buf, size );
    }
    else if (CARD_IsBackupFram())
    {
        CARD_WriteAndVerifyFram(dst, buf, size );
    }
    CARD_UnlockBackup(s_CardLockId);

    last_result = CARD_GetResultCode();
    if (last_result != CARD_RESULT_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_Init

  Description:  �J�[�h�Ƀ��[�U�f�[�^������΂�����g�p���A
  �@�@�@�@�@�@�@�Ȃ���΁A�V���Ƀ��[�U�f�[�^���쐬����B

  Arguments:    NONE.

  Returns:      BOOL - �J�[�h�Ƀ��[�U�f�[�^�������TRUE
                       �Ȃ����FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL
DTUD_Init( void )
{

    // �o�b�N�A�b�v�f�[�^�������āA�������Ƀ��[�U�f�[�^������΁A
    // �S�ă��[�h����TRUE��Ԃ�
    if ( DTUDs_CheckBackup() )
    {
        (void)DTUD_LoadBackup( 0, &s_PlayerInfo, sizeof(DTUDPlayerInfo) );

        OS_TPrintf("Load From Backup\n");

        if ( DWC_CheckUserData( &s_PlayerInfo.userData ) )
        {
            DWC_ReportUserData( &s_PlayerInfo.userData );
            return TRUE;
        }
    }

    // �L���ȃ��[�U�f�[�^���Z�[�u����Ă��Ȃ������ꍇ
    OS_TPrintf("no Backup UserData\n");

    // ���[�U�f�[�^���쐬����
    DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );

    OS_TPrintf("Create UserData.\n");
    DWC_ReportUserData( &s_PlayerInfo.userData );

    // �F�B���X�g������
    MI_CpuClear32( &s_PlayerInfo.friendList, sizeof( s_PlayerInfo.friendList ) );
    
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_CheckUpdateUserData

  Description:  ���[�U�f�[�^��Dirty�t���O�`�F�b�N���s���B

  Arguments:    NONE.

  Returns:      BOOL - ���[�U�f�[�^���ύX����Ă����TRUE
                       ����ĂȂ����FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL
DTUD_CheckUpdateUserData( void )
{
    return DWC_CheckDirtyFlag( &s_PlayerInfo.userData );
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_SaveUserData

  Description:  ���[�U�f�[�^���J�[�h�ɃZ�[�u����B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void
DTUD_SaveUserData( void )
{
    if ( DTUDs_CheckBackup() )
    {
        DWC_ClearDirtyFlag( &s_PlayerInfo.userData );
        DTUD_SaveBackup( 0, &s_PlayerInfo.userData, sizeof(DWCUserData) );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_DeleteUserData

  Description:  ���[�U�f�[�^�����[�J���A�J�[�h�����������B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void
DTUD_DeleteUserData( void )
{
    if ( DTUDs_CheckBackup() )
    {
        MI_CpuClear32( &s_PlayerInfo.userData, sizeof( DWCUserData ) );
        DTUD_SaveBackup( 0, &s_PlayerInfo.userData, sizeof(DWCUserData) );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_GetUserData

  Description:  ���[�U�f�[�^���擾����B

  Arguments:    NONE.

  Returns:      ���[�U�f�[�^�̃|�C���^��Ԃ��B
 *---------------------------------------------------------------------------*/
DWCUserData* DTUD_GetUserData( void )
{
    return &s_PlayerInfo.userData;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_SaveFriendList

  Description:  �F�B���X�g���J�[�h�ɃZ�[�u����B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void
DTUD_SaveFriendList( void )
{
    if ( DTUDs_CheckBackup() )
    {
        DTUD_SaveBackup( (u32)s_PlayerInfo.friendList - (u32)&s_PlayerInfo,
                         &s_PlayerInfo.friendList, sizeof( s_PlayerInfo.friendList ) );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_DeleteFriendList

  Description:  �F�B���X�g�����[�J���A�J�[�h�����������B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void
DTUD_DeleteFriendList( void )
{
    if ( DTUDs_CheckBackup() )
    {
        MI_CpuClear32( &s_PlayerInfo.friendList, sizeof( s_PlayerInfo.friendList ) );
        DTUD_SaveBackup( (u32)s_PlayerInfo.friendList - (u32)&s_PlayerInfo,
                         &s_PlayerInfo.friendList, sizeof( s_PlayerInfo.friendList ) );
    }
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_GetFriendList

  Description:  �F�B���X�g���擾����B

  Arguments:    NONE.

  Returns:      �F�B���X�g�̃|�C���^��Ԃ��B
 *---------------------------------------------------------------------------*/
DWCFriendData* DTUD_GetFriendList( void )
{
    return s_PlayerInfo.friendList;
}

