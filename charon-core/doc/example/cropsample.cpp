/*  Copyright (C) 2009 Jens-Malte Gottfried

 This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
/// @file cropsample.cpp
/// @author <a href="mailto:bc002@ix.urz.uni-heidelberg-de">Cornelius Ratsch</a>
/// Sample usage of parameter files
#include "PluginManager.h"

int main() {
	//Create PluginManager instance with the plugin path "./"
	PluginManager man(".");
	man.loadParameterFile("cropsample.wrp");
	man.executeWorkflow();
	man.saveParameterFile("cropsample2.wrp");
}
