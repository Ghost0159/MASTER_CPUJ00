##################################################################
#	gndcv.rb
#	��Ԃꂽ�������@�n�`�t�@�C���R���o�[�g
##################################################################

#=================================================================
#	�G���[�I��
#=================================================================
def	gndcv_error_end( file0, file1, file2, file3, dfile0, dfile1 )
	file0.close
	file1.close
	file2.close
	file3.close
	File.delete( dfile0 )
	File.delete( dfile1 )
end

#=================================================================
#	�A�g���r���[�g�A�[�J�C�u�C���f�b�N�X������
#	str	��������A�g���r���[�g�t�@�C����
#	�A�g���r���[�g�C���f�b�N�X�t�@�C��
#	return	�C���f�b�N�X�ԍ�	-1=�G���[
#=================================================================
def gndcv_attrfile_idxget( str, idx )
	enum = str.sub( /\.dat/, "\_dat" )		# *.dat -> *_dat
	
	if( str.include?("\.dat") )
		str = str.sub( /\.dat/, "" )
	end
	
	no = 0
	idx.pos = 0								#�擪�s
	
	loop{									#enum�s�܂Ői�s
		line = idx.gets
		
		if( line == @nil )
			return -1
		end
		
		if( line.include?("enum {") )
			break
		end
	}
	
	count = 0
	
	loop{
		line = idx.gets
		
		if( line == @nil )
			return -1
		end
		
		if( line.include?(str) )
			return count
		end
		
		count = count + 1
	}
	
	return -1
end

#=================================================================
#	��s�R���o�[�g
#	line	��s����
#	wfile	�������ݐ�t�@�C��
#	head	�n�`�w�b�_�[�t�@�C��
#	idx		�C���f�b�N�X�t�@�C��
#	heads	�w�b�_�[����
#	no		���Ԗڂ̃f�[�^��
#	return	0=����I�� 1=�ُ�I�� 2=�I�[ 3=�f�[�^�����s�ł���
#=================================================================
def gndcv_conv_line( line, wfile, hfile, ifile, no, heads )
	num = 0
	ary = Array.new
	str = line.split( "," )					#","��؂�z��
	
	if( str == @nil || str[0] =~ /#end/ )	# �I���`�F�b�N
		return 2
	end
	
	#if( str[1] == @nil )					# ��s�`�F�b�N
	#return 3
	#end
	
	if( str[1] == @nil )
		return 1
	end
	
	hfile.printf( "\t%s_%s = %d,\n", heads, str[0], no-1 );	#�ԍ����w�b�_��
	
	if( str[1] == "NORMAL" )				#�ʏ�n�`
		num = 4
		ary = Array( num.to_i )
		wfile.write( ary.pack("S*") )
		
		num = 0								#�c����_�~�[�Ŗ��߂�
		ary = Array( num.to_i )
		wfile.write( ary.pack("s*") )
		
		wfile.write( ary.pack("i*") )
		wfile.write( ary.pack("i*") )
		wfile.write( ary.pack("i*") )
	end
	
	case str[1]								# �n�`�^�C�v s16
	when "GROUND"
		num = 0
	when "SIDE_L"
		num = 1
	when "SIDE_R"
		num = 2
	when "ROOF"
		num = 3
	else
		printf( "���n�ʈُ� No=%d\n", no )
		return 1
	end
	ary = Array( num.to_i )
	wfile.write( ary.pack("s*") )
	
	ret = gndcv_attrfile_idxget( str[10], ifile ) #�A�g���r���[�gindex u16
	if( ret == -1 )
		printf( "�A�g���r���[�g�t�@�C�����ُ� No=%d\n", no )
		return 1
	end
	ary = Array( ret )
	wfile.write( ary.pack("S*") )
	
	if( str[2] == @nil )					# X���W�@u16
		printf( "X���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[2].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[3] == @nil )					# Y���W�@u16
		printf( "Y���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[3].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[4] == @nil )					# Z���W�@u16
		printf( "Z���W�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[4].to_i )
	wfile.write( ary.pack("S*") )
	
	//
	if( str[5] == @nil )					# X�͈́@u16
		printf( "�n�`�T�C�YX�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[5].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[6] == @nil )					# Y�͈́@u16
		printf( "�n�`�T�C�YY�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[6].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[7] == @nil )					# Z�͈́@u16
		printf( "�n�`�T�C�YZ�ُ� No=%d\n", no )
		return 1
	end
	ary = Array( str[7].to_i )
	wfile.write( ary.pack("S*") )
	
	//
	if( str[8] == @nil )					# X�͈́@u16
		printf( "X�A�g���r���[�g�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[8].to_i )
	wfile.write( ary.pack("S*") )
	
	if( str[9] == @nil )					# Y�͈́@u16
		printf( "Y�A�g���r���[�g�ُ͈͈� No=%d\n", no )
		return 1
	end
	ary = Array( str[9].to_i )
	wfile.write( ary.pack("S*") )
	
	return 0								# ����I��
end

#=================================================================
#	evj�t�@�C���R���o�[�g
#	txtname		�R���o�[�g����W�����v�f�[�^�e�L�X�g�t�@�C��
#	idxname		�Q�Ƃ���A�g���r���[�g�C���f�b�N�X�t�@�C����
#	�߂�l�@0=����I�� 1=�ُ�I��
#=================================================================
def gndcv_txtfileconv( txtname, idxname )
	if( txtname == @nil )
		printf( "ERROR gndcv.rb \"filename\"\n" )
		return 1
	end
	
	if( idxname == @nil )
		printf( "ERROR gndcv.rb \"attr_idxname\n" )
		return 1
	end
	
	binname = txtname.sub( /\.txt/, "\.bin" )	#output name.txt -> name.bin
	headname = txtname.sub( /\.txt/, "\.h" )	#output name.txt -> name.h
	
	tfile = File.open( txtname, "r" )
	wfile = File.open( binname, "wb" )
	hfile = File.open( headname, "w" )
	ifile = File.open( idxname, "r" )
	
	printf( "Tw GroundFile Convert %s -> %s\n",
		File.basename(txtname), File.basename(binname) )
	
	heads = txtname.sub( /\.txt/, "" )
	
	#�w�b�_�[ num��������
	hfile.printf( "#ifndef %s_H_FILE_\n#define %s_HFILE_\nenum {\n", File.basename(heads), File.basename(heads) )
	
	data_no = 0									#�f�[�^�ԍ�
	tfile.gets 									#��s�ڂ��΂�
	ary = Array( data_no.to_i )					#�f�[�^������\�ߏ�������
	wfile.write( ary.pack("i*") )
	
	loop{										#�R���o�[�g
		line = tfile.gets
		ret = gndcv_conv_line(
			line, wfile, hfile, ifile, data_no+1, File.basename(heads) )
		
		case ret
		when 1		#�G���[
			gndcv_error_end( tfile, wfile, hfile, ifile, binname, headname )
			return 1
		when 2		#�I�[
			break
		when 3		#��s
			next
		end
	
		data_no = data_no + 1
	}
	
	hfile.printf( "};\n#endif" )
	
	wfile.pos = 0								#�擪�ɖ߂�
	ary = Array( data_no.to_i )
	wfile.write( ary.pack("i*") )				#�f�[�^��������������
	
	tfile.close
	wfile.close
	hfile.close
	ifile.close
	return 0
end
