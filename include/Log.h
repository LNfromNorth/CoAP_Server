#ifndef __COAP_SERVER_LOG_H__
#define __COAP_SERVER_LOG_H__

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Singleton.h"

// enum to represent log levels
enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL};

class Logger: public Singleton<Logger> {
    friend class Singleton<Logger>;
public:
    Logger(const Logger&) = delete;
    Logger& operator =(const Logger&) = delete;
    ~Logger() { logFile.close();}
    void log(LogLevel level, const std::string& message);

private:
    Logger();
    std::ofstream logFile;
    std::string levelToString(LogLevel level);
};

#endif