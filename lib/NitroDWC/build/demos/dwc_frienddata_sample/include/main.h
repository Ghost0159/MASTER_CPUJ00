#ifndef AUTOMATCH_H_
#define AUTOMATCH_H_

#include "utility.h"

extern void NitroMain ();
extern void DrawConsole( void );

extern KeyControl		g_KeyCtrl;
/*
typedef struct stMatchKey
{
    char	key[16];
    char	value[16];
} MatchKey;
*/
enum
{
    state_init,
    state_connect,
    state_login,
    state_online,
    state_initFriendKey,
    state_changeFriendKey,
    state_initFriendData,
    state_inputFriendIndex,
    state_deleteFriendData,
    state_disconnect,
    state_save,
    state_error,
    state_last
};

#endif // AUTOMATCH_H_
