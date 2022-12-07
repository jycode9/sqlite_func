@echo off
echo ****Note: gen lib start****
set lib_path=%~dp0
set vs_path=D:/Software/VisualStudio2022/VC/Auxiliary/Build
set lib_build_file=%lib_path%build\ 
if not exist %lib_build_file% (
	md %lib_build_file%
)
cd %lib_build_file%
cmake ..
cd /d %vs_path%
call vcvars64.bat
cd /d %lib_path%/build 
devenv sqlite3.sln /build