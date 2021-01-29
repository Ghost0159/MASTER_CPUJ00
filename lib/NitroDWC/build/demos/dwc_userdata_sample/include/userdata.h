#ifndef USERDATA_H_
#define USERDATA_H_

#if !defined(DTUD_INITIAL_CODE)
#define DTUD_INITIAL_CODE 'ABCD'
#endif

#define FRIEND_LIST_LEN  64        // —F’BƒŠƒXƒg’·

extern BOOL DTUD_SaveBackup( u32 dst, const void* buf, u32 size );
extern BOOL DTUD_LoadBackup( u32 src, void* buf, u32 size );

extern BOOL DTUD_Init( void );

extern BOOL DTUD_CheckUpdateUserData( void );
extern void DTUD_SaveUserData( void );
extern void DTUD_DeleteUserData( void );
extern DWCUserData* DTUD_GetUserData( void );

extern void DTUD_SaveFriendList( void );
extern void DTUD_DeleteFriendList( void );
extern DWCFriendData* DTUD_GetFriendList( void );

extern int  DTUD_GetAvailableFriendListIndex( void );
extern void DTUD_DispFriendList( void );

#endif // USERDATA_H_
