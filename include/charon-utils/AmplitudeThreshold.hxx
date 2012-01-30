/*  Copyright (C) 2011 Julian Coordts

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
/** \file AmplitudeThreshold.hxx
 *  Implementation of the class AmplitudeThreshold.
 *  \author Julian Coordts, Stephan Meister
 *  \date 13.01.2012
 */

#ifndef _AMPLITUDETHRESHOLD_HXX_
#define _AMPLITUDETHRESHOLD_HXX_

#include "AmplitudeThreshold.h"

template <typename T>
AmplitudeThreshold<T>::AmplitudeThreshold(const std::string& name) :
	TemplatedParameteredObject<T>("amplitudethreshold", name,
		"Cut off flow vectors whose amplitude are strict higher (ore lower) "
		"than a given threshold.<br>"
		"The Amplitude is defined as the square-root of the sum of all pixel values"
		" along the first Dimension of the input CImgList (usually 2 for classical"
		" optical flow fields).<br>"
		"This Plugin is mostly usefull for visualizing flow fields with some outliers."
		)
{
	ParameteredObject::_addParameter (_threshold, "threshold", 
		"Flow Amplitude Threshold Value", 0.0) ;
	
	ParameteredObject::_addParameter (_killOutlier, "killOutlier", 
		"true: all pixels whose amplitude falls outside "
		"the given amplitude will be set to zero<br>"
		"false: flow at each pixel will be scaled down (or up for highpass)"
		"to the threshold amplitude(flow direction is preserved)", false) ;

	ParameteredObject::_addParameter (_lowpass, "lowpass",
		"true: act as a lowpass filter; pixels whose amplitude is smaller "
		"(or equal) to the threshold are left untouched.<br>"
		"false: act as a highpass filter; pixels whose amplitude is higher"
		"(or equal) to the threshold are left untouched.", true) ;
	
	ParameteredObject::_addInputSlot(_input, "input", "input image", "CImgList<T>") ;
	
	ParameteredObject::_addOutputSlot(_output, "output", 
		"output image (will have same dimensions as input image)",
		"CImgList<T>"); 
}

template <typename T>
void AmplitudeThreshold<T>::execute() {
	using namespace cimg_library ;

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const CImgList<T>& input = _input() ;
	CImgList<T>& output = _output() ;
	bool& killOutlier = _killOutlier();
	double threshold = _threshold();
	bool lowpass = _lowpass() ;
	//check if input is valid
	if(threshold < 0.0)
	{
		ParameteredObject::raise("Amplitude can not be smaller than 0; "
		"Negative thresholds make no sense");
	}
	
	cimglist_for(input,l)
	{
		if (l==0) continue;
		if	   ((input[l].width() != input[l-1].width()) 
			||	(input[l].height() != input[l-1].height())
			||	(input[l].depth()  != input[l-1].depth())
			||	(input[l].spectrum() != input[l-1].spectrum()))
		{
			ParameteredObject::raise("Input Images have inconsistent dimensions!<br>"
				"sizes must be the same along all dimensions of the "
				"input CImgList");
		}
	}

	
	output.assign(input.size(),input[0].width(), input[0].height(), input[0].depth(),input[0].spectrum());

	cimg_forXYZC(input[0],x,y,z,c)
	{
		T val = T(0.0f);
		cimglist_for(input,l)
			{
				val += (input[l](x,y,z,c)*input[l](x,y,z,c));
			}
		//length of current flow
		double flowLength = sqrt((double)val);
		
		if(flowLength < 1e-15) //prevent division by zero errors
		{
			cimglist_for(input,l)
			{
				output[l](x,y,z,c) = input[l](x,y,z,c);
			}
		}
		else if ((lowpass && flowLength > threshold) ||
				(!lowpass && flowLength < threshold))
		{
			float scaleFactor = threshold / flowLength;
			cimglist_for(input,l)
			{
				if (killOutlier==true) 
					{
						output[l](x,y,z,c) = T(0.0f);
					}
				else 
					{
						output[l](x,y,z,c) = input[l](x,y,z,c)* scaleFactor;
					}
			}
		}
		else
		{
			cimglist_for(input,l)
			{
				output[l](x,y,z,c) = input[l](x,y,z,c);
			}
		}
	}
}

#endif /* _AmplitudeThreshold_HXX_ */
