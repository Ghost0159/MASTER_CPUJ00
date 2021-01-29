##################################################################
#	mtblcv.rb
#	��Ԃꂽ�������@�}�b�v�f�[�^�R���o�[�g
#	�o�^���ꂽ�}�b�v�f�[�^��A������̃o�C�i���f�[�^�ɕϊ�
#	maptblcv.rb "�}�b�v�e�[�u���t�@�C����"
#	�O�� "exec" "system"
##################################################################

#���C�u����
require "tempfile.rb"

#�萔
$KCODE = "SJIS"

#=================================================================
#	�]�[��ID����
#	str	��������]�[��ID������
#	hfile	�]�[��ID�w�b�_�[�t�@�C��
#	return	�]�[��ID�̐��l -1=�G���[
#=================================================================
def mtbl_zoneid_get( str, hfile )
	ary = Array.new
	hfile.pos = 0							#�擪�s��
	
	loop{
		line = hfile.gets
		
		if( line == @nil )
			return -1
		end
		
		
		if( line.include?(str) )
			zone = line
			zone = zone.scan( /\(\d+\)/ )
			zone_id = zone[0].delete( "\(\)" )
			
			if( zone_id == @nil )
				printf( "�s���ȃ]�[��ID id=%s\n", str )
				return -1
			end
			
			return zone_id
		end
	}
	
	return -1
end

#=================================================================
#	�]�[��ID->�f�[�^�C���f�b�N�X�ꗗ�t�@�C���A
#	�]�[���z�u�ʒu�e�[�u�����쐬
#
#	�f�[�^�C���f�b�N�X�ꗗ�t�@�C���\��
#	�擪4�o�C�g�@�C���f�b�N�X����
#	�ȉ�������[zone(u32)],[arcive index(u16)][x(s16)][y(s16][z(s16)]�Ƒ���
#	0-3 [�C���f�b�N�X����]
#	4--[ZONE ID][ARC IDX][x][y][z]...
#=================================================================
def mtbl_posfile_create( wfile, tfile, hfile )
	tfile.pos = 0
	wfile.pos = 0
	
	count = 0
	ary = Array( count.to_i )
	wfile.write( ary.pack("i*") )
	
	tfile.gets 									#��s�ڂ��΂�
	
	loop{
		line = tfile.gets
		
		if( line == @nil )
			return 1
		end
		
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		zone = mtbl_zoneid_get( str[0], hfile )
		
		if( zone == -1 )
			return 1
		end
		
		zone_num = Array( zone.to_i )			#zone id u32
		cnt = Array( count.to_i )				#arcive index u16
		wfile.write( zone_num.pack("I*") )
		wfile.write( cnt.pack("S*") )
		count = count + 1
		
		if( str[1] == "" || str[1] == @nil )	#pos x s16
			return 1
		end
		x = Array( str[1].to_i )
		wfile.write( x.pack("s*") )
		
		if( str[2] == "" || str[2] == @nil )	#pos y s16
			return 1
		end
		y = Array( str[2].to_i )
		wfile.write( y.pack("s*") )
		
		if( str[3] == "" || str[3] == @nil )	#pos z s16
			return 1
		end
		z = Array( str[3].to_i )
		wfile.write( z.pack("s*") )
	}
	
	wfile.pos = 0
	ary = Array( count.to_i )
	wfile.write( ary.pack("i*") )
	return 0
end

#=================================================================
#	�f�o�b�O�Q�Ɨp�]�[���z�u�ʒu�e�[�u���̍쐬
#	#define DEBUG_DATA_FIELDTORNWORLDZONEBLOCKMAX ([����])
#	static const int DEBUG_DATA_FieldTornWorldZoneBlockPosTable[][4] =
#	{
#		{ zone id, x, y, z },
#		{ zone id, x, y, z },
#		{ zone id, x, y, z },
#		....
#		{ ZONE_ID_MAX, 0, 0, 0},	//�I�[
#	},
#=================================================================
def mtbl_debugfile_create( tfile, hfile, filename )
	save_pos = tfile.pos
	tfile.pos = 0
	tfile.gets 									#��s�ڂ��΂�
	
	d_file = File.open( filename, "w" )
	count = 0
	
	loop{
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		count = count + 1
	}
	
	d_file.printf( "#ifndef DEBUG_DATA_FIELD_TORNWORLD_ZONE_BLOCK_MAX\n" )
	d_file.printf( "/*�j�ꂽ���E �f�o�b�O�Q�Ɨp�]�[���u���b�N�z�u���W*/\n" );
	d_file.printf( "/*{ [ZoneID],[X],[Y],[Z] },*/\n" );
	
	d_file.printf(
		"#define DEBUG_DATA_FIELD_TORNWORLD_ZONE_BLOCK_MAX (%s)\n", count );
	d_file.printf(
		"static const int DEBUG_DATA_FieldtornWorldZoneBlockPosTable" );
	d_file.printf(
		"[DEBUG_DATA_FIELD_TORNWORLD_ZONE_BLOCK_MAX][4] =\n{\n" );
	
	tfile.pos = 0
	tfile.gets 									#��s�ڂ��΂�
	
	loop{
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		zone = mtbl_zoneid_get( str[0], hfile )
		x = str[1]
		y = str[2]
		z = str[3]
		d_file.printf( "{ %d, %s, %s, %s },\n", zone, x, y, z );
	}
	
	d_file.printf( "};\n" )
	d_file.printf( "#endif\n" );
	d_file.close
end

#=================================================================
#	���\�[�X�Q�Ɨp�]�[���z�u�ʒu�e�[�u���̍쐬
#	#define DATA_FIELDTORNWORLDZONEBLOCKMAX ([����])
#	static const int DATA_FieldTornWorldZoneBlockPosTable[][4] =
#	{
#		{ zone id, x, y, z },
#		{ zone id, x, y, z },
#		{ zone id, x, y, z },
#		....
#		{ ZONE_ID_MAX, 0, 0, 0},	//�I�[
#	},
#=================================================================
def mtbl_etcsrc_readfile_create( tfile, hfile, filename )
	save_pos = tfile.pos
	tfile.pos = 0
	tfile.gets 									#��s�ڂ��΂�
	
	d_file = File.open( filename, "w" )
	count = 0
	
	loop{
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		count = count + 1
	}
	
	d_file.printf( "/*�j�ꂽ���E �]�[���ʃu���b�N�z�u�擪���W*/\n" );
	d_file.printf( "/*{ [ZoneID],[X],[Y],[Z] },*/\n" );
	
	d_file.printf(
		"#define DATA_FIELD_TORNWORLD_ZONE_BLOCK_MAX (%s)\n", count );
	d_file.printf(
		"static const int DATA_FieldTornWorldZoneBlockPosTable" );
	d_file.printf(
		"[DATA_FIELD_TORNWORLD_ZONE_BLOCK_MAX][4] =\n{\n" );
	
	tfile.pos = 0
	tfile.gets 									#��s�ڂ��΂�
	
	loop{
		line = tfile.gets
		str = line.split( "," )
		
		if( str == @nil || str[0] =~ /#end/ )
			break
		end
		
		zone = mtbl_zoneid_get( str[0], hfile )
		x = str[1]
		y = str[2]
		z = str[3]
		d_file.printf( "{ %d, %s, %s, %s },\n", zone, x, y, z );
	}
	
	d_file.printf( "};\n" )
	d_file.close
end

#=================================================================
#	�o�C�i���t�@�C���쐬
#	no			�������ݔԍ�
#	wpath		�������ރt�@�C����
#	gndname		�n�`�t�@�C���Ǎ���
#	evjmppath	�W�����v�t�@�C���Ǎ���
#	cmapath		�J�����A�N�V�����t�@�C���Ǎ���
#	fstonepath	��ѐ΃t�@�C���Ǎ���
#	return		0=����I�� 1=�ُ�I��
#	
#	�o�C�i���\��
#	0-3		�o�C�i���ԍ�
#	4-7		�n�`�f�[�^�T�C�Y(gnd)
#	8-11	�W�����v�C�x���g�f�[�^�T�C�Y(evj)
#	12-15	�J�����A�N�V�����f�[�^�T�C�Y(cma)
#	16-19	��ѐ΃f�[�^�T�C�Y(fstone)
#	�ȉ��e�T�C�Y���̒n�`�f�[�^�A�W�����v�C�x���g�f�[�^�A�J�����A�N�V�����f�[�^�A��ѐ΃f�[�^�Ƒ���
#
#=================================================================
def mtbl_binfile_create( no, wpath, gndpath, ejmppath, cmapath, fstonepath )
	wfile = File.open( wpath, "wb" )
	
	if( gndpath != "" )
		gndpath = gndpath.sub( /\.xls/, ".bin" )
		gndfile = File.open( "./tw_ground/" + gndpath, "rb" )
	else
		gndpath = @nil
	end
	
	if( ejmppath != "" )
		ejmppath = ejmppath.sub( /\.xls/, ".bin" )
		ejmpfile = File.open( "./tw_evjump/" + ejmppath, "rb" )
	else
		ejmppath = @nil
	end
	
	if( cmapath != "" )
		cmapath = cmapath.sub( /\.xls/, ".bin" )
		cmafile = File.open( "./tw_cmact/" + cmapath, "rb" )
	else
		cmapath = @nil
	end
	
	if( fstonepath != "" )
		fstonepath = fstonepath.sub( /\.xls/, ".bin" )
		fstonefile = File.open( "./tw_fstone/" + fstonepath, "rb" )
	else
		fstonepath = @nil
	end
	
	
	num = Array( no.to_i )
	wfile.write( num.pack("i*") )				#�ԍ������o��
	
	size = 0									#gnd size
	if( gndpath != @nil )
		size = gndfile.stat.size
	end
	ary = Array( size.to_i )
	wfile.write( ary.pack("i*") )				#evj�f�[�^�ʒu�ƂȂ�
	
	size = 0
	if( ejmppath != @nil )						#evj size
		size = ejmpfile.stat.size
	end
	ary = Array( size.to_i )
	wfile.write( ary.pack("i*") )				#cma�f�[�^�ʒu
	
	size = 0
	if( cmapath != @nil )						#cma size
		size = cmafile.stat.size
	end
	ary = Array( size.to_i )
	wfile.write( ary.pack("i*") )				#evj size
	
	size = 0
	if( fstonepath != @nil )					#fstone size
		size = fstonefile.stat.size
	end
	ary = Array( size.to_i )
	wfile.write( ary.pack("i*") )				#end
	
	if( gndpath != @nil )
		wfile.write( gndfile.read )
		gndfile.close
	end
	if( ejmppath != @nil )
		wfile.write( ejmpfile.read )
		ejmpfile.close
	end
	if( cmapath != @nil ) 
		wfile.write( cmafile.read )
		cmafile.close
	end
	if( fstonepath != @nil )
		wfile.write( fstonefile.read )
		fstonefile.close
	end
	
	wfile.close
	return 0
end

#=================================================================
#	�}�b�v�e�[�u���R���o�[�g
#	argv[0] �}�b�v�e�[�u��.xls
#	argv[1] �f�[�^���X�g�������݃t�@�C����
#	argv[2] �]�[��ID�w�b�_�[�t�@�C���� 
#=================================================================
convert = "../../../convert/exceltool/excelcommaconv"

xlsname = ARGV[0]
if( xlsname == @nil )
	printf( "ERROR mtblcv.rb \"MapTableFileName\"\n" )
	exit 1
end

listname = ARGV[1]
if( listname == @nil )
	printf( "ERROR mtblcv.rb \"ListFileName\"\n" )
	exit 1
end
listfile = File.open( listname, "w" )

zonename = ARGV[2]
if( zonename == @nil )
	printf( "ERROR mtblcv.rb \"ZoneHeaderFileName\"\n" )
	exit 1
end
zonefile = File.open( zonename, "r" )

xtxtname = xlsname.sub( /\.xls/, "\.txt" )		#map xls -> txt
system( convert, xlsname )
xtxtfile = File.open( xtxtname, "r" )

#MAPTABLE_LIST�������݊J�n
listfile.printf( "MAPTABLE_LIST =" )

#�e�[�u���C���f�b�N�X�t�@�C���쐬
idxname = File.basename( xlsname )
idxname = idxname.sub( /\.xls/, "_idx" )
printf( "mtblcv create index file %s\n", idxname )
idxfile = File.open( idxname, "wb" )

listfile.printf( " \\\n" )
listfile.printf( "\t%s", idxname )

ret = mtbl_posfile_create( idxfile, xtxtfile, zonefile )

if( ret == 1 )
	printf( "�}�b�v�e�[�u���C���f�b�N�X�����G���[\n" )
	listfile.close
	zonefile.close
	xtxtfile.close
	idxfile.close
	
	File.delete( xtxtname )
	File.delete( idxname )
	exit 1
end

#��s�R���o�[�g
count = 0
xtxtfile.pos = 0
xtxtfile.gets		#��s�ڂ��΂�

loop{
	line = xtxtfile.gets
	if( line == @nil )
		break
	end
	
	str = line.split( ',' )
	
	if( str == @nil || str[0] =~ /#end/ )
		break
	end
	
	mbinname = "map_" + str[0] + ".bin"
	
	ret = mtbl_binfile_create(
		count, mbinname, str[4], str[5], str[6], str[7] )
	
	if( ret == 1 )
		break
	end
	
	listfile.printf( " \\\n" )
	listfile.printf( "\t%s", mbinname )
}

mtbl_debugfile_create( xtxtfile, zonefile, "debug_tornworld.dat" )
mtbl_etcsrc_readfile_create( xtxtfile, zonefile, "field_tornworld_pos.dat" )

listfile.close
zonefile.close
xtxtfile.close
idxfile.close

if( ret == 1 )
	File.delete( xtxtname )
	File.delete( idxfile )
end
