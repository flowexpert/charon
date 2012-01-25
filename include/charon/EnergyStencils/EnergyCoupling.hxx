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
/** \file EnergyCoupling.hxx
 *  Implementation of the parameter class EnergyCoupling.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 24.11.2011
 */

#ifndef _ENERGYCOUPLING_HXX_
#define _ENERGYCOUPLING_HXX_

#include "EnergyCoupling.h"

#include <charon/EnergyStencil.hxx>

template <class T>
EnergyCoupling<T>::EnergyCoupling(const std::string& name) :
  EnergyStencil<T>(
	     "EnergyCoupling", name,
	     "<h2>Coupling of two motion fields</h2>."
	     )
{
        this->_addInputSlot(firstMotionUV,
                            "firstMotionUV",
                            "first flow field",
                            "CImgList<T>");
        this->_addInputSlot(secondMotionUV,
                            "secondMotionUV",
                            "second flow field",
                            "CImgList<T>");
  ParameteredObject::_addParameter< T >(temp,
                      "temp",
                      "temperature",
                      1, "T");
}

template <class T>
void EnergyCoupling<T>::execute() {
  PARAMETEREDOBJECT_AVOID_REEXECUTION;
  ParameteredObject::execute();

  _lamb = this->lambda();
}

template <class T>
T EnergyCoupling<T>::getEnergy( int, int xI, int yI, int zI, int )
{
	T energy, u1, u2, v1, v2;
	T temperature;

        u1 = firstMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
        u2 = firstMotionUV().atNXYZC( 1, xI, yI, zI, 0 );
        v1 = secondMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
        v2 = secondMotionUV().atNXYZC( 1, xI, yI, zI, 0 );

        energy = (u1 - v1) * (u1 - v1) + (u2 - v2) * (u2 - v2);

	temperature = temp();

	return T(_lamb * (energy/(2*temperature)));
}

template <class T>
std::vector<T> EnergyCoupling<T>::getEnergyGradient( int, int xI, int yI, int zI, int )
{
	std::vector<T> ret( 2, T(0.0) );
	T pixelGradientU, pixelGradientV, u1, u2, v1, v2;
	T temperature;

        u1 = firstMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
        u2 = firstMotionUV().atNXYZC( 1, xI, yI, zI, 0 );
        v1 = secondMotionUV().atNXYZC( 0, xI, yI, zI, 0 );
        v2 = secondMotionUV().atNXYZC( 1, xI, yI, zI, 0 );

	pixelGradientU = u1 - v1;
	pixelGradientV = u2 - v2;

	temperature = temp();

	ret[0] = T(_lamb * (pixelGradientU/temperature));
	ret[1] = T(_lamb * (pixelGradientV/temperature));

        return ret;
}

template <class T>
int EnergyCoupling<T>::getGradientComponentsCnt() { return 2; }

template <class T>
EnergyCoupling<T>::~EnergyCoupling()
{
}

#endif /* _ENERGYCOUPLING_HXX_ */

