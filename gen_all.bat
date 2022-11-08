@echo off
set all_path=%~dp0
call %all_path%/baselib/gen_lib.bat
echo ****Note: copy thirdparty start****
copy /y %all_path%\baselib\build\Debug\sqlite3.lib %all_path%\sqlite_func\thirdparty
copy /y %all_path%\baselib\include\sqlite3.h %all_path%\sqlite_func\thirdparty
call %all_path%/sqlite_func/gen_main.bat
cmd