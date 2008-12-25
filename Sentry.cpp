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

#include "Sentry.h"
#include "PluginLoader.h"
#include "Logger.h"
#include "SentryHookPoint.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib> /* For exit(int) */
#include <dirent.h> /* For reading the plug-in directory */
#include <string.h> /* For strcmp */
#include <unistd.h> /* For usleep */

using std::vector;

/* Construcor */
sentry::Sentry::Sentry(){
    /* Load the config */
    try{
        _config = new SentryConfig("./sentry.conf");
        Logger::setDestination(Logger::DEST_STDOUT);
        
        // and setup logging
        string logdestination = _config->getValue("log", "destination");
        if(logdestination == "file"){
            Logger::setDestination(Logger::DEST_FILE);
            Logger::setlogFile(_config->getValue("log", "filename"));
        }else if(logdestination == "database"){
            Logger::setDestination(Logger::DEST_DATABASE);
        }
    }catch(string error){
        Logger::log(error, Logger::LOG_FATAL);
        exit(Sentry::EXIT_NO_CONFIG);
    }
    
    /* Load the hookpoints that Sentry itself provides */
    _setupHookpoints();
    
    /* Execute the post-startup actions */
    IHookPoint* post_startup = findHookPoint("core.post_startup");
    
    if( ! post_startup ){
        Logger::log("Could not find hookpoint core.post_startup", Logger::LOG_ERROR);
    }else{
        _executeCommandsIn(post_startup);
    }
}

/* Destructor */
sentry::Sentry::~Sentry() throw(){

    /* Execute the pre-shutdown actions */
    IHookPoint* pre_shutdown = findHookPoint("core.pre_shutdown");
    if( ! pre_shutdown ){
        Logger::log("Could not find hookpoint core.pre_shutdown", Logger::LOG_ERROR);
    }else{
        _executeCommandsIn(pre_shutdown);
        delete pre_shutdown;
    }

    /* Unload the plug-ins */
    for(map<string, IPlugin*>::iterator it = _plugins.begin(); it != _plugins.end(); it++){
	IPlugin* plugin = it->second;
        Logger::log("Unloading " + plugin->getName(), Logger::LOG_INFO);
	delete plugin;
    }

    delete _config;
        
    IHookPoint* post_startup = findHookPoint("core.post_startup");
    if(post_startup != 0){
        delete post_startup;
    }

}

void sentry::Sentry::setContext(context_t context){
    switch(context){
        case Sentry::CONTEXT_DEBUG:
            Logger::log("Context is DEBUG", Logger::LOG_INFO); break;
        case Sentry::CONTEXT_LIVE:
        default:
            Logger::log("Context is LIVE", Logger::LOG_INFO); break;
    }
    _context = context;
}

sentry::Sentry::context_t sentry::Sentry::getContext(){
    return _context;
}

void sentry::Sentry::loadPlugins(){
    string plugindir(_config->getValue("application", "plugindir") + "/");
    if(plugindir == ""){
        Logger::log("Plugin directory not in config.", Logger::LOG_FATAL);
        exit(Sentry::EXIT_NO_PLUGIN_DIR);
    }

    vector<string> files = _getPluginLibNames(plugindir);

    /**
     * Load the plug-ins
     */
    for(int i = 0; i < files.size(); i++){
	try{
	    string pluginfile = files.at(i);
	    IPlugin* plugin = PluginLoader::loadPlugin(pluginfile, this);

	    if(plugin != 0){
		_plugins[plugin->getName()] = plugin;
		_pluginpathmap[plugin->getName()] = pluginfile;
		vector<IHookPoint*> hookpoints = plugin->getProvidingHookPoints();

		for(int h = 0; h < hookpoints.size(); h++){
		    IHookPoint* hookpoint = hookpoints.at(h);

		    if( findHookPoint(hookpoint->getName()) == 0 ){
			_hookpoints[hookpoint->getName()] = hookpoint;
		    }else{
                        Logger::log("Ignoring already available hookpoint " + hookpoint->getName(), Logger::LOG_WARNING);
		    }
		}
	    }
	}
	catch(NoSuchLibraryException& nsle){
            Logger::log(nsle.what(), Logger::LOG_ERROR);
	}
	catch(NoSuchSymbolException& nsse){
            Logger::log(nsse.what(), Logger::LOG_ERROR);
	}

        IHookPoint* post_load_plugins = findHookPoint("core.post_load_plugins");
        if(post_load_plugins == 0){
            Logger::log("Could not find hookpoint core.post_load_plugins", Logger::LOG_ERROR);
        }else{
            _executeCommandsIn(post_load_plugins);
        }

    }

    while(this->_haveActivePlugins()){
        usleep(100000);
    }
}

/* Returns the names if the files in the plug-in directory */
vector<string> sentry::Sentry::_getPluginLibNames(string directory){
    vector<string> ret;
    struct dirent** files;
    
    int numfiles = scandir(directory.c_str(), &files, 0, alphasort);
    
    if(numfiles >= 0){
	for(int i = 0; i < numfiles; i++){
            if( strcmp(files[i]->d_name, ".") != 0 && strcmp(files[i]->d_name, "..") != 0 ){
		string filename(directory + files[i]->d_name);
		ret.push_back(filename);
	    }
            free(files[i]);
	}
    }else{
        Logger::log("Could not open directory " + directory, Logger::LOG_ERROR);
    }

    free(files);
    return ret;
}

/* Sets up the hookpoints for the Sentry core */
void sentry::Sentry::_setupHookpoints(){
    SentryHookPoint* postStartup = new SentryHookPoint(string("core.post_startup"));
    SentryHookPoint* preShutdown = new SentryHookPoint(string("core.pre_shutdown"));
    SentryHookPoint* postLoadPlugins = new SentryHookPoint(string("core.post_load_plugins"));
        
    _hookpoints[postStartup->getName()] = postStartup;
    _hookpoints[preShutdown->getName()] = preShutdown;
    _hookpoints[postLoadPlugins->getName()] = postLoadPlugins;
}

/* Executes the commands in the given hookpoint */
void sentry::Sentry::_executeCommandsIn(IHookPoint* hp){
    map<string, IPluginCommand*> commands = hp->getAttachedPluginCommands();
    map<string, IPluginCommand*>::iterator it = commands.begin();
    vector<string> not_used;

    for(; it != commands.end(); it++){
        IPluginCommand* command = it->second;
        if(command != 0){
            command->execute(not_used);
        }
    }
}

bool sentry::Sentry::_haveActivePlugins(){
    bool have = false;

    for(map<string, IPlugin*>::iterator it = _plugins.begin(); it != _plugins.end(); it++){
        IPlugin* plugin = it->second;
        if(plugin->isActive()){
            have = true;
            break;
        }
    }
    return have;
}

/* Finds a hookpoint by name */
IHookPoint* sentry::Sentry::findHookPoint(string name){
    map<string, IHookPoint*>::iterator hookpoint = _hookpoints.find(name);

    if( hookpoint == _hookpoints.end() ){
	// hookpoint doesn't exist
	return (IHookPoint*)0;
    }

    return hookpoint->second;
}

/* Finds a plug-in by name */
sentry::IPlugin* sentry::Sentry::findPlugin(string name){
    map<string, IPlugin*>::iterator plugin = _plugins.find(name);

    if(plugin == _plugins.end() ){
        return (IPlugin*)0;
    }
    return plugin->second;
}