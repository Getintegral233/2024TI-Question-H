/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key_reset_Pin GPIO_PIN_3
#define Key_reset_GPIO_Port GPIOA
#define L_S_Pin GPIO_PIN_4
#define L_S_GPIO_Port GPIOA
#define Key_yaw_Pin GPIO_PIN_10
#define Key_yaw_GPIO_Port GPIOB
#define Key_mode_Pin GPIO_PIN_11
#define Key_mode_GPIO_Port GPIOB
#define MOTOR_1_Pin GPIO_PIN_12
#define MOTOR_1_GPIO_Port GPIOB
#define MOTOR_2_Pin GPIO_PIN_13
#define MOTOR_2_GPIO_Port GPIOB
#define MOTOR_3_Pin GPIO_PIN_14
#define MOTOR_3_GPIO_Port GPIOB
#define MOTOR_4_Pin GPIO_PIN_15
#define MOTOR_4_GPIO_Port GPIOB
#define G4_Pin GPIO_PIN_8
#define G4_GPIO_Port GPIOA
#define G7_Pin GPIO_PIN_11
#define G7_GPIO_Port GPIOA
#define G6_Pin GPIO_PIN_12
#define G6_GPIO_Port GPIOA
#define G5_Pin GPIO_PIN_15
#define G5_GPIO_Port GPIOA
#define G3_Pin GPIO_PIN_3
#define G3_GPIO_Port GPIOB
#define G2_Pin GPIO_PIN_4
#define G2_GPIO_Port GPIOB
#define G1_Pin GPIO_PIN_5
#define G1_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define G1 HAL_GPIO_ReadPin(G1_GPIO_Port,G1_Pin)
#define G2 HAL_GPIO_ReadPin(G2_GPIO_Port,G2_Pin)
#define G3 HAL_GPIO_ReadPin(G3_GPIO_Port,G3_Pin)
#define G4 HAL_GPIO_ReadPin(G4_GPIO_Port,G4_Pin)
#define G5 HAL_GPIO_ReadPin(G5_GPIO_Port,G5_Pin)
#define G6 HAL_GPIO_ReadPin(G6_GPIO_Port,G6_Pin)
#define G7 HAL_GPIO_ReadPin(G7_GPIO_Port,G7_Pin)

#define Key_mode HAL_GPIO_ReadPin(Key_mode_GPIO_Port,Key_mode_Pin)
#define Key_yaw HAL_GPIO_ReadPin(Key_yaw_GPIO_Port,Key_yaw_Pin)
#define Key_reset HAL_GPIO_ReadPin(Key_reset_GPIO_Port,Key_reset_Pin)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
