/*
 * meter.h
 *
 *  Created on: Aug 8, 2026
 *      Author: ashish
 */

#ifndef INC_METER_H_
#define INC_METER_H_

#include <stdint.h>

typedef struct
{
    uint32_t voltage_mV;
    uint32_t current_mA;
    uint32_t power_mW;
    uint32_t energy_mWh;
} MeterReading;

MeterReading Meter_CreateDefault(void);
void Meter_UpdateEnergy(MeterReading *reading,uint32_t elapsedMs);

#endif /* INC_METER_H_ */
