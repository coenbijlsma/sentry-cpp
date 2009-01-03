#include "PongCommand.h"
#include "IRCMessage.h"
#include "GenericIRCBaseException.h"
#include "Logger.h"

using sentry::Logger;

PongCommand::PongCommand(IRCBase* plugin) : IRCBaseAbstractCommand(plugin) {
    _name = "ircbase.pong";
}

PongCommand::~PongCommand(){

}

void PongCommand::execute(vector<string> params) {
    if(params.size() < 4){
        return;
    }

    string host = params.at(3);

    if(host[0] == ':'){
        host = host.substr(1);
    }

    string pong("PONG :" + host + IRCMessage::MESSAGE_SEPARATOR);
    _plugin->enqueue(pong);
}
