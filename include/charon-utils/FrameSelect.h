/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file FrameSelect.h
 *  Declaration of the parameter class FrameSelect.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef _CROP_H_
#define _CROP_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef frameselect_EXPORTS
///Visual C++ specific code
#define frameselect_DECLDIR __declspec(dllexport)
#else
#define frameselect_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define frameselect_DECLDIR
#endif

#include <charon-core/ParameteredObject.hxx>
#include <vigra/multi_array.hxx>
#include <FrameSelectWidget.hpp>

/// Select frames to be displayed (GUI-capable version of Crop)
/** This class is able to crop images along dimensions 2-4.
 *  It is possible to leave the last dimension untouched on RGB images.
 *  The first two dimensions (x,y) are not cropped.
 */
template <typename T>
class frameselect_DECLDIR FrameSelect : public TemplatedParameteredObject<T>
{
public:
	/// image data input slot
	InputSlot< vigra::MultiArrayView<5,T> >  in;
	/// image data output slot
	OutputSlot< vigra::MultiArray<5,T> > out;
	/// QWidget to be displayed in ArgosDisplay
	OutputSlot< QWidget* > widget;

	/// enable cropping last (RGB) dimension
	Parameter<bool> cropV;
	/// \name select frames/channels
	//\{
	Parameter<uint> z,t,v;
	//\}

	/// default constructor
	/// @param name             instance name
	FrameSelect(const std::string& name = "");

	/// \implements ParameteredObject::execute
	virtual void execute();

private:
	/// manipulation widget to be docked somewhere
	FrameSelectWidget* _gui;
};

#endif // _CROP_H_
