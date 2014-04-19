/*  Copyright (C) 2010 Stephan Meister

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
/** \file charonwidgets.h
 *  This files sets the library defines needed by all classes in charonwidgets
 *  \author Stephan Meister
 *  \date 02.12.2010
 */


#ifndef _CHARONWIDGETS_H_
#define _CHARONWIDGETS_H_

#if defined(MSVC) && defined(HANDLE_DLL)
	#ifdef charonwidgets_EXPORTS
	/// Visual C++ specific code
		#define charonwidgets_DECLDIR __declspec(dllexport)
	#else
		#define charonwidgets_DECLDIR __declspec(dllimport)
	#endif /*Export or import*/
#else /* No DLL handling or GCC */
	/// Not needed with GCC
	#define charonwidgets_DECLDIR
#endif

#endif /* _CHARONWIDGETS_H_ */