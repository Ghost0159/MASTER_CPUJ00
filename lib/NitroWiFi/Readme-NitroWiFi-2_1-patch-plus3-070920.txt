������������������������������������������������������������������������������
��                                                                          ��
��  Readme-NitroWiFi-2_1-patch-plus3-070920.txt                             ��
��                                                                          ��
��  Plus Patch for NITRO-SDK Wi-Fi ���C�u���� 2.1                           ��
��                                                                          ��
��  Sep. 20, 2007                                                           ��
��                                                                          ��
������������������������������������������������������������������������������


�y �͂��߂� �z

    ���̃p�b�`�́ANITRO-SDK Wi-Fi ���C�u���� 2.1 �̃����[�X�ȍ~��
    �������ꂽ�s����C�����邽�߂̂��̂ł��B�C���X�g�[�����邽�߂ɂ́A
    ��������Ă���t�@�C���� NITRO-SDK Wi-Fi ���C�u���� 2.1 (070710)��
    �C���X�g�[������Ă���f�B���N�g���ɏ㏑���R�s�[���Ă��������B

    �� NITRO-SDK Wi-Fi ���C�u���� 2.1 �����g�p�̏ꍇ�A
       �K������plus�p�b�`��K�p���ĉ������B

�y ���� �z

�@�@�@NITRO-SDK Wi-Fi ���C�u���� 2.1 �̃��C�u�������r���h������������ꍇ�A
    �{�p�b�`�𓖂Ă��������ƃL���b�V���̉e���Ńv���O�������r���h����Ƃ���
    �G���[����������\��������܂��B
�@�@�@�����r���h���ɃG���[�����������ꍇ�́ANitroWiFi�̃��[�g�f�B���N�g����
�@�@�umake clobber�v�����s���邱�ƂŃL���b�V�����폜����܂��B
�@�@���̌�A����NitroWiFi�̃��[�g�f�B���N�g���Łumake�v���邱�ƂŁA
�@�@���̏Ǐ�͉��P����܂��B

�y �C�����e �z 
    
    NitroWiFi-2_1-patch-plus3-070920 �ŏC�����ꂽ���e
    --------------------------------------------------------------------------
    
    �E(CPS/WCM) TCP ���M�ɂ�����f�[�^�����̏C��

    NitroWiFi �ɂ͑傫�ȃT�C�Y�� TCP �f�[�^�𑗐M�����ꍇ�ɁA
    �f�[�^�������邱�Ƃ�����Ƃ����s�������A������C�����܂����B


    NitroWiFi-2_1-patch-plus2-070912 �ŏC�����ꂽ���e
    --------------------------------------------------------------------------
    
    �E(CPS) �ڑ����Ƀ^�C���A�E�g���Ȃ��Ȃ�s��̏C��

    �ڑ����� RST �p�P�b�g����M�����ꍇ�ɁA�ڑ����^�C���A�E�g���Ȃ��Ȃ�
    �Ƃ����s�������܂����̂ŁA������C�����܂����B


    NitroWiFi-2_1-patch-plus-070829-1321 �ŏC�����ꂽ���e
    --------------------------------------------------------------------------
    
    �E(CPS/SSL) �A�N�Z�X��O�̏C��

    �ʐM���ɂ��A�H�ɃA�N�Z�X��O���������邱�Ƃ�����܂����̂ŁA
    ������C�����܂����B
    ���̏C���ɂ́AWarning:mac mismatch �ƕ\�������
    �f�[�^�A�N�Z�X��O������������̏C�����܂܂�܂��B


�y �t�@�C�����X�g �z

    ���\�[�X�t�@�C��
	/NitroWiFi/build/libraries/wcm/src/cpsif.c

    ���w�b�_�t�@�C��
	/NitroWiFi/build/libraries/wcm/include/wcm_cpsif.h
	/NitroWiFi/include/nitroWiFi/version.h

    �����C�u�����t�@�C��
	/NitroWiFi/lib/ARM9-TS/Debug/libcps.a
	/NitroWiFi/lib/ARM9-TS/Debug/libcps.thumb.a
	/NitroWiFi/lib/ARM9-TS/Debug/libsoc.a
	/NitroWiFi/lib/ARM9-TS/Debug/libsoc.thumb.a
	/NitroWiFi/lib/ARM9-TS/Debug/libssl.a
	/NitroWiFi/lib/ARM9-TS/Debug/libssl.thumb.a
	/NitroWiFi/lib/ARM9-TS/Debug/libwcm.a
	/NitroWiFi/lib/ARM9-TS/Debug/libwcm.thumb.a
	/NitroWiFi/lib/ARM9-TS/Release/libcps.a
	/NitroWiFi/lib/ARM9-TS/Release/libcps.thumb.a
	/NitroWiFi/lib/ARM9-TS/Release/libsoc.a
	/NitroWiFi/lib/ARM9-TS/Release/libsoc.thumb.a
	/NitroWiFi/lib/ARM9-TS/Release/libssl.a
	/NitroWiFi/lib/ARM9-TS/Release/libssl.thumb.a
	/NitroWiFi/lib/ARM9-TS/Release/libwcm.a
	/NitroWiFi/lib/ARM9-TS/Release/libwcm.thumb.a
	/NitroWiFi/lib/ARM9-TS/Rom/libcps.a
	/NitroWiFi/lib/ARM9-TS/Rom/libcps.thumb.a
	/NitroWiFi/lib/ARM9-TS/Rom/libsoc.a
	/NitroWiFi/lib/ARM9-TS/Rom/libsoc.thumb.a
	/NitroWiFi/lib/ARM9-TS/Rom/libssl.a
	/NitroWiFi/lib/ARM9-TS/Rom/libssl.thumb.a
	/NitroWiFi/lib/ARM9-TS/Rom/libwcm.a
	/NitroWiFi/lib/ARM9-TS/Rom/libwcm.thumb.a

�ȏ�
