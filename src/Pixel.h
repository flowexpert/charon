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
 *  @brief declaration of class Pixel, child class of Point4D
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 14.09.2009
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include <vector>
#include "Point4D.h"

/// Pixel is a data structure to save one pixel
template<typename T>
class Pixel : public Point4D<T>
{
protected:
	///is able to save pixel data
	T x; ///@param x	x position
	T y; ///@param y	y position
	T z; ///@param z	z position
	T t; ///@param t	Time value
	std::vector<T> intensity; ///@param intensity 	intensity at x,y,z,t position

public:
	/// standard constructor
	Pixel();
	/// standard set method for parameter x
	void setX(const T& newX);
	/// standard get function for parameter x
	const T& getX() const;
	/// standard set method for parameter y
	void setY(const T& newY);
	/// standard get function for parameter y
	const T& getY() const;
	/// standard set method for parameter z
	void setZ(const T& newZ);
	/// standard get function for parameter z
	const T& getZ() const;
	/// standard set method for parameter t
	void setT(const T& newT);
	/// standard get function for parameter t
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
	/// standard get function for parameter intensity
	const std::vector<T>& getIntensity() const;
	/// copy constructor
	Pixel(const Pixel<T> & rhs);
	/// overloaded operator ==
	bool operator== (const Pixel<T> &rhs) const;
	/// overloaded operator =
	Pixel<T>& operator= (const Pixel<T>& rhs);
};

#endif /* PIXEL_H_ */
