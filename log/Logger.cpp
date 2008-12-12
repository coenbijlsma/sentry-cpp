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
