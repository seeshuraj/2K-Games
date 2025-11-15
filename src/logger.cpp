// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "logger.h"

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : exitFlag(false) {
    logFile.open("log.txt", std::ios_base::trunc);
    logThread = std::thread(&Logger::ProcessLogs, this);
}

Logger::~Logger() {
    exitFlag = true;
    
    if (logThread.joinable()) {
        logThread.join();
    }
    
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::Log(const std::string& message) {
    std::string formattedMessage = GetCurrentTimestamp() + " " + message;
    
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(formattedMessage);
    }
}

void Logger::Flush() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (logQueue.empty()) {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    if (logFile.is_open()) {
        logFile.flush();
    }
}

void Logger::ProcessLogs() {
    while (true) {
        std::string message;
        
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            
            if (!logQueue.empty()) {
                message = logQueue.front();
                logQueue.pop();
            }
        }
        
        if (!message.empty()) {
            logFile << message << std::endl;
            logFile.flush();
        }
        
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (exitFlag && logQueue.empty()) {
                break;
            }
        }
        
        if (message.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

std::string Logger::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
