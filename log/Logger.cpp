#include "Logger.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

void Logger::log(string message, log_level_t log_level){
    switch(log_level){
        case LOG_INFO:
            cout << "[INFO] " << message << endl; break;
        case LOG_WARNING:
            cout << "[WARNING] " << message << endl; break;
        case LOG_ERROR:
            cerr << "[ERROR] " << message << endl; break;
        case LOG_FATAL:
            cerr << "[FATAL] " << message << endl; break;
        default:
            cerr << "[UNKNOWN] " << message << endl; break;
    }
}
