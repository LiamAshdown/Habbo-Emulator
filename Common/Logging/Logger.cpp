#include "Logger.h"
#include <iostream>

Logger * Logger::instance()
{
    static Logger instance;
    return &instance;
}

void Logger::Log(const char * log, LogLevels level)
{
    const char* levelType = "";
    switch (level)
    {
    case LOG_INFO:
        levelType = "INFO";
        break;
    case LOG_DEBUG:
        levelType = "DEBUG";
        break;
    case LOG_ERROR:
        levelType = "DEBUG";
        break;
    }
    std::cout << "[" << levelType << "]: " << log << std::endl;
}
