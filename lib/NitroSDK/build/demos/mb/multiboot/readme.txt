MB���C�u���� �}���`�u�[�g�e�@�f�� - ����T�v - 

�v���O�����𗧂��グ��ƁA�e�@�������I�ɗ����オ��A

AID USERNAME STATE
 1  name1    NONE
 2  name2    NONE
 3  name3    NONE
 4  name4    NONE
....

�Ƃ������A�\���\������܂��B

�\���L�[�ŁA����ΏۂƂ���q�@��I��ŁA
A�{�^����STATE�ɑΉ�����������s���܂��B
�܂��X�^�[�g�{�^���ŁA�ڑ����Ă����q�@�S�ĂɌ�����
A�{�^������������Ɠ����̂��Ƃ��s���܂��B

SELECT�{�^���������ƁA�������L�����Z���I�����A�ė����グ���܂��B

���eSTATE�Ɋւ������

NONE          ... �����ڑ�����Ă��Ȃ����
CONNECTED     ... �q�@����ڑ�������
DISCONNECTED  ... �q�@���ڑ���؂���
ENTRY REQUEST ... �q�@����G���g���[���N�G�X�g������
                  �yA�{�^��:�G���g���[������ B�{�^��:�G���g���[�����ہz
KICKED        ... �q�@���L�b�N����
ENTRY OK      ... �q�@����̃_�E�����[�h�v����������
                  �yA�{�^��:�o�C�i���𑗐M�J�n�z
SENDING       ... �q�@�փo�C�i�����M��
SEND COMPLETE ... �q�@�ւ̃o�C�i�����M���I������
                  �yA�{�^��:�u�[�g�v�����M�z
BOOT REQUEST  ... �q�@�փu�[�g�v���̑��M���J�n
BOOT READY    ... �_�E�����[�h�������I���A�_�E�����[�h�����A�v�����N����
MEMBER FULL   ... ����ɒB�����Q�[���Ɏq�@���ڑ����Ă���

�܂��A�y�z���Ɏ�������́A���Ɏ���#define����1�ɂ��邱�Ƃ�
���������邱�Ƃ��ł��܂��B

#define AUTO_ACCEPT_ENTRY	(0)
#define AUTO_SENDFILE		(0)
#define AUTO_BOOTREQ		(0)

�� 
�@ ���̃v���O�����́AASCII�����݂̂̕\���Ή��ƂȂ�܂��̂ŁA
   ���݂̎q�@�o�C�i��mb_child.srl�̃��[�U�[�����͐������\������܂���B
   �\�ߌ䗹�����������B


