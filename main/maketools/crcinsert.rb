#!/bin/ruby
#  k.ohno    2007.12.7 create
#  crcinsert.rb
#
#  �g�p���@�Fruby crcinsert.rb list.txt
#  crc�����֐����������ފ֐����e�[�u������ǂݍ��ݖ��ߍ���ł���
#  ���̂΂������ruby�X�N���v�g


$FILE_TAG = "File"
$LABEL_TAG = "Label"
$SETFUNC_TAG = "Set"
$CHECKFUNC_TAG = "Chk"


## �֐��𖄂ߍ��ރ��X�g
class FuncList
  def initialize
    @bSet = false
    @FuncParam = ""
    @FuncString2 = ""
    @SaveLabel = ""       ## GMDATA_ID_WIFILIST�Ƃ�
    @SwitchTag = ""  ## �������s���̂��ǂ���
  end
  
  def setBSet(bSet)
    @bSet =bSet
  end
  
  def setFuncParam( funcparam )
    @FuncParam = funcparam
  end
  
  def setSaveLabel( savelabel )
    @SaveLabel = savelabel
    @SwitchTag = "CRCLOADCHECK_" + @SaveLabel
  end
  
  def setFuncString( funcstring )
    @FuncString2 = funcstring.chomp
  end
  def getFuncString
    return @FuncString2
  end
  
  def getPripro
    return "#if (CRC_LOADCHECK && " + @SwitchTag + ")"
  end
  
  def getInsertFunc
    if @bSet == true
      return "\tSVLD_SetCrc(#{@SaveLabel});"
    else
      return "\tSVLD_CheckCrc(#{@SaveLabel});"
    end
  end
  
end

## �t�@�C���ϊ������W�߂��N���X
class ConvFile
  def initialize
    @FILE_NAME = ""       ## /src/savedata/wifilist.c�Ȃ�
    @SaveLabel = ""       ## GMDATA_ID_WIFILIST�Ƃ�
    @SetListArray = []    ## 
  end
  def setFileName(fname)
    @FILE_NAME = fname
  end
  def getFileName
    return @FILE_NAME
  end
  
  def setSaveLabel(funcname)
    @SaveLabel = funcname
  end
  
  def addSetFuncListArray(bSet, line)
    fs = FuncList::new
    fs.setFuncString(line)
    fs.setBSet(bSet)
    fs.setSaveLabel(@SaveLabel)
    
    @SetListArray.push(fs)
  end
  def getSetListArray
    return @SetListArray
  end
    
end



##
##  ���̓e�[�u����array�ɍ����ւ���
##
def readTable( listArray, inputListFile )
  cf = ConvFile.new

  File.open(inputListFile){ |_file|
    _file.each{ |line|
      
      if /^#{$FILE_TAG}\s(.*)/ =~ line then
        if cf.getFileName != "" then
          listArray.push(cf)
        end
        cf = ConvFile.new
        cf.setFileName( $1 )
      elsif /^#{$SETFUNC_TAG}\s(.*)/ =~ line then
        cf.addSetFuncListArray(true,$1)
      elsif /^#{$CHECKFUNC_TAG}\s(.*)/ =~ line then
        cf.addSetFuncListArray(false,$1)
      elsif /^#{$LABEL_TAG}\s(.*)/ =~ line then
        cf.setSaveLabel( $1.chomp )
      end
    }
  }
  if cf.getFileName != "" then
    listArray.push(cf)
  end
end



##
##  �e�[�u���t�@�C������֐������邩�ǂ�����T��
##
##line  �t�@�C���̈�s
##
# $SETFUNC_FIND = 1
# $CHECKFUNC_FIND = 2


def scanFunc( line, convfileP )
  lineName = line.chomp
  
  convfileP.getSetListArray.each{ |flist|
    funcName = flist.getFuncString
    if funcName == lineName
      p funcName
      return flist
    end
  }
  return nil
end


def outFilePuts(outFile , fList)
  outFile.puts(fList.getPripro)
  outFile.puts(fList.getInsertFunc)
  outFile.puts("#endif //CRC_LOADCHECK")
end


##
##  ��`�ɂ������֐��������Ē�`��ǉ����Ă���
##
def convertFile( readFile , writeFile, convfileP )
  r = `pwd`
  r = r.chomp
  setFuncFound = nil
  
  outFile = open(r + writeFile,"w")
  File.open(r + readFile){ |_file|
    _file.each{ |line|
      
      ret = scanFunc(line,convfileP)
      if ret != nil
        setFuncFound = ret
      end
      if /^\treturn.*/ =~ line
        if setFuncFound != nil
          outFilePuts(outFile, setFuncFound)
          setFuncFound = nil
        end
      elsif /^    return.*/ =~ line
        if setFuncFound != nil
          outFilePuts(outFile, setFuncFound)
          setFuncFound = nil
        end
      elsif /^\}/ =~ line
        if setFuncFound != nil
          outFilePuts(outFile, setFuncFound)
          setFuncFound = nil
        end
      end
      outFile.puts(line)
      }
  }
  outFile.close;
end



##main����

InputTable = ARGV.shift    ## �e�[�u���t�@�C���ǂݍ���
list_array = []

#��͂��s��
readTable( list_array, InputTable )

list_array.each{ |cf|
  name = cf.getFileName.chomp
  convertFile(name, name+"new", cf)
  name = "." + name
  disp = `rm #{name}`
  p disp
  namecomm = name + "new " + name
  disp = `mv #{namecomm}`
  p disp
}


