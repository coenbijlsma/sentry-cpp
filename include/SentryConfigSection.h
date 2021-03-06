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
#ifndef SENTRYCONFIGSECTION_H
#define	SENTRYCONFIGSECTION_H

#include <string>
#include <map>
#include <vector>
#include "ConvertException.h"

using std::string;
using std::map;
using std::vector;

namespace sentry {
    /**
     * @brief Class for handling config sections.
     * @author $Author$
     * @copyright Copyright (C) 2008 by Coen Bijlsma
     * @since 2008-11-20
     * @changed $Date$
     * @version $Id$
     * @url $HeadURL$
     */
    class SentryConfigSection {
    private:
            string _name;
            vector<string> _comment;
            map<string, string> _entries;

    public:

            /*
             * Constructor.
             */
            SentryConfigSection(string name);

            /*
             * Destructor. Does nothing at this time.
             */
            virtual ~SentryConfigSection();

            /*
             * Returns the name of this SentryConfigSection
             */
            string getName();

            /**
             * Returns the comment for this section
             */
            vector<string> getComment();

            /**
             * Adds the given string to the comment of this section.
             */
            void addComment(string comment);

            /**
             * Adds the contents of the given vector to the comment of this section.
             */
            void addAllComment(vector<string> comment);

            /*
             * Returns the value of the requested setting.
             * If the setting does not exist, an empty
             * string is returned.
             */
            string get(string setting);

            /**
             * Returns the valie of the requested setting as an int (if possible).
             * If the setting doesn't exist or the value can't be converted
             * to an int, a ConvertException is thrown.
             */
            int getInt(string setting) throw(ConvertException);

            /*
             * Sets or creates the setting with the
             * provided value.
             */
            void set(string setting, string value);

            /*
             * Returns all the entries for this SentryConfigSection.
             */
            map<string, string> all();
    };
}

#endif	/* SENTRYCONFIGSECTION_H */
