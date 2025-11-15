// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class Logger {
public:
    static Logger& GetInstance();
    ~Logger();
    
    void Log(const std::string& message);
    void Flush();

private:
    Logger();
    
    std::ofstream logFile;
    std::thread logThread;
    std::queue<std::string> logQueue;
    std::mutex queueMutex;
    bool exitFlag;
    
    void ProcessLogs();
    std::string GetCurrentTimestamp();
};

#endif // LOGGER_H
