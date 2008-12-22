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

#ifndef IRCMESSAGEPREFIX_H
#define	IRCMESSAGEPREFIX_H

#include <string>
#include "GenericIRCBaseException.h"

using std::string;

/**
 * @brief This class represents a message prefix for an RFC 1459 formatted message.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-21
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 * @url http://www.irchelp.org/irchelp/rfc/rfc.html
 *
 * Ther following text is taken from the RFC (chapter 2.3.1) to show the
 * pseudo-format of an IRC message prefix.
 *
 * <prefix> ::=
 *    <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
 */
class IRCMessagePrefix {
private:
    string _serverOrNick;
    string _user;
    string _host;
    
    /**
     * Called if the non-empty constructor is used.
     */
    void _init(string raw) throw(GenericIRCBaseException);
    
public:

    static char PREFIX_START;
    static char PREFIX_UID;
    static char PREFIX_HID;

    /**
     * Constructs an empty prefix
     */
    IRCMessagePrefix() throw();

    /**
     * Constructs a prefix from the given raw string
     */
    IRCMessagePrefix(string raw) throw(GenericIRCBaseException);

    /**
     * Destructor
     */
    virtual ~IRCMessagePrefix() throw();

    void setServerOrNick(string son) throw();

    void setUser(string user) throw();

    void setHost(string host) throw();

    string getServerOrNick() throw();

    string getUser() throw();

    string getHost() throw();

    /**
     * Returns the prefix so that it will comply to the RFC
     */
    string toRFCFormat() throw();

};

#endif	/* IRCMESSAGEPREFIX_H */

