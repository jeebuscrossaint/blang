#!/usr/bin/env bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON build
ln -s build/compile_commands.json compile_commands.json
