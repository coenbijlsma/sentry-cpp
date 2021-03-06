#include "IRCSocketListener.h"
#include "IRCMessage.h"
#include "IPluginCommand.h"
#include "Logger.h"

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

using sentry::Logger;
using sentry::IPluginCommand;

IRCSocketListener::IRCSocketListener(IRCSocket* socket, IHookPoint* hookpoint){
    _socket = socket;
    _hookpoint = hookpoint;
    _doListen = false;
}

IRCSocketListener::~IRCSocketListener(){

}

void IRCSocketListener::run(){
    _doListen = true;
    
    while(_doListen){
        if( ! _socket->connected()){
            Logger::log("Not connected to server " + _socket->getHost(), Logger::LOG_WARNING);
            _doListen = false;
            continue;
        }

        string message = _socket->readMessage(IRCMessage::MESSAGE_SEPARATOR);

        if(message.size() > 0){
            Logger::log(message, Logger::LOG_INFO);
            
            if(_hookpoint){
                map<string, IPluginCommand*> commands = _hookpoint->getAttachedPluginCommands();
                vector<string> params;
                params.push_back(message);

                for(map<string, IPluginCommand*>::iterator it = commands.begin(); it != commands.end(); it++){
                    IPluginCommand* command = it->second;
                    command->execute(params);
                }
            }
        }
    }
}
