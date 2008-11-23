#ifndef SENTRYHOOKPOINT_H
#define SENTRYHOOKPOINT_H

#include "IHookPoint.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class SentryHookPoint : public IHookPoint {
private:
    string _name;
    vector<string> _attachedPluginCommands;
    
    vector<string>::iterator _findPluginCommand(string name);
public:
    
    /**
     * Creates a HookPoint that has the given name.
     * @brief Constructor
     */
    SentryHookPoint(string name) throw();
    
    /**
     * @brief Destructor
     */
    virtual ~SentryHookPoint();
    
    /**
     * @brief Returns the name of thie hookpoint.
     */
    string getName() throw();
    
    /**
     * @brief Returns the attached plug-in commands.
     */
    vector<string> getAttachedPluginCommands() throw();
    
    /**
     * @brief Adds the given plug-in command to the list.
     */
    void addPluginCommand(string name) throw();
    
    /**
     * @brief Removes the given plug-in command from the list.
     */
    bool removePluginCommand(string name) throw();
};

#endif
