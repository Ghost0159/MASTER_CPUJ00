##################################################################
#	fstonecv.rb
#	��Ԃꂽ�������@��ѐ΃f�[�^�R���o�[�g
##################################################################

$KCODE = "SJIS"

#=================================================================
#	�G���[�I��
#=================================================================
def	fstonecv_error_end( file0, file1, delfile )
	file0.close
	file1.close
	File.delete( delfile )
end

#=================================================================
#	��ѐ΁@�z�u����������
#=================================================================
def fstonecv_write_putmax( wfile, count )
	pos = wfile.pos
	wfile.pos = 0
	ary = Array( count )
	wfile.write( ary.pack("i*") )
	wfile.pos = pos
end

#=================================================================
#	��ѐ΁@�t���O������W�f�[�^����������
#=================================================================
def fstonecv_write_flagposmax( wfile, count )
	pos = wfile.pos
	wfile.pos = 4
	ary = Array( count )
	wfile.write( ary.pack("i*") )
	wfile.pos = pos
end

#=================================================================
#	��ѐ΁@�t���O������ԏ�������
#=================================================================
def fstonecv_write_flaginit( wfile, flag )
	pos = wfile.pos
	wfile.pos = 8
	ary = Array( flag )
	wfile.write( ary.pack("I*") )
	wfile.pos = pos
end

#=================================================================
#	��ѐ΁@�f�[�^�̈�擪�ʒu��Ԃ�
#=================================================================
def fstonecv_seek_topdata()
	return 12
end

#=================================================================
#	��ѐ΃t���O������ԃR���o�[�g
#	format 0-3 �\���t���O
#=================================================================
def fstonecv_conv_initflag( rfile, wfile )
	no = 0
	bit = 0
	flag = 0
	ary = Array.new
	
	rfile.gets			#"�\���t���ONo"��΂�
	line = rfile.gets	#�\���t���O�ԍ����X�g
	
	line.scan( /\d+/ ){ |match|		#���������o��
		flag = match.to_i
		bit |= 1 << flag
	}
	
	#	ary = Array( bit )
	#	wfile.write( ary.pack("I*") )	#�\���r�b�g u32
	fstonecv_write_flaginit( wfile, bit )
end

#=================================================================
#	��ѐΔz�u�@��s�R���o�[�g
#	return 1 == ERROR
#=================================================================
def fstonecv_conv_put_line( str, wfile )
	num = str[0].to_i				#�Q�ƃt���O�ԍ� u32
	ary = Array( num )
	wfile.write( ary.pack("I*") )
	
	case str[1]						#��ѐΎ�� u16
	when "STEP"
		num = 0
	when "PUT"
		num = 1
	when "STEP3A"
		num = 2
	when "STEP3B"
		num = 3
	when "STEP3C"
		num = 4
	when "STEP4"
		num = 5
	when "STEP5"
		num = 6
	when "STEP6"
		num = 7
	when "STEP7"
		num = 8
	when "STEP8"
		num = 9
	when "STEP9"
		num = 10
	when "STEP10"
		num = 11
	when "STEP11"
		num = 12
	when "STEP12"
		num = 13
	when "STEP13"
		num = 14
	when "STEP14"
		num = 15
	when "STEP15"
		num = 16
	when "STEP16"
		num = 17
	when "STEP17"
		num = 18
	when "STEP18"
		num = 19
	when "LAND_TREE"
		num = 22
	when "LAND_ROCK"
		num = 23
	else
		printf( "��ѐ΃^�C�v�ُ�\n" )
		return 1
	end
	ary = Array( num )
	wfile.write( ary.pack("S*") )
	
	num = str[2].to_i				#��шʒu���W X s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[3].to_i				#��шʒu���W Y s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[4].to_i				#��шʒu���W Z s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	
	return 0
end

#=================================================================
#	��ѐΔz�u�@�R���o�[�g
#	format 0-3 ��ѐ΂̐� 4.. ��ѐ΃f�[�^�Q
#	return 1 == ERROR 2 == END
#=================================================================
def fstonecv_conv_put( rfile, wfile )
	ret = 0
	count = 0
	rfile.gets						#���ڍs���΂�
	
	loop{
		line = rfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			ret = 2
			break
		end
		
		if( str[0] == "" || str[0] == "\r\n" )
			break
		end
		
		ret = fstonecv_conv_put_line( str, wfile )
		
		if( ret == 1 )
			return 1
		end
		
		count = count + 1
	}
	
	fstonecv_write_putmax( wfile, count )
	return ret
end

#=================================================================
#	��ѐ΃t���O������W�f�[�^�@��s�R���o�[�g
#	return 1 == ERROR
#=================================================================
def fstonecv_conv_flagpos_line( str, wfile )
	num = str[0].to_i				#�t���ONo u32
	ary = Array( num )
	wfile.write( ary.pack("I*") )
	
	num = 0
	case str[1]						#�ړ����� s16
	when "UP"
		num = 0
	when "DOWN"
		num = 1
	when "LEFT"
		num = 2
	when "RIGHT"
		num = 3
	else
		printf( "�����ُ�\n" )
		return 1
	end
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	
	case str[2]						#ON,OFF s16
	when "OFF"
		num = 0
	when "ON"
		num = 1
	else
		printf( "����t���O�ُ�\n" )
		return 1
	end
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	
	num = str[3].to_i				#X���W s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[4].to_i				#Y���W s16
		eflag_flagpos = 1
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[5].to_i				#Z���W s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[6].to_i				#X�͈� s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[7].to_i				#Y�͈� s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	num = str[8].to_i				#Z�͈� s16
	ary = Array( num )
	wfile.write( ary.pack("s*") )
	
	return 0
end

#=================================================================
#	��ѐ΃t���O������W�@�R���o�[�g
#	format 0-3 �f�[�^�� 4.. ������W�f�[�^
#	return 1 == ERROR 2 == end
#=================================================================
def fstonecv_conv_flagpos( rfile, wfile )
	ret = 0
	count = 0
	rfile.gets						#���ڍs���΂�
	
	loop{
		line = rfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			ret = 2
			break
		end
		
		if( str[0] == "" || str[0] == "\r\n" )
			break
		end
		
		ret = fstonecv_conv_flagpos_line( str, wfile )
		
		if( ret == 1 )
			return 1
		end
		
		count = count + 1
	}
	
	fstonecv_write_flagposmax( wfile, count )
	return ret
end

#=================================================================
#	��ѐ΃f�[�^�R���o�[�g
#	txtname �R���o�[�g�����ѐ΃f�[�^�e�L�X�g�t�@�C��
#	�߂�l 0=����I�� 1=�ُ�I��
#
#	�t�@�C���t�H�[�}�b�g
#	0-3		��ѐΔz�u��
#	4-7		��ѐΑ�����W�f�[�^��
#	8-11	��ѐ΃t���O�������
#	�ȉ��������@��ѐ΃f�[�^�A������W�f�[�^������
#=================================================================
def fstonecv_conv( txtname )
	if( txtname == @nil )
		printf( "ERROR fstonecv.rb \"FileName\"\n" )
		return 1
	end
	
	binname = txtname.sub( /\.txt/, "\.bin" )	#output name.txt -> name.bin
	
	tfile = File.open( txtname, "r" )
	wfile = File.open( binname, "wb" )
	
	printf( "Tw FroatStoneFile Convert %s -> %s\n",
		File.basename(txtname), File.basename(binname) )
	
	eflag_flag = 0
	eflag_put = 0
	eflag_flagpos = 0
	
	fstonecv_write_putmax( wfile, 0 )				#�w�b�_������
	fstonecv_write_flagposmax( wfile, 0 )
	fstonecv_write_flaginit( wfile, 0 )
	
	wfile.pos = fstonecv_seek_topdata()				#�f�[�^�������ݐ擪�ʒu
	
	loop{											#��ѐ΃t���O�������
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		if( str[0] == "����ѐ΃t���O�������" )
			eflag_flag = 1
			fstonecv_conv_initflag( tfile, wfile )
			break
		end
	}
	
	if( eflag_flag == 0 )
		printf( "init flag error\n" )
		fstonecv_error_end( tfile, wfile, binname )
		return 1
	end
	
	tfile.pos = 0									#�擪�ɖ߂�

	loop{											#��ѐΔz�u�f�[�^
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
	
		if( str[0] == "����ѐΔz�u" )
			eflag_put = 1
			ret = fstonecv_conv_put( tfile, wfile )
			case ret
			when 1									#error
				eflag_put = 0
				break
			when 2
				break
			end
		end
	}
	
	if( eflag_put == 0 )
		printf( "fstone putdata error\n" )
		fstonecv_error_end( tfile, wfile, binname )
		return 1
	end
	
	tfile.pos = 0									#�擪�ɖ߂�
	
	loop{											#�t���O����f�[�^
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		if( str[0] == "����ѐ΃t���O������W�f�[�^" )
			eflag_flagpos = 1
			ret = fstonecv_conv_flagpos( tfile, wfile )
			case ret
			when 1
				printf( "fstone flag pos error\n" )
				fstonecv_error_end( tfile, wfile, binname )
				return 1
			when 2
				break
			end
		end
	}
	
	tfile.close
	wfile.close
	return 0
end
