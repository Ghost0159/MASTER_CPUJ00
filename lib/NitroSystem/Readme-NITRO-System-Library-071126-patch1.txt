������������������������������������������������������������������������������
���@Readme-NITRO-System-Library-071126-patch1.txt �@�@�@�@�@�@�@�@�@�@�@�@�@��
���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
���@NITRO-System-Library 071126 patch1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
���@Jan. 23, 2008 �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
������������������������������������������������������������������������������


�ڎ�
==============================================================================
�@�@1. �͂��߂�
�@�@2. �C�����e
�@�@3. �t�@�C�����X�g


1. �͂��߂�
==============================================================================
�@�@���̃p�b�`�́ANITRO-System Library 2007/11/26 �ł̃����[�X�ȍ~�ɔ������ꂽ
�@�@�s��̏C���ƁANITRO-SDK4.2�ւ̑Ή����s�����߂̂��̂ł��B
�@�@�ipatch-NitroSDK4_2�ł̏C���_���܂݂܂��B�j

�@�@���̃p�b�`���C���X�g�[�����邽�߂ɂ́A��������Ă���t�@�C����NITRO-System
�@�@ Library 2007/11/26 �ł��C���X�g�[������Ă���f�B���N�g���ɏ㏑���R�s�[��
�@�@�Ă��������B


2. �C�����e
===============================================================================

�@�@NITRO-System-Library 071126 patch1 �ŏC�����ꂽ���e
�@�@---------------------------------------------------------------------------

�@�@(1) g3d: �A�j���[�V����������������Ȃ��s����C��

�@�@�@�@�ENNS_G3dAnmObjDisableID()�֐��Ń}�e���A���A�j���[�V�����A�܂��̓r�W�r
�@�@�@�@�@���e�B�A�j���[�V�������w�肵�Ă��A�j���[�V�����������ɂȂ�Ȃ��s�
�@�@�@�@�@���C�����܂����B


�@�@(2) g3d: DMA �]�����g�p����֐��ɂ��Ċe���C��

�@�@�@�@�ENNS_G3dTexLoad(), NNS_G3dPlttLoad(), NNS_G3dDraw(), 
�@�@�@�@�@NNS_G3dDraw1Mat1Shp(), NNS_G3dResDefaultSetup() �֐��ɂ��āA�֐���
�@�@�@�@�@����DMA �]����p���Ă��邽�߁A�Ăяo���O�ɓ]���f�[�^���L���b�V������
�@�@�@�@�@�������Ƀ��C�g�o�b�N���Ă����K�v������|���֐����t�@�����X�}�j���A��
�@�@�@�@�@�ɒǋL���܂����B

�@�@�@�@�E��L�Ɋ֘A���A�e��T���v���f�����C�����܂����B


�@�@(3) g3d: demolib �ɂ�����J�����v�Z�̕s����C��

�@�@�@�@�Edemolib �� G3DDemo_CalcCamera() �֐��ɂ��āA�J�����ʒu�̌v�Z���@��
�@�@�@�@�@��肪�������̂ŏC�����܂����B


�@�@(4) g3dcvtr: SBC �R�}���h�̈ꕔ������č폜���Ă��܂��s����C��

�@�@(5) g3d: ���t�@�����X�}�j���A�����C��


3. �t�@�C�����X�g
===============================================================================

�@�@���\�[�X�t�@�C��
�@�@�@NitroSystem/build/libraries/g3d/src/anm.c
�@�@�@NitroSystem/build/libraries/g3d/src/util.c
�@�@�@NitroSystem/include/nnsys/version.h


�@�@�����C�u�����t�@�C��
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsfnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsfnd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsg2d.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsg2d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsg3d.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsg3d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsgfd.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsgfd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsmcs.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnsmcs.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnssnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Debug/libnnssnd.thumb.a

�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsfnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsfnd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsg2d.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsg2d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsg3d.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsg3d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsgfd.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsgfd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsmcs.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnsmcs.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnssnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Release/libnnssnd.thumb.a

�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsfnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsfnd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsg2d.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsg2d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsg3d.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsg3d.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsgfd.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsgfd.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsmcs.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnsmcs.thumb.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnssnd.a
�@�@�@NitroSystem/lib/ARM9-TS/Rom/libnnssnd.thumb.a



�@�@���T���v��
�@�@�@NitroSystem/build/demos/g3d/demolib/src/camera.c
�@�@�@NitroSystem/build/demos/g3d/samples/1mat1shp/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/callback1/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/callback2/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/callback3/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/callback4/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/callback5/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/CameraMtx/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/CustomizeSbcOp/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/DisposeTex/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/Envelope/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/EnvMap/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/GeComBuffering/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/JointAnm/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/ManualSetup/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/MatColAnm/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/ModifyNsbmd/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/MotionLOD/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/MultiModel/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/PartialAnm1/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/PartialAnm2/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/ProjMap/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/RecordJoint/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/RecordMaterial/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/RecordMtx/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/ScreenUtil/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/SeparateTex/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/ShadowVolume/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/SharedMotion/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/simple/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/SlowMotion/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/TexPatternAnm/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/TexSRTAnm/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/Translucent/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/UnbindTex/src/main.c
�@�@�@NitroSystem/build/demos/g3d/samples/VisibilityAnm/src/main.c


�@�@�����t�@�����X
�@�@�@NitroSystem/man/ja_JP/main.html
�@�@�@NitroSystem/man/ja_JP/g3d/list_g3d.html
�@�@�@NitroSystem/man/ja_JP/g3d/kernel/NNS_G3dPlttLoad.html
�@�@�@NitroSystem/man/ja_JP/g3d/kernel/NNS_G3dTexLoad.html
�@�@�@NitroSystem/man/ja_JP/g3d/sbc/NNS_G3dDraw.html
�@�@�@NitroSystem/man/ja_JP/g3d/sbc/NNS_G3dDraw1Mat1Shp.html
�@�@�@NitroSystem/man/ja_JP/g3d/util/NNS_G3dResDefaultSetup.html


�@�@���T�E���h�v���C���p�t�@�C��
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Debug/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Debug/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Debug/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Release/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Release/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Release/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Rom/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Rom/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS/Rom/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Debug/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Debug/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Debug/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Release/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Release/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Release/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Rom/sub.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Rom/sub.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM7-TS.thumb/Rom/sub_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Debug/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Debug/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Debug/main_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Release/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Release/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Release/main_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Rom/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Rom/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS/Rom/main_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Debug/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Debug/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Debug/main_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Release/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Release/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Release/main_defs.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Rom/main.nef
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Rom/main.sbin
�@�@�@NitroSystem/tools/nitro/SoundPlayer/components/ARM9-TS.thumb/Rom/main_defs.sbin


�@�@��NITRO�v���C���p�t�@�C��
�@�@�@NitroSystem/tools/win/NitroPlayer/nitro/ARM9-TS/Debug/NitroPlayer.srl
�@�@�@NitroSystem/tools/win/NitroPlayer/nitro/ARM9-TS/Release/NitroPlayer.srl
�@�@�@NitroSystem/tools/win/NitroPlayer/nitro/ARM9-TS.thumb/Debug/NitroPlayer.srl
�@�@�@NitroSystem/tools/win/NitroPlayer/nitro/ARM9-TS.thumb/Release/NitroPlayer.srl


�@�@���R���o�[�^
�@�@�@NitroSystem/tools/win/bin/g3dcvtr.exe

