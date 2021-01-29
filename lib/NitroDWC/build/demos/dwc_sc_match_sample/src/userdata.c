#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
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

#if !defined(DTUD_INITIAL_CODE)
#define DTUD_INITIAL_CODE 'ABCD'
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
DTUD_LoadBackup( void* buf, u32 size )
{
    int last_result;

    CARD_LockBackup(s_CardLockId);
    if (CARD_IsBackupEeprom())
    {
        CARD_ReadEeprom(0, buf, size );
    }
    else if (CARD_IsBackupFlash())
    {
        CARD_ReadFlash(0, buf, size );
    }
    else if (CARD_IsBackupFram())
    {
        CARD_ReadFram(0, buf, size );
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
void
DTUD_Init( void )
{
    // �o�b�N�A�b�v�̃`�F�b�N
    if ( DTUDs_CheckBackup() )
    {
        (void)DTUD_LoadBackup( &s_PlayerInfo, sizeof(DTUDPlayerInfo) );

        OS_TPrintf("Load From Backup\n");

        if ( !DWC_CheckUserData( &s_PlayerInfo.userData ) )
        {
            // ���߂���������B
            DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );

            OS_TPrintf("But UserData was broken.\n");
        }

        if ( !DWC_CheckValidConsole( &s_PlayerInfo.userData ) )
        {
            // �R���\�[�����Ԉ���Ă���Logon�ł��Ȃ��̂ŁA��蒼���B
            DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );

            OS_TPrintf("UserData is not Valid Console.\nCreate UserData.\n");
        }
    }
    else
    {
        OS_TPrintf("no Backup.. create UserData\n");
        // �Ȃ��Ƃ��͍��B
        DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );
    }

    DWC_ReportUserData( &s_PlayerInfo.userData );

    // Dirty�`�F�b�N������B
    if ( DTUD_CheckUpdateUserData() )
    {
        DTUD_SaveUserData();
    }
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
  Name:         DTUD_GetFriendList

  Description:  �F�B���X�g���擾����B

  Arguments:    NONE.

  Returns:      �F�B���X�g�̃|�C���^��Ԃ��B
 *---------------------------------------------------------------------------*/
DWCFriendData* DTUD_GetFriendList( void )
{
    return s_PlayerInfo.friendList;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_DispFriendList

  Description:  �F�B���X�g��\������B

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void DTUD_DispFriendList( void )
{
    u8   state;
    u8   maxEntry;
    u8   numEntry;
    int  profileID;
    int  i;
    
    for ( i = 0; i < FRIEND_LIST_LEN; i++ )
    {
        if ( !DWC_IsValidFriendData( &s_PlayerInfo.friendList[i] ) ) continue;

        state = DWC_GetFriendStatusSC( &s_PlayerInfo.friendList[i], &maxEntry, &numEntry, NULL );
        profileID = DWC_GetGsProfileId( DTUD_GetUserData(), &s_PlayerInfo.friendList[i] );
        if ( profileID < 0 ) profileID = 0;
        
        OS_TPrintf( "[%d] (s:%d, p:%u) %d/%d\n",
                   i, state, profileID, numEntry, maxEntry );
    }
    OS_TPrintf("\n");
}
