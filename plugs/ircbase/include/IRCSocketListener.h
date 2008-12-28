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
#ifndef IRCSOCKETLISTENER_H
#define	IRCSOCKETLISTENER_H

#include <Runnable.h>
#include "IRCSocket.h"
#include "IHookPoint.h"

using libthread::Runnable;
using sentry::IHookPoint;

class IRCSocketListener : public Runnable {
private:
    IRCSocket* _socket;
    IHookPoint* _hookpoint;
    bool _doListen;

public:

    IRCSocketListener(IRCSocket* socket, IHookPoint* hookpoint);
    virtual ~IRCSocketListener();

    void run();
};

#endif	/* IRCSOCKETLISTENER_H */

