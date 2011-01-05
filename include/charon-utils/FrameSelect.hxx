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

#ifndef _CROP_HXX_
#define _CROP_HXX_

#include "FrameSelect.h"
#include <QString>

template <typename T>
FrameSelect<T>::FrameSelect(const std::string& name) :
		TemplatedParameteredObject<T>("frameselect", name, "select frames"),
		_gui(0)
{
	ParameteredObject::_addInputSlot(in, "in",
			"image input", "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(out, "out",
			"image output", "vigraArray5<T>");
	ParameteredObject::_addOutputSlot(widget, "widget",
			"QWidget to be displayed in ArgosDisplay", "QWidget*");
	ParameteredObject::_addParameter(z, "z", "select z slice");
	ParameteredObject::_addParameter(t, "t", "select t slice");
	ParameteredObject::_addParameter(v, "v", "select v slice");
	ParameteredObject::_addParameter(cropV, "cropV",
			"enable cropping last (RGB) dimension", true);
}

template <typename T>
void FrameSelect<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const vigra::MultiArrayView<5,T>& i = in();
	vigra::MultiArray<5,T>& o = out();
	const vigra::MultiArrayShape<5>::type& s = i.shape();
	vigra_assert(s.size() == 5, "s has != 5 elements");

	if(z() >= s[2] || t() >= s[3] || v() >= s[4])
		throw std::out_of_range("z/t/v coorinate too large");

	if(!_gui)
		_gui = new FrameSelectWidget(this,cropV,z,t,v);
	_gui->setDisplay(&(*widget.getTargets().begin())->getParent());
	_gui->setWindowTitle(QString("FrameSelect [%1]").arg(
			ParameteredObject::getName().c_str()));
	_gui->setShape(s[2],s[3],s[4]);
	widget = _gui;

	const bool& cV = cropV();
	vigra_assert(cV||s[4]==3u,"last dim has to be 3 if not cropping");
	vigra::MultiArrayShape<5>::type os(s[0],s[1],1u,1u,(cV?1u:3u));
	o.reshape(os);
	const uint& zz=z();
	const uint& tt=t();
	uint vBegin = cV?v():0u, vEnd = cV?v()+1:3u;

	for(uint vv=vBegin;vv<vEnd;vv++)
		for(uint yy=0u;yy<s[1];yy++)
			for(uint xx=0u;xx<s[0];xx++)
				o(xx,yy,0u,0u,cV?0u:vv) = i(xx,yy,zz,tt,vv);
}
#endif // _CROP_HXX_
