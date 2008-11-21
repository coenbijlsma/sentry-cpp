#include "NoSuchLibraryException.cpp" 

const char* NoSuchLibraryException::what(){
    return "Library not found.";
}
