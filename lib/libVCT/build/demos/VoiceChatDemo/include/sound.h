#ifndef __SOUND_H__
#define __SOUND_H__

#include <nnsys/snd.h>
#include "vct.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    SE_RINGIN = 0,
    SE_RINGOUT,
    SE_BEEP,
    SE_NOISE,
    SE_BUSY
};

void        SetupSound();
void        StartSoundLoop();
void        StopSoundLoop();
void        TerminateSound();

void    PlaySE(int id);
void    StopSE();
void    SendBeep(BOOL flag);

#ifdef __cplusplus
}
#endif
    
#endif // __SOUND_H__
