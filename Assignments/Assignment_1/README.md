// Name: Muhammad Zaman
// SID: 200449177
// Assignment #: 1

Complex Number Calculator

Overview:
This program performs basic arithmetic operations (Addition, Subtraction, Multiplication, and Division) on complex numbers. It reads input from the standard input and outputs results to the standard output.

Usage:
Run the program and input commands as specified (A, S, M, D) followed by two complex numbers.
To exit, type Q.
Example:
A 1 2 3 4 -> 4 + j 6

Limitations:
The program does not handle non-numeric.

Error Handling:
- Illegal commands print "error code: 1".
- Missing arguments print "error code: 2".
- Extra arguments print "error code: 3".
- Division by zero prints "error code: 4".

Build Instructions:
Use the following command to compile the program:

1. g++ -o complex_calculator main.cpp complex.cpp 
2. ./runtests.py ./complex_calculator

side note: i did run all the warnings, and it did work with warnings aswell

IF you'd like to run the program with warnings, you can paste it as one line and it will compile 

g++ -o complex_calculator main.cpp complex.cpp \
    -Wall -Wextra -Wfloat-equal -Wconversion -Wparentheses \
    -pedantic -Wunused-parameter -Wunused-variable -Wreturn-type \
    -Wunused-function -Wredundant-decls -Wunused-value \
    -Wswitch-default -Wuninitialized -Winit-self -Werror
