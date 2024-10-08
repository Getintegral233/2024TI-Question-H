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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>
#include "Encoder.h"
#include "oled.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void xunji(void);
void key_work(void);
void mode_state(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int16_t encoder_num_left=0,encoder_num_right=0;
int16_t encoder_target_left=0,encoder_target_right=0;
PID_Increment_Struct PID_left,PID_right,*PID_left1,*PID_right1;
int left_pwm=0,right_pwm=0,encoder_count_left=0,encoder_count_right=0;
uint8_t printf_flag=0,mode=10,usart_num=0,usart_flag=0,xunji_flag=0;
uint8_t usart_rx[30]={0},rx_data=0,reset_str[]={0xFF,0xAA,0x67};
int yaw=0,yaw0=0,yaw1=0;
float yaw_sourse=0.0;
uint8_t count_mode=0,L_S_flag=0,L_S_work=0;
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim2);
	
	
	PID_left.Kp=28.3407911607792;
	PID_left.Ki=4.5387430800153;
	PID_left.Kd=0;
	PID_left1 = &PID_left;
	
	PID_right.Kp=28.3407911607792;
	PID_right.Ki=4.5387430800153;
	PID_right.Kd=-0.663541586688759;
	PID_right1 = &PID_right;
//	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port,MOTOR_1_Pin,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port,MOTOR_2_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port,MOTOR_3_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port,MOTOR_4_Pin,GPIO_PIN_SET);
//	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,1000);
//	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,1000);



	HAL_UART_Receive_IT(&huart1,(uint8_t *)&rx_data,1);
	
	OLED_Init();
//	OLED_ShowChar(1,1,'a');
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  if(printf_flag==1)
//	  {
//		  printf_flag=0;
//		  printf("d:%f,%f\n",(double)encoder_num_left,(double)encoder_num_right);
//	  }
//	  xunji();
//	  if(mode==1)
//	  {
//		  encoder_target_right=40;
//		  encoder_target_left=40;
//	  }
	  key_work();
	  mode_state();
	  if(usart_flag)
	  {
		  usart_flag=0;
		  if(usart_rx[0]==0x55)
		  {
			  if(usart_rx[1]==0x53)
			  {
				  yaw_sourse=(usart_rx[7]<<8|usart_rx[6])*180/32768;
				  if(yaw_sourse)
				  yaw=(int)yaw_sourse;
			  }
		  }
		  OLED_ShowString(1,1,"yaw:");
		  OLED_ShowNum(1,5,yaw,4);
		  OLED_ShowString(2,1,"yaw0:");
		  OLED_ShowNum(2,6,yaw0,4);
		  OLED_ShowString(3,1,"yaw1:");
		  OLED_ShowNum(3,6,yaw1,4);
		  OLED_ShowString(4,1,"mode:");
		  OLED_ShowNum(4,6,mode,3);
	  }
	  if(L_S_work==1)
	  {
		  HAL_GPIO_WritePin(L_S_GPIO_Port,L_S_Pin,GPIO_PIN_SET);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(L_S_GPIO_Port,L_S_Pin,GPIO_PIN_RESET);
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */

void Get_num(void)//left
{
	encoder_num_left=__HAL_TIM_GetCounter(&htim3);
	__HAL_TIM_SetCounter(&htim3,0);
}

void Get_num2(void)//right
{
	encoder_num_right=__HAL_TIM_GetCounter(&htim4);
	__HAL_TIM_SetCounter(&htim4,0);
}

uint8_t encoder_timer=0,usart_timer=0,L_S_timer=0;
uint16_t motor_time=0,xunji_flag_timer=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		
//		usart_timer++;
//		if(usart_timer>=5)
//		{
//			usart_timer=0;
//			usart_flag=1;
//		}
		if(L_S_flag==1)
		{
			L_S_work=1;
			L_S_timer++;
			if(L_S_timer>=250)
			{
				L_S_timer=0;
				L_S_flag=0;
				L_S_work=0;
			}
		}
		
		if(mode==11||mode==21||mode==23)
		{
			if(yaw<yaw0)
			{
				encoder_target_left=20;
				encoder_target_right=18;
			}
			else if(yaw>yaw0)
			{
				encoder_target_left=18;
				encoder_target_right=20;
			}
			else if(yaw==yaw0)
			{
				encoder_target_left=20;
				encoder_target_right=20;
			}
		}
		else if(mode==31||mode==35||mode==41||mode==45)
		{
			if(yaw<yaw0)
			{
				encoder_target_left=3;
				encoder_target_right=-3;
			}
			else if(yaw>yaw0)
			{
				encoder_target_left=-3;
				encoder_target_right=3;
			}
			else if(yaw==yaw0)
			{
				encoder_target_left=0;
				encoder_target_right=0;
			}
		}
		else if(mode==33||mode==37||mode==43||mode==47)
		{
			if(yaw<yaw0)
			{
				encoder_target_left=10;
				encoder_target_right=1;
			}
			else if(yaw>yaw0)
			{
				encoder_target_left=1;
				encoder_target_right=10;
			}
			else if(yaw==yaw0)
			{
				encoder_target_left=10;
				encoder_target_right=10;
			}
		}
		else if(mode==32||mode==36||mode==42||mode==46)
		{
			if(yaw<yaw0)
			{
				encoder_target_left=32;
				encoder_target_right=25;
			}
			else if(yaw>yaw0)
			{
				encoder_target_left=25;
				encoder_target_right=32;
			}
			else 
			{
				encoder_target_left=30;
				encoder_target_right=30;
			}
		}
		encoder_timer++;
		if(encoder_timer>=5)//PID调参  right和left是反的
		{
			encoder_timer=0;
			usart_timer=0;
			usart_flag=1;
//			printf_flag=1;
			Get_num();
			Get_num2();
			if(mode==32||mode==36||mode==42||mode==46)
			{
				encoder_count_left+=encoder_num_left;
				encoder_count_right+=encoder_num_right;
				
			}
			if(encoder_target_left!=0)
			{
				left_pwm=(int)PID_Increment(PID_left1,encoder_num_left,encoder_target_left);
			}
			else
			{
				left_pwm=0;
			}
			if(encoder_target_right!=0)
			{
				right_pwm=(int)PID_Increment(PID_right1,encoder_num_right,encoder_target_right);
			}
			else
			{
				right_pwm=0;
			}
			
//			if(mode==31||mode==33||mode==35||mode==37||mode==41||mode==43||mode==45||mode==47)
//			{
//				if(yaw<yaw0)
//				{
//					left_pwm=220;
//					right_pwm=-220;
//				}
//				else if(yaw>yaw0)
//				{
//					left_pwm=-220;
//					right_pwm=220;
//				}
//				else if(yaw==yaw0)
//				{
//					left_pwm=0;
//					right_pwm=0;
//				}
//			}
			set_pwm_left(left_pwm);
			set_pwm_right(right_pwm);
		}
		if(xunji_flag)
		{
			xunji_flag_timer++;
			if(xunji_flag_timer>=1000)
			{
				xunji_flag_timer=0;
				xunji_flag=0;
			}
		}
	}
}

void xunji(void)
{
	if(G1==1&&G2==0&&G3==0&&G4==0&&G5==0&&G6==0&&G7==0)//最左侧检测  right和left是反的
	{
		encoder_target_right=1;
		encoder_target_left=30;
	}
	if(G1==1&&G2==1&&G3==0&&G4==0&&G5==0&&G6==0&&G7==0)
	{
		encoder_target_right=5;
		encoder_target_left=30;
	}
	if(G1==0&&G2==1&&G3==0&&G4==0&&G5==0&&G6==0&&G7==0)
	{
		encoder_target_right=10;
		encoder_target_left=30;
	}
	if(G1==0&&G2==1&&G3==1&&G4==0&&G5==0&&G6==0&&G7==0)
	{
		encoder_target_right=15;
		encoder_target_left=30;
	}
	if(G1==0&&G2==0&&G3==1&&G4==0&&G5==0&&G6==0&&G7==0)
	{
		encoder_target_right=20;
		encoder_target_left=30;
	}
	if(G1==0&&G2==0&&G3==1&&G4==1&&G5==0&&G6==0&&G7==0)
	{
		encoder_target_right=25;
		encoder_target_left=30;
	}
	if(G1==0&&G2==0&&G3==0&&G4==1&&G5==0&&G6==0&&G7==0)//中间检测
	{
		encoder_target_right=30;
		encoder_target_left=30;
	}
	if(G1==0&&G2==0&&G3==0&&G4==1&&G5==1&&G6==0&&G7==0)
	{
		encoder_target_right=30;
		encoder_target_left=25;
	}
	if(G1==0&&G2==0&&G3==0&&G4==0&&G5==1&&G6==0&&G7==0)
	{
		encoder_target_right=30;
		encoder_target_left=20;
	}
	if(G1==0&&G2==0&&G3==0&&G4==0&&G5==1&&G6==1&&G7==0)
	{
		encoder_target_right=30;
		encoder_target_left=15;
	}
	if(G1==0&&G2==0&&G3==0&&G4==0&&G5==0&&G6==1&&G7==0)
	{
		encoder_target_right=30;
		encoder_target_left=10;
	}
	if(G1==0&&G2==0&&G3==0&&G4==0&&G5==0&&G6==1&&G7==1)
	{
		encoder_target_right=30;
		encoder_target_left=5;
	}
	if(G1==0&&G2==0&&G3==0&&G4==0&&G5==0&&G6==0&&G7==1)//最右侧检测
	{
		encoder_target_right=30;
		encoder_target_left=1;
	}
	
}

void mode_state(void)
{
	if(mode==10)
	{
		
	}
	else if(mode==11)
	{
//		encoder_target_right=20;
//		encoder_target_left=20;
		yaw0=yaw1;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			encoder_target_right=1;
			encoder_target_left=1;
			mode=20;
			L_S_flag=1;
		}
	}
	else if(mode==20)
	{
		if(encoder_num_right==1)
			encoder_target_right=0;
		if(encoder_num_left==1)
		encoder_target_left=0;
	}
	else if(mode==21)
	{
//		encoder_target_right=20;
//		encoder_target_left=20;
		yaw0=yaw1;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			mode=22;
			L_S_flag=1;
		}
	}
	else if(mode==22)
	{
		xunji();
		if(!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)
		{
			mode=23;
			yaw0=yaw0-180;
			L_S_flag=1;
		}
	}
	else if(mode==23)
	{
//		encoder_target_right=20;
//		encoder_target_left=20;
		
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			mode=24;
			L_S_flag=1;
		}
	}
	else if(mode==24)
	{
		xunji();
		if(!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)
		{
			mode=30;
			encoder_target_right=1;
			encoder_target_left=1;
			L_S_flag=1;
		}
	}
	else if(mode==30)
	{
		if(encoder_num_right==1)
			encoder_target_right=0;
		if(encoder_num_left==1)
		encoder_target_left=0;
	}
	else if(mode==31)
	{
		yaw0=yaw1-38.6;
		if(yaw==yaw0)
		{
			mode=32;
		}
	}
	else if(mode==32)
	{
		if(encoder_count_left>=6800||encoder_count_right>=6800)
		{
			mode=33;
			encoder_count_left=0;
			encoder_count_right=0;
//			xunji_flag=1;
		}
//		if(G1||G2||G3||G4||G5||G6||G7)
//		{
//			mode=33;
//			xunji_flag=1;
//		}
	}
	else if(mode==33)
	{
		yaw0=yaw1;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			mode=34;
			xunji_flag=1;
			L_S_flag=1;
		}
	}
	else if(mode==34)
	{
		xunji();
		if((!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)&&(xunji_flag==0))
		{
			encoder_target_right=-1;
			encoder_target_left=-1;
			
			mode=35;
			yaw0=yaw1-143;
			L_S_flag=1;
		}
	}
	else if(mode==35)
	{
		if(yaw==yaw0)
		{
			mode=36;
		}
	}
	else if(mode==36)
	{
		
		if(encoder_count_left>=6700||encoder_count_right>=6700)
		{
			mode=37;
			encoder_count_left=0;
			encoder_count_right=0;
//			xunji_flag=1;
		}

//		if(G1||G2||G3||G4||G5||G6||G7)
//		{
//			mode=37;
//			xunji_flag=1;
//		}
	}
	else if(mode==37)
	{
		yaw0=yaw1-180;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			mode=38;
			xunji_flag=1;
			L_S_flag=1;
		}
	}
	else if(mode==38)
	{
		xunji();
		if((!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)&&(xunji_flag==0))
		{
			mode=40;
			encoder_target_right=-1;
			encoder_target_left=-1;
			L_S_flag=1;
		}
	}
	else if(mode==40)
	{
		if(encoder_num_right==-1)
			encoder_target_right=0;
		if(encoder_num_left==-1)
		encoder_target_left=0;
	}
	else if(mode==41)
	{
		yaw0=yaw1-38.6;
		if(yaw==yaw0)
		{
			mode=42;
		}
	}
	else if(mode==42)
	{
		if(encoder_count_left>=6800||encoder_count_right>=6800)
		{
			mode=43;
			encoder_count_left=0;
			encoder_count_right=0;
//			xunji_flag=1;
		}
	}
	else if(mode==43)
	{
		yaw0=yaw1;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			mode=44;
			xunji_flag=1;
			L_S_flag=1;
		}
	}
	else if(mode==44)
	{
		xunji();
		if((!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)&&(xunji_flag==0))
		{
			encoder_target_right=-1;
			encoder_target_left=-1;
			L_S_flag=1;
			
			yaw0=yaw1-143;
			mode=45;
		}
	}
	else if(mode==45)
	{
		if(yaw==yaw0)
		{
			mode=46;
		}
	}
	else if(mode==46)
	{
		if(encoder_count_left>=6700||encoder_count_right>=6700)
		{
			
			encoder_count_left=0;
			encoder_count_right=0;
			mode=47;
//			xunji_flag=1;
		}
	}
	else if(mode==47)
	{
		yaw0=yaw1-180;
		if(G1||G2||G3||G4||G5||G6||G7)
		{
			
			xunji_flag=1;
			L_S_flag=1;
			mode=48;
		}
	}
	else if(mode==48)
	{
		xunji();
		if((!G1&&!G2&&!G3&&!G4&&!G5&&!G6&&!G7)&&(xunji_flag==0))
		{
			encoder_target_right=-1;
			encoder_target_left=-1;
			L_S_flag=1;
			count_mode++;
			if(count_mode>=4)
			{
				mode=50;
			}
			else
			{
				mode=41;
				yaw0=yaw1-38.6;
			}
			
//			yaw0=yaw1-38.6;
		}
		
	}
	else if(mode==50)
	{
		if(encoder_num_right==-1)
			encoder_target_right=0;
		if(encoder_num_left==-1)
			encoder_target_left=0;
	}
}

void key_work(void)
{
	if(Key_mode==0)
	{
		HAL_Delay(20);
		if(Key_mode==0)
		{
			while(Key_mode==0);
			if(mode%10==0)
				mode=mode+1;
		}

	}
	if(Key_yaw==0)
	{
		HAL_Delay(20);
		if(Key_yaw==0)
		{
			while(Key_yaw==0);
			yaw1=yaw;
		}
	}
	if(Key_reset==0)
	{
		HAL_Delay(20);
		if(Key_reset==0)
		{
			while(Key_reset==0);
			HAL_UART_Transmit(&huart1,reset_str,3,100);
		}
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{
//		
//		
//		
//	}
//	
//}
/* USER CODE END 4 */

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
