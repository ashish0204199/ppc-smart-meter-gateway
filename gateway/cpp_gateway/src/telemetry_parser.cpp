#include"telemetry_parse.h"
#include<cstdint>
#include<string>
#include<limits>
#include<array>
#include<sstream>

static bool parseUint32(const std::string &text,std::uint32_t &value)
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

static bool parseHexByte(const std::string &text, std::uint8_t& value)
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

static std::uint8_t calculateChecksumXor(const std::string &text)
{

	std::uint8_t checksum=0U;
	for(char c :text)
	{
		checksum=checksum^static_cast<std::uint8_t>(c);
	}
	return checksum;
}

bool parseTelemetryPacket(const std::string &packet, TelemetryReading &reading,std::string &errorMessage)
{
	const std::size_t starPosition =packet.find("*");
	if(starPosition==std::string::npos)
	{
		errorMessage="Invalid packet:checksum separator * is missing";
		return false;
	}
	const std::string payload =packet.substr(0,starPosition);
	const std::string checksumText=packet.substr(starPosition+1);
	
	std::uint8_t receivedChecksum=0U;

	if(!parseHexByte(checksumText,receivedChecksum))
	{
		errorMessage="checksum is not a valid hexadecimal";
		return false;
	}
	

	const std::uint8_t calculatedChecksum=calculateChecksumXor(payload);

	if(calculatedChecksum!=receivedChecksum)
	{
		errorMessage="checksum mismatch";
		return false;
	}
	std::array<std::string,7>fields{};
	std::istringstream payloadStream(payload);
	std::string field;
	std::size_t fieldCount=0;
	while(std::getline(payloadStream,field,','))
	{
		if(fieldCount>=fields.size())
		{
			errorMessage ="packet contains too many fields";
			return false;
		}
		fields[fieldCount]=field;
		fieldCount++;
	}
	if(fieldCount !=fields.size())
	{
		errorMessage="packet must contain exactly seven fields";
		return false;
	}

	if(fields[0]!="MB1")
	{
		errorMessage="unsupported protocol identifier";
		return false;
	}
	std::array<std::uint32_t,6> numericValues{};
	for(std::size_t i=0;i<numericValues.size();i++)
	{
		if(!parseUint32(fields[i+1],numericValues[i]))
		{
			errorMessage="field"+std::to_string(i+1)+"is not a valid unsigned number";
			return false;
		}
	}


	reading=TelemetryReading{
		numericValues[0],
		numericValues[1],
		numericValues[2],
		numericValues[3],
		numericValues[4],
		numericValues[5]

	};
	errorMessage.clear();
	return true;

}