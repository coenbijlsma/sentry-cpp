
#include <vector>
#include "Logger.h"

#include "CheckUserJoinCommand.h"
#include "IRCMessage.h"
#include "GenericIRCBaseException.h"

using sentry::Logger;

string CheckUserJoinCommand::_JOIN_COMMAND("JOIN");

CheckUserJoinCommand::CheckUserJoinCommand(IRCBase* plugin){
    _plugin = plugin;
    _name = string("ircbase.check_user_join");
}

CheckUserJoinCommand::~CheckUserJoinCommand(){

}

IPlugin* CheckUserJoinCommand::getPlugin(){
    return _plugin;
}

string CheckUserJoinCommand::getName(){
    return _name;
}

map<string, IHookPoint*> CheckUserJoinCommand::getHookPoints(){
    return _hookpoints;
}

void CheckUserJoinCommand::addAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp != _hookpoints.end() ){ return; }
    _hookpoints[hookpoint->getName()] = hookpoint;
}

void CheckUserJoinCommand::removeAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp == _hookpoints.end() ){ return; }
    _hookpoints.erase(hp);
}

void CheckUserJoinCommand::execute(vector<string> params){
    if(params.size() == 0){ return; }

    string message = params.at(0);

    try{
        IRCMessage ircmessage(message);
        if(ircmessage.getCommand() == CheckUserJoinCommand::_JOIN_COMMAND){
            if( ! ircmessage.getPrefix()){
                Logger::log("CheckUserJoinCommand::execute -- missing message prefix", Logger::LOG_INFO);
                return;
            }

            vector<string> msgparams = ircmessage.getParams();
            if(msgparams.size() == 0){
                Logger::log("CheckUserJoinCommand::execute -- missing parameters in message", Logger::LOG_WARNING);
                return;
            }

            string channel(msgparams.at(0));
            if(channel.at(0) == ':'){
                channel = channel.substr(1);
            }
            string user(ircmessage.getPrefix()->getUser());
            string nick(ircmessage.getPrefix()->getServerOrNick());
            string host(ircmessage.getPrefix()->getHost());

            // Execute commands in the post_join_user hookpoint
            IHookPoint* post_join_user = _plugin->findHookPoint("ircbase.post_join_user");
            if(post_join_user){
                vector<string> post_join_user_params;
                post_join_user_params.push_back(user);
                post_join_user_params.push_back(host);
                post_join_user_params.push_back(nick);
                post_join_user_params.push_back(channel);

                Logger::log(nick + "@" + host + " joined " + channel, Logger::LOG_INFO);
                map<string, IPluginCommand*> commands = post_join_user->getAttachedPluginCommands();

                for(map<string, IPluginCommand*>::iterator it = commands.begin(); it != commands.end(); it++){
                    IPluginCommand* command = it->second;
                    command->execute(post_join_user_params);
                }
            }
            //_plugin->enqueue("MODE " + channel + " +o " + ircmessage.getPrefix()->getServerOrNick() + IRCMessage::MESSAGE_SEPARATOR);
        }
    }catch(GenericIRCBaseException ex){
        Logger::log("CheckUserJoinCommand::execute -- GenericIRCBaseException -- " + string(ex.what()), Logger::LOG_WARNING);
    }
}