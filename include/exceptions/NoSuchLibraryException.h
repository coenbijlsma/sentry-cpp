#ifndef NOSUCHLIBRARYEXCEPTION_H
#define NOSUCHLIBRARYEXCEPTION_H

#include <exception>

using std::exception;

class NoSuchLibraryException : public exception {
public:
    
    virtual const char* what() const throw();
};

#endif
