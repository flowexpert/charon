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
/** @file Newton2.hxx
 *  Implementation of class Newton2.
 *  @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  @date 06.02.2012
 */

#ifndef _NEWTON2_HXX_
#define _NEWTON2_HXX_

#include "Newton2.h"

#include <charon-core/ParameteredObject.hxx>
#include <vector>

template <typename T>
Newton2<T>::Newton2(const std::string& name) : 
		TemplatedParameteredObject<T>("Newton2", name),
                stencils(false,true)
{
	this->_addInputSlot(stencils,
	                    "stencils",
	                    "stencils",
	                    "Stencil<T>*");
	this->_addInputSlot(roi, "roi", "region of interest", "Roi<int>*");
        this->_addOutputSlot(result,
                             "result",
                             "result",
                             "CImgList<T>");
}

template <typename T>
void Newton2<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	const Roi<int>& _roi = *(this->roi());

	cimg_library::CImgList<T> _result(2,
	                                  _roi.xEnd - _roi.xBegin,
	                                  _roi.yEnd - _roi.yBegin,
	                                  _roi.zEnd - _roi.zBegin,
	                                  1, T(0.0));

	std::vector<T> tmp2( 2, T(0.0) );
	std::vector<T> tmp4( 4, T(0.0) );
	std::vector<T> localGradient( 2, T(0.0) );
	std::vector<T> localHessian( 4, T(0.0) );
	cimg_library::CImg<T> matHessian( 2, 2, 1, 1, T(0.0) );
	cimg_library::CImg<T> vecGradient( 1, 2, 1, 1, T(0.0) );
	cimg_library::CImg<T> vecSolution( 1, 2, 1, 1, T(0.0) );

	int x, y, z;
	Stencil::Base<T> *is;
	typename std::set<AbstractSlot<Stencil::Base<T>*>*>::const_iterator sIt;
	for (x=_roi.xBegin; x<_roi.xEnd; ++x)
	for (y=_roi.yBegin; y<_roi.yEnd; ++y)
	for (z=_roi.zBegin; z<_roi.zEnd; ++z)
	{
		localGradient[0] = T(0.0);
		localGradient[1] = T(0.0);
		localHessian[0] = T(0.0);
		localHessian[1] = T(0.0);
		localHessian[2] = T(0.0);
		localHessian[3] = T(0.0);
		for (sIt =  this->stencils.begin();
		     sIt != this->stencils.end();
		     sIt ++)
		{
			is = (*((InputSlot<Stencil::Base<T>*>*)*sIt))();
			tmp2 = dynamic_cast<Stencil::EnergyGradient<T>*>(is)->getEnergyGradient( 0, x, y, z, 0 );
			localGradient[0] += tmp2[0];
			localGradient[1] += tmp2[1];
			tmp4 = dynamic_cast<Stencil::EnergyHessian<T>*>(is)->getEnergyHessian( 0, x, y, z, 0 );
			localHessian[0]  += tmp4[0];
			localHessian[1]  += tmp4[1];
			localHessian[2]  += tmp4[2];
			localHessian[3]  += tmp4[3];
		}
		vecGradient(0,0,0,0) = localGradient[0];
		vecGradient(0,1,0,0) = localGradient[1];
		matHessian(0,0,0,0) = localHessian[0];
		matHessian(0,1,0,0) = localHessian[1];
		matHessian(1,0,0,0) = localHessian[2];
		matHessian(1,1,0,0) = localHessian[3];

		vecSolution = vecGradient.solve( matHessian );
		_result( 0, x, y, z, 0 ) = -vecSolution(0,0,0,0);
		_result( 1, x, y, z, 0 ) = -vecSolution(0,1,0,0);
	}

	result() = _result;
}

#endif // _NEWTON2_HXX_

