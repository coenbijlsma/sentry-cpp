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
#ifndef IHOOKPOINT_H
#define IHOOKPOINT_H

#include "IPluginCommand.h"
#include <map>
#include <string>

using std::string;
using std::map;

class IPluginCommand;

/**
 * @brief Interface for hookpoints.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 *
 * This class is an interface for hookpoints in Sentry and plug-ins.
 * A hookpoint is actually a 'hook' that your code can grab onto, and
 * let it execute when certain events happen. For example, Sentry has
 * a hookpoint with the name sentry.core.post_startup. After Sentry
 * has finished it's own startup tasks, Sentry looks for commands that
 * are hooked to the hookpoint sentry.core.post_startup, and executes 
 * those commands.
 * If you write your own plug-in, you can define your own hookpoints,
 * as long as they implement this interface.
 */
class IHookPoint {
public:

    /**
     * Destructor
     */
    virtual ~IHookPoint(){};
    
    /**
     * Returns the hookpoint's name. To avoid ambiguous names, hookpoint names
     * must be in the form <plugin_name>.<hookpoint_name>
     * For example, the hookpoint for sentry's core after startup 
     * is "core.post_startup"
     * @brief Returns the name of the hookpoint.
     */
    virtual string getName() =0;

    /**
     * Returns the names of the plug-in commands that 
     * are attached to this hookpoint. If no commands are
     * attached, this method returns an empty vector.
     * @brief Returns the attached plug-in commands.
     */
    virtual map<string, IPluginCommand*> getAttachedPluginCommands() =0;
    
    /**
     * Returns the IPluginCommand* that has the given name.
     * If that command does not exist, 0 is returned.
     * @brief Finds a IPluginCommand* by name.
     */
    virtual IPluginCommand* findPluginCommand(string name) =0;
    
    /**
     * Adds the given plug-in command to the list of attached
     * plug-in commands.
     * If the command already exists, this method has no effect.
     */
    virtual void attach(IPluginCommand* command) =0;
    
    /**
     * Removes the given command from the list of attached plug-in commands.
     * If the name does not exist in the list, nothing happens and
     * true is returned. For now.
     * @brief Removes a plug-in command.
     * @return bool Whether removing the name has succeeded.
     */
    virtual bool detach(IPluginCommand* command) =0;
};

#endif
