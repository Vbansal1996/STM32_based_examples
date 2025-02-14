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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "certificates.h"

#define AT_CHECK 						"AT\r\n"
#define AT_CHECK_RESP					"OK\0"
#define SIM_ECO_MODE_OFF				"ATE0\r\n"
#define SIM_ECO_MODE_OFF_RESP			"OK\0"
#define SIM_BAUD_115200					"AT+IPR=115200&W\r\n"
#define SIM_BAUD_115200_RESP			"OK\0"
#define SIM_READY 						"AT+CPIN?\r\n"
#define SIM_READY_RESP	 				"+CPIN: READY"
#define SIM_CREG						"AT+CREG?\r\n"
#define SIM_CGREG						"AT+CGREG?\r\n"
#define SIM_EN_TCPIP_MODE				"AT+QIMODE=0\r\n"
#define SIM_EN_TCPIP_MODE_RESP			"OK\0"
#define SIM_APN_SET						"AT+QICSGP=1,\"Airteliot.com\"\r\n"
#define SIM_APN_SET_RESP				"OK\0"
#define SIM_TCPIP_STACK_START			"AT+QIREGAPP= \"Airteliot.com\"\r\n"
#define SIM_TCPIP_STACK_START_RESP		"OK"
#define SIM_QUERY_CONN_MODE				"AT+QICSGP?\r\n"
#define SIM_EN_GPRS						"AT+QIACT\r\n"
#define SIM_EN_GPRS_RESP				"OK"
#define SIM_QUERY_LOCAL_IP				"AT+QILOCIP\r\n"
#define SIM_DEL_OLD_CA_CERT				"AT+QSECDEL=\"RAM:root-CA\"\r\n"
#define SIM_DEL_OLD_CA_CERT_RESP		"OK"
#define SIM_DEL_OLD_CLIENT_CERT			"AT+QSECDEL=\"RAM:Cleint-certificate.pem.crt\"\r\n"
#define SIM_DEL_OLD_CLIENT_CERT_RESP	"OK"
#define SIM_DEL_OLD_USERKEY_CERT		"AT+QSECDEL=\"RAM:ClientKey-private.pem\"\r\n"
#define SIM_DEL_OLD_USERKEY_CERT_RESP	"OK"
#define SIM_SSL_EN						"AT+QMTCFG=\"SSL\",1,1,2\r\n"
#define SIM_SSL_EN_RESP					"OK"
#define SIM_STORE_CA_CERT_RAM			"AT+QSECWRITE=\"RAM:root-CA\",%d,%d\r\n" // needs reconfiguration
#define SIM_STORE_CA_CERT_RAM_RESP		"CONNECT"
#define SIM_CA_CERT_PAYLOAD_RESP		"+QSECWRITE"
#define SIM_STORE_CLIENT_CERT_RAM		"AT+QSECWRITE=\"RAM:Cleint-certificate.pem.crt\",%d,%d\r\n" // needs reconfiguration
#define SIM_STORE_CLIENT_CERT_RAM_RESP	"CONNECT"
#define SIM_CLIENT_CERT_PAYLOAD_RESP	"+QSECWRITE"
#define SIM_STORE_USR_KEY_CERT_RAM		"AT+QSECWRITE=\"RAM:ClientKey-private.pem\",%d,%d\r\n" // needs reconfiguration
#define SIM_STORE_USR_KEY_CERT_RAM_RESP	"CONNECT"
#define SIM_USR_KEY_CERT_PAYLOAD_RESP	"+QSECWRITE"
#define SIM_VALIDATE_CA_CHECKSUM		"AT+QSECREAD=\"RAM:root-CA\"\r\n"
#define SIM_VALIDATE_CA_CHECKSUM_RESP	"+QSECREAD:"
#define SIM_VALIDATE_CLIENT_CHECKSUM		"AT+QSECREAD=\"RAM:Cleint-certificate.pem.crt\"\r\n"
#define SIM_VALIDATE_CLIENT_CHECKSUM_RESP	"+QSECREAD:"
#define SIM_VALIDATE_USR_KEY_CHECKSUM		"AT+QSECREAD=\"RAM:ClientKey-private.pem\"\r\n"
#define SIM_VALIDATE_USR_KEY_CHECKSUM_RESP	"+QSECREAD:"
#define SIM_SET_CA_CERT_LOC				"AT+QSSLCFG=\"cacert\",2,\"RAM:root-CA\"\r\n"
#define SIM_SET_CA_CERT_LOC_RESP		"OK"
#define	SIM_SET_CLIENT_CERT_LOC			"AT+QSSLCFG=\"clientcert\",2,\"RAM:Cleint-certificate.pem.crt\"\r\n"
#define	SIM_SET_CLIENT_CERT_LOC_RESP	"OK"
#define SIM_SET_USER_KEY_CERT_LOC		"AT+QSSLCFG=\"clientkey\",2,\"RAM:ClientKey-private.pem\"\r\n"
#define SIM_SET_USER_KEY_CERT_LOC_RESP	"OK"
#define SIM_SET_SECURITY_LEVEL			"AT+QSSLCFG=\"seclevel\",2,2\r\n"
#define SIM_SET_SECURITY_LEVEL_RESP		"OK"
#define SIM_SET_SSL_VERSION				"AT+QSSLCFG=\"sslversion\",2,4\r\n"
#define SIM_SET_SSL_VERSION_RESP		"OK"
#define SIM_SET_CIPHER_SUITES			"AT+QSSLCFG=\"ciphersuite\",2,\"0xFFFF\"\r\n"
#define SIM_SET_CIPHER_SUITES_RESP		"OK"
#define SIM_SET_IGNORE_RTC_TIME			"AT+QSSLCFG=\"ignorertctime\",1\r\n"
#define SIM_SET_IGNORE_RTC_TIME_RESP	"OK"
#define SIM_OPEN_MQTT_CONNECTION		"AT+QMTOPEN=1,\"%s\",%d\r\n" //url and port
#define SIM_OPEN_MQTT_CONNECTION_RESP	"OK"
#define SIM_OPEN_MQTT_CONNECTION_RESP2	"+QMTOPEN: 1,0"
#define SIM_SEND_USER_ID				"AT+QMTCONN=1,\"%s\"\r\n" //user ID
#define SIM_SEND_USER_ID_RESP			"OK"
#define SIM_SEND_USER_ID_RESP2			"+QMTCONN: 1,0,0"
#define SIM_MQTT_SUBSCRIBE				"AT+QMTSUB=1,1,\"%s\",1\r\n" // subscribe topic
#define SIM_MQTT_SUBSCRIBE_RESP			"OK"
#define SIM_MQTT_SUBSCRIBE_RESP2		"+QMTSUB: 1,1,0,1"
#define SIM_MQTT_PUBLISH_DATA			"AT+QMTPUB=1,1,1,0,\"%s\",%d\r\n" // publish topic
#define SIM_MQTT_PUBLISH_DATA_RESP		">"
#define SIM_MQTT_PUBLISH_DATA_RESP1		"OK"
#define SIM_MQTT_PUBLISH_DATA_RESP2		"+QMTPUB: 1,1,0"
#define SIM_MQTT_DATA_RECV				"+QMTRECV:"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void gsmPower(bool state);
void powerKey(bool state);
bool checkSimStatus();
bool connectToAWS();
bool configureSSL();
bool uploadCertificates();
bool enableGPRS();
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

typedef enum State
{
	disable = 0,
	enable,
}State_t;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief This function handles USART1 global interrupt.
  */
uint8_t Data[1000] = {0};
int counter = 0;

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  Data[counter++] = USART1->RDR;
  if(counter > 999)
	  counter = 0;
/*  if(Data[counter - 1] == '\n' && Data[counter - 2] == '\r' && counter >= 4)
  {
	  counter = 0;
	  ResponseReceived = true;
  }*/
 // printf(" Data received = %c\n",(char)data);

  /* USER CODE END USART1_IRQn 1 */
}

void transmit_data(char *data, uint16_t length)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)data,length,100);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	initialise_monitor_handles();
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
  gsmPower(enable);
  powerKey(disable);
  HAL_Delay(100);
  powerKey(enable);
  HAL_Delay(1000);
  powerKey(disable);
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
  HAL_Delay(5000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //memset(Data,0,100);
  //counter = 0;
  if(checkSimStatus() == false)
	  NVIC_SystemReset();

  if(enableGPRS() == false)
	  NVIC_SystemReset();

  if(uploadCertificates() == false)
	  NVIC_SystemReset();

  if(configureSSL() == false)
	  NVIC_SystemReset();

  if(connectToAWS() == false)
	  NVIC_SystemReset();

	  /*
	   * bool connectToAWS();
	   * bool configureSSL();
	   * bool uploadCertificates();
	   * bool enableGPRS();
	   */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		transmit_data("AT+QMTCONN?\r\n",strlen("AT+QMTCONN?\r\n"));
		HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void gsmPower(bool state)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,state);
}

void powerKey(bool state)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,state);
}

bool checkSimStatus()
{
	retry:
	//transmit_data("AT+CMEE=2\r\n",strlen("AT+CMEE=2\r\n"));
	//HAL_Delay(1000);

	transmit_data("AT+QGNSSC=0\r\n",strlen("AT+QGNSSC=0\r\n"));
	HAL_Delay(1000);

	transmit_data(AT_CHECK,strlen(AT_CHECK));
	HAL_Delay(300);
	if(strstr((char *)Data,AT_CHECK_RESP) == 0)
		goto retry;

	transmit_data(SIM_ECO_MODE_OFF,strlen(SIM_ECO_MODE_OFF));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_ECO_MODE_OFF_RESP) == 0)
		goto retry;

	transmit_data(SIM_BAUD_115200,strlen(SIM_BAUD_115200));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_BAUD_115200_RESP) == 0)
		goto retry;

	/*HAL_Delay(300);
	transmit_data(SIM_READY,strlen(SIM_READY));
	while(!ResponseReceived);
	ResponseReceived = false;
	if(strstr((char *)Data,SIM_READY_RESP) == 0)
		goto retry;*/

	transmit_data(SIM_CREG,strlen(SIM_CREG));
	HAL_Delay(300);

	transmit_data(SIM_CGREG,strlen(SIM_CGREG));
	HAL_Delay(300);

	memset(Data,'_', sizeof(Data));
	return true;
}

bool enableGPRS()
{
	//select TCP IP stack mode
	retry1:
	transmit_data(SIM_EN_TCPIP_MODE,strlen(SIM_EN_TCPIP_MODE));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_EN_TCPIP_MODE_RESP) == 0)
		goto retry1;

	//Configure APN
	retry2:
	transmit_data(SIM_APN_SET,strlen(SIM_APN_SET));
	HAL_Delay(1000);
	if(strstr((char *)Data,SIM_APN_SET_RESP) == NULL)
		goto retry2;

	//start TCP IP Stack
	retry3:
	transmit_data(SIM_TCPIP_STACK_START,strlen(SIM_TCPIP_STACK_START));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_TCPIP_STACK_START_RESP) == 0)
		goto retry3;

	//check current connection mode
	transmit_data(SIM_QUERY_CONN_MODE,strlen(SIM_QUERY_CONN_MODE));
	HAL_Delay(10000);

	// enable gprs context
	retry4:
	transmit_data(SIM_EN_GPRS,strlen(SIM_EN_GPRS));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_EN_GPRS_RESP) == 0)
		goto retry4;

	memset(Data,'_', sizeof(Data));
	return true;
}

bool uploadCertificates()
{
	// Delete OLD certificates
	/*transmit_data(SIM_DEL_OLD_CA_CERT,strlen(SIM_DEL_OLD_CA_CERT));
	HAL_Delay(300);

	transmit_data(SIM_DEL_OLD_CLIENT_CERT,strlen(SIM_DEL_OLD_CLIENT_CERT));
	HAL_Delay(300);

	transmit_data(SIM_DEL_OLD_USERKEY_CERT,strlen(SIM_DEL_OLD_USERKEY_CERT));
	HAL_Delay(300);*/

	// Send SSL Command
	retry8:
	HAL_Delay(2000);
	transmit_data(SIM_SSL_EN,strlen(SIM_SSL_EN));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SSL_EN_RESP) == 0)
		goto retry8;

	//Write CA Certificate command'
	char data[100];
	retry9:
	sprintf(data,SIM_STORE_CA_CERT_RAM,strlen(CA_Certificate),100);
	transmit_data(data,strlen(data));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_STORE_CA_CERT_RAM_RESP) == 0)
		goto retry9;

	HAL_Delay(300);
	retry10:
	for(int i=0;i<strlen(CA_Certificate);i++)
	{
		transmit_data(CA_Certificate + i,1);
	}
	//Validate the data write response
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_CA_CERT_PAYLOAD_RESP) == 0)
		goto retry10;

	//Write Client Certificate command
	retry11:
	memset(data,0,sizeof(data));
	sprintf(data,SIM_STORE_CLIENT_CERT_RAM,strlen(client_certificate),100);
	transmit_data(data,strlen(data));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_STORE_CLIENT_CERT_RAM_RESP) == 0)
		goto retry11;

	retry12:
	for(int i=0;i<strlen(client_certificate);i++)
		transmit_data(client_certificate + i,1);
	//Validate the data write response
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_CLIENT_CERT_PAYLOAD_RESP) == 0)
		goto retry12;

	//Write user Key Certificate command'
	retry13:
	memset(data,0,sizeof(data));
	sprintf(data,SIM_STORE_USR_KEY_CERT_RAM,strlen(user_key),100);
	transmit_data(data,strlen(data));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_STORE_USR_KEY_CERT_RAM_RESP) == 0)
		goto retry13;

	retry14:
	for(int i=0;i<strlen(user_key);i++)
		transmit_data(user_key+i,1);
	//Validate the data write response
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_USR_KEY_CERT_PAYLOAD_RESP) == 0)
		goto retry14;

	//check checksum of CA certificate
	HAL_Delay(10000);
	transmit_data(SIM_VALIDATE_CA_CHECKSUM,strlen(SIM_VALIDATE_CA_CHECKSUM));
	HAL_Delay(300);

	//check checksum of Client certificate
	transmit_data(SIM_VALIDATE_CLIENT_CHECKSUM,strlen(SIM_VALIDATE_CLIENT_CHECKSUM));
	HAL_Delay(300);

	//check checksum of User key certificate
	transmit_data(SIM_VALIDATE_USR_KEY_CHECKSUM,strlen(SIM_VALIDATE_USR_KEY_CHECKSUM));
	HAL_Delay(300);

	memset(Data,'_', sizeof(Data));
	return true;
}

bool configureSSL()
{
	//set location of ca cert
	retry15:
	transmit_data(SIM_SET_CA_CERT_LOC,strlen(SIM_SET_CA_CERT_LOC));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_CA_CERT_LOC_RESP) == 0)
		goto retry15;

	//set location of client cer
	retry16:
	transmit_data(SIM_SET_CLIENT_CERT_LOC,strlen(SIM_SET_CLIENT_CERT_LOC));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_CLIENT_CERT_LOC_RESP) == 0)
		goto retry16;

	//set location of user key cert
	retry17:
	transmit_data(SIM_SET_USER_KEY_CERT_LOC,strlen(SIM_SET_USER_KEY_CERT_LOC));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_USER_KEY_CERT_LOC_RESP) == 0)
		goto retry17;

	//set security level 2
	retry18:
	transmit_data(SIM_SET_SECURITY_LEVEL,strlen(SIM_SET_SECURITY_LEVEL));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_SECURITY_LEVEL_RESP) == 0)
		goto retry18;

	//set ssl version = 4
	retry19:
	transmit_data(SIM_SET_SSL_VERSION,strlen(SIM_SET_SSL_VERSION));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_SSL_VERSION_RESP) == 0)
		goto retry19;

	//set support for all ssl cipher suites
	retry20:
	transmit_data(SIM_SET_CIPHER_SUITES,strlen(SIM_SET_CIPHER_SUITES));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_CIPHER_SUITES_RESP) == 0)
		goto retry20;

	//ignore RTC time
	retry21:
	transmit_data(SIM_SET_IGNORE_RTC_TIME,strlen(SIM_SET_IGNORE_RTC_TIME));
	HAL_Delay(300);
	if(strstr((char *)Data,SIM_SET_IGNORE_RTC_TIME_RESP) == 0)
		goto retry21;

	memset(Data,'_', sizeof(Data));
	return true;
}

bool connectToAWS()
{
	char data1[100];
	transmit_data(SIM_QUERY_LOCAL_IP,strlen(SIM_QUERY_LOCAL_IP));
	HAL_Delay(10000);

	//Open Mqtt Connection
	retry22:
	sprintf(data1,SIM_OPEN_MQTT_CONNECTION,"<url>",8883);
	transmit_data(data1,strlen(data1));
	HAL_Delay(20000);
	if(strstr((char *)Data,SIM_OPEN_MQTT_CONNECTION_RESP2) == 0)
		goto retry22;

	//Connect with your user iD
	memset(data1,0,sizeof(data1));
	sprintf(data1,SIM_SEND_USER_ID,"clientExample");
	transmit_data(data1,strlen(data1));
	HAL_Delay(20000);
	if(strstr((char *)Data,SIM_SEND_USER_ID_RESP2) == 0)
		goto retry22;

	//Subscribe to the topic
	retry24:
	memset(data1,0,sizeof(data1));
	sprintf(data1,SIM_MQTT_SUBSCRIBE,"test_topic");
	transmit_data(data1,strlen(data1));
	HAL_Delay(1000);
	if(strstr((char *)Data,SIM_MQTT_SUBSCRIBE_RESP2) == 0)
		goto retry24;

	//Publish the samples data
	retry23:
	memset(data1,0,sizeof(data1));
	sprintf(data1,SIM_MQTT_PUBLISH_DATA,"test",strlen("test_data1234\r\n"));
	transmit_data(data1,strlen(data1));
	HAL_Delay(1000);
	if(strstr((char *)Data,SIM_MQTT_PUBLISH_DATA_RESP) == 0)
		goto retry23;

	transmit_data("test_data1234\r\n",strlen("test_data1234\r\n"));
	HAL_Delay(20000);
	if(strstr((char *)Data,SIM_MQTT_PUBLISH_DATA_RESP2) == 0)
		goto retry23;


	return true;
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
