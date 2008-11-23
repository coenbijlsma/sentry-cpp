#ifndef SENTRY_H
#define SENTRY_H

#include "IHookPoint.h"
#include "IPlugin.h"
#include <map>

using std::map;

class Sentry {
private:
    map<string, IHookPoint*> _hookPoints;
    map<string, IPlugin*> _plugins;
    
    void _loadPlugins();
    
public:
    Sentry();
    ~Sentry();
};

#endif
