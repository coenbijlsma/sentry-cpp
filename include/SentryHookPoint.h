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
#ifndef SENTRYHOOKPOINT_H
#define SENTRYHOOKPOINT_H

#include "IHookPoint.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class SentryHookPoint : public IHookPoint {
private:
    string _name;
    vector<string> _attachedPluginCommands;
    
    vector<string>::iterator _findPluginCommand(string name);
public:
    
    /**
     * Creates a HookPoint that has the given name.
     * @brief Constructor
     */
    SentryHookPoint(string name) throw();
    
    /**
     * @brief Destructor
     */
    virtual ~SentryHookPoint();
    
    /**
     * @brief Returns the name of thie hookpoint.
     */
    string getName() throw();
    
    /**
     * @brief Returns the attached plug-in commands.
     */
    vector<string> getAttachedPluginCommands() throw();
    
    /**
     * @brief Adds the given plug-in command to the list.
     */
    void addPluginCommand(string name) throw();
    
    /**
     * @brief Removes the given plug-in command from the list.
     */
    bool removePluginCommand(string name) throw();
};

#endif
