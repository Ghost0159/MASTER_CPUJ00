#ifndef DBS_CONSOLE_H_
#define DBS_CONSOLE_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tDbsConsole tDbsConsole;
struct tDbsConsole
{
    u32 linenum;              // �R���\�[�����ێ��ł��镶���̑��s���B
    u32 currentline;          // �����s�ڂ܂ŕ�����ۑ��������B���X�Ƒ��������܂��B
    u32 nowcolumn;            // �s�̂ǂ��ɕ����������Ă��邩�B
    u32 displayline;          // �\���ӏ��𐧌䂷�邽�߂̃J�E���^�B�R���\�[����[�����s�ڂ�\�����Ă��邩�B
    u8* linebuf;              // �����ۑ��o�b�t�@�B
    tDbsConsole* next;        // �����R���\�[�����Ǘ����邽�߂̃��X�g�p�����N
    u8 x;                     // �R���\�[��X���W�B�P�ʂ̓L�����N�^�B
    u8 y;                     // �R���\�[��Y���W�B�P�ʂ̓L�����N�^�B
    u8 width;                 // �R���\�[�����B�P�ʂ̓L�����N�^�B
    u8 height;                // �R���\�[�������B�P�ʂ̓L�����N�^�B
    u8 visible;               // �����Ă邩�ǂ����BCreate�����TRUE�B
    u8 linenumber_width;      // �s�ԍ��ɉ����g�����BCreate�����0�D
    u8 stop_bufferfull;       // �o�b�t�@�����^���ɂȂ����Ƃ��~�܂邩�ǂ����BCreate�����FALSE�B
    u8 default_color;         // �f�t�H���g�F
    
};


// �쐬�E�폜
void dbs_CreateConsole( tDbsConsole* console, void* linebuf, u32 linebuf_size, int x, int y, int width, int height );
void dbs_DestroyConsole( tDbsConsole* console );
void dbs_CleanupConsole( void );

// �v�����g
void dbs_Puts( const char* fmt, ... );
void dbs_PutsColor( int color, const char* fmt, ... );
void dbs_CPrintf ( tDbsConsole* console, const char* fmt, ... );
void dbs_CVPrintf( tDbsConsole* console, const char* fmt, va_list vlist );
void dbs_PrintColorConsole( tDbsConsole* console, int color, const char* fmt, ... );
void dbs_ClearConsole( tDbsConsole* console );
void dbs_DrawConsole( void );
int  dbs_VSNPrintf( char *dst, size_t len, const char *fmt, va_list vlist );
int  dbs_VNPrintf( char *dst, size_t len, const char *fmt, va_list vlist );

// �X�N���[��
void dbs_ScrollConsole( tDbsConsole* console, int lines );
void dbs_ScrollConsoleToFirstLine( tDbsConsole* console );
void dbs_ScrollConsoleToLastLine( tDbsConsole* console );

void dbs_ScrollConsoleToPrevPage( tDbsConsole* console );
void dbs_ScrollConsoleToNextPage( tDbsConsole* console );

// �ݒ�ύX�E�擾
void dbs_SetDefaultConsole( tDbsConsole* console );
tDbsConsole* dbs_GetDefaultConsole( void );

void dbs_SetConsoleVisibility( tDbsConsole* console, BOOL show );
BOOL dbs_GetConsoleVisibility( tDbsConsole* console );

void dbs_SetConsoleStopOnBufferFull( tDbsConsole* console,  BOOL stop );
BOOL dbs_GetConsoleStopOnBufferFull( const tDbsConsole* console );

void dbs_SetConsoleLineNumberWidth( tDbsConsole* console, int width );


#ifdef __cplusplus
}/* extern "C" */
#endif
    

#endif // DBS_CONSOLE_H_
