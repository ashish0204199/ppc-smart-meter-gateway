#include "telemetry_parser.h"
#include<iostream>
#include<string>
#include "serial_port.h"
#include <unistd.h>

int main(int argc , char* argv[])
{
	std::string devicePath{"/dev/ttyACM0"};
	if(argc>1)
	{
		devicePath=argv[1];
	}
	std::string errorMessage;
	const int serialFd=openSerialPort(devicePath,errorMessage);
	if(serialFd<0)
	{
		std::cerr<<"Serial error="<<errorMessage<<'\n';
		return 1;
	}
	if(!configureSerialPort(serialFd,errorMessage))
	{
		std::cerr<<"Failed to configure serial port: "<<errorMessage<<'\n';
		::close(serialFd);
		return 1;
	}

	std::cout<<"Listening for STM32 telemetry on serial port"<<devicePath<<'\n';
	while(true)
	{
		std::string packet;
		if(!readSerialLine(serialFd,packet,errorMessage))
		{
			std::cerr<<"Failed to read serial Line"<<errorMessage<<'\n';
			::close(serialFd);
			return 1;
		}

		if(packet.empty())
		{
			continue;
		}

		if(packet.rfind("MB1",0)!=0)
		{
			std::cout<<"Device Message"<<packet<<'\n';
			continue;
		}

		TelemetryReading reading{};
		if(!parseTelemetryPacket(packet,reading,errorMessage))
		{
			std::cerr<<"packet rejected:"<<errorMessage<<'\n';
			continue;
		}

		std::cout<<"Packet accepted \n"<<"Sequence="<<reading.sequence<<", Timestamp="<<reading.timestamp_ms<<"ms, Voltage="<<reading.voltage_mV<<"mV, Current="
		<<reading.current_mA<<"mA, Power="<<reading.power_mW<<"mW, Energy="<<reading.energy_mWh<<"mWh\n";
	}

}



// #include<string>
// #include<cstdint>
// #include<iostream>
// #include<array>
// #include<sstream>
// #include<limits>
// #include "telemetry_reading.h"


// int main()
// {

//  const std::string packet{ "MB1,18,18075,230000,1000,230000,1154*0C"};
//  std::cout<<"RAW STM32 packet ="<<packet<<'\n';

//  const std::size_t starPosition =packet.find('*');

//  if(starPosition == std::string::npos)
//  {
//    std::cerr<<"invalid packet: checksum seperator * is missing\n ";
//    return 1;
//  }

//  const std::string payload = packet.substr(0,starPosition);
//  const std::string checksumText=packet.substr(starPosition+1);
//  std::cout<<"Payload =" <<payload<<'\n';
//  std::cout<<"Checksum ="<<checksumText<<'\n';



//  const std::uint8_t calculateChecksum=calculateChecksumXor(payload);
//  std::cout<<"Calculated checksum in decimal="<<static_cast<unsigned int>(calculateChecksum)<<"\n";
//  std::uint8_t receivedChecksum=0U;

//  if(!parseHexByte(checksumText,receivedChecksum))
//  {
// 	std::cerr<<"Invalid packet:checksum is not Valid Hexa decimal;\n";
// 	return 1;
//  }
//  std::cout<<"Received checksum in decimal value ="<<static_cast<unsigned int>(receivedChecksum)<<"\n"; 

//  if(calculateChecksum!= receivedChecksum)
//  {
// 	std::cerr<<"Invalid packed received !!! checksum mismatch\n";
// 	return 1;
//  }

//  std::cout<<"Checksum verified : The packet is a valid packet\n";

//  std::array<std::string,7>fields{};
//  std::istringstream payloadStream(payload);
//  std::string field;
//  std::size_t fieldCount=0;

//  while(std::getline(payloadStream,field,','))
//  {
// 	if(fieldCount>=fields.size())
// 	{
// 		std::cerr<<"invalid packet: too many fields\n";
// 		return 1;
// 	}
// 	fields[fieldCount]=field;
// 	fieldCount++;
//  }

//  if(fieldCount!=fields.size())
//  {
// 	std::cerr<<"Invalid packet: expected 7 fields but received"<<fieldCount<<"\n";
// 	return 1;
//  }
//  if(fields[0]!="MB1")
//  {
// 	std::cerr<<"Invalid packet: unsupported protocol. Expected MB1 but received "<<fields[0]<<"\n";
// 	return 1;
//  }
//  //TelemetryReading reading{};
// //  if(!parseUint32(fields[1],reading.sequence))
// //  {
// // 	std::cerr<<"Invalid packet:sequence is not vaild number \n";
// // 	return 1;
// //  }

// std::array<std::uint32_t,6>numericValues{};

// for(std::size_t i=0;i<numericValues.size();i++)
// {
// 	if(!parseUint32(fields[i+1],numericValues[i]))
// 	{
// 		std::cerr<<"invalid paket:field"<<i+1<<"is not valid number\n";
// 		return 1;
// 	}
// }

// TelemetryReading reading{

// 	numericValues[0],
// 	numericValues[1],
// 	numericValues[2],
// 	numericValues[3],
// 	numericValues[4],
// 	numericValues[5]

// };
// //std::cout<<"parsed sequence ="<<reading.sequence<<'\n';

// std::cout<<"Sequence= "<<reading.sequence <<"\n";
// std::cout<<"timestamp ="<<reading.timestamp_ms<<"ms\n";
// std::cout<<"voltage_mV= "<<reading.voltage_mV<<"mV\n";
// std::cout<<"current_mA="<<reading.current_mA<<"mA\n";
// std::cout<<"power_mW="<<reading.power_mW<<"mW\n";
// std::cout<<"energy_mWh="<<reading.energy_mWh<<"mWh\n";

// return 0;

// }
