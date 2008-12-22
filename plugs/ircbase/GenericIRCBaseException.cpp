#include "GenericIRCBaseException.h"

GenericIRCBaseException::GenericIRCBaseException() throw() : exception() {
    
}

GenericIRCBaseException::GenericIRCBaseException(string what) throw() : exception(), _what(what) {

}

GenericIRCBaseException::~GenericIRCBaseException() throw() {}

void GenericIRCBaseException::setWhat(string what) throw(){
    this->_what = what;
}

const char* GenericIRCBaseException::what() const throw() {
    return _what.c_str();
}
