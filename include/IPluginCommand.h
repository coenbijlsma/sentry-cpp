/**
 * This file is part of Sentry.
 *
 * Sentry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sentry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Sentry.  If not, see <http://www.gnu.org/licenses/>.
 */
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
