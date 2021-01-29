#ifndef AUTOMATCH_H_
#define AUTOMATCH_H_

#include "utility.h"
#include "lobby.h"

extern void NitroMain ();
extern void drawConsole( void );
extern void receivedRecruit(s32 userId, PPW_LobbyRecruitInfo recruitInfo);
extern void receivedRecruitStopped(s32 userId, PPW_LobbyRecruitInfo recruitInfo);

extern KeyControl		g_KeyCtrl;

/** --------------------------------------------------------------------
  defines
  ----------------------------------------------------------------------*/
#define GAME_NAME        "dwctest" // ���̃T���v�����g�p����Q�[����
#define GAME_SECRET_KEY  "d4q9GZ"  // ���̃T���v�����g�p����V�[�N���b�g�L�[
#define GAME_PRODUCTID   10824      // ���̃T���v�����g�p����v���_�N�gID
#define	MAX_PLAYERS		 2	       // �������܂߂��ڑ��l��

typedef struct stMatchKey
{
    char	key[16];
    char	value[16];
} MatchKey;

enum
{
    state_init,
    state_connect,
    state_login,
    state_initialize_lobby,
    state_login_lobby,
    state_online,
    state_matching,
    state_matched,
    state_disconnect,
    state_save,
    state_error,
    state_last
};

#endif // AUTOMATCH_H_
