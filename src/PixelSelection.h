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
/**
 *  @file PixelSelection.h
 *  @brief declaration of abstract class PixelSelection
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 17.08.2009
 */

#ifndef PIXELSELECTION_H_
#define PIXELSELECTION_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef pixelselection_EXPORTS
///Visual C++ specific code
#define pixelselection_DECLDIR __declspec(dllexport)
#else
#define pixelselection_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define pixelselection_DECLDIR
#endif

#include <ParameteredObject.hxx>
#include <Roi.h>
#include "Pixel.hxx"
#include <CImg.h>
#include <vector>

/// Pixel Selection
/// This class saves all Pixel we are interested in, in a list of pixel
template<typename T>
class pixelselection_DECLDIR PixelSelection: public TemplatedParameteredObject<T>
{
protected:
	std::vector<Pixel<T>*> pixelList;

public:
	/// standard constructor
	PixelSelection(const std::string& name ="", const std::string& pluginName ="");
	/// inputslot ROI
	InputSlot<Roi<int> *> range;
	/// inputslot sequence
	InputSlot<cimg_library::CImgList<T> > sequence;
	/// outputslot list of pixel
	OutputSlot<PixelSelection<T> *> out;
	/// standard execute from ParameteredObject
	virtual void execute() = 0;
	/// return the list of Pixel
	std::vector<Pixel<T>*>& getListOfPixel();
};

#endif /* PIXELSELECTION_H_ */
