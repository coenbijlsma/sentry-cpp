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
#ifndef SENTRY_H
#define SENTRY_H

#include "IHookPoint.h"
#include "IPlugin.h"
#include "Logger.h"
#include "SentryConfig.h"
#include "IPluginProvider.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

/**
 * @brief The core class of Sentry.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 * @see IPlugin.h
 * @see IHookPoint.h
 *
 * This class is the core class of Sentry.
 * The goal of Sentry actually was to create a irc-bot as a learning project,
 * but as it turns out, Sentry has become not so much a bot after all: it has
 * changed to a small core that can read and handle plugins (or libraries, if
 * you will).
 * The whole idea is that Sentry provides a number of hooks any plug-in command
 * can hold onto, and be executed when described in the hook defenition.
 *
 * At the moment, Sentry provides three hookpoints:
 * - core.post_startup
 * - core.pre_shutdown
 * - core.post_load_plugins
 * The loaded plug-in commands are then respectively executed when Sentry has
 * started, and before Sentry shuts down.
 * If you create your own plug-in, you can define your own hookpoints. For
 * more information about this, see IPlugin.h
 *
 */

class Sentry : public IPluginProvider {
public:
    enum context_t {CONTEXT_LIVE, CONTEXT_DEBUG};
    
private:

    /* contains mappings between plugin names and theirs paths*/
    map<string, string> _pluginpathmap;

    /* contains the provided hookpoints from all plug-ins */
    map<string, IHookPoint*> _hookpoints;

    /* contains all loaded plug-ins */
    map<string, IPlugin*> _plugins;

    /* contains the config for Sentry */
    SentryConfig* _config;

    context_t _context;

    /*
     * Returns the names of the plug-ins in the given directory.
     */
    vector<string> _getPluginLibNames(string directory);

    /*
     * Sets up the hookpoints that Sentry provides
     */
    void _setupHookpoints();

    /*
     * Executes the commands contained in the given hookpoint.
     */
    void _executeCommandsIn(IHookPoint* hp);
    
public:
    enum exit_code_t {EXIT_NO_CONFIG = 1, EXIT_NO_PLUGIN_DIR};

    /*
     * Constructor
     * Locates the libraries and loads them. Sentry exits if the config file
     * cannot be found, or if the plug-in directory doesn't exist with the
     * following exit codes:
     * - Config file not found: EXIT_NO_CONFIG
     * - Plug-in directory not found: EXIT_NO_PLUGIN_DIR
     */
    Sentry();
    
    /* Destructor */
    ~Sentry() throw();

    /**
     * Sets the context for the current run of Sentry
     */
    void setContext(context_t context);

    /**
     * Returns the context Sentry currently runs in.
     */
    context_t getContext();

    /**
     * Loads the plugins
     */
    void loadPlugins();

    /*
     * Finds the hookpoint that has the given name
     */
    IHookPoint* findHookPoint(string name);

    /**
     * Finds the plugin that has the given name
     */
    IPlugin* findPlugin(string name);
};

#endif
