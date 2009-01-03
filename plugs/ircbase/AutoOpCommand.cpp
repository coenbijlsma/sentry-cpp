
#include "Logger.h"

#include "AutoOpCommand.h"
#include "SentryConfigSection.h"
#include "StringTokenizer.h"
#include "IRCMessage.h"

using sentry::SentryConfigSection;
using sentry::StringTokenizer;
using sentry::Logger;

AutoOpCommand::AutoOpCommand(IRCBase* plugin, SentryConfig* config) 
    : IRCBaseAbstractCommand(plugin), _config(config) {

    _name = "ircbase.auto_op";
}

AutoOpCommand::~AutoOpCommand(){

}

void AutoOpCommand::execute(vector<string> params){
    bool userPresent = false;
    bool channelPresent = false;

    SentryConfigSection* section = this->_config->getSection("auto_op");

    if(section && params.size() > 3){
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
