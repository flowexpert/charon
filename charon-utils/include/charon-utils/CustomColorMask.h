/*  Copyright (C) 2012 Niels Buwen

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file CustomColorMask.h
 *  Declaration of the parameter class CustomColorMask.
 *  \author <a href="mailto:niels.buwen@web.de">
 *      Niels Buwen</a>
 *  \date 27.04.2012
 */

#ifndef _CUSTOMCOLORMASK_H_
#define _CUSTOMCOLORMASK_H_

#ifdef _MSC_VER
#ifdef customcolormask_EXPORTS
/// Visual C++ specific code
#define customcolormask_DECLDIR __declspec(dllexport)
#else
#define customcolormask_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else
/// Not needed without MSVC
#define customcolormask_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>
//#include "CustomColorMaskWidget.hpp"

class CustomColorMaskWidget ;
class QWidget ;

/// loads color map and applies it to an image
template <typename T>
class customcolormask_DECLDIR CustomColorMask :
		public TemplatedParameteredObject<T> {
public:
	/// default constructor
	/// \param name          Instance name
	CustomColorMask(const std::string& name = "");

	/// input image
	InputSlot< cimg_library::CImgList<T> > in;
	/// costum color map
	InputSlot< cimg_library::CImgList<T> > mask;

	/// input widget
	OutputSlot< QWidget*> widget;

	/// output image
	OutputSlot< cimg_library::CImgList<T> > out;

	/// Update object.
	virtual void execute();

	/// start value to applay mask
	Parameter<T> begin;

	/// max value to applay mask
	Parameter<T> end;

	/// \name widget references
	// \{
	Parameter<double> wBegin;
	Parameter<double> wEnd;
	// \}

	/// mask type
	Parameter<std::string> maskType; 

	/// max value to applay mask
	Parameter<bool> minimap;

	//destruktor
	~CustomColorMask();

private:
	CustomColorMaskWidget* _gui;

	std::vector<T> _maskSw;

	std::vector<T> _maskWs;

	std::vector<T> _maskRainbow[3];

	std::vector<T> _maskJet[3] ;

	std::vector<T> _maskCustom[3] ;

};

#endif // _CUSTOMCOLORMASK_H_
