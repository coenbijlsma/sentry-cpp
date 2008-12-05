#include "PluginLoader.h"
#include "Logger.h"
#include <dlfcn.h>
#include <iostream>

/*
using std::cerr;
using std::cout;
using std::endl;
*/

/* Retrieve the library from the given path */
void* PluginLoader::_getLib(string libpath) throw (NoSuchLibraryException){
    void* lib = dlopen(libpath.c_str(), RTLD_LAZY);
    
    if( ! lib ){
	throw NoSuchLibraryException();
    }
    
    return lib;
}

/* Loads the plug-in from the given path */
IPlugin* PluginLoader::loadPlugin(string libpath) throw (NoSuchLibraryException, NoSuchSymbolException){
    void* lib = _getLib(libpath);
    
    /* Load the create symbol */
    create_plugin_t* create_plugin = (create_plugin_t*) dlsym(lib, "create_plugin");
    const char* dlsym_error = dlerror();
    
    if(dlsym_error){
	dlclose(lib);
	
	// reset dlerror
	dlerror();
	
	throw NoSuchSymbolException();
    }else{
        Logger::log(string("Successfully loaded plugin " + libpath), Logger::LOG_INFO);
    }

    return (IPlugin*)create_plugin();
}

/* Destroys the given IPlugin, which has to reside in the library in the given path */
bool PluginLoader::unloadPlugin(IPlugin* plugin, string libpath) throw (NoSuchLibraryException, NoSuchSymbolException){
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
