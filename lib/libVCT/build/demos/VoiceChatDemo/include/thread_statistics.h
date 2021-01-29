#ifndef __THREAD_STATISTICS_H__
#define __THREAD_STATISTICS_H__

#ifdef __cplusplus
extern "C" {
#endif

void InitThreadStatistics();
void GetThreadTicks(u64 result[3]);
void DumpThreadStatistics();
    
#ifdef __cplusplus
}
#endif

#endif // __THREAD_STATISTICS_H__
