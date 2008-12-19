#include "BufferedSocketReader.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

BufferedSocketReader::BufferedSocketReader(int fd, bool blocking){
    _fd = fd;
    _blocking = blocking;

    if(!_blocking){
        int flags = fcntl(_sockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(_sockfd, flags, 0);
    }
}

BufferedSocketReader::~BufferedSocketReader(){
    
}

void BufferedSocketReader::_fillBuffer(){

    char* newbuf = (char*)malloc(this->_bufsize +1);
    memset(newbuf, 0, this->_bufsize +1);

    switch(recv(_fd, chars, this->_bufsize, 0)){
        case 0:
            // shutdown
            break;
        case -1:
            // error
            break;
        default:
            while(newbuf){
                _buffer.push_back(*newbuf);
                newbuf++;
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

string BufferedSocketReader::readLine(){
    string line;

    while( (char c = (char)this->read()) != '\n' ){
        line.append(1, c);
    }

    return line;
}
