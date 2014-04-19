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
/** \file FrameSelectCImg.hxx
 *  Implementation of the parameter class Threshold.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef _FRAMESELECTCIMG_HXX_
#define _FRAMESELECTCIMG_HXX_

#include <QApplication>
#include <QString>

#include "FrameSelectCImg.h"
#include "Roi.hxx"

template <typename T>
FrameSelectCImg<T>::FrameSelectCImg(const std::string& name) :
		TemplatedParameteredObject<T>("FrameSelectCImg", name, 
		"select single frame from a multidimensional image "
		"by specifying the z,t and c coordinate<br>"
		"Behaves exactly as FrameSelect but for CImgLists"),
		in(true,false), // optional
		widget(0), roi(0),
		_gui(0)
{
	ParameteredObject::_setTags("charon-utils;CImg;Qt") ;

	ParameteredObject::_addInputSlot(in, "in",
			"image input", "CImgList<T>");
	ParameteredObject::_addOutputSlot(out, "out",
			"image output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(widget, "widget",
			"QWidget to be displayed in ArgosDisplay", "QWidget*");
	ParameteredObject::_addOutputSlot(roi, "roi",
			"Crop border output", "Roi<int>*");
	ParameteredObject::_addParameter(z, "z", "select z slice",uint(0),"");
	ParameteredObject::_addParameter(t, "t", "select t slice", uint(0),"");
	ParameteredObject::_addParameter(v, "v", "select v slice", uint(0),"");
	ParameteredObject::_addParameter(cropV, "cropV",
			"enable cropping last (RGB) dimension", true);

	if(!qApp)
	{
		sout << "FrameSelectCImg::No QApplication found! " 
				"FrameSelectCImg can only be used in a Qt GUI Application! "
				"(e.g. Tuchulcha)" << std::endl ;
		return ;
	}

}

template <typename T>
FrameSelectCImg<T>::~FrameSelectCImg()
{
	delete _gui ;
}

template <typename T>
void FrameSelectCImg<T>::execute() {
	if(!_gui)
		_gui = new FrameSelectWidget(this,cropV,z,t,v);

	widget() = _gui;
	roi() = &_roi;
	int lz = z() ;
	int lt = t() ;
	int lv = v() ;
	bool cV = cropV() ;

	// update roi boundaries
	_roi.xEnd =  _roi.xBegin = 0; // no crop in x-dim
	_roi.yEnd =  _roi.yBegin = 0; // no crop in y-dim
	_roi.zEnd = (_roi.zBegin = lz) + 1;
	_roi.tEnd = (_roi.tBegin = lt) + 1;
	_roi.vEnd = (_roi.vBegin = lv) + 1;
	if(!cV)
		_roi.vEnd = _roi.vBegin = 0; // no crop in v-dim

	// crop input if neccessary
	if(in.connected()) {
		const cimg_library::CImgList<T>& i = in();
		cimg_library::CImgList<T>& o = out();

		if(i.width() == 0 || lv >= i.width() || lz >= i[0].depth() || lt >= i[0].spectrum())
			ParameteredObject::raise("z/t/v coorinate too large");

		if(_gui && widget.connected()) {
			_gui->setDisplay(&(*widget.getTargets().begin())->getParent());
			_gui->setTitle(ParameteredObject::getName());
			_gui->setShape(i[0].depth(),i[0].spectrum(),i.width());
		}

		if(!cV && i.width()!=3u)
		{	ParameteredObject::raise("last dim has to be 3 if not cropping");}
		lv = cV?lv:0 ;
		
		int w = i[lv].width() ;
		int h = i[lv].height() ;
		//int d = i[lv].depth() ;

		o.assign(cV?1:3,w, h,1,1) ;

		if(cV)
		{
			memmove(o[0].data(),&i[lv](0,0,lz,lt), size_t(w*h*sizeof(T))) ;
		}
		else
		{
			memmove(o[0].data(),&(i[0](0,0,lz,lt)),size_t(w*h*sizeof(T))) ;
			memmove(o[1].data(),&(i[1](0,0,lz,lt)),size_t(w*h*sizeof(T))) ;
			memmove(o[2].data(),&(i[2](0,0,lz,lt)),size_t(w*h*sizeof(T))) ;
		}
	}
	else {
		sout << "\tnot cropping anything since in not connected" << std::endl;
	}
	if(_gui && widget.connected())
		_gui->updateWidget() ;
}
#endif // _FRAMESELECTCIMG_HXX_
