#=========================================================================================
=begin

	07.06.06 Satoshi Nohara

	�o�g���X�e�[�W�̏��ނ������������āA�g���[�i�[�e�[�u�����̍쐬

	stage.xls����stage.txt���쐬���āAstage_tr.dat�ɏo��
	trtype_def.h�̃R�����g���g�p���Ă���̂ŁA����Ƃ͈Ⴄ��������
	����Ă�����̂́A���܂��R���o�[�g�o���܂���

=end
#=========================================================================================


#-----------------------------------------------------------------------------------------
=begin

	�ϐ�

=end
#-----------------------------------------------------------------------------------------
$trtype_buf = {}									#�n�b�V��
$key


#-----------------------------------------------------------------------------------------
=begin

	�n�b�V���쐬

=end
#-----------------------------------------------------------------------------------------
def trtype_buf_make( file )

	key = "test"

	while text = file.gets do						#�t�@�C�����P�s���ǂݍ���

		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������
		ary2 = text.split(/\/\//)					#�������g��//�����ŕ�������

		if( ary[0] =~ /#define/ )

			#print ary[1], "\n"						#TRTYPE_TANPAN(�l)
			#print ary2[1], "\n"						#����ς񂱂���(�L�[)
			#print ary[3], "\n"						#����ς񂱂���(�L�[)
			#print "***********", ary2[1], "\n"

			#sub,split�������Ŏg���Ȃ��̂�ary2���g�p

			#�G���[�`�F�b�N�p�̃u���C�N�ʒu
			#if( ary[1] =~ /TANPAN/ )
			#	return
			#end

			#$trtype_buf[ ary[3] ] = ary[1]
			$trtype_buf[ ary2[1] ] = ary[1]
		end

	end
end


#-----------------------------------------------------------------------------------------
=begin

	stage_tr.dat�ɏ�������

=end
#-----------------------------------------------------------------------------------------
def stage_trtype_write( file, io )
	while text = file.gets do						#�t�@�C�����P�s���ǂݍ���
		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������

		if( ary[1] == nil )
			next
		end

		#for( i=1; i < 8 ;i++ )
		i = 1
		while 1

			if( ary[i] == nil )
				io.print "\n"
				break
			end

			#print "i = ", i, " �`�F�b�N����P�� = ", ary[i], "   trtype_buf = ", $trtype_buf[ ary[i] ], "\n"

			io.print $trtype_buf[ ary[i] ], ",\t"
			i+=1
		end

	end
end


#-----------------------------------------------------------------------------------------
=begin

	���C���̗���

=end
#-----------------------------------------------------------------------------------------
file = open( "../../include/battle/trtype_def.h" )	#�g���[�i�[�^�C�v��`�t�@�C���ǂݍ���
trtype_buf_make( file )								#trtype_buf�쐬
file.close

io = open( "stage_tr.dat", "w" )					#�o�̓t�@�C��
file = open( ARGV[0] )								#stage.txt
stage_trtype_write( file, io )						#stage_tr.dat�ɏ�������
file.close

io.close											#�o�̓t�@�C������


