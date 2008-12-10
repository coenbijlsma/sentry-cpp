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
#include "IPluginCommand.h"
#include <string>
#include <map>

using std::string;
using std::map;

/**
 * @brief HookPoint implementation for Sentry.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 * @see Sentry.h
 *
 * This class is an implementation of the IHookPoint interface.
 * Sentry provides a number of hookpoints; you can find their
 * descriptions in the Sentry.h file.
 */
class SentryHookPoint : public IHookPoint {
private:
    string _name;
    map<string, IPluginCommand*> _attachedPluginCommands;
    
    map<string, IPluginCommand*>::iterator _findPluginCommand(string name);
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
     * @brief Returns the name of this hookpoint.
     */
    string getName() throw();
    
    /**
     * @brief Returns the attached plug-in commands.
     */
    map<string, IPluginCommand*> getAttachedPluginCommands() throw();
    
    /**
     * @brief Returns the IPluginCommand with the given name.
     */
    IPluginCommand* findPluginCommand(string name) throw();
    
    
    /**
     * @brief Adds the given plug-in command to the list.
     */
    void attach(IPluginCommand* command) throw();
    
    /**
     * @brief Removes the given plug-in command from the list.
     */
    bool detach(IPluginCommand* command) throw();
};

#endif
