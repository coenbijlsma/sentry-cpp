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

using std::string;
using std::vector;

/**
 * @brief Interface for plug-ins.
 * Remember to clean up your own IHookpoint*s!
 */
class IPlugin {
public:

    /**
     * Returns the name of this plug-in.
     */
    virtual string getName() =0;
    
    /**
     * Returns the hookpoints this plug-in provides.
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
