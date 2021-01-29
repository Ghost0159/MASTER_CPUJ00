/*---------------------------------------------------------------------------*

  dbs���C�u�����̂ӂ��̎g�����̃f�� �� �ȈՏ������C���^�t�F�C�X

  - �Ƃ肠������ʂɕ������o�����������Ȑl�����̋@�\�ł�
  - �T�u��ʂ�BG0���g���܂�
  - VRAM-H���g���܂�
  - VRAM-H��32�L���̂����A�擪�̂U�L���i0x0000�`0x17FF�j���g���܂��B
  - SDK�f����DEMOInitDisplaySubBG0Only�����̓����{dbs�̏����� �����܂��B
  - �T�u��ʂ̉������ɃR���\�[�����ЂƂ��܂�
  - ��O���n���h�����ݒ肳��܂�

 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include "dbs.h"
#include "dbs_demo.h"


static tDbsConsole sDemoConsole;
static char        sDemoLineBuf[ 32 * 100 ];



void dbs_DemoInit( void )
{
    GX_SetBankForSubBG( GX_VRAM_SUB_BG_32_H );
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);

    G2S_SetBG0Control( GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                       GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x00000,
                       GX_BG_EXTPLTT_01 );

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0x1000,
                      GX_BG_CHARBASE_0x00000,
                      GX_BG_EXTPLTT_01
                      );

    G2S_SetBG0Priority(0);

    G2S_BG0Mosaic(FALSE);

    dbs_Init( eDBS_INIT_SUB_BG0,
              0,  
              0xE,
              0xF,
              0x8,
              8 );

    // ���[�U��O�R�[���o�b�N��ݒ肵�܂�
    OS_SetUserExceptionHandler( dbs_DisplayException, NULL );
    //---- �����ŃL���b�V�����t���b�V������̂́A
    //     ��O����������ƃv���e�N�V�������j�b�g���ύX�����̂�
    //     ��O�Ɋւ���A�h���X���͊m���ɏ�������ł�����������B
    DC_FlushAll();

    // �R���\�[���쐬
    dbs_CreateConsole( & sDemoConsole, sDemoLineBuf, sizeof(sDemoLineBuf), 0, 5, 32, 19 );
    
}

void dbs_DemoFinalize( void )
{
    dbs_DestroyConsole( &sDemoConsole );
}



void dbs_DemoUpdate( void )
{
    dbs_DrawConsole();
}



void dbs_DemoLoad( void )
{
    dbs_Load();
}
