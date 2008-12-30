/**
 * This file is part of Sentry.
 *
 * Sentry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sentry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sentry.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MESSAGEQUEUELISTENER_H
#define	MESSAGEQUEUELISTENER_H

#include <string>
#include <vector>
#include <Runnable.h>
#include "IRCSocket.h"

using std::string;
using std::vector;
using libthread::Runnable;

class MessageQueueListener : public Runnable {
private:
    vector<string>* _queue;
    IRCSocket* _socket;
    bool _doListen;

public:

    MessageQueueListener(IRCSocket* socket, vector<string>* queue);
    ~MessageQueueListener();

    void run();
};

#endif	/* MESSAGEQUEUELISTENER_H */

