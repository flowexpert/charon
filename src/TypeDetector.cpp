/*  This file is part of Charon.

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
/** @file TypeDetector.cpp
 *  @brief Implementation of class TypeDetector
 *
 *  @date   17.12.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#include "TypeDetector.h"

// init static pointer
TypeDetector* TypeDetector::_instance = 0;

TypeDetector::TypeDetector()
{
    _typemap[typeid(bool).name()]                     = "bool";
    _typemap[typeid(int).name()]                      = "int";
    _typemap[typeid(unsigned int).name()]             = "uint";
    _typemap[typeid(char).name()]                     = "char";
    _typemap[typeid(float).name()]                    = "float";
    _typemap[typeid(double).name()]                   = "double";
    _typemap[typeid(long int).name()]                 = "long";
    _typemap[typeid(unsigned long int).name()]        = "ulong";
    // long long int not in ISO C++ 1998
    //_typemap[typeid(long long int).name()]          = "longlong";
    //_typemap[typeid(unsigned long long int).name()] = "ulonglong";
    _typemap[typeid(std::string).name()]              = "string";
}

const TypeDetector& TypeDetector::instance() {
    if(!_instance)
        _instance = new TypeDetector();
    return *_instance;
}

std::string TypeDetector::type(const std::string& info) const {
    std::map<std::string, std::string>::const_iterator entry;
    entry = _typemap.find(info);
    if(entry == _typemap.end())
        return info;
    else
        return entry->second;
}

