/*
 * telemetry.c
 *
 *  Created on: Aug 12, 2026
 *      Author: ashish
 */

#include<stdio.h>
#include<stdint.h>
#include"telemetry.h"

static uint32_t sequenceNumber =0U;

static uint8_t Telemetry_checkSum(const char* text)
{
	uint8_t checksum=0U;
	while(*text !='\0')
	{
		checksum=checksum ^ (uint8_t)*text;
		text++;
	}
	return checksum;

}

void Telemetry_sendReading(const MeterReading *reading, uint32_t timestampMs)
{
	char payload[128];

	snprintf(payload,sizeof(payload),"MB1,%lu,%lu,%lu,%lu,%lu,%lu",
			(unsigned long)sequenceNumber,
			(unsigned long)timestampMs,
			(unsigned long)reading->voltage_mV,
			(unsigned long)reading->current_mA,
			(unsigned long)reading->power_mW,
			(unsigned long)reading->energy_mWh);

	uint8_t checksum=Telemetry_checkSum(payload);
	printf("%s*%02X\r\n",payload,(unsigned int)checksum);

	sequenceNumber++;
}


