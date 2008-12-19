#include "BufferedSocketReader.h"
#include "Logger.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* For recv() */
#include <sys/socket.h> /* For recv() */

#include <iostream>

using std::cout;
using std::endl;

BufferedSocketReader::BufferedSocketReader(int fd, bool blocking, int bufsize) : _bufsize(bufsize) {
    _fd = fd;
    _blocking = blocking;

    if(!_blocking){
        int flags = fcntl(_fd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(_fd, flags, 0);
    }
}

BufferedSocketReader::~BufferedSocketReader(){
    
}

void BufferedSocketReader::_fillBuffer(){
    char* newbuf = (char*)malloc(this->_bufsize +1);
    memset(newbuf, 0, this->_bufsize +1);

    switch(recv(_fd, newbuf, this->_bufsize, 0)){
        case 0:
            // shutdown
            break;
        case -1:
            // error
            break;
        default:
            for(unsigned int i = 0; i <= strlen(newbuf); i++){
                _buffer.push_back(newbuf[i]);
            }

            break;
    }

    free(newbuf);
}

int BufferedSocketReader::read(){
    if(_buffer.size() == 0){
        _fillBuffer();
    }

    if(_buffer.size() > 0){
        char c = _buffer.at(0);
        _buffer.erase(_buffer.begin());
        return (int)c;
    }else{
        return -1;
    }
}

string BufferedSocketReader::read(string delim, bool includeDelim){
    string ret("");
    int i;
    
    while( (i = this->read()) != -1){
        ret.append(1, (char)i);

        if(ret.length() >= delim.length()){
            if(ret.substr(ret.size() - delim.size()) == delim){
                // return the delimeter?
                if(includeDelim){
                    return ret;
                }else{
                    return ret.substr(0, ret.size() - delim.size());
                }
            }
        }
    }

    return ret;
}

string BufferedSocketReader::readLine(){
    return this->read("\n", false);
}
