/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "nucleo_hal_bsp.h"
#include "cmsis_os.h"
#include "retarget.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <reent.h>

/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void blinkThread(void *argument);
void delayThread(void  *argument);

/* Definitions for blinkThread and UARTThread */
osThreadId_t blinkThreadID;
const osThreadAttr_t blinkThread_attr = {
  .name = "blinkThread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4, /* In bytes */
};

osThreadId_t delayThreadID;
const osThreadAttr_t delayThread_attr = {
  .name = "delayThread",
  .stack_size = 128 * 4, /* In bytes */
  .priority = (osPriority_t) osPriorityNormal,
};

osSemaphoreId_t semID;

int main(void) {
  HAL_Init();

  Nucleo_BSP_Init();
  RetargetInit(&huart2);

  /* Init scheduler */
  osKernelInitialize();

  /* Creation of binary semaphore */
  semID = osSemaphoreNew(1, 1, NULL);
  osSemaphoreAcquire(semID, osWaitForever);
  /* Creation of blinkThread */
  blinkThreadID = osThreadNew(blinkThread, NULL, NULL);
  /* Creation of UARTThread */
  delayThreadID = osThreadNew(delayThread, NULL, NULL);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  while (1);
}

void blinkThread(void *argument) {
  while(1) {
    osSemaphoreAcquire(semID, osWaitForever);
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}

void delayThread(void *argument) {
  while(1) {
    osDelay(500);
    osSemaphoreRelease(semID);
  }
}

#ifdef DEBUG

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ) {
  asm("BKPT #0");
}

#endif
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

