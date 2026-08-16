#include<string>
#include<cstdint>
#include<iostream>
#include<array>
#include<sstream>
#include<limits>
 


struct TelemetryReading
{

std::uint32_t  sequence;
std::uint32_t  timestamp_ms;
std::uint32_t  voltage_mV;
std::uint32_t  current_mA;
std::uint32_t  power_mW;
std::uint32_t  energy_mWh;

};

bool parseUint32(const std::string &text, std::uint32_t& value)
{
	try{
		std::size_t charactersUsed=0;
		const unsigned long parsedValue=std::stoul(text,&charactersUsed,10);
		if(charactersUsed!=text.size())
		{
			return false;
		}

		if(parsedValue>std::numeric_limits<std::uint32_t>::max())
		{
			return false;
		}
		value=static_cast<std::uint32_t>(parsedValue);
		return true;
	}
	catch(...)
	{
		return false;
	}
	
}

bool parseHexaByte(const std::string &text, std::uint8_t& value)
{

	try{
		std::size_t charsUsed=0;
		const unsigned long parsedValue=std::stoul(text,&charsUsed,16);
		if(charsUsed!=text.size())
		{
			return false;
		}
		if(parsedValue>std::numeric_limits<std::uint8_t>::max())
		{
			return false;
		}
		value=static_cast<std::uint8_t>(parsedValue);
		return true;
	}
	catch(...)
	{
		return false;
	}
}


std::uint8_t calculateCheksumXor(const std::string &text)
{

	std::uint8_t checksum=0U;
	for(char c :text)
	{
		checksum=checksum^static_cast<std::uint8_t>(c);
	}
	return checksum;
}

int main()
{


 const std::string packet{ "MB1,18,18075,230000,1000,230000,1154*0C"};
 std::cout<<"RAW STM32 packet ="<<packet<<'\n';

 const std::size_t starPosition =packet.find('*');

 if(starPosition == std::string::npos)
 {
   std::cerr<<"invalid packet: checksum seperator * is missing\n ";
   return 1;
 }

 const std::string payload = packet.substr(0,starPosition);
 const std::string checksumText=packet.substr(starPosition+1);
 std::cout<<"Payload =" <<payload<<'\n';
 std::cout<<"Checksum ="<<checksumText<<'\n';



 const std::uint8_t calculateChecksum=calculateCheksumXor(payload);
 std::cout<<"Calculated checksum in decimal="<<static_cast<unsigned int>(calculateChecksum)<<"\n";
 std::uint8_t receivedChecksum=0U;

 if(!parseHexaByte(checksumText,receivedChecksum))
 {
	std::cerr<<"Invalid packet:checksum is not Valid Hexa decimal;\n";
	return 1;
 }
 std::cout<<"Received checksum in decimal value ="<<static_cast<unsigned int>(receivedChecksum)<<"\n"; 

 if(calculateChecksum!= receivedChecksum)
 {
	std::cerr<<"Invalied packed received !!! checksum mismatch\n";
	return 1;
 }

 std::cout<<"Checksum verified : The packet is a valied packet\n";

 std::array<std::string,7>fields{};
 std::istringstream payloadStream(payload);
 std::string field;
 std::size_t fieldCount=0;

 while(std::getline(payloadStream,field,','))
 {
	if(fieldCount>=fields.size())
	{
		std::cerr<<"invalid packet: too many fields\n";
		return 1;
	}
	fields[fieldCount]=field;
	fieldCount++;
 }

 if(fieldCount!=fields.size())
 {
	std::cerr<<"Invalid packet: expected 7 fields but received"<<fieldCount<<"\n";
	return 1;
 }
 if(fields[0]!="MB1")
 {
	std::cerr<<"Invalid packet: unsupported protocol. Expected MB1 but received "<<fields[0]<<"\n";
	return 1;
 }
 //TelemetryReading reading{};
//  if(!parseUint32(fields[1],reading.sequence))
//  {
// 	std::cerr<<"Invalid packet:sequence is not vaild number \n";
// 	return 1;
//  }

std::array<std::uint32_t,6>numericValues{};

for(std::size_t i=0;i<numericValues.size();i++)
{
	if(!parseUint32(fields[i+1],numericValues[i]))
	{
		std::cerr<<"invalid paket:field"<<i+1<<"is not valid number\n";
		return 1;
	}
}

TelemetryReading reading{

	numericValues[0],
	numericValues[1],
	numericValues[2],
	numericValues[3],
	numericValues[4],
	numericValues[5]

};
//std::cout<<"parsed sequence ="<<reading.sequence<<'\n';

std::cout<<"Sequence= "<<reading.sequence <<"\n";
std::cout<<"timestamp ="<<reading.timestamp_ms<<"ms\n";
std::cout<<"voltage_mV= "<<reading.voltage_mV<<"mV\n";
std::cout<<"current_mA="<<reading.current_mA<<"mA\n";
std::cout<<"power_mW="<<reading.power_mW<<"mW\n";
std::cout<<"energy_mWh="<<reading.energy_mWh<<"mWh\n";

return 0;

}
