#include <map>

#include "SentryConfig.h"
#include "StringTokenizer.h"
#include "Logger.h"

using std::endl;
using std::ios;
using std::ifstream;

SentryConfig::SentryConfig(string filename) throw(string){
    _filename = filename;

    ifstream _ifstream(_filename.c_str(), ios::in);

    if( ! _ifstream.is_open()){
        Logger::log("Config not found!", Logger::LOG_FATAL);
        throw string("Config file not found (was looking for sentry.conf)");
    }

    _ifstream.close();
    _init();
    _filename = filename;

}

SentryConfig::~SentryConfig() throw(){
    writeConfig();

    while( ! _sections.empty() ){
        SentryConfigSection* section = (_sections.begin())->second;
        _sections.erase(_sections.begin());
        delete section;
    }
}

void SentryConfig::_init() throw(string) {
    ifstream _ifstream(_filename.c_str(), ios::in);
    
    if( ! _ifstream.is_open()){
        _ifstream.close();
        string error("Could not open config file " + _filename);
        Logger::log(error, Logger::LOG_ERROR);
        throw error;
    }

    string line;
    SentryConfigSection* section = 0;
    bool inSection = false;

    while(getline(_ifstream, line)){
        
        // ignore empty lines and comment
        if( ! line.empty() && line.at(0) != '#'){

            // create a new section if one is found
            if( ! inSection ){
                section = new SentryConfigSection(line);
                inSection = true;
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

        } // !line.empty() && line.at(0) != '#'

    } // while(getline())

    _ifstream.close();
    
}

/* Writes one section to the given stream */
void SentryConfig::_writeSection(ofstream* fs, SentryConfigSection* section) throw(string) {
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

    (*fs) << endl;
    (*fs) << section->getName() << endl;
    (*fs) << "{" << endl;

    for(it = entries.begin(); it != entries.end(); it++){
        (*fs) << it->first << " " << it->second << endl;
    }

    (*fs) << "}" << endl;
    (*fs) << endl;
}

/* Returns the section that has the given name */
SentryConfigSection* SentryConfig::getSection(string name) throw() {
    map<string, SentryConfigSection*>::iterator it = _sections.find(name);

    if(it == _sections.end()){
        return (SentryConfigSection*)0;
    }
    return it->second;
}

string SentryConfig::getValue(string section, string setting) throw() {
    SentryConfigSection* sec = getSection(section);

    if(sec == 0){
        return string("");
    }

    return sec->get(setting);
}

void SentryConfig::setSetting(string section, string setting, string value) throw(string) {
    SentryConfigSection* sec = getSection(section);

    if(sec == 0){
        string error("Writing non existant section " + section);
        throw error;
    }

    sec->set(setting, value);
}

bool SentryConfig::writeConfig() throw() {
    ofstream _ofstream(_filename.c_str(), ios::in);

    if(_ofstream.is_open()){

        // file exists, so empty it before writing
        _ofstream.open(_filename.c_str(), ios::in | ios::out | ios::app | ios::trunc);
    }else{

        // file doesn't exist, so also create it here
        _ofstream.open(_filename.c_str(), ios::in | ios::out | ios::app);
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

    _ofstream.close();
    return true;
}
