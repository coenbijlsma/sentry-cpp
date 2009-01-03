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

#include <Thread.h>
#include "IPlugin.h"
#include "IPluginProvider.h"
#include "IHookPoint.h"
#include "IPluginCommand.h"
#include "IRCSocket.h"
#include "SentryConfig.h"

#include "IRCSocketListener.h"
#include "MessageQueueListener.h"

using sentry::IPlugin;
using sentry::IPluginProvider;
using sentry::IHookPoint;
using sentry::IPluginCommand;
using sentry::SentryConfig;

using libthread::Thread;
using libthread::Runnable;

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

    /*--------------------------------------------------------------------------
     *                          PRIVATE VARIABLES
     -------------------------------------------------------------------------*/

    IPluginProvider* _provider;
    string _name;
    vector<IHookPoint*> _providingHookPoints;
    vector<string> _dependencies;
    vector<IPluginCommand*> _commands;
    vector<string> _messageQueue; // queue of messages to be sent over the socket

    /* Thread stuff */
    IRCSocketListener* _listener;
    MessageQueueListener* _queueListener;    
    Thread* _listenerThread; // listens for new messages on the socket
    Thread* _queueListenerThread; // looks if there is anything in the messagequeue and sends it to the socket
    
    IRCSocket* _socket;

    SentryConfig* _config;

    /*--------------------------------------------------------------------------
     *                          PRIVATE METHODS
     -------------------------------------------------------------------------*/

    /**
     * Creates the thread that listens for incoming messages
     */
    void _createListenerThread();

    /**
     * Creates the thread that listens for outgoing messages
     */
    void _createQueueListenerThread();

    /**
     * Creates the commands that this plug-in provides
     */
    void _setupCommands();

    /**
     * Creates the hookpoints that are available in this plug-in
     */
    void _setupHookpoints();

    /*
     * Connects to the server specified in the config file of thie plug-in,
     * and tries to register at the channel.
     */
    bool _connect();

    /**
     * Tries to attach the plugincommand that has the given name to the hookpoint.
     */
    bool _attachTo(string plugincommand, string hookpoint);
    
public:

    /*--------------------------------------------------------------------------
     *                          PUBLIC METHODS
     -------------------------------------------------------------------------*/

    /**
     * Constructor
     * @param string name The name of the plugin
     * @throws string If the config file ircbase.conf is not found.
     */
    IRCBase(string name) throw(string);

    /**
     * Destructor.
     * Also cleans up the hookpoints and commands this plug-in provides.
     */
    virtual ~IRCBase();

    /**
     * Sets the plug-in provider.
     * @param IPluginProvider* The plug-in provider.
     */
    void setProvider(IPluginProvider* provider);

    /**
     * @return string The name of this plug-in
     */
    string getName();

    /**
     * Returns the hookpoints this plug-in provides., currently being only
     * ircbase.post_receive.
     * @return vector<IHookPoint*> The hookpoints.
     */
    vector<IHookPoint*> getProvidingHookPoints();

    /**
     * Returns the names of the plug-ins this plug-in depends on.
     * @return vector<string> The dependencies.
     */
    vector<string> getDependencies();

    /**
     * Returns the commands this plug-in provides, currently being only
     * ircbase.enqueue_message.
     * @return vector<IPluginCommand*> The commands.
     * @see EnqueueMessageCommand.h
     */
    vector<IPluginCommand*> getCommands();

    /**
     * Finds a IPluginCommand by name.
     * @param string name The name of the command.
     * @return IPluginCommand* The found command, or (IPluginCommand*)0 of the
     * command is not found.
     */
    IPluginCommand* findCommand(string name);

    /**
     * Finds a hookpoint by name
     */
    IHookPoint* findHookPoint(string name);

    /**
     * @return bool Whether this plug-in is currently active.
     */
    bool isActive();

    /**
     * Activates the plug-in. This effectively connects to the specified server,
     * and starts the listening threads.
     */
    bool activate();

    /**
     * Reloads the config for this plug-in
     */
    bool reloadConfig();

    /**
     * Enqueues the given string to send it later on. This function is actually
     * called by the EnqueueMessageCommand when it is executed.
     * @param string The message in RFC 1459 format to be enqueued.
     */
    void enqueue(string message);

};

#endif	/* IRCBASE_H */
