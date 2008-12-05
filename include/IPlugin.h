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

class IHookPoint;
class IPluginCommand;

using std::string;
using std::vector;

/**
 * @author Coen Bijlsma
 * @copyright Copyright (C) 2008 by Coen Bijlsma 
 * @since 2008-11-20
 * @version 0.1
 * @brief Interface for plug-ins.
 *
 * Sentry uses this interface to work with plug-ins.
 * If you want to create your own plug-ins, you have
 * to implement this interface and create a shared library
 * that has to reside in the plug-ins directory of Sentry.
 * Remember to clean up your own IHookpoint*s, because
 * Sentry doesn't to that kind of cleanup for you.
 */
class IPlugin {
public:

    virtual ~IPlugin(){}
    
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
     * Two plug-ins may have a reference to each other, and because of that
     * all plug-ins are first loaded into the sentry core, and checked 
     * afterwards.
     * If a plug-in is missing that is required by other plug-ins, those
     * plug-ins are removed as well, and therefore cannot be used.
     */
    virtual vector<string> getDependencies() =0;
    
    /**
     * Returns the commands this plug-in provides.
     */
    virtual vector<IPluginCommand*> getCommands() =0;
    
};

typedef IPlugin* create_plugin_t();
typedef bool destroy_plugin_t(IPlugin* plugin);

#endif
