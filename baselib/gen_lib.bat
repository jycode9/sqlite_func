@echo off
echo ****Note: gen lib start****
set lib_path=%~dp0
set vs_path=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/
cd %lib_path%/build
cmake ..
cd /d %vs_path%
call vcvars64.bat
cd /d %lib_path%/build 
devenv sqlite3.sln /build
echo ****Note: gen lib done****