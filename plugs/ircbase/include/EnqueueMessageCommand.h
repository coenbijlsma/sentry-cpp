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

#ifndef ENQUEUEMESSAGECOMMAND_H
#define	ENQUEUEMESSAGECOMMAND_H

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

class EnqueueMessageCommand : public IPluginCommand {
private:

    IRCBase* _plugin;
    string _name;
    map<string, IHookPoint*> _hookpoints;
    
public:

    /**
     * Constructor.
     */
    EnqueueMessageCommand(IRCBase* plugin);
    virtual ~EnqueueMessageCommand();

    IPlugin* getPlugin();

    /**
     * Returns the name of this command (ircbase.enqueue_message)
     */
    string getName();

    map<string, IHookPoint*> getHookPoints();

    void addAttachedHookPoint(IHookPoint* hookpoint);

    void removeAttachedHookPoint(IHookPoint* hookpoint);

    /**
     * Executes the command, effectively adding
     * the messages in params to the queue of messages to be sent.
     * @param vector<string> Each item in the vector must contain
     * a RFC 1459 formatted message, otherwise sending it wil fail.
     */
    void execute(vector<string> params);
};

#endif	/* ENQUEUEMESSAGECOMMAND_H */

