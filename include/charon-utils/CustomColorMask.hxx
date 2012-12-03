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
#include <boost/assign/std/vector.hpp>

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
		widget(0),
		_gui(0)
{
	ParameteredObject::_setTags("charon-utils;CImg;Qt") ;

	ParameteredObject::_addParameter(
		begin, "begin", "start value to apply mask (lower values will be assigned zero)", "T");

	ParameteredObject::_addParameter(
		end, "end", "end value to apply mask (higher values will be assigned zero)", "T");

	ParameteredObject::_addParameter(
		minimap, "minimap", "adds a colorbar to the output image", false);

	ParameteredObject::_addParameter(maskType, "maskType", "Available masks are:<br>"
		"<u>BlackWhite<br>WhiteBlack<br>Rainbow<br>Jet<br>Custom</u> <p>(input slot \"mask\" must be connected otherwise Rainbow will be selected)</p>"
		"<p>top line of \"mask\" image will be custom mask</p><br>",
	"{BlackWhite;WhiteBlack;Rainbow;Jet;Custom}") ;

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


	//Rainbow mask
	_maskRainbow[0].assign(306,T(0.0));
	_maskRainbow[1].assign(306,T(0.0));
	_maskRainbow[2].assign(306,T(0.0));

	//calc values for bw and wb
	for(int i = 0; i < 255; ++i)
	{
		_maskSw.push_back((T)i);
		_maskWs.push_back((T)(255 - i));
	}
	
	//calc values for rainbow mask
	int xu = 0;
	int xg = 255;
	for(int i = 0; i < 51; ++i)
	{
		xu += 5;
		xg -= 5;

		int ind = i;
		_maskRainbow[0][ind] = 255;
		_maskRainbow[1][ind] = xu;
		_maskRainbow[2][ind] = 0;

		ind += 51;
		_maskRainbow[0][ind] = xg;
		_maskRainbow[1][ind] = 255;
		_maskRainbow[2][ind] = 0;

		ind += 51;
		_maskRainbow[0][ind] = 0;
		_maskRainbow[1][ind] = 255;
		_maskRainbow[2][ind] = xu;

		ind += 51;
		_maskRainbow[0][ind] = 0;
		_maskRainbow[1][ind] = xg;
		_maskRainbow[2][ind] = 255;

		ind += 51;
		_maskRainbow[0][ind] = xu;
		_maskRainbow[1][ind] = 0;
		_maskRainbow[2][ind] = 255;

		ind += 51;
		_maskRainbow[0][ind] = 255;
		_maskRainbow[1][ind] = 0;
		_maskRainbow[2][ind] = xg;
	}
	
	//matlab jet colormap
	using namespace boost::assign;
	#ifdef MSVC
		#pragma warning(push)
		#pragma warning(disable : 4305) 
	#endif /*MSVC */

	_maskJet[0] += 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.015625,0.03125,
					0.046875,0.0625,0.078125,0.09375,0.10938,0.125,0.14063,0.15625,0.17188,0.1875,
					0.20313,0.21875,0.23438,0.25,0.26563,0.28125,0.29688,0.3125,0.32813,0.34375,
					0.35938,0.375,0.39063,0.40625,0.42188,0.4375,0.45313,0.46875,0.48438,0.5,
					0.51563,0.53125,0.54688,0.5625,0.57813,0.59375,0.60938,0.625,0.64063,0.65625,
					0.67188,0.6875,0.70313,0.71875,0.73438,0.75,0.76563,0.78125,0.79688,0.8125,
					0.82813,0.84375,0.85938,0.875,0.89063,0.90625,0.92188,0.9375,0.95313,0.96875,
					0.98438,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0.98438,0.96875,
					0.95313,0.9375,0.92188,0.90625,0.89063,0.875,0.85938,0.84375,0.82813,0.8125,
					0.79688,0.78125,0.76563,0.75,0.73438,0.71875,0.70313,0.6875,0.67188,0.65625,
					0.64063,0.625,0.60938,0.59375,0.57813,0.5625,0.54688,0.53125,0.51563,0.5;

	_maskJet[1] += 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.015625,
					0.03125,0.046875,0.0625,0.078125,0.09375,0.10938,0.125,0.14063,0.15625,0.17188,
					0.1875,0.20313,0.21875,0.23438,0.25,0.26563,0.28125,0.29688,0.3125,0.32813,0.34375,
					0.35938,0.375,0.39063,0.40625,0.42188,0.4375,0.45313,0.46875,0.48438,0.5,0.51563,
					0.53125,0.54688,0.5625,0.57813,0.59375,0.60938,0.625,0.64063,0.65625,0.67188,0.6875,
					0.70313,0.71875,0.73438,0.75,0.76563,0.78125,0.79688,0.8125,0.82813,0.84375,0.85938,
					0.875,0.89063,0.90625,0.92188,0.9375,0.95313,0.96875,0.98438,1,1,1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1,0.98438,0.96875,0.95313,0.9375,0.92188,0.90625,0.89063,0.875,0.85938,
					0.84375,0.82813,0.8125,0.79688,0.78125,0.76563,0.75,0.73438,0.71875,0.70313,0.6875,
					0.67188,0.65625,0.64063,0.625,0.60938,0.59375,0.57813,0.5625,0.54688,0.53125,0.51563,
					0.5,0.48438,0.46875,0.45313,0.4375,0.42188,0.40625,0.39063,0.375,0.35938,0.34375,0.32813,
					0.3125,0.29688,0.28125,0.26563,0.25,0.23438,0.21875,0.20313,0.1875,0.17188,0.15625,0.14063,
					0.125,0.10938,0.09375,0.078125,0.0625,0.046875,0.03125,0.015625,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ;
	_maskJet[2] += 0.51563,0.53125,0.54688,0.5625,0.57813,0.59375,0.60938,0.625,0.64063,0.65625,0.67188,0.6875,
					0.70313,0.71875,0.73438,0.75,0.76563,0.78125,0.79688,0.8125,0.82813,0.84375,0.85938,0.875,
					0.89063,0.90625,0.92188,0.9375,0.95313,0.96875,0.98438,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					1,0.98438,0.96875,0.95313,0.9375,0.92188,0.90625,0.89063,0.875,0.85938,0.84375,0.82813,0.8125,
					0.79688,0.78125,0.76563,0.75,0.73438,0.71875,0.70313,0.6875,0.67188,0.65625,0.64063,0.625,
					0.60938,0.59375,0.57813,0.5625,0.54688,0.53125,0.51563,0.5,0.48438,0.46875,0.45313,0.4375,
					0.42188,0.40625,0.39063,0.375,0.35938,0.34375,0.32813,0.3125,0.29688,0.28125,0.26563,0.25,
					0.23438,0.21875,0.20313,0.1875,0.17188,0.15625,0.14063,0.125,0.10938,0.09375,0.078125,0.0625,
					0.046875,0.03125,0.015625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ;

	#ifdef MSVC
		#pragma warning (pop)
	#endif /*MSVC */

	for(size_t ii = 0 ; ii < _maskJet[0].size() ; ii++)
	{
		_maskJet[0][ii] *= 255.0 ;
		_maskJet[1][ii] *= 255.0 ;
		_maskJet[2][ii] *= 255.0 ;
	}

}

template<typename T>
CustomColorMask<T>::~CustomColorMask()
{
	delete _gui;
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
	{	sout << "Error: qApp not found" << std::endl;	}

	//no image
	if(in().size() == 0)
		ParameteredObject::raise("Empty sequence given!");
	const cimg_library::CImgList<T>& i = in();

	//bounds ok?
	double cmin;
	double cmax;
	cmax = i.get_append('c').max_min(cmin);

	if(begin() == end())
	{
		begin() = cmin ;
		end() = cmax ;
	}

	//create gui and populate it with values set in ObjectInspector
	if(!_gui)
	{
		wBegin() = begin() ;
		wEnd() = end() ;
		
		//create DockWIdget
		_gui = new CustomColorMaskWidget(this, minimap, wBegin, wEnd, maskType,this->ParameteredObject::getName());
		widget = _gui;
		std::set<Slot*> targets = widget.getTargets() ;
		if(targets.begin() != targets.end())
			_gui->setDisplay(&((*targets.begin())->getParent()));
	}
	_gui->setMinMax(cmin,cmax) ;

	//pointer for mask values
	std::vector<T>* rgbValues[3] ;

	//mask length
	unsigned int mWidth;
	//select Mask
	if(maskType() == "BlackWhite")
	{	rgbValues[0] = rgbValues[1]= rgbValues[2] = &_maskSw;
		mWidth = _maskSw.size();
	}
	else if(maskType() =="WhiteBlack")
	{	rgbValues[0] = rgbValues[1]= rgbValues[2] = &_maskWs;
		mWidth = _maskWs.size();
	}
	else if(maskType() == "Rainbow")
	{	rgbValues[0] = &_maskRainbow[0];
		rgbValues[1] = &_maskRainbow[1];
		rgbValues[2] = &_maskRainbow[2];
		mWidth = _maskRainbow[0].size() ;
	}
	else if(maskType() == "Jet")
	{	rgbValues[0] = &_maskJet[0];
		rgbValues[1] = &_maskJet[1];
		rgbValues[2] = &_maskJet[2];
		mWidth = _maskJet[0].size() ;
	}
	else if(maskType() == "Custom" && mask.connected() && mask().size() > 0 && mask()[0].spectrum() >= 3)
	{	
		_maskCustom[0].clear() ; _maskCustom[1].clear() ; _maskCustom[2].clear() ;
		const cimg_library::CImg<T>& m = mask()[0];
		mWidth = m.width();
		cimg_forX(m, x)
		{
			_maskCustom[0].push_back(m(x, 0, 0, 0));
			_maskCustom[1].push_back(m(x, 0, 0, 1));
			_maskCustom[2].push_back(m(x, 0, 0, 2));
		}
		rgbValues[0] = &_maskCustom[0];
		rgbValues[1] = &_maskCustom[1];
		rgbValues[2] = &_maskCustom[2];

	}
	else //default to BlackWhite
	{	rgbValues[0] = rgbValues[1]= rgbValues[2] = &_maskSw;
		mWidth = _maskSw.size();
	}

	const std::vector<T>& rValues = *(rgbValues[0]) ;
	const std::vector<T>& gValues = *(rgbValues[1]) ;
	const std::vector<T>& bValues = *(rgbValues[2]) ;

	//calc bounds
	T diff = wEnd()-wBegin();

	//create output
	cimg_library::CImgList<T>& o = out();

	//adding minimap
	if(minimap() == true)
	{
		o.assign(i.size(), i[0].width(), i[0].height()+20, i[0].depth(), 3u);

		for(int y = i[0].height(); y < i[0].height()+10; ++y)
			for(int x = 0; x < i[0].width(); ++x)
			{
				T val = wBegin() + (diff * x)/i[0].width();
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

				if((first >= wBegin() && first <= wEnd()) || (first >= wEnd() && first <= wBegin()))
				{
					//calc mask index
					unsigned int index = valToInd((T)wBegin, (T)wEnd, first, mWidth);
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
