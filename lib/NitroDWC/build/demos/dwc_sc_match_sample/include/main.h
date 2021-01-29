#ifndef AUTOMATCH_H_
#define AUTOMATCH_H_

#include "utility.h"

#define FRIEND_LIST_LEN  64        // �F�B���X�g��

typedef struct stMatchResult
{
    OSTick		start_time;	// matching�J�n����
    OSTick		end_time;	// matching�I������

    BOOL		success;	// ����/���s
    u32			error_code;	// �G���[�R�[�h
} MatchResult;

extern void NitroMain ();
extern void drawConsole( void );

extern KeyControl		g_KeyCtrl;

enum
{
    state_init,
    state_connect,
    state_login,
    state_online,
    state_server,
    state_client,
    state_matching,
    state_matched,
    state_close,
    state_disconnect,
    state_save,
    state_error,
    state_last
};

#endif // AUTOMATCH_H_
