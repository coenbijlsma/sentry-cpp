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

#ifndef _IPLUGINPROVIDER_H
#define	_IPLUGINPROVIDER_H

#include <string>
#include "IHookPoint.h"
#include "IPlugin.h"

using std::string;

namespace sentry {

    class IPlugin;
    class IHookPoint;

    /**
     * @brief Interface for the plug-in providing part of Sentry.
     * @author $Author$
     * @copyright Copyright (C) 2008 by Coen Bijlsma
     * @since 2008-12-14
     * @changed $Date$
     * @version $Id$
     * @url $HeadURL$
     */
    class IPluginProvider {
    public:

        virtual ~IPluginProvider(){}

        virtual IHookPoint* findHookPoint(string name) =0;

        virtual IPlugin* findPlugin(string name) =0;

    };
}

#endif	/* _IPLUGINPROVIDER_H */

