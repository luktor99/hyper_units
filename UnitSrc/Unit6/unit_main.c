/**
 * @file Unit6/unit_main.c
 * @author Łukasz Kilaszewski (luktor99)
 * @date 4-July-2017
 * @brief This file contains implementation of the main functions (UNIT_Init() and UNIT_Loop()), which are specific to each of the units.
 */

#include "unit.h"
#include "hyper.h"
#include "unit_can.h"
#include "shared_drivers/brakes.h"
#include "unit_drivers/power.h"
#include "watchdog.h"

/**
 * @brief This function performs initialization of the peripherals specific to the unit.
 */
void UNIT_Init(void) {
	Brakes_Init();
	Power_Init();
	Watchdog_Init();
}

/**
 * @brief This function is run in an infinite loop. This is where outgoing data gets updated.
 */
inline void UNIT_Loop(void) {
	Watchdog_Tick();
}

/**
 * @brief This function processes a received CAN message
 * @param msg_type The message type @see MsgType_t
 * @param msg_data The message contents
 */
void UNIT_CAN_ProcessFrame(MsgType_t msg_type, uint8_t *msg_data) {
	if(msg_type == MSG_BRAKESHOLD) {
		if(!Watchdog_IsLocked())
			Brakes_Hold();
	}
	else if(msg_type == MSG_BRAKESRELEASE)
			Brakes_Release();
	else if(msg_type == MSG_BRAKESPOWEROFF) {
		if(!Watchdog_IsLocked())
			Brakes_PowerOff();
	}
	else if(msg_type == MSG_POWERDOWN) {
		if(!Watchdog_IsLocked())
			Brakes_PowerOff();
		Power_Down();
	}
	else if(msg_type == MSG_START || msg_type == MSG_WATCHDOGRESET)
		Watchdog_Reset();
	else if(msg_type == MSG_BRAKESLOCKUPDATE) {
		uint16_t delay = (msg_data[1] << 8) | msg_data[2];
		Watchdog_Lock(delay);
	}
}
