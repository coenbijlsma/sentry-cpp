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
#include "SentryHookPoint.h"
#include <iostream>
#include <vector>
#include <cstdlib> /* For exit(int) */
#include <dirent.h> /* For reading the plug-in directory */
#include <string.h> /* For strcmp */

using std::vector;
//using std::cerr;
//using std::cout;
//using std::endl;

/* Construcor */
Sentry::Sentry(){
    /* Load the config */
    try{
        _config = new SentryConfig("./sentry.conf");
    }catch(string error){
        Logger::log(error, Logger::LOG_FATAL);
        exit(1);
    }
    
    /* Load the hookpoints that Sentry itself provides */
    _setupHookpoints();
    
    string plugindir(_config->getValue("application", "plugindir") + "/");
    vector<string> files = _getPluginLibNames(plugindir); // XXX replace by configged value
    
    /**
     * Load the plug-ins 
     */
    for(int i = 0; i < files.size(); i++){
	try{
	    string pluginfile = files.at(i);
	    IPlugin* plugin = PluginLoader::loadPlugin(pluginfile);
	    
	    if(plugin != 0){
		_plugins[plugin->getName()] = plugin;
		_pluginpathmap[plugin->getName()] = pluginfile;
		vector<IHookPoint*> hookpoints = plugin->getProvidingHookPoints();
		
		for(int h = 0; h < hookpoints.size(); h++){
		    IHookPoint* hookpoint = hookpoints.at(h);
		    
		    if( _findHookPoint(hookpoint->getName()) == 0 ){
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
	    
    }
    
    /* Execute the post-startup actions */
    IHookPoint* post_startup = _findHookPoint("core.post_startup");
    
    if( ! post_startup ){
        Logger::log("Could not find hookpoint core.post_startup", Logger::LOG_ERROR);
    }else{
        _executeCommandsIn(post_startup);
    }
    
}

/* Destructor */
Sentry::~Sentry(){

    /* Execute the pre-shutdown actions */
    IHookPoint* pre_shutdown = _findHookPoint("core.pre_shutdown");
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
        
    IHookPoint* post_startup = _findHookPoint("core.post_startup");
    if(post_startup != 0){
        delete post_startup;
    }

}

/* Finds a hookpoint by name */
IHookPoint* Sentry::_findHookPoint(string name){
    map<string, IHookPoint*>::iterator hookpoint = _hookpoints.find(name);
    
    if( hookpoint == _hookpoints.end() ){
	// hookpoint doesn't exist
	return (IHookPoint*)0;
    }
    
    return hookpoint->second;
}

/* Returns the names if the files in the plug-in directory */
vector<string> Sentry::_getPluginLibNames(string directory){
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
void Sentry::_setupHookpoints(){
    SentryHookPoint* postStartup = new SentryHookPoint(string("core.post_startup"));
    SentryHookPoint* preShutdown = new SentryHookPoint(string("core.pre_shutdown"));
        
    _hookpoints[postStartup->getName()] = postStartup;
    _hookpoints[preShutdown->getName()] = preShutdown;
}

/* Executes the commands in the given hookpoint */
void Sentry::_executeCommandsIn(IHookPoint* hp){
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
