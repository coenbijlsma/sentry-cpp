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
#ifndef SENTRY_H
#define SENTRY_H

#include "IHookPoint.h"
#include "IPlugin.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

class Sentry {
private:
    map<string, string> _pluginpathmap;
    map<string, IHookPoint*> _hookpoints;
    map<string, IPlugin*> _plugins;
    
    vector<string> _getPluginLibNames(string directory);
    void _setupHookpoints();
    
public:

    /* Constructor */
    Sentry();
    
    /* Destructor */
    ~Sentry();
    
};

#endif
