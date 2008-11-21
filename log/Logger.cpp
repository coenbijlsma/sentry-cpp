#include "Logger.h"
#include <iostream>

using std::cerr;
using std::endl;

void Logger::log(string message){
    cerr << message << endl;
}
