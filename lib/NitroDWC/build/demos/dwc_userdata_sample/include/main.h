#ifndef AUTOMATCH_H_
#define AUTOMATCH_H_

#include "utility.h"

extern void DrawConsole( void );
extern void DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l );

extern KeyControl		g_KeyCtrl;

enum
{
    state_init,
    state_initMPData,
    state_changeMPData,
    state_initFriendKey,
    state_changeFriendKey,
    state_readyToConnect,
    state_last
};

enum
{
    msg_seq_init,
    msg_seq_0002,
    msg_seq_0003,
    msg_seq_0004,
    msg_seq_0005,
    msg_seq_0006,
    msg_seq_0007,
    msg_seq_0008,
    msg_seq_0009,
    msg_seq_exit,
    msg_seq_last
};

#endif // AUTOMATCH_H_
