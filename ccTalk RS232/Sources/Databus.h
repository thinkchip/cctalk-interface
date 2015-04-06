/*
 * Databus.h
 *
 *  Created on: Feb 7, 2015
 *      Author: THINKCHIP
 */

#ifndef DATABUS_H_
#define DATABUS_H_

#include "Cpu.h"
#include "AS_DATABUS.h"
#include "FC_DATABUS.h"
#include "UTIL.h"
#include "EEPROM.h"
#include "EEpromAddress.h"


#include "cctalk.h"


typedef struct{
	uint8_t FlagData;
	uint8_t FlagEndData;
	uint8_t FlagTimeUp;
	uint8_t BufferInput[15];
	uint8_t pointer;
	
}VarDatabus;


extern VarDatabus data;

void dataprintf(uint8_t *string,uint8_t number);
void TaskDatabus(void);

#endif /* DATABUS_H_ */
