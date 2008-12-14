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


#include "TestPlugin.h"

TestPlugin::TestPlugin(string name){
    _name = name;
}

TestPlugin::~TestPlugin(){
    _hookpoints.clear();
    _dependencies.clear();
    _commands.clear();
}

void TestPlugin::setProvider(IPluginProvider* provider){
    _provider = provider;
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

IPluginCommand* TestPlugin::findCommand(string name){
    for(vector<IPluginCommand*>::iterator it = _commands.begin(); it != _commands.end(); it++){
        IPluginCommand* cmd = *it;
        if(cmd->getName() == name){
            return cmd;
        }
    }
    return (IPluginCommand*)0;
}

extern "C" IPlugin* create_plugin(){
    string name("testplugin");
    return (IPlugin*)new TestPlugin(name);
}

extern "C" bool destroy_plugin(IPlugin* plugin){
    delete plugin;
    return true;
}
