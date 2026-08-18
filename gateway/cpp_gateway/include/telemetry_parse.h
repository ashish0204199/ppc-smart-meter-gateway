#pragma once
#include<string>
#include"telemetry_reading.h"

bool parseTelemetryPacket(const std::string &packet,TelemetryReading &reading,std::string& errorMessage);

