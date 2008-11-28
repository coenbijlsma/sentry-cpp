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
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "IPlugin.h"
#include "NoSuchLibraryException.h"
#include "NoSuchSymbolException.h"
#include <string>
#include <map>

using std::string;
using std::map;

class PluginLoader {
private:
    /**
     * @brief loads the library in libpath.
     */
    static void* _getLib(string libpath) throw(NoSuchLibraryException);
    
public:

    /**
     * @brief Returns the loaded plug-in
     * @return IPlugin* The loaded plug-in.
     * @see IPlugin.h
     */
    static IPlugin* loadPlugin(string libpath) throw(NoSuchLibraryException, NoSuchSymbolException);
    
    /**
     * @brief Unloads the provided plug-in.
     * @return bool Whether unloading the plug-in succeeded.
     */
    static bool unloadPlugin(IPlugin* plugin, string libpath) throw(NoSuchLibraryException, NoSuchSymbolException);
};

#endif
