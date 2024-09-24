#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
public:
    double real;
    double imag;

    Complex(double r = 0, double i = 0);

    void display() const;

    Complex add(const Complex& other) const;
    Complex subtract(const Complex& other) const;
    Complex multiply(const Complex& other) const;
    Complex divide(const Complex& other) const;
};

#endif // COMPLEX_H
