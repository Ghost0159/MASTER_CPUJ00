//
//
//
#include "vct.h"
#include "vct_menu.h"
#include "debug_menu.h"

#ifdef SDK_DEBUG

#pragma profile off

/////////////////////////////////////////////////////////////////////////////////////
//
//  オーディオデバッグメニュー 
//
extern "C" { void VCTi_AudioDebug(int param); }

static const char * const __menu_elements[11] = {
    "No debug",
    "Jam packet",
    "Drop packet",
    "Jitter low  (<32ms)",
    "Jitter high (<96ms)",
    "Spike",
    "Invalid size",
    "Future timestamp",
    "Past timestamp",
    "Invalid sequence",
};

const char* AudioDebug::menuElement(int index, int cursor, u8* outColor)
{
#pragma unused(cursor, outColor)
    static char buf[32];
    OS_SNPrintf(buf, 32, "%c %s", (index == _nowMode) ? '*' : ' ', __menu_elements[index]);
    return buf;
}

int AudioDebug::keyDown(int index, int key)
{
    if (key & PAD_BUTTON_A) {
        VCTi_AudioDebug(index);
        _nowMode = index;
        key &= ~PAD_BUTTON_A;
    }
    return key;
}
#endif
