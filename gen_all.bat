@echo off
echo gen all start
set bat_src=%~dp0
set vs_path=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build
cmd /k "cd ./baselib/build && cmake .. && cd %vs_path% && C: && vcvars64.bat && cd %bat_src%/baselib/build && D: && devenv sqlite3.sln /build"
echo gen all done
pause