/**
 * @file angular_encoder.c
 * @author Serafin Bachman
 * @date 19-July-2017
 * @brief This file contains the implementation of the angular encoder driver
 */

#include "stm32f10x.h"
#include "angular_encoder.h"

#define Enk_ch1		GPIO_Pin_6 		/**< The GPIO pin connected to the angular encoder ch1 input */
#define Enk_ch2		GPIO_Pin_7 		/**< The GPIO pin connected to the angular encoder ch2 input */

volatile uint16_t pulse_count = 0;

/**
 * @brief This function performs initialization of TIM3 in enkoder mode.
 */
void AngularEncoder_Init() {
	/* Enkoder GPIO */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Enk_ch1 | Enk_ch2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Timer configuration ENKODER MODE */
	TIM_DeInit(TIM3);  // Konfiguracja timera do zliczania impulsow z enkodera
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	TIM3->ARR = 1024;
	TIM3->CR1 = TIM_CR1_UDIS;  // przeładowanie licznika
	TIM3->CCMR1 = (0x01 << 8) | (0x01 << 0);  // konfiguracja wejsc 1 i 2 odpowiednio na pinach TI1 i TI2

	TIM_Cmd(TIM3, ENABLE);

	/* NVIC configuration */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //najwyzszy priorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief This function performs number of counted pulses.
 */
uint16_t AngularEnkoder_Read() {
	/* Encoder counter */
	pulse_count = TIM_GetCounter(TIM3);
	return pulse_count;
}
