/*
 * telemetry.h
 *
 *  Created on: Aug 12, 2026
 *      Author: ashish
 */

#ifndef INC_TELEMETRY_H_
#define INC_TELEMETRY_H_

#include <stdint.h>
#include "meter.h"



void Telemetry_sendReading(const MeterReading *reading, uint32_t timestampMs);





#endif /* INC_TELEMETRY_H_ */
