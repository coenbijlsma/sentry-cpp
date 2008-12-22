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

#ifndef IRCBASE_H
#define	IRCBASE_H

#include <string>
#include <vector>
#include <pthread.h>
#include "IPlugin.h"
#include "IPluginProvider.h"
#include "IHookPoint.h"
#include "IPluginCommand.h"
#include "IRCSocket.h"
#include "SentryConfig.h"

using sentry::IPlugin;
using sentry::IPluginProvider;
using sentry::IHookPoint;
using sentry::IPluginCommand;
using sentry::SentryConfig;

using std::string;
using std::vector;

/**
 * @brief The IRC base plug-in
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-14
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 *
 * This class provides some IRC functionality for usage by other
 * Sentry plug-ins. The functionality this plug-in offers is very basic: it
 * attaches to the hookpoint core.post_load_plugins and when executed
 * creates a connection as specified in the configuration of the plug-in.
 * This plug-in provides the following hookpoint:
 * - ircbase.post_receive
 *
 */
class IRCBase : public IPlugin {
private:

    IPluginProvider* _provider;
    string _name;
    vector<IHookPoint*> _providingHookPoints;
    vector<string> _dependencies;
    vector<IPluginCommand*> _commands;
    vector<string> _messageQueue; // queue of messages to be sent over the socket
    pthread_t _listener;
    bool _doListen;
    
    IRCSocket* _socket;

    SentryConfig* _config;

    void _setupCommands();

    /*
     * Connects to the server specified in the config file of thie plug-in,
     * and tries to register at the channel.
     */
    bool _connect();

    /**
     * Does the actual listening
     */
    void __listen();

    /**
     * 
     */
    static void* _listen(void* ptr);
    
public:

    IRCBase(string name) throw(string);

    virtual ~IRCBase();

    /* IPlugin functions */
    
    void setProvider(IPluginProvider* provider);
    
    string getName();

    vector<IHookPoint*> getProvidingHookPoints();

    vector<string> getDependencies();

    vector<IPluginCommand*> getCommands();

    IPluginCommand* findCommand(string name);

    /* IRCBase functions */

    /**
     * Enqueues the given string to send it later on.
     */
    void enqueue(string message);

};

#endif	/* IRCBASE_H */
