#include "BufferedSocketWriter.h"
#include "Logger.h"
#include <sys/types.h> /* For send() */
#include <sys/socket.h> /* For send () */
#include <errno.h> /* For errno */
#include <string.h> /* For strerror() */
#include <math.h> /* For chopping up strings */
#include <vector>

#include <iostream>

using sentry::Logger;

using std::cout;
using std::endl;

BufferedSocketWriter::BufferedSocketWriter(int fd, unsigned int maxStringSize, int pollTimeout)
    : _maxStringSize(maxStringSize), _pollTimeout(pollTimeout), _fd(fd){

    /* Setup polling */
    _pfd[0].fd = _fd;
    _pfd[0].events = POLLOUT;
}

BufferedSocketWriter::~BufferedSocketWriter(){

}

bool BufferedSocketWriter::_write(){

    while( ! _buffer.empty() ){
        string message = _buffer.at(0);
        int bytes_to_write = strlen(message.c_str());
        
        while(bytes_to_write != 0){
            int poll_result = poll(_pfd, 1, _pollTimeout);

            /* Stop if poll() went wrong */
            if(poll_result == 0 || poll_result == -1){
                string errmsg(strerror(errno));
                Logger::log(errmsg, Logger::LOG_WARNING);
                return false;
            }

            int send_result = send(_fd, message.c_str(), strlen(message.c_str()), MSG_DONTWAIT );
            if(send_result != -1){
                bytes_to_write -= send_result; //
                if(bytes_to_write != 0){
                    message = message[send_result];
                }
            }else{
                string errmsg(strerror(errno));
                Logger::log(errmsg, Logger::LOG_WARNING);
            }
        }
        _buffer.erase(_buffer.begin());
    }
    return true;
}

bool BufferedSocketWriter::write(int c){

    if(_buffer.size() > 0){
        if( ! _write() ){
            return false;
        }
    }

    string s("");
    s.append(1, (char)c);
    _buffer.push_back(s);
    return true;
}

bool BufferedSocketWriter::write(string s){
    if(s.size() < _maxStringSize){
        _buffer.push_back(s);
    }else{
        int factor = (int)ceil( (s.size() / _maxStringSize) * 1.0 );
        for(int i = 0; i < factor; i++){
            string sub;
            if(i + 1 == factor){
                sub = s.substr(i * _maxStringSize, s.size() - (i * _maxStringSize));
            }else{
                sub = s.substr(i * _maxStringSize, _maxStringSize -1);
            }
            _buffer.push_back(sub);
        }
    }

    return true;
}

bool BufferedSocketWriter::flush(){
    return this->_write();
}
