#include "EnqueueMessageCommand.h"

EnqueueMessageCommand::EnqueueMessageCommand(IRCBase* plugin) : IRCBaseAbstractCommand(plugin){
    _name = "ircbase.enqueue_message";
}

EnqueueMessageCommand::~EnqueueMessageCommand(){}

void EnqueueMessageCommand::execute(vector<string> params){
    for(vector<string>::iterator it = params.begin(); it != params.end(); it++){
        _plugin->enqueue(*it);
    }
}