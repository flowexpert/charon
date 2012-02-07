/*  Copyright (C) 2012 Gerald Mwangi

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file   groupVirtualSlot.cpp
 *  @brief  Unit tests VirtualSlot and ParameteredGroupObject class.
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */

#include "../include/charon-core/IfGroup.h"
#include "../include/charon-core/ParameteredGroupObject.h"

int main()
{
    ParameteredGroupObject maingr("MainGroup","MainGroup");
    std::vector<std::string> paths;
    paths.push_back("/home/gmwangi/Programming/workspace-qtcreator/Charon/sources/supernodes/install/lib/charon-plugins");
    maingr.pluginPaths=paths;
    maingr.workFlowFile="/home/gmwangi/Programming/workspace-qtcreator/Charon/sources/supernodes/testworkflows/ifgroup/global.par";
    maingr.initialize();
    maingr.execute();
    maingr.finalize();
}
