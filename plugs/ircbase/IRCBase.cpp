
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
#include "MessageDispatcherCommand.h"

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
     * Attach the commands to the hookpoints.
     */
    this->_attachTo("ircbase.message_dispatcher", "ircbase.post_receive");
    //this->_attachTo("ircbase.check_user_join", "ircbase.post_receive");
    this->_attachTo("ircbase.auto_op", "ircbase.post_receive_join");
    this->_attachTo("ircbase.pong", "ircbase.post_receive_ping");
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

bool IRCBase::_attachTo(string plugincommand, string hookpoint){
    IPluginCommand* command = this->findCommand(plugincommand);
    if(command){
        IHookPoint* hp = this->findHookPoint(hookpoint);

        if(hp){
            hp->attach(command);
            return true;
        }
        return false;
    }
    return false;
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
    _commands.push_back(new EnqueueMessageCommand(this));
    _commands.push_back(new CheckUserJoinCommand(this));
    _commands.push_back(new AutoOpCommand(this, this->_config));
    _commands.push_back(new PongCommand(this));
    _commands.push_back(new MessageDispatcherCommand(this, this->_config));
}

/* Setup the hookpoints this plug-in provides */
void IRCBase::_setupHookpoints(){
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_pass"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_nick"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_user"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_server"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_oper"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_quit"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_squit"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_join"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_part"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_mode"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_topic"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_names"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_list"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_invite"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_kick"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_version"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_stats"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_links"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_time"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_connect"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_trace"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_admin"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_info"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_privmsg"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_notice"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_who"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_whois"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_whowas"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_kill"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_ping"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_pong"));
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_receive_error"));
    
    _providingHookPoints.push_back(new IRCBaseHookPoint("ircbase.post_join"));
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
}

extern "C" IPlugin* create_plugin(){
    string name("ircbase");
    return (IPlugin*)new IRCBase(name);
}

extern "C" bool destroy_plugin(IPlugin* plugin){
    delete plugin;
    return true;
}
