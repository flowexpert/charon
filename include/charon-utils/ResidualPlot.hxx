/*  Copyright (C) 2012 Heidelberg Collaboratory for Image Processing

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
/**
 *  \file ResidualPlot.hxx
 *  Implementation of the class ResidualPlot.
 *  \author Michael Baron
 *  \date 11.04.2012
 */

#ifndef _RESIDUALPLOT_HXX_
#define _RESIDUALPLOT_HXX_

#include <QApplication>
#include <ResidualPlotWidget.hpp>
#include "ResidualPlot.h"

#include <charon-utils/CImg.h>

template <typename T>
ResidualPlot<T>::ResidualPlot(const std::string& name) :
	TemplatedParameteredObject<T>("ResidualPlot", name,
		"Plots the residual energy of a given input<br> "),
		_display(0),
		_exportWidget(0)
{
	ParameteredObject::_addInputSlot(
		residual, "residual",
		"residual image",
		"CImgList<T>");

	ParameteredObject::_addOutputSlot(
		_display, "display",
		"A QWidget displaying the residual plot, "
		"can for example be connected to a ArgosDisplay instance",
		"QWidget*") ;

	if(!qobject_cast<QApplication*>(qApp))
	{
		sout << "(WW) ResidualPlot::No QApplication found!\n"
			<< "(WW) ResidualPlot can only be used in a Qt GUI Application!"
			<< std::endl ;
		return ;
	}

	executionCount = 0;
}

template <typename T>
ResidualPlot<T>::~ResidualPlot()
{
	delete _exportWidget ;
}

template <typename T>
void ResidualPlot<T>::execute() {
	if(!_exportWidget)
		_exportWidget = new ResidualPlotWidget ;

	_display() = _exportWidget ;

	double residualEnergy = 0;
	const cimg_library::CImgList<T> & _residual = residual();

	cimglist_for( _residual, nn )
	cimg_forXYZC( _residual[nn], xx, yy, zz, cc )
	{
		residualEnergy += _residual[nn](xx,yy,zz,cc);
	}

	if(_display.connected() && _exportWidget)
	{	
		// (!!) energy to be calculated wrt current input (TBD)
		_exportWidget->updateResidual( executionCount, residualEnergy ) ;
		executionCount++;
		sout  << "(DD) Execution count = " << executionCount << std::endl;
	}
}

#endif /* _RESIDUALPLOT_HXX_ */

