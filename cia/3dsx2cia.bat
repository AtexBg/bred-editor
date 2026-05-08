cxitool.exe -t 000400000BED1700 -b banner.bnr -c CTR-N-KRJP ../bred-editor.3dsx youCanSafelyDeleteThis.cxi
makerom.exe -f cia -o ../build/BRED-EDITOR.CIA -target t -i youCanSafelyDeleteThis.cxi:0:0 -ignoresign & REM Ignore CXI signature
del youCanSafelyDeleteThis.cxi
pause
