#=========================================================================================
=begin

	07.06.07 Satoshi Nohara

	stage_poke.xls����o�g���X�e�[�W�̃|�P�����e�[�u�����쐬

	�֘A�t�@�C��
		stage_poke.txt
		stage_poke.h
		stage_poke.dat		�|�P�����^�C�v
		stage_poke2.dat		�|�P�����i���o�[

=end
#=========================================================================================


#-----------------------------------------------------------------------------------------
=begin

	�ϐ�

=end
#-----------------------------------------------------------------------------------------
$poketype_buf = {}									#�n�b�V��
$pokeno_buf = {}									#�n�b�V��
$count = 0


##########################################################################################
#
#	�|�P�����^�C�v
#
##########################################################################################

#-----------------------------------------------------------------------------------------
=begin

	�n�b�V���쐬

=end
#-----------------------------------------------------------------------------------------
def poketype_buf_make( file )

	while text = file.gets do						#�t�@�C�����P�s���ǂݍ���

		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������

		if( ary[0] =~ /#define/ )

			#print ary[1], "\n"						#NORMAL_TYPE(�l)
			#print ary[3], "\n"						#�m�[�}��(�L�[)

			$poketype_buf[ ary[3] ] = ary[1]
		end

	end
end

#-----------------------------------------------------------------------------------------
=begin

	stage_poke.dat�ɏ�������

=end
#-----------------------------------------------------------------------------------------
def stage_poketype_write( file, io )

	key = "�G�X�p�|"

	while text = file.gets do						#�t�@�C�����P�s���ǂݍ���
		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������

		if( ary[1] == nil )
			next
		end

		#�����N���܂܂�Ă�����
		if( ary[5] =~ /�����N/ )

			#�G�X�p�[�@���̖��O�����n�C�t���ɂȂ��Ă���̂œ��ꏈ��
			if( (ary[3] =~ /���N�V/) ||
				(ary[3] =~ /�G�����b�g/) ||
				(ary[3] =~ /�A�O�m��/) )

				io.print "\n"
				io.print "\t{ SP_TYPE,\tSP_TYPE"
				io.print " },\t\t//", ary[1]
			else####################################################
				io.print "\n"
				io.print "\t{ ", $poketype_buf[ ary[6] ], ",\t"
	
				#�P�̃^�C�v���������Ă��Ȃ����̂́A
				#�Q�Ƃ��������̂������Ă���̂ŁA
				#���̃`�F�b�N�͂���Ȃ����ꉞ�A�A�A
				if( ary[7] != nil )
					io.print $poketype_buf[ ary[7] ]
				end

				io.print " },\t\t//", ary[1]
			end
		end

	end
end


##########################################################################################
#
#	�|�P�����i���o�[
#
##########################################################################################

#-----------------------------------------------------------------------------------------
=begin

	�|�P�����i���o�[��`�̃n�b�V���쐬

=end
#-----------------------------------------------------------------------------------------
def pokeno_buf_make( file2 )

	while text = file2.gets do						#�t�@�C�����P�s���ǂݍ���

		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������

		if( ary[1] =~ /MONSNO/ )

			#print ary[1], "\n"						#NORMAL_TYPE(�l)
			#print ary[2], "\n"						#�m�[�}��(�L�[)

			$pokeno_buf[ ary[2] ] = ary[1]
		end

	end
end

#-----------------------------------------------------------------------------------------
=begin

	stage_poke2.dat�ɏ�������

=end
#-----------------------------------------------------------------------------------------
def stage_pokeno_write( file, io )

	key = "("

	while text = file.gets do						#�t�@�C�����P�s���ǂݍ���
		text.chomp!									#���s������ꍇ�̂ݍ��(�j��I)
		ary = text.split(/\s+/)						#�������g���󔒕����ŕ�������

		key = "("

		#�����N���܂܂�Ă�����
		if( ary[5] =~ /�����N/ )

			key += ary[2]
			key += ")"

			#print key, "\n"
			io.print "\t", $pokeno_buf[ key ], ",\t\t\/\/", ary[1], "\n"
		end

	end
end


#-----------------------------------------------------------------------------------------
=begin

	���C���̗���

=end
#-----------------------------------------------------------------------------------------
#file = open( "../../include/battle/battle_common.h" )	#�Z�̃^�C�v��`�t�@�C���ǂݍ���
file = open( "stage_poke.h" )						#�Z�̃^�C�v��`�t�@�C���ǂݍ���
poketype_buf_make( file )							#poketype_buf�쐬
file.close

file2 = open( "../../include/poketool/monsno.h" )	#�|�P������`�t�@�C���ǂݍ���
pokeno_buf_make( file2 )							#pokeno_buf�쐬
file2.close

io = open( "stage_poke.dat", "w" )					#�o�̓t�@�C��
file = open( ARGV[0] )								#stage_poke.txt
stage_poketype_write( file, io )					#stage_poke.dat�ɏ�������
file.close
io.close											#�o�̓t�@�C������

io = open( "stage_poke2.dat", "w" )					#�o�̓t�@�C��
file = open( ARGV[0] )								#stage_poke.txt
stage_pokeno_write( file, io )						#stage_poke2.dat�ɏ�������
file.close
io.close											#�o�̓t�@�C������


