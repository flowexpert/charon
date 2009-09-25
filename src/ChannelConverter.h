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
/** @file ChannelConverter.h
 *  Channel Converter for CImg.
 *  Converts v dimension to t and vice versa.
 *  
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */
#ifndef _CHANNELCONVERTER_H_
#define _CHANNELCONVERTER_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef channelconverter_EXPORTS
///Visual C++ specific code
#define channelconverter_DECLDIR __declspec(dllexport)
#else
#define channelconverter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define channelconverter_DECLDIR
#endif

#include <CImg.h>
#include <ParameteredObject.h>

template <class T>
class channelconverter_DECLDIR ChannelConverter : public TemplatedParameteredObject<T>
{
	public:
		///Sequence to be converted.
		InputSlot<cimg_library::CImgList<T> > in;
		
		///Converted sequence.
		OutputSlot<cimg_library::CImgList<T> > out;
		
		///default constructor
		ChannelConverter(const std::string& name = "");
		
		///main function.
		virtual void execute();
		
		///default descructor.
		virtual ~ChannelConverter();
};

#endif