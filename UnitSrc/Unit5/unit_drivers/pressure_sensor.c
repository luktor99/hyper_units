/**
 * @file pressure_sensor.c
 * @author Łukasz Kilaszewski (luktor99)
 * @author Serafin Bachman
 * @date 19-July-2017
 * @brief This file contains the implementation of the pod pressure sensor driver
 */

#include "stm32f10x.h"
#include "pressure_sensor.h"

/**
 * @brief This function initializes peripherals required to read the pod pressure sensor
 */
void PressureSensor_Init(void) {
	// ADC1 should be enabled by now

	// Set up the GPIO
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	gpio_init.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &gpio_init);
}

/**
 * @brief This function performs a single conversion of the ADC channel connected to the pod pressure sensor
 */
uint8_t PressureSensor_Read(void) {
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);

    uint16_t adc_result = ADC_GetConversionValue(ADC1);
	return adc_result;
}
