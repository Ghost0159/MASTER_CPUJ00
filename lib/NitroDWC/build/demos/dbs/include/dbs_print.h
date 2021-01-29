#ifndef DBS_PRINT_H_
#define DBS_PRINT_H_



#ifdef __cplusplus
extern "C" {
#endif

   
// ANSI/VT100 Terminal Controlの、文字色変更コード
#define DBS_BLACK   30
#define DBS_RED     31
#define DBS_GREEN   32
#define DBS_YELLOW  33
#define DBS_BLUE    34
#define DBS_MAGENTA 35
#define DBS_CYAN    36
#define DBS_WHITE   37


// ANSI/VT100 Terminal Controlのエスケープシーケンス
#define DBS_BLACK_STR   "\e[30m"
#define DBS_RED_STR     "\e[31m"
#define DBS_GREEN_STR   "\e[32m"
#define DBS_YELLOW_STR  "\e[33m"
#define DBS_BLUE_STR    "\e[34m"
#define DBS_MAGENTA_STR "\e[35m"
#define DBS_CYAN_STR    "\e[36m"
#define DBS_WHITE_STR   "\e[37m"

                              
typedef enum
{
    eDBS_INIT_MAIN_BG0,
    eDBS_INIT_MAIN_BG1,
    eDBS_INIT_MAIN_BG2,
    eDBS_INIT_MAIN_BG3,
    
    eDBS_INIT_SUB_BG0,
    eDBS_INIT_SUB_BG1,
    eDBS_INIT_SUB_BG2,
    eDBS_INIT_SUB_BG3,

    eDBS_INIT_PARAMNUM
} tDbsInitParam;

    

void dbs_Init             ( tDbsInitParam param, int fontchar_location, int pixcolor1, int pixcolor2, int pltt_base, int pltt_num );
void dbs_Load             ( void );    
void dbs_Print            ( int x, int y, const char* fmt, ... );
void dbs_VPrint           ( int x, int y, const char *fmt, va_list vlist );
    
void dbs_DisplayException ( u32 context_addr, void* arg );
    


#ifdef __cplusplus
}/* extern "C" */
#endif



#endif // DBS_PRINT_H_
