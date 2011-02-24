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

class QWidget ;

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

	/// Pointers to widgets which will be displayed as QDockWidgets
	InputSlot <QWidget*> _widgets ;

	/// interpret image as RGB(0-255) if dim 5 is of size 3
	
	Parameter<bool> _inputIsRGB ;

private:

	///main plugin window
	MainWindow* _mainWindow ;

} ; //class ArgosDisplayPlugin

///abstract adapter class which allows ViewStack access to templated image data
class AbstractPixelInspector
{
public :	
	
	///constructor
	AbstractPixelInspector(const std::string& name, bool rgb) ;
	
	///destructor
	~AbstractPixelInspector() {;}

	///access pixel values at xy-position
	virtual const std::vector<double> operator()(int x, int y) const = 0;

	///return RGBA copy of image data
	virtual const vigra::QRGBImage getRGBAImage() = 0;

	///return float copy of image data
	virtual const vigra::FImage getFImage() = 0;

	///name of parent plugin instance
	const std::string name ;

	///is image data RGBA
	const bool isRGBA ;

} ;

///concrete template instances of PixelInspector
template<typename T>
class PixelInspector : public AbstractPixelInspector
{
public:

	PixelInspector(const vigra::MultiArrayView<5, T>& mArray, const std::string& name, bool rgb) ;
	~PixelInspector() ;

	virtual const std::vector<double> operator()(int x, int y) const ;

	virtual const vigra::QRGBImage getRGBAImage() ;

	virtual const vigra::FImage getFImage() ;

private:

	const vigra::MultiArrayView<5, T>& _mArray ;

} ;


} ; //namespace ArgosDisplay
#endif // _ARGOSDISPLAY_H_



