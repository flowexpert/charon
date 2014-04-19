/*  Copyright (C) 2011 Daniel Kondermann

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
/** \file MaskedBlur.hxx
 *  Implementation of the parameter class MaskedBlur.
 *  \author <a href="mailto:daniel@kondermann.de">
 *      Daniel Kondermann</a>
 *  \date 19.12.2011
 */

#ifndef _MASKEDBLUR_HXX_
#define _MASKEDBLUR_HXX_

#include <charon-core/ParameteredObject.hxx>
#include "MaskedBlur.h"
#include <exception>

using namespace std;

template <typename T>
MaskedBlur<T>::MaskedBlur(const std::string& name) :
		TemplatedParameteredObject<T>(
			"MaskedBlur", name,
			"<h2>Applies a Gaussian blur with an individual sigma for each "
			"pixel</h2><br>"
			"The provided mask is used to scale between a minimum and "
			"maximum sigma value which is used as blur strength."
		)
{
	ParameteredObject::_setTags("charon-utils;CImg") ;

	ParameteredObject::_addInputSlot(
		img, "img",
		"Image to be blurred",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		mask, "mask",
		"Mask (with same dimensions as img) with values between 0 and 1 used"
		"for blur strength",
		"CImgList<float>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"Resulting blurred image",
		"CImgList<T>");

	ParameteredObject::_addParameter< float >(
		minSigma, "minSigma",
		"Sigma to be used when mask value is zero",
		0, "float");
	ParameteredObject::_addParameter< float >(
		maxSigma, "maxSigma",
		"Sigma to be used when mask value is 1 (linearly interpolated between"
		"minSigma and maxSigma)",
		5, "float");
	ParameteredObject::_addParameter< bool >(
		useOpacity, "useOpacity",
		"When useOpacity is true, the image is blurred once with maxSigma"
		"and then blended according to the mask with the original",
		true, "bool");
	ParameteredObject::_addParameter< bool >(
		doNormalize, "normalize",
		"When true, the mask is normalized per image list entry",
		false, "bool");
	ParameteredObject::_addParameter< bool >(
		doInvert, "invert",
		"When true, the mask is inverted per image list entry (after normalization)",
		false, "bool");
}

template <typename T>
void MaskedBlur<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	if(img().size() != mask().size())
		throw runtime_error("img().size() != mask().size()");
	cimglist_for(img(), i)
	{
		char err[1024];
		if(img()[i].width() != mask()[i].width())
		{
			sprintf(err, "Width of input image %d is %d but width of mask is %d!", i, img()[i].width(), mask()[i].width());
			throw runtime_error(err);
		}
		if(img()[i].height() != mask()[i].height())
		{
			sprintf(err, "Height of input image %d is %d but height of mask is %d!", i, img()[i].height(), mask()[i].height());
			throw runtime_error(err);
		}
		if(img()[i].depth() != mask()[i].depth())
		{
			sprintf(err, "Depth of input image %d is %d but depth of mask is %d!", i, img()[i].depth(), mask()[i].depth());
			throw runtime_error(err);
		}
		if(img()[i].spectrum() != mask()[i].spectrum())
		{
			sprintf(err, "Spectrum of input image %d is %d but spectrum of mask is %d! Using first color channel.", i, img()[i].spectrum(), mask()[i].spectrum());
			sout << "WARNING: " << err << endl;
			//throw runtime_error(err);
		}
	}

	out() =img();
	if(useOpacity())
	{
		sout << "using blur-mask for opacity of an image blurred with sigma " << maxSigma() << endl;
	}
	cimglist_for(img(), i)
	{
		sout << "working on image " << i << "." << endl;
		if(useOpacity())
		{
			sout << "normalize:" << doNormalize() << " invert:" << doInvert() << endl;
			cimg_library::CImg<T> tmp =img()[i].get_blur(maxSigma());
			cimg_library::CImg<T> msk =mask()[i];
			if(doNormalize())
			{
				sout << "normalizing mask " << i << endl;
				if(!msk.sum())
				{
					sout << "mask " << i << " is zero." << endl;
				}
				else
				{
					msk.normalize((T)0, (T)1);
					if(!msk.sum())
					{
						sout << "mask " << i << " was single-valued but non-zero. setting all values to 1." << endl;
						msk.fill((T)1);
					}
				}
			}
			if(doInvert())
			{
				sout << "inverting mask " << i << endl;
				msk =(T)1-msk;
			}
			cimg_forXYZC(img()[i], x, y, z, c)
			{
				out()[i](x, y, z, c) =(T)(msk(x, y, z, c)*tmp(x, y, z, c)+(1.0f-msk(x, y, z, c))*img()[i](x, y, z, c)); //mask is supposed to be in the range 0..1
			}
		}
		else
		{
			cimg_library::CImg<T> msk =mask()[i];
			if(doNormalize())
			{
				sout << "normalizing mask " << i << endl;
				if(!msk.sum())
				{
					sout << "mask " << i << " is zero." << endl;
				}
				else
				{
					msk.normalize((T)0, (T)1);
					if(!msk.sum())
					{
						sout << "mask " << i << " was single-valued but non-zero. setting all values to 1." << endl;
						msk.fill((T)1);
					}
				}
			}
			if(doInvert())
			{
				sout << "inverting mask " << i << endl;
				msk =(T)1-msk;
			}
			cimg_forXYZC(img()[i], x, y, z, c)
			{
				float sigma =(float)(msk(x, y, z, c)*(maxSigma-minSigma)+minSigma); //mask is supposed to be in the range 0..1
				if(sigma)
				{
					unsigned int w =(int)(sigma*3+0.5f);
					if(w%2 == 0) w++; //create mask width with center pixel
					unsigned int cp =w/2;
					cimg_library::CImg<T> m(w, w, 1, 1, 0);
					T color=(T)1.0f;
					m.draw_gaussian((float)cp, (float)cp, sigma, &color);
					m /=m.sum(); //make sure that sum of all weights is one
					double value =0;
					int px, py;
					cimg_forXY(m, mx, my) //convolve this point with the gaussian of current sigma
					{
						px =x+(mx-cp);
						py =y+(my-cp);
						if(img()[i].containsXYZC(px, py, z, c))
							value +=img()[i](px, py, z, c)*m(mx, my);
					}
					out()[i](x, y, z, c) =(T)value;
				}
				else
				{
					out()[i](x, y, z, c) =img()[i](x, y, z, c);
				}
			}
		}
	}
}

#endif /* _MASKEDBLUR_HXX_ */
