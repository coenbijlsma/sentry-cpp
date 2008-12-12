#include "NoSuchSymbolException.h" 

const char* NoSuchSymbolException::what() const throw(){
    if(_what == 0){
        return "Symbol not found.";
    }
    
    return _what;
}

void NoSuchSymbolException::setWhat(string what) throw(){
    _what = what.c_str();
}