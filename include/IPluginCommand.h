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
#include <string>
#include <vector>

using std::string;
using std::vector;

class IPlugin;

/**
 * @author Coen Bijlsma
 * @copyright Copyright (C) 2008 by Coen Bijlsma 
 * @since 2008-11-20
 * @version 0.1
 *
 * This interface is used by Sentry to work with plug-in commands.
 * If you want to write your own plug-ins, you have to implement this
 * interface.
 */
class IPluginCommand {
public:

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
     * @brief The name of the hookpoint this command is attached to.
     */
    virtual string getHookPoint() =0;
    
    /**
     * @brief Executes the command with the given parameters.
     */
    virtual void execute(vector<string> params) =0;
};

#endif
