/**
 * This file is part of Sentry.
 *
 * Sentry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sentry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sentry.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Logger.h"
#include <iostream>
#include <fstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::ios_base;

sentry::Logger::log_destination_t sentry::Logger::log_destination;
string sentry::Logger::logfile;

void sentry::Logger::log(string message, log_level_t log_level) throw(){
    switch(log_level){
        case LOG_INFO:
            message = string("[INFO] " + message); break;
        case LOG_WARNING:
            message = string("[WARNING] " + message); break;
        case LOG_ERROR:
            message = string("[ERROR] " + message); break;
        case LOG_FATAL:
            message = string("[FATAL] " + message); break;
        default:
            message = string("[UNKNOWN] " + message); break;
    }

    if(Logger::log_destination == Logger::DEST_STDOUT){
        switch(log_level){
            case LOG_INFO:
            case LOG_WARNING:
                cout << message << endl; break;
            case LOG_ERROR:
            case LOG_FATAL:
            default:
                cout << message << endl; break;

        }
    }else if(Logger::log_destination == Logger::DEST_FILE){
        ofstream os(Logger::logfile.c_str(), ios_base::out | ios_base::app);
        
        os << message << '\n';
        os.flush();
        os.close();
    }else if(Logger::log_destination == Logger::DEST_DATABASE){
        cout << "[INFO] Redirecting logger to stdout" << endl;
        Logger::setDestination(Logger::DEST_STDOUT);
        Logger::log(message, log_level);
    }
    
}

void sentry::Logger::setDestination(log_destination_t dest) throw(){
    Logger::log_destination = dest;
}

void sentry::Logger::setlogFile(string filepath) throw(){
    Logger::logfile = filepath;
}
