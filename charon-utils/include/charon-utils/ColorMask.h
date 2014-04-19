/*  Copyright (C) 2009 Jens-Malte Gottfried

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
/// \file ColorMask.h
/// Declaration of the parameter class ColorMask.
/// \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
///     Jens-Malte Gottfried</a>
/// \date 27.11.2010

#ifndef COLORMASK_H_
#define COLORMASK_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef colormask_EXPORTS
///Visual C++ specific code
#define colormask_DECLDIR __declspec(dllexport)
#else
#define colormask_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define colormask_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

/// Convert gray values into false colors.
/** Assigns red to negative values and blue to positive values
 *  \ingroup charon-modules
 *  \ingroup charon-utils
 *  \ingroup image-manipulators
 */
template<typename T>
class colormask_DECLDIR ColorMask : public TemplatedParameteredObject<T>
{
public:
	/// standard constructor
	ColorMask(const std::string& name = "" /** [in] instance name*/);
	virtual ~ColorMask();

	///  color mask selection
	/**  select coloring pattern:
	 *   - "BR": blue/red colormap, zero is white, red is neg, blue is pos
	 *   - "Rainbow": rainbow colors for 10 bit int data
	 */
	Parameter<std::string> colorMask;

	/// values with abs larger than fullVal are assigned to red/blue
	/// (0=auto)
	Parameter<T> fullVal;

	/// image data input (gray values) [t](x,y,z,1)
	InputSlot<cimg_library::CImgList<T> > in;

	/// RGB data output [t](x,y,z,c)
	OutputSlot<cimg_library::CImgList<T> > out;

protected:
	/// \copybrief ParameteredObject::execute()
	/** \copydetails ParameteredObject::execute() */
	virtual void execute();

private:
	/// calculated value for full red/blue values
	T _max;

	/// calculate color mask value
	/** \param val    gray value to convert
	 *  \param res    result (1x1x1x3)
	 */
	void _applyMask(T val, cimg_library::CImg<double>& res) const;

	/// calculate rainbow color mask
	/** \param val    gray value to convert (expected: 11 bit uint)
	 *  \param res    result (1x1x1x3)
	 */
	void _rainbowMask(
		unsigned short val, cimg_library::CImg<unsigned char>& res) const;

	/// gamma correction for rainbow pattern
	unsigned short* _gamma;
};

#endif // COLORMASK_H_
