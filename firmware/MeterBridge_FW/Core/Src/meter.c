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
