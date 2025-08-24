@echo off
qmake
make release
cd release
windeployqt .
cd ..
del Makefile.Release Makefile.Debug Makefile .qmake.stash
rmdir /S /Q debug