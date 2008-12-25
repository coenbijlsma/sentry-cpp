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

#ifndef BUFFEREDSOCKETREADER_H
#define	BUFFEREDSOCKETREADER_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-12-19
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$
 *
 * This class privides buffered reading on a file descriptor, much like the
 * Java API.
 */ 
class BufferedSocketReader {
private:
    const int _bufsize;
    
    int _fd;
    bool _blocking;
    vector<char> _buffer;
    bool _shutdownFlag;
    
    void _fillBuffer();
    
public:

    /**
     * Constructor.
     * @param int fd The file descriptor of the socket.
     * @param bool blocking Whether or not the reading should
     * @param bufsize int The maximum size of the buffer
     * block or not. This is nog possible if you use
     * BufferedSocketReader::readLine()
     */
    BufferedSocketReader(int fd, bool blocking = true, int bufsize = 513);

    /**
     * Destructor.
     */
    ~BufferedSocketReader();

    /**
     * Reads the next byte from the socket.
     * If there is nothing to be read or the connection is closed,
     * this function returns -1.
     *@return int The next char from the stream
     */
    int read();


    /**
     * Reads a string from the input stream, until delim is encountered.
     * Note that this call blocks until delim is found.
     * @return string The string.
     */
    string read(string delim, bool includeDelim = false);
    
    /**
     * Reads a line from the input stream and returns it, discarding the
     * newline character.. Note that this call blocks until a newlihe
     * character (\n) is encountered.
     *@return string The line.
     */
    string readLine();

    /**
     * Returns whether the peer has performed an orderly shutdown
     * (I.E.recv() returned 0).
     * @return bool Whether the peer has shutdown the connection.
     */
    bool peerHasShutdown();

};

#endif	/* BUFFEREDSOCKETREADER_H */

