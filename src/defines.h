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
/**	@file	defines.h
 *	@brief	Common definitions.
 *	@date	16.10.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef DEFINES_H_
#define DEFINES_H_

#ifdef NDEBUG
	// Release mode
	#define QT_NO_DEBUG_OUTPUT
	#define MH_NO_DEBUG_OUTPUT
	#define MH_NO_WARNING_OUTPUT
#else
	// Debug mode
#endif

#endif /*DEFINES_H_*/
