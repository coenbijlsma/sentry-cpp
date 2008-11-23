#ifndef IHOOKPOINT_H
#define IHOOKPOINT_H

#include <vector>
#include <string>

using std::string;
using std::vector;

class IHookPoint {
public:
    /**
     * Returns the hookpoint's name. To avoid name collisions, hookpoint names
     * must be in the form <plugin_name>.<hookpoint_name>
     * For example, the hookpoint for sentry's core after startup 
     * is "core.post_startup"
     * @brief Returns the name of the hookpoint.
     */
    virtual string getName() =0;

    /**
     * Returns the names of the plug-in commands that 
     * are attached to this hookpoint. If no commands are
     * attached, this method returns an empty vector.
     * @brief Returns the attached plug-in commands.
     */
    virtual vector<string> getAttachedPluginCommands() =0;
    
    /**
     * Adds the given plug-in command to the list of attached
     * plug-in commands.
     * If the command already exists, this method has no effect.
     */
    virtual void addPluginCommand(string name) =0;
    
    /**
     * Removes the given name from the list of attached plug-in commands.
     * If the name does not exist in the list, nothing happens and
     * true is returned.
     * @brief Removes a plug-in command.
     * @return bool Whether removing the name has succeeded.
     */
    virtual bool removePluginCommand(string name) =0;
};

#endif
