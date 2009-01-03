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
#ifndef MESSAGEDISPATCHERCOMMAND_H
#define	MESSAGEDISPATCHERCOMMAND_H

#include "IRCBaseAbstractCommand.h"
#include "IRCBase.h"
#include "SentryConfig.h"
#include <string>
#include <map>
#include <vector>

using sentry::SentryConfig;

using std::string;
using std::map;
using std::vector;

/**
 * @brief This class reads the raw incoming message, and dispatches it to the
 * correct hookpoint.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2009-01-03
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 */
class MessageDispatcherCommand : public IRCBaseAbstractCommand {
private:

    SentryConfig* _config;

public:
    MessageDispatcherCommand(IRCBase* plugin, SentryConfig* config);
    virtual ~MessageDispatcherCommand();

    /**
     * Does the actual dispatching work by executing the commands in the
     * hookpoints found. The commands executed receive the following parameters
     * in the order as specified below:
     * - user
     * - host
     * - nick
     * - other parameters
     */
    void execute(vector<string> params);
};

#endif	/* MESSAGEDISPATCHERCOMMAND_H */

