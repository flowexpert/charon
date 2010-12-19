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

#ifndef _STATISTICSDISPLAY_H_
#define _STATISTICSDISPLAY_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef statisticsdisplay_EXPORTS
/// Visual C++ specific code
#define statisticsdisplay_DECLDIR __declspec(dllexport)
#else
#define statisticsdisplay_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define statisticsdisplay_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>


class QWidget ;

/// Charon Plugin Frontend for statistics calculation and display widget
template <typename T>
class statisticsdisplay_DECLDIR StatisticsDisplay :
		public TemplatedParameteredObject<T> {
public:
	
	typedef std::map<std::string, double> StatMap ;

	/// create a new StatisticsDisplay
	/// \param name          Instance name
	StatisticsDisplay(const std::string& name);

	/// Destruct Object
	~StatisticsDisplay() ;

	/// Update object.
	virtual void execute();

	/// The vigra::MultiArray input
	InputSlot < vigra::MultiArrayView<5, T> > _in;

	OutputSlot < QWidget*> _display ;
	
	Parameter <bool> _writeToSout ;
	
private:

	QWidget* _exportWidget ;
	
	std::vector<std::map<std::string, double> > _statistics ;

	void createWidget() ;

};

#endif // _STATISTICSDISPLAY_H_




