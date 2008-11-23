#include "Sentry.h"
#include <iostream>
#include <vector>
#include <dirent.h> /* For reading the plug-in directory */

using std::vector;
using std::cerr;
using std::endl;

Sentry::Sentry(){
    vector<string> files = getPluginLibs;
    
}

Sentry::~Sentry(){
}

void Sentry::_loadPlugins(){

}

vector<string> getPluginLibs(string directory){
    vector<string> ret;
    struct dirent** files;
    
    int numfiles = scandir(directory, &files, 0, alphasort);
    
    if(numfiles >= 0){
	for(int i = 0; i < numfiles; i++){
	    string filename(files[i]->d_name);
	    ret.push_back(files[i]->d_name);
	}
    }else{
	cerr << "Could not open directory " << directory << endl;
    }
	 
    return ret;
}
