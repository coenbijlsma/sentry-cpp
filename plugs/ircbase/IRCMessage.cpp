
#include "Logger.h"

#include "IRCMessage.h"
#include "IRCBase.h"
#include "StringTokenizer.h"

using sentry::StringTokenizer;
using sentry::Logger;

string IRCMessage::MESSAGE_SEPARATOR = "\r\n";

IRCMessage::IRCMessage() throw() {

}

IRCMessage::IRCMessage(string raw) throw(GenericIRCBaseException) {
    this->_init(raw);
}

IRCMessage::~IRCMessage() throw() {
    if(this->_prefix != 0){
        delete this->_prefix;
    }
}

void IRCMessage::_init(string raw) throw(GenericIRCBaseException){
    if(raw.size() == 0){
        GenericIRCBaseException ex("Cannot initialise empty raw message. Maybe use standard constructor?");
        throw ex;
    }
    StringTokenizer st(raw, ' ');

    if(raw.at(0) == IRCMessagePrefix::PREFIX_START){
        _prefix = new IRCMessagePrefix(st.next());
    }else{
        _prefix = 0;
    }

    if( ! st.hasNext()){
        GenericIRCBaseException ex(raw + " :: has no command!");
        throw ex;
    }
    _command = st.next();

    while(st.hasNext()){
        this->addParam(st.next());
    }
}

void IRCMessage::setPrefix(IRCMessagePrefix* prefix) throw(){
    // clean up the last prefix first
    if(this->_prefix != 0){
        delete this->_prefix;
    }
    this->_prefix = prefix;
}

void IRCMessage::setCommand(string command) throw() {
    this->_command = command;
}

void IRCMessage::addParam(string param) throw() {
    this->_params.push_back(param);
}

void IRCMessage::addAllParams(vector<string> params) throw() {
    for(unsigned int i = 0; i < params.size(); i++){
        this->addParam(params.at(i));
    }
}

void IRCMessage::removeParam(string param) throw() {
    for(vector<string>::iterator it = _params.begin(); it != _params.end(); it++){
        if( *it == param){
            _params.erase(it);
            return;
        }
    }
}

IRCMessagePrefix* IRCMessage::getPrefix() throw() {
    return this->_prefix;
}

string IRCMessage::getCommand() throw() {
    return this->_command;
}

vector<string> IRCMessage::getParams() throw() {
    return this->_params;
}

string IRCMessage::getParamsAsString() throw() {
    string ret;

    for(unsigned int i = 0; i < _params.size(); i++){
        ret += (i == 0) ? "" : " ";
        ret += _params.at(i);
    }
    return ret;
}

string IRCMessage::toRFCFormat() throw() {
    string rfc;

    if(_prefix != 0){
        rfc += _prefix->toRFCFormat();
        rfc += " ";
    }

    rfc += _command;

    if(_params.size() > 0){
        for(unsigned int i = 0; i < _params.size(); i++){
            rfc += " ";
            rfc += _params.at(i);
        }
    }

    rfc += IRCMessage::MESSAGE_SEPARATOR;
    return rfc;
}