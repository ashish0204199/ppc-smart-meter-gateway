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
	reading.power_mW=(uint32_t)(((uint64_t)reading.voltage_mV * reading.current_mA)/1000);
	reading.energy_mWh=0U;
	reading.energyRemainder_mWms=0U;

	return reading;
}

void Meter_UpdateEnergy (MeterReading *reading,uint32_t elapsedMs)
{

	uint64_t accumulated_mWms = ((uint64_t)reading->power_mW *elapsedMs)+ reading->energyRemainder_mWms;
	reading->energy_mWh = reading->energy_mWh +(uint32_t)(accumulated_mWms / 3600000ULL);
	reading->energyRemainder_mWms= (uint32_t) (accumulated_mWms %3600000ULL);

}
