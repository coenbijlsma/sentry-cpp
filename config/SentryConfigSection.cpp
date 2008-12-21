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

#include "SentryConfigSection.h"
#include <iostream>

sentry::SentryConfigSection::SentryConfigSection(string name){
    _name = name;
}

sentry::SentryConfigSection::~SentryConfigSection(){
    _entries.clear();
}

string sentry::SentryConfigSection::getName(){
    return _name;
}

vector<string> sentry::SentryConfigSection::getComment(){
    return _comment;
}

void sentry::SentryConfigSection::addComment(string comment){
    if(comment.at(0) != '#'){
        string prefix("#");
        prefix += comment;
        comment = prefix;
    }

    _comment.push_back(comment);
}

void sentry::SentryConfigSection::addAllComment(vector<string> comment){
    for(vector<string>::iterator it = comment.begin(); it != comment.end(); it++){
        addComment(*it);
    }
}

string sentry::SentryConfigSection::get(string setting){
    map<string, string>::iterator it = _entries.find(setting);

    if(it == _entries.end()){
        return string("");
    }
    return _entries[setting];
}

void sentry::SentryConfigSection::set(string setting, string value){
    _entries[setting] = value;
}

map<string, string> sentry::SentryConfigSection::all(){
    return _entries;
}
