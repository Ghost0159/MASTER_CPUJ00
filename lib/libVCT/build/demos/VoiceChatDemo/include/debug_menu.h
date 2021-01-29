//
//
//

#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

#ifdef SDK_DEBUG

#include "vct.h"
#include "vct_menu.h"

#pragma profile off

//
// 音声デバッグメニュー
//
class AudioDebug : public MenuItem
{
public:
    AudioDebug() : _nowMode(0) {}
    
    const char* title()    { return "Audio Debug"; }
    int         numItems() { return 10; }
    const char* menuElement(int index, int cursor, u8* outColor);
    MenuResult  keyDown(int index, int key);

private:
    int     _nowMode;
};

#endif

#endif // __DEBUG_MENU_H__
