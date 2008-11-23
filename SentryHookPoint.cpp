#include "SentryHookPoint.h"

/* Constructor */
SentryHookPoint::SentryHookPoint(string name) throw(){
    _name = name;
}

/* Destructor */
SentryHookPoint::~SentryHookPoint(){
    _attachedPluginCommands.clear();
}

/* Returns the iterator to the searched-for command */
vector<string>::iterator SentryHookPoint::_findPluginCommand(string name){
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

/* Adds a plug-in command to the list */
void SentryHookPoint::addPluginCommand(string name) throw(){
    /* Only attach if not already attached */
    if( _findPluginCommand(name) == _attachedPluginCommands.end() ){
	_attachedPluginCommands.push_back(name);
    }
}

/* Removes a plug-in command from the list */
bool SentryHookPoint::removePluginCommand(string name) throw(){
    vector<string>::iterator it = _findPluginCommand(name);
    
    if(it != _attachedPluginCommands.end() ){
	_attachedPluginCommands.erase(it);
    }
    
    return true;
}


