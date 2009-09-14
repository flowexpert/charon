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
/**
 *  @file Pixel.h
 *  defines class Point4D::Pixel
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 14.09.2009
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include <vector>
#include "Point4D.h"

/// Pixel is a data structure to save one pixel in it
template<typename T>
class Pixel : public Point4D<T>
{
protected:
	///is able to save pixel data
	T x; ///@param x		x position
	T y; ///@param y		y position
	T z; ///@param z		z position
	T t; ///@param t		Time value
	std::vector<T> intensity; ///@param intensity 		intensity at x,y,z position

public:
	void setX(const T& newX);
	const T& getX() const;
	void setY(const T& newY);
	const T& getY() const;
	void setZ(const T& newZ);
	const T& getZ() const;
	void setT(const T& newT);
	const T& getT() const;
	/**
	 *  insert a new intensity parameter at the end of the list
	 *  @param newI new intensity element
	 */
	void pushBackIntensity(const T& newI);
	/**
	 *  insert a new intensity parameter at the inseted Position
	 *  @param pos position where the new element has to be inserted
	 *  @param updatedI new intensity element
	 */
	void insertIntensity(const int pos, const T& newI);
	const std::vector<T>& getIntensity() const;
	Pixel<T>& operator= (const Pixel<T>& inPixel);
};

#endif /* PIXEL_H_ */
