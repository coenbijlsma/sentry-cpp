#include "IRCMessagePrefix.h"
#include "StringTokenizer.h"

#include "Logger.h"

using sentry::StringTokenizer;

using sentry::Logger;

char IRCMessagePrefix::PREFIX_START = ':';
char IRCMessagePrefix::PREFIX_UID = '!';
char IRCMessagePrefix::PREFIX_HID = '@';

IRCMessagePrefix::IRCMessagePrefix() throw(){
}

IRCMessagePrefix::IRCMessagePrefix(string raw) throw(GenericIRCBaseException){
    this->_init(raw);
}

IRCMessagePrefix::~IRCMessagePrefix() throw() {}

void IRCMessagePrefix::_init(string raw) throw(GenericIRCBaseException){
    if(raw.size() == 0){
        Logger::log("Returning because the raw string has size of 0", Logger::LOG_INFO);
        return;
    }

    if(raw.at(0) != IRCMessagePrefix::PREFIX_START){
        GenericIRCBaseException ex("The prefix must start with a colon");
        throw ex;
    }

    /*
     * Discard the colon because it is just the identifier of this prefix,
     * and split on the !
     */
    StringTokenizer st(raw.substr(1), IRCMessagePrefix::PREFIX_UID);

    /*
     * We can safely do this, because we already firgured out
     * that the raw string contains at least 1 character.
     */
    this->_serverOrNick = st.next();

    if(st.count() > 1){
        /*
         * The prefix could also contain a user and host
         */
        StringTokenizer stUserHost(st.next(), IRCMessagePrefix::PREFIX_HID);

        this->_user = stUserHost.next();

        if(stUserHost.count() > 1){
            /*
             * We also have a hostname
             */
            this->_host = stUserHost.next();
        }
    }
}

void IRCMessagePrefix::setServerOrNick(string son) throw() {
    this->_serverOrNick = son;
}

void IRCMessagePrefix::setUser(string user) throw() {
    this->_user = user;
}

void IRCMessagePrefix::setHost(string host) throw() {
    this->_host = host;
}

string IRCMessagePrefix::getServerOrNick() throw() {
    return this->_serverOrNick;
}

string IRCMessagePrefix::getUser() throw() {
    return this->_user;
}

string IRCMessagePrefix::getHost() throw() {
    return this->_host;
}

string IRCMessagePrefix::toRFCFormat() throw() {
    string rfc;
    if(this->_serverOrNick.empty()){
        return rfc;
    }

    rfc.append(1, IRCMessagePrefix::PREFIX_START);
    rfc += this->_serverOrNick;

    if( ! this->_user.empty()){
        rfc.append(1, IRCMessagePrefix::PREFIX_UID);
        rfc += this->_user;
    }

    if( ! this->_host.empty()){
        rfc.append(1, IRCMessagePrefix::PREFIX_HID);
        rfc += this->_host;
    }

    return rfc;

}