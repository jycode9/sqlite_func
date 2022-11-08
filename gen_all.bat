@echo off
echo ****Note: gen all start****
set all_path=%~dp0
call %all_path%/baselib/gen_lib.bat
echo ****Note: copy thirdparty start****
copy /y %all_path%\baselib\build\Debug\sqlite3.lib %all_path%\sqlite_func\thirdparty
copy /y %all_path%\baselib\include\sqlite3.h %all_path%\sqlite_func\thirdparty
echo ****Note: copy thirdparty done****
echo ****Note: gen all done****
cmd