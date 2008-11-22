#include "NoSuchLibraryException.h"
#include "NoSuchSymbolException.h"
#include "PluginLoader.h"
#include <iostream>

using std::cerr;
using std::endl;

int main(int argc, char** argv){
    try{
        PluginLoader loader;
	loader.unloadPlugin(0, "./blaat.so");
    }catch(NoSuchLibraryException& ex){
	cerr << ex.what() << endl;
    }
}
