Name: Muhammad Zaman
SID: 200449177
Assignment #: 1

Complex Number Calculator

Overview:
This program performs basic arithmetic operations (Addition, Subtraction, Multiplication, and Division) on complex numbers. It reads input from the standard input and outputs results to the standard output.

Usage:
Run the program and input commands as specified (A, S, M, D) followed by two complex numbers.
To exit, type Q.
Example:
A 1 2 3 4 -> 4 + j 6

Limitations:
The program does not handle non-numeric input gracefully.

Error Handling:
- Illegal commands print "error code: 1".
- Missing arguments print "error code: 2".
- Extra arguments print "error code: 3".
- Division by zero prints "error code: 4".

Build Instructions:
Use the following command to compile the program:
g++ -o complex_calculator main.cpp complex.cpp -Wall -Wextra
