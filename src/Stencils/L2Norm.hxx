/*  This file is part of Charon.

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
/** @file L2Norm.hxx
 *  Implementation of class L2Norm.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */


#ifndef _L2NORM_HXX_
#define _L2NORM_HXX_

#include "../Stencil.hxx"
#include "L2Norm.h"

template <class T>
L2Norm<T>::L2Norm(const std::string& name) : Stencil<T>("L2Norm", name,
			"Stencil modeling spatial smoothness using laplacian operator."),
		flowGuess(true, false)
#ifdef ROBUSTNESS
		, robustnessTerm(true,false) // optional
#endif
{
	this->_addParameter(dimensions, "dimensions", "Number of dimensions", 2u);
	this->_addParameter(pUnknowns, "unknowns", "List of unknowns");
	this->_addParameter(rhsWeight, "rhsWeight",
			"Weight of the calculated RHS (only if initial guess is given)",
			1.);
	this->_addInputSlot(flowGuess, "flowGuess",
			"Initial flow guess for rhs calculation", "CImgList<T>");
#ifdef ROBUSTNESS
		this->_addInputSlot(robustnessTerm, "robustnessTerm",
			"containing the robustness term","RobustnessTerm*");
#endif
}

template <class T>
void L2Norm<T>::execute() {
	ParameteredObject::execute();

	// Copy the unknowns from the Parameter list into the set, which was
	// inherited from the Stencil class
	std::vector<std::string>::iterator puIt;
	for(puIt=pUnknowns().begin() ; puIt!=pUnknowns().end() ; puIt++) {
		this->_unknowns.insert(*puIt);
	}

	
	switch (dimensions) {
			case 1:
				//already has the correct size
				_dataMask.assign(3,1,1,1);
				_patternMask.assign(3,1,1,1);
				_patternMask.fill(1,0,1);
				_center = Point4D<int>(1,0,0,0);
				break;

			case 2:					
				_dataMask.assign(3,3,1,1);
				_patternMask.assign(3,3,1,1);
				_patternMask.fill(
						0, 1, 0,
						1, 0, 1,
						0, 1, 0);
				_center = Point4D<int>(1,1,0,0);
				break;

			case 3:
				_dataMask.assign(3,3,3,1,0);
				_patternMask.assign(3,3,3,1,0);
				_patternMask(1,1,0) = 1;
				_patternMask(1,0,1) = 1;
				_patternMask(0,1,1) = 1;
				_patternMask(1,1,1) = 1;
				_patternMask(2,1,1) = 1;
				_patternMask(1,2,1) = 1;
				_patternMask(1,1,2) = 1;
				_center = Point4D<int>(1,1,1,0);
				break;

			case 4:
				_dataMask.assign(3,3,3,3,0);
				_patternMask.assign(3,3,3,3,0);
				_patternMask(1,1,1,0) = 1;
				_patternMask(1,1,0,1) = 1;
				_patternMask(1,0,1,1) = 1;
				_patternMask(0,1,1,1) = 1;
				_patternMask(1,1,1,1) = 1;
				_patternMask(2,1,1,1) = 1;
				_patternMask(1,2,1,1) = 1;
				_patternMask(1,1,2,1) = 1;
				_patternMask(1,1,1,2) = 1;
				_center = Point4D<int>(1,1,1,1);
				break;
				default:
					throw std::out_of_range("invalid dimensions (too large)");
				}

	
#ifdef ROBUSTNESS
	if(robustnessTerm.connected())
	{

	_gradientWeight.assign(flowGuess()[0].width(), flowGuess()[0].height(),1,1);
		double gradsum;
		int x=0;
		int y=0;
		int left=0;
		int up=0;
		
		cimg_forXY(_gradientWeight,x,y)
		{
			gradsum = 0;
			for(unsigned int i=0; i<this->pUnknowns.size(); i++)
			{
				//calculating gradient for each unknown an add

				//boundary conditions
				if(x==0)
				{
					left=0;
				}
				else
				{
					left=x-1;
				}
				if(y==0)
				{
					up=0;
				}
				else
				{
					up=y-1;
				}

				gradsum+= ( (( flowGuess()[i](x+1,y)-flowGuess()[i](left,y) ) / 2)*(( flowGuess()[i](x+1,y)-flowGuess()[i](left,y) ) / 2) );
				gradsum+= ( (( flowGuess()[i](x,y+1)-flowGuess()[i](x,up) ) / 2)*(( flowGuess()[i](x,y+1)-flowGuess()[i](x,up) )/2  ) );
			}

			_gradientWeight(x,y) = robustnessTerm()->DPsi(T(gradsum));

		}

	}

	else
#endif
	{
		// create masks in appropriate dimensions,
		// then fill mask with values and set the center
		switch (dimensions) {
		case 1:
			//already has the correct size
			_dataMask.fill(T(-1), T(2), T(-1));
			break;
		case 2:
			_dataMask.fill(
					T( 0), T(-1), T( 0),
					T(-1), T( 4), T(-1),
					T( 0), T(-1), T( 0));
			break;
		case 3:
			_dataMask(1,1,0) = T(-1);
			_dataMask(1,0,1) = T(-1);
			_dataMask(0,1,1) = T(-1);
			_dataMask(1,1,1) = T( 6);
			_dataMask(2,1,1) = T(-1);
			_dataMask(1,2,1) = T(-1);
			_dataMask(1,1,2) = T(-1);
			break;
		case 4:
			_dataMask(1,1,1,0) = T(-1);
			_dataMask(1,1,0,1) = T(-1);
			_dataMask(1,0,1,1) = T(-1);
			_dataMask(0,1,1,1) = T(-1);
			_dataMask(1,1,1,1) = T( 8);
			_dataMask(2,1,1,1) = T(-1);
			_dataMask(1,2,1,1) = T(-1);
			_dataMask(1,1,2,1) = T(-1);
			_dataMask(1,1,1,2) = T(-1);
			break;
		default:
			throw std::out_of_range("invalid dimensions (too large)");
		}
	}
		_dataMask *= this->lambda();

		// precalculate rhs values for whole image
		if (flowGuess.connected()) {

			assert(flowGuess().size() == dimensions());
			const cimg_library::CImgList<T>& flow = flowGuess();
			_rhsVals.assign(flow);
			cimglist_for(flow, kk) {
				_rhsVals[kk] = this->apply(flow, kk);
				_rhsVals[kk] *= -rhsWeight();
			}
		}
	
}

template <class T>
void L2Norm<T>::updateStencil(
		const std::string& unknown,
		const Point4D<int>& p, const int&) {

	//save point (needed for L2Norm<T>::execute())
	_point = Point4D<int>();
	double a=0;
	double b=0;
	double c=0;
	double d=0;
	double e=0;
	std::ostringstream msg;
#ifdef ROBUSTNESS
	if(robustnessTerm.connected())
		{
			switch (dimensions) {
							case 1:
								msg << __FILE__ << ":" << __LINE__ << ":\n\t";
								msg << "robustness for 1D is not implemented yet!\n\t";
								throw std::runtime_error(msg.str());

								break;

							case 2:					
								a=(_gradientWeight(_point.x-1,_point.y)+_gradientWeight(_point.x,_point.y))/2;
								b=(_gradientWeight(_point.x,_point.y)-_gradientWeight(_point.x-1,_point.y))/2;
								c=-(_gradientWeight(_point.x+1,_point.y)+2*_gradientWeight(_point.x-1,_point.y)
									+_gradientWeight(_point.x-1,_point.y)+4*_gradientWeight(_point.x,_point.y))/2;
								c=(_gradientWeight(_point.x+1,_point.y)+_gradientWeight(_point.x,_point.y))/2;
								d=(_gradientWeight(_point.x,_point.y+1)+_gradientWeight(_point.x,_point.y))/2;

								_dataMask.fill(
								T( 0), T(a), T( 0),
								T(b), T(c), T(d),
								T( 0), T(e), T( 0));
								break;

							case 3:
								msg << __FILE__ << ":" << __LINE__ << ":\n\t";
								msg << "robustness for 1D is not implemented yet!\n\t";
								throw std::runtime_error(msg.str());

								break;

							case 4:
								msg << __FILE__ << ":" << __LINE__ << ":\n\t";
								msg << "robustness for 1D is not implemented yet!\n\t";
								throw std::runtime_error(msg.str());

								break;
					
			}
		}
#endif
	// fill stencil with masks
		for(unsigned int i=0; i< this->pUnknowns.size() ; i++) {
			SubStencil<T> entry;
			if(pUnknowns[i] == unknown) {
				entry.center  = _center;
				// shared assignment (no copying of values)
				entry.data.assign(_dataMask,true);
				entry.pattern.assign(_patternMask,true);
				if(flowGuess.connected())
					this->_rhs = _rhsVals[i](p.x,p.y,p.z,p.t);
				else
					this->_rhs = T(0);
			}
			else {
				// empty substencil for other unknowns
				entry.center = Point4D<int>();
				entry.data.clear();
				entry.pattern.clear();
			}
			this->_subStencils[pUnknowns[i]] = entry;
		}
}

template <class T>
cimg_library::CImg<T> L2Norm<T>::apply(
		const cimg_library::CImgList<T>& seq,
		const unsigned int frame) const {
	if(dimensions() > 3 || dimensions() < 2)
		throw std::out_of_range(
				"invalid dimensions (apply works for 2D, 3D)");
	return seq[frame].get_convolve(_dataMask);
}

template <class T>
L2Norm<T>::~L2Norm() {}

#endif // _L2NORM_HXX_
