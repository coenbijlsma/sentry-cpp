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

#include "IRCBase.h"
#include "IRCBaseAbstractCommand.h"
#include <string>
#include <vector>

using sentry::IPluginCommand;
using sentry::IHookPoint;

using std::string;
using std::vector;

class EnqueueMessageCommand : public IRCBaseAbstractCommand {
public:

    /**
     * Constructor.
     */
    EnqueueMessageCommand(IRCBase* plugin);
    virtual ~EnqueueMessageCommand();

    /**
     * Executes the command, effectively adding
     * the messages in params to the queue of messages to be sent.
     * @param vector<string> Each item in the vector must contain
     * a RFC 1459 formatted message, otherwise sending it wil fail.
     */
    void execute(vector<string> params);
};

#endif	/* ENQUEUEMESSAGECOMMAND_H */

