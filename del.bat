@echo off
for /r %%i in (debug,release,ipch) do rd /s /q "%%i"
for /r %%i (*.sdf *.VC.db) do del /s /f /q "%%i"
pause