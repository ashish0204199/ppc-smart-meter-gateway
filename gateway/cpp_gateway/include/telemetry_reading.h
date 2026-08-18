#pragma once

#include<cstdint>

struct TelemetryReading
{
    std::uint32_t sequence;
    std::uint32_t timestamp_ms;
    std::uint32_t voltage_mV;
    std::uint32_t current_mA;
    std::uint32_t power_mW;
    std::uint32_t energy_mWh;
};
