#include "NoSuchSymbolException.cpp" 

const char* NoSuchSymbolException::what(){
    return "Symbol not found.";
}
