#include "PluginLoader.h" 
#include "Logger.h"
#include <dlfcn.h>

PluginLoader::PluginLoader(string libpath){
    Logger::log("loading library" + libpath + " from PluginLoader::PluginLoader");
    _lib = dlopen(libpath.c_str(), RTLD_LAZY);
    const char* dlsym_error = 0;
    
    if( ! _lib){
	Logger::log( string(dlerror()) + libpath );
	
	// reset dlerror()
	dlerror();
	throw NoSuchLibraryException;
    }
    
    /* Load the create symbol */
    create_t* create_plugin = (create_t*) dlsym(_lib, "create_plugin");
    dlsym_error = dlerror();
    
    if(dlsym_error){
	Logger::log( string(dlsym_error) + libpath );
	dlclose(_lib);
	
	// reset dlerror
	dlerror();
	
	throw NoSuchSymbolException;
    }

    _plugin = (IPlugin*)create_plugin();    
    Logger::log("Successfully loaded library " + libpath );
}

PluginLoader::~PluginLoader(){
    destroy_t* destroy_plugin = (destroy_t*) dlsym(_lib, "destroy_plugin");
    const char* dlsym_error = dlerror();
    
    if(dlsym_error){
	Logger::log( string(dlsym_error) );
	
	// reset dlerror()
	dlerror();
	
	/* If the destroy method cannot be loaded, delete the plug-in */
	delete _plugin;
    }else{
	destroy_plugin(_plugin);
    }
    
    if(dlclose(_lib) != 0){
	Logger::log("Could not unload plugin library");
    }else{
	Logger::log("Successfully unloaded plugin library");
    }
}

IPlugin* PluginLoader::getPlugin() {
    return _plugin;
}
