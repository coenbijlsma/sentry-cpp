#ifndef NOSUCHSYMBOLEXCEPTION_H
#define NOSUCHSYMBOLEXCEPTION_H

#include <exception>

using std::exception;

class NoSuchSymbolException : public exception {
public:
    
    virtual const char* what() const throw();
};

#endif
