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
/** \file DllEx.h
 *  Dynamic library API import/export prprocessor macros
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 02.09.2011
 */

/** \def charon_core_PUBLIC
 *  Preprocessor macro to define this class/function as public visible,
 *  i.e. to be exported to the ELF dynamic section but not to the dll import
 *  library. This should be used e.g. for templates to avoid the
 *  "...dllimport cannot be defined" MSVC error.
 */
/** \def charon_core_DLL_PUBLIC
 *  Preprocessor macro to define this class/function as public visible,
 *  i.e. to be exported to the charon-core dll import library
 *  and to the ELF dynamic section
 */
/** \def charon_plugins_DLL_PUBLIC
 *  Preprocessor macro to define this class/function as public visible,
 *  i.e. to be exported to the charon-plugins dll import library
 *  and to the ELF dynamic section
 */
/** \def charon_groups_DLL_PUBLIC
 *  Preprocessor macro to define this class/function as public visible,
 *  i.e. to be exported to the charon-groups dll import library
 *  and to the ELF dynamic section
 */
/** \def charon_core_LOCAL
 *  Preprocessor macro to define this class/function as hidden,
 *  i.e. not available from outside the charon-core library.
 */
/** \def charon_DEPRECATED
 *  handle deprecated messages on GCC and MSVC
 */

#ifndef _CHARON_DLL_EX_H_
#define _CHARON_DLL_EX_H_

#ifdef _MSC_VER // MSVC
	#define charon_core_PUBLIC
	#ifdef charon_core_EXPORTS
		#define charon_core_DLL_PUBLIC __declspec(dllexport)
	#else
		#define charon_core_DLL_PUBLIC __declspec(dllimport)
	#endif
	#ifdef charon_plugins_EXPORTS
		#define charon_plugins_DLL_PUBLIC __declspec(dllexport)
	#else
		#define charon_plugins_DLL_PUBLIC __declspec(dllimport)
	#endif
	#ifdef charon_groups_EXPORTS
		#define charon_groups_DLL_PUBLIC __declspec(dllexport)
	#else
		#define charon_groups_DLL_PUBLIC __declspec(dllimport)
	#endif
	#define charon_core_LOCAL
	#define charon_DEPRECATED __declspec(deprecated)
#else // UNIX
	#if __GNUC__ >= 4
		#define charon_core_PUBLIC __attribute__ ((visibility ("default")))
		#define charon_core_DLL_PUBLIC __attribute__ ((visibility ("default")))
		#define charon_plugins_DLL_PUBLIC __attribute__ ((visibility ("default")))
		#define charon_groups_DLL_PUBLIC __attribute__ ((visibility ("default")))
		#define charon_core_LOCAL __attribute__ ((visibility ("hidden")))
	#else
		#define charon_core_PUBLIC
		#define charon_core_DLL_PUBLIC
		#define charon_plugins_DLL_PUBLIC
		#define charon_groups_DLL_PUBLIC
		#define charon_core_LOCAL
	#endif
	#define charon_DEPRECATED __attribute__((deprecated))
#endif

#endif // _CHARON_DLL_EX_H_
