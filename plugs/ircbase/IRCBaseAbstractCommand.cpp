#include "IRCBaseAbstractCommand.h"

IRCBaseAbstractCommand::IRCBaseAbstractCommand(IRCBase* plugin) {
    _plugin = plugin;
}

IRCBaseAbstractCommand::~IRCBaseAbstractCommand(){
    
}

IPlugin* IRCBaseAbstractCommand::getPlugin(){
    return _plugin;
}

string IRCBaseAbstractCommand::getName(){
    return _name;
}

map<string, IHookPoint*> IRCBaseAbstractCommand::getHookPoints(){
    return _hookpoints;
}

void IRCBaseAbstractCommand::addAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp != _hookpoints.end() ){ return; }
    _hookpoints[hookpoint->getName()] = hookpoint;
}

void IRCBaseAbstractCommand::removeAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp == _hookpoints.end() ){ return; }
    _hookpoints.erase(hp);
}
