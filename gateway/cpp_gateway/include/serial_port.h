#pragma once
#include<string>

int openSerialPort(const std::string& devicePath,std::string& errorMessage);

bool configureSerialPort(int serialFd,std::string& errorMessage);

bool readSerialLine(int serialFd,std::string& line,std::string& errorMessage);

//bool readSerialLine(int serialFd,std::string& line, std::string& errorMessage);