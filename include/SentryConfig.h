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
#ifndef SENTRYCONFIG_H
#define SENTRYCONFIG_H

#include "SentryConfigSection.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using std::string;
using std::map;
using std::ofstream;

/**
 * @brief Class for reading config files.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 */
class SentryConfig {
private:
        string _filename;
        map<string, SentryConfigSection*> _sections;

        /*
         * Tries to read the configfile and fill
         * the _sections map.
         * @throws string If the configfile could not
         * be opened.
         */
        void _init() throw(string);

        /*
         * Writes the content of _sections[name] to
         * _fs.
         * @throws string If the ofstream* is not open.
         */
        void _writeSection(ofstream* fs, SentryConfigSection* section) throw(string);
        
public:

        /*
         * Constructor.
         * Tries to open the provided configfile
         * and then calls _init() to read its contents.
         * If the provided configfile does not exist,
         * this constructor tries the default configfile.
         * If that doesn's exist either, it tries to write
         * the default configfile, and _init() with that.
         * If that also fails, it gives up all hope and
         * stops.
         * @throws string If all hope has gone.
         */
        SentryConfig(string filename = "sentry.conf") throw(string);

        /*
         * Writes the current config to the configfile, and
         * cleans up the map of sections, effectively calling
         * their destructors.
         */
        virtual ~SentryConfig() throw();

        /*
         * Looks for the requested SentryConfigSection.
         * Returns it if it exists, otherwise returns
         * a (SentryConfigSection*)0 pointer.
         */
        SentryConfigSection* getSection(string name) throw();

        /*
         * Tries to locate the requested section and setting.
         * If either doesn't exist, this function returns an
         * empty string.
         * Otherwise calls get() on the SentryConfigSection.
         * @see SentryConfigSection.h
         */
        string getValue(string section, string setting) throw();

        /*
         * Sets or creates a setting in the requested section.
         * Of the section does not exist, an error occurs.
         * @throws string If the requested section does not
         * exist.
         */
        void setSetting(string section, string setting, string value) throw(string);

        /*
         * Tries to write the current config to the configfile.
         * In fact this function always returns true, but I will
         * to work on that, promise.
         */
        bool writeConfig() throw();

};

#endif /* SENTRYCONFIG_H */
