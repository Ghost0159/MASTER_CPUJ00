//
//
//
#include "vct.h"
#include "vct_menu.h"
#include "demo_menu.h"
#include "screen.h"

#pragma profile off

static void show_level_meter(int y, VCTVADInfo *info)
{
    int i;
    ++y;
    
    int level = info->scale;

    if (level == 0) level = 1;

    // SND_CalcDecibelを用いて、0-127のスケール値をデシベル値に変更して
    // レベルメーターを表示 
    //
    level = SND_CalcDecibel(level);

    level = (421 + level) / 13;
    if (level == 0) level = 1;
    if (level > 31) level = 31;
    for (i = 0; i < level; ++i) {
        if (i >= 28) {
            PutMainScreen((s16)i, (s16)y, FONT_COLOR_RED, "%c", 0x7f);
        }
        else {
            PutMainScreen((s16)i, (s16)y, (info->activity) ? FONT_COLOR_GREEN : FONT_COLOR_GRAY, "%c", 0x7f);
        }
    }

}

VADInfo::VADInfo()
    : _gain(PM_AMPGAIN_160), _enableVAD(TRUE), _enableEC(FALSE)
{
    VCT_GetVADInfo(&_vadInfo);
    PM_SetAmpGain((PMAmpGain)_gain);
}

MenuResult VADInfo::keyDown(int index, int key)
{
    if (!(key & (PAD_BUTTON_A | PAD_KEY_LEFT | PAD_KEY_RIGHT))) {
        return key;
    }
    switch (index) {
    case 0:
        _enableVAD = !_enableVAD;
        VCT_EnableVAD(_enableVAD);
        break;

    case 1:
        _enableEC = !_enableEC;
        VCT_EnableEchoCancel(_enableEC);
        break;

    case 2:
        if (key & PAD_KEY_RIGHT) {
            if (++_gain > (int)PM_AMPGAIN_160) _gain = (int)PM_AMPGAIN_160;
        }
        else if (key & PAD_KEY_LEFT) {
            if (--_gain < (int)PM_AMPGAIN_20) _gain = (int)PM_AMPGAIN_20;
        }
        PM_SetAmpGain((PMAmpGain)_gain);
        VCT_ResetVAD();
        break;

    case 3:
        if (key & PAD_KEY_RIGHT) {
            if (++_vadInfo.releaseCount > 50) _vadInfo.releaseCount = 50;
        }
        else if (key & PAD_KEY_LEFT) {
            if (--_vadInfo.releaseCount < 0) _vadInfo.releaseCount = 0;
        }
        VCT_SetVADReleaseTime(_vadInfo.releaseCount);
        break;

    }

    return key & ~(PAD_BUTTON_A | PAD_KEY_LEFT | PAD_KEY_RIGHT);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  VAD情報表示メニュー 
//

static const char *gain_str[4] = {
    "20", "40", "80", "160"
};

const char* VADInfo::menuElement(int index, int cursor, u8* outColor)
{
#pragma unused(cursor, outColor)
    switch (index) {
    case 0:
        return (_enableVAD) ? "VAD On" : "VAD Off";

    case 1:
        return (_enableEC) ? "Echo Cancel On" : "Echo Cancel Off";

    case 2:
        return "[<-]    MIC gain      [->]";
    case 3:
        return "[<-] VAD release time [->]";
    default:
        return "Error";
    }
}

int VADInfo::showExInfo(int y, int cursor)
{
#pragma unused(cursor)
    
    VCT_GetVADInfo(&_vadInfo);
    
    ++y;
    PutMainScreen(0, (s16)y++, 15, "Mic Amp gain:      %sdb", gain_str[_gain]);
    PutMainScreen(0, (s16)y++, 15, "VAD release time:  %2dms", _vadInfo.releaseTime);

    show_level_meter(y, &_vadInfo);
    
    return 6;
}
#undef   OS_TicksToMilliSeconds
#define  OS_TicksToMilliSeconds( tick ) ( ((tick) * 64) / (OS_SYSTEM_CLOCK/1000) )
/////////////////////////////////////////////////////////////////////////////////////
//
//  オーディオ情報表示メニュー 
//

const char * audio_format_str[VCT_CODEC_END] = {
    "8Bit Raw",
    "G.711 u-Law",
    "ADPCM 2bit",
    "ADPCM 3bit",
    "ADPCM 4bit",
};

const char* AudioInfo::menuElement(int index, int cursor, u8* outColor)
{
#pragma unused(index, cursor, outColor)
    return NULL;
}

MenuResult AudioInfo::keyDown(int index, int key)
{
#pragma unused(index)
    if (!(key & (PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_X | PAD_BUTTON_Y))) {
        return key;
    }

    if (key & PAD_KEY_LEFT) {
        if (--_channel < 0) {
            _channel = VCT_MAX_AUDIO_STREAM - 1;
        }
    }
    if (key & PAD_KEY_RIGHT) {
        if (++_channel > VCT_MAX_AUDIO_STREAM - 1) {
            _channel = 0;
        }
    }

    int codec = VCT_GetCodec();
    
    if (key & PAD_BUTTON_Y) {
        if (--codec < 0) {
            codec = VCT_CODEC_END - 1;
        }
        VCT_SetCodec((VCTCodec)codec);
    }
    else if (key & PAD_BUTTON_X) {
        if (++codec >= VCT_CODEC_END) {
            codec = VCT_CODEC_8BIT_RAW;
        }
        VCT_SetCodec((VCTCodec)codec);
    }

    return key & ~(PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_X | PAD_BUTTON_Y);
}
int AudioInfo::showExInfo(int y, int cursor)
{
#pragma unused(cursor)

    VCTAudioInfo info;
    VCT_GetAudioInfo((u32)_channel, &info);

    PutMainScreen(1, (s16)y++, 15, "Stream channel:   %7d", _channel);
    PutMainScreen(1, (s16)y++, 15, "Sequence:         %7d", info.sequence);
    PutMainScreen(1, (s16)y++, 15, "Internal latency: %7dms", OS_TicksToMilliSeconds(info.internalLatency));
    PutMainScreen(1, (s16)y++, 15, "Buffer latency:   %7dms", OS_TicksToMilliSeconds(info.bufferLatency));
    PutMainScreen(1, (s16)y++, 15, "Buffer count:     %7d", info.bufferCount);
    PutMainScreen(1, (s16)y++, 15, "Clock skew:       %7dms", OS_TicksToMilliSeconds(info.clockSkew));
    PutMainScreen(1, (s16)y++, 15, "Jitter:           %7dms", OS_TicksToMilliSeconds(info.jitter));
    PutMainScreen(1, (s16)y++, 15, "Drop count:       %7d", info.dropCount);
    PutMainScreen(1, (s16)y++, 15, "Recover count:    %7d", info.recoverCount);
    PutMainScreen(1, (s16)y++, 15, "Jam count:        %7d", info.jamCount);
    PutMainScreen(1, (s16)y++, 15, "Recv audio format:  %s", audio_format_str[info.codec]);

    ++y;

    VCTVADInfo vad;
    VCT_GetVADInfo(&vad);

    VCTCodec codec = VCT_GetCodec();

    PutMainScreen(0, (s16)y++, 15, "Send audio format:   %s", audio_format_str[codec]);

    u8 color = FONT_COLOR_GRAY;
    if (vad.activity) {
        color = FONT_COLOR_GREEN;
    }

    show_level_meter(y, &vad);
    return 15;
}
