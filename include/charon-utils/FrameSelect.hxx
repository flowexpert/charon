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
/** \file FrameSelect.hxx
 *  Implementation of the parameter class Threshold.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#ifndef _FRAMESELECT_HXX_
#define _FRAMESELECT_HXX_

#include <QApplication>
#include <QString>

#include "FrameSelect.h"

template <typename T>
FrameSelect<T>::FrameSelect(const std::string& name) :
		TemplatedParameteredObject<T>("frameselect", name, "select frames"),
		in(true,false), // optional
		widget(0), roi(0),
		_gui(0)
{
	ParameteredObject::_addInputSlot(in, "in",
			"image input", "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(out, "out",
			"image output", "vigraArray5<T>");
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
		sout << "FrameSelect::No QApplication found! " 
				"FrameSelect can only be used in a Qt GUI Application! "
				"(e.g. Tuchulcha)" << std::endl ;
		return ;
	}

	_gui = new FrameSelectWidget(this,cropV,z,t,v);
}

template <typename T>
FrameSelect<T>::~FrameSelect()
{
	delete _gui ;
}

template <typename T>
void FrameSelect<T>::execute() {
	widget() = _gui;
	roi() = &_roi;
	vigra::MultiArrayIndex lz = z() ;
	vigra::MultiArrayIndex lt = t() ;
	vigra::MultiArrayIndex lv = v() ;
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
		const vigra::MultiArrayView<5,T>& i = in();
		vigra::MultiArray<5,T>& o = out();
		const vigra::MultiArrayShape<5>::type& s = i.shape();
		vigra_assert(s.size() == 5, "s has != 5 elements");

		if(lz >= s[2] || lt >= s[3] || lv >= s[4])
			throw std::out_of_range("z/t/v coorinate too large");

		if(_gui && widget.connected()) {
			_gui->setDisplay(&(*widget.getTargets().begin())->getParent());
			_gui->setTitle(ParameteredObject::getName());
			_gui->setShape(s[2],s[3],s[4]);
		}

		vigra_assert(cV||s[4]==3u,"last dim has to be 3 if not cropping");
		vigra::MultiArrayShape<5>::type os(s[0],s[1],1u,1u,(cV?1u:3u));
		o.reshape(os);
		const uint zz=lz;
		const uint tt=lt;
		vigra::MultiArrayIndex vBegin = cV?lv:0u, vEnd = cV?lv+1:3u;

		for(vigra::MultiArrayIndex vv=vBegin;vv<vEnd;vv++)
			for(vigra::MultiArrayIndex yy=0u;yy<s[1];yy++)
				for(vigra::MultiArrayIndex xx=0u;xx<s[0];xx++)
					o(xx,yy,0u,0u,cV?0u:vv) = i(xx,yy,zz,tt,vv);
	}
	else {
		sout << "\tnot cropping anything since in not connected" << std::endl;
	}
	if(_gui && widget.connected())
		_gui->updateWidget() ;
}
#endif // _FRAMESELECT_HXX_
