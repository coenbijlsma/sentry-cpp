#include "MessageQueueListener.h"
#include <unistd.h> /* For usleep */

MessageQueueListener::MessageQueueListener(IRCSocket* socket, vector<string>* queue){
    _socket = socket;
    _queue = queue;
    _doListen = false;
}

MessageQueueListener::~MessageQueueListener(){

}

void MessageQueueListener::run(){
    _doListen = true;

    while(_doListen){
        if( ! _socket->connected()){
            _doListen = false;
            continue;
        }

        while( ! _queue->empty() ){
            string message = *(_queue->begin());
            if(_socket->sendMessage(message)){
                _queue->erase(_queue->begin());
            }
        }

        usleep(100000);
    }
}
