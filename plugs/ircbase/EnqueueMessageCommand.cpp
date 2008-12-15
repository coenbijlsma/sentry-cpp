#include "EnqueueMessageCommand.h"

EnqueueMessageCommand::EnqueueMessageCommand(IRCBase* plugin){
    _plugin = plugin;
    _name = "ircbase.enqueue_message";
}

EnqueueMessageCommand::~EnqueueMessageCommand(){}

IPlugin* EnqueueMessageCommand::getPlugin(){
    return _plugin;
}

string EnqueueMessageCommand::getName(){
    return _name;
}

map<string, IHookPoint*> EnqueueMessageCommand::getHookPoints(){
    return _hookpoints;
}

void EnqueueMessageCommand::addAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp != _hookpoints.end() ){ return; }
    _hookpoints[hookpoint->getName()] = hookpoint;
}

void EnqueueMessageCommand::removeAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp == _hookpoints.end() ){ return; }
    _hookpoints.erase(hp);
}

void EnqueueMessageCommand::execute(vector<string> params){
    for(vector<string>::iterator it = params.begin(); it != params.end(); it++){
        _plugin->enqueue(*it);
    }
}