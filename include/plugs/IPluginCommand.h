#ifndef IPLUGINCOMMAND_H
#define IPLUGINCOMMAND_H

#include "IPlugin.h"
#include <string>

using std::string;

class IPluginCommand {
public:
    IPlugin* getPlugin() =0;
    string getName() =0;
    string getHookPoint() =0;
    void execute() =0;
};

#endif
