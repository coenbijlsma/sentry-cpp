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
#ifndef NOSUCHLIBRARYEXCEPTION_H
#define NOSUCHLIBRARYEXCEPTION_H

#include <exception>

using std::exception;

/**
 * @author Coen Bijlsma
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @version 0.1
 * @since 2008-11-20
 * @brief Exception class in case a library doesn't exist.
 */
class NoSuchLibraryException : public exception {
public:
    
    virtual const char* what() const throw();
};

#endif
