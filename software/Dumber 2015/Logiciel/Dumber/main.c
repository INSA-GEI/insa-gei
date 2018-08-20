#include "Battery.h"
#include "system_dumby.h"
#include <stm32f10x.h>
#include "cmde_usart.h"
#include "motor.h"
#include <string.h>
#include "cmde_spi.h"
#include "MAE.h"
#include "led.h"
#include <stdio.h>

#include "debug.h"

char cptMesureHigh=0;
char cptMesureLow=0;
char cptMesureDisable=0;
uint32_t cptMesureEmergencyHalt=0;

uint16_t vbatLowerVal;
uint16_t vbatHighVal;
uint16_t vbatDiff;

uint16_t testPostion=0;
uint32_t mesureVoltage;
uint32_t meanVoltage;

#define COMPTEUR_SEUIL_HIGH				8
#define COMPTEUR_SEUIL_LOW				8
#define COMPTEUR_SEUIL_DISABLE		8
#define COMPTEUR_SEUIL_EMERGENCY_HALT		30

//#define BUFFER_ID_SIZE  100
//uint32_t bufferIdentification[BUFFER_ID_SIZE];
//uint8_t bufferIdentificationIndex;

/** 
	* @brief Initialise les fréquences des périphérique et du micro. 
	*/
void Configure_Clock_Periph(void)
{
	///Configuration de la fréqyence d'horloge de l'adc */ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
  ///Activation de l'horloge du GPIO, de A B et C, de ADC1, de AFIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM2|RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1| RCC_APB2Periph_TIM1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1, ENABLE);		
}

int main(void)
{	
uint16_t k;
/**	
	* Initialisation 
	*/
  
	Configure_Clock_Periph();
	default_settings();
	
	MAP_PinShutDown();

	MAP_MotorPin();
	MAP_LEDpin();
	MAP_UsartPin();
	MAP_batteryPin();
	INIT_TIM2();
	INIT_OCMotorPwm();
	Configure_SysTick();
	
	INIT_USART();
	INIT_IT_UsartReceive();

	DMA_BAT();
	ADC1_CONFIG();
	INIT_IT_DMA();
	IC_TIM1_CHANEL3();
	IC_TIM1_CHANEL1();
	IT_TIM1();
	GPIO_ResetBits(GPIOA,GPIO_Pin_12); //enable encodeurs
	
	/* LEDgreen();
		
	bufferIdentificationIndex=0;
	previousTIMD=0;
	TIM_Cmd(TIM3, ENABLE);
	
	cmdRightMotor(FORWARD, 255);
		
	while (1)
	{
		if (Dumber.flagSystick == 1)
		{
			Dumber.flagSystick = 0;
			
			bufferIdentification[bufferIdentificationIndex]=(uint32_t)vitesseCapteurD;
			bufferIdentificationIndex++;
		
		
		  if (bufferIdentificationIndex>=BUFFER_ID_SIZE)
		  {
				cmdRightMotor(BRAKE, 0);
				
				for (bufferIdentificationIndex=0;bufferIdentificationIndex<BUFFER_ID_SIZE; bufferIdentificationIndex++)
				{
					SendUInt32(bufferIdentification[bufferIdentificationIndex]);
				}
				
				SendUInt32(0);
				SendUInt32(SysTick->LOAD);
				
				while (1);
			}
	  }
	} */
	
	regulationMoteur(FORWARD,FORWARD,(unsigned)2000,(unsigned)2000, 10000, 10000);
	
	while (1)
	{	
		__WFE(); // Bascule la puce en sleep mode
		
		if (Dumber.flagSystick == 1)
		{
			Dumber.flagSystick = 0;
			
			if(Dumber.acquisition==VOLTAGE && Dumber.BatterieChecking==TRUE)
			{
				vbatLowerVal = 0xFFF;
				vbatHighVal = 0;
				
				for(k=0; k<VOLTAGE_BUFFER_SIZE; k++)
				{
					meanVoltage+=ADCConvertedValue[k];
					
					if (vbatLowerVal> ADCConvertedValue[k]) vbatLowerVal = ADCConvertedValue[k];
					if (vbatHighVal< ADCConvertedValue[k]) vbatHighVal = ADCConvertedValue[k];
				}
				
				vbatDiff = vbatHighVal - vbatLowerVal;
				
				meanVoltage= meanVoltage/VOLTAGE_BUFFER_SIZE;
				voltageADC = meanVoltage;
				
				mesureVoltage = meanVoltage;
				
				Dumber.BatteryPercentage = mesureVoltage;
				Dumber.acquisition=FALSE;
				
				if(Dumber.BatteryPercentage >= VBAT_SEUIL_LOW)
				{
					cptMesureHigh++;
					if(cptMesureHigh >= COMPTEUR_SEUIL_HIGH)
					{
						if(Dumber.StateSystem == LOW)	Dumber.StateSystem = RUN;	
						
						Dumber.stateBattery = 2;
						cptMesureHigh=0;
						cptMesureLow=0;
						cptMesureDisable=0;
						cptMesureEmergencyHalt=0;
					}
				}
				else if (Dumber.BatteryPercentage < VBAT_SEUIL_LOW && Dumber.BatteryPercentage >= VBAT_SEUIL_DISABLE)
				{
					cptMesureLow++;
					if(cptMesureLow >= COMPTEUR_SEUIL_LOW)
					{
						if(Dumber.StateSystem == RUN)	Dumber.StateSystem=LOW;
						
						Dumber.stateBattery =1;
						cptMesureHigh=0;
						cptMesureLow=0;
						cptMesureDisable=0;
					}
				}
				else // Dumber.BatteryPercentage < VBAT_SEUIL_DISABLE
				{
					cptMesureDisable++;
					
					if(cptMesureDisable >= COMPTEUR_SEUIL_DISABLE)
					{
						Dumber.StateSystem = DISABLE;
						cptMesureHigh=0;
						cptMesureLow=0;
						cptMesureDisable=0;
						Dumber.stateBattery= 0;
						
						cmdRightMotor(BRAKE,0);
						cmdLeftMotor(BRAKE,0);
					}
				}
			}

			if (Dumber.StateSystem == DISABLE)
			{
				if(etatLED %2 == 0)	LEDred();
				else LEDoff();
				
				cptMesureEmergencyHalt++;
				
				if (cptMesureEmergencyHalt >= COMPTEUR_SEUIL_EMERGENCY_HALT)
				{
					shutDown();
					
					while (1);
				}
			}
				
			if (Dumber.StateSystem == RUN || Dumber.StateSystem == LOW || Dumber.StateSystem == IDLE)
			{
				if(Dumber.StateSystem == IDLE)
				{
					if(etatLED == 1) LEDON = 1;
					else if (etatLED==2) LEDON = 0;
				}
				else LEDON = 1;
				
				if (LEDON==1)
				{
					if(Dumber.stateBattery==1 && Dumber.StateSystem!=DISABLE) LEDorange();
					if(Dumber.stateBattery==2 && Dumber.StateSystem!=DISABLE) LEDgreen();
				} 
				else LEDoff(); 
			}
		}
	}
	
	#pragma diag_suppress 111
	return 0;
}
