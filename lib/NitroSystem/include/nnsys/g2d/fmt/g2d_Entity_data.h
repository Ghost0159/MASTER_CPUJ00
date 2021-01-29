/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_Entity_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_ENTITY_DATA_H_
#define NNS_G2D_ENTITY_DATA_H_

#include <nitro/types.h>
#include <nnsys/g2d/fmt/g2d_Common_data.h>
#include <nnsys/g2d/fmt/g2d_Anim_data.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NNS_G2D_BINFILE_SIG_ENTITY        (u32)'NENR'
#define NNS_G2D_BLKSIG_ENTITYBANK         (u32)'ENBK'

#define NNS_G2D_BINFILE_EXT_ENTITY          "NENR"

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
//
#define NNS_G2D_NENR_MAJOR_VER              1
#define NNS_G2D_NENR_MINOR_VER              0


//------------------------------------------------------------------------------
// Entity �� ���
typedef enum NNSG2dEntityType
{
    NNS_G2D_ENTITYTYPE_INVALID      = 0,
    NNS_G2D_ENTITYTYPE_CELL         ,
    NNS_G2D_ENTITYTYPE_MULTICELL    ,
    NNS_G2D_ENTITYTYPE_MAX          

}NNSG2dEntityType;

//------------------------------------------------------------------------------
// Entity �����A�j���[�V�����f�[�^�z��
typedef struct NNSG2dEntityAnimData
{
    u16         numAnimSequence;
    u16         pad16;
    u16*        pAnimSequenceIdxArray;      // NNSG2dAnimSequence�ԍ��z��
    
}NNSG2dEntityAnimData;

//------------------------------------------------------------------------------
// Entity �ݒ���
typedef struct NNSG2dEntityData
{
    
    NNSG2dEntityAnimData          animData;      // Entity �����A�j���[�V�����f�[�^�z��
    NNSG2dEntityType              type;          // �`����̂̎��
    
    // const NNSG2dAnimBankData*     pAnimDataBank; // �֘A����AnimBank (�����^�C���ɂ�Unpack���ɐݒ肳��܂�)
    // 
    // ���̏��͎��̑��ŕێ����邱�ƂƂ���
    // 
    // ���̕ύX�́ANNSG2dEntityData �� �Ƃ��Ă� �ŗǂ̑I���ł͂Ȃ��B
    // �i NNSG2dEntityData �̐��͏��Ȃ��A��������L������̂̐��͑��� �j
    // NNSG2dMultiCellDataBank �Ƃ̓��ꊴ���o�����߂̕ύX�ł���B
    // �����ŁA��������R�X�g�ɑ΂��Ă͂��܂�i�[�o�X�ɂ͂Ȃ�Ȃ��Ƃ����X�^���X�B
    // 
    // NNSG2dMultiCellDataBank �錾 �� �R�����g �� �Q�Ƃ��ꂽ���B
    // 
    
}NNSG2dEntityData;


//------------------------------------------------------------------------------
// Entity �ݒ���o���N
typedef struct NNSG2dEntityDataBank
{
    u16                     numEntityDatas;
    u16                     pad16;
    NNSG2dEntityData*       pEntityDataArray;
    u16*                    pAnimSequenceIdxArray;
    void*                   pStringBank;
    void*                   pExtendedData;        // offset addr (if it exists.)
    
}NNSG2dEntityDataBank;


typedef struct NNSG2dEntityDataBankBlock
{
    
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dEntityDataBank        entityDataBank;
    
}NNSG2dEntityDataBankBlock;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_ENTITY_DATA_H_

