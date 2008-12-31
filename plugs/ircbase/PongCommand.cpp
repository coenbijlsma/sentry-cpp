#include "PongCommand.h"
#include "IRCMessage.h"
#include "GenericIRCBaseException.h"
#include "Logger.h"

using sentry::Logger;

string PongCommand::_PING_COMMAND("PING");

PongCommand::PongCommand(IRCBase* plugin) : IRCBaseAbstractCommand(plugin) {
    _name = "ircbase.pong";
}

PongCommand::~PongCommand(){

}

void PongCommand::execute(vector<string> params) {
    if(params.size() == 0){
        return;
    }

    try{
        string message = params.at(0);
        IRCMessage ircmessage(message);

        if(ircmessage.getCommand() == PongCommand::_PING_COMMAND){
            vector<string> msgparams = ircmessage.getParams();

            if(msgparams.size() == 0){
                return;
            }

            string host = msgparams.at(0);
            if(host.size() == 0){
                return;
            }

            if(host[0] == ':'){
                host = host.substr(1);
            }

            string pong("PONG :" + host + IRCMessage::MESSAGE_SEPARATOR);
            _plugin->enqueue(pong);
        }
    }catch(GenericIRCBaseException ex){
        Logger::log(ex.what(), Logger::LOG_WARNING);
    }
}
