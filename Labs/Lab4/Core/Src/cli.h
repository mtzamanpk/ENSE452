/**
* FILENAME: cli.h
*
* DESCRIPTION:
* This header file contains the function declarations and global variable
* declarations necessary for the Command-Line Interface (CLI) functionality
* in the project. It includes functions for processing commands, sending
* and receiving data via UART, updating the status window, and managing
* the user input buffer.
*
* AUTHOR:  Muhammad Zaman 200449177
*/

#ifndef CLI_H
#define CLI_H

#include "stm32f1xx_hal.h"

// Global buffer for storing CLI input
extern char input_buffer[100];  // Stores the current command entered by the user
extern int buffer_index;        // Tracks the position in the input buffer

// UART input character (used for non-blocking reception)
extern volatile uint8_t received_char;  // Holds the character received from UART

// CLI Function Prototypes
void uart_send_string(const char *str);      // Sends a string via UART (non-blocking)
void cli_process_command(const char *cmd);   // Processes user commands like "LED ON", "STATUS", etc.
void cli_prompt(void);                       // Displays the CLI prompt on the terminal
void cli_welcome(void);                      // Displays the welcome message and initial CLI prompt
void cli_run(void);                          // Main loop for running the CLI and handling user input
void update_status_window(void);             // Updates the status window (LED status, etc.)

// Callback Function Prototypes
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  // UART RX complete callback

#endif  // CLI_H
