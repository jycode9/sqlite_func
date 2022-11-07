@echo off
echo ****Note: gen all start****
set all_path=%~dp0
call %all_path%/baselib/gen_lib.bat
copy /y %all_path%\baselib\build\Debug\sqlite3.lib %all_path%
echo ****Note: gen all done****
cmd