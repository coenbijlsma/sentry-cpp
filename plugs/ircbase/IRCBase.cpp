
#include <vector>
#include "IRCBase.h"
#include "Logger.h"
#include "EnqueueMessageCommand.h"

IRCBase::IRCBase(string name) throw(string){
    _name = name;
    _config = new SentryConfig("./ircbase.conf");
    _setupCommands();
}

IRCBase::~IRCBase(){
    for(vector<IHookPoint*>::iterator it = _providingHookPoints.begin(); it != _providingHookPoints.end(); it++){
        delete *it;
    }

    for(vector<IPluginCommand*>::iterator it = _commands.begin(); it != _commands.end(); it++){
        delete *it;
    }
}

void IRCBase::_setupCommands(){
    IPluginCommand* enqueue_message = new EnqueueMessageCommand(this);

    // do stuff

    _commands.push_back(enqueue_message);
}

void IRCBase::setProvider(IPluginProvider* provider){
    _provider = provider;
}

string IRCBase::getName(){
    return _name;
}

vector<IHookPoint*> IRCBase::getProvidingHookPoints(){
    return _providingHookPoints;
}

vector<string> IRCBase::getDependencies(){
    return _dependencies;
}

vector<IPluginCommand*> IRCBase::getCommands(){
    return _commands;
}

IPluginCommand* IRCBase::findCommand(string name){
    for(vector<IPluginCommand*>::iterator it = _commands.begin(); it != _commands.end(); it++){
        IPluginCommand* command = *it;
        if(command->getName() == name){
            return command;
        }
    }
    return (IPluginCommand*)0;
}

void IRCBase::enqueue(string message){
    _messageQueue.push_back(message);
}

extern "C" IPlugin* create_plugin(){
    string name("ircbase");
    return (IPlugin*)new IRCBase(name);
}

extern "C" bool destroy_plugin(IPlugin* plugin){
    delete plugin;
    return true;
}
