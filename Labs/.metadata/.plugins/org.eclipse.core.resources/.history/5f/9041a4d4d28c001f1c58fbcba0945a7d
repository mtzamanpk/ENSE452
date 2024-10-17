/**
* FILENAME: cli.h
*
* DESCRIPTION:
*
* In this file you will find the function headers
*
* AUTHOR: Muhammad Zaman 200449177
*/
#ifndef CLI_H
#define CLI_H

#include "stm32f1xx_hal.h"

// Buffer for CLI input
extern char input_buffer[100];
extern int buffer_index;

// Declare received_char as extern so it can be used in main.c
extern volatile uint8_t received_char;

// Function prototypes
void uart_send_string(const char *str);      // Sends a string via UART
void cli_process_command(const char *cmd);   // Processes commands entered by the user
void cli_prompt(void);                       // Displays the prompt in the terminal
void cli_welcome(void);                      // Displays the welcome message and initial prompt
void cli_run(void);                          // Main CLI function to handle input
void update_status_window(void);             // Function to update the status window


// Callback function prototypes
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  // UART receive callback

#endif  // CLI_H
