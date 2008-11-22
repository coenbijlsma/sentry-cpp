#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <vector>
#include "IHookPoint.h"
#include "IPluginCommand.h"

using std::string;
using std::vector;

class IPlugin {
public:

    /**
     * Returns the name of this plug-in.
     */
    virtual string getName() =0;
    
    /**
     * Returns the hookpoints this plug-in provides.
     */
    virtual vector<IHookPoint*> getProvidingHookPoints() =0;
    
    /**
     * Returns the names of other plug-ins this plug-in depends on.
     * Two plug-ins may have a reference to each other, and because of that
     * all plug-ins are first loaded into the sentry core, and checked 
     * afterwards.
     * If a plug-in is missing that is required by other plug-ins, those
     * plug-ins are removed as well, and therefore cannot be used.
     */
    virtual vector<string> getDependencies() =0;
    
    /**
     * Returns the commands this plug-in provides.
     */
    virtual vector<IPluginCommand*> getCommands() =0;
    
};

typedef IPlugin* create_t();
typedef bool destroy_t(IPlugin*);

#endif
