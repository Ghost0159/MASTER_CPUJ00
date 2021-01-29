//
//
//

#ifndef __DEMO_MENU_H__
#define __DEMO_MENU_H__

#include "vct.h"
#include "vct_menu.h"

#pragma profile off

enum {
    MENU_AUDIO_INFO,
    MENU_VAD,
    MENU_AUDIO_DEBUG,

    NUM_OF_MENU
};

//
//  �����X�g���[�~���O���\�� 
//
class AudioInfo : public MenuItem
{
public:
    AudioInfo() : _channel(0) {}
    
    const char* title()    { return "AudioInfo"; }
    int         numItems() { return 0; }
    const char* menuElement(int index, int cursor, u8* outColor);
    MenuResult  keyDown(int index, int key);
    
    int         showExInfo(int y, int cursor);

private:
    int    _channel;
};

//
//  VAD���\���A�ݒ�ύX 
//
class VADInfo : public MenuItem
{
public:
    VADInfo();
    
    const char* title()    { return "VAD info"; }
    int         numItems() { return 4; }
    const char* menuElement(int index, int cursor, u8* outColor);
    MenuResult  keyDown(int index, int key);
    int         showExInfo(int y, int cursor);

private:
    int         _gain;
    VCTVADInfo  _vadInfo;
    BOOL        _enableVAD;
    BOOL        _enableEC;

};

#endif // __DEMO_MENU_H__
