/*  Copyright (C) 2009 Jens-Malte Gottfried

    This file is part of Tuchulcha.
    
    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file modeltest.cpp
/// Tests for usage of (qt related) parameter file models.
/// Search for memory allocation bug.

#include <iostream>
#include <cstdlib>
#include <ParameterFile.hxx>
#include <ParameterFileModel.h>

#ifndef DEFAULTCLASSFILE
#error DEFAULTCLASSFILE not defined
#endif

/// Starting point of application.
int main() {
    QString fileName = DEFAULTCLASSFILE;

    // Check if file can be loaded and display content
    ParameterFile file(fileName.toAscii().constData());
    std::cout << "Set Params (should be empty):" << std::endl;
    file.showSetParams();
    std::cout << "\nParameter list:" << std::endl;
    std::vector<std::string> parameters = file.getKeyList();
    for(uint i=0; i < parameters.size(); i++)
        std::cout << "\t" << parameters[i] << std::endl;

    // Try to load the same file via a ParameterFileModel
    ParameterFileModel model;
    model.load(fileName);

    return EXIT_SUCCESS;
}
