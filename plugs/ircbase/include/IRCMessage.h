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

#ifndef IRCMESSAGE_H
#define	IRCMESSAGE_H

#include <string>
#include <vector>
#include "IRCMessagePrefix.h"
#include "GenericIRCBaseException.h"

using std::string;
using std::vector;

/**
 * @brief This class represents a message following the RFC 1459 format
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-21
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 * @url http://www.irchelp.org/irchelp/rfc/rfc.html
 *
 * Ther following text is taken from the RFC (chapter 2.3.1) to show the
 * pseudo-format of an IRC message.
 *
 * The BNF representation for this is:
 * <message> ::=
 *     [':' <prefix> <SPACE> ] <command> <params> <crlf>
 * <prefix> ::=
 *    <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
 * <command> ::=
 *     <letter> { <letter> } | <number> <number> <number>
 * <SPACE> ::=
 *     ' ' { ' ' }
 * <params> ::=
 *     <SPACE> [ ':' <trailing> | <middle> <params> ]
 * <middle> ::=
 *     <Any *non-empty* sequence of octets not including SPACE or NUL or CR
 *     or LF, the first of which may not be ':'>
 * <trailing> ::=
 *     <Any, possibly *empty*, sequence of octets not including NUL or CR or LF>
 * <crlf> ::=
 *    CR LF
 *
 * NOTES:
 *
 *   1. <SPACE> is consists only of SPACE character(s) (0x20). Specially notice
 *      that TABULATION, and all other control characters are considered
 *      NON-WHITE-SPACE.
 *   2. After extracting the parameter list, all parameters are equal, whether
 *      matched by <middle> or <trailing>. <Trailing> is just a syntactic trick
 *      to allow SPACE within parameter.
 *   3. The fact that CR and LF cannot appear in parameter strings is just
 *      artifact of the message framing. This might change later.
 *   4. The NUL character is not special in message framing, and basically
 *      could end up inside a parameter, but as it would cause extra
 *      complexities in normal C string handling. Therefore NUL is not allowed
 *      within messages.
 *   5. The last parameter may be an empty string.
 *   6. Use of the extended prefix (['!' <user> ] ['@' <host> ]) must not be
 *      used in server to server communications and is only intended for server
 *      to client messages in order to provide clients with more useful
 *      information about who a message is from without the need for additional
 *      queries.
 *
 */
class IRCMessage {
private:
    IRCMessagePrefix* _prefix;
    string _command;
    vector<string> _params;

    /**
     * Called if the non-standard constructor is used.
     */
    void _init(string raw) throw(GenericIRCBaseException);
public:

    static string MESSAGE_SEPARATOR;

    /**
     * Constructs an empty message
     */
    IRCMessage() throw();

    /**
     * Constructs a message from the given raw string
     */
    IRCMessage(string raw) throw(GenericIRCBaseException);

    /**
     * Destructor
     */
    virtual ~IRCMessage() throw();

    /**
     * Sets the prefix
     * @param IRCMessagePrefix* prefix The prefix to be set
     */
    void setPrefix(IRCMessagePrefix* prefix) throw();

    /**
     * Sets the command of the message.
     * @param string command The command.
     */
    void setCommand(string command) throw(); // maybe exception if command is faulty

    /**
     * Adds a parameter to the parameter list.
     * @param string param The parameter to be added.
     */
    void addParam(string param) throw();

    /**
     * Adds the items in the given vector to the list of parameters.
     * @param vector<string> params The parameters to be added
     */
    void addAllParams(vector<string> params) throw();

    /**
     * Removes the given parameter from the list, if it exists.
     * If the string occurs more than once, only the first occurrence
     * is deleted.
     * @param string param The to be removed parameter.
     */
    void removeParam(string param) throw();

    /**
     * Returns the prefix of the message, if any. If there is no prefix,
     * (IRCMessagePrefix*)0 is returned.
     * @return IRCMessagePrefix*
     */
    IRCMessagePrefix* getPrefix() throw();

    /**
     * Returns the command of this message.
     * @return string The command.
     */
    string getCommand() throw();

    /**
     * Returns the parameters of the command.
     * @return vector<string> The parameters.
     */
    vector<string> getParams() throw();

    string getParamsAsString() throw();

    /**
     * Returns the message formatted to the RFC's demands
     * @return string The formatted message.
     */
    string toRFCFormat() throw();

};

#endif	/* IRCMESSAGE_H */

