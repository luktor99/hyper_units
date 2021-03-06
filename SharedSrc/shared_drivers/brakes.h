/**
 * @file brakes.h
 * @author Łukasz Kilaszewski (luktor99)
 * @date 13-July-2017
 * @brief This file contains the headers of the brakes driver
 */

#ifndef SHARED_DRIVERS_BRAKES_H_
#define SHARED_DRIVERS_BRAKES_H_

void Brakes_Init(void);
void Brakes_PowerOff(void);
void Brakes_Normal(void);
void Brakes_Hold(void);
void Brakes_Release(void);
#ifdef UNIT_6
void Buttons_Tick(void);
#endif

#endif /* SHARED_DRIVERS_BRAKES_H_ */
