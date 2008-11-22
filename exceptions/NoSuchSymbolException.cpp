#include "NoSuchSymbolException.h" 

const char* NoSuchSymbolException::what() const throw(){
    return "Symbol not found.";
}
