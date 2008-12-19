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
    const int _bufsize = 513;
    
    int _fd;
    bool _blocking;
    vector<char> _buffer;
    
    void _fillBuffer();
    
public:

    BufferedSocketReader(int fd, bool blocking = true);
    ~BufferedSocketReader();

    char read();

    string readLine();

};

#endif	/* BUFFEREDSOCKETREADER_H */

