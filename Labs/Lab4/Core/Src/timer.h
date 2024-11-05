/*
 * timer.h
 *
 *  Created on: Oct 23, 2024
 *      Author: mtzam
 */




#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Function to start the timer and return the current timer value
int16_t timer_start(void);

// Function to stop the timer and return the elapsed time (in ticks)
int16_t timer_stop(int16_t start_time);
void initialize_random_generator(void);
void print_result(const char *test_name, int32_t total_time, int trials);
void measure_32bit_addition(int trials);
void measure_64bit_addition(int trials);
void measure_32bit_multiplication(int trials);
void measure_64bit_multiplication(int trials);
void measure_32bit_division(int trials);
void measure_64bit_division(int trials);
void measure_copy_8byte_struct(int trials);
void measure_copy_128byte_struct(int trials);
void measure_copy_1024byte_struct(int trials);
#endif  // TIMER_H
