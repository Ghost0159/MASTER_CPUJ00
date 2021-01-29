#include <nitro.h>
#include "thread_statistics.h"

struct thread_ticks {
    OSTick     start;
    OSTick     total;
    OSTick     prev;
};

struct thread_ticks sTicks[3];

static void thread_statistics(OSThread *from, OSThread *to)
{
    OSTick tick = OS_GetTick();

    u32 from_id, to_id;

    from_id = from->id;
    to_id   = to->id;

    if (from_id > 1) {
        from_id = 2;
    }
    if (to_id > 1) {
        to_id = 2;
    }
    
    sTicks[from_id].total += tick - sTicks[from_id].start;
    sTicks[to_id].start    = tick;
}

void InitThreadStatistics()
{
    u32 i;
    for (i = 0; i < 3; ++i) {
        sTicks[i].start = 0;
        sTicks[i].total = 0;
        sTicks[i].prev  = 0;
    }

    OS_SetSwitchThreadCallback(thread_statistics);
}

static const char * thr_name[3] = {
    "main:  ",
    "idle:  ",
    "other: ",
};

void GetThreadTicks(u64 result[3])
{
    int i;

    for (i = 0; i < 3; ++i) {
        result[i] = sTicks[i].total - sTicks[i].prev;
        sTicks[i].prev = sTicks[i].total;
    }
}

void DumpThreadStatistics()
{
    int i;
    static OSTick prev = 0;
    OSTick now = OS_GetTick();
    OSTick d = now - prev;

    if (d < OS_SecondsToTicks(1)) {
        return;
    }
        
    OS_TPrintf("thread:     total:      ticks:  %\n");
    for (i = 0; i < 3; ++i) {
        OSTick past = sTicks[i].total - sTicks[i].prev;
        u64 per = past * 1000 / d;
        OS_TPrintf("%s %10lld  %10lld  %2lld.%2lld%%\n",
                   thr_name[i],
                   sTicks[i].total,
                   past,
                   per / 10,
                   per % 10);

        sTicks[i].prev = sTicks[i].total;
    }

    prev = now;
}
    
