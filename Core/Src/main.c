#include "main.h"
#include <stdbool.h>

#define NUMBER_OF_LED_PINS 8
#define NUMBER_OF_RGB_PINS 3
#define NUMBER_OF_JOY_STICK_PINS 5
#define SPEED_OF_PINS 50
#define LED_0_PIN GPIO_PIN_6 // PC6
#define LED_0_PORT GPIOC
#define LED_1_PIN GPIO_PIN_7 // PC7
#define LED_1_PORT GPIOC
#define LED_2_PIN GPIO_PIN_8 // PC8
#define LED_2_PORT GPIOC
#define LED_3_PIN GPIO_PIN_9 // PC9
#define LED_3_PORT GPIOC
#define LED_4_PIN GPIO_PIN_4 // PE4
#define LED_4_PORT GPIOE
#define LED_5_PIN GPIO_PIN_3 // PD3
#define LED_5_PORT GPIOD
#define LED_6_PIN GPIO_PIN_5 // PE5
#define LED_6_PORT GPIOE
#define LED_7_PIN GPIO_PIN_6 // PE6
#define LED_7_PORT GPIOE
#define JOY_RT_PIN GPIO_PIN_0 // PE0
#define JOY_RT_PORT GPIOE
#define JOY_LT_PIN GPIO_PIN_1 // PE1
#define JOY_LT_PORT GPIOE
#define JOY_DN_PIN GPIO_PIN_2 // PE2
#define JOY_DN_PORT GPIOE
#define JOY_UP_PIN GPIO_PIN_3 // PE3
#define JOY_UP_PORT GPIOE
#define JOY_CNT_PIN GPIO_PIN_15 // PE15
#define JOY_CNT_PORT GPIOE
#define LED_R_PIN GPIO_PIN_13 // PD13
#define LED_R_PORT GPIOD
#define LED_G_PIN GPIO_PIN_8 // PB8
#define LED_G_PORT GPIOB
#define LED_B_PIN GPIO_PIN_12 // PD12
#define LED_B_PORT GPIOD
#define RED 0
#define GREEN 1
#define BLUE 2

typedef enum {
    JOYSTICK_IDLE,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
	JOYSTICK_CENTER
} JOYSTICK_State;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Pin;

const GPIO_Pin LED_PINS[NUMBER_OF_LED_PINS] = {
		{LED_0_PORT, LED_0_PIN},
		{LED_1_PORT, LED_1_PIN},
		{LED_2_PORT, LED_2_PIN},
		{LED_3_PORT, LED_3_PIN},
		{LED_4_PORT, LED_4_PIN},
		{LED_5_PORT, LED_5_PIN},
		{LED_6_PORT, LED_6_PIN},
		{LED_7_PORT, LED_7_PIN}
};

const GPIO_Pin JOY_STICK[NUMBER_OF_JOY_STICK_PINS] = {
		{JOY_RT_PORT, JOY_RT_PIN},
		{JOY_LT_PORT, JOY_LT_PIN},
		{JOY_DN_PORT, JOY_DN_PIN},
		{JOY_UP_PORT, JOY_UP_PIN},
		{JOY_CNT_PORT, JOY_CNT_PIN}
};

const GPIO_Pin LED_RGB[NUMBER_OF_RGB_PINS] = {
		{LED_R_PORT, LED_R_PIN},
		{LED_G_PORT, LED_G_PIN},
		{LED_B_PORT, LED_B_PIN}
};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void JOY_STICK_Init(void);
static void LED_PIN_Init(GPIO_Pin pin);
static void LEDS_PINS_Init(void);
static void RGB_LEDS_PIN_Init(void);
//void static Blink_Pin(GPIO_Pin pin);
static void Turn_On_Pin(GPIO_Pin pin);
static void Turn_Off_Pin(GPIO_Pin pin);
static void LED_JOY_Handler(void);
static void WAIT_WITH_INPUT(void);
JOYSTICK_State JOY_INPUT_Checker(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  while (1)
  {
	for(int i = 0;i < NUMBER_OF_LED_PINS;i++)
	{
		WAIT_WITH_INPUT();
		Turn_On_Pin(LED_PINS[i]);
		WAIT_WITH_INPUT();
		Turn_Off_Pin(LED_PINS[i]);
	}

	for(int i = NUMBER_OF_LED_PINS-1;i >= 0;i--)
	{
		WAIT_WITH_INPUT();
		Turn_On_Pin(LED_PINS[i]);
		WAIT_WITH_INPUT();
		Turn_Off_Pin(LED_PINS[i]);
	}
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	LEDS_PINS_Init();
	RGB_LEDS_PIN_Init();
	JOY_STICK_Init();
}

static void JOY_STICK_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(GPIOE, JOY_RT_PIN| JOY_LT_PIN | JOY_DN_PIN | JOY_UP_PIN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = JOY_RT_PIN| JOY_LT_PIN | JOY_DN_PIN | JOY_UP_PIN | JOY_CNT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

static void LED_PIN_Init(GPIO_Pin pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = pin.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}

static void LEDS_PINS_Init(void)
{
	for(int i = 0;i < NUMBER_OF_LED_PINS;i++)
	{
		LED_PIN_Init(LED_PINS[i]);
	}
}

static void RGB_LEDS_PIN_Init(void)
{
	for(int i = 0;i < NUMBER_OF_RGB_PINS;i++)
	{
		LED_PIN_Init(LED_RGB[i]);
	}
}

//static void Blink_Pin(GPIO_Pin pin)
//{
//	Turn_On_Pin(pin);
//	HAL_Delay(SPEED_OF_PINS);
//	Turn_Off_Pin(pin);
//	HAL_Delay(SPEED_OF_PINS);
//}

static void Turn_On_Pin(GPIO_Pin pin)
{
	HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_SET);
}

static void Turn_Off_Pin(GPIO_Pin pin)
{
	HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_RESET);
}

JOYSTICK_State JOY_INPUT_Checker(void)
{
	if(HAL_GPIO_ReadPin(GPIOE, JOY_LT_PIN) == GPIO_PIN_RESET)
	{
		return JOYSTICK_LEFT;
	}
	if(HAL_GPIO_ReadPin(GPIOE, JOY_DN_PIN) == GPIO_PIN_RESET)
	{
		return JOYSTICK_DOWN;
	}
	if(HAL_GPIO_ReadPin(GPIOE, JOY_RT_PIN) == GPIO_PIN_RESET)
	{
		return JOYSTICK_RIGHT;
	}
	if(HAL_GPIO_ReadPin(GPIOE, JOY_UP_PIN) == GPIO_PIN_RESET)
	{
		return JOYSTICK_UP;
	}
	if(HAL_GPIO_ReadPin(GPIOE, JOY_CNT_PIN) == GPIO_PIN_RESET)
	{
		return JOYSTICK_CENTER;
	}
	return JOYSTICK_IDLE;
}

static void LED_JOY_Handler(void)
{
	switch(JOY_INPUT_Checker())
	{
	case JOYSTICK_UP:
		Turn_On_Pin(LED_RGB[BLUE]);
		Turn_On_Pin(LED_RGB[RED]);
		break;
	case JOYSTICK_LEFT:
		Turn_On_Pin(LED_RGB[RED]);
		break;
	case JOYSTICK_DOWN:
		Turn_On_Pin(LED_RGB[GREEN]);
		break;
	case JOYSTICK_RIGHT:
		Turn_On_Pin(LED_RGB[BLUE]);
		break;
	case JOYSTICK_CENTER:
		Turn_On_Pin(LED_RGB[RED]);
		Turn_On_Pin(LED_RGB[GREEN]);
		Turn_On_Pin(LED_RGB[BLUE]);
		break;
	default:
		Turn_Off_Pin(LED_RGB[RED]);
		Turn_Off_Pin(LED_RGB[GREEN]);
		Turn_Off_Pin(LED_RGB[BLUE]);
	}
}

static void WAIT_WITH_INPUT(void)
{
	int loopTimer = 0;
	while(loopTimer < SPEED_OF_PINS)
	{
		loopTimer ++;
		HAL_Delay(1);
		LED_JOY_Handler();
	}
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	while(1);
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
