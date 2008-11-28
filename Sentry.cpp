#include "Sentry.h"
#include "PluginLoader.h"
#include "SentryHookPoint.h"
#include <iostream>
#include <vector>
#include <dirent.h> /* For reading the plug-in directory */
#include <string.h> /* For strcmp */

using std::vector;
using std::cerr;
using std::cout;
using std::endl;

/* Construcor */
Sentry::Sentry(){
    /* Load the hookpoints that Sentry itself provides */
    _setupHookpoints();
    
    string plugindir = "./plugs/";
    vector<string> files = _getPluginLibNames(plugindir); // XXX replace by configged value
    
    /* Load the plug-ins */
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
		    
		    if( _hookpoints.find(hookpoint->getName()) == _hookpoints.end() ){
			_hookpoints[hookpoint->getName()] = hookpoint;
		    }else{
			cerr << "Ignoring already available hookpoint " << hookpoint->getName() << endl;
		    }
		}
	    }
	}
	catch(NoSuchLibraryException& nsle){}
	catch(NoSuchSymbolException& nsse){}
	    
    }
}

/* Destructor */
Sentry::~Sentry(){

    /* Clear the hookpoints */
    _hookpoints.clear();
    
    /* Unload the plug-ins */
    for(map<string, IPlugin*>::iterator it = _plugins.begin(); it != _plugins.end(); it++){
	IPlugin* plugin = it->second;
	delete plugin;
    }
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
	}
    }else{
	cerr << "Could not open directory " << directory << endl;
    }
	 
    return ret;
}

/* Sets up the hookpoints for the Sentry core */
void Sentry::_setupHookpoints(){
    // XXX figure out if map.clear() also calls destructors of contained objects
    SentryHookPoint* postStartup = new SentryHookPoint("sentry.core.post_startup");
    SentryHookPoint* preShutdown = new SentryHookPoint("sentry.core.pre_shutdown");
    
    _hookpoints[postStartup->getName()] = postStartup;
    _hookpoints[preSHutdown->getName()] = preShutdown;
}
