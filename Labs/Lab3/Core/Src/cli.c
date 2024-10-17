#include "usart.h"
#include <string.h>

// Buffers and indexes
char input_buffer[100];
int buffer_index = 0;
const int prompt_length = 2;  // Length of the prompt ('> ')

// Variable to store the received character
volatile uint8_t received_char;
volatile uint8_t data_received = 0;

// Helper function to send strings via UART (non-blocking)
void uart_send_string(const char *str) {
    // Wait until UART is ready before sending another string
    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY);

    // Now send the string in non-blocking mode
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)str, strlen(str));
}

// Function to update the status window (shown once)
void update_status_window(void) {
    uart_send_string("\x1b[2J");  // Clear the screen
    uart_send_string("\x1b[24;1H");  // Move cursor to bottom-left corner (row 24, col 1)
    uart_send_string("\x1b[10;r");  // Create a scrollable window
    uart_send_string("==== Status Window ====\r\n");
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) {
        uart_send_string("LED is ON\r\n");
    } else {
        uart_send_string("LED is OFF\r\n");
    }
    uart_send_string("=======================\r\n");
}

// Process command function
void cli_process_command(const char *cmd) {
    uart_send_string("\r\n");  // Start new line before processing command output

    if (strcmp(cmd, "LED ON") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        uart_send_string("LED turned ON\r\n");
    } else if (strcmp(cmd, "LED OFF") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        uart_send_string("LED turned OFF\r\n");
    } else if (strcmp(cmd, "STATUS") == 0) {
        uart_send_string("LED Status: ");
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) {
            uart_send_string("ON\r\n");
        } else {
            uart_send_string("OFF\r\n");
        }
    } else if (strcmp(cmd, "HELP") == 0) {
        uart_send_string("Available commands:\r\n");
        uart_send_string("LED ON - Turns the LED on\r\n");
        uart_send_string("LED OFF - Turns the LED off\r\n");
        uart_send_string("STATUS - Shows the current LED state\r\n");
        uart_send_string("HELP - Displays this help message\r\n");
    } else {
        uart_send_string("Unknown command: ");
        uart_send_string(cmd);
        uart_send_string("\r\n");
    }

    // Show the prompt again after processing the command
    cli_prompt();
}

// Function to display welcome message
void cli_welcome(void) {
    uart_send_string("\x1b[2J");  // Clear the screen
    uart_send_string("\x1b[24;1H");  // Move cursor to bottom-left corner (row 24, col 1)
    uart_send_string("\x1b[10;r");  // Create a scrollable window

    update_status_window();  // Initial status window update
    uart_send_string("Welcome to the Lab 3 CLI!\r\n");
    uart_send_string("Type 'HELP' for a list of commands.\r\n\r\n");
    cli_prompt();  // Show initial prompt
}

void cli_prompt(void) {
    uart_send_string("\x1b[24;1H");  // Move cursor to bottom-left corner (row 24, col 1)
    uart_send_string("> ");          // Display the prompt
    buffer_index = prompt_length;    // Reset buffer index to start after the prompt
}

void cli_run(void) {
    if (data_received) {
        data_received = 0;  // Reset the flag

        // If Enter key is pressed
        if (received_char == '\r' || received_char == '\n') {
            input_buffer[buffer_index - prompt_length] = '\0';  // Null-terminate the string
            uart_send_string("\r\n");           // Move to new line
            cli_process_command(input_buffer);  // Process the command
            buffer_index = prompt_length;       // Reset buffer after processing the command
        }
        else if (received_char == '\b') {  // Backspace handling
            // Ensure the user can't delete past the start of input (after the prompt)
            if (buffer_index > prompt_length) {  // Only allow backspace if something is typed
                buffer_index--;  // Remove last character from buffer
                uart_send_string("\b \b");  // Erase the character from terminal
            }
        }
        else {
            // Add received character to buffer
            input_buffer[buffer_index - prompt_length] = received_char;
            buffer_index++;  // Move the buffer index forward
            uart_send_string((const char *)&received_char);  // Echo character

            // Prevent buffer overflow
            if (buffer_index - prompt_length >= sizeof(input_buffer)) {
                buffer_index = prompt_length;  // Reset buffer
                uart_send_string("\r\nError: Input buffer overflow.\r\n");
                cli_prompt();  // Show prompt after error
            }
        }

        // Re-enable receiving next character (non-blocking)
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&received_char, 1);
    }
}

// UART Receive Complete Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // Set the data received flag
        data_received = 1;
    }
}
