#ifndef NOSUCHLIBRARYEXCEPTION_H
#define NOSUCHLIBRARYEXCEPTION_H

#include <exception>

class NoSuchLibraryException : public exception {
public:
    
    virtual const char* what() const throw();
};

#endif
