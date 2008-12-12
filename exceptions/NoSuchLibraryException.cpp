#include "NoSuchLibraryException.h" 

const char* NoSuchLibraryException::what() const throw(){
    if(_what == 0){
        return "Library not found.";
    }
    return _what;
}

void NoSuchLibraryException::setWhat(string what) throw() {
    _what = what.c_str();
}