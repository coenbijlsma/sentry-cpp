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
#include "IPlugin.h"
#include "IPluginProvider.h"
#include "IHookPoint.h"
#include "IPluginCommand.h"

#include <vector>
#include <string>

using sentry::IPlugin;
using sentry::IPluginProvider;
using sentry::IHookPoint;
using sentry::IPluginCommand;

using std::string;
using std::vector;

class TestPlugin : public IPlugin {
private:
    string _name;
    IPluginProvider* _provider;
    vector<IHookPoint*> _hookpoints;
    vector<string> _dependencies;
    vector<IPluginCommand*> _commands;
        
public:

    TestPlugin(string name);
    virtual ~TestPlugin();

    void setProvider(IPluginProvider* provider);
    
    string getName();
    
    vector<IHookPoint*> getProvidingHookPoints();
    
    vector<string> getDependencies();
    
    vector<IPluginCommand*> getCommands();

    IPluginCommand* findCommand(string name);

    bool isActive();
    
};

//typedef IPlugin* create_plugin_t();
//typedef bool destroy_plugin_t(IPlugin* plugin);
