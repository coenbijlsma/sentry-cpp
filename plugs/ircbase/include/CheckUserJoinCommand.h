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

#ifndef CHECKUSERJOINCOMMAND_H
#define	CHECKUSERJOINCOMMAND_H

#include "IPluginCommand.h"
#include "IHookPoint.h"
#include "IRCBase.h"
#include <string>
#include <map>
#include <vector>

using sentry::IPluginCommand;
using sentry::IHookPoint;

using std::string;
using std::map;
using std::vector;

class CheckUserJoinCommand : public IPluginCommand {
private:

    IRCBase* _plugin;
    string _name;
    map<string, IHookPoint*> _hookpoints;

    static string _JOIN_COMMAND;

public:

    CheckUserJoinCommand(IRCBase* plugin);
    virtual ~CheckUserJoinCommand();

    IPlugin* getPlugin();

    /**
     * Returns the name of this command (ircbase.enqueue_message)
     */
    string getName();

    map<string, IHookPoint*> getHookPoints();

    void addAttachedHookPoint(IHookPoint* hookpoint);

    void removeAttachedHookPoint(IHookPoint* hookpoint);

    /**
     * Executes the command, checking if the first param contains a JOIN
     * message. If so, the commands attached to the hookpoint post_join_user
     * are executed, with the following parameters (in the specified order):
     * - user
     * - host
     * - nick
     * - channel
     */
    void execute(vector<string> params);
    
};

#endif	/* CHECKUSERJOINCOMMAND_H */

