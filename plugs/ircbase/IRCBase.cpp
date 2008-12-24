
#include <vector>
#include <stdlib.h>

#include "IRCSocket.h"
#include "IRCBase.h"
#include "Logger.h"
#include "EnqueueMessageCommand.h"
#include "IRCMessage.h"

using sentry::Logger;

IRCBase::IRCBase(string name) throw(string){

    Logger::setDestination(Logger::DEST_STDOUT);
    Logger::setlogFile("sentry.log");
    _name = name;
    _config = new SentryConfig("./ircbase.conf");
    _setupCommands();
    if(_connect()){
        _socket->sendMessage("PASS foo\r\n");
        _socket->sendMessage("NICK sentry`\r\n");
        _socket->sendMessage("USER Sentry` foo bar Sentry`\r\n");
        _socket->sendMessage("JOIN " + _config->getValue("connection", "channel") + "\r\n");

        /* Setup the thread to listen to incoming data */
        _doListen = false;
        int li = pthread_create(&this->_listener, NULL, IRCBase::_listen, (void*)this );
        pthread_join(_listener, NULL);
    }

}

IRCBase::~IRCBase(){
    /* Stop the listening thread */
    _doListen = false;

    for(vector<IHookPoint*>::iterator it = _providingHookPoints.begin(); it != _providingHookPoints.end(); it++){
        delete *it;
    }

    for(vector<IPluginCommand*>::iterator it = _commands.begin(); it != _commands.end(); it++){
        delete *it;
    }

    if(_socket){
        Logger::log("Deleting socket", Logger::LOG_INFO);
        delete _socket;
    }
}

void IRCBase::_setupCommands(){
    IPluginCommand* enqueue_message = new EnqueueMessageCommand(this);

    // do stuff

    _commands.push_back(enqueue_message);
}

bool IRCBase::_connect(){
    if(_socket == 0){
        try{
            string host = _config->getValue("connection", "host");
            int port = atoi(_config->getValue("connection", "port").c_str());
            _socket = new IRCSocket(const_cast<char*>(host.c_str()), port);
            if(_socket->createConnection()){
                Logger::log("Connected to " + host + ":" + _config->getValue("connection", "port"), Logger::LOG_INFO);
            }else{
                Logger::log("Could not connect to " + host + ":" + _config->getValue("connection", "port"), Logger::LOG_WARNING);
            }
        }catch(string message){
            Logger::log(message, Logger::LOG_ERROR);
        }
    }

    // Return if we already are connected
    return true;
}

void IRCBase::__listen(){
    while(_doListen){

        string message = _socket->readMessage("\r\n");
        
        if(message.size() > 0){
            Logger::log(message, Logger::LOG_INFO);
            /*
            try{
                IRCMessage ircmessage(message);
                Logger::log(ircmessage.toRFCFormat(), Logger::LOG_INFO);
            }catch(GenericIRCBaseException& ex){
                Logger::log(ex.what(), Logger::LOG_ERROR);
            }
            */
        }
         
        //Logger::log("message: " + message, Logger::LOG_INFO);
    }
    Logger::log("No longer listening", Logger::LOG_INFO);
}

void* IRCBase::_listen(void* ptr){
    IRCBase* ircbase = (IRCBase*)ptr;
    ircbase->_doListen = true;
    ircbase->__listen();
    return 0;
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
