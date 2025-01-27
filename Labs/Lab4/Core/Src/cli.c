/**
* FILENAME: cli.c
*
* DESCRIPTION:
* This file implements the Command-Line Interface (CLI) functionality, allowing
* communication with the STM32 board via UART. The CLI processes user commands
* such as "LED ON", "LED OFF", and "STATUS", and updates the terminal display accordingly.
* It also manages UART communication in non-blocking mode and ensures the prompt
* is always available for the user to enter commands.
*
* The main features include:
* - Processing and executing user commands.
* - Updating the status window to reflect the state of the LED.
* - Handling UART reception via interrupts for non-blocking communication.
* - Backspace and overflow handling in the input buffer.
*
* AUTHOR: Muhammad Zaman 200449177
*/

#include "usart.h"
#include <string.h>

// Buffers and indexes for handling input
char input_buffer[100];     // Input buffer for storing the command
int buffer_index = 0;       // Index for tracking the buffer position
const int prompt_length = 2; // Length of the prompt ('> ')

// Variables for UART input handling
volatile uint8_t received_char;  // Stores the received character
volatile uint8_t data_received = 0;  // Flag indicating whether data was received

// Helper function to send strings via UART in non-blocking mode
void uart_send_string(const char *str) {
    // Wait until UART is ready before sending another string
    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);

    // Transmit the string in non-blocking mode
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)str, strlen(str));
}

// Function to update the status window (called after LED ON/OFF or periodically)
void update_status_window(void) {
    uart_send_string("\x1b[2J");      // Clear the screen
    uart_send_string("\x1b[24;1H");   // Move cursor to bottom-left corner (row 24, col 1)
    uart_send_string("\x1b[10;r");    // Create a scrollable window

    // Display the current LED status
    uart_send_string("==== Status Window ====\r\n");
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) {
        uart_send_string("LED is ON\r\n");
    } else {
        uart_send_string("LED is OFF\r\n");
    }
    uart_send_string("=======================\r\n");
}

// Function to process user-entered commands (LED control, STATUS, HELP)
void cli_process_command(const char *cmd) {
    uart_send_string("\r\n");  // New line before command output

    // Command to turn the LED ON
    if (strcmp(cmd, "LED ON") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        uart_send_string("LED turned ON\r\n");
        update_status_window();  // Update status window after turning LED ON

    // Command to turn the LED OFF
    } else if (strcmp(cmd, "LED OFF") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        uart_send_string("LED turned OFF\r\n");
        update_status_window();  // Update status window after turning LED OFF

    // Command to display current LED status
    } else if (strcmp(cmd, "STATUS") == 0) {
        uart_send_string("LED Status: ");
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) {
            uart_send_string("ON\r\n");
        } else {
            uart_send_string("OFF\r\n");
        }
    // Command to display help information
    } else if (strcmp(cmd, "TIMING TEST") == 0) {
        uart_send_string("Running timing tests...\r\n");

        measure_32bit_addition(100);
        measure_64bit_addition(100);
        measure_32bit_multiplication(100);
        measure_64bit_multiplication(100);
        measure_32bit_division(100);
        measure_64bit_division(100);
        measure_copy_8byte_struct(100);
        measure_copy_128byte_struct(100);
        measure_copy_1024byte_struct(100);
        // Add other tests as needed

        uart_send_string("Timing tests completed.\r\n");
    }else if (strcmp(cmd, "HELP") == 0) {
        uart_send_string("Available commands:\r\n");
        uart_send_string("LED ON - Turns the LED on\r\n");
        uart_send_string("LED OFF - Turns the LED off\r\n");
        uart_send_string("STATUS - Shows the current LED state\r\n");
        uart_send_string("HELP - Displays this help message\r\n");
        uart_send_string("TIMING TEST - Displays \r\n");


    // Handling unknown commands
    } else {
        uart_send_string("Unknown command: ");
        uart_send_string(cmd);
        uart_send_string("\r\n");
    }

    // Show the prompt again after processing the command
    cli_prompt();
}

// Function to display the welcome message when the CLI starts
void cli_welcome(void) {
    uart_send_string("\x1b[2J");      // Clear the screen
    uart_send_string("\x1b[24;1H");   // Move cursor to bottom-left corner
    uart_send_string("\x1b[10;r");    // Create a scrollable window

    // Initial status window update
    update_status_window();
    uart_send_string("Welcome to the Lab 3 CLI!\r\n");
    uart_send_string("Type 'HELP' for a list of commands.\r\n\r\n");

    // Show the initial prompt
    cli_prompt();
}

// Function to display the prompt (bottom-left of the terminal)
void cli_prompt(void) {
    uart_send_string("\x1b[24;1H");   // Move cursor to bottom-left corner
    uart_send_string("> ");           // Display the prompt
    buffer_index = prompt_length;     // Reset buffer index to start after the prompt
}

// Main CLI function that handles user input and processes commands
void cli_run(void) {
    // Check if data has been received
    if (data_received) {
        data_received = 0;  // Reset the data received flag

        // If the Enter key is pressed
        if (received_char == '\r' || received_char == '\n') {
            input_buffer[buffer_index - prompt_length] = '\0';  // Null-terminate the string
            uart_send_string("\r\n");  // Move to a new line
            cli_process_command(input_buffer);  // Process the command
            buffer_index = prompt_length;  // Reset buffer index after processing the command
        }
        // Backspace handling to prevent deleting the prompt
        else if (received_char == '\b') {
            if (buffer_index > prompt_length) {  // Only allow backspace if there's input
                buffer_index--;  // Remove last character from buffer
                uart_send_string("\b \b");  // Erase the character from terminal
            }
        }
        // Handle normal character input
        else {
            input_buffer[buffer_index - prompt_length] = received_char;  // Add character to buffer
            buffer_index++;  // Move buffer index forward
            uart_send_string((const char *)&received_char);  // Echo the character

            // Prevent buffer overflow
            if (buffer_index - prompt_length >= sizeof(input_buffer)) {
                buffer_index = prompt_length;  // Reset buffer
                uart_send_string("\r\nError: Input buffer overflow.\r\n");
                cli_prompt();  // Show prompt after error
            }
        }

        // Re-enable receiving the next character (non-blocking)
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&received_char, 1);
    }
}

// UART receive complete callback function (called when a character is received)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        data_received = 1;  // Set the flag to indicate that data was received
    }
}
