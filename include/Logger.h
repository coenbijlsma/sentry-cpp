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
#ifndef LOGGER_H
#define LOGGER_H

#include <string>

using std::string;

/**
 * @author Coen Bijlsma
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @version 0.1
 * @deprecated To be replaced by log4cpp
 * @brief This class logs the given messages.
 *
 */
class Logger {
public:

    static enum log_level_t {LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL};
    static void log(string message, log_level_t log_level);

};

#endif
