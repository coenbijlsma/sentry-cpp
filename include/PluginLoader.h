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

/**
 * @brief Loads plug-ins into Sentry.
 * @author $Author$
 * @copyright Copyright (C) 2008 by Coen Bijlsma
 * @since 2008-11-20
 * @changed $Date$
 * @version $Id$
 * @url $HeadURL$.
 * @see IPlugin.h
 *
 * This class is used by Sentry to load plug-ins from the given path, which
 * has to be the full path to the plug-in that has to be loaded.
 * If a plug-in cannot be loaded, an exception is thrown, or, if the loading
 * succeeds but some other error occured, the returned IPlugin* will be 0.
 * 
 */
class PluginLoader {
private:

    /**
     * @brief Loads the library in libpath.
     * @throws NoSuchLibraryException If the library doesn't exist.
     */
    static void* _getLib(string libpath) throw(NoSuchLibraryException);
    
public:

    /**
     * @brief Returns the loaded plug-in
     * @return IPlugin* The loaded plug-in.
     * @see IPlugin.h
     * @throws NoSuchLibraryException If the given path does not exist
     *	       or is no library.
     * @throws NoSuchSymbolException If the symbol create_plugin is not found.
     *
     * Tries to load the library (if any) at the given path. After it has
     * found and loaded the library, the pluginloader searches for the symbol
     * create_plugin, which has to return a IPlugin*. If you want an example
     * of how to program this, you can fetch the sources of Sentry and
     * check out some of the already provided plug-ins.
     */
    static IPlugin* loadPlugin(string libpath) throw(NoSuchLibraryException, NoSuchSymbolException);
    
    /**
     * @brief Unloads the provided plug-in.
     * @return bool Whether unloading the plug-in succeeded.
     *
     * You can use this function to unload your on plug-in, or whatever,
     * but Sentry doesn't dispose of bodies that way. Just so you know.
     * In fact I planned to use this in Sentry, but as it currently involves an
     * extra loading of the library currently, I decided to bluntly delete the
     * plug-ins when Sentry is destructed (at least for now).
     */
    static bool unloadPlugin(IPlugin* plugin, string libpath) throw(NoSuchLibraryException, NoSuchSymbolException);
};

#endif
