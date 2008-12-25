
#include <vector>
#include <stdlib.h>
#include <unistd.h> /* For usleep */

#include "IRCSocket.h"
#include "IRCBase.h"
#include "IRCBaseHookPoint.h"
#include "Logger.h"
#include "EnqueueMessageCommand.h"
#include "IRCMessage.h"
#include "IPluginCommand.h"

using sentry::Logger;

IRCBase::IRCBase(string name) throw(string){

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

void* IRCBase::__listen(void* ptr){
    IRCBase* ircbase = (IRCBase*)ptr;
    ircbase->_doListen = true;
    ircbase->_listen();
    return 0;
}

void* IRCBase::__queueListen(void* ptr){
    IRCBase* ircbase = (IRCBase*)ptr;
    ircbase->_queueListen();
    return 0;
}

void IRCBase::_listen(){
    while(_doListen){

        /* Skip the read if the socket is disconnected */
        if( ! _socket->connected()){
            _doListen = false;
            continue;
        }

        string message = _socket->readMessage(IRCMessage::MESSAGE_SEPARATOR);

        if(message.size() > 0){
            // execute the IPluginCommands attached to the post_receive hookpoint
            IHookPoint* post_receive = _findHookPoint("ircbase.post_receive");

            if(post_receive){
                Logger::log(message, Logger::LOG_INFO);
                map<string, IPluginCommand*> commands = post_receive->getAttachedPluginCommands();
                vector<string> params;
                params.push_back(message);

                for(map<string, IPluginCommand*>::iterator it = commands.begin(); it != commands.end(); it++){
                    IPluginCommand* command = it->second;
                    command->execute(params);
                }
            }
        }

    }
    Logger::log("No longer listening", Logger::LOG_INFO);
}

void IRCBase::_queueListen(){
    while(_doListen){
        if( ! _socket->connected() ){
            continue;
        }

        while( ! _messageQueue.empty()){
            string message = *(_messageQueue.begin());
            if(_socket->sendMessage(message)){
                _messageQueue.erase(_messageQueue.begin());
            }
        }

        usleep(100000);
    }
}

/* create the socket-listener thread */
void IRCBase::_createListenerThread(){
        switch(pthread_create(&this->_listener, NULL, IRCBase::__listen, (void*)this )){
            case 0:
                switch(pthread_detach(_listener)){
                    case 0:
                        break;
                    case EINVAL:
                        Logger::log("Provided thread does not refer to a joinable thread"
                            , Logger::LOG_ERROR);
                        _doListen = false;
                        break;
                    case ESRCH:
                        Logger::log("Thread with specified ID could not be found"
                            , Logger::LOG_ERROR);
                        _doListen = false;
                        break;
                }
                break;
            case EAGAIN:
                Logger::log("Could not create new thread due to a lack of resources, of the maximum amount of threads has been reached."
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
            case EINVAL:
                Logger::log("Invalid attribute(2) while creating thread"
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
            case EPERM:
                Logger::log("Insufficient permissions to set scheduling parameters or scheduling policy."
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
        }
}

/* create the messagequeue-listener thread */
void IRCBase::_createQueueListenerThread(){
        switch(pthread_create(&this->_queueListener, NULL, IRCBase::__queueListen, (void*)this )){
            case 0:
                switch(pthread_detach(_queueListener)){
                    case 0:
                        break;
                    case EINVAL:
                        Logger::log("Provided thread does not refer to a joinable thread"
                            , Logger::LOG_ERROR);
                        _doListen = false;
                        break;
                    case ESRCH:
                        Logger::log("Thread with specified ID could not be found"
                            , Logger::LOG_ERROR);
                        _doListen = false;
                        break;
                }
                break;
            case EAGAIN:
                Logger::log("Could not create new thread due to a lack of resources, of the maximum amount of threads has been reached."
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
            case EINVAL:
                Logger::log("Invalid attribute(2) while creating thread"
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
            case EPERM:
                Logger::log("Insufficient permissions to set scheduling parameters or scheduling policy."
                    , Logger::LOG_ERROR);
                _doListen = false;
                break;
        }
}

/* Setup the commands this plug-in provides */
void IRCBase::_setupCommands(){
    IPluginCommand* enqueue_message = new EnqueueMessageCommand(this);

    // do stuff ??

    _commands.push_back(enqueue_message);
}

/* Setup the hookpoints this plug-in provides */
void IRCBase::_setupHookpoints(){
    IHookPoint* post_receive = new IRCBaseHookPoint("ircbase.post_receive");

    _providingHookPoints.push_back(post_receive);
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
IHookPoint* IRCBase::_findHookPoint(string name){
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
    return _doListen;
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
        _doListen = false;
        this->_createListenerThread();
        this->_createQueueListenerThread();
        return true;
    }
    return false;
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
