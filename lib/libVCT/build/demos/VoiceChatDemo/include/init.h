#ifndef __INIT_H
#define __INIT_H

#ifdef __cplusplus
extern "C" {
#endif

void Heap_Init(void);
void Heap_Debug(void);
void Heap_Dump(void);
void Alarm_Init(void);

extern u8 gAudioBuffer[];
    
#ifdef __cplusplus
}
#endif
    
#endif // INIT_H

