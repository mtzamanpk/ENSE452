#include "complex.h"
#include <iostream>
#include <stdexcept>
#include <cmath>  // For std::fabs()

const double EPSILON = 1e-9;  // Small tolerance for floating-point comparisons

// Constructor to initialize real and imaginary parts
Complex::Complex(double r, double i) : real(r), imag(i) {}

// Display function to output complex numbers in the format "real + j imag"
void Complex::display() const {
    std::cout << real << " + j " << imag << std::endl;
}

// Add two complex numbers and return the result
Complex Complex::add(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

// Subtract two complex numbers and return the result
Complex Complex::subtract(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

// Multiply two complex numbers and return the result
Complex Complex::multiply(const Complex& other) const {
    double r = (real * other.real) - (imag * other.imag);
    double i = (real * other.imag) + (imag * other.real);
    return Complex(r, i);
}

// Divide two complex numbers and return the result
Complex Complex::divide(const Complex& other) const {
    double denominator = (other.real * other.real) + (other.imag * other.imag);

    // Check if the denominator is close to zero
    if (std::fabs(denominator) < EPSILON) {
        throw std::runtime_error("Divide by zero error");
    }

    double r = (real * other.real + imag * other.imag) / denominator;
    double i = (imag * other.real - real * other.imag) / denominator;
    return Complex(r, i);
}
