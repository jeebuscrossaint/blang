#!/bin/sh
# Regenerate the build system files

# Run autoreconf to generate the configure script and other necessary files
autoreconf --install --force --verbose

# Run configure script to generate Makefile
./configure

# Run make to build the project
make