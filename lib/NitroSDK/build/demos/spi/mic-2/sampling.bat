@ECHO OFF
IF EXIST *.wav  del *.wav /q
REM del log.txt
echo now capturing MIC...
"%NITROSDK_ROOT%/tools/bin/loadrun.exe" "bin/ARM9-TS/Release/main.srl" > log.txt
"%NITROSDK_ROOT%/tools/bin/mic2wav.exe" log.txt
