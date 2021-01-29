/*---------------------------------------------------------------------------*

  ��ʂւ̃f�o�b�O�p�\��

  - BG�X�N���[���ɕ������\�����܂�
  - 16�F�A256x256�̃X�N���[����p�ł�
  
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include "dbs_print.h"
#include "dbs_print_data.h"




static BOOL          s_initialized;                                 // �������ς݂��ǂ����̃t���O
static tDbsInitParam s_initparam;                                   // �������p�����[�^�ۑ��p
static GXScrFmtText  s_screen_data[24][32];                         // ������p�X�N���[���f�[�^
static void*         s_screen_loadaddr;                             // �X�N���[�����[�h��̃A�h���X
static int           s_charname_top;                                // �t�H���g�f�[�^�̐擪�̃L�����N�^�l�[��
static int           s_pltt_base;                                   // �p���b�g�ԍ��̐擪
static int           s_pltt_num;                                    // ������p�Ɏg���p���b�g�̐�
static u8            s_converted_font[ sizeof( s_font_chardata ) ]; // �s�N�Z���F��ύX��̃t�H���g
static char          s_format_buffer[ 256 ];                        // ������t�H�[�}�b�g�p�o�b�t�@�B
static GXRgb         s_fgcolor[] = { GX_RGB(  0,  0,  0 ),          // black
                                    GX_RGB( 31,  0,  0 ),           // red
                                    GX_RGB(  0, 31,  0 ),           // green
                                    GX_RGB( 31, 25,  0 ),           // yellow
                                    GX_RGB(  0,  0, 31 ),           // blue
                                    GX_RGB( 31,  0, 31 ),           // magenta
                                    GX_RGB(  0, 31, 31 ),           // cyan
                                    GX_RGB( 31, 31, 31 )            // white
};
static GXRgb         s_bgcolor = GX_RGB( 31, 31, 31 );



// static functions
static int  GetEscapeCode( const char* p, int * advance );
static void DummyVBlankIntr(void);
static void PutString             ( int x, int y, const char* str, u32 strbufsize );
static void ConvertFontPixelcolor ( const u8* font_in, u8* font_out, int fontsize, int fg_color, int bg_color );





/*---------------------------------------------------------------------------*
  
  ������`������������܂��B���x�Ăяo���Ă����܂��܂���B�Ăяo������
  �ɃO���t�B�b�N�X�G���W����ݒ肵�܂��B

  param             : ������p�Ɏg��BG�ʂ͂ǂ����Bdbs_print.h�Œ�`���ꂽenum�l�B
  fontchar_location : �t�H���g�p�L�����f�[�^��z�u����ꏊ�BBG�ʂ̃L�����N�^�x�[�X����̃o�C�g��
  fg_color          : ������̕����F�Ɋ��蓖�Ă�s�N�Z���J���[
  bg_color          : ������̔w�i�F�Ɋ��蓖�Ă�s�N�Z���J���[
  pltt_base         : ������p�Ɏg���p���b�g�̎n�܂�
  pltt_num          : �����F�����F�܂Ŏg���������B�g�����������p���b�g���L����B

  �������ݒ�̗� ���̂P

  fg_color  = 8
  bg_color  = 15
  pltt_base = 12
  pltt_num  = 4  �̏ꍇ��

  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������

  ��}�́��̕�����������p�Ƃ��Ďg���܂��B

  
  �������ݒ�̗� ���̂Q
  
  fg_color  = 15
  bg_color  = 0
  pltt_base = 0
  pltt_num  = 2  �̏ꍇ��

  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������
  ��������������������������������

  ��}�́��̕�����������p�Ƃ��Ďg���܂��B���̏ꍇ�͔w�i�F�������ɂȂ�܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_Init       ( tDbsInitParam param, int fontchar_location, int fg_color, int bg_color, int pltt_base, int pltt_num )
{
    GXCharBGText16* bgchar;


    // �ݒ�l��ۑ�����
    s_initparam    = param;
	s_charname_top = fontchar_location / (int)sizeof( GXCharFmt16 );
    s_pltt_base    = pltt_base;
    s_pltt_num     = pltt_num;

    switch( param )
    {
    case eDBS_INIT_MAIN_BG0:
        bgchar = (GXCharBGText16*)G2_GetBG0CharPtr();
        s_screen_loadaddr = G2_GetBG0ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG1:
        bgchar = (GXCharBGText16*)G2_GetBG1CharPtr();
        s_screen_loadaddr = G2_GetBG1ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG2:
        bgchar = (GXCharBGText16*)G2_GetBG2CharPtr();
        s_screen_loadaddr = G2_GetBG2ScrPtr();
        break;
    case eDBS_INIT_MAIN_BG3:
        bgchar = (GXCharBGText16*)G2_GetBG3CharPtr();
        s_screen_loadaddr = G2_GetBG3ScrPtr();
        break;
    case eDBS_INIT_SUB_BG0:
        bgchar = (GXCharBGText16*)G2S_GetBG0CharPtr();
        s_screen_loadaddr = G2S_GetBG0ScrPtr();
        break;
    case eDBS_INIT_SUB_BG1:
        bgchar = (GXCharBGText16*)G2S_GetBG1CharPtr();
        s_screen_loadaddr = G2S_GetBG1ScrPtr();
        break;
    case eDBS_INIT_SUB_BG2:
        bgchar = (GXCharBGText16*)G2S_GetBG2CharPtr();
        s_screen_loadaddr = G2S_GetBG2ScrPtr();
        break;
    case eDBS_INIT_SUB_BG3:
        bgchar = (GXCharBGText16*)G2S_GetBG3CharPtr();
        s_screen_loadaddr = G2S_GetBG3ScrPtr();
        break;
    default:
        OS_Panic( "dbs_init : invalid parameter\n" );
        break;
    }


    // �t�H���g�����H����
    if ( ! s_initialized ) // ���H�͍ŏ��̈�񂾂�
    {
        ConvertFontPixelcolor( s_font_chardata, s_converted_font, sizeof(s_font_chardata), fg_color, bg_color );
    }

    // �t�H���g�����[�h����
    MI_DmaCopy32( GX_GetDefaultDMA(), s_converted_font, & bgchar->ch[ s_charname_top ], sizeof( s_font_chardata ) );

    // �p���b�g�����[�h����
    if( eDBS_INIT_MAIN_BG0 <= param && param <= eDBS_INIT_MAIN_BG3 ) // ����
    {
        int i;

        for( i = 0 ; i < s_pltt_num; i++ )
        {
            GX_LoadBGPltt ( & s_fgcolor[ i ], sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * fg_color,  sizeof(GXRgb) );

            if( bg_color != 0 ) // �w�i�F�͓����łȂ��ꍇ�̂݃��[�h����
            {
                GX_LoadBGPltt ( & s_bgcolor,  sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * bg_color,  sizeof(GXRgb) );
            }
        }
        
    }
    else // �����
    {
        int i;

        for( i = 0 ; i < s_pltt_num; i++ )
        {
            GXS_LoadBGPltt ( & s_fgcolor[ i ], sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * fg_color,  sizeof(GXRgb) );

            if( bg_color != 0 ) // �w�i�F�͓����łȂ��ꍇ�̂݃��[�h����
            {
                GXS_LoadBGPltt ( & s_bgcolor,      sizeof(GXBGPltt16) * ( s_pltt_base + i ) + sizeof(GXRgb) * bg_color,  sizeof(GXRgb) );
            }
        }
        
    }

    // �X�N���[���o�b�t�@���N���A����
    MI_CpuClear8( s_screen_data, sizeof( s_screen_data ) );

    s_initialized = TRUE;
    
}




/*---------------------------------------------------------------------------*
  
  �O���t�B�b�N�G���W���ɃX�N���[���f�[�^�̃��[�h���s���܂��B�ʏ�̃��[
  �h�֐����l�AVBLANK ���Ԓ��ɌĂяo���K�v������܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_Load       ( void )
{
    if ( ! s_initialized ) return; // ���������Ȃ�Ȃɂ����Ȃ�

    DC_StoreRange( s_screen_data, sizeof( s_screen_data ) );
    MI_DmaCopy32( GX_GetDefaultDMA(), s_screen_data, s_screen_loadaddr, sizeof( s_screen_data ) );

    // ���[�h��̓N���A����
    MI_CpuClear8( s_screen_data, sizeof( s_screen_data ) );

}




/*---------------------------------------------------------------------------*
  
  �X�N���[�����W ( x, y ) �̏ꏊ����_�Ƃ��ĕ������`�悵�܂��B�P�t���[
  �������̕\���ɂȂ�܂��̂ŁA�ʏ�͖����[�v�ĂԕK�v������܂��B
  
 *---------------------------------------------------------------------------*/
void dbs_Print      ( int x, int y, const char* fmt, ... )
{
    va_list va;
    va_start(va, fmt);
    (void)vsnprintf( s_format_buffer, sizeof(s_format_buffer), fmt, va );
    va_end(va);

    PutString( x, y, s_format_buffer, sizeof(s_format_buffer) );
}


void dbs_VPrint           ( int x, int y, const char *fmt, va_list vlist )
{
    (void)vsnprintf( s_format_buffer, sizeof(s_format_buffer), fmt, vlist );

    PutString( x, y, s_format_buffer, sizeof(s_format_buffer) );
    
}



/*---------------------------------------------------------------------------*
  
  �R���e�L�X�g����\�����܂��B��O�������̃��[�U�R�[���o�b�N�Ƃ��Ďg
  �����Ƃ�z�肵���֐��ł��B

 *---------------------------------------------------------------------------*/
void dbs_DisplayException ( u32 context_addr, void* /* arg */ )
{
    static int bg_offset = 8 * 8; // ��O���͉�ʊO�ɕ`���āA���炵�ĕ\������B
    OSContext* context = (OSContext*) context_addr;
    int i;

    if ( ! s_initialized ) return; // ���������Ȃ�Ȃɂ����Ȃ�

    // ��O�����v�����g����
    for( i=0; i<13; i++ )
    {
        dbs_Print( ( i % 2) * 16, i / 2, "R%02d=%08X ", i, context->r[i] );
    }

    dbs_Print( ( i % 2 ) * 16, i / 2, "SP =%08X ", context->sp );
    i++;
    
    dbs_Print( ( i % 2 ) * 16, i / 2, "LR =%08X ",  context->lr );
    i++;
    
    dbs_Print( ( i % 2 ) * 16, i / 2, "PC =%08X " , context->pc_plus4 );
    i++;


    DC_StoreRange( s_screen_data, sizeof( s_screen_data ) );

    // �Ȃɂ����Ȃ�VBLANK�֐����Z�b�g���āA���荞�݋���Ԃɂ���
    OS_SetIrqFunction( OS_IE_V_BLANK, DummyVBlankIntr );
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);

    // VBLANK���Ԓ��Ƀ��[�h����
    OS_WaitIrq( 1, OS_IE_V_BLANK );
    MI_DmaCopy32( GX_GetDefaultDMA(), s_screen_data,
                  ((GXScrText32x32*)s_screen_loadaddr)->scr[24], // ��ʊO�̗̈�Ƀ��[�h����
                  sizeof(GXScrFmtText) * 32 * 8 );

    // �R���g���[���ŗ�O���\���ʒu�𓮂������߂̃��C�����[�v
    while ( 1 )
    {
        u16 pad;

        pad = PAD_Read();

        if ( pad & PAD_KEY_UP )
        {
            bg_offset ++;
        }
        else if ( pad & PAD_KEY_DOWN )
        {
            bg_offset --;
        }

        if( bg_offset > 8 * 8 )
        {
            bg_offset = 8 * 8;
        }
        else if ( bg_offset < 0 )
        {
            bg_offset = 0;
        }
        
        switch( s_initparam )
        {
        case eDBS_INIT_MAIN_BG0:
            G2_SetBG0Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG1:
            G2_SetBG1Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG2:
            G2_SetBG2Offset( 0, bg_offset );
            break;
        case eDBS_INIT_MAIN_BG3:
            G2_SetBG3Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG0:
            G2S_SetBG0Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG1:
            G2S_SetBG1Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG2:
            G2S_SetBG2Offset( 0, bg_offset );
            break;
        case eDBS_INIT_SUB_BG3:
            G2S_SetBG3Offset( 0, bg_offset );
            break;
        }
        
        OS_WaitIrq( 1, OS_IE_V_BLANK );
    }

}





/*---------------------------------------------------------------------------*
  
  �t�H���g�̃L�����f�[�^�����������܂��B���ƂɂȂ�L�����f�[�^�́A4bit�P�s�N�Z����
  ���� ���O�A�����F���P�A�w�i�F���Q�A�Ƃ����\���ł��B
  
 *---------------------------------------------------------------------------*/
static void ConvertFontPixelcolor( const u8* font_in, u8* font_out, int fontsize, int fg_color, int bg_color )
{
    int i;
    int table[3]; //

    SDK_NULL_ASSERT( font_in );
    SDK_NULL_ASSERT( font_out );
    SDK_ASSERT( fontsize > 0 );
    SDK_ASSERT( fg_color < 16 );
    SDK_ASSERT( bg_color < 16 );

    table[0] = 0;
    table[1] = fg_color;
    table[2] = bg_color;
    
    for( i = 0; i < fontsize; i++ )
    {
        int high, low;

        high = table[ ( font_in[ i ] & 0xF0 ) >> 4 ];
        low  = table[ ( font_in[ i ] &  0xF ) >> 0 ];
        
        font_out[ i ] = (u8) ( ( high << 4 ) | low );

    }

}





/*---------------------------------------------------------------------------*
  
  �X�N���[���ɕ�������Z�b�g���܂�
  
 *---------------------------------------------------------------------------*/
static void PutString( int x, int y, const char* str, u32 strbufsize )
{
    int i;
    int start_x = x;
    int color_index = 0; // s_fgcolor�̃C���f�N�X

    for( i = 0 ; str[i] != '\0' && i < strbufsize; i++ )
    {
        if ( x < 0 )             continue; // �͈͊O�Ȃ�Ȃɂ����Ȃ�
        if ( y < 0 || y >= 24 )  continue; // �͈͊O�Ȃ�Ȃɂ����Ȃ�

        if ( str[i] == '\e' ) // �G�X�P�[�v��������������F��ς���
        {
            int code, advance;
            
            code = GetEscapeCode( & str[i], & advance );

            if ( code >= 0 )
            {
                color_index = code - DBS_BLACK;
            }

            i += advance - 1; // for����i++�����̂ŁA1�����Ă���
            
            continue;
            
        }

        // ���s�����Ȃ���s����
        if ( str[i] == '\n' )
        {
            x = start_x;
            y ++;
            continue;
        }
        

        s_screen_data[ y ][ x ] = GX_SCRFMT_TEXT( s_pltt_base + color_index % s_pltt_num, 0, 0, s_charname_top + str[i] - ' ' );

        x ++;
        // �E�[�ɂ����玩���I�ɉ��s����
        if ( x >= 32 )
        {
            x = start_x;
            y ++;
        }

    }

}




/*---------------------------------------------------------------------------*
  
  ��O�\�����ɐݒ肷��A�Ȃɂ����Ȃ�VBLANK�֐��B���ꂪ�Ȃ��ƁAVBLANK��
  ����dbs_Load���Ă�ł���Ƃ��ɍ���B
  
 *---------------------------------------------------------------------------*/
static void DummyVBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}




/*---------------------------------------------------------------------------*

  �����F�ύX�R�[�h���擾

  p : �G�X�P�[�v�V�[�P���X�擪
  advance : ���o�C�g�g��������Ԃ����߂̕ϐ�

  returns:
  -1 ���s�i�m��Ȃ��G�X�P�[�v�V�[�P���X�������j
  30 "\e[30m" �������A�܂荕�ɃZ�b�g
  �F
  �F
  37 "\e[37m" �������A�܂蔒�ɃZ�b�g
  
 *---------------------------------------------------------------------------*/
static int  GetEscapeCode( const char* p, int * advance )
{
    int code = -1; // �G���[�l������Ă���
    
    SDK_ASSERT( *p == '\e' ); // �G�X�P�[�v�V�[�P���X����Ȃ��Ƃ��ŌĂ΂��͂��͂Ȃ�

    // �G���[�`�F�b�N
    if ( p[1] != '[' )
    {
        *advance = 1;
        return code;
    }

    // �G���[�`�F�b�N���̂Q
    if ( p[2] != '3' )
    {
        *advance = 2;
        return code;
    }

    // �G���[�`�F�b�N���̂R
    if ( p[3] < '0' || p[3] > '7' )
    {
        *advance = 3;
        return code;
    }

    // �G���[�`�F�b�N����4
    if ( p[4] != 'm' )
    {
        *advance = 4;
        return code;
    }

    code = DBS_BLACK + p[3] - '0';
    *advance = 5;

    return code;
        
}
