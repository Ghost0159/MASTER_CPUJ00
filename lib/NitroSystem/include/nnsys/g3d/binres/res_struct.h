/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d - binres
  File:     res_struct.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.37 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_BINRES_RES_STRUCT_H_
#define NNSG3D_BINRES_RES_STRUCT_H_

#include <nnsys/g3d/config.h>
#define NNS_G3D_RES_INLINE NNS_G3D_INLINE

#ifdef NNS_FROM_TOOL
// VC++
typedef u32 NNSGfdTexKey;
#else
#include <nnsys/gfd.h>
SDK_COMPILER_ASSERT(sizeof(NNSGfdTexKey) == sizeof(u32));
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    An overview of relation between NNS_G3D_DATABLK_MDLSET structs
    (NNSG3dResMdlSet as a root)
    
    .... -- 1:N relation(referenced by name/index, using NNSG3dResDict)
    ---> -- 1:1 relation(own/refer by offset)

    NNSG3dResMdlSet .... NNSG3dResMdl ---> NNSG3dResMdlInfo
                                      ---> NNSG3dResNodeInfo
                                      ---> Structure Byte Code
                                      ---> NNSG3dResMat
                                      ---> NNSG3dResShp

    NNSG3dResMdlInfo ---> NNSG3dSbcType/NNSG3dScalingRule/NNSG3dTexMtxMode etc.
    
    NNSG3dResNodeInfo .... NNSG3dResNodeData ---> NNSG3dSRTFlag etc.

    NNSG3dResMat .... NNSG3dResMatData

    NNSG3dResShp .... NNSG3dResShpData ---> Display List
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    Signatures for G3D's binary files

 *---------------------------------------------------------------------------*/
#define NNS_G3D_SIGNATURE_NSBMD  '0DMB'
#define NNS_G3D_SIGNATURE_NSBTX  '0XTB'
#define NNS_G3D_SIGNATURE_NSBCA  '0ACB'
#define NNS_G3D_SIGNATURE_NSBVA  '0AVB'
#define NNS_G3D_SIGNATURE_NSBMA  '0AMB'
#define NNS_G3D_SIGNATURE_NSBTP  '0PTB'
#define NNS_G3D_SIGNATURE_NSBTA  '0ATB'


/*---------------------------------------------------------------------------*
    Signatures for G3D data blocks

 *---------------------------------------------------------------------------*/
#define NNS_G3D_DATABLK_MDLSET     '0LDM'
#define NNS_G3D_DATABLK_TEX        '0XET'
#define NNS_G3D_DATABLK_JNT_ANM    '0TNJ'
#define NNS_G3D_DATABLK_VIS_ANM    '0SIV'
#define NNS_G3D_DATABLK_MATC_ANM   '0TAM'
#define NNS_G3D_DATABLK_TEXPAT_ANM '0TAP'
#define NNS_G3D_DATABLK_TEXSRT_ANM '0TRS'


/*---------------------------------------------------------------------------*
    �}�e���A���E�V�F�C�v�̎�ʃC���f�b�N�X

    �}�e���A���E�V�F�C�v�ɂ��Ă̓f�[�^�t�H�[�}�b�g���������݂��邱�Ƃ��\�Ȃ悤��
    ���Ă���B���_�A�j���[�V�������s�����ߒ��_�f�[�^���C���f�b�N�X��������悤�ȃV�F�C�v��
    �K�v���A�Ƃ������ꍇ�̊g����z�肵�Ă���B

    NOTICE:
        �֐��e�[�u���ւ̃C���f�b�N�X�ɂȂ��Ă���̂ŁA
        ���p�\�Ȓl�͊֐��e�[�u���̃T�C�Y�ɂ��B
 *---------------------------------------------------------------------------*/
typedef u16 NNSG3dItemTag;
#define NNS_G3D_ITEMTAG_MAT_STANDARD  0
#define NNS_G3D_ITEMTAG_SHP_STANDARD  0

/////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------*
    Name:        NNSG3dResFileHeader

    Description: A header of a resouce file

                 signature  = (sigVal = NNS_G3D_SIGNATURE_XXXX)
                 byteOrder  = 0xfeff
                 version    = described in u8.u8 style
                 fileSize   = size of a data file
                 headerSize = always 16
                 dataBlocks = the number of blocks
                              (the number of datablock header)
                 Following 'dataBlocks' words are the indices to the blocks.
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResFileHeader_
{
    union 
    {
        char signature[4];
        u32  sigVal;
    };
    u16  byteOrder;
    u16  version;
    u32  fileSize;
    u16  headerSize;
    u16  dataBlocks;
}
NNSG3dResFileHeader;


/*---------------------------------------------------------------------------*
   Name:        NNSG3dResDataBlockHeader:

   Description: A header of a data block
                
                kind = the kind of a data block
                size = size of a data block
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDataBlockHeader_
{
    union
    {
        u32 kind;
        char chr[4];
    };
    u32 size;
}
NNSG3dResDataBlockHeader;


/////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------*
   Name:        NNSG3dResDictTreeNode:

   Description: A node of a patricia tree

                refBit   = an index to a referred bit(0-127)
                idxLeft  = go left if the bit is zero
                idxRight = go right if the bit is one
                idxEntry = an index to a data entry
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictTreeNode_
{
    u8  refBit;
    u8  idxLeft;
    u8  idxRight;
    u8  idxEntry;
}
NNSG3dResDictTreeNode;


/*---------------------------------------------------------------------------*
   Name:        NNSG3dResDict:

   Description: ���O�����p�̎����w�b�_
                �C���f�b�N�X��&NNSG3dResDict����̃I�t�Z�b�g
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDict_
{
    u8  revision;      // revision of dictionary
    u8  numEntry;      // num of entry(not that of tree node)
    u16 sizeDictBlk;   // size of this block
    u16 dummy_;
    u16 ofsEntry;      // (NNSG3dResDictEntryHeader*)   ((u8*)this + idxEntry)

    // patricia tree and its data entry
    NNSG3dResDictTreeNode node[1];  // variable size
}
NNSG3dResDict;


/*---------------------------------------------------------------------------*
   Name:        NNSG3dResName

   Description: A structure for a resouce name
                '\0' must be filled if the length of string is less than 16.
 *---------------------------------------------------------------------------*/
#define NNS_G3D_RESNAME_SIZE    (16)
#define NNS_G3D_RESNAME_VALSIZE (NNS_G3D_RESNAME_SIZE / sizeof(u32))

typedef union NNSG3dResName_
{
    char name[NNS_G3D_RESNAME_SIZE];
    u32  val[NNS_G3D_RESNAME_VALSIZE];
}
NNSG3dResName;


/*---------------------------------------------------------------------------*
   Name:        NNSG3dResDictEntryHeader

   Description: ���O�����p�̎����G���g���w�b�_
                ���O�̌��sizeUnit-16�o�C�g�̃f�[�^������
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictEntryHeader_
{
    u16 sizeUnit;      // �f�[�^�G���g��1���̃T�C�Y�i�o�C�g�j
    u16 ofsName;       // 
    u8  data[4];       // NNSG3dResDictEntryItem�̕���(1������̑傫����sizeUnit)
}
NNSG3dResDictEntryHeader;


/////////////////////////////////////////////////////////////////////////////////
//
//
// Texture Image
//
//


/*---------------------------------------------------------------------------*
    NNSG3dTexImageParam

    �e�N�X�`���u���b�N���̃e�N�X�`���C���[�W���
    ���f���̃}�e���A���Ɗ֘A�t�����s���Ƃ��ɂ����̏�񂪃R�s�[�����
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_MASK  = 0x0000ffff,
    NNS_G3D_TEXIMAGE_PARAM_S_SIZE_MASK    = 0x00700000,
    NNS_G3D_TEXIMAGE_PARAM_T_SIZE_MASK    = 0x03800000,
    NNS_G3D_TEXIMAGE_PARAM_TEXFMT_MASK    = 0x1c000000,
    NNS_G3D_TEXIMAGE_PARAM_TR_MASK        = 0x20000000,

    NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_SHIFT = 0,
    NNS_G3D_TEXIMAGE_PARAM_S_SIZE_SHIFT   = 20,
    NNS_G3D_TEXIMAGE_PARAM_T_SIZE_SHIFT   = 23,
    NNS_G3D_TEXIMAGE_PARAM_TEXFMT_SHIFT   = 26,
    NNS_G3D_TEXIMAGE_PARAM_TR_SHIFT       = 29
}
NNSG3dTexImageParam;


/*---------------------------------------------------------------------------*
    NNSG3dTexImageParamEx

    ORIGW��ORIGH�̓e�N�X�`���s��̌v�Z�ɂ����ĕK�v
    NNSG3dTexImageParam�̒l�Ɠ����ł���Όv�Z���������ł���̂ŁA
    �t���O(WHSAME)�������Ă���
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_MASK  = 0x000007ff,
    NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_MASK  = 0x003ff800,
    NNS_G3D_TEXIMAGE_PARAMEX_WHSAME_MASK = 0x80000000,

    NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_SHIFT  = 0,
    NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_SHIFT  = 11,
    NNS_G3D_TEXIMAGE_PARAMEX_WHSAME_SHIFT = 31
}
NNSG3dTexImageParamEx;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexData

    �e�N�X�`���������̃f�[�^�G���g��
    �ʏ�A�����̃f�[�^�ɂ͉ϒ��f�[�^�ւ̃I�t�Z�b�g�������Ă��邱�Ƃ��������A
    �Œ蒷�̏ꍇ�͎����̃G���g���Ƃ��ē��ꍞ�ނ��Ƃ��ł���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictTexData_
{
    // 31 30 29     28  26 25 23 22 20 19   16 15                       0
    //       Pltt0  TexFmt TSize SSize         3bit�E�V�t�g���ꂽ�I�t�Z�b�g
    u32           texImageParam;  // NNSG3dTexImageParam

    // 31      30           22 21     11 10     0
    // SameBit                   OrigH     OrigW
    u32           extraParam;     // NNSG3dTexImageParamEx
}
NNSG3dResDictTexData;


/*---------------------------------------------------------------------------*
    NNSG3dResTexFlag

    �e�N�X�`���C���[�W(4x4�t�H�[�}�b�g�ȊO)���e�N�X�`���C���[�W�X���b�g�Ƀ��[�h�����Ƃ��ɁA
    NNS_G3D_RESTEX_LOADED���Z�b�g�����B�A�����[�h�����Ƃ��ɂ̓��Z�b�g�����B
    NNSG3dResTexInfo::flag�p�̒l�ł���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_RESTEX_LOADED   = 0x0001
}
NNSG3dResTexFlag;


/*---------------------------------------------------------------------------*
    NNSG3dResTexInfo

    NNSG3dResTex�����L����\���́B
    4x4COMP�t�H�[�}�b�g�ȊO�̃e�N�X�`���C���[�W�Ɋւ������ێ����Ă���B
    nsbmd/nsbtx�t�@�C�����Ƀe�N�X�`���C���[�W�f�[�^�͂P�ɂ܂Ƃ߂��Ă��āA
    �܂Ƃ܂育�ƂɃ��[�h�^�A�����[�h����悤�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTexInfo_
{
    NNSGfdTexKey vramKey;         // gfd���C�u�����d�l��VRAM�L�[���i�[����B
    u16          sizeTex;         // �e�N�X�`���f�[�^�̃T�C�Y���E��3bit�V�t�g�������̂��\�ߓ����Ă���
    u16          ofsDict;
    u16          flag;            // NNSG3dResTexFlag
    u16          dummy_;
    u32          ofsTex;          // �e�N�X�`���f�[�^�ւ̃I�t�Z�b�g(&NNSG3dResTexImage����̃I�t�Z�b�g)
}
NNSG3dResTexInfo;


/*---------------------------------------------------------------------------*
    NNSG3dResTex4x4Flag

    4x4COMP�t�H�[�}�b�g�̃e�N�X�`���C���[�W���e�N�X�`���C���[�W�X���b�g�Ƀ��[�h�����Ƃ��ɁA
    NNS_G3D_RESTEX4x4_LOADED���Z�b�g�����B�A�����[�h�����Ƃ��ɂ̓��Z�b�g�����B
    NNSG3dResTex4x4Info::flag�p�̒l�ł���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_RESTEX4x4_LOADED   = 0x0001   // VRAM�Ƀ��[�h���ꂽ���ǂ���
}
NNSG3dResTex4x4Flag;


/*---------------------------------------------------------------------------*
    NNSG3dResTex4x4Info:

    NNSG3dResTex�����L����\���́B
    4x4COMP�t�H�[�}�b�g�̃e�N�X�`���C���[�W�Ɋւ������ێ����Ă���B
    nsbmd/nsbtx�t�@�C�����Ƀe�N�X�`���C���[�W�f�[�^�͂P�ɂ܂Ƃ߂��Ă��āA
    �܂Ƃ܂育�ƂɃ��[�h�^�A�����[�h����悤�ɂȂ��Ă���B   
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTex4x4Info_
{
    NNSGfdTexKey vramKey;         // gfd���C�u�����d�l��VRAM�L�[���i�[����B
    u16          sizeTex;         // �e�N�X�`���f�[�^�̃T�C�Y���E��3bit�V�t�g��������(�p���b�g�C���f�b�N�X�̏ꍇ��2bit)
    u16          ofsDict;
    u16          flag;            // 0�r�b�g�ڂ̓e�N�X�`����VRAM�Ƀ��[�h���ꂽ���ǂ����̃t���O
    u16          dummy_;
    u32          ofsTex;          // �e�N�X�`���f�[�^�ւ̃I�t�Z�b�g(&NNSG3dResTexImage����̃o�C�g)
    u32          ofsTexPlttIdx;   // �e�N�X�`���p���b�g�C���f�b�N�X�f�[�^�ւ̃I�t�Z�b�g(&NNSG3dResTexImage����̃o�C�g)
}
NNSG3dResTex4x4Info;


/*---------------------------------------------------------------------------*
    NNSG3dResDictPlttData

    �p���b�g�������̃f�[�^�G���g��
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictPlttData_
{
    u16 offset;          // �p���b�g�f�[�^�̋N�_�ɑ΂���I�t�Z�b�g��3bit�E�V�t�g��������
    u16 flag;            // 0�r�b�g�ڂ�Tex4Pltt���ǂ����������t���O
}
NNSG3dResDictPlttData;


/*---------------------------------------------------------------------------*
    NNSG3dResPlttFlag

    �e�N�X�`���p���b�g���e�N�X�`���p���b�g�X���b�g�Ƀ��[�h�����Ƃ���
    NNS_G3D_RESPLTT_LOADED���Z�b�g�����B�A�����[�h�����Ƃ��ɂ̓��Z�b�g�����B
    NNS_G3D_RESPLTT_USEPLTT4�́A�p���b�g�f�[�^��4�F�J���[�p���b�g���܂܂�Ă���
    �ꍇ�ɃZ�b�g����Ă���B4�F�J���[�p���b�g�̃��[�h��ɂ͐��������邩��ł���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_RESPLTT_LOADED   = 0x0001,   // VRAM�Ƀ��[�h���ꂽ���ǂ���
    NNS_G3D_RESPLTT_USEPLTT4 = 0x8000    // �u���b�N����4�F�p���b�g�������Ă��邩�ǂ���
}
NNSG3dResPlttFlag;


/*---------------------------------------------------------------------------*
    NNSG3dResPlttInfo

    NNSG3dResTex�����L����\���́B
    �e�N�X�`���p���b�g�Ɋւ������ێ����Ă���B
    nsbmd/nsbtx�t�@�C�����Ƀe�N�X�`���p���b�g�f�[�^�͂P�ɂ܂Ƃ߂��Ă��āA
    �܂Ƃ܂育�ƂɃ��[�h�^�A�����[�h����悤�ɂȂ��Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResPlttInfo_
{
    NNSGfdTexKey vramKey;      // gfd���C�u�����d�l��VRAM�L�[���i�[����B
    u16          sizePltt;     // �e�N�X�`���p���b�g�̃f�[�^�T�C�Y���E��3bit�V�t�g�������̂�����
    u16          flag;         // NNSG3dResPlttFlag
    u16          ofsDict;
    u16          dummy_;
    u32          ofsPlttData;
}
NNSG3dResPlttInfo;


/*---------------------------------------------------------------------------*
    NNSG3dResTex
   
    �i�[����Ă���e��I�t�Z�b�g�́AVRAM���̃X���b�g�ɑ΂���I�t�Z�b�g�f�[�^�ȊO�́A
    &NNSG3dResTex���x�[�X�ɂ��Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTex_
{
    NNSG3dResDataBlockHeader header;
    NNSG3dResTexInfo         texInfo;
    NNSG3dResTex4x4Info      tex4x4Info;
    NNSG3dResPlttInfo        plttInfo;
    NNSG3dResDict            dict;       // �e�N�X�`���p����
    // �p���b�g�p����
    // �e�N�X�`���f�[�^(4x4�ȊO)
    // �e�N�X�`���f�[�^(4x4)
    // �e�N�X�`���p���b�g�C���f�b�N�X�f�[�^
    // �e�N�X�`���p���b�g�f�[�^
    // �Ƒ���
}
NNSG3dResTex;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �}�e���A��
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResDictMatData

    �}�e���A���������f�[�^�B
    �}�e���A���f�[�^�͉ϒ��Ȃ̂�NNSG3dResMatData(��)�ւ̃I�t�Z�b�g���������Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictMatData_
{
    u32           offset;       // &NNSG3dResMat����̃I�t�Z�b�g�f�[�^
}
NNSG3dResDictMatData;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexToMatIdxData

    �e�N�X�`����->�}�e���A���C���f�b�N�X�񎫏��f�[�^
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictTexToMatIdxData_
{
    u16           offset;       // NNSG3dResMat����̃I�t�Z�b�g
    u8            numIdx;       // �e�N�X�`�����ɑΉ�����}�e���A���h�c�̐�
    u8            flag;         // �e�N�X�`��<->�}�e���A���̃o�C���h�����ꂽ���ǂ����̃t���O
}
NNSG3dResDictTexToMatIdxData;


/*---------------------------------------------------------------------------*
    NNSG3dResDictPlttToMatIdxData

    �p���b�g��->�}�e���A���C���f�b�N�X�񎫏��f�[�^
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictPlttToMatIdxData_
{
    u16           offset;       // NNSG3dResMat����̃I�t�Z�b�g
    u8            numIdx;       // �p���b�g���ɑΉ�����}�e���A���h�c�̐�
    u8            flag;         // �p���b�g<->�}�e���A���̃o�C���h�����ꂽ���ǂ����̃t���O
}
NNSG3dResDictPlttToMatIdxData;


/*---------------------------------------------------------------------------*
    NNSG3dMatFlag

    �}�e���A���Ɋւ���t���O���
    �r�b�g���Z�b�g����Ă���΁AX���g�p����A�Ƃ������t���O�ɂȂ��Ă���B
    ���������ɁA�e�N�X�`���f�[�^�Ɗ֘A�t����Ƃ��ɑ��삳���t���O������B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_MATFLAG_TEXMTX_USE       = 0x0001, // �e�N�X�`���s����g�p���邩�ǂ���
    NNS_G3D_MATFLAG_TEXMTX_SCALEONE  = 0x0002, // �X�P�[����1.0�Ȃ�ON(�e�N�X�`���s��g�p��)
    NNS_G3D_MATFLAG_TEXMTX_ROTZERO   = 0x0004, // ��]���Ȃ��Ȃ�ON(�e�N�X�`���s��g�p��)
    NNS_G3D_MATFLAG_TEXMTX_TRANSZERO = 0x0008, // ���s�ړ����Ȃ��Ȃ�ON(�e�N�X�`���s��g�p��)
    NNS_G3D_MATFLAG_ORIGWH_SAME      = 0x0010, // �e�N�X�`����Width/Height���V�X�e���Ɠ����ꍇ�Z�b�g�����
    NNS_G3D_MATFLAG_WIREFRAME        = 0x0020, // ���C���[�t���[���\���Ȃ�ON
    NNS_G3D_MATFLAG_DIFFUSE          = 0x0040, // �}�e���A����diffuse���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_AMBIENT          = 0x0080, // �}�e���A����ambient���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_VTXCOLOR         = 0x0100, // �}�e���A����vtxcolor�t���O���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_SPECULAR         = 0x0200, // �}�e���A����specular���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_EMISSION         = 0x0400, // �}�e���A����emission���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_SHININESS        = 0x0800, // �}�e���A����shininess�t���O���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_TEXPLTTBASE      = 0x1000, // �e�N�X�`���p���b�g�x�[�X�A�h���X���w�肷��Ȃ�ON
    NNS_G3D_MATFLAG_EFFECTMTX        = 0x2000  // effect_mtx�����݂���Ȃ�ON
}
NNSG3dMatFlag;


/*---------------------------------------------------------------------------*
    NNSG3dResMatData

    �X�̃}�e���A���̃f�[�^�B
    �f�[�^���g������ꍇ�́AitemTag�̒l��ς��āA�\���̂̌���Ƀf�[�^��z�u����
    �悤�ɂ���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMatData_
{
    NNSG3dItemTag itemTag;                // �}�e���A����ʔF���^�O(���̍\���̂̏ꍇ��NNS_G3D_ITEMTAG_MAT_STANDARD�ł���K�v������)
    u16           size;                   // �T�C�Y
    u32           diffAmb;                // MaterialColor0�R�}���h�ւ̃p�����^
    u32           specEmi;                // MaterialColor1�R�}���h�ւ̃p�����^
    u32           polyAttr;               // PolygonAttr�R�}���h�ւ̃p�����^
    u32           polyAttrMask;           // �}�e���A���Ŏw�肷��r�b�g��1�ɂȂ��Ă���
    u32           texImageParam;          // TexImageParam�R�}���h�ւ̃p�����^
    u32           texImageParamMask;      // �}�e���A���Ŏw�肷��r�b�g��1�ɂȂ��Ă���
    u16           texPlttBase;            // TexPlttBase�R�}���h�ւ̃p�����^
    u16           flag;                   // NNSG3dMatFlag�փL���X�g
    u16           origWidth, origHeight;  // ���������Ƀe�N�X�`���C���[�W����]�������
    fx32          magW;                   // �o�C���h���ꂽ�e�N�X�`���̕�/origWidth
    fx32          magH;                   // �o�C���h���ꂽ�e�N�X�`���̍���/origHeight
//    fx32 scaleS, scaleT;                // exists if NNS_G3D_MATFLAG_TEXMTX_SCALEONE is off
//    fx16 rotSin, rotCos;                // exists if NNS_G3D_MATFLAG_TEXMTX_ROTZERO is off
//    fx32 transS, transT;                // exists if NNS_G3D_MATFLAG_TEXMTX_TRANSZERO is off
//    fx32 effectMtx[16];                 // exists if NNS_G3D_MATFLAG_ENV_EFFECTMTX is on
}
NNSG3dResMatData;


/*---------------------------------------------------------------------------*
    NNSG3dResMat

    �P�̃��f�������}�e���A���̏W���𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMat_
{
    u16              ofsDictTexToMatList;  // �e�N�X�`��->�}�e���A��ID�񎫏��ւ̃I�t�Z�b�g
    u16              ofsDictPlttToMatList; // �p���b�g->�}�e���A��ID�񎫏��ւ̃I�t�Z�b�g
    NNSG3dResDict    dict;                 // NNSG3dResDictMatData�ւ̎���
//  NNSG3dResDict    dictTexToMatList;     // (u8*)this + idxDictTexToMatList
//  NNSG3dResDict    dictPlttToMatList;    // (u8*)this + idxDictPlttToMatList
//  u8[]             matIdxData;           // (u8*)this + idxMatIdxData
//  seq of NNSG3dResMatData matData        // (u8*)this + idxMatData
}
NNSG3dResMat;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �V�F�C�v
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResDictShpData

    �V�F�C�v�������f�[�^�B
    �V�F�C�v�f�[�^�͉ϒ��Ȃ̂�NNSG3dResShpData(��)�ւ̃I�t�Z�b�g���������Ă���
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictShpData_
{
    u32           offset;  // (u8*)&NNSG3DResShp + offset -> NNSG3dResShpData&
}
NNSG3dResDictShpData;


/*---------------------------------------------------------------------------*
    NNSG3dShpFlag

    �V�F�C�v�̃f�B�X�v���C���X�g�̓��F��\���t���O�ł��B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_SHPFLAG_USE_NORMAL     = 0x00000001,    // DL����Normal�R�}���h�����݂��܂��B
    NNS_G3D_SHPFLAG_USE_COLOR      = 0x00000002,    // DL����Color�R�}���h�����݂��܂��B
    NNS_G3D_SHPFLAG_USE_TEXCOORD   = 0x00000004,    // DL����TexCoord�R�}���h�����݂��܂��B
    NNS_G3D_SHPFLAG_USE_RESTOREMTX = 0x00000008     // DL����RestoreMtx�R�}���h�����݂��܂��B
}
NNSG3dShpFlag;


/*---------------------------------------------------------------------------*
    NNSG3dResShpData

    �X�̃V�F�C�v�f�[�^�B
    �قȂ�f�[�^�\���ŃV�F�C�v���L�q����ꍇ�ł��AitemTag��size�����͏�������K�v������B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResShpData_
{
    NNSG3dItemTag itemTag;          // �V�F�C�v��ʔF���^�O(���̍\���̂̏ꍇ��NNS_G3D_ITEMTAG_SHP_STANDARD�ł���K�v������)
    u16           size;             // �T�C�Y
    u32           flag;             // NNSG3dShpFlag
    u32           ofsDL;            // �f�B�X�v���C���X�g�ւ̃I�t�Z�b�g
    u32           sizeDL;           // �f�B�X�v���C���X�g�̃T�C�Y
}
NNSG3dResShpData;


/*---------------------------------------------------------------------------*
    NNSG3dResShp

    �P�̃��f�������V�F�C�v�̏W���𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResShp_
{
    NNSG3dResDict dict;             // NNSG3dResDictShpData�ւ̎���
    // NNSG3dResShpData[] shpData;
    // u32[]              DL;
}
NNSG3dResShp;


typedef struct NNSG3dResEvpMtx_
{
    MtxFx43 invM;
    MtxFx33 invN;
}
NNSG3dResEvpMtx;


/////////////////////////////////////////////////////////////////////////////////
//
//
// ModelInfo
//
//


/*---------------------------------------------------------------------------*
    NNSG3dSbcType

    ���f������������Structure Byte Code�̃^�C�v���w�肷��
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_SBCTYPE_NORMAL       = 0        // ���݂̂Ƃ���P��̃^�C�v�����Ȃ�
}
NNSG3dSbcType;


/*---------------------------------------------------------------------------*
    NNSG3dScalingRule

    ���f���̃X�P�[�����O���[�����w�肷��
    ���ԃt�@�C���t�H�[�}�b�g��<model_info>::scaling_rule�ɑΉ�����B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_SCALINGRULE_STANDARD = 0,   // ��ʓI�ȍs��v�Z�ł悢
    NNS_G3D_SCALINGRULE_MAYA     = 1,   // Maya��Segment Scaling Compensate�̍l�����s��Ȃ���΂Ȃ�Ȃ��m�[�h�����݂���
    NNS_G3D_SCALINGRULE_SI3D     = 2    // Softimage3D�̃X�P�[�����O�v�Z���s��Ȃ��Ă͂Ȃ�Ȃ�
}
NNSG3dScalingRule;


/*---------------------------------------------------------------------------*
    NNSG3dTexMtxMode

    �e�N�X�`���s��̌v�Z���@���w�肷��
    ���ԃt�@�C���t�H�[�}�b�g��<model_info>::tex_mtx_mode�ɑΉ�����B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_TEXMTXMODE_MAYA      = 0x00, // Maya�̌v�Z���@��K�p����B
    NNS_G3D_TEXMTXMODE_SI3D      = 0x01, // Softimage3D�̌v�Z���@��K�p����B
    NNS_G3D_TEXMTXMODE_3DSMAX    = 0x02, // 3dsMax�̌v�Z���@��K�p����B
    NNS_G3D_TEXMTXMODE_XSI       = 0x03  // XSI�̌v�Z���@��K�p����B
}
NNSG3dTexMtxMode;


/*---------------------------------------------------------------------------*
   Name:        NNSG3dResMdlInfo

   Description: ���f���̕t�����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMdlInfo_
{
    u8         sbcType;       // Structure Byte Code�^�C�v���ʎq(NNSG3dSbcType)
    u8         scalingRule;   // �X�P�[�����O���[�����ʎq(NNSG3dScalingRule)
    u8         texMtxMode;    // �e�N�X�`���s��v�Z�������ʎq(NNSG3dTexMtxMode)
    u8         numNode;

    u8         numMat;
    u8         numShp;
    u8         firstUnusedMtxStackID;
    u8         dummy_;

    // ���_�ʒu���W�ɂ�����X�P�[���l�Ƃ��̋t��
    // FX32_ONE�ȊO�������ꍇ�A�W�I���g���G���W���̃X�P�[���R�}���h�𗘗p����
    // ���_�ʒu���W�����H����B�V�F�C�v�̃f�B�X�v���C���X�g���ōs�񂪃��X�g�A����Ă���
    // (�G���x���[�v���p)�ꍇ�A���X�g�A��AposScale�ŃX�P�[�����O����悤�ɂȂ��Ă���
    fx32       posScale;      
    fx32       invPosScale;

    u16        numVertex;    // IMD�t�@�C��<output_info>::vertex_size�̒l
    u16        numPolygon;   // IMD�t�@�C��<output_info>::polygon_size�̒l
    u16        numTriangle;  // IMD�t�@�C��<output_info>::triangle_size�̒l
    u16        numQuad;      // IMD�t�@�C��<output_info>::quad_size�̒l

    fx16       boxX, boxY, boxZ; // boxPosScale�ł�����Ǝ��ۂ̍��W�ɂȂ�
    fx16       boxW, boxH, boxD; // boxPosScale�ł�����Ǝ��ۂ̍��W�ɂȂ�

    fx32       boxPosScale;
    fx32       boxInvPosScale;
}
NNSG3dResMdlInfo;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �m�[�h
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResDictNodeData

    �m�[�h�������f�[�^�B
    NNSG3dResNodeData�ւ̃I�t�Z�b�g���������Ă���B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictNodeData_
{
    u32       offset;       // &NNSG3dResNodeInfo����̃I�t�Z�b�g�f�[�^
}
NNSG3dResDictNodeData;


/*---------------------------------------------------------------------------*
    NNSG3dSRTFlag

    �m�[�h��SRT�f�[�^�Ɋւ���t���O���
    ���̃t���O�ɂ���Č㑱�ɂǂ̂悤�ȃf�[�^�����݂��邩�����肷��B
    �f�[�^�ʂ̍팸�Ɨ]���Ȍv�Z�̏ȗ���ړI�Ƃ��Ă���B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_SRTFLAG_TRANS_ZERO       = 0x0001, // Trans=(0,0,0), �ړ��f�[�^�̏o�͂��ȗ������B
    NNS_G3D_SRTFLAG_ROT_ZERO         = 0x0002, // Rot=Identity, ��]�s��f�[�^�̏o�͂��ȗ������B
    NNS_G3D_SRTFLAG_SCALE_ONE        = 0x0004, // Scale=(1,1,1), �X�P�[���f�[�^�̏o�͂��ȗ������B
    NNS_G3D_SRTFLAG_PIVOT_EXIST      = 0x0008, // ��]�s�����+1��-1�̗v�f(Pivot)�����݂���B
    NNS_G3D_SRTFLAG_IDXPIVOT_MASK    = 0x00f0, // +1,-1�v�f(Pivot)�̏ꏊ
    NNS_G3D_SRTFLAG_PIVOT_MINUS      = 0x0100, // ON�Ȃ��Pivot��-1, OFF�Ȃ��+1
    NNS_G3D_SRTFLAG_SIGN_REVC        = 0x0200, // �Z�b�g����Ă����C��B�̔��΂̕���
    NNS_G3D_SRTFLAG_SIGN_REVD        = 0x0400, // �Z�b�g����Ă����D��A�̔��΂̕���
    NNS_G3D_SRTFLAG_IDXMTXSTACK_MASK = 0xf800, // 0-30���ƃX�^�b�N�ɂ���,31���ƃX�^�b�N�ɂȂ�

    NNS_G3D_SRTFLAG_IDENTITY         = NNS_G3D_SRTFLAG_TRANS_ZERO |
                                       NNS_G3D_SRTFLAG_ROT_ZERO   |
                                       NNS_G3D_SRTFLAG_SCALE_ONE,

    NNS_G3D_SRTFLAG_IDXPIVOT_SHIFT    = 4,
    NNS_G3D_SRTFLAG_IDXMTXSTACK_SHIFT = 11
}
NNSG3dSRTFlag;


/*---------------------------------------------------------------------------*
    NNSG3dResNodeData

    flag(NNSG3dSRTFlag)�̒l�ɂ���Č㑱�ɂǂ̂悤�ȃf�[�^�����邩���ω�����B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResNodeData_
{
    u16 flag;       // NNSG3dSRTFlag
    fx16 _00;
    // exists if !NNS_G3D_SRT_FLAG_TRANS_ZERO
    //      fx32 Tx, Ty, Tz;
    // exists if !NNS_G3D_SRT_FLAG_ROT_ZERO && !NNS_G3D_SRT_FLAG_PIVOT_EXIST
    //      fx16 _01, _02;
    //      fx16 _10, _11;
    //      fx16 _12, _20;
    //      fx16 _21, _22;
    // exists if !NNS_G3D_SRT_FLAG_ROT_ZERO && NNS_G3D_SRT_FLAG_PIVOT_EXIST
    //      fx16 A, B;
    // exists if !NNS_G3D_SCALE_ONE
    //      fx32 Sx, Sy, Sz;
    //      fx32 InvSx, InvSy, InvSz;
}
NNSG3dResNodeData;


/*---------------------------------------------------------------------------*
    NNSG3dResNodeInfo

    �P�̃��f�������m�[�h�r�q�s���̏W���𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResNodeInfo_
{
    NNSG3dResDict  dict;
    // a seq of NNSG3dResNodeSRTInfo
}
NNSG3dResNodeInfo;


/////////////////////////////////////////////////////////////////////////////////
//
//
// ���f��
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResMdl

    �P�̃��f���Ɋւ���i�e�N�X�`���ȊO�́j�S�Ă̏��𑩂˂�\���� 
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMdl_
{
    u32                size;        // ���f���̃T�C�Y
    u32                ofsSbc;      // &NNSG3dResMdl���N�_�Ƃ���Sbc�ւ̃I�t�Z�b�g
    u32                ofsMat;      // &NNSG3dResMdl���N�_�Ƃ���NNSG3dResMat�ւ̃I�t�Z�b�g
    u32                ofsShp;      // &NNSG3dResMdl���N�_�Ƃ���NNSG3dResShp�ւ̃I�t�Z�b�g
    u32                ofsEvpMtx;   // &NNSG3dResMdl���N�_�Ƃ���NNSG3dEvpMtx�ւ̃I�t�Z�b�g
    NNSG3dResMdlInfo   info;        // ���f�����(�Œ蒷)
    NNSG3dResNodeInfo  nodeInfo;    // �m�[�h���
    // SBC
    // Materials
    // Shapes
    // EvpMtxs
}
NNSG3dResMdl;


/////////////////////////////////////////////////////////////////////////////////
//
//
// ���f���Z�b�g
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResDictMdlSetData

    �X�̃��f���ւ̃I�t�Z�b�g(&NNSG3dResMdlSet���N�_)���i�[���Ă���
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictMdlSetData_
{
    u32 offset;
}
NNSG3dResDictMdlSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResMdlSet

    ���f���Z�b�g�u���b�N�̍\���́B�f�[�^�u���b�N�w�b�_���܂ލ\���̂ł��邱�Ƃɒ���
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMdlSet_
{
    NNSG3dResDataBlockHeader header;
    NNSG3dResDict            dict;
    // a seq of NNSG3dResMdl follows
}
NNSG3dResMdlSet;



/////////////////////////////////////////////////////////////////////////////////
//
//
// Structure Byte Code(Sbc)
//
//

/*---------------------------------------------------------------------------*
    SBC���߃Z�b�g

 *---------------------------------------------------------------------------*/
#define NNS_G3D_SBC_NOP           0x00     // NOP
#define NNS_G3D_SBC_RET           0x01     // RET
#define NNS_G3D_SBC_NODE          0x02     // NODE      idxNode, visibility
#define NNS_G3D_SBC_MTX           0x03     // MTX       stackIdx
#define NNS_G3D_SBC_MAT           0x04     // MAT       idxMat
#define NNS_G3D_SBC_SHP           0x05     // SHP       idxShp
#define NNS_G3D_SBC_NODEDESC      0x06     // NODEDESC  idxNode, idxMtxDest, idxMtxParent
#define NNS_G3D_SBC_BB            0x07
#define NNS_G3D_SBC_BBY           0x08
#define NNS_G3D_SBC_NODEMIX       0x09     // NODEMIX   idxNode, idxMtxDest, numMtx, idxMtx1, ratio1, ..., idxMtxN, ratioN
#define NNS_G3D_SBC_CALLDL        0x0a     // CALLDL    u32(relative), u32(size)
#define NNS_G3D_SBC_POSSCALE      0x0b     // POSSCALE
#define NNS_G3D_SBC_ENVMAP        0x0c
#define NNS_G3D_SBC_PRJMAP        0x0d


#define NNS_G3D_SBC_NOCMD         0x1f     // not exist in compiled structure byte code mnemonic

#define NNS_G3D_SBC_COMMAND_NUM   0x20

#define NNS_G3D_SBCFLG_000        0x00
#define NNS_G3D_SBCFLG_001        0x20
#define NNS_G3D_SBCFLG_010        0x40
#define NNS_G3D_SBCFLG_011        0x60
#define NNS_G3D_SBCFLG_100        0x80
#define NNS_G3D_SBCFLG_101        0xa0
#define NNS_G3D_SBCFLG_110        0xc0
#define NNS_G3D_SBCFLG_111        0xe0

#define NNS_G3D_SBCCMD_MASK       0x1f
#define NNS_G3D_SBCFLG_MASK       0xe0


typedef enum
{
    NNS_G3D_SBC_NODEDESC_FLAG_MAYASSC_APPLY  = 0x01,
    NNS_G3D_SBC_NODEDESC_FLAG_MAYASSC_PARENT = 0x02
}
NNSG3dSbcNodeDescFlag;

#define NNS_G3D_GET_SBCCMD(x) ((x) & NNS_G3D_SBCCMD_MASK)
#define NNS_G3D_GET_SBCFLG(x) ((u32)((x) & NNS_G3D_SBCFLG_MASK))


/////////////////////////////////////////////////////////////////////////////////
//
//
// �A�j���[�V����
//
//

//
// REQUIREMENT:
// �A�j���[�V�����t�@�C���̃u���b�N�͂P�ŁANNSG3dResAnmSet����f�B�N�V���i������A
// NNSG3dResDictAnmSetData��ʂ���NNSG3dResAnmHeader��擪�Ɏ��e�A�j���[�V������
// �\���̂ɒB���邱�Ƃ��ł��Ȃ��Ă͂Ȃ�Ȃ��B
//

/*---------------------------------------------------------------------------*
    NNSG3dResAnmHeader
    
    �e�A�j���[�V�������\�[�X�̐擪�ɂ���w�b�_���
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResAnmHeader_
{
    // 'M' for material animation
    // 'J' for joint animation
    // 'V' for visibility animation
    u8  category0;
    u8  revision;
    // 'CA' for character(joint) animation
    // 'VA' for visibility animation
    // 'MA' for material color animation
    // 'TP' for texture pattern animation
    // 'TA' for texture SRT animation
    u16  category1; 
}
NNSG3dResAnmHeader;

/*---------------------------------------------------------------------------*
    NNSG3dResAnmCommon

    �S�ẴA�j���[�V�����̓w�b�_��numFrame�������Ȃ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
typedef struct
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u16                dummy_;
}
NNSG3dResAnmCommon;


/*---------------------------------------------------------------------------*
    NNSG3dResDictAnmSetData

    (u8*)&NNSG3dResAnmSet+offset�ŌX�̃A�j���[�V�����ɒB���邱�Ƃ��ł���B
 *---------------------------------------------------------------------------*/
typedef struct
{
    u32 offset;                      // �X�̃A�j���[�V�����ɒB���邽�߂̃I�t�Z�b�g���i�[����Ă���B
}
NNSG3dResDictAnmSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResAnmSet

    �A�j���[�V�����̏W���𑩂˂邽�߂̍\���́B
 *---------------------------------------------------------------------------*/
typedef struct
{
    NNSG3dResDataBlockHeader header;
    NNSG3dResDict            dict;   // �t�@�C����->�eAnmSetData
}
NNSG3dResAnmSet;



/////////////////////////////////////////////////////////////////////////////////
//
//
// �r�W�r���e�B�A�j���[�V����
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResVisAnm

    �r�W�r���e�B�A�j���[�V����1���̃f�[�^�𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResVisAnm_
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u16                numNode;
    u16                size;
    u16                dummy_;
    // variable length
    u32                visData[1];
}
NNSG3dResVisAnm;

/*---------------------------------------------------------------------------*
    NNSG3dResDictVisAnmSetData

    �X�̃r�W�r���e�B�A�j���[�V����(NNSG3dResVisAnm)�̃f�[�^�ւ̃I�t�Z�b�g���i�[����Ă���B
    &NNSG3dResVisAnmSet���N�_�Ƃ���B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResDictAnmSetData NNSG3dResDictVisAnmSetData;

/*---------------------------------------------------------------------------*
    NNSG3dResVisAnmSet

    �r�W�r���e�B�A�j���[�V�����̃Z�b�g�𑩂˂�\����
    ����ɂ�NNSG3dResVisAnm������ł���B
    ������NNSG3dResDictVisAnmSetData�o�R��NNSG3dResVisAnm���Q�Ƃ��鎫��
    kind==NNS_G3D_SIGNATURE_NSBVA�łȂ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResAnmSet NNSG3dResVisAnmSet;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �}�e���A���J���[�A�j���[�V����
//
//

/*---------------------------------------------------------------------------*
    NNSG3dMatCElem

    NNSG3dResDictMatCAnmData�̊e�f�[�^�����o�̒l
    NNS_G3D_MATCANM_ELEM_CONST���Z�b�g����Ă���΁A����16bit�͒萔�J���[�l�A
    �Z�b�g����Ă��Ȃ���΁A�J���[�f�[�^��ւ̃I�t�Z�b�g�A�ƂȂ�B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_MATCANM_ELEM_CONST                 = 0x20000000, // �f�[�^���A�j���[�V�������Ȃ��ꍇ��ON
    NNS_G3D_MATCANM_ELEM_STEP_1                = 0x00000000, // �f�[�^�����t���[������Ƃ�
    NNS_G3D_MATCANM_ELEM_STEP_2                = 0x40000000, // �f�[�^��2�t���[�����ɂ���Ƃ�
    NNS_G3D_MATCANM_ELEM_STEP_4                = 0x80000000, // �f�[�^��4�t���[�����ɂ���Ƃ�

    NNS_G3D_MATCANM_ELEM_STEP_MASK             = 0xc0000000,
    NNS_G3D_MATCANM_ELEM_LAST_INTERP_MASK      = 0x1fff0000,
    NNS_G3D_MATCANM_ELEM_OFFSET_CONSTANT_MASK  = 0x0000ffff,

    NNS_G3D_MATCANM_ELEM_OFFSET_CONSTANT_SHIFT = 0,
    NNS_G3D_MATCANM_ELEM_LAST_INTERP_SHIFT     = 16
}
NNSG3dMatCElem;


/*---------------------------------------------------------------------------*
    NNSG3dResDictMatCAnmData

    �}�e���A���J���[�A�j���[�V�����̊e�v�f�ɂ��Ă̏��
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictMatCAnmData_
{
    // NNSG3dMatCElem:
    // 31  30 29      28         16 15               0
    // frmStp isConst iFrame_interp offset/constantCol
    u32 diffuse;
    u32 ambient;
    u32 specular;
    u32 emission;
    u32 polygon_alpha;
}
NNSG3dResDictMatCAnmData;


/*---------------------------------------------------------------------------*
    NNSG3dMatCAnmOption

    �}�e���A���J���[�A�j���[�V�����̃I�v�V����(���ԃt�@�C���Ŏw��)
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_MATCANM_OPTION_INTERPOLATION              = 0x0001, // <mat_color_info>::interpolation�ɑΉ�
    NNS_G3D_MATCANM_OPTION_END_TO_START_INTERPOLATION = 0x0002  // <mat_color_info>::interp_end_to_start�ɑΉ�
}
NNSG3dMatCAnmOption;


/*---------------------------------------------------------------------------*
    NNSG3dResMatCAnm

    �}�e���A���J���[�A�j���[�V����1���̃f�[�^�𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResMatCAnm_
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u16                flag;     // NNSG3dMatCAnmOption
    NNSG3dResDict      dict;
}
NNSG3dResMatCAnm;


/*---------------------------------------------------------------------------*
    NNSG3dResDictMatCAnmSetData

    �X�̃}�e���A���J���[�A�j���[�V�����̃f�[�^�ւ̃I�t�Z�b�g���i�[����Ă���B
    &NNSG3dResMatCAnmSet���N�_�Ƃ���B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResDictAnmSetData NNSG3dResDictMatCAnmSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResMatCAnmSet

    �}�e���A���J���[�A�j���[�V�����̃Z�b�g�𑩂˂�\����
    ����ɂ�NNSG3dResMatCAnm������ł���B
    ������NNSG3dResDictMatCAnmSetData�o�R��NNSG3dResMatCAnm���Q�Ƃ��鎫��
    kind = NNS_G3D_DATABLK_MATC_ANM�łȂ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResAnmSet NNSG3dResMatCAnmSet;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �e�N�X�`���p�^�[���A�j���[�V����
//
//


/*---------------------------------------------------------------------------*
    NNSG3dResTexPatAnmFV

    �t���[��->(�e�N�X�`��, �p���b�g)�f�[�^�ł��B
    ���̃f�[�^�̕��т��e�N�X�`���p�^���A�j���[�V�������\�����܂��B
    idxFrame����e�N�X�`����idTex��,�p���b�g��idPltt�ɐ؂�ւ��A
    �Ƃ����Ӗ��ɂȂ�܂��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTexPatAnmFV_
{
    u16 idxFrame;
    u8  idTex;
    u8  idPltt;
}
NNSG3dResTexPatAnmFV;


/*---------------------------------------------------------------------------*
    NNSG3dTexPatAnmOption

    �e�N�X�`���p�^�[���A�j���[�V�����̃I�v�V�����ł��B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_TEXPATANM_OPTION_NOPLTT = 0x0001    // �p���b�g�̓A�j���[�V�������Ȃ��ꍇ��ON
}
NNSG3dTexPatAnmOption;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexPatAnmData

    �e�}�e���A�����Ɏ��e�N�X�`���p�^�[���A�j���[�V�����̃f�[�^�ł��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictTexPatAnmData_
{
    u16  numFV;           // NNSG3dResTexPatAnmFV�̐�
    u16  flag;            // NNSG3dTexPatAnmOption (���݂͎g�p����Ă��܂���)
    fx16 ratioDataFrame;  // numFV / numFrame�ł��B�f�[�^�T���p�̃q���g�ł��B
    u16  offset;          // &NNSG3dResTexPatAnm���N�_�Ƃ���FV�f�[�^�ւ̃I�t�Z�b�g
}
NNSG3dResDictTexPatAnmData;


/*---------------------------------------------------------------------------*
    NNSG3dResTexPatAnm

    �e�N�X�`���p�^�[���A�j���[�V����1���̃f�[�^�𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTexPatAnm_
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u8                 numTex;       // �g�p�e�N�X�`���̐�
    u8                 numPltt;      // �g�p�p���b�g�̐�
    u16                ofsTexName;   // �e�N�X�`�����z��ւ̃I�t�Z�b�g(NNSG3dResName��numTex����ł���)
    u16                ofsPlttName;  // �p���b�g���z��ւ̃I�t�Z�b�g(NNSG3dResName��numPltt����ł���)
    NNSG3dResDict      dict;         // �}�e���A��������NNSG3dResDictTexPatAnmData���Ђ�����
    // Frame-Value data
    // �e�N�X�`�����z��
    // �p���b�g���z��
}
NNSG3dResTexPatAnm;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexPatAnmSetData

    �X�̃e�N�X�`���p�^�[���A�j���[�V�����̃f�[�^�ւ̃I�t�Z�b�g���i�[����Ă���B
    &NNSG3dResTexPatAnmSet���N�_�Ƃ���B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResDictAnmSetData NNSG3dResDictTexPatAnmSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResTexPatAnmSet

    �e�N�X�`���p�^�[���A�j���[�V�����̃Z�b�g�𑩂˂�\����
    ����ɂ�NNSG3dResTexPatAnm������ł���B
    ������NNSG3dResDictTexPatAnmSetData�o�R��NNSG3dResTexPatAnm���Q�Ƃ��鎫��
    kind = NNS_G3D_DATABLK_TEXPAT_ANM�łȂ��Ă͂Ȃ�Ȃ�
 *---------------------------------------------------------------------------*/
typedef NNSG3dResAnmSet NNSG3dResTexPatAnmSet;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �e�N�X�`��SRT�A�j���[�V����
//
//

/*---------------------------------------------------------------------------*
    NNSG3dTexSRTElem

    �e�N�X�`��SRT�A�j���[�V�����̊e��t���O�B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_TEXSRTANM_ELEM_FX16   = 0x10000000, // �f�[�^��fx16�̃x�N�^�Ŏ��ꍇ��ON(rot�̂Ƃ��͏��OFF)
    NNS_G3D_TEXSRTANM_ELEM_CONST  = 0x20000000, // �f�[�^���A�j���[�V�������Ȃ��ꍇ��ON
    NNS_G3D_TEXSRTANM_ELEM_STEP_1 = 0x00000000, // �f�[�^�����t���[������Ƃ�
    NNS_G3D_TEXSRTANM_ELEM_STEP_2 = 0x40000000, // �f�[�^��2�t���[�����ɂ���Ƃ�
    NNS_G3D_TEXSRTANM_ELEM_STEP_4 = 0x80000000, // �f�[�^��4�t���[�����ɂ���Ƃ�

    NNS_G3D_TEXSRTANM_ELEM_STEP_MASK         = 0xc0000000,
    NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK  = 0x0000ffff,

    NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_SHIFT = 0
}
NNSG3dTexSRTElem;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexSRTAnmData

    �e�}�e���A�����̃e�N�X�`���r�q�s�A�j���[�V�����̏��
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResDictTexSRTAnmData_
{
    // word #0(NNSG3dTexSRTElem)
    // 31  30 29      28     15          0
    // frmStp isConst isFx16 iFrame_interp 
    // word #1
    // 31               0
    // offset/constantVal
    u32 scaleS;             // NNSG3dTexSRTElem
    u32 scaleSEx;           // NNS_G3D_TEXSRTANM_ELEM_CONST�̂Ƃ��͒萔�l�A����ȊO�̏ꍇ�̓f�[�^��ւ̃I�t�Z�b�g

    u32 scaleT;             // NNSG3dTexSRTElem
    u32 scaleTEx;           // NNS_G3D_TEXSRTANM_ELEM_CONST�̂Ƃ��͒萔�l�A����ȊO�̏ꍇ�̓f�[�^��ւ̃I�t�Z�b�g

    u32 rot;                // NNSG3dTexSRTElem
    u32 rotEx;              // NNS_G3D_TEXSRTANM_ELEM_CONST�̂Ƃ��͒萔�l�A����ȊO�̏ꍇ�̓f�[�^��ւ̃I�t�Z�b�g
                            //
                            // rot �� cos �l sin �l���y�A�ƂȂ��Ċi�[����Ă���
                            // 31          15          0
                            // cos in fx16   sin in fx16
                            
                            
    u32 transS;             // NNSG3dTexSRTElem
    u32 transSEx;           // NNS_G3D_TEXSRTANM_ELEM_CONST�̂Ƃ��͒萔�l�A����ȊO�̏ꍇ�̓f�[�^��ւ̃I�t�Z�b�g

    u32 transT;             // NNSG3dTexSRTElem
    u32 transTEx;           // NNS_G3D_TEXSRTANM_ELEM_CONST�̂Ƃ��͒萔�l�A����ȊO�̏ꍇ�̓f�[�^��ւ̃I�t�Z�b�g
}
NNSG3dResDictTexSRTAnmData;


/*---------------------------------------------------------------------------*
    NNSG3dTexSRTAnmOption

    �e�N�X�`��SRT�A�j���[�V�����̃I�v�V����(���ԃt�@�C���Ŏw��)
 *---------------------------------------------------------------------------*/
typedef enum NNSG3dTexSRTAnmOption_
{
    NNS_G3D_TEXSRTANM_OPTION_INTERPOLATION              = 0x01, // <tex_srt_info>::interpolation�ɑΉ�
    NNS_G3D_TEXSRTANM_OPTION_END_TO_START_INTERPOLATION = 0x02  // <tex_srt_info>::interp_end_to_start�ɑΉ�
}
NNSG3dTexSRTAnmOption;


/*---------------------------------------------------------------------------*
    NNSG3dResTexSRTAnm

    �e�N�X�`��SRT�A�j���[�V����1���̃f�[�^�𑩂˂�\����
 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResTexSRTAnm_
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u8                 flag;             // NNSG3dTexSRTAnmOption
    u8                 texMtxMode;       // NNSG3dTexMtxMode
    NNSG3dResDict      dict;             // �}�e���A����->NNSG3dResDictTexSRTAnmData�̎���
}
NNSG3dResTexSRTAnm;


/*---------------------------------------------------------------------------*
    NNSG3dResDictTexSRTAnmSetData

    �X�̃e�N�X�`���r�q�s�A�j���[�V�����̃f�[�^�ւ̃I�t�Z�b�g���i�[����Ă���B
    &NNSG3dResTexSRTAnmSet���N�_�Ƃ���B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResDictAnmSetData NNSG3dResDictTexSRTAnmSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResTexSRTAnmSet

    �e�N�X�`��SRT�A�j���[�V�����̃Z�b�g�𑩂˂�\����
    ����ɂ�NNSG3dResTexSRTAnm������ł���B
    ������NNSG3dResDictTexSRTAnmSetData�o�R��NNSG3dResTexSRTAnm���Q�Ƃ��鎫��
    kind == NNS_G3D_SIGNATURE_NSBTA�łȂ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResAnmSet NNSG3dResTexSRTAnmSet;


/////////////////////////////////////////////////////////////////////////////////
//
//
// �W���C���g�A�j���[�V����
//
//

/*---------------------------------------------------------------------------*
    NNSG3dResJntAnmSRT�Ƃ��̒��Ԃ���

    �W���C���g�P��SRT�A�j���[�V���������L�q����
    info�̂ǂ̃r�b�g��ON���ɂ���Ă��̌�̃��������C�A�E�g�����肷��B
    �㑱�f�[�^��TRS�̏��Ԃɕ���ł���B
    
    NNS_G3D_JNTANM_SRTINFO_IDENTITY
        --> �ȍ~�̃f�[�^�͑��݂��Ȃ��B
        NNS_G3D_JNTANM_SRTINFO_IDENTITY_T -- Trans = (0, 0, 0)
        NNS_G3D_JNTANM_SRTINFO_BASE_T     -- ���f����Trans���Q��
            --> Translation�̃f�[�^�͑��݂��Ȃ��B
            NNS_G3D_JNTANM_SRTINFO_CONST_TX -- Trans��X������const
                --> 1wd����Tx�f�[�^���擾 / otherwise NNSG3dJntAnmTInfo(2wd)
            NNS_G3D_JNTANM_SRTINFO_CONST_TY -- Trans��Y������const
                --> 1wd����Ty�f�[�^���擾 / otherwise NNSG3dJntAnmTInfo(2wd)
            NNS_G3D_JNTANM_SRTINFO_CONST_TZ -- Trans��Z������const
                --> 1wd����Tz�f�[�^���擾 / otherwise NNSG3dJntAnmTInfo(2wd)
        NNS_G3D_JNTANM_SRTINFO_IDENTITY_R -- Rot = I
        NNS_G3D_JNTANM_SRTINFO_BASE_R     -- ���f����Rot���Q��
            --> Rotation�̃f�[�^�͑��݂��Ȃ�
            NNS_G3D_JNTANM_SRTINFO_CONST_R -- Rot��const
                --> 1wd����RIdx�ւ̃C���f�b�N�X�f�[�^ / otherwise NNSG3dJntAnmRInfo(2wd)
        NNS_G3D_JNTANM_SRTINFO_IDENTITY_S -- Scale = (1, 1, 1)
        NNS_G3D_JNTANM_SRTINFO_BASE_S     -- ���f����Scale���Q��
            NNS_G3D_JNTANM_SRTINFO_CONST_SX -- Scale��X������const
                --> 2wd����Sx,InvSx�f�[�^���擾 / otherwise NNSG3dJntAnmSInfo(2wd)
            NNS_G3D_JNTANM_SRTINFO_CONST_SY -- Scale��Y������const
                --> 2wd����Sy,InvSy�f�[�^���擾 / otherwise NNSG3dJntAnmSInfo(2wd)
            NNS_G3D_JNTANM_SRTINFO_CONST_SZ -- Scale��Z������const
                --> 2wd����Sz,InvSz�f�[�^���擾 / otherwise NNSG3dJntAnmSInfo(2wd)

    �ȉ��ł̓I�t�Z�b�g�́ASRT�f�[�^�̈悩��̃I�t�Z�b�g�ƂȂ�B
    NNSG3dJntAnmTInfo
        NNS_G3D_JNTANM_TINFO_FX16ARRAY�Ȃ�΁A
        �I�t�Z�b�g�ŎQ�Ƃ���Ă���̂�fx16�^�̔z��ł���

    NNSG3dJntAnmRInfo
        �I�t�Z�b�g�ŎQ�Ƃ���Ă���̂�RotIdx�z��ł���B
        RotIdx������f�[�^�ɃA�N�Z�X���邱�ƂɂȂ�B

    NNSG3dJntAnmSInfo

    RotIdx�z��
        16bit�̒l�̕��тŁA0x8000��ON�Ȃ��Rot3�z��(���k3x3�s��)�ւ̃C���f�b�N�X
        OFF�Ȃ��Rot9�z��ւ̃C���f�b�N�X
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    NNSG3dJntAnmSRTTag

    SRT�f�[�^�̐������L�q����Ă���
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_SRTINFO_IDENTITY    = 0x00000001,    // SRT�ɉ����ύX���Ȃ��Ƃ�ON
    NNS_G3D_JNTANM_SRTINFO_IDENTITY_T  = 0x00000002,    // Trans��(0,0,0)�̂܂܂̂Ƃ�ON
    NNS_G3D_JNTANM_SRTINFO_BASE_T      = 0x00000004,    // Trans�Ƀ��f���̒l���g���Ƃ�ON
    NNS_G3D_JNTANM_SRTINFO_CONST_TX    = 0x00000008,    // Tx���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_CONST_TY    = 0x00000010,    // Ty���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_CONST_TZ    = 0x00000020,    // Tz���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_IDENTITY_R  = 0x00000040,    // Rot���Ȃ��ꍇON
    NNS_G3D_JNTANM_SRTINFO_BASE_R      = 0x00000080,    // Rot�Ƀ��f���̒l���g���Ƃ�ON
    NNS_G3D_JNTANM_SRTINFO_CONST_R     = 0x00000100,    // Rot���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_IDENTITY_S  = 0x00000200,    // Scale��������Ȃ��ꍇON
    NNS_G3D_JNTANM_SRTINFO_BASE_S      = 0x00000400,    // Scale�Ƀ��f���̒l���g���ꍇON
    NNS_G3D_JNTANM_SRTINFO_CONST_SX    = 0x00000800,    // Sx���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_CONST_SY    = 0x00001000,    // Sy���萔�̏ꍇON
    NNS_G3D_JNTANM_SRTINFO_CONST_SZ    = 0x00002000,    // Sz���萔�̏ꍇON
    
    NNS_G3D_JNTANM_SRTINFO_NODE_MASK   = 0xff000000,    // �m�[�hID������B�S�Ẵm�[�h�Œ�`����Ă��Ȃ��Ă������悤�ɂ��Ă���
    NNS_G3D_JNTANM_SRTINFO_NODE_SHIFT  = 24
}
NNSG3dJntAnmSRTTag;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmTInfo

    Trans�f�[�^�ɂ��Ă̏��
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_TINFO_STEP_1            = 0x00000000,    // �f�[�^�����t���[������Ƃ�
    NNS_G3D_JNTANM_TINFO_STEP_2            = 0x40000000,    // �f�[�^��2�t���[�����ɂ���Ƃ�
    NNS_G3D_JNTANM_TINFO_STEP_4            = 0x80000000,    // �f�[�^��4�t���[�����ɂ���Ƃ�
    NNS_G3D_JNTANM_TINFO_FX16ARRAY         = 0x20000000,    // �A�j���[�V�����f�[�^��fx16�̔z��̏ꍇON

    NNS_G3D_JNTANM_TINFO_LAST_INTERP_MASK  = 0x1fff0000,
    NNS_G3D_JNTANM_TINFO_STEP_MASK         = 0xc0000000,

    NNS_G3D_JNTANM_TINFO_LAST_INTERP_SHIFT = 16,
    NNS_G3D_JNTANM_TINFO_STEP_SHIFT        = 30
}
NNSG3dJntAnmTInfo;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmRInfo

    ��]�f�[�^�ɂ��Ă̏��
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_RINFO_STEP_1            = 0x00000000,    // �f�[�^�����t���[������Ƃ�
    NNS_G3D_JNTANM_RINFO_STEP_2            = 0x40000000,    // �f�[�^��2�t���[�����ɂ���Ƃ�
    NNS_G3D_JNTANM_RINFO_STEP_4            = 0x80000000,    // �f�[�^��4�t���[�����ɂ���Ƃ�

    NNS_G3D_JNTANM_RINFO_LAST_INTERP_MASK  = 0x1fff0000,
    NNS_G3D_JNTANM_RINFO_STEP_MASK         = 0xc0000000,

    NNS_G3D_JNTANM_RINFO_LAST_INTERP_SHIFT = 16,
    NNS_G3D_JNTANM_RINFO_STEP_SHIFT        = 30
}
NNSG3dJntAnmRInfo;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmPivotInfo

    ���k���ꂽ��]�s��ɂ��Ă̏��
    +1/-1�ɂȂ��Ă���v�f�̍s�E�����菜���ƁA2x2�s��ɂȂ��āA������
    A B
    C D
    �̂悤�ɖ��Â���BC=+B/-B D=+A/-A�ƂȂ�̂ŁA
    NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVC
    NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVD
    �ɕ����̏��������Ă����B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_PIVOTINFO_IDXPIVOT_MASK = 0x000f,    // +1/-1�ɂȂ��Ă���v�f�̏ꏊ
    NNS_G3D_JNTANM_PIVOTINFO_MINUS         = 0x0010,    // ON�Ȃ�-1/ OFF�Ȃ�+1
    NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVC     = 0x0020,    // C=-B�Ȃ�ON
    NNS_G3D_JNTANM_PIVOTINFO_SIGN_REVD     = 0x0040,    // D=-A�Ȃ�ON

    NNS_G3D_JNTANM_PIVOT_INFO_IDXPIVOT_SHIFT = 0
}
NNSG3dJntAnmPivotInfo;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmRIdx

    ��]�s��f�[�^�ւ̃C���f�b�N�X
    NNS_G3D_JNTANM_RIDX_PIVOT��ON�̂Ƃ��́Apivot���k�s��ւ̃C���f�b�N�X
    OFF�̏ꍇ�́A�ʏ�̉�]�s��f�[�^�ւ̃C���f�b�N�X�ƂȂ�B
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_RIDX_PIVOT         = 0x8000,
    NNS_G3D_JNTANM_RIDX_IDXDATA_MASK  = 0x7fff,
    NNS_G3D_JNTANM_RIDX_IDXDATA_SHIFT = 0
}
NNSG3dJntAnmRIdx;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmSInfo


 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_SINFO_STEP_1            = 0x00000000,    // �f�[�^�����t���[������Ƃ�
    NNS_G3D_JNTANM_SINFO_STEP_2            = 0x40000000,    // �f�[�^��2�t���[�����ɂ���Ƃ�
    NNS_G3D_JNTANM_SINFO_STEP_4            = 0x80000000,    // �f�[�^��4�t���[�����ɂ���Ƃ�
    NNS_G3D_JNTANM_SINFO_FX16ARRAY         = 0x20000000,    // �f�[�^�l��fx16�^�̂Ƃ�

    NNS_G3D_JNTANM_SINFO_LAST_INTERP_MASK  = 0x1fff0000,
    NNS_G3D_JNTANM_SINFO_STEP_MASK         = 0xc0000000,

    NNS_G3D_JNTANM_SINFO_LAST_INTERP_SHIFT = 16,
    NNS_G3D_JNTANM_SINFO_STEP_SHIFT        = 30
}
NNSG3dJntAnmSInfo;


/*---------------------------------------------------------------------------*
    NNSG3dResJntAnmSRTInfo


 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResJntAnmSRTInfo_
{
    u32 info;    // NNSG3dJntAnmTInfo �� NNSG3dJntAnmRInfo �� NNSG3dJntAnmSInfo
    u32 offset;
}
NNSG3dResJntAnmSRTInfo;


/*---------------------------------------------------------------------------*
    NNSG3dResJntAnmSRTTag


 *---------------------------------------------------------------------------*/
typedef struct NNSG3dResJntAnmSRTTag_
{
    u32 tag;    // NNSG3dJntAnmSRTTag;
}
NNSG3dResJntAnmSRTTag;


/*---------------------------------------------------------------------------*
    NNSG3dJntAnmOption

    �W���C���g�A�j���[�V�����̃I�v�V����(���ԃt�@�C���Ŏw��)
 *---------------------------------------------------------------------------*/
typedef enum
{
    NNS_G3D_JNTANM_OPTION_INTERPOLATION              = 0x01, // <node_anm_info>::interpolation
    NNS_G3D_JNTANM_OPTION_END_TO_START_INTERPOLATION = 0x02  // <node_anm_info>::interp_end_to_start
}
NNSG3dJntAnmOption;


typedef struct NNSG3dResJntAnm_
{
    NNSG3dResAnmHeader anmHeader;
    u16                numFrame;
    u16                numNode;
    u32                flag;
    u32                ofsRot3;
    u32                ofsRot5;
    // u16[numNode] ofsTag
    // vector of NNSG3dResJntAnmSRTTag
}
NNSG3dResJntAnm;


/*---------------------------------------------------------------------------*
    NNSG3dResDictJntAnmSetData

    �X�̃W���C���g�A�j���[�V����(NNSG3dResJntAnm)�̃f�[�^�ւ̃I�t�Z�b�g���i�[����Ă���B
    &NNSG3dResJntAnmSet���N�_�Ƃ���B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResDictAnmSetData NNSG3dResDictJntAnmSetData;


/*---------------------------------------------------------------------------*
    NNSG3dResJntAnmSet

    �W���C���g�A�j���[�V�����̃Z�b�g�𑩂˂�\����
    ����ɂ�NNSG3dResJntAnm������ł���B
    ������NNSG3dResDictJntAnmSetData�o�R��NNSG3dResJntAnm���Q�Ƃ��鎫��
    kind==NNS_G3D_SIGNATURE_NSBCA�łȂ��Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
typedef NNSG3dResAnmSet NNSG3dResJntAnmSet;


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // NNSG3D_BINRES_RES_STRUCT_H_
