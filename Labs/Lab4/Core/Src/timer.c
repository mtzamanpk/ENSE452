/*
 * timer.c
 *
 *  Created on: Oct 23, 2024
 *      Author: mtzam
 *
 * This file implements timing and measurement functions to assess the performance of various operations.
 * The timer functions use TIM2 and provide basic start/stop functionality to measure elapsed time.
 */

#include "timer.h"
#include <stdlib.h>  // For random number generation
#include <stdint.h>
#include <stdio.h>
#include <time.h>  // For seeding the random number generator with time
#include "stm32f1xx_hal.h"  // Include HAL for hardware access

// Declare the TIM2 handle (extern) to reference the initialized timer from main.c
extern TIM_HandleTypeDef htim2;

// Struct definitions for copy measurement
typedef struct {
    char data[8];  // 8-byte struct
} struct_8byte;

typedef struct {
    char data[128];  // 128-byte struct
} struct_128byte;

typedef struct {
    char data[1024];  // 1024-byte struct
} struct_1024byte;

// Helper function to print timing results
void print_result(const char *test_name, int32_t total_time, int trials) {
    char time_str[20];  // Buffer to hold the time value as a string

    uart_send_string(test_name);               // Send the test name
    uart_send_string(": Avg ticks = ");        // Send the result label

    // Convert and send average time per trial
    itoa(total_time / trials, time_str, 10);   // Convert time to string
    uart_send_string(time_str);                // Send the time string
    uart_send_string("\r\n");                  // Add newline
}

// Start the timer and return the current timer value
int16_t timer_start(void) {
    return (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
}

// Stop the timer and calculate elapsed time since start_time, accounting for wraparound
int16_t timer_stop(int16_t start_time) {
    int16_t current_time = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    if (current_time >= start_time) {
        return current_time - start_time;
    } else {
        // Handle wraparound (0xFFFF to 0x0000)
        return (0xFFFF - start_time) + current_time + 1;
    }
}

// Initialize random number generator with current time
void initialize_random_generator(void) {
    srand(time(NULL));  // Seed the random number generator with the current time
}

// 32-bit addition time
void measure_32bit_addition(int trials) {
    int32_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = rand();  // Generate random 32-bit integer
        b = rand();  // Generate another random 32-bit integer

        start_time = timer_start();  // Start the timer
        result = a + b;  // Perform the addition
        elapsed_time = timer_stop(start_time);  // Stop the timer

        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("32-bit Addition", total_time, trials);  // Print the result
}

// 64-bit addition
void measure_64bit_addition(int trials) {
    int64_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = ((int64_t)rand() << 32) | rand();  // Generate random 64-bit integer
        b = ((int64_t)rand() << 32) | rand();  // Generate another random 64-bit integer

        start_time = timer_start();  // Start the timer

        result = a + b;  // Perform 64-bit addition

        elapsed_time = timer_stop(start_time);  // Stop the timer
        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("64-bit Addition", total_time, trials);
}

// 32-bit multiplication
void measure_32bit_multiplication(int trials) {
    int32_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = rand();  // Generate random 32-bit integer
        b = rand();  // Generate another random 32-bit integer

        start_time = timer_start();  // Start the timer

        result = a * b;  // Perform 32-bit multiplication

        elapsed_time = timer_stop(start_time);  // Stop the timer
        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("32-bit Multiplication", total_time, trials);
}

// 64-bit multiplication
void measure_64bit_multiplication(int trials) {
    int64_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = ((int64_t)rand() << 32) | rand();  // Generate random 64-bit integer
        b = ((int64_t)rand() << 32) | rand();  // Generate another random 64-bit integer

        start_time = timer_start();  // Start the timer

        result = a * b;  // Perform 64-bit multiplication

        elapsed_time = timer_stop(start_time);  // Stop the timer
        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("64-bit Multiplication", total_time, trials);
}

// 32-bit division
void measure_32bit_division(int trials) {
    int32_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = rand();  // Generate random 32-bit integer
        b = rand();  // Generate another random 32-bit integer

        start_time = timer_start();  // Start the timer

        result = a / b;  // Perform division

        elapsed_time = timer_stop(start_time);  // Stop the timer
        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("32-bit Division", total_time, trials);
}

// 64-bit division
void measure_64bit_division(int trials) {
    int64_t a, b, result;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        a = ((int64_t)rand() << 32) | rand();  // Generate random 64-bit integer
        b = ((int64_t)rand() << 32) | rand();  // Generate another random 64-bit integer

        start_time = timer_start();  // Start the timer

        result = a / b;  // Perform division

        elapsed_time = timer_stop(start_time);  // Stop the timer
        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("64-bit Division", total_time, trials);
}

// Copy 8-byte struct
void measure_copy_8byte_struct(int trials) {
    struct_8byte src, dest;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        start_time = timer_start();  // Start the timer
        dest = src;                  // Copy struct
        elapsed_time = timer_stop(start_time);  // Stop the timer

        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("8-byte Struct Copy", total_time, trials);  // Print the result
}

// Copy 128-byte struct
void measure_copy_128byte_struct(int trials) {
    struct_128byte src, dest;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        start_time = timer_start();  // Start the timer
        dest = src;                  // Copy struct
        elapsed_time = timer_stop(start_time);  // Stop the timer

        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("128-byte Struct Copy", total_time, trials);  // Print the result
}

// Copy 1024-byte struct
void measure_copy_1024byte_struct(int trials) {
    struct_1024byte src, dest;
    int16_t start_time, elapsed_time;
    int32_t total_time = 0;

    initialize_random_generator();  // Initialize random number generator

    for (int i = 0; i < trials; i++) {
        start_time = timer_start();  // Start the timer
        dest = src;                  // Copy struct
        elapsed_time = timer_stop(start_time);  // Stop the timer

        total_time += elapsed_time;  // Accumulate elapsed time
    }

    print_result("1024-byte Struct Copy", total_time, trials);  // Print the result
}
