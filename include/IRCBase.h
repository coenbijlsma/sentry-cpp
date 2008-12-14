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
#include "IPlugin.h"
#include "IHookPoint.h"
#include "IPluginCommand.h"

using std::string;
using std::vector;

/**
 * @brief The IRC base plug-in
 * @author $Author $
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-14
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 */
class IRCBase : public IPlugin {
private:

    IPluginProvider* _provider;
    string _name;
    vector<IHookPoint*> _providingHookPoints;
    vector<string> _dependencies;
    vector<IPluginCommand*> _commands;
    
public:

    IRCBase(string name);
    virtual ~IRCBase();

    void setProvider(IPluginProvider* provider);
    
    string getName();

    vector<IHookPoint*> getProvidingHookPoints();

    vector<string> getDependencies();

    vector<IPluginCommand*> getCommands();

    IPluginCommand* findCommand(string name);
};

#endif	/* IRCBASE_H */
