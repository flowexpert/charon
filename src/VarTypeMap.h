/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	VarTypeMap.h
 *	@brief	Mapping of types given as string to QVariant::type.
 *	@date	01.10.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef VARTYPEMAP_H_
#define VARTYPEMAP_H_

#include <map>
#include <string>
#include <QVariant>
#include <QMutex>

/**	Convert parameter types into Qt versions.
 *	This class serves to convert parameter types given as std::string
 *	into the corresponding QVariant::Type.
 *	This class implements the singleton pattern to ensure, that only one map
 *	exists. This class uses a Mutex to be thread-safe. */
class VarTypeMap {
private:
	VarTypeMap();
	~VarTypeMap();
	VarTypeMap(const VarTypeMap&); 				///< not copyable
    VarTypeMap& operator= (const VarTypeMap&);	///< not assignable

    /// mapping from given type values to QVariant types
    std::map<std::string, QVariant::Type> _typeMap;
    /// pointer to the singleMapper instance
    static VarTypeMap* _address;
    /// Mutex to avoid multiple creation
    static QMutex _lock;
public:
	/// return reference to an VarTypeMap instance
	static VarTypeMap& instance();

	/// return type map
	QVariant::Type operator[](std::string type) const;
};

#endif /*VARTYPEMAP_H_*/
