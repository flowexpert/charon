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
#include "vigraqt/vigraqimage.hxx"
#include <vigra/stdimage.hxx>
#include <charon-utils/CImg.h>

class QWidget ;

namespace ArgosDisplay
{
	class MainWindow ;

	/// Charon Plugin Frontend for advanced Argos Display
	/** Displays CImg and Vigra Images.
	 *  May be extended by arbitrary widgets, e.g. for
	 *  frame selection or statistics display.
	 *  \ingroup charon-modules
	 *  \ingroup charon-utils
	 *  \ingroup visualization
	 */
	template <typename T>
	class argosdisplay_DECLDIR ArgosDisplayPlugin :
			public TemplatedParameteredObject<T> {
	public:
		/// create a new ArgosDisplay
		/// \param name          Instance name
		ArgosDisplayPlugin(const std::string& name);

		/// Destruct Object
		~ArgosDisplayPlugin() ;

		/// The vigra::MultiArray input
		InputSlot < vigra::MultiArray<5, T> > _vigraIn;

		/// The CimgList input
		InputSlot < cimg_library::CImgList<T> > _cimgIn ;

		/// Pointers to widgets which will be displayed as QDockWidgets
		InputSlot <QWidget*> _widgets;

	protected:
		/// Update object.
		virtual void execute();

	private:
		/// main plugin window
		MainWindow* _mainWindow ;

	}; // class ArgosDisplayPlugin

	/// abstract adapter class which allows ViewStack access to
	/// templated image data
	class AbstractPixelInspector
	{
	public :
		/// constructor
		/** \param name          displayed name
		 */
		AbstractPixelInspector(const std::string& name) ;

		/// destructor
		virtual ~AbstractPixelInspector() {}

		/// access dimensions of image
		virtual const std::vector<int>& dim() const = 0 ;
		
		/// access pixel values at xy-position
		virtual const std::vector<double>& operator()(int x, int y) const = 0;

		/// return RGBA copy of image data
		virtual const vigra::QRGBImage getRGBImage() const = 0;

		/// return float copy of image data
		virtual const vigra::FImage getFImage() const = 0;

		/// is last dimension at least of size 3
		virtual bool isRGB() const = 0;
		
		/// name of parent plugin instance
		const std::string name ;
	} ;

	/// vigra specific template instances of PixelInspector
	template<typename T>
	class VigraPixelInspector : public AbstractPixelInspector
	{
	public:
		/// constructor
		/** \param mArray        Image Data
		 *  \param name          displayed name
		 */
		VigraPixelInspector(
				const vigra::MultiArrayView<5, T>& mArray,
				const std::string& name) ;
		virtual ~VigraPixelInspector() {}
		
		inline virtual const std::vector<int>& dim() const ;
		inline virtual const std::vector<double>& operator()(int x, int y) const ;
		virtual const vigra::QRGBImage getRGBImage() const;
		virtual const vigra::FImage getFImage() const;
		virtual bool isRGB() const ;

	private:
		/// data store
		const vigra::MultiArrayView<5, T>& _mArray ;
	} ;

	/// CImg specific template instances of PixelInspector
	template<typename T>
	class CImgPixelInspector : public AbstractPixelInspector {
	public :
		/// constructor
		/** \param mArray        Image Data
		 *  \param name          displayed name
		 */
		CImgPixelInspector(
				const cimg_library::CImgList<T>& mArray,
				const std::string& name) ;
		virtual ~CImgPixelInspector() {}

		inline virtual const std::vector<int>& dim() const ;
		inline virtual const std::vector<double>& operator()(int x, int y) const ;
		virtual const vigra::QRGBImage getRGBImage() const;
		virtual const vigra::FImage getFImage() const;
		virtual bool isRGB() const ;

	private:
		/// data store
		const cimg_library::CImgList<T>& _mArray ;
	};

} // namespace ArgosDisplay
#endif // _ARGOSDISPLAY_H_



