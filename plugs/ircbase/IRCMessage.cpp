
#include "Logger.h"

#include "IRCMessage.h"
#include "IRCBase.h"
#include "StringTokenizer.h"

using sentry::StringTokenizer;
using sentry::Logger;

string IRCMessage::MESSAGE_SEPARATOR = "\r\n";

IRCMessage::IRCMessage() throw() {
    this->_minParams = 0;
}

IRCMessage::IRCMessage(string raw) throw(GenericIRCBaseException) {
    this->_minParams = 0;
    this->_init(raw);
}

IRCMessage::~IRCMessage() throw() {
    if(this->_prefix != 0){
        delete this->_prefix;
    }
}

void IRCMessage::_init(string raw) throw(GenericIRCBaseException){
    bool haveLastParam = false;

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

    if(st.hasNext()){
        string lastParam;

        while(st.hasNext()){
            string param = st.next();
            if( (! param.empty()) && param.at(0) == ':'){
                if( ! haveLastParam ){
                    haveLastParam = true;
                    lastParam = param;
                }else{
                    lastParam += " ";
                    lastParam += param;
                }
            }else{
                if(haveLastParam){
                    lastParam += " ";
                    lastParam += param;
                }else{
                    this->addParam(param);
                }
            }
        }

        if(haveLastParam){
            this->addParam(lastParam);
        }
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