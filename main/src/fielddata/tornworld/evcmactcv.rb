##################################################################
#	evjmpcv.rb
#	��Ԃꂽ�������@�C�x���g�J�����A�N�V�����f�[�^�R���o�[�g
##################################################################
	
#=================================================================
#	�G���[�I��
#=================================================================
def	evcmactcv_error_end( file0, file1, file )
	file0.close
	file1.close
	File.delete( file )
end

#=================================================================
#	��s�R���o�[�g
#	line	��s����
#	wfile	�������ݐ�t�@�C��p
#	no		���Ԗڂ̃f�[�^��
#	return	0=����I�� 1=�ُ�I�� 2=�I�[ 3=�f�[�^�����s�ł���
#=================================================================
def evcmactcv_conv_line( line, wfile, no )
	num = 0
	ary = Array.new
	str = line.split( "," )					#","��؂�z��
	
	if( str == @nil || str[0] =~ /#end/ )	#�I�[�`�F�b�N
		return 2
	end
	
	#if( str[1] == @nil )					# ��s�`�F�b�N
	#return 3
	#end
	
	if( str[2] == @nil )					# X���W�@s16
		printf( "X���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[2].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[3] == @nil )					# Y���W�@s16
		printf( "Y���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[3].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[4] == @nil )					# Z���W�@s16
		printf( "Z���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[4].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[5] == @nil )					# X�͈́@s16
		printf( "X�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[5].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[6] == @nil )					# Y�͈́@s16
		printf( "Y�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[6].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[7] == @nil )					# Z�͈́@s16
		printf( "Z�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[7].to_i )
	wfile.write( ary.pack("s*") )
	
	if( str[8] == @nil )					# �J����X�@u16
		printf( "�J����X�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[8].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[9] == @nil )					# �J����Y�@u16
		printf( "�J����Y�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[9].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[10] == @nil )					# �J����Z�@u16
		printf( "�J����Z�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[10].to_i )
	wfile.write( ary.pack("S*") )
	
	case str[1]								# ���� s16
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
	wfile.write( ary.pack("S*") )
	
	if( str[11] == @nil )					# �t���[�� s32
		printf( "�t���[�����ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[11].to_i )
	wfile.write( ary.pack("i*") )
	
	return 0								# ����I��
end

#=================================================================
#	cmact�t�@�C���R���o�[�g
#	txtname �R���o�[�g����J�����A�N�V�����f�[�^�e�L�X�g�t�@�C��
#	�߂�l�@0=����I�� 1=�ُ�I��
#=================================================================
def evcmactcv_txtfileconv( txtname )
	if( txtname == @nil )
		printf( "ERROR evcmactcv.rb \"filename\"\n" )
		return 1
	end
	
	#filepath = Dir.pwd							#���݂̃f�B���N�g��
	
	binname = txtname
	binname = txtname.sub( /\.txt/, "\.bin" )	#output name.txt -> name.bin
	
	tfile = File.open( txtname, "r" )
	wfile = File.open( binname, "wb" )

	printf( "Tw EventCameraActionFile Convert %s -> %s\n",
		File.basename(txtname), File.basename(binname) )
	
	data_no = 0									#�f�[�^�ԍ�
	tfile.gets 									#��s�ڂ��΂�
	ary = Array( data_no.to_i )					#�f�[�^������\�ߏ�������
	wfile.write( ary.pack("i*") )
	
	loop{										#�R���o�[�g
		line = tfile.gets
		ret = evcmactcv_conv_line( line, wfile, data_no+1 )
		
		case ret
		when 1		#�G���[
			evcmactcv_error_end( tfile, wfile, binname )
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
