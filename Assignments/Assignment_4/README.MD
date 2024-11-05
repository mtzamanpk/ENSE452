# Sine Lookup Table (LUT) for ENEL452 Assignment 4

This project implements a lookup table (LUT) to calculate the sine of an angle in the range of -359.5 to +359.5 degrees with a 0.5-degree resolution. The LUT avoids floating-point arithmetic by scaling the sine values by 100,000, providing a potentially faster alternative to the standard `sin()` function at the cost of increased memory usage.

## How to run

1. make clean
2. make run

## Target Environment

### Replit Virtual Environment
- **Platform**: Replit Virtual Environment
- **Date**: November 4, 2024
- **Compiler**: g++ (default Replit version)
- **Compiler Flags**: -O3 for optimization

### Snoopy Environment
- **Platform**: Snoopy
- **Date**: November 4, 2024
- **Compiler**: g++ (default Snoopy version)
- **Compiler Flags**: -O4, -O3, -O2, -O1 for optimization

## Measurement Method

The timing measurements were conducted using the `clock()` function from the C++ standard library. Each function (`lookup_sin` for LUT access and `std::sin` for the standard function) was called in a loop for 1,000,000 iterations to obtain consistent timing results.

## Results

### Timing Comparison

#### Replit

 Function          | Speed
-------------------|----------------
 Table Lookup (LUT)| 1e-06          
 Built-in `sin()`  | 1e-06          
 **Table Speedup** | 1x             

#### Snoopy

Optimization level 1: 

      Function      |    Speed 
---------------------------------------
 Table Lookup (LUT) | 0.001584 seconds
 Built-in `sin()`   | 0.000315 seconds
 **Table Speedup**  | 0.198864x

Optimization level 2: 

      Function      |    Speed 
---------------------------------------
 Table Lookup (LUT) | 2e-06 seconds
 Built-in `sin()`   | 1e-06 seconds
 **Table Speedup**  | 0.5x

Optimization level 3: 

      Function      |    Speed 
---------------------------------------
 Table Lookup (LUT) | 3e-06 seconds
 Built-in `sin()`   | 2e-06 seconds
 **Table Speedup**  | 0.666667x

Optimization level 4: 

      Function      |    Speed 
---------------------------------------
 Table Lookup (LUT) | 2e-06 seconds
 Built-in `sin()`   | 2e-06 seconds
 **Table Speedup**  | 1x


- **Table Speedup**: The speedup varied across different runs and environments:
  - Replit: No speedup observed (1x).
  - Snoopy: Speedup factors ranged from 0.19x to 1x, with the LUT not consistently outperforming `sin()`.
  - command used to compile the following tests: 
  - Optimization level 1: "g++ -O1 -Wall -std=c++11 -o sin_lut main.cpp -lm " 
  - Optimization level 2: "g++ -O2 -Wall -std=c++11 -o sin_lut main.cpp -lm "
  - Optimization level 3: "g++ -O3 -Wall -std=c++11 -o sin_lut main.cpp -lm "
  - Optimization level 4: "g++ -O4 -Wall -std=c++11 -o sin_lut main.cpp -lm "

### Accuracy

The maximum error between the LUT values and the standard `sin()` function was calculated in parts-per-million (ppm).

- **Maximum Error (ppm)**: 0

This suggests that the LUT values match exactly with the calculated `sin()` values, given the resolution and scaling applied.

## Code Structure

- `main.cpp`: Contains the implementation for initializing the lookup table, calculating and retrieving sine values, measuring timing, and calculating accuracy.
