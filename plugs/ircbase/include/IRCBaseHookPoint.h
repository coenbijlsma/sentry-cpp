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

#ifndef IRCBASEHOOKPOINT_H
#define	IRCBASEHOOKPOINT_H

#include <map>
#include <string>
#include "IHookPoint.h"
#include "IPluginCommand.h"

using std::string;
using sentry::IHookPoint;
using sentry::IPluginCommand;

class IRCBaseHookPoint : public IHookPoint {
private:
    string _name;
    map<string, IPluginCommand*> _attachedPluginCommands;

    map<string, IPluginCommand*>::iterator _findPluginCommand(string name);
    
public:

    IRCBaseHookPoint(string name) throw();
    virtual ~IRCBaseHookPoint();

    string getName() throw();

    map<string, IPluginCommand*> getAttachedPluginCommands() throw();

    IPluginCommand* findPluginCommand(string name) throw();

    void attach(IPluginCommand* command) throw();

    bool detach(IPluginCommand* command) throw();
    
};

#endif	/* IRCBASEHOOKPOINT_H */

