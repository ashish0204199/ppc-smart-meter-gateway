#include "serial_port.h"
#include <cerrno>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<cstring>

int openSerialPort(const std::string& devicePath,std::string& errorMessage)
{
    const int serialFd = ::open(devicePath.c_str(),O_RDWR|O_NOCTTY);

    if(serialFd<0)
    {
        errorMessage="failed to open Serial Port "+devicePath+":"+std::strerror(errno);
        return -1;
    }
    errorMessage.clear();
    return serialFd;
}

bool configureSerialPort(int serialFd,std::string& errorMessage)
{
    termios serialSettings{};
    if(::tcgetattr(serialFd,&serialSettings)!=0)
    {
        errorMessage=std::string("failed to read serial settings:")+std::strerror(errno);
        return false;
    }
    ::cfmakeraw(&serialSettings);
    if(::cfsetispeed(&serialSettings,B115200)!=0 || ::cfsetospeed(&serialSettings,B115200)!=0)
    {
        errorMessage=std::string("failed to set baud rate:")+std::strerror(errno);
        return false;
    }
    serialSettings.c_cflag &= ~PARENB;
    serialSettings.c_cflag &= ~CSTOPB;
    serialSettings.c_cflag &= ~CSIZE;
    serialSettings.c_cflag |= CS8;
    serialSettings.c_cflag |= CLOCAL | CREAD;
    serialSettings.c_cflag &= ~CRTSCTS;
    serialSettings.c_cc[VMIN]=1;
    serialSettings.c_cc[VTIME]=0;
    if(::tcsetattr(serialFd,TCSANOW,&serialSettings)!=0)
    {
        errorMessage=std::string("failed to apply serial settings:")+std::strerror(errno);
        return false;
    }
    ::tcflush(serialFd,TCIFLUSH);
    errorMessage.clear();
    return true;
}

bool readSerialLine(int serialFd, std::string& line,std::string& errorMessage)
{
    line.clear();
    char receivedCharacter='\0';
    while(true)
    {
        const ssize_t bytesRead=::read(serialFd,&receivedCharacter,1);
        if(bytesRead<0)
        {
            if(errno==EINTR)
            {
                continue;
            }
            errorMessage= std::string("failed to read serial port:")+std::strerror(errno);
            return false;
        }

        if(bytesRead==0)
        {
            continue;
        }
        if(receivedCharacter =='\n')
        {
            errorMessage.clear();
            return true;
        }
        if(receivedCharacter !='\r')
        {
            line += receivedCharacter;
        
        }
    }
}