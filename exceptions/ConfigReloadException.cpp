#include "ConfigReloadException.h"

sentry::ConfigReloadException::ConfigReloadException(){

}

sentry::ConfigReloadException::ConfigReloadException(string what){
    _what = what;
}

sentry::ConfigReloadException::~ConfigReloadException(){

}

string sentry::ConfigReloadException::what() throw() {
    if(_what.empty()){
        return "Could not reload config.";
    }
    return _what;
}

void sentry::ConfigReloadException::setWhat(string what) throw() {
    _what = what;
}

