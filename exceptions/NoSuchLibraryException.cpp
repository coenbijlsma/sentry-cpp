#include "NoSuchLibraryException.h" 

const char* NoSuchLibraryException::what() const throw(){
    return "Library not found.";
}
