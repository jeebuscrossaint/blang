# KiddieScript

simple compiled programming language to maybe help kids learn how to program and low level programming.


## Why make this?

Great question, I made this programming language because for the longest time I wanted to learn assembly but frankly I was too lazy to learn it. So I made this programming language to help me learn assembly and to help others learn how to program and learn low level programming.

## Does this use LLVM?

Nope, I decidede to go the hard route and while its possible to go from the AST directly to the assembly code I chose to write my own IR specifically for this language. Also LLVM is just too complicated for 1 man to handle.

## How do I use this?

There are no dependencies for this project, just clone and build with a standard C install.

**Compilation**
```bash
mkdir -p build
cd build
cmake ..
make
```

**Running the program**
```bash
./skid <filename> # there are no flags for this program yet
```

## Is it optimized/has optimizations?

Nope, for two reasons. One, theres no way any optimizations I make can compete with optimizations made in languages like C or C++ or Rust or any of the other compiled langs like haskell or go. Two, I thought it would be interesting to see what the performance of a language would look like without any optimizations. Especially since develoeprs are so used to having their code optimized for them.

## What does the language look like?

There are examples in the tests folder but here is a simple example of what the language looks like.

```c
x=5
y=10
x++
y--
z=x+y
```
```c
print("Hello World");
```

The syntax is REALLY similar to C, as for syntax inspiration I literally went to cppreference.com and looked at a list of all C keywords, and of course I already know C syntax, since this is written in C.

## What does the language support?

As of the time of writing this, the language supports the following:

Absolutely nothing I just have a lexer like partially done right now.

## What will the language support?

I have this information in include/notes.txt that I plan on implementing.

## What does the language compile to?

My plan for this is basically as the following goes:
```
./skid <filename>
-> lexer
-> parser with abstract syntax tree (ast)
-> intermediate representation (ir)
-> assembly code
-> assembler (will probably end up using like GAS api)
-> linker (will probably end up using ld api)
-> executable
```
