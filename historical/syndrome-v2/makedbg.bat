@echo off
SET PATH=C:\MinGW\bin;%PATH%
make -s -f Makefile.win debug
pause
cls