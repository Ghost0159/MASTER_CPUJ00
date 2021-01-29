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
  バックアップはコンパイルタイムに決定します。
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

  Description:  カードのバックアップの状態を確認する。

  Arguments:    None.

  Returns:      BOOL - カードのバックアップデバイスが読み出せる状態ならTRUE、
                       読み出せない状態ならFALSEを返す。
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

  Description:  カードのバックアップからのロードを行う。

  Arguments:    src  - 転送元のアドレス
                buf  - 転送先のメモリアドレス
                size - 読み出すデータのバイト長

  Returns:      BOOL - カードからデータの読み出しに成功したらTRUE、
                       失敗ならFALSEを返す。
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

  Description:  カードのバックアップへのセーブを行う。

  Arguments:    dst  - 転送先のアドレス
                buf  - 転送元のメモリアドレス
                size - 読み出すデータのバイト長

  Returns:      BOOL - カードへデータの書き込みに成功したらTRUE、
                       失敗ならFALSEを返す。
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

  Description:  カードにユーザデータがあればそれを使用し、
  　　　　　　　なければ、新たにユーザデータを作成する。

  Arguments:    NONE.

  Returns:      BOOL - カードにユーザデータがあればTRUE
                       なければFALSEを返す。
 *---------------------------------------------------------------------------*/
void
DTUD_Init( void )
{
    // バックアップのチェック
    if ( DTUDs_CheckBackup() )
    {
        (void)DTUD_LoadBackup( &s_PlayerInfo, sizeof(DTUDPlayerInfo) );

        OS_TPrintf("Load From Backup\n");

        if ( !DWC_CheckUserData( &s_PlayerInfo.userData ) )
        {
            // だめだったら作る。
            DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );

            OS_TPrintf("But UserData was broken.\n");
        }

        if ( !DWC_CheckValidConsole( &s_PlayerInfo.userData ) )
        {
            // コンソールが間違ってたらLogonできないので、作り直す。
            DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );

            OS_TPrintf("UserData is not Valid Console.\nCreate UserData.\n");
        }
    }
    else
    {
        OS_TPrintf("no Backup.. create UserData\n");
        // ないときは作る。
        DWC_CreateUserData( &s_PlayerInfo.userData, DTUD_INITIAL_CODE );
    }

    DWC_ReportUserData( &s_PlayerInfo.userData );

    // Dirtyチェックをする。
    if ( DTUD_CheckUpdateUserData() )
    {
        DTUD_SaveUserData();
    }
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_CheckUpdateUserData

  Description:  ユーザデータのDirtyフラグチェックを行う。

  Arguments:    NONE.

  Returns:      BOOL - ユーザデータが変更されていればTRUE
                       されてなければFALSEを返す。
 *---------------------------------------------------------------------------*/
BOOL
DTUD_CheckUpdateUserData( void )
{
    return DWC_CheckDirtyFlag( &s_PlayerInfo.userData );
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_SaveUserData

  Description:  ユーザデータをカードにセーブする。

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

  Description:  ユーザデータを取得する。

  Arguments:    NONE.

  Returns:      ユーザデータのポインタを返す。
 *---------------------------------------------------------------------------*/
DWCUserData* DTUD_GetUserData( void )
{
    return &s_PlayerInfo.userData;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_SaveFriendList

  Description:  友達リストをカードにセーブする。

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

  Description:  友達リストを取得する。

  Arguments:    NONE.

  Returns:      友達リストのポインタを返す。
 *---------------------------------------------------------------------------*/
DWCFriendData* DTUD_GetFriendList( void )
{
    return s_PlayerInfo.friendList;
}

/*---------------------------------------------------------------------------*
  Name:         DTUD_DispFriendList

  Description:  友達リストを表示する。

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
