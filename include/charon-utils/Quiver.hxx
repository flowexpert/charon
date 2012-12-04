/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

	This file is part of charon-free.

	charon-free is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	charon-free is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with charon-free.  If not, see <http://www.gnu.org/licenses/>.
*/
/// \file Quiver.hxx
/// Implementation of the parameter class Quiver.
/// \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
///     Michael Baron</a>
/// \date 23.04.2012

#ifndef QUIVER_HXX_
#define QUIVER_HXX_

#include "Quiver.h"
#include <charon-core/ParameteredObject.hxx>

#include <mgl2/data.h>

template<typename T>
Quiver<T>::Quiver(const std::string& name) :
	TemplatedParameteredObject<T>("Quiver", name,
		"Plot flow quiver"),
	_display(0),
	quiverPlot(0)
{
	ParameteredObject::_setTags("charon-utils;CImg;Visualization;Qt") ;

	ParameteredObject::_addInputSlot(motion, "motion",
		"motion", "CImgList<T>");
	ParameteredObject::_addOutputSlot(_display, "display",
		"flow quiver display", "QWidget*");
	ParameteredObject::_addOutputSlot(quiverImage, "quiverImage",
		"quiver image", "CImgList<T>");
	ParameteredObject::_addParameter(stepWidth, "stepWidth",
		"step width", "unsigned int");

}

template<typename T>
Quiver<T>::~Quiver()
{
	delete quiverPlot ;
}

template<typename T>
void Quiver<T>::execute()
{

	if(!quiverPlot)
		quiverPlot = new QuiverPlot;


	hasWidget = false;
	if (_display.connected()) hasWidget = true;

	_display() = quiverPlot;

	const cimg_library::CImgList<T> &_motion = motion();

	int _imWidth = _motion[0].width();
	int _imHeight = _motion[0].height();

	int _stepWidth = stepWidth();
	if (_stepWidth == 0) _stepWidth = 1;

	int _width = (_imWidth + _stepWidth - 1) / _stepWidth;
	int _height = (_imHeight + _stepWidth - 1) / _stepWidth;

	quiverPlot->draw.a.Create( _width, _height );
	quiverPlot->draw.b.Create( _width, _height );

	int idx, x, y;
	cimg_forXY( _motion[0], imX, imY )
	{
		if (imX % _stepWidth) continue;
		if (imY % _stepWidth) continue;

		x = imX / _stepWidth;
		y = imY / _stepWidth;

		idx = x+_width*(_height-y-1);  //  put origin top-left
		quiverPlot->draw.a.a[idx] = _motion[0]( imX, imY, 0, 0 );
		quiverPlot->draw.b.a[idx] = -_motion[1]( imX, imY, 0, 0 );
	}

	quiverPlot->setSize(_imWidth,_imHeight);
	quiverPlot->updatePlot( hasWidget );
	QPixmap pix = quiverPlot->getPixmap();
	QImage img = pix.toImage();

	cimg_library::CImgList<T> _quiverImage(1,_imWidth,_imHeight,1,3);
	cimg_forXYZC(_quiverImage[0],x,y,z,c)
	{
		switch (c) {
		case 0:
			_quiverImage[0](x,y,z,0) = qRed(img.pixel(x,y));
			break;
		case 1:
			_quiverImage[0](x,y,z,1) = qGreen(img.pixel(x,y));
			break;
		case 2:
			_quiverImage[0](x,y,z,2) = qBlue(img.pixel(x,y));
			break;
		}
	}
	quiverImage() = _quiverImage;
}

#endif

