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
