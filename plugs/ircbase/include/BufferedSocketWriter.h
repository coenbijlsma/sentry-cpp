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

#ifndef BUFFEREDSOCKETWRITER_H
#define	BUFFEREDSOCKETWRITER_H

#include <string>
#include <vector>
#include <poll.h>

using std::string;
using std::vector;

/**
 * @brief Class for buffered writing to a socket
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-19
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 */
class BufferedSocketWriter {
private:
    const unsigned int _maxStringSize;
    const int _pollTimeout;

    struct pollfd _pfd[1];
    int _fd;
    vector<string> _buffer;

    bool _write();
    
public:

    /**
     * Constructor.
     * @param int fd The file descriptor of the socket to write to.
     * @param int bufsize The maximum buffer size.
     */
    BufferedSocketWriter(int fd, unsigned int maxStringSize = 512, int pollTimeout = 500);

    /**
     * Destructor.
     */
    ~BufferedSocketWriter();

    /**
     * Writes one single character.
     * @param int c The character to be written.
     * @return bool Whether writing succeeded.
     */
    bool write(int c);

    /**
     * Writes the given string to the buffer.
     * @param string s The string to be written.
     * @return bool Whether writing the string succeeded.
     */
    bool write(string s);

    /**
     * Forces the buffer to write it's current contents to the socket.
     * @return bool Whether the flushing succeeded or not.
     */
    bool flush();
};

#endif	/* BUFFEREDSOCKETWRITER_H */

