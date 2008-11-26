#ifndef IPLUGINCOMMAND_H
#define IPLUGINCOMMAND_H

#include "IPlugin.h"
#include <string>

using std::string;

class IPlugin;

class IPluginCommand {
public:
    virtual IPlugin* getPlugin() =0;
    virtual string getName() =0;
    virtual string getHookPoint() =0;
    virtual void execute() =0;
};

#endif
