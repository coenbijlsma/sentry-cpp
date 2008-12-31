#include "IRCSocket.h"
#include "Logger.h"
#include <fcntl.h>

using sentry::Logger;

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

    if(_br){
        delete _br;
    }

    if(_bw){
        delete _bw;
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

    _br = new BufferedSocketReader(_sockfd, false, 512);
    _bw = new BufferedSocketWriter(_sockfd, 513, 10);

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
    //int bytes_sent;

    if( ! _connected){
        Logger::log("In IRCSocket::sendMessage()  Not connected", Logger::LOG_WARNING);
        return false;
    }

    _bw->write(msg);
    _bw->flush();
    /*
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
    */

    return true;
}

string IRCSocket::readMessage(string delim){
    string temp_msg;

    /*
     * Check if the BufferedReader knows that the peer has shutdown the
     * connection.
     */
    if(_br->peerHasShutdown()){
        this->disconnect();
    }
    if( ! _connected){
        Logger::log("In IRCSocket::readMessage() Not connected", Logger::LOG_WARNING);
        return temp_msg;
    }
    temp_msg = _br->read(delim);
    
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

