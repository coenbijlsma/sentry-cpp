
#include "Logger.h"

#include "AutoOpCommand.h"
#include "SentryConfigSection.h"
#include "StringTokenizer.h"
#include "IRCMessage.h"

using sentry::SentryConfigSection;
using sentry::StringTokenizer;
using sentry::Logger;

AutoOpCommand::AutoOpCommand(IRCBase* plugin, SentryConfig* config){
    _plugin = plugin;
    _config = config;
    _name = "ircbase.auto_op";
}

AutoOpCommand::~AutoOpCommand(){

}

IPlugin* AutoOpCommand::getPlugin(){
    return _plugin;
}

string AutoOpCommand::getName(){
    return _name;
}

map<string, IHookPoint*> AutoOpCommand::getHookPoints(){
    return _hookpoints;
}

void AutoOpCommand::addAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp != _hookpoints.end() ){ return; }
    _hookpoints[hookpoint->getName()] = hookpoint;
}

void AutoOpCommand::removeAttachedHookPoint(IHookPoint* hookpoint){
    if(hookpoint == 0){ return; }

    map<string, IHookPoint*>::iterator hp = _hookpoints.find(hookpoint->getName());
    if(hp == _hookpoints.end() ){ return; }
    _hookpoints.erase(hp);
}

void AutoOpCommand::execute(vector<string> params){
    bool userPresent = false;
    bool channelPresent = false;

    if(params.size() != 4){
        Logger::log("AutoOpCommand::execute() -- Wrong parameter count (need 4)", Logger::LOG_WARNING);
        return;
    }

    SentryConfigSection* section = this->_config->getSection("auto_op");

    if(section){
        string user = params.at(0);
        string host = params.at(1);
        string nick = params.at(2);
        string channel = params.at(3);
        string userhost = (user + "@" + host);

        string users = section->get("users");
        StringTokenizer st(users, ',');

        while(st.hasNext()){
            string next = st.next();
            if(userhost == next){
                userPresent = true; break;
            }
        }

        if(userPresent){
            string channels = section->get(userhost);

            if( ! channels.empty() ){
                StringTokenizer cst(channels, ',');

                while(cst.hasNext()){
                    string next = cst.next();
                    if(channel == next){
                        channelPresent = true; break;
                    }
                }

                if(channelPresent){
                    string opmsg("MODE " + channel + " +o " + nick + IRCMessage::MESSAGE_SEPARATOR);
                    _plugin->enqueue(opmsg);
                }
            }
        }
    }
}
