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

namespace StatisticsDisplay {

	struct Statistics
	{
		///all calculated statistic values and properties
		std::map<std::string, double> stats ;
		///name of parent object
		std::string origin ;
	} ;

	/// Charon Plugin Frontend for statistics calculation and display widget
	template <typename T>
	class statisticsdisplay_DECLDIR StatisticsDisplayPlugin :
			public TemplatedParameteredObject<T> {
	public:
	
		typedef std::map<std::string, double> StatMap ;

		/// create a new StatisticsDisplay
		/// \param name          Instance name
		StatisticsDisplayPlugin(const std::string& name);

		/// Destruct Object
		~StatisticsDisplayPlugin() ;

		/// Update object.
		virtual void execute();

		/// The vigra::MultiArray input
		InputSlot < vigra::MultiArrayView<5, T> > _in;

		InputSlot < vigra::MultiArrayView<5, T> > _mask;

		OutputSlot < QWidget*> _display ;
	
		Parameter <bool> _writeToSout ;
	
	private:

		QWidget* _exportWidget ;
	
		std::vector<Statistics> _statistics ;

		void createWidget() ;

	}; //class StatisticsDisplayPlugin

} ; //namespace StatisticsDisplay
#endif // _STATISTICSDISPLAY_H_




