#include "Logger.h"
#include <iostream>
#include <string>

using namespace MoonEngine;

std::ostream * Logger::_logStream;
LogLevel     Logger::_logLevel = ERROR;

void Logger::ProvideErrorStream(std::ostream * str)
{
    _logStream = str;
}

void Logger::SetLogLevel(LogLevel level)
{
    _logLevel = level;
}

void Logger::Log(LogLevel lv, std::string log, std::string file, int line)
{
    if (lv <= _logLevel)
    {
        std::ostream * stream = &std::cerr;
        if (_logStream != nullptr)
        {
            stream = _logStream;
        }
        switch (lv)
        {
            case FATAL_ERROR:
                (*stream) << "[!FATAL ERROR!]: ";
                break;
            case ERROR:
                (*stream) << "[ERROR]: ";
                break;
            case WARN:
                (*stream) << "[WARN]: ";
                break;
            case GAME:
                (*stream) << "[GAME]: ";
                break;
            case INFO:
                (*stream) << "[INFO]: ";
                break;
            default:
                (*stream) << "[Log]: ";
                break;
        }
		if (file != "") {
			file = file.substr(file.find_last_of("\\/") + 1, file.length());
			file = " @" + file;
			
		}
		if(line == -1)
			(*stream) << log << file << std::endl;
		else
			(*stream) << log << file << "[" << line << "]" << std::endl;

    }
}