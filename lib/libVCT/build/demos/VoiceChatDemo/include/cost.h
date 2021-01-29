//
// 関数コスト計測用
//

#ifndef __COST_H__
#define __COST_H__

#ifdef __cplusplus
extern "C" {
#endif
    void __vct_cost_audio(void *in, void *out[], u32 channels, u32 length);
    void __vct_cost_count();
#ifdef __cplusplus
}
#endif

#endif __COST_H__
