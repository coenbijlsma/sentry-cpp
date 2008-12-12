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

#include "SentryHookPoint.h"

/* Constructor */
SentryHookPoint::SentryHookPoint(string name) throw(){
    _name = name;
}

/* Destructor */
SentryHookPoint::~SentryHookPoint(){
    // DO NOT delete THESE IPluginCommand*s, they might be needed elsewhere!
    _attachedPluginCommands.clear();
}

/* Returns the iterator to the searched-for command */
map<string, IPluginCommand*>::iterator SentryHookPoint::_findPluginCommand(string name){
    for(map<string, IPluginCommand*>::iterator it = _attachedPluginCommands.begin(); it != _attachedPluginCommands.end(); it++){
        IPluginCommand* cmd = it->second;

	if( cmd->getName() == name ){
	    return it;
	}
    }
    
    return _attachedPluginCommands.end();
}

/* Returns the name of this hookpoint */
string SentryHookPoint::getName() throw(){
    return _name;
}

/* Gets the attached plug-in commands */
map<string, IPluginCommand*> SentryHookPoint::getAttachedPluginCommands() throw(){
    return _attachedPluginCommands;
}

/* Returns the IPluginCommand that has the given name */
IPluginCommand* SentryHookPoint::findPluginCommand(string name) throw() {
    map<string, IPluginCommand*>::iterator it = _findPluginCommand(name);

    if(it == _attachedPluginCommands.end()){
        return (IPluginCommand*)0;
    }
    return it->second;
}

/* Adds a plug-in command to the list */
void SentryHookPoint::attach(IPluginCommand* command) throw(){
    /* Only attach if not already attached */
    if( _findPluginCommand(command->getName()) == _attachedPluginCommands.end() ){
	_attachedPluginCommands[command->getName()] = command;
    }
}

/* Removes a plug-in command from the list */
bool SentryHookPoint::detach(IPluginCommand* command) throw(){
    map<string, IPluginCommand*>::iterator it = _findPluginCommand(command->getName());
    
    if(it != _attachedPluginCommands.end() ){
	_attachedPluginCommands.erase(it);
    }
    
    return true;
}


