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
#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <vector>
#include "IHookPoint.h"
#include "IPluginCommand.h"
#include "IPluginProvider.h"

using std::string;
using std::vector;

namespace sentry {
    class IHookPoint;
    class IPluginCommand;
    
    /**
     * @brief Interface for plug-ins.
     * @author $Author$
     * @copyright Copyright (C) 2008 by Coen Bijlsma
     * @since 2008-11-20
     * @changed $Date$
     * @version $Id$
     * @url $HeadURL$
     *
     * Sentry uses this interface to work with plug-ins.
     * If you want to create your own plug-ins, you have
     * to implement this interface and create a shared library
     * that has to reside in the plug-ins directory of Sentry.
     * Remember to clean up your own IHookpoint*s, because
     * Sentry doesn't do that kind of cleanup for you.
     */
    class IPlugin {
    public:

        /**
         * Destructor
         */
        virtual ~IPlugin(){}

        /**
         * @brief Sets the provider for this plug-in
         */
        virtual void setProvider(IPluginProvider* provider) =0;

        /**
         * @brief Returns the name of this plug-in.
         */
        virtual string getName() =0;

        /**
         * @brief Returns the hookpoints this plug-in provides.
         */
        virtual vector<IHookPoint*> getProvidingHookPoints() =0;

        /**
         * Returns the names of other plug-ins this plug-in depends on.
         * Two plug-ins may not have a reference to each other, thus preventing
         * circular references.
         * If a plug-in is missing that is required by other plug-ins, those
         * plug-ins are removed as well, and therefore cannot be used.
         */
        virtual vector<string> getDependencies() =0;

        /**
         * Returns the commands this plug-in provides.
         */
        virtual vector<IPluginCommand*> getCommands() =0;

        /**
         * Returns the command that has the given name
         */
        virtual IPluginCommand* findCommand(string name) =0;

        /**
         * Returns wheter this plug-in is currently active.
         * If no plug-in is active, Sentry shuts down.
         */
        virtual bool isActive() =0;

        /**
         * Changes the state of the plug-in to active. From this point on, the
         * plug-in can start it's work, whatever that is.
         */
        virtual bool activate() =0;

        /**
         * Reloads the config of the plug-in, if any.
         */
        virtual bool reloadConfig() =0;

    };
}

typedef sentry::IPlugin* create_plugin_t();
typedef bool destroy_plugin_t(sentry::IPlugin* plugin);

#endif
