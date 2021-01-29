===========================================================================
Stationery for Nitro SDK 3.0
Japanese description is available at the second half of this file.
===========================================================================

This stationery uses precompiled headers.

1. The files and settings relating to the use of the precompiled header.

 - Nitro_TS.pch
    This is the pch file to generate a mch file which is used by the C source.
    The actual mch file name depends on the build target you chose.
    "nitro_TS_prefix.h" and "nitro.h" are included in the precompiled header.
    If you want to include other header files in the precompiled header, please
    add the files to the bottom of this pch file.

 - Nitro_TS.pch++
    This is the pch file to generate a mch file which is used by the C++ source.
    The actual mch file name depends on the build target you chose.
    "nitro_TS_prefix.h" and "nitro.h" are included in the precompiled header.
    If you want to include other header files in the precompiled header, please
    add the files to the bottom of this pch file.

 - Nitro_TS.h
    This is the header file for each C and C++ source file to include
    the proper precompiled header.
    This header file is included from the Prefix Text field in the C/C++
    Preprocessor panel thus making it automatically included in each
    C/C++ source file.

 - The settings in the C/C++ Preprocessor panel
   The settings in the C/C++ Preprocessor panel ("Target Settings"
     -> "Language Settings" -> "C/C++ Preprocessor") is the following.
    1) "Nitro_TS.h" is included by the Prefix Text.
    2) "nitro_TS_prefix.h" is not included by the Prefix Text.
    3) "Use prefix text in precompiled header" is checked.


2. The behavior when building is as follows.

  1) Nitro_TS.pch (or Nitro_TS.pch++) is precompiled.
     The mch file with the proper file name is generated.
  2) Each C source and each C++ source is compiled.
     Nitro_TS.h is included by the Prefix Text in the C/C++
     Preprocessor panel.
     The proper mch file is included.


3. If you don't want to use precompiled headers, please follow the steps below.

  1) Remove Nitro_TS.pch from Project.
  2) Remove Nitro_TS.pch++ from Project.
  3) Remove Nitro_TS.h from Project.
  4) Change the contents of Prefix Text field on C/C++ Preprocessor panel.
     - Remove the line " #include "Nitro_TS.h" ".
     - Remove the comment out of the line "#include <nitro_TS_prefix.h>" to
       make this #include effective.


4. You may be able to remove Nitro_TS.pch or Nitro_TS.pch++.

  Both of Nitro_TS.pch and Nitro_TS.pch++ are used in both the stationery
  for C and C++ Project because both C and C++ source may be used 
  in one Project.

   - If the precompiled header for C++ is unnecessary in your C Project,
     you can remove the Nitro_TS.pch++ from your Project.
   - If the precompiled header for C is unnecessary in your C++ Project,
     you can remove the Nitro_TS.pch from your Project.


/*--- The lines below are written in Japanese character set ---*/
===========================================================================
Nitro SDK 3.0 �p�X�e�[�V���i��
�v���R���p�C���w�b�_�̎g�p�ɂ���
===========================================================================

���̃X�e�[�V���i���ł̓v���R���p�C���w�b�_���g�p���Ă��܂��B

�P�D�v���R���p�C���w�b�_�̎g�p�Ɋ֘A�����ݒ�A�t�@�C��

 �E Nitro_TS.pch
    C�\�[�X���g��mch�t�@�C���𐶐�����ׂ�pch�t�@�C���ł��B
    ���������mch�t�@�C���̃t�@�C�����̓r���h�^�[�Q�b�g�ɂ���ĈقȂ�܂��B
    �v���R���p�C���w�b�_�ɂ� nitro_TS_prefix.h �� nitro.h ���܂߂Ă��܂��B
    ���[�U�[���p�ӂ����w�b�_�t�@�C�����܂߂�ꍇ�́A���̃t�@�C���̖�����
    �ǋL���ĉ������B

  �E Nitro_TS.pch++
    C++�\�[�X���g��mch�t�@�C���𐶐�����ׂ�pch�t�@�C���ł��B
    ���������mch�t�@�C���̃t�@�C�����̓r���h�^�[�Q�b�g�ɂ���ĈقȂ�܂��B
    �v���R���p�C���w�b�_�ɂ� nitro_TS_prefix.h �� nitro.h ���܂߂Ă��܂��B
    ���[�U�[���p�ӂ����w�b�_�t�@�C�����܂߂�ꍇ�́A���̃t�@�C���̖�����
    �ǋL���ĉ������B

  �E Nitro_TS.h
    �eC�\�[�X����C++�\�[�X���K�؂ȃv���R���p�C���w�b�_���C���N���[�h����
    �ׂ̃w�b�_�t�@�C���ł��B
    �eC�\�[�X����C++�\�[�X�̃R���p�C�����ɕK�����̃w�b�_�t�@�C�����g�p
    �����悤�ɁA�^�[�Q�b�g�ݒ�p�l�� �| ����ݒ� �| C/C++ Preprocessor ��
    Prefix Text �ŃC���N���[�h����Ă��܂��B

 �E C/C++ Preprocessor �p�l�����̐ݒ�
    �^�[�Q�b�g�ݒ�p�l�� �| ����ݒ� �| C/C++ Preprocessor �ɂ����Ď��̐ݒ�
    ������Ă��܂��B
    1) Nitro_TS.h �� Prefix Text �ŃC���N���[�h����
    2) nitro_TS_prefix.h �� Prefix Text �ł̓C���N���[�h���Ȃ�
    3) Use prefix text in precompiled header �`�F�b�N�{�b�N�X���I��


�Q�D�r���h���̓����͉��L�̂悤�ɂȂ�܂��B

  1) Nitro_TS.pch �i�܂���Nitro_TS.pch++�j���v���R���p�C������A�r���h
     �^�[�Q�b�g�ɉ������K�؂ȃt�@�C������mch�t�@�C�������������B
  2) �eC�\�[�X����C++�\�[�X���R���p�C�������B
     ���̎��AC/C++ Preprocessor�p�l���� Prefix Text�̎w��ɂ��ANitro_TS.h
     ���ÖٓI�ɃC���N���[�h����A�K�؂ȃt�@�C������mch�t�@�C�����C���N���[�h
     �����B


�R�D�v���R���p�C���w�b�_���g�p���Ȃ��\���ɕύX����ɂ́A���L�̎菇���s����
    �������B

  1) �v���W�F�N�g���� Nitro_TS.pch ���폜����
  2) �v���W�F�N�g���� Nitro_TS.pch++ ���폜����
  3) �v���W�F�N�g���� Nitro_TS.h ���폜����
  4) �^�[�Q�b�g�ݒ�p�l�� �| ����ݒ� �| C/C++ Preprocessor �� Prefix Text ��
     ���e�ɂ��āA���L�̕ύX���s��
     �E�u#include "Nitro_TS.h"�v�̍s���폜����B
     �E�u#include <nitro_TS_prefix.h>�v�̍s�̃R�����g�A�E�g���������A
         ���̃C���N���[�h��L���ɂ���B


�S�D�s�v�� Nitro_TS.pch ���� Nitro_TS.pch++ �ɂ��āB

  C�̃v���W�F�N�g�p�̃X�e�[�V���i���y��C++�v���W�F�N�g�p�̃X�e�[�V���i��
  �̗����ɂ����āAC�\�[�X�y��C++�\�[�X�����݉\�Ƃ���ׂɁANitro_TS.pch ��
  Nitro_TS.pch++ �̗������g�ݍ��܂�Ă��܂��B

  �����AC�̃v���W�F�N�g��C++�p�̃v���R���p�C���w�b�_���S���s�v�ł����
  ��΁A�v���W�F�N�g���� Nitro_TS.pch++ ���폜���Ē����Ă��܂��܂���B
  ���l�ɁAC++�̃v���W�F�N�g��C�p�̃v���R���p�C���w�b�_���S���s�v�ł���
  �Ȃ�΁A�v���W�F�N�g���� Nitro_TS.pch ���폜���Ē����Ă��܂��܂���B


