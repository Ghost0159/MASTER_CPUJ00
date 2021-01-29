##################################################################
#	evjmpcv.rb
#	��Ԃꂽ�������@�C�x���g�W�����v�f�[�^�R���o�[�g
##################################################################

#=================================================================
#	�G���[�I��
#=================================================================
def	evjmpcv_error_end( file0, file1, file )
	file0.close
	file1.close
	File.delete( file )
end

#=================================================================
#	�n��������
#	str	��������A�g���r���[�g�t�@�C����
#	idx	�A�g���r���[�g�C���f�b�N�X�t�@�C����
#	return	�C���f�b�N�X�ԍ�	-1=�G���[
#=================================================================
#def evjmpcv_attrfile_idxget( str, idx )
#end

#=================================================================
#	��s�R���o�[�g
#	line	��s����
#	wfile	�������ݐ�t�@�C��p
#	no		���Ԗڂ̃f�[�^��
#	return	0=����I�� 1=�ُ�I�� 2=�I�[ 3=�f�[�^�����s�ł���
#=================================================================
def evjmpcv_conv_line( line, wfile, no )
	num = 0
	ary = Array.new
	str = line.split( "," )					#","��؂�z��
	
	if( str == @nil || str[0] =~ /#end/ )	# �I���`�F�b�N
		return 2
	end
	
	#if( str[1] == @nil )					# ��s�`�F�b�N
	#return 3
	#end
	
	case str[1]								# �C�x���g�^�C�v u16
	when "WALLJUMP"
		num = 0
	else
		printf( "�C�x���g�^�C�v�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("S*") )
	
	case str[2] 							# �L�[���͕����@s16
	when "UP"
		num = 0
	when "DOWN"
		num = 1
	when "LEFT"
		num = 2
	when "RIGHT"
		num = 3
	else
		printf( "���̓L�[�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[3] == @nil )					# �p�����^�@s32
		printf( "�p�����^�ُ� No=%d\n", no )
	end
	ary = Array( str[3].to_i )
	wfile.write( ary.pack("i*") )
	
	if( str[4] == @nil )					# X���W�@s16
		printf( "X���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[4].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[5] == @nil )					# Y���W�@s16
		printf( "Y���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[5].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[6] == @nil )					# Z���W�@s16
		printf( "Z���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[6].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[7] == @nil )					# X�͈́@s16
		printf( "X�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[7].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[8] == @nil )					# Y�͈́@s16
		printf( "Y�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[8].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[9] == @nil )					# Z�͈́@s16
		printf( "Z�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[9].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[10] == @nil )					# X�ړ��ʁ@s16
		printf( "X�ړ��ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[10].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[11] == @nil )					# Y�ړ��ʁ@s16
		printf( "Y�ړ��ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[11].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[12] == @nil )					# Z�ړ��ʁ@s16
		printf( "Z�ړ��ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[12].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[13] == @nil )					# ��]�� s16
		printf( "��]�ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[13].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[14] == @nil )					# �t���[�� s16
		printf( "��]�ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[14].to_i )
	wfile.write( ary.pack("s*") )
	
	case str[15]							#�x�N�g���I�t�Z�b�g u16
	when "x"
	when "X"
		num = 0
	when "y"
	when "Y"
		num = 1
	when "z"
	when "Z"
		num = 2
	else
		printf( "�x�N�g���I�t�Z�b�g�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("S*") )
	
	case str[16]							# �I�t�Z�b�g���] u16
	when "ON"
		num = 1
	when "OFF"
		num = 0
	else
		printf( "�I�t�Z�b�g���]�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("S*") )
	
	case str[17]							# �I������ s16
	when "UP"
		num = 0
	when "DOWN"
		num = 1
	when "LEFT"
		num = 2
	when "RIGHT"
		num = 3
	else
		printf( "�I�������ُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("s*") )
	
	case str[18]							# ���n�� s16
	when "GROUND"
		num = 0
	when "SIDE_L"
		num = 1
	when "SIDE_R"
		num = 2
	when "ROOF"
		num = 3
	when "NORMAL"
		num = 4
	else
		printf( "���n�ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[19] == @nil )					# ���n�ԍ� u16
		printf( "���n�ԍ��ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[19].to_i )
	ary[0] = ary[0] - 1;					#1org -> 0org
	wfile.write( ary.pack("S*") )
	
	return 0								# ����I��
end

#=================================================================
#	evj�t�@�C���R���o�[�g
#	txtname �R���o�[�g����W�����v�f�[�^�e�L�X�g�t�@�C��
#	�߂�l�@0=����I�� 1=�ُ�I��
#=================================================================
def evjmpcv_txtfileconv( txtname )
	#txtname = ARGV[0] 								#filepath

	if( txtname == @nil )
		printf( "ERROR evjmpcv.rb \"filename\"\n" )
		return 1
	end
	
	#filepath = Dir.pwd							#���݂̃f�B���N�g��
	
	binname = txtname
	binname = txtname.sub( /\.txt/, "\.bin" )	#output name.txt -> name.bin
	
	tfile = File.open( txtname, "r" )
	wfile = File.open( binname, "wb" )

	printf( "Tw EventJumpFile Convert %s -> %s\n",
		File.basename(txtname), File.basename(binname) )
	
	data_no = 0									#�f�[�^�ԍ�
	tfile.gets 									#��s�ڂ��΂�
	ary = Array( data_no.to_i )					#�f�[�^������\�ߏ�������
	wfile.write( ary.pack("i*") )
	
	loop{										#�R���o�[�g
		line = tfile.gets
		ret = evjmpcv_conv_line( line, wfile, data_no+1 )
		
		case ret
		when 1		#�G���[
			evjmpcv_error_end( tfile, wfile, binname )
			return 1
		when 2		#�I�[
			break
		when 3		#��s
			next
		end
	
		data_no = data_no + 1
	}
	
	wfile.pos = 0								#�擪�ɖ߂�
	ary = Array( data_no.to_i )
	wfile.write( ary.pack("i*") )				#�f�[�^��������������
	
	tfile.close
	wfile.close
	return 0
end
