/*  Copyright (C) 2012 Niels Buwen

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
/** \file CustomColorMask.hxx
 *  Implementation of the parameter class CustomColorMask.
 *  \author <a href="mailto:niels.buwen@web.de">
 *      Niels Buwen</a>
 *  \date 27.04.2012
 */

#ifndef _CUSTOMCOLORMASK_HXX_
#define _CUSTOMCOLORMASK_HXX_

#include <CustomColorMaskWidget.hpp>
#include <QApplication>
#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CustomColorMask.h>
#include <list>

template <typename T>
CustomColorMask<T>::CustomColorMask(const std::string& name) :
		TemplatedParameteredObject<T>(
			"CustomColorMask", name,
			"<h2>applies color maps to an image</h2><br>"
			"Select a given color map or load an image as a custom color map.<br>"
			"Input values outside of the selected range will be set to zero (black)"
		),
		in(false, false),
		mask(true, false),
		_gui(0)
{
	ParameteredObject::_addParameter(
		begin, "begin", "start value to apply mask (lower values will be assigned zero)", "T");

	ParameteredObject::_addParameter(
		end, "end", "end value to apply mask (higher values will be assigned zero)", "T");

	ParameteredObject::_addParameter(
		minimap, "minimap", "adds a colorbar to the output image", false);

	ParameteredObject::_addParameter(maskIndex, "maskType", "Available masks are:<br>"
		"<u>BlackWhite<br>WhiteBlack<br>Rainbow<br>Custom</u> <p>(input slot \"mask\" must be connected otherwise Rainbow will be selected)</p>"
		"<p>top line of \"mask\" image will be custom mask</p><br>",
	"{BlackWhite;WhiteBlack;Rainbow;Custom}") ;

	ParameteredObject::_addInputSlot(
		in, "in",
		"input image",
		"CImgList<T>");
	ParameteredObject::_addInputSlot(
		mask, "mask",
		"costum color map<br>"
		"The color of the leftmost pixel in line 1 will be assigned to the minimum value of the input image<br>"
		"The color of the rightmost pixel will be assigned to the maximum input value accordingly",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		out, "out",
		"output image",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(widget, "widget",
		"QWidget to be displayed in ArgosDisplay", "QWidget*");
	widget = 0;
	maskType = 0;

	_updateQt = true;
	
	//BlackWhite mask
	_maskSw = new T[255];
	//WhiteBlack mask
	_maskWs = new T[255];
	//Rainbow mask
	_maskRainbowR = new T[306];
	_maskRainbowG = new T[306];
	_maskRainbowB = new T[306];

	//calc values for bw and wb
	for(int i = 0; i < 255; ++i)
	{
		_maskSw[i] = (T)i;
		_maskWs[i] = (T)(255 - i);
	}
	
	//calc values for rainbow mask
	int xu = 0;
	int xg = 255;
	for(int i = 0; i < 51; ++i)
	{
		xu += 5;
		xg -= 5;

		int ind = i;
		_maskRainbowR[ind] = 255;
		_maskRainbowG[ind] = xu;
		_maskRainbowB[ind] = 0;

		ind += 51;
		_maskRainbowR[ind] = xg;
		_maskRainbowG[ind] = 255;
		_maskRainbowB[ind] = 0;

		ind += 51;
		_maskRainbowR[ind] = 0;
		_maskRainbowG[ind] = 255;
		_maskRainbowB[ind] = xu;

		ind += 51;
		_maskRainbowR[ind] = 0;
		_maskRainbowG[ind] = xg;
		_maskRainbowB[ind] = 255;

		ind += 51;
		_maskRainbowR[ind] = xu;
		_maskRainbowG[ind] = 0;
		_maskRainbowB[ind] = 255;

		ind += 51;
		_maskRainbowR[ind] = 255;
		_maskRainbowG[ind] = 0;
		_maskRainbowB[ind] = xg;
	}
	

}

template<typename T>
CustomColorMask<T>::~CustomColorMask()
{
	delete _gui;
	delete[] _maskRainbowR;
	delete[] _maskRainbowG;
	delete[] _maskRainbowB;

	delete[] _maskSw;

	delete[] _maskWs;
}

//calc red value to index
template <typename T>
unsigned int valToInd(T start, T end, T value, unsigned int w)
{	
	T diff = end -  start;

	T index = (value - start);
	return (unsigned int)(index * (w-1) / diff);
}

template <typename T>
void CustomColorMask<T>::execute() {
	
	if(!qApp)
	{
		sout << "Error: qApp not found" << std::endl;
	}
	else if(_gui == NULL)
	{
		//create DockWIdget
		wBegin() = begin();
		wEnd() = end();
		_gui = new CustomColorMaskWidget(this, minimap, wBegin, wEnd, maskType, QString(ParameteredObject::getName().c_str()));
		widget = _gui;
	}

	//no image
	if(in().size() == 0)
		ParameteredObject::raise("Empty sequence given!");
	const cimg_library::CImgList<T>& i = in();

	//assign widget values
	if(_gui && widget.connected())
	{	
		begin() = wBegin();
		end() = wEnd();
	}


	//bounds ok?
	double cmin;
	double cmax;
	cmax = i.get_append('c').max_min(cmin);
	// if 0 0 set min max
	if(end == 0 && begin == 0)
	{
		begin = cmin;
		end = cmax;
	}
	else if(end() == begin())
	{
		return;
	}
	else if(end() < begin())
	{
		return;
	}

	//Setting Display for Widget and adjusting min, max, minimap and masktype
	if(_updateQt && _gui && widget.connected())
		{
			_gui->setDisplay(&(*widget.getTargets().begin())->getParent());
			_gui->setMinMax(cmin, cmax, minimap(), (mask.size() > 0));
		}
	// no gui => select by input Parameter
	if(!_gui || !widget.connected())
	{
		if(maskIndex() == "BlackWhite")
			maskType = 0;
		if(maskIndex() == "WhiteBlack")
			maskType = 1;
		if(maskIndex() == "Rainbow")
			maskType = 2;
		if(maskIndex() == "Custom")
        {
            if(mask.size() == 0)
				maskType = 2;
			else
				maskType = 3;
        }
	}
	_updateQt = false;

	//pointer for mask values
	T* rValues;
	T* gValues;
	T* bValues;

	//mask length
	unsigned int mWidth;

	//select Mask
	switch(maskType)
	{
	case 0:		//SW
		rValues = _maskSw;
		gValues = _maskSw;
		bValues = _maskSw;
		mWidth = 255;
		break;
	case 1:		//WS
		rValues = _maskWs;
		gValues = _maskWs;
		bValues = _maskWs;
		mWidth = 255;
		break;
	case 2:		//Rainbow
		rValues = _maskRainbowR;
		gValues = _maskRainbowG;
		bValues = _maskRainbowB;
		mWidth = 306;
		break;
	case 3:		//Custom
		const cimg_library::CImgList<T>& m = mask();
		mWidth = m[0].width();
		rValues = new T[mWidth];
		gValues = new T[mWidth];
		bValues = new T[mWidth];
		cimg_forX(m[0], x)
		{
			rValues[x] = m[0](x, 0, 0, 0);
			gValues[x] = m[0](x, 0, 0, 1);
			bValues[x] = m[0](x, 0, 0, 2);
		}
		break;
	}

	//calc bounds
	T diff = end-begin;

	//create output
	cimg_library::CImgList<T>& o = out();

	//adding minimap
	if(minimap() == true)
	{
		o.assign(i.size(), i[0].width(), i[0].height()+20, i[0].depth(), 3u);

		for(int y = i[0].height(); y < i[0].height()+10; ++y)
			for(int x = 0; x < i[0].width(); ++x)
			{
				T val = begin() + (diff * x)/i[0].width();
				o[0](x, y, 0, 0) = val;
				o[0](x, y, 0, 1) = val;
				o[0](x, y, 0, 2) = val;

				unsigned int index = mWidth * x / i[0].width();
				o[0](x, y+10, 0, 0) = rValues[index];
				o[0](x, y+10, 0, 1) = gValues[index];
				o[0](x, y+10, 0, 2) = bValues[index];
			}
	}	
	else
	{
		o.assign(i.size(), i[0].width(), i[0].height(), i[0].depth(), 3u);
	}
		
	//applying mask


	int w = i[0].width();
	int h = i[0].height();
	int d = i[0].depth();


	for(int z = 0; z < d; ++z)
	{
		#pragma omp parallel for
		for(int y = 0; y < h; ++y)
		{

			for(int x = 0; x < w; ++x)
			{
				//red value
				T first = i[0](x,y,z,0);

				if((first >= begin() && first <= end()) || (first >= end() && first <= begin()))
				{
					//calc mask index
					unsigned int index = valToInd((T)begin, (T)end, first, mWidth);
					//apply mask
					o[0](x,y,z,0) = rValues[index];
					o[0](x,y,z,1) = gValues[index];
					o[0](x,y,z,2) = bValues[index];
				}
				else
				{
					o[0](x, y, z, 0) = 0;
					o[0](x, y, z, 1) = 0;
					o[0](x, y, z, 2) = 0;
				}
			}
		}
	}
	
}

#endif /* _CUSTOMCOLORMASK_HXX_ */
