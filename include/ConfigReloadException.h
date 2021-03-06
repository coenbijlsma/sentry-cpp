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
#ifndef CONFIGRELOADEXCEPTION_H
#define	CONFIGRELOADEXCEPTION_H

#include <string>

using std::string;

namespace sentry {
    /**
     * @brief Exception class in case reloading the SentryConfig fails
     * @author $Author$
     * @copyright Copyright (C) 2008 by Coen Bijlsma
     * @since 2008-12-31
     * @changed $Date$
     * @version $Id$
     * @url $HeadURL$
     */
    class ConfigReloadException {
    private:
        string _what;

    public:
        ConfigReloadException();
        ConfigReloadException(string what);
        ~ConfigReloadException();
        
        string what() throw();

        void setWhat(string what) throw();
    };
}

#endif	/* CONFIGRELOADEXCEPTION_H */

