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
/** \file ArgosDisplay.h
 *  Declaration of the parameter class ArgosDisplay.
 *  \author Stephan Meister
 *  \date 26.11.2010
 */

#ifndef _ARGOSDISPLAY_H_
#define _ARGOSDISPLAY_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef argosdisplay_EXPORTS
/// Visual C++ specific code
#define argosdisplay_DECLDIR __declspec(dllexport)
#else
#define argosdisplay_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define argosdisplay_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>
#include <QWidget>

namespace ArgosDisplay
{
	
class MainWindow ;

/// Charon Plugin Frontend for advanced Argos Display
template <typename T>
class argosdisplay_DECLDIR ArgosDisplayPlugin :
		public TemplatedParameteredObject<T> {
public:
	
	/// create a new ArgosDisplay
	/// \param name          Instance name
	ArgosDisplayPlugin(const std::string& name);

	/// Destruct Object
	~ArgosDisplayPlugin() ;

	/// Update object.
	virtual void execute();

	/// The vigra::MultiArray input
	InputSlot < vigra::MultiArrayView<5, T> > _in;

	InputSlot <QWidget*> _widgets ;

	Parameter<bool> _inputIsRGB ;

private:

	///main plugin window
	MainWindow* _mainWindow ;
} ; //class ArgosDisplayPlugin


} ; //namespace ArgosDisplay
#endif // _ARGOSDISPLAY_H_



