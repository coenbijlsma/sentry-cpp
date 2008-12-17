#include "IRCSocket.h"
#include "Logger.h"
#include <fcntl.h>

IRCSocket::IRCSocket(char* host, int port){
    _port = port;

    struct hostent* h = gethostbyname(host);

    if(h == 0){
        _host = host;
    }else{
        _host = inet_ntoa(*((struct in_addr*)h->h_addr));
    }

    _connected = false;
    _pollTimeoutMSecs = 0;
}

IRCSocket::~IRCSocket(){
    if(_connected){
        disconnect();
    }
}

bool IRCSocket::connected(){
    return _connected;
}

bool IRCSocket::createConnection(){
    if( (_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        char* errmsg = strerror(errno);
        Logger::log("In IRCSocket::createConnection() " + string(errmsg), Logger::LOG_ERROR);
        return false;
    }

    /* set-up non-blocking socket */
    int flags = fcntl(_sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(_sockfd, flags, 0);

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = inet_addr(_host);

    if(connect(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1){
        close(_sockfd);
        char* errmsg = strerror(errno);
        Logger::log("In IRCSocket::createConnection() " + string(errmsg), Logger::LOG_ERROR);
        return false;
    }

    _connected = true;

    /* Setup polling */
    _pfd[0].fd = _sockfd;
    _pfd[0].events = POLLRDNORM;

    return true;
}

bool IRCSocket::disconnect(){
    if( ! _connected){
        return false;
    }

    if(shutdown(_sockfd, SHUT_RDWR) == -1){
        char* errmsg = strerror(errno);
        Logger::log("In IRCSocket::disconnect() " + string(errmsg), Logger::LOG_ERROR);
        return false;
    }

    _connected = false;

    if(close(_sockfd) == -1){
        _sockfd = 0; // just to be sure
        char* errmsg = strerror(errno);
        Logger::log("In IRCSocket::disconnect() " + string(errmsg), Logger::LOG_ERROR);
        return false;
    }

    return true;
}

bool IRCSocket::sendMessage(string msg){
    int bytes_sent;

    if( ! _connected){
        Logger::log("In IRCSocket::sendMessage()  Not connected", Logger::LOG_WARNING);
        return false;
    }

    if(msg[strlen(msg.c_str())] != '\0'){
        msg += '\0';
    }

    if( (bytes_sent = send(_sockfd, msg.c_str(), strlen(msg.c_str()), 0)) == -1
        || bytes_sent != (int)strlen(msg.c_str()) ){
        char* errmsg = strerror(errno);
        Logger::log("In IRCSocket::sendMessage() " + string(errmsg), Logger::LOG_ERROR);
        return false;
    }else{
        Logger::log("Successfully sent message " + msg, Logger::LOG_INFO);
    }

    return true;
}

string IRCSocket::readMessage(string delim){
    string temp_msg;
    if( ! _connected){
        Logger::log("In IRCSocket::readMessage() Not connected", Logger::LOG_WARNING);
        return temp_msg;
    }

    /*
    XXX Maybe fix this?

    */
    while(temp_msg.size() <= 512){
        char* chars = (char*)malloc(513);
        memset(chars, 0, 513);

        switch(recv(_sockfd, chars, 512, 0)){
            case 0:
                free(chars);
                return temp_msg;
            case -1:
                free(chars);
                Logger::log("In IRCSocket::readMessage() Error receiving data", Logger::LOG_ERROR);;
                return temp_msg;
            default:
                temp_msg.append(chars);
        }

        if(temp_msg.size() >= delim.size()){
            if(temp_msg.substr(temp_msg.size() - delim.size()) == delim){
                free(chars);
                return temp_msg;
            }
        }
        if(chars != 0){
            free(chars);
        }
    }

    return temp_msg;
}

int IRCSocket::pollConnection(){
    return poll(_pfd, 1, _pollTimeoutMSecs);
}

string IRCSocket::getHost(){
    return _host;
}

int IRCSocket::getPort(){
    return _port;
}

string IRCSocket::getProtocol(){
    return _protocol;
}

