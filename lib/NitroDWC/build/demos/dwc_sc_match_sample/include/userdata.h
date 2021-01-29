#ifndef USERDATA_H_
#define USERDATA_H_

extern void DTUD_Init( void );
extern BOOL DTUD_CheckUpdateUserData( void );
extern void DTUD_SaveUserData( void );
extern BOOL DTUD_SaveBackup( u32 dst, const void* buf, u32 size );
extern BOOL DTUD_LoadBackup( void* buf, u32 size );
extern DWCUserData* DTUD_GetUserData( void );
extern void DTUD_SaveFriendList( void );
extern void DTUD_DispFriendList( void );
DWCFriendData* DTUD_GetFriendList( void );

#endif // USERDATA_H_
