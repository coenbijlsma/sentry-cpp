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

#include <limits.h> /* For realpath */
#include <stdlib.h> /* For realpath */

using std::endl;
using std::ios_base;
using std::ifstream;
using std::vector;

vector<string> sentry::SentryConfig::_loadingConfigs;
string sentry::SentryConfig::_INCLUDE_DIRECTIVE("!include");

sentry::SentryConfig::SentryConfig(string filename) throw(string){
    this->_loadAndInit(filename);
}

sentry::SentryConfig::~SentryConfig() throw(){
    this->_writeAndEmpty();
}

bool sentry::SentryConfig::_writeAndEmpty() throw() {
    bool success = writeConfig();

    while( ! _sections.empty() ){
        SentryConfigSection* section = (_sections.begin())->second;
        _sections.erase(_sections.begin());
        delete section;
    }

    while( ! _includedConfigs.empty() ){
        SentryConfig* config = *(_includedConfigs.begin());
        _includedConfigs.erase(_includedConfigs.begin());
        delete config;
    }

    return success;
}

void sentry::SentryConfig::_loadAndInit(string filename) throw(string) {
    char* rpath = realpath(filename.c_str(), NULL);
    if(rpath){
        string path(rpath);
        free(rpath);

        if(SentryConfig::isLoading(path)){
            throw string("Configfile " + path + " already loading (included in " + filename + " ), possible recursion?");
        }else{
            SentryConfig::_loadingConfigs.push_back(path);
        }
        _filename = path;
        _originalFilename = filename;

        ifstream _ifstream(path.c_str(), ios_base::in);

        if( ! _ifstream.is_open()){
            throw string("Config file not found (was looking for " + path + ")");
        }

        _ifstream.close();
        _init();
        _filename = path;

        SentryConfig::_removeLoadingConfig(path);
    }else{
        throw string("Error while looking up the realpath for " + filename);
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
    SentryConfig* included = 0;
    SentryConfigSection* section = 0;
    bool inSection = false;
    vector<string> tempComment;

    while(getline(_ifstream, line)){
        
        // ignore empty lines and comment
        if( ! line.empty() && line.at(0) != '#'){

            // create a new section if one is found
            if( ! inSection ){

                // Check if maybe it's an include
                StringTokenizer st(line, ' ');
                if(st.count() == 2){
                    string first = st.next();
                    string second = st.next();

                    if(first == SentryConfig::_INCLUDE_DIRECTIVE){
                        included = new SentryConfig(second);
                        _includedConfigs.push_back( included );
                    }
                    
                }else{
                    section = new SentryConfigSection(line);
                    inSection = true;

                    // add the comments to the section, if any
                    if(tempComment.size() > 0){
                        section->addAllComment(tempComment);
                        tempComment.clear();
                    }
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

string sentry::SentryConfig::getFileName() throw(){
    return _filename;
}

string sentry::SentryConfig::getOriginalFileName() throw() {
    return _originalFilename;
}

/* Returns the section that has the given name */
sentry::SentryConfigSection* sentry::SentryConfig::getSection(string name) throw() {
    for(unsigned int i = 0; i < _includedConfigs.size(); i++){
        SentryConfig* config = _includedConfigs.at(i);
        SentryConfigSection* section = config->getSection(name);
        if(section){
            return section;
        }
    }
    
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

    // write the includes
    for(unsigned int i = 0; i < _includedConfigs.size(); i++){
        SentryConfig* config = _includedConfigs.at(i);
        _ofstream << SentryConfig::_INCLUDE_DIRECTIVE << " " << config->getOriginalFileName() << endl;
    }

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

void sentry::SentryConfig::reload() throw(ConfigReloadException) {
    if( ! this->_writeAndEmpty() ){
        throw ConfigReloadException("Could not write configfile " + this->_filename + ", reloading failed.");
    }
    try{
        this->_loadAndInit(this->_originalFilename);
    }catch(string error){
        throw ConfigReloadException(error);
    }
}

bool sentry::SentryConfig::isLoading(string configfile) throw() {
    for(vector<string>::iterator it = SentryConfig::_loadingConfigs.begin(); 
        it != SentryConfig::_loadingConfigs.end(); it++ ){

            if( *(it) == configfile ){
                return true;
            }
    }
    return false;
}

void sentry::SentryConfig::_removeLoadingConfig(string filename) throw() {
    for(unsigned int i = 0; i < SentryConfig::_loadingConfigs.size(); i++){

        string current = SentryConfig::_loadingConfigs.at(i);
        if( current ==  filename ){
            SentryConfig::_loadingConfigs.erase(SentryConfig::_loadingConfigs.begin() + i);
        }
    }
}
