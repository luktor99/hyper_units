/**
 * @file Unit2/unit_main.c
 * @author Łukasz Kilaszewski (luktor99)
 * @date 4-July-2017
 * @brief This file contains implementation of the main functions (UNIT_Init() and UNIT_Loop()), which are specific to each of the units.
 */

#include "unit.h"
#include "hyper.h"
#include "unit_can.h"
#include "shared_drivers/lm35.h"
#include "shared_drivers/vl6180x.h"
#include "shared_drivers/mlx90614.h"
#include "unit_drivers/max6675.h"
#include "shared_drivers/voltmeter.h"
#include "shared_drivers/brakes.h"

/**
 * @brief This function performs initialization of the peripherals specific to the unit.
 */
void UNIT_Init(void) {
	Brakes_Init();
	LM35_Init();

	VL6180X_Init();
	VL6180X_InitSensor(VL6180X_ID1);
	VL6180X_InitSensor(VL6180X_ID2);
	VL6180X_InitSensor(VL6180X_ID3);
	VL6180X_InitSensor(VL6180X_ID4);

	MLX90614_Init();
	MAX6675_Init();
	Voltmeter_Init();
}

/**
 * @brief This function is run in an infinite loop. This is where outgoing data gets updated.
 */
inline void UNIT_Loop(void) {
	// Read and update VL6180X sensors if there are new samples available
	if(VL6180X_IsSampleReady(VL6180X_ID1)) {
		uint8_t range1 = VL6180X_GetRange(VL6180X_ID1);
		HYPER_CAN_Update(updateVL6180X_1, &range1);
	}
	if(VL6180X_IsSampleReady(VL6180X_ID2)) {
		uint8_t range2 = VL6180X_GetRange(VL6180X_ID2);
		HYPER_CAN_Update(updateVL6180X_2, &range2);
	}
	if(VL6180X_IsSampleReady(VL6180X_ID3)) {
		uint8_t range3 = VL6180X_GetRange(VL6180X_ID3);
		HYPER_CAN_Update(updateVL6180X_3, &range3);
	}
	if(VL6180X_IsSampleReady(VL6180X_ID4)) {
		uint8_t range4 = VL6180X_GetRange(VL6180X_ID4);
		HYPER_CAN_Update(updateVL6180X_4, &range4);
	}

	// Read and update the pyrometer sensor
	uint8_t pyro_temp = MLX90614_readTemp();
	HYPER_CAN_Update(updatePyro, &pyro_temp);

	// Read and update the thermocouple sensor (every 250ms)
	static uint32_t tcouple_timestamp = 0;
	if(HYPER_Delay_Check(tcouple_timestamp, 250)) {
		uint8_t tcouple_temp = MAX6675_ReadTemp();
		HYPER_CAN_Update(updateTCouple, &tcouple_temp);

		// Update the time stamp
		tcouple_timestamp = HYPER_Delay_GetTime();
	}

	// Read and update the 12V rail voltage
	uint8_t voltage12v = Voltmeter_Read();
	HYPER_CAN_Update(updateVoltage12V, &voltage12v);

	// Read and update the LM35 sensor
	uint8_t lm35_temp = LM35_ReadTemp8();
	HYPER_CAN_Update(updateLM35, &lm35_temp);
}

/**
 * @brief This function processes a received CAN message
 * @param msg_type The message type @see MsgType_t
 * @param msg_data The message contents
 */
void UNIT_CAN_ProcessFrame(MsgType_t msg_type, uint8_t *msg_data) {
	if(msg_type == MSG_BRAKESHOLD)
		Brakes_Hold();
	else if(msg_type == MSG_BRAKESRELEASE)
		Brakes_Release();
	else if(msg_type == MSG_BRAKESPOWEROFF)
		Brakes_PowerOff();
}
