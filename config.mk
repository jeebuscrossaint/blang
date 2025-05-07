# Set your compiler
CXX ?= cc

CFLAGS ?= -Wall -Wextra -O2 -std=c11 -Iinclude -pedantic

# Linker flags (e.g., for LLVM JIT)
LDFLAGS ?= 

# Installation prefix
PREFIX ?= /usr/local
