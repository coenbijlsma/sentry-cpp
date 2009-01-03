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

#include "ConvertException.h"

sentry::ConvertException::ConvertException(){

}

sentry::ConvertException::ConvertException(string what){
    _what = what;
}

sentry::ConvertException::~ConvertException(){

}

string sentry::ConvertException::what() throw() {
    if(_what.empty()){
        return "Could not convert value";
    }
    return _what;
}

void sentry::ConvertException::setWhat(string what) throw() {
    _what = what;
}
