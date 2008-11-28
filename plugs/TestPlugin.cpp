#include "TestPlugin.h"

TestPlugin::TestPlugin(string name){
    _name = name;
}

TestPlugin::~TestPlugin(){
    _hookpoints.clear();
    _dependencies.clear();
    _commands.clear();
}

string TestPlugin::getName(){
    return _name;
}

vector<IHookPoint*> TestPlugin::getProvidingHookPoints(){
    return _hookpoints;
}

vector<string> TestPlugin::getDependencies(){
    return _dependencies;
}

vector<IPluginCommand*> TestPlugin::getCommands(){
    return _commands;
}

extern "C" IPlugin* create_plugin(){
    string name("testplugin");
    return (IPlugin*)new TestPlugin(name);
}

extern "C" bool destroy_plugin(IPlugin* plugin){
    delete plugin;
    return true;
}
