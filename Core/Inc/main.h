/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define GPIO_LED_HEARTBEAT_Pin GPIO_PIN_2
#define GPIO_LED_HEARTBEAT_GPIO_Port GPIOE
#define GPIO_RADM_ONOFF_Pin GPIO_PIN_3
#define GPIO_RADM_ONOFF_GPIO_Port GPIOE
#define GPIO_VBUSOUT_EN_Pin GPIO_PIN_13
#define GPIO_VBUSOUT_EN_GPIO_Port GPIOC
#define AVIBUS_I2C2_SDA_Pin GPIO_PIN_0
#define AVIBUS_I2C2_SDA_GPIO_Port GPIOF
#define AVIBUS_I2C2_SCL_Pin GPIO_PIN_1
#define AVIBUS_I2C2_SCL_GPIO_Port GPIOF
#define GPIO_RADF_SYNCWORD_Pin GPIO_PIN_2
#define GPIO_RADF_SYNCWORD_GPIO_Port GPIOF
#define GPIO_RADF_CE_Pin GPIO_PIN_3
#define GPIO_RADF_CE_GPIO_Port GPIOF
#define GPIO_STACK_BUS_GOOD_Pin GPIO_PIN_7
#define GPIO_STACK_BUS_GOOD_GPIO_Port GPIOF
#define ERRATA_GPIO_PA_EN_Pin GPIO_PIN_8
#define ERRATA_GPIO_PA_EN_GPIO_Port GPIOF
#define GPIO_STACK_RBUS_INT0_Pin GPIO_PIN_9
#define GPIO_STACK_RBUS_INT0_GPIO_Port GPIOF
#define GPIO_STACK_IBUS_INT0_Pin GPIO_PIN_10
#define GPIO_STACK_IBUS_INT0_GPIO_Port GPIOF
#define GPIO_BTN0_Pin GPIO_PIN_10
#define GPIO_BTN0_GPIO_Port GPIOB
#define GPIO_BTN1_Pin GPIO_PIN_11
#define GPIO_BTN1_GPIO_Port GPIOB
#define GPIO_LED_ALLGOOD_Pin GPIO_PIN_6
#define GPIO_LED_ALLGOOD_GPIO_Port GPIOG
#define GPIO_LED_FAULT_Pin GPIO_PIN_7
#define GPIO_LED_FAULT_GPIO_Port GPIOG
#define GPIO_LEDC_Pin GPIO_PIN_9
#define GPIO_LEDC_GPIO_Port GPIOG
#define GPIO_LEDA_Pin GPIO_PIN_11
#define GPIO_LEDA_GPIO_Port GPIOG
#define GPIO_LEDB_Pin GPIO_PIN_12
#define GPIO_LEDB_GPIO_Port GPIOG
#define GPIO_IMU_INT0_Pin GPIO_PIN_4
#define GPIO_IMU_INT0_GPIO_Port GPIOB
#define GPIO_MAG_DRDY_Pin GPIO_PIN_5
#define GPIO_MAG_DRDY_GPIO_Port GPIOB
#define GPIO_RADPA_PWRCTL_Pin GPIO_PIN_6
#define GPIO_RADPA_PWRCTL_GPIO_Port GPIOB
#define GPIO_RADSW_CTL_Pin GPIO_PIN_7
#define GPIO_RADSW_CTL_GPIO_Port GPIOB
#define GPIO_RADF_MUX_Pin GPIO_PIN_8
#define GPIO_RADF_MUX_GPIO_Port GPIOB
#define GPIO_RADF_MUXOUT_Pin GPIO_PIN_9
#define GPIO_RADF_MUXOUT_GPIO_Port GPIOB
#define GPIO_RADF_TRCLK_Pin GPIO_PIN_0
#define GPIO_RADF_TRCLK_GPIO_Port GPIOE
#define GPIO_RADF_TRDATA_Pin GPIO_PIN_1
#define GPIO_RADF_TRDATA_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
