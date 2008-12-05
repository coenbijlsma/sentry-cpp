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
#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

/**
 * @brief The core class of Sentry
 * @author Coen Bijlsma
 * @copyright Copyright (c) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @version 0.1
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
 * At the moment, Sentry provides two hookpoints:
 * - core.post_startup
 * - core.pre_shutdown
 * The loaded plug-in commands are then respectively executed when Sentry has
 * started, and before Sentry shuts down.
 * If you create your own plug-in, you can define your own hookpoints. For
 * more information about this, see IPlugin.h
 *
 */
class Sentry {
private:

    /* contains mappings between plugin names and theirs paths*/
    map<string, string> _pluginpathmap;

    /* contains the provided hookpoints from all plug-ins */
    map<string, IHookPoint*> _hookpoints;

    /* contains all loaded plug-ins */
    map<string, IPlugin*> _plugins;

    /* contains the config for Sentry */
    SentryConfig* _config;

    /*
     * Returns the names of the plug-ins in the given directory.
     */
    vector<string> _getPluginLibNames(string directory);

    /*
     * Finds the hookpoint that has the given name
     */
    IHookPoint* _findHookPoint(string name);

    /*
     * Sets up the hookpoints that Sentry provides
     */
    void _setupHookpoints();

    /*
     * Executes the commands contained in the given hookpoint.
     */
    void _executeCommandsIn(IHookPoint* hp);
    
public:

    /* Constructor */
    Sentry();
    
    /* Destructor */
    ~Sentry();
    
};

#endif
