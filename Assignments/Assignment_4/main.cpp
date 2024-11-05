#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

// Constants for lookup table configuration
constexpr int TABLE_SIZE = 1440;          // Number of entries for angles from -359.5 to +359.5 degrees
constexpr int SCALE_FACTOR = 100000;      // Scaling factor to store sine values as integers
constexpr double ANGLE_RESOLUTION = 0.5;  // Increment of each angle in degrees

// Lookup table to store scaled sine values
vector<int> sin_lut(TABLE_SIZE);

// Initializes the sine lookup table with scaled integer values
void initialize_sin_lut() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        // Calculate angle in degrees for each index
        double angle = (i * ANGLE_RESOLUTION) - 359.5;
        // Store scaled sine value as an integer
        sin_lut[i] = static_cast<int>(sin(angle * M_PI / 180.0) * SCALE_FACTOR);
    }
}

// Retrieves the sine value from the lookup table for a given angle index
int lookup_sin(int angle_index) {
    // Check if the index is within the valid range
    if (angle_index < 0 || angle_index >= TABLE_SIZE) {
        cerr << "Error: angle out of range.\n";
        exit(1);  // Exit the program if index is invalid
    }
    return sin_lut[angle_index];  // Return the scaled sine value from the lookup table
}

// Benchmarks the lookup table against the standard sin() function
void benchmark() {
    int num_iterations = 1000000;
    int angle_index = 719;

    // Measure time taken for lookup table access
    clock_t start = clock();
    for (int i = 0; i < num_iterations; ++i) {
        lookup_sin(angle_index);
    }
    clock_t end = clock();
    double lut_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    // Measure time taken for the standard sin() function
    start = clock();
    double dummy = 0.0;  // Use a dummy variable to accumulate the results
    for (int i = 0; i < num_iterations; ++i) {
        dummy += sin(0.0);  // Accumulate to ensure sin(0.0) is evaluated
    }
    end = clock();
    double sin_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    cout << "Lookup Table Time: " << lut_time << " seconds\n";
    cout << "Standard sin() Time: " << sin_time << " seconds\n";
    cout << "Speedup Factor: " << sin_time / lut_time << "x\n";
}

// Calculates and outputs the accuracy of the LUT in parts per million (ppm)
void calculate_accuracy() {
    double max_error_ppm = 0.0;  // Initialize the maximum error in ppm
    for (int i = 0; i < TABLE_SIZE; ++i) {
        // Calculate angle in degrees for each index
        double angle = (i * ANGLE_RESOLUTION) - 359.5;
        // Retrieve the lookup table value and calculate the actual sine value
        int lut_value = lookup_sin(i);
        int actual_value = static_cast<int>(sin(angle * M_PI / 180.0) * SCALE_FACTOR);
        // Calculate the error in ppm
        double error_ppm = abs(static_cast<double>(lut_value - actual_value) / actual_value) * 1e6;
        // Update the maximum error if the current error is greater
        if (error_ppm > max_error_ppm) {
            max_error_ppm = error_ppm;
        }
    }
    cout << "Maximum Error (ppm): " << max_error_ppm << "\n";  // Output the maximum error in ppm
}

int main() {
    initialize_sin_lut();  // Initialize the lookup table with sine values
    benchmark();           // Perform timing comparison between LUT and standard sin()
    calculate_accuracy();  // Calculate and output the accuracy of the LUT

    return 0;
}
