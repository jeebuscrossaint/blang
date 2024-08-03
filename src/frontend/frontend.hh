
#ifndef FRONTEND_HH
#define FRONTEND_HH

#include <iostream>
#include <string>
#include <cstdarg>
#include <fstream>
#include <csignal>
#include <unistd.h>

extern bool exit_requested;

void render_cli() {}
void version() {}
void exit_cli() {}
void file_grab(std::string filename) {}
void parser() {}

#endif