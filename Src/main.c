
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
char time[10];
char date[10];
char setalarm[10];
char dadoS[4];
char dadoS2[4];
RTC_DateTypeDef hDate;
RTC_TimeTypeDef hTime;
RTC_AlarmTypeDef hAlarm;


uint8_t dayweek =0;
uint8_t check;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}


void set_time (void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	/**Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 23;
	sTime.Minutes = 06;
	sTime.Seconds = 00;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/* USER CODE BEGIN RTC_Init 3 */

	/* USER CODE END RTC_Init 3 */

	sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
	sDate.Month = RTC_MONTH_DECEMBER;
	sDate.Date = 06;
	sDate.Year = 18;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/* USER CODE BEGIN RTC_Init 4 */



	/* USER CODE END RTC_Init 4 */
}
void set_alarm (void)
{
	RTC_AlarmTypeDef sAlarm;

	/**Enable the Alarm A
	 */
	sAlarm.AlarmTime.Hours = 12;
	sAlarm.AlarmTime.Minutes = 06;
	sAlarm.AlarmTime.Seconds = 0;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_SATURDAY;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/* USER CODE BEGIN RTC_Init 5 */

	/* USER CODE END RTC_Init 5 */
}
void get_time(void)
{
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	/* Display time Format: hh:mm:ss */
	sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

	/* Display date Format: mm-dd-yy */
	sprintf((char*)date,"%02d/%02d/%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);  // I like the date first
	dayweek = gDate.WeekDay;
}
void get_alarm(void)
{
	RTC_AlarmTypeDef gAlarm;


	HAL_RTC_GetAlarm(&hrtc, &gAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);


	/* Display time Format: hh:mm:ss */
	sprintf((char*)setalarm,"%02d:%02d:%02d",gAlarm.AlarmTime.Hours, gAlarm.AlarmTime.Minutes, gAlarm.AlarmTime.Seconds);


}
void display_alarm (void)
{

	SSD1306_GotoXY(0,25);
	SSD1306_Puts(setalarm,&Font_7x10,1);
	SSD1306_UpdateScreen();
}


//Let's display the time and date on lcd

void display_time (void)
{
	SSD1306_GotoXY(0,10);
	SSD1306_Puts(time,&Font_7x10,1);
	SSD1306_GotoXY(0,20);
	SSD1306_Puts(date,&Font_7x10,1);
	SSD1306_UpdateScreen();
	SSD1306_GotoXY(0,30);
	if(dayweek ==1){

		SSD1306_Puts("Monday", &Font_7x10,1);
	}

	else if(dayweek ==2){
		SSD1306_Puts("Tuesday", &Font_7x10,1);
	}

	else if(dayweek ==3){
		SSD1306_Puts("Wednesday", &Font_7x10,1);
	}

	else if(dayweek ==4){
		SSD1306_Puts("Thursday", &Font_7x10,1);
	}

	else if(dayweek ==5){
		SSD1306_Puts("Friday", &Font_7x10,1);
	}

	else if(dayweek ==6){
		SSD1306_Puts("Saturday", &Font_7x10,1);
	}

	else if(dayweek ==7){
		SSD1306_Puts("Sunday", &Font_7x10,1);
	}

	SSD1306_UpdateScreen();
}
void display_hour (void)
{

	SSD1306_GotoXY(0,25);
	SSD1306_Puts(time,&Font_7x10,1);
	SSD1306_UpdateScreen();
}

void display_date (void)
{

	SSD1306_GotoXY(0,25);
	SSD1306_Puts(date,&Font_7x10,1);
	SSD1306_UpdateScreen();
}
void display_week (void)
{

	SSD1306_GotoXY(0,25);
	if(dayweek ==1){

		SSD1306_Puts("Monday", &Font_7x10,1);
	}

	else if(dayweek ==2){
		SSD1306_Puts("Tuesday", &Font_7x10,1);
	}

	else if(dayweek ==3){
		SSD1306_Puts("Wednesday", &Font_7x10,1);
	}

	else if(dayweek ==4){
		SSD1306_Puts("Thursday", &Font_7x10,1);
	}

	else if(dayweek ==5){
		SSD1306_Puts("Friday", &Font_7x10,1);
	}

	else if(dayweek ==6){
		SSD1306_Puts("Saturday", &Font_7x10,1);
	}

	else if(dayweek ==7){
		SSD1306_Puts("Sunday", &Font_7x10,1);
	}
	SSD1306_UpdateScreen();
}






void to_do_on_alarm (void)
{
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("ALARM",&Font_7x10,1);
	SSD1306_UpdateScreen();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay (200);
	SSD1306_Fill(0);
	SSD1306_UpdateScreen();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
	check = SSD1306_Init();
	set_time();
	set_alarm ();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */




		HAL_RTC_GetAlarm(&hrtc, &hAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&hrtc, &hTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &hDate, RTC_FORMAT_BIN);
		hAlarm.AlarmDateWeekDay=hDate.WeekDay;
		char alan2=1;
		if((hAlarm.AlarmTime.Hours == hTime.Hours)&&(hAlarm.AlarmTime.Minutes==hTime.Minutes)&&(hTime.Seconds==0) )
		{
			while(alan2){
				SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
				SSD1306_UpdateScreen();
				to_do_on_alarm();

				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){


					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
						HAL_Delay(20);

					}
					alan2=0;
				}

			}


		}

		get_time();
		display_time();
		char aux = 0;
		char alan=0;

		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
			aux=1;

			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
				HAL_Delay(20);

			}
			SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
			SSD1306_UpdateScreen();

			while(aux){ // Setar horas
				//mostrar horas
				get_time();
				display_hour();
				SSD1306_GotoXY(0,15);
				SSD1306_Puts("..",&Font_7x10,1);
				SSD1306_GotoXY(0,8);
				SSD1306_Puts("Set Hours",&Font_7x10,1);
				SSD1306_UpdateScreen();
				HAL_RTC_GetTime(&hrtc, &hTime, RTC_FORMAT_BIN);

				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){

					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
						HAL_Delay(20);
					}


					hTime.Hours ++;
					if(hTime.Hours==24){
						hTime.Hours = 0;
					}

					HAL_RTC_SetTime(&hrtc, &hTime, RTC_FORMAT_BIN);


				}

				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
						HAL_Delay(20);
					}
					SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
					SSD1306_UpdateScreen();
					while(aux){// Setar minutos
						//mostra horas
						get_time();
						display_hour();
						SSD1306_GotoXY(21,15);
						SSD1306_Puts("..",&Font_7x10,1);
						SSD1306_GotoXY(0,8);
						SSD1306_Puts("Set Minutes",&Font_7x10,1);
						SSD1306_UpdateScreen();

						HAL_RTC_GetTime(&hrtc, &hTime, RTC_FORMAT_BIN);

						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
							while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
								HAL_Delay(20);
							}


							hTime.Minutes ++;
							if(hTime.Minutes==60){
								hTime.Minutes = 0;
							}
							HAL_RTC_SetTime(&hrtc, &hTime, RTC_FORMAT_BIN);

						}

						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){

							while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
								HAL_Delay(20);
							}
							SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
							SSD1306_UpdateScreen();
							while(aux){//setar dia
								//mostrar data
								get_time();
								display_date();
								SSD1306_GotoXY(0,15);
								SSD1306_Puts("..",&Font_7x10,1);
								SSD1306_GotoXY(0,8);
								SSD1306_Puts("Set Day",&Font_7x10,1);
								SSD1306_UpdateScreen();
								HAL_RTC_GetDate(&hrtc, &hDate, RTC_FORMAT_BIN);

								if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
									while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
										HAL_Delay(20);
									}


									hDate.Date ++;
									if((hDate.Date==32) && ((hDate.Month==1 ) || (hDate.Month==3 ) || (hDate.Month==5 ) || (hDate.Month==7 )|| (hDate.Month==8 )|| (hDate.Month==10 )|| (hDate.Month==12 ))){
										hDate.Date = 1;
									}
									if((hDate.Date==30) &&(hDate.Month==2 ) ){
										hDate.Date = 1;
									}
									if((hDate.Date==31) && ((hDate.Month==4 ) || (hDate.Month==6 ) || (hDate.Month==9 )|| (hDate.Month==11 ))){
										hDate.Date = 1;
									}

									HAL_RTC_SetDate(&hrtc, &hDate, RTC_FORMAT_BIN);

								}
								if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
									while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
										HAL_Delay(20);
									}
									SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
									SSD1306_UpdateScreen();
									while(aux){//setar mes
										//mostrar data
										get_time();
										display_date();
										SSD1306_GotoXY(21,15);
										SSD1306_Puts("..",&Font_7x10,1);
										SSD1306_GotoXY(0,8);
										SSD1306_Puts("Set Month",&Font_7x10,1);
										SSD1306_UpdateScreen();

										HAL_RTC_GetDate(&hrtc, &hDate, RTC_FORMAT_BIN);
										uint8_t mes=hDate.Month;

										if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
											while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
												HAL_Delay(20);
											}

											mes ++;
											if(mes==13){
												mes = RTC_MONTH_JANUARY;
											}
											hDate.Month = mes;
											HAL_RTC_SetDate(&hrtc, &hDate, RTC_FORMAT_BIN);


										}
										if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){

											while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
												HAL_Delay(20);
											}
											SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
											SSD1306_UpdateScreen();
											while(aux){//setar dia da semana
												//mostrar dia da semana

												get_time();
												display_week();
												HAL_RTC_GetDate(&hrtc, &hDate, RTC_FORMAT_BIN);
												uint8_t aux1 = hDate.WeekDay;
												if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
													while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
														HAL_Delay(20);
													}

													aux1 ++;
													if(aux1==8){
														aux1=1;
													}
													hDate.WeekDay=aux1;
													HAL_RTC_SetDate(&hrtc, &hDate, RTC_FORMAT_BIN);
													SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
													SSD1306_UpdateScreen();

												}
												if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){

													while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
														HAL_Delay(20);
													}
													SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
													SSD1306_UpdateScreen();
													aux=0;
												}


											}
										}

									}
								}
							}
						}


					}
				}


			}
		}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
			alan=1;
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
				HAL_Delay(20);
			}
			SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
			SSD1306_UpdateScreen();
			while(alan){

				get_alarm();
				display_alarm();
				SSD1306_GotoXY(0,15);
				SSD1306_Puts("..",&Font_7x10,1);
				SSD1306_GotoXY(0,8);
				SSD1306_Puts("Set Alarm",&Font_7x10,1);
				SSD1306_UpdateScreen();
				HAL_RTC_GetAlarm(&hrtc, &hAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);
				int alhour = hAlarm.AlarmTime.Hours;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
						HAL_Delay(20);
					}
					alhour ++;
					if(alhour==24){
						alhour=0;
					}
					hAlarm.AlarmTime.Hours=alhour;
					HAL_RTC_SetAlarm(&hrtc, &hAlarm, RTC_FORMAT_BIN);

				}
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
						HAL_Delay(20);
					}
					SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
					SSD1306_UpdateScreen();
					while(alan){
						get_alarm();
						display_alarm();
						SSD1306_GotoXY(21,15);
						SSD1306_Puts("..",&Font_7x10,1);
						SSD1306_GotoXY(0,8);
						SSD1306_Puts("Set Alarm",&Font_7x10,1);
						SSD1306_UpdateScreen();
						HAL_RTC_GetAlarm(&hrtc, &hAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);
						int almin = hAlarm.AlarmTime.Minutes ;

						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
							while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)){
								HAL_Delay(20);
							}
							almin ++;
							if(almin==60){
								almin=1;
							}
							hAlarm.AlarmTime.Minutes = almin;
							HAL_RTC_SetAlarm(&hrtc, &hAlarm, RTC_FORMAT_BIN);

						}
						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){

							while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)){
								HAL_Delay(20);
							}
							SSD1306_Fill(SSD1306_COLOR_BLACK);// Tela preta
							SSD1306_UpdateScreen();
							alan=0;
						}

					}

				}

			}
			alan=0;
		}
		HAL_ADC_Start(&hadc);
		int dado =HAL_ADC_GetValue(&hadc);
		HAL_Delay(20);
		HAL_ADC_Start(&hadc);
		int dado2 =HAL_ADC_GetValue(&hadc);
		//intToStr(dado,dadoS,4);
		//intToStr(dado2,dadoS2,4);
		//SSD1306_GotoXY(93,40);
		//SSD1306_Puts(dadoS,&Font_7x10,1);

		//SSD1306_GotoXY(93,50);
		//SSD1306_Puts(dadoS2,&Font_7x10,1);
		//SSD1306_UpdateScreen();
		if((dado-dado2)>=500){
			SSD1306_GotoXY(93,20);
			SSD1306_Puts("<3",&Font_7x10,1);
			SSD1306_UpdateScreen();
			SSD1306_GotoXY(93,20);
			SSD1306_Puts("  ",&Font_7x10,1);
			SSD1306_UpdateScreen();

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC init function */
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  RTC_AlarmTypeDef sAlarm;

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enable the Alarm A 
    */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
