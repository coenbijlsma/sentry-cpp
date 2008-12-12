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
 * @brief This class logs the given messages.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 *
 * This class logs messages throughout Sentry. If you want, you can use this
 * class in your own plug-ins to achieve a more generic way of logging for
 * the whole thing. test
 */
class Logger {
public:

    /**
     * The available logging levels
     */
    enum log_level_t {LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL};

    /**
     * The available log destinations
     */
    enum log_destination_t {DEST_STDOUT, DEST_FILE, DEST_DATABASE};
    
    /**
     * Logs the given message. Currently this is printed to the standard
     * outputstreams, but I plan to change that and use log4cpp for that.
     */
    static void log(string message, log_level_t log_level) throw();

    /**
     * Sets the destination of the logfile. I already put DEST_DATABASE in
     * the options, but that's not supported yet. If no log_destination is
     * set, DEST_STDOUT is used.
     */
    static void setDestination(log_destination_t dest) throw();

    /**
     * Sets the full path to the logfile
     */
    static void setlogFile(string filepath) throw();

private:
    /**
     * The destination of the logges messages
     */
    static log_destination_t log_destination;

    /**
     * The logfile in case the logging is done to a file
     */
    static string logfile;
};

#endif
