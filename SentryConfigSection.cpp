#include "SentryConfigSection.h"
#include <iostream>

SentryConfigSection::SentryConfigSection(string name){
    _name = name;
}

SentryConfigSection::~SentryConfigSection(){
    _entries.clear();
}

string SentryConfigSection::getName(){
    return _name;
}

string SentryConfigSection::get(string setting){
    map<string, string>::iterator it = _entries.find(setting);

    if(it == _entries.end()){
        return string("");
    }
    return _entries[setting];
}

void SentryConfigSection::set(string setting, string value){
    _entries[setting] = value;
}

map<string, string> SentryConfigSection::all(){
    return _entries;
}
