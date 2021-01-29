#include <nitro.h>
#include "dbs_console.h"
#include "dbs_print.h"


typedef struct tLine
{
    u8 color;
    char str[1];
} tLine;


static tDbsConsole* sConsoleList; // �R���\�[���̃����N���X�g
static tDbsConsole* sDefaultConsole; // ������NULL���w�肵���Ƃ��Ɏg����A�f�t�H���g�̃R���\�[��



// static functions
static void CorrectOutOfRangeDisplay( tDbsConsole* con );
static const char* GetString( const tDbsConsole* con, u32 line );
static int GetColor( const tDbsConsole* con, u32 line );
static BOOL IsInsideBuffer( const tDbsConsole* con, u32 line );
static void Print( tDbsConsole* console, int color, const char* fmt, va_list vlist );
static tLine* GetLine( tDbsConsole* con, u32 line );
static void Draw( tDbsConsole* console );
static tDbsConsole* GetTarget( tDbsConsole* arg );
static void AppendConsole( tDbsConsole* appending );





/*---------------------------------------------------------------------------*

 �R���\�[�����쐬���܂�
  
 *---------------------------------------------------------------------------*/
void dbs_CreateConsole( tDbsConsole* console, void* linebuf, u32 linebuf_size, int x, int y, int width, int height )
{
    SDK_NULL_ASSERT( console );

    console->linenum         = linebuf_size / ( width + 1 ) ; // �F�ۑ��p�ɂP�o�C�g�g���̂ŁAwidth + 1�Ŋ���
    console->currentline     = 0;
    console->nowcolumn       = 0;
    console->displayline     = 0;
    MI_CpuClear8( linebuf, linebuf_size );
    console->linebuf         = (u8*)linebuf;
    AppendConsole( console );
    console->x               = (u8) x;
    console->y               = (u8) y;
    console->width           = (u8) width;
    console->height          = (u8) height;
    console->visible         = (u8) TRUE;
    console->linenumber_width = (u8) 0;
    console->stop_bufferfull = (u8) FALSE;
    console->default_color   = DBS_BLACK;

    // ��ڂ̏ꍇ�͎����Ńf�t�H���g��
    if ( sDefaultConsole == NULL )
    {
        sDefaultConsole = console;
    }
    
}




/*---------------------------------------------------------------------------*
  
  �R���\�[�����폜���܂�
  
 *---------------------------------------------------------------------------*/
void dbs_DestroyConsole( tDbsConsole* removing )
{
    tDbsConsole* con; // tDbsConsole��next�t�B�[���h�̃A�h���X���ǂ�ǂ񂢂�Ă����ϐ�

    if ( removing == NULL ) return;

    for( con = sConsoleList; ; con = con->next )
    {
        if ( !con ) break;

        if ( con == removing )
        {
            // �ŏ��̗v�f��removing.
            sConsoleList = removing->next;

            break;
        }
        else if ( con->next == removing )
        {
            // ����ȍ~�B
            con->next = removing->next;

            break;
        }
    }

    if ( removing == sDefaultConsole )
    {
        sDefaultConsole = sConsoleList;
    }
}





/*---------------------------------------------------------------------------*
  
  �Ǘ����Ă���R���\�[�������ׂĔj�����܂�
  
 *---------------------------------------------------------------------------*/
void dbs_CleanupConsole( void )
{
    sConsoleList    = NULL;
    sDefaultConsole = NULL;
}



/*---------------------------------------------------------------------------*
  
  �R���\�[����`�悵�܂��B�O���t�B�b�N�G���W���ɂ͈�؂���炸�A
  dbs_print�֐����Ăяo�������ł��B
  
 *---------------------------------------------------------------------------*/
void dbs_DrawConsole( void )
{
    tDbsConsole* con;

    if ( ! sConsoleList ) return ; // �R���\�[�����ЂƂ�����ĂȂ�������Ȃɂ����Ȃ�

    for ( con = sConsoleList; con != NULL; con = con->next )
    {
        if ( con->visible )
        {
            Draw( con );
        }
    }

}




/*---------------------------------------------------------------------------*
  
  �f�t�H���g�̃R���\�[���ɑ΂��ĕ�����\�����܂�
  
 *---------------------------------------------------------------------------*/
void dbs_Puts( const char* fmt, ... )
{
    va_list vlist;
    
    if ( ! sDefaultConsole ) return;

    va_start( vlist, fmt );
    Print( sDefaultConsole, sDefaultConsole->default_color, fmt, vlist );
    va_end( vlist );
    
}




/*---------------------------------------------------------------------------*
  
  �f�t�H���g�̃R���\�[���ɑ΂��ĐF���ŕ�����\�����܂�
  
 *---------------------------------------------------------------------------*/
void dbs_PutsColor( int color, const char* fmt, ... )
{
    va_list vlist;
    
    if ( ! sDefaultConsole ) return;

    va_start( vlist, fmt );
    Print( sDefaultConsole, color, fmt, vlist );
    va_end( vlist );

}




/*---------------------------------------------------------------------------*
  
  �R���\�[�����w�肵�ĕ�����\�����܂��B�R���\�[����NULL���w�肷��ƃf�t�H���g�ɕ\�����܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_CPrintf( tDbsConsole* console, const char* fmt, ... )
{
    va_list vlist;

    va_start( vlist, fmt );
    dbs_CVPrintf( console, fmt, vlist );
    va_end( vlist );
}

void dbs_CVPrintf( tDbsConsole* console, const char* fmt, va_list vlist )
{
    tDbsConsole* target;
    
    if ( ! sConsoleList ) return;

    target = GetTarget( console );

    Print( target, target->default_color, fmt, vlist );
}




/*---------------------------------------------------------------------------*
  
  �R���\�[���ƐF���w�肵�ĕ�����\�����܂��B�R���\�[����NULL���w�肷��ƃf�t�H���g�ɕ\�����܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_PrintColorConsole( tDbsConsole* console, int color, const char* fmt, ... )
{
    tDbsConsole* target;
    va_list vlist;
    
    if ( ! sConsoleList ) return;

    target = GetTarget( console );


    va_start( vlist, fmt );
    Print( target, color, fmt, vlist );
    va_end( vlist );
    
}





/*---------------------------------------------------------------------------*
  
  �R���\�[���̓��e���A�o�b�t�@���܂߂Ă��ׂď������܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_ClearConsole( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );
    
    con->currentline     = 0;
    con->displayline     = 0;
    MI_CpuClear8( con->linebuf, sizeof(u8) * ( con->width + 1 ) * ( con->linenum ) );
    
}





/*---------------------------------------------------------------------------*

  ������͈͂��ړ����܂��Blines�𐳂̒l�ɂ���Ɛi�݁Alines�𕉂̒l�ɂ���Ɩ߂�܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsole( tDbsConsole* console, int lines )
{
    tDbsConsole* con = GetTarget( console );

    // displayline���[�������ɂ��Ă��܂������ȏꍇ�A�[���łƂ߂�
    if ( lines < 0 && con->displayline < -lines )
    {
        con->displayline = 0;
        return ;
    }

    con->displayline += lines;

    CorrectOutOfRangeDisplay( con );

}




/*---------------------------------------------------------------------------*

 �o�b�t�@�Ɏc���Ă������̈�ԌÂ��s��������悤�ɂ��܂�
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToFirstLine( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    con->displayline = con->currentline - con->linenum;

}



/*---------------------------------------------------------------------------*
  
  ��ԍŌ�̈�s������������悤�Ȉʒu�ɃX�N���[�����܂�
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToLastLine( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    con->displayline = con->currentline - 1;

}




/*---------------------------------------------------------------------------*
  
  1�y�[�W�O�ɂ��ǂ�܂�
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToPrevPage( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );
    
    dbs_ScrollConsole( con, - con->height );
}




/*---------------------------------------------------------------------------*
  
  1�y�[�W��ɐi�݂܂�
  
 *---------------------------------------------------------------------------*/
void dbs_ScrollConsoleToNextPage( tDbsConsole* console )
{
    tDbsConsole* con = GetTarget( console );

    dbs_ScrollConsole( con, con->height );
}




/*---------------------------------------------------------------------------*
  
  �f�t�H���g�R���\�[����ύX���܂�
  
 *---------------------------------------------------------------------------*/
void dbs_SetDefaultConsole( tDbsConsole* console )
{
    SDK_ASSERT( console );

    sDefaultConsole = console;

}



/*---------------------------------------------------------------------------*
  
  �f�t�H���g�R���\�[�����擾���܂�
  
 *---------------------------------------------------------------------------*/
tDbsConsole* dbs_GetDefaultConsole( void )
{
    return sDefaultConsole;
}






/*---------------------------------------------------------------------------*
  
  �R���\�[���̕\����؂�ւ��܂�
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleVisibility( tDbsConsole* console, BOOL show )
{
    GetTarget( console )->visible = (u8)show;
}



/*---------------------------------------------------------------------------*
  
  �R���\�[���̉���Ԃ��擾���܂�
  
 *---------------------------------------------------------------------------*/
BOOL dbs_GetConsoleVisibility( tDbsConsole* console )
{
    return GetTarget( console )->visible;
}



/*---------------------------------------------------------------------------*
  
  �R���\�[���̍s�ԍ��\���ɉ����g������ݒ肵�܂��B�s�ԍ��́APrint����
  ������擪�ɑ}������܂��̂ŁA���I�ɂ�����������肷�邱�Ƃ͂ł���
  ����B
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleLineNumberWidth( tDbsConsole* console, int width )
{
    SDK_ASSERT( width >= 0 );

    GetTarget( console )->linenumber_width = (u8)width;
    
}




/*---------------------------------------------------------------------------*
  
  �o�b�t�@���܂񂽂�ɂȂ����Ƃ��̋������w�肵�܂�
  
 *---------------------------------------------------------------------------*/
void dbs_SetConsoleStopOnBufferFull( tDbsConsole* console,  BOOL stop )
{
    GetTarget( console )->stop_bufferfull = (u8)stop;
}




/*---------------------------------------------------------------------------*
  
  �o�b�t�@���܂񂽂�ɂȂ����Ƃ��̋������擾���܂�
  
 *---------------------------------------------------------------------------*/
BOOL dbs_GetConsoleStopOnBufferFull( const tDbsConsole* console )
{
    return GetTarget( (tDbsConsole*)console )->stop_bufferfull;
}



/*---------------------------------------------------------------------------*
  
  ���X�g�ɒǉ�
  
 *---------------------------------------------------------------------------*/
static void AppendConsole( tDbsConsole* appending )
{
    appending->next = sConsoleList;
    sConsoleList = appending;
}





static void Print( tDbsConsole* console, int color, const char* fmt, va_list vlist )
{
    int i;
    tLine* linebuf = GetLine( console, console->currentline );
    char str[128];
    
    (void) OS_VSNPrintf( str, sizeof(str), fmt, vlist );

    // �F�͍s�P�ʁB
    linebuf->color = (u8)color;

    i = 0;
    while ( str[i] )
    {
        if ( console->nowcolumn == 0 )
        {
            linebuf = GetLine( console, console->currentline );
            linebuf->color = (u8)color;

            // ���ꂩ�珑���s�̃o�b�t�@�������Ă����B
            MI_CpuFill8( linebuf->str, ' ', console->width );
        }

        // �����̕`��
        if ( str[i] == 0x0d || str[i] == 0x0a )
        {
            // ���s
            console->nowcolumn = 0;
            console->currentline ++;

            // DOS���s�Ή�
            if ( str[i] == 0x0d && str[i+1] == 0x0a )
            {
                i+=2;

                continue;
            }

            // UNIX/MAC���s
            i++;
            continue;
        }
        else if ( console->nowcolumn < console->width )
        {
            linebuf->str[console->nowcolumn] = str[i];

            console->nowcolumn++;

            if ( console->nowcolumn == console->width )
            {
                // ��荞�ݏ����B
                console->nowcolumn = 0;
                console->currentline ++;
            }
        }

        i++;
    }

    // �R���\�[���ŉ��[�����Ă���ꍇ�́A�����I�ɃX�N���[��������
    if( console->currentline > console->displayline + console->height )
    {
        // ���s��Ԃ��Ƃ�����̂ŁA�΍�B
        console->displayline = console->currentline - console->height + 1;
    }
}


static void Draw( tDbsConsole* console )
{
    int i;

    for ( i = 0; i < console->height; i++ )
    {
        char tmpstr[64]; // �R���\�[���̕�����o�b�t�@�ɂ̓k�������̂Ԃ񂪖����̂ł���ō��
        
        MI_CpuCopy8( GetString( console, console->displayline + i ), tmpstr, console->width );
        tmpstr[ console->width ] = '\0';
        
        dbs_Print( console->x, console->y + i, tmpstr );
    }
    
}


static tDbsConsole* GetTarget( tDbsConsole* arg )
{
    if ( arg )
    {
        return arg;
    }
    else
    {
        return sDefaultConsole;
    }
    
}


static tLine* GetLine( tDbsConsole* con, u32 line )
{
    int offset; // �s�̐擪�����o�C�g�ڂɂ����邩

	offset = (int)(( line % con->linenum ) * ( con->width + 1 ));

    return (tLine*)( & con->linebuf[ offset ] ) ;
    
}



/*---------------------------------------------------------------------------*
  
  �R���\�[���̃o�b�t�@���當����擾�B�o�b�t�@����͂ݏo���͈͂��w�肵
  ���ꍇ�A�󔒂�32�������񂾂��̂ւ̃|�C���^���Ԃ�B
  
 *---------------------------------------------------------------------------*/
static const char* GetString( const tDbsConsole* con, u32 line )
{
                                  // 12345678901234567890123456789012
    static const char* whitespace = "                                ";

    if ( IsInsideBuffer( con, line ) )
    {
        return GetLine( (tDbsConsole*)con, line )->str;
    }
    else
    {
        return whitespace;
    }
}


static int GetColor( const tDbsConsole* con, u32 line )
{
    if ( IsInsideBuffer( con, line ) )
    {
        return GetLine( (tDbsConsole*)con, line )->color;
    }
    else
    {
        return DBS_BLACK;
    }
}


static BOOL IsInsideBuffer( const tDbsConsole* con, u32 line )
{
    // �I�[�o�[�t���[���l������Au32�l���m�̔�r�B�u > �v�̂����ɂ����B
    // ����0xFFFFFFFF/2(=2147483647)��������ƁA�s���Ȍ��ʂ�Ԃ��܂��B
#define GREATER( x, y )  ( 0 < (u32)((x) - (y)) && (u32)((x) - (y)) <= 0xFFFFFFFF/2 )

    u32 bufstart;

    bufstart = con->currentline - con->linenum - 1;
    
    if ( GREATER( line, bufstart ) &&
         GREATER( con->currentline, line ) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




static void CorrectOutOfRangeDisplay( tDbsConsole* con )
{

    // �߂肷���̏ꍇ�A��ԌÂ��s��������悤�ɂ���
    if ( con->displayline < con->currentline - con->linenum )
    {
        dbs_ScrollConsoleToFirstLine( con );
    }

}
