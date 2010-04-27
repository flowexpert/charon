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
/** \file ChannelConverter.h
 *  Declaration of class ChannelConverter.
 *  \author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  \date 8.09.2009
 */
#ifndef _CHANNELCONVERTER_H_
#define _CHANNELCONVERTER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef channelconverter_EXPORTS
/// Visual C++ specific code
#define channelconverter_DECLDIR __declspec(dllexport)
#else
#define channelconverter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
/// Not needed with GCC
#define channelconverter_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>


/// Channel Converter for CImgLists
/** Permutes image dimensions.
 *  Select permutation using the parameter <i>scheme</i>
 *  Per default converts v dimension to t and vice versa.
 */
template <class T>
class channelconverter_DECLDIR ChannelConverter :
		public TemplatedParameteredObject<T>
{
public:
	/// default constructor
	ChannelConverter(const std::string& name = "");
	
	/// Sequence to be converted.
	InputSlot<cimg_library::CImgList<T> > in;
	
	/// Converted sequence.
	OutputSlot<cimg_library::CImgList<T> > out;

	/// Conversion scheme
	/** <i>xyzvt</i> or <i>01234</i> leaves dimensions unpermuted.
	 *  Exchange dimensions as needed, e.g. the default <i>xyztv</i> will
	 *  permute the 4th CImg dimension (v) and the list element dimension (t).
	 *
	 *  Having a flow field with time in the z-dimension and flow
	 *  components in the v-dimension, you have to select <i>xytzv</i>
	 *  or <i>01423</i>.
	 *  This does also work to convert 2D image sequences having their
	 *  time axis in the z-dimension (monochrome or color channels in v-dim).
	 */
	Parameter<std::string> scheme;
	
	/// Perform conversion.
	/** The input CImgList has all the frames in different CImg objects in
	 *  the list what we want is a CImgList that splits the sequence by
	 *  channel into seperate CImg objects.
	 */
	virtual void execute();

private:
	/// dimension select for permutation
	/// \param x,y,z,v,t input
	/// \param sel	select wich variable to return
	unsigned int _select(
		unsigned int x, unsigned int y, unsigned int z,
		unsigned int v, unsigned int t, char sel) const;
};

#endif
