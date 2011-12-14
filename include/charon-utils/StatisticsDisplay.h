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
#include <charon-utils/CImg.h>

class QWidget ;

namespace StatisticsDisplay {
	class StatisticsDisplayWidget ;

	///Struct holding all statistics data for an object
	struct Statistics
	{
		///all calculated statistic values and properties
		std::map<std::string, double> stats ;
		///name of parent object
		std::string origin ;
	} ;

	/// Charon Plugin Frontend for statistics calculation and display widget
	/**
	 *  \ingroup charon-modules
	 *  \ingroup charon-utils
	 *  \ingroup visualization
	 */
	template <typename T>
	class statisticsdisplay_DECLDIR StatisticsDisplayPlugin :
			public TemplatedParameteredObject<T> {
	public:
	

		///Name-Statistics Map
		typedef std::map<std::string, double> StatMap ;

		/// create a new StatisticsDisplay
		/// \param name          Instance name
		StatisticsDisplayPlugin(const std::string& name);

		/// Destruct Object
		~StatisticsDisplayPlugin() ;

		/// Update object.
		virtual void execute();

		/// The vigra::MultiArray input
		InputSlot < vigra::MultiArrayView<5, T> > _vigraIn;

		/// The CImgList input
		InputSlot < cimg_library::CImgList<T> > _cimgIn;

		/// A mask to decide where to compute the statistics
		InputSlot < vigra::MultiArrayView<5, T> > _vigraMask;

		/// A mask to decide where to compute the statistics
		InputSlot < cimg_library::CImgList<T> > _cimgMask;

		/// image of histogram
		OutputSlot < cimg_library::CImgList<T> > _histograms ;

		/// exported QWidget to display the results
		OutputSlot < QWidget*> _display ;
	
		/// also write the results to sout stream
		Parameter <bool> _writeToSout ;

		/// number of bins for histogram
		Parameter <size_t> _numBins ;

	
	private:

		/// pointer to the export widget
		StatisticsDisplayWidget* _exportWidget ;
	
		/// statistics data for all input slots
		std::vector<Statistics> _statistics ;



	}; //class StatisticsDisplayPlugin

} //namespace StatisticsDisplay
#endif // _STATISTICSDISPLAY_H_




