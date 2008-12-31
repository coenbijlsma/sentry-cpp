#include "PluginLoader.h"
#include "Logger.h"
#include <dlfcn.h>
#include <iostream>

/* Retrieve the library from the given path */
void* sentry::PluginLoader::_getLib(string libpath) throw (NoSuchLibraryException){
    void* lib = dlopen(libpath.c_str(), RTLD_LAZY);
    
    if( ! lib ){
        NoSuchLibraryException ex;
        ex.setWhat(string("Could not load library " + libpath).c_str() ) ;
	throw ex;
    }
    
    return lib;
}

/* Loads the plug-in from the given path */
sentry::IPlugin* sentry::PluginLoader::loadPlugin(string libpath, IPluginProvider* provider) throw (NoSuchLibraryException, NoSuchSymbolException){
    void* lib = _getLib(libpath);

    
    /* Load the create symbol */
    create_plugin_t* create_plugin = (create_plugin_t*) dlsym(lib, "create_plugin");
    const char* dlsym_error = dlerror();
    
    if(dlsym_error){
        Logger::log(dlsym_error, Logger::LOG_INFO);
        NoSuchSymbolException ex;
        ex.setWhat(string(dlsym_error).c_str() );
        dlclose(lib);
        // reset dlerror
	dlerror();
	throw ex;
    }

    IPlugin* plugin = (IPlugin*)create_plugin();    
    if(plugin){
        if(provider == 0){
            Logger::log("PluginProvider is NULL, unloading plugin", Logger::LOG_ERROR);
            delete plugin;
            dlclose(lib);
            return (IPlugin*)0;
        }else{
            plugin->setProvider(provider);
            Logger::log(string("Successfully loaded plugin " + libpath), Logger::LOG_INFO);
        }
    }
    return plugin;
}

/* Destroys the given IPlugin, which has to reside in the library in the given path */
bool sentry::PluginLoader::unloadPlugin(IPlugin* plugin, string libpath) throw (NoSuchLibraryException, NoSuchSymbolException){
    void* lib = _getLib(libpath);
    
    destroy_plugin_t* destroy_plugin = (destroy_plugin_t*) dlsym(lib, "destroy_plugin");
    const char* dlsym_error = dlerror();
    
    if(dlsym_error){
	// reset dlerror()
	dlerror();
	
	/* If the destroy method cannot be loaded, delete the plug-in */
	delete plugin;
	
	throw NoSuchSymbolException();
    }else{
	destroy_plugin( plugin );
    }
    
    if(dlclose(lib) != 0){
        Logger::log(string("Could not unload plugin " + libpath), Logger::LOG_WARNING);
    }else{
        Logger::log(string("Successfully unloaded plugin " + libpath), Logger::LOG_INFO);
    }
}
