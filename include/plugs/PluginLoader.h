#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "IPlugin.h"
#include "NoSuchLibraryException.h"
#include <string>

using std::string;

class PluginLoader {
private:

    /* The library the plug-in resides in */
    void* _lib;
    
    /* The actual plug-in */
    IPlugin* _plugin;
    
public:

    /**
     * Loads the plug-in from the library
     * @brief Constructor
     * @throws NoSuchLibraryException If the library is not found.
     * @throws NoSuchSymbolException If the library is found, but the 
     *	       symbol doesn't exist.
     */
    PluginLoader(string libpath) throw(NoSuchLibraryException, NoSuchSymbolException);
    
    /**
     * Also unloads the plug-in
     * @brief Destructor.
     */
    virtual ~PluginLoader();
    
    /**
     * @brief Returns the loaded
     * @return IPlugin* The loaded plug-in.
     * @see IPlugin.h
     */
    IPlugin* getPlugin() throw();
    
};

#endif
