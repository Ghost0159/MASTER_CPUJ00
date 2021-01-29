#ifndef INIT_H
#define INIT_H

void Heap_Init(void);
void Heap_Debug(void);
void Heap_Dump(void);
void Heap_SetDebug( BOOL flag );
void Heap_Print( void );
void Heap_Update( void );
void Heap_Clear( void );

void DTDB_SetPrintOverride( BOOL flag );
DWCAllocEx DTDB_GetAllocFunc( void );
DWCFreeEx  DTDB_GetFreeFunc( void );

#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif
extern void OS_TVPrintf(const char *fmt, va_list vlist );
extern void OS_TPrintf(const char *fmt, ...);

#endif

