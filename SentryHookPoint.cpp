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
    for(vector<string>::iterator it = _attachedPluginCommands.begin(); it != _attachedPluginCommands.end(); it++){
	
	if( *it == name ){
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
vector<string> SentryHookPoint::getAttachedPluginCommands() throw(){
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
    vector<string>::iterator it = _findPluginCommand(command->getName());
    
    if(it != _attachedPluginCommands.end() ){
	_attachedPluginCommands.erase(it);
    }
    
    return true;
}


