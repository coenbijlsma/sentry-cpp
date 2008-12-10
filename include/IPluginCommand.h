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
#ifndef IPLUGINCOMMAND_H
#define IPLUGINCOMMAND_H

#include "IPlugin.h"
#include "IHookPoint.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class IPlugin;

/**
 * @brief Interface for plug-in commands.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 *
 * This interface is used by Sentry to work with plug-in commands.
 * If you want to write your own plug-ins, you have to implement this
 * interface.
 */
class IPluginCommand {
public:

    /**
     * Destructor
     */
    virtual ~IPluginCommand(){};
    
    /**
     * @brief Returns the IPlugin* this IPluginCommand* belongs to.
     */
    virtual IPlugin* getPlugin() =0;
    
    /**
     * @brief Returns the name of this command.
     * To avoid ambiguous command names, the names must
     * be in the form <plugin_name>.<command_name>
     */
    virtual string getName() =0;
    
    /**
     * @brief The hookpoints this command is attached to.
     */
    virtual map<string, IHookPoint*> getHookPoints() =0;
    
    /**
     * @brief Executes the command with the given parameters.
     * This method executes the command with the given parameters.
     * To be clear about what parameters are expected for each command,
     * I recommend putting the requirements in the header file(s) of
     * your own commands.
     */
    virtual void execute(vector<string> params) =0;
};

#endif
