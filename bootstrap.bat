@echo off
REM Regenerate the build system files

REM Run autoreconf to generate the configure script and other necessary files
autoreconf --install --force --verbose

REM Run configure script to generate Makefile
configure

REM Run make to build the project
make