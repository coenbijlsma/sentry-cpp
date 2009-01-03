#include "MessageDispatcherCommand.h"
#include "IHookPoint.h"
#include "IPluginCommand.h"
#include "Logger.h"
#include "IRCMessage.h"
#include "SentryConfigSection.h"
#include "ConvertException.h"
#include <algorithm> /* To transform a string to lowercase */

using sentry::IHookPoint;
using sentry::IPluginCommand;
using sentry::Logger;
using sentry::ConvertException;

MessageDispatcherCommand::MessageDispatcherCommand(IRCBase* plugin, SentryConfig* config)
        : IRCBaseAbstractCommand(plugin), _config(config) {

    _name = "ircbase.message_dispatcher";
}

MessageDispatcherCommand::~MessageDispatcherCommand(){

}

void MessageDispatcherCommand::execute(vector<string> params){
    if(params.size() != 1){
        return;
    }

    string message = params.at(0);
    IRCMessage ircmessage(message);
    string command = ircmessage.getCommand();

    // Check the amount of parameters
    SentryConfigSection* section = _config->getSection("messages");

    if(section){
        try{
            unsigned int minparams = section->getInt(command);
            if(ircmessage.getParams().size() < minparams){
                Logger::log("Wrong parameter count, discarding message", Logger::LOG_WARNING);
                return;
            }
        }catch(ConvertException ex){
            Logger::log(ex.what() , Logger::LOG_ERROR );
        }
    }else{
        Logger::log("Section 'messages' not found, discarding message.", Logger::LOG_WARNING);
        return;
    }
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    IHookPoint* hookpoint = _plugin->findHookPoint("ircbase.post_receive_" + command);

    if(hookpoint){
        string user;
        string nick;
        string host;

        if(ircmessage.getPrefix()){
            user = ircmessage.getPrefix()->getUser();
            nick = ircmessage.getPrefix()->getServerOrNick();
            host = ircmessage.getPrefix()->getHost();
        }
        vector<string> mparams;
        vector<string> ircmessageparams = ircmessage.getParams();
        mparams.push_back(user);
        mparams.push_back(host);
        mparams.push_back(nick);

        for(unsigned int i = 0; i < ircmessageparams.size(); i++){
            mparams.push_back(ircmessageparams.at(i));
        }

        Logger::log("Dispatching message to " + hookpoint->getName(), Logger::LOG_INFO);
        map<string, IPluginCommand*> commands = hookpoint->getAttachedPluginCommands();

        for(map<string, IPluginCommand*>::iterator it = commands.begin();
                it != commands.end(); it++){
            IPluginCommand* cmd = it->second;
            cmd->execute(mparams);
        }
    }
}
