#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class IPlugin {
public:
    string getName() =0;
    string getFileName() =0;
    vector<string> getDependencies() =0;
    vector<IPluginCommand*> getCommands() =0;
    vector<IPluginCommand*> getCommandsByHookPoint(string hook) =0;
};

#endif
