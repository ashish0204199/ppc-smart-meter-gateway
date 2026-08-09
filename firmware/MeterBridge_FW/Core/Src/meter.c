/*
 * meter.c
 *
 *  Created on: Aug 8, 2026
 *      Author: ashish
 */

#include "meter.h"

MeterReading Meter_CreateDefault(void)
{
	MeterReading reading;
	reading.voltage_mV=230000U;
	reading.current_mA=1000U;
	reading.power_mW=(reading.voltage_mV * reading.current_mA)/1000;
	reading.energy_mWh=0U;

	return reading;
}

void Meter_UpdateEnergy (MeterReading *reading,uint32_t elapsedMs)
{
	uint64_t temp=((uint64_t)reading->power_mW * elapsedMs)/3600000;
	reading->energy_mWh= temp;
}
