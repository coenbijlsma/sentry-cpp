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

#ifndef AUTOOPCOMMAND_H
#define	AUTOOPCOMMAND_H

#include "IPluginCommand.h"
#include "IHookPoint.h"
#include "IRCBase.h"
#include "SentryConfig.h"
#include <string>
#include <map>
#include <vector>

using sentry::IPluginCommand;
using sentry::IHookPoint;
using sentry::SentryConfig;

using std::string;
using std::map;
using std::vector;

class AutoOpCommand : public IPluginCommand {
private:
    string _name;
    IRCBase* _plugin;
    map<string, IHookPoint*> _hookpoints;
    SentryConfig* _config;
    
public:

    AutoOpCommand(IRCBase* plugin, SentryConfig* config);
    virtual ~AutoOpCommand();

    IPlugin* getPlugin();

    string getName();

    map<string, IHookPoint*> getHookPoints();

    void addAttachedHookPoint(IHookPoint* hookpoint);

    void removeAttachedHookPoint(IHookPoint* hookpoint);

    void execute(vector<string> params);
};

#endif	/* AUTOOPCOMMAND_H */

