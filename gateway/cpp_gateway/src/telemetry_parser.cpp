#include"telemetry_parser.h"
#include<cstdint>
#include<string>
#include<limits>
#include<array>



static bool parseUint32(const std::string& text, std::uint32_t& value)
{
    if (text.empty())
    {
        return false;
    }

    unsigned long parsedValue = 0;

    for (char c : text)
    {
        // Only decimal digits are allowed
        if (c < '0' || c > '9')
        {
            return false;
        }

        const unsigned long digit =
            static_cast<unsigned long>(c - '0');

        // Check before multiplying so uint32_t cannot overflow
        if (parsedValue >
            (std::numeric_limits<std::uint32_t>::max() - digit) / 10)
        {
            return false;
        }

        parsedValue = parsedValue * 10 + digit;
    }

    value = static_cast<std::uint32_t>(parsedValue);

    return true;
}

static bool parseHexByte(const std::string& text, std::uint8_t& value)
{
    if (text.empty())
    {
        return false;
    }

    unsigned long parsedValue = 0;
	const unsigned long maxUint8 =std::numeric_limits<std::uint8_t>::max();

    for (char c : text)
    {
        unsigned long digit = 0;

        if (c >= '0' && c <= '9')
        {
            digit = static_cast<unsigned long>(c - '0');
        }
        else if (c >= 'A' && c <= 'F')
        {
            digit = static_cast<unsigned long>(c - 'A' + 10);
        }
        else if (c >= 'a' && c <= 'f')
        {
            digit = static_cast<unsigned long>(c - 'a' + 10);
        }
        else
        {
            return false;
        }

       

        if (parsedValue > (maxUint8 - digit) / 16)
        {
            return false;
        }

        parsedValue = parsedValue * 16 + digit;
    }

    value = static_cast<std::uint8_t>(parsedValue);

    return true;
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

bool parseTelemetryPacket(const std::string& packet,
                          TelemetryReading& reading,
                          std::string& errorMessage)
{


    std::size_t starPosition = packet.size();

    for (std::size_t i = 0; i < packet.size(); i++)
    {
        if (packet[i] == '*')
        {
            starPosition = i;
            break;
        }
    }

    if (starPosition == packet.size())
    {
        errorMessage = "Invalid packet: checksum separator * is missing";
        return false;
    }


    // -------------------------------------------------
    // 2. Manually create payload and checksum text
    // -------------------------------------------------

    std::string payload;

    for (std::size_t i = 0; i < starPosition; i++)
    {
        payload.push_back(packet[i]);
    }


    std::string checksumText;

    for (std::size_t i = starPosition + 1; i < packet.size(); i++)
    {
        checksumText.push_back(packet[i]);
    }


    // -------------------------------------------------
    // 3. Convert received hexadecimal checksum
    // -------------------------------------------------

    std::uint8_t receivedChecksum = 0U;

    if (!parseHexByte(checksumText, receivedChecksum))
    {
        errorMessage = "checksum is not a valid hexadecimal";
        return false;
    }


    // -------------------------------------------------
    // 4. Calculate our own checksum
    // -------------------------------------------------

    const std::uint8_t calculatedChecksum =
        calculateChecksumXor(payload);

    if (calculatedChecksum != receivedChecksum)
    {
        errorMessage = "checksum mismatch";
        return false;
    }


    // -------------------------------------------------
    // 5. Manually split payload at commas
    // -------------------------------------------------

    std::array<std::string, 7> fields{};

    std::size_t fieldIndex = 0;

    for (char c : payload)
    {
        if (c == ',')
        {
            if (fieldIndex >= fields.size() - 1)
            {
                errorMessage = "packet contains too many fields";
                return false;
            }

            fieldIndex++;
        }
        else
        {
            fields[fieldIndex].push_back(c);
        }
    }


    // fieldIndex is zero-based:
    // 0 means 1 field
    // 6 means 7 fields

    const std::size_t fieldCount = fieldIndex + 1;

    if (fieldCount != fields.size())
    {
        errorMessage = "packet must contain exactly seven fields";
        return false;
    }


    // -------------------------------------------------
    // 6. Check protocol identifier
    // -------------------------------------------------

    if (fields[0] != "MB1")
    {
        errorMessage = "unsupported protocol identifier";
        return false;
    }


    // -------------------------------------------------
    // 7. Convert the six numeric fields
    // -------------------------------------------------

    std::array<std::uint32_t, 6> numericValues{};

    for (std::size_t i = 0; i < numericValues.size(); i++)
    {
        if (!parseUint32(fields[i + 1], numericValues[i]))
        {
            errorMessage = "numeric field is not valid";
            return false;
        }
    }


    // -------------------------------------------------
    // 8. Store parsed values
    // -------------------------------------------------

    reading = TelemetryReading{
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