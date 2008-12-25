
#include <map>

#include "IRCBaseHookPoint.h"
#include "IRCBase.h"

IRCBaseHookPoint::IRCBaseHookPoint(string name) throw() {
    _name = name;
}

IRCBaseHookPoint::~IRCBaseHookPoint(){
    _attachedPluginCommands.clear();
}

map<string, IPluginCommand*>::iterator IRCBaseHookPoint::_findPluginCommand(string name){
    return _attachedPluginCommands.find(name);
}

string IRCBaseHookPoint::getName() throw(){
    return _name;
}

map<string, IPluginCommand*> IRCBaseHookPoint::getAttachedPluginCommands() throw(){
    return _attachedPluginCommands;
}

IPluginCommand* IRCBaseHookPoint::findPluginCommand(string name) throw(){
    map<string, IPluginCommand*>::iterator it = _findPluginCommand(name);

    if(it == _attachedPluginCommands.end() ){
        return (IPluginCommand*)0;
    }
    return it->second;
}

void IRCBaseHookPoint::attach(IPluginCommand* command) throw(){
    if(_findPluginCommand(command->getName()) == _attachedPluginCommands.end()){
        _attachedPluginCommands[command->getName()] = command;
    }
}

bool IRCBaseHookPoint::detach(IPluginCommand* command) throw(){
    map<string, IPluginCommand*>::iterator it = _findPluginCommand(command->getName());

    if(it != _attachedPluginCommands.end() ){
        _attachedPluginCommands.erase(it);
    }
    return true;
}
