#include <cstdint>
#include <iostream>

struct TelemetryReading
{

std::uint32_t  sequence;
std::uint32_t  timestamp_ms;
std::uint32_t  voltage_mV;
std::uint32_t  current_mA;
std::uint32_t  power_mW;
std::uint32_t  energy_mWh;

};

int main()
{

 TelemetryReading reading{

	1U,
	1000U,
	230000U,
	1000U,
	230000U,
	64U
};

std::cout<<"Sequence= "<<reading.sequence <<"\n";
std::cout<<"timestamp ="<<reading.timestamp_ms<<"ms\n";
std::cout<<"voltage_mV= "<<reading.voltage_mV<<"mV\n";
std::cout<<"current_mA="<<reading.current_mA<<"mA\n";
std::cout<<"power_mW="<<reading.power_mW<<"mW\n";
std::cout<<"energy_mWh="<<reading.energy_mWh<<"mWh\n";

return 0;

}
