/*
 * telemetry.c
 *
 *  Created on: Aug 12, 2026
 *      Author: ashish
 */

#include<stdio.h>
#include"telemetry.h"

static uint32_t sequenceNumber =0U;

void Telemetry_sendReading(const MeterReading *reading, uint32_t timestampMs)
{
	printf("MB1,%lu,%lu,%lu,%lu,%lu,%lu\r\n",
			(unsigned long)sequenceNumber,
			(unsigned long)timestampMs,
			(unsigned long)reading->voltage_mV,
			(unsigned long)reading->current_mA,
			(unsigned long)reading->power_mW,
			(unsigned long)reading->energy_mWh);

	sequenceNumber++;
}


