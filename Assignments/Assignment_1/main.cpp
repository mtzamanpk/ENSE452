// Name: Muhammad Zaman
// SID: 200449177
// Assignment #: 1

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cmath>  // For fabs()

#include "complex.h"  // Include Complex class

const double EPSILON = 1e-9;  // Tolerance for floating-point comparisons

// Function to display errors, outputs error codes to stdout
void display_error(int error_code) {
    switch (error_code) {
        case 1:
            std::cout << "error code: 1: illegal command" << std::endl;
            break;
        case 2:
            std::cout << "error code: 2: missing arguments" << std::endl;
            break;
        case 3:
            std::cout << "error code: 3: extra arguments" << std::endl;
            break;
        case 4:
            std::cout << "error code: 4: divide by zero" << std::endl;
            break;
        default:
            std::cout << "error code: unknown" << std::endl;
            break;
    }
}

int main() {
    std::string input;
    char command;
    double r1, i1, r2, i2;

    // Print informative messages and prompts to stderr
    std::cerr << "Complex calculator\nType a letter to specify the arithmetic operator (A, S, M, D)\n";
    std::cerr << "Followed by two complex numbers expressed as pairs of doubles.\nType Q to quit.\n";

    while (true) {
        // Prompt for user input
        std::cerr << "Enter exp: ";  
        std::getline(std::cin, input);

        // Continue if input is empty
        if (input.empty()) continue;

        std::istringstream iss(input);

        // Read the command and handle invalid input
        if (!(iss >> command)) {
            display_error(1);  // Illegal command error
            continue;
        }

        // Convert command to uppercase to handle case-insensitivity
        command = static_cast<char>(std::toupper(static_cast<unsigned char>(command)));

        if (command == 'Q') break;  // Quit command

        // Check for valid commands (A, S, M, D)
        if (command != 'A' && command != 'S' && command != 'M' && command != 'D') {
            display_error(1);  // Illegal command error
            continue;
        }

        // Parse the two complex numbers
        if (!(iss >> r1 >> i1 >> r2 >> i2)) {
            display_error(2);  // Missing arguments error
            continue;
        }

        // Check for extra arguments
        std::string extra;
        if (iss >> extra) {
            display_error(3);  // Extra arguments error
            continue;
        }

        // Create complex numbers and perform the required operation
        Complex z1(r1, i1), z2(r2, i2), result;

        try {
            switch (command) {
                case 'A':
                    result = z1.add(z2);  // Add two complex numbers
                    break;
                case 'S':
                    result = z1.subtract(z2);  // Subtract two complex numbers
                    break;
                case 'M':
                    result = z1.multiply(z2);  // Multiply two complex numbers
                    break;
                case 'D':
                    // Check for divide-by-zero condition
                    if (std::fabs(z2.real) < EPSILON && std::fabs(z2.imag) < EPSILON) {
                        throw std::runtime_error("Divide by zero error");
                    }
                    result = z1.divide(z2);  // Divide two complex numbers
                    break;
                default:
                    display_error(1);  // Shouldn't reach here
                    break;
            }
            result.display();  // Display the result

        } catch (const std::runtime_error& e) {
            // Handle divide-by-zero errors
            if (std::string(e.what()) == "Divide by zero error") {
                display_error(4);  // Output divide by zero error
            }
        }
    }

    return 0;
}
