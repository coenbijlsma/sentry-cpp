
#include <vector>
#include <stdlib.h>

#include <Runnable.h>
#include <ThreadException.h>

#include "IRCSocket.h"
#include "IRCBase.h"
#include "IRCBaseHookPoint.h"
#include "Logger.h"
#include "IRCMessage.h"
#include "IPluginCommand.h"
#include "ConfigReloadException.h"

#include "EnqueueMessageCommand.h"
#include "CheckUserJoinCommand.h"
#include "AutoOpCommand.h"
#include "PongCommand.h"

using sentry::Logger;
using sentry::ConfigReloadException;

using libthread::Runnable;
using libthread::ThreadException;

IRCBase::IRCBase(string name) throw(string){

    _socket = 0;
    Logger::setDestination(Logger::DEST_STDOUT);

    _name = name;
    _config = new SentryConfig("./ircbase.conf");

    string logdestination = _config->getValue("log", "destination");
    if(logdestination == "file"){
        Logger::setDestination(Logger::DEST_FILE);
        Logger::setlogFile(_config->getValue("log", "filename"));
    }else if(logdestination == "database"){
        Logger::setDestination(Logger::DEST_DATABASE);
    }
    
    _setupCommands();
    _setupHookpoints();

    /*
     * Attach the check_user_join command to the post_receive hookpoint.
     */
    IPluginCommand* check_user_join = this->findCommand("ircbase.check_user_join");
    if(check_user_join){
        IHookPoint* post_receive = this->findHookPoint("ircbase.post_receive");

        if(post_receive){
            post_receive->attach(check_user_join);
        }
    }

    /**
     * Attach the post_join_user command to the auto_op hookpoint
     */
    IPluginCommand* auto_op = this->findCommand("ircbase.auto_op");
    if(auto_op){
        IHookPoint* post_join_user = this->findHookPoint("ircbase.post_join_user");

        if(post_join_user){
            post_join_user->attach(auto_op);
        }
    }

    /**
     * Attach the pong command to the post_receive hookpoint
     */
    IPluginCommand* pong = this->findCommand("ircbase.pong");
    if(pong){
        IHookPoint* post_receive = this->findHookPoint("ircbase.post_receive");

        if(post_receive){
            post_receive->attach(pong);
        }
    }
}

IRCBase::~IRCBase(){
    /* Stop the listening thread */

    for(vector<IHookPoint*>::iterator it = _providingHookPoints.begin(); it != _providingHookPoints.end(); it++){
        delete *it;
    }

    for(vector<IPluginCommand*>::iterator it = _commands.begin(); it != _commands.end(); it++){
        delete *it;
    }

    if(_config){
        delete _config;
    }

    if(_listener){
        delete _listener;
    }

    if(_listenerThread){
        delete _listenerThread;
    }

    if(_queueListener){
        delete _queueListener;
    }

    if(_queueListenerThread){
        delete _queueListenerThread;
    }

    if(_socket){
        Logger::log("Deleting socket", Logger::LOG_INFO);
        delete _socket;
    }
}

/* create the socket-listener thread */
void IRCBase::_createListenerThread(){
    try{
        _listener = new IRCSocketListener(this->_socket, this->findHookPoint("ircbase.post_receive"));
        _listenerThread = new Thread( _listener );
        _listenerThread->start();
        _listenerThread->detach();
    }catch(ThreadException ex){
        Logger::log(ex.what(), Logger::LOG_ERROR);
    }

}

/* create the messagequeue-listener thread */
void IRCBase::_createQueueListenerThread(){
    try{
        _queueListener = new MessageQueueListener(this->_socket, &(this->_messageQueue) );
        _queueListenerThread = new Thread( _queueListener );
        _queueListenerThread->start();
        _queueListenerThread->detach();
    }catch(ThreadException ex){
        Logger::log(ex.what(), Logger::LOG_ERROR);
    }
}

/* Setup the commands this plug-in provides */
void IRCBase::_setupCommands(){
    IPluginCommand* enqueue_message = new EnqueueMessageCommand(this);
    IPluginCommand* check_user_join = new CheckUserJoinCommand(this);
    IPluginCommand* auto_op = new AutoOpCommand(this, this->_config);
    IPluginCommand* pong = new PongCommand(this);

    // do stuff ??

    _commands.push_back(enqueue_message);
    _commands.push_back(check_user_join);
    _commands.push_back(auto_op);
    _commands.push_back(pong);
}

/* Setup the hookpoints this plug-in provides */
void IRCBase::_setupHookpoints(){
    IHookPoint* post_receive = new IRCBaseHookPoint("ircbase.post_receive");
    IHookPoint* post_join_user = new IRCBaseHookPoint("ircbase.post_join_user");

    _providingHookPoints.push_back(post_receive);
    _providingHookPoints.push_back(post_join_user);
}

/* Connects to the server with the provided data of the configfile */
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

/* Finds a hookpoint by name */
IHookPoint* IRCBase::findHookPoint(string name){
    for(vector<IHookPoint*>::iterator it = _providingHookPoints.begin(); it != _providingHookPoints.end(); it++){
        IHookPoint* hookpoint = *it;
        if(hookpoint->getName() == name){
            return hookpoint;
        }
    }

    return (IHookPoint*)0;
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

bool IRCBase::isActive(){
    return this->_socket->connected();
}

bool IRCBase::activate(){
    if(_connect()){

        /* Enqueue the messages to register sentry */
        this->enqueue("PASS "
            + _config->getValue("connection", "password")
            + IRCMessage::MESSAGE_SEPARATOR);
        this->enqueue("NICK "
            + _config->getValue("connection", "nick")
            + IRCMessage::MESSAGE_SEPARATOR);
        this->enqueue("USER "
            + _config->getValue("connection", "name")
            + " foo bar "
            + _config->getValue("connection", "realname")
            + IRCMessage::MESSAGE_SEPARATOR);
        this->enqueue("JOIN "
            + _config->getValue("connection", "channel")
            + IRCMessage::MESSAGE_SEPARATOR);

        /* Setup the threads */
        this->_createListenerThread();
        this->_createQueueListenerThread();
        return true;
    }
    return false;
}

bool IRCBase::reloadConfig(){
    try{
        _config->reload();
        return true;
    }catch(ConfigReloadException ex){
        Logger::log(ex.what(), Logger::LOG_WARNING);
        return false;
    }
}

void IRCBase::enqueue(string message){
    _messageQueue.push_back(message);
    Logger::log("Enqueued " + message, Logger::LOG_INFO);
}

extern "C" IPlugin* create_plugin(){
    string name("ircbase");
    return (IPlugin*)new IRCBase(name);
}

extern "C" bool destroy_plugin(IPlugin* plugin){
    delete plugin;
    return true;
}
