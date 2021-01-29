/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - samples - VramManager - FramePlttVramMan
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/
//
// �t���[��VRAM�}�l�[�W��(�e�N�X�`���p���b�g)���g�p����T���v���ł��B
// ���܂��܂ȃp�^�[���ŗ̈�m�ۂ��s���܂��B
// 
#include <nitro.h>
#include <nnsys/gfd.h>


//   �v���O�������Ŏg���܂킷�ϐ�
static NNSGfdFrmPlttVramState           state_;
static NNSGfdPlttKey                    mem;
static int                              vramLowerPtr,vramHigherPtr;

//  �u�q�`�l�̏�ʃ|�C���^,���ʃ|�C���^���}�l�[�W���̂��̂Əƍ�����
static BOOL checkVramPtr() {
     NNS_GfdGetFrmPlttVramState(&state_);
	return ((state_.address[0] == vramLowerPtr) && (state_.address[1] == vramHigherPtr));
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  �T���v���̃��C���֐�
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    OS_Init();
    
    {
        const char* vramName[6] 
            = {"VRAM-F","VRAM-G","VRAM-F,G","VRAM-E","VRAM-E,F","VRAM-E,F,G"};
        const GXVRamTexPltt vramEnum[6] 
            = {GX_VRAM_TEXPLTT_0_F,GX_VRAM_TEXPLTT_0_G,GX_VRAM_TEXPLTT_01_FG,
               GX_VRAM_TEXPLTT_0123_E,GX_VRAM_TEXPLTT_01234_EF,GX_VRAM_TEXPLTT_012345_EFG};
        const int vramSize[6] 
            = {16 * 1024,16 * 1024,(16 + 16) * 1024,64 * 1024,(64 + 16) * 1024,(64 + 16 + 16) * 1024};
       
       
       
        //  �u�q�`�l�g�ݍ��킹�S�Ẵp�^�[�����e�N�X�`���p���b�g�Ɋ��蓖�Ăăe�X�g
        int i;
        for(i = 0;i < 6;i ++) 
        {
            //  �|�C���^�̏����l�ݒ�
            vramLowerPtr = 0;
            vramHigherPtr = vramSize[i];
            
            OS_Printf("%s  checking\n",vramName[i]);
            GX_SetBankForTexPltt(vramEnum[i]);
            
            //
            // �}�l�[�W���̏�����
            //
            NNS_GfdInitFrmPlttVramManager( (u32)vramSize[i], TRUE );
            
            //   �S�F�p���b�g�łȂ��̈�����ʃA�h���X����m��
            mem = NNS_GfdAllocPlttVram(0x100,FALSE,TRUE);
            vramLowerPtr += 0x100;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x100);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x100);
            
            //  �S�F�p���b�g�łȂ��̈����ʃA�h���X����m��
            mem = NNS_GfdAllocPlttVram(0x100,FALSE,FALSE);
            vramHigherPtr -= 0x100;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x100);
            
            //  �S�F�p���b�g�̗̈�����ʃA�h���X����m��
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            //   �S�F�p���b�g�����A���C�����g��0x8�Ȃ̂ł��ꂪ��肭�����Ă��邩�m���߂�
            mem = NNS_GfdAllocPlttVram(0x10,TRUE,TRUE);
            vramLowerPtr += 0x10;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x10);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,TRUE);
            vramLowerPtr += 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x8);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            
            // �}�l�[�W���̏�Ԃ��f�o�b�N�o�͂���
            NNS_GfdDumpFrmPlttVramManager();
            
            //  �A�h���X��0x****8�Ȃ̂ł����łS�F�p���b�g�łȂ��e�N�X�`���p�ɗ̈���m��
            //  ���̃A�h���X+0x8�̃A�h���X����̈悪�m�ۂ����
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,TRUE);
            vramLowerPtr += 0x10 + 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramLowerPtr - 0x10);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            
            //  0x10000�𒴂����T�C�Y�̂u�q�`�l�����蓖�ĂĂ���,
            //  ����,0x10000�ȏ�̗̈�S�Ă��m�ۍς݂łȂ����
            //  �S�F�p���b�g�̃e�N�X�`���̈����ʑ�����m�ۂ��邱�Ƃ͏o���Ȃ�
            if(vramSize[i] > 0x10000) {
                //
                // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
                //
                //   ����͊m�ۂł��Ȃ�
                //mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
                //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
                
                //   0x10000�𒴂���̈��S�Ċm�ۂ���
                mem = NNS_GfdAllocPlttVram((u32)(vramSize[i] - 0x10000),FALSE,FALSE);
                vramHigherPtr -= vramSize[i] - 0x10000;
                NNS_GFD_ASSERT(checkVramPtr());
                NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
                NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == vramSize[i] - 0x10000);
            }
            
            //   ���if���Ŋm�ۂł��Ȃ������ꍇ�ł����x�͊m�ۂł���
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            
            //   ���ʑ��̃A�h���X�̎��Ɠ��l,
            //   �S�F�p���b�g�����A���C�����g��0x8�Ȃ̂ł��ꂪ��肭�����Ă��邩�m���߂�
            mem = NNS_GfdAllocPlttVram(0x10,TRUE,FALSE);
            vramHigherPtr -= 0x10;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            mem = NNS_GfdAllocPlttVram(0x8,TRUE,FALSE);
            vramHigherPtr -= 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x8);
            //  �A�h���X��0x****8�Ȃ̂ł����łS�F�p���b�g�łȂ��e�N�X�`���p�ɗ̈���m��
            //  ���̃A�h���X-0x8�̃A�h���X����̈悪�m�ۂ����
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,FALSE);
            vramHigherPtr -= 0x10 + 0x8;
            NNS_GFD_ASSERT(checkVramPtr());
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeyAddr(mem) == vramHigherPtr);
            NNS_GFD_ASSERT(NNS_GfdGetPlttKeySize(mem) == 0x10);
            
            //
            // �������m�ۓ���̎��s���m�F�������ꍇ�̓R�����g���������s���Ă݂Ă�������
            //
            //  �󂫗e�ʈȏ�̗̈��v�����Ēf����
            //mem = NNS_GfdAllocPlttVram(vramSize[i],TRUE,FALSE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],FALSE,FALSE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],TRUE,TRUE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            //mem = NNS_GfdAllocPlttVram(vramSize[i],FALSE,TRUE);
            //NNS_GFD_ASSERT(mem == NNS_GFD_ALLOC_ERROR_PLTTKEY);
            
            //  ���݂̏�Ԃ�state_�ɕۑ�
            NNS_GfdGetFrmPlttVramState(&state_);
            
            //  ��ʑ�,���ʑ��ɓK���ȗ̈���m��
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,FALSE);
            mem = NNS_GfdAllocPlttVram(0x18,TRUE,FALSE);
            mem = NNS_GfdAllocPlttVram(0x10,FALSE,TRUE);
            mem = NNS_GfdAllocPlttVram(0x18,TRUE,TRUE);
            
            //  state_�ɋL�^����Ă�����̂��}�l�[�W���ɖ߂�
            NNS_GfdSetFrmPlttVramState(&state_);
            NNS_GFD_ASSERT(checkVramPtr());
            
            
            {
                // �T�C�Y�[��
                mem = NNS_GfdAllocPlttVram(0x0,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN ��菬����
                mem = NNS_GfdAllocPlttVram(0x1,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN ���傤��
                mem = NNS_GfdAllocPlttVram(NNS_GFD_PLTTSIZE_MIN,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN );
                NNS_GfdSetFrmPlttVramState(&state_);
                // NNS_GFD_PLTTSIZE_MIN ��肷�����傫��
                mem = NNS_GfdAllocPlttVram(NNS_GFD_PLTTSIZE_MIN+1,TRUE,FALSE);
                NNS_GFD_ASSERT( NNS_GfdGetPlttKeySize(mem) == NNS_GFD_PLTTSIZE_MIN * 2);
                NNS_GfdSetFrmPlttVramState(&state_);
            }
            
            (void)GX_DisableBankForTex();
            
            OS_Printf("%s  OK\n",vramName[i]);
        }
    }
    
    while(1) {}
}

