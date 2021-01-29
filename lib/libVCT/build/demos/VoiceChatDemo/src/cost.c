//
// 関数コスト計測用
//
#pragma profile on

#include "vct.h"
#include "cost.h"

void __vct_cost_count()
{
    static int dummy = 0;
    ++dummy;
}

void __vct_cost_audio(void *in, void *out[], u32 channels, u32 length)
{
    u32 ch;
    VCT_SendAudio(in, length);
    for (ch = 0; ch < channels; ++ch) {
        VCT_ReceiveAudio(out[ch], length, ch, NULL);
    }
}
