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

#include "SentryConfig.h"
#include "StringTokenizer.h"
#include "Logger.h"
#include <vector>

using std::endl;
using std::ios_base;
using std::ifstream;
using std::vector;

sentry::SentryConfig::SentryConfig(string filename) throw(string){
    _filename = filename;

    ifstream _ifstream(_filename.c_str(), ios_base::in);

    if( ! _ifstream.is_open()){
        Logger::log("Config not found!", Logger::LOG_FATAL);
        throw string("Config file not found (was looking for " + filename + ")");
    }

    _ifstream.close();
    _init();
    _filename = filename;

}

sentry::SentryConfig::~SentryConfig() throw(){
    writeConfig();

    while( ! _sections.empty() ){
        SentryConfigSection* section = (_sections.begin())->second;
        _sections.erase(_sections.begin());
        delete section;
    }
}

void sentry::SentryConfig::_init() throw(string) {
    ifstream _ifstream(_filename.c_str(), ios_base::in);
    
    if( ! _ifstream.is_open()){
        _ifstream.close();
        string error("Could not open config file " + _filename);
        Logger::log(error, Logger::LOG_ERROR);
        throw error;
    }

    string line;
    SentryConfigSection* section = 0;
    bool inSection = false;
    vector<string> tempComment;

    while(getline(_ifstream, line)){
        
        // ignore empty lines and comment
        if( ! line.empty() && line.at(0) != '#'){

            // create a new section if one is found
            if( ! inSection ){
                section = new SentryConfigSection(line);
                inSection = true;

                // add the comments to the section, if any
                if(tempComment.size() > 0){
                    section->addAllComment(tempComment);
                    tempComment.clear();
                }
            }else{
                if( line.at(0) != '{' && line.at(0) != '}'){
                    StringTokenizer st(line, ' ');

                    // Only read the entry if if has 2 tokens
                    if(st.count() == 2){
                        string setting(st.next());
                        string value(st.next());
                        section->set(setting, value);
                    }else if(st.count() > 2){
                        string setting(st.next());
                        string value(st.next());

                        while(st.hasNext()){
                            value.append(" ");
                            value.append(st.next());
                        }

                        section->set(setting, value);
                    } // st.count() > 2
                }else{

                    // save the section and leave if end marker is found
                    if(line.at(0) == '}'){
                        _sections[section->getName()] = section;
                        inSection = false;
                    }
                } // line.at(0) != '{' && line.at(0) != '}'

            } // inSection

        }else{
            // add the comment to the vector if it's comment
            if(!line.empty() && line.at(0) == '#'){
                tempComment.push_back(line);
            }

        } // !line.empty() && line.at(0) != '#'

    } // while(getline())

    _ifstream.close();
    
}

/* Writes one section to the given stream */
void sentry::SentryConfig::_writeSection(ofstream* fs, SentryConfigSection* section) throw(string) {
    if( !fs->is_open() ){
        string error("Could not access file " + _filename);
        throw error;
    }
    if(section == 0){
        string error("Non existant config section: " + section->getName());
        throw error;
    }

    map<string, string> entries = section->all();
    map<string, string>::iterator it;
    vector<string> comment = section->getComment();
    vector<string>::iterator ci;

    (*fs) << endl;
    for(ci = comment.begin(); ci != comment.end(); ci++){
        (*fs) << *ci << endl;
    }

    (*fs) << section->getName() << endl;
    (*fs) << "{" << endl;

    for(it = entries.begin(); it != entries.end(); it++){
        (*fs) << it->first << " " << it->second << endl;
    }

    (*fs) << "}" << endl;
    (*fs) << endl;
}

/* Returns the section that has the given name */
sentry::SentryConfigSection* sentry::SentryConfig::getSection(string name) throw() {
    map<string, SentryConfigSection*>::iterator it = _sections.find(name);

    if(it == _sections.end()){
        return (SentryConfigSection*)0;
    }
    return it->second;
}

string sentry::SentryConfig::getValue(string section, string setting) throw() {
    SentryConfigSection* sec = getSection(section);

    if(sec == 0){
        return string("");
    }

    return sec->get(setting);
}

void sentry::SentryConfig::setSetting(string section, string setting, string value) throw(string) {
    SentryConfigSection* sec = getSection(section);

    if(sec == 0){
        string error("Writing non existant section " + section);
        throw error;
    }

    sec->set(setting, value);
}

bool sentry::SentryConfig::writeConfig() throw() {
    ofstream _ofstream(_filename.c_str());

    if(_ofstream.is_open()){
        // file exists, so empty it before writing
        _ofstream.open(_filename.c_str(), ios_base::app | ios_base::trunc);
        _ofstream.flush();
        _ofstream.close();
    }

    _ofstream.open(_filename.c_str(), ios_base::app);

    map<string, SentryConfigSection*>::iterator it;

    // write all the sections
    for(it = _sections.begin(); it != _sections.end(); it++){
        try{
            _writeSection(&_ofstream, it->second);
        }catch(string error){
            Logger::log(error, Logger::LOG_WARNING);
        }
    }

    _ofstream.flush();
    _ofstream.close();
    return true;
}
