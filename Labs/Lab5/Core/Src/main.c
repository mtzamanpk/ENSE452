/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/**
* FILENAME: main.c
*
* DESCRIPTION:
* This file contains the main implementation for the FreeRTOS-based Command-Line Interface (CLI)
* on the STM32 microcontroller. It includes:
* - Initialization of FreeRTOS tasks for LED control and CLI handling.
* - UART communication setup for receiving and processing user commands.
* - Functions to control the LED blink rate based on user input.
*
* AUTHOR: Muhammad Zaman
* STUDENT ID: 200449177
* DATE: [Insert Date]
*/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Forward declarations
char cliBufferTX[100];
void printStringBlocking(const char *str);
void welcomeMessage(void);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

osThreadId LED_TaskHandle;
osThreadId CLI_ReceiveHandle;
osThreadId CLI_UpdateHandle;
/* USER CODE BEGIN PV */
QueueHandle_t periodQueue;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartLEDTask(void const * argument);
void StartCLIReceiveTask(void const * argument);
void StartCLIUpdateTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */
  // Initial screen setup
  // Initial screen setup
  strcpy(cliBufferTX, "\x1b[2J");  // ANSI escape code for clearing screen
  CLI_Transmit(cliBufferTX); // Clear entire screen
  HAL_Delay(500);

  // Print a welcome message
  welcomeMessage();
  HAL_Delay(2000);

  // Clear the screen and set up initial cursor position and scrolling
  CLI_Transmit("\x1b[2J");               // Clear entire screen
  CLI_Transmit("\x1b[5;1H");             // Move cursor to row 5, column 1
  CLI_Transmit("\x1b[5;1r");             // Set scrolling region from row 5 to bottom
  CLI_Transmit("\x1b[5;1H");             // Move cursor back to row 5, column 1
  CLI_Transmit("Waiting for Command...\n"); // Initial prompt for the user

  // FreeRTOS setup
  periodQueue = xQueueCreate(1, sizeof(uint32_t));
  if (periodQueue == NULL) {
      Error_Handler();
  }
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LED_Task */
  osThreadDef(LED_Task, StartLEDTask, osPriorityAboveNormal, 0, 128);
  LED_TaskHandle = osThreadCreate(osThread(LED_Task), NULL);

  /* definition and creation of CLI_Receive */
  osThreadDef(CLI_Receive, StartCLIReceiveTask, osPriorityNormal, 0, 256);
  CLI_ReceiveHandle = osThreadCreate(osThread(CLI_Receive), NULL);

  /* definition and creation of CLI_Update */
  osThreadDef(CLI_Update, StartCLIUpdateTask, osPriorityBelowNormal, 0, 128);
  CLI_UpdateHandle = osThreadCreate(osThread(CLI_Update), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void CLI_Transmit(const char *message) {
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

void welcomeMessage(void) {
    CLI_Transmit("\x1b[2J");               // Clear the entire screen
    CLI_Transmit("\x1b[1;1H");             // Move cursor to row 1, column 1
    CLI_Transmit("  Welcome to the CLI Interface\n");
    CLI_Transmit("Commands:\n");
    CLI_Transmit("  - Type 'LED_RATE=<value>' to set LED blink rate\n");

}


#define RX_BUFFER_SIZE 100
char rx_buffer[RX_BUFFER_SIZE];

int CLI_Receive(char *buffer, uint16_t buffer_size) {
    uint16_t index = 0;
    uint8_t rx_char;

    // Loop to receive characters until a newline or buffer limit is reached
    while (index < buffer_size - 1) {
        // Blocking receive for a single character
        if (HAL_UART_Receive(&huart2, &rx_char, 1, HAL_MAX_DELAY) == HAL_OK) {
            // Echo the character back to the terminal
            HAL_UART_Transmit(&huart2, &rx_char, 1, HAL_MAX_DELAY);

            // Check for Enter key (carriage return or line feed)
            if (rx_char == '\r' || rx_char == '\n') {
                buffer[index] = '\0';  // Null-terminate the string
                return 1;  // Successful reception
            } else {
                buffer[index++] = rx_char;  // Append character to buffer
            }
        } else {
            return 0;  // Reception error
        }
    }

    buffer[index] = '\0';  // Null-terminate if buffer limit is reached
    return 1;
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartLEDTask */
/**
  * @brief  Function implementing the LED_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void const * argument)
{
    uint32_t blink_period = 500;  // Default period in milliseconds

    for (;;) {
        // Check if there’s a new period available in the queue
        if (xQueueReceive(periodQueue, &blink_period, 0) == pdPASS) {
            CLI_Transmit("LED Period Updated\n");
        }

        // Toggle LED and delay according to the current period
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        osDelay(blink_period);
    }
}


/* USER CODE BEGIN Header_StartCLIReceiveTask */
/**
* @brief Function implementing the CLI_Receive thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCLIReceiveTask */
void StartCLIReceiveTask(void const * argument)
{
    char command_buffer[RX_BUFFER_SIZE];
    uint32_t current_period = 500;  // Default initial period

    for (;;) {
        // Clear the screen and set a consistent layout
        CLI_Transmit("\x1b[2J");       // Clear the entire screen
        CLI_Transmit("\x1b[1;1H");     // Move cursor to the top-left corner

        // Display the current LED rate at the top
        char rate_message[50];
        snprintf(rate_message, sizeof(rate_message), "Current LED Rate: %lu ms\n", current_period);
        CLI_Transmit("\n To update LED Rate, use command LED_RATE=X, X being the rate               \n");
        CLI_Transmit(rate_message);

        // Display the prompt for command input at a fixed line
        CLI_Transmit("\x1b[6;1H");  // Move cursor to row 6, column 1
        CLI_Transmit("> ");         // Prompt symbol for user input

        // Receive command input
        if (CLI_Receive(command_buffer, RX_BUFFER_SIZE)) {
            // Move to a fixed position to display the received command
            CLI_Transmit("\x1b[8;1H");  // Move cursor to row 8, column 1
            CLI_Transmit("Received: ");
            CLI_Transmit(command_buffer);
            CLI_Transmit("\n");

            // Process the LED rate command
            if (strncmp(command_buffer, "LED_RATE=", 9) == 0) {
                uint32_t new_period = atoi(command_buffer + 9); // Parse the period from command
                xQueueSend(periodQueue, &new_period, 0);        // Send the new period to the LED Task
                current_period = new_period;                    // Update current period locally

                // Display confirmation message on a fixed line
                CLI_Transmit("\x1b[10;1H");  // Move to row 10 for status update
                CLI_Transmit("LED Rate Updated\n");
            } else {
                CLI_Transmit("\x1b[10;1H");  // Move to row 10 for error message
                CLI_Transmit("Invalid command. Use LED_RATE=<value>\n");
            }

            // Add a line break after processing the command
            CLI_Transmit("------------------------------------\n");
        }

        // Small delay before showing the prompt again
        osDelay(500);
    }
}




/* USER CODE BEGIN Header_StartCLIUpdateTask */
/**
* @brief Function implementing the CLI_Update thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCLIUpdateTask */
void StartCLIUpdateTask(void const * argument)
{
    uint32_t current_period = 500;
    char message[50];

    for (;;) {
        // Display the current LED rate at the top of the screen
        CLI_Transmit("\x1b[5;1H");  // Move cursor to row 5, column 1
        snprintf(message, sizeof(message), "Current LED Rate: %lu ms\n", current_period);
        CLI_Transmit(message);

        // Update period from queue if available
        if (xQueuePeek(periodQueue, &current_period, 0) == pdPASS) {
            snprintf(message, sizeof(message), "Updated LED Rate: %lu ms\n", current_period);
            CLI_Transmit("\x1b[7;1H"); // Update at row 7, column 1
            CLI_Transmit(message);
        }

        // Delay before updating again
        osDelay(1000);  // Update every 1 second
    }
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
