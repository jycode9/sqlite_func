@echo off
echo ****Note: gen main start****
set main_path=%~dp0
set main_build_file=%main_path%build\

if not exist %main_build_file% (
	md %main_build_file%
)
cd %main_build_file%
cmake ..