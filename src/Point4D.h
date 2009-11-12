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
/** @file Point4D.h
 *  Implementation of class Point4D.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _POINT4D_H_
#define _POINT4D_H_

#include <iostream>

/// This class represents a point in four-dimensional space
template <typename T>
class Point4D
{
public:
	/// \name coordinates of the Point4D.
	//@{
	T x, y, z, t;
	//@}

	/// default constructor
	Point4D() : x(0), y(0), z(0), t(0) {
	}

	/// constructor with coordinates
	/** \param[in] x,y,z,t	initial coordinates */
	Point4D(T x, T y, T z, T t) :
		x(x), y(y), z(z), t(t)
	{
	}

	/// copy constructor
	Point4D(const Point4D<T>& rhs /**< [in] copy source */) :
		x(rhs.x), y(rhs.y), z(rhs.z), t(rhs.t)
	{
	}

	/// cast to Point4D<int>.
	operator Point4D<int>() const {
		return Point4D<int>(int(x), int(y), int(z), int(t));
	}

	/// cast to Point4D<unsigned int>.
	operator Point4D<unsigned int>() const {
		return Point4D<unsigned int>(
			(unsigned int)(std::abs(x)),
			(unsigned int)(std::abs(y)),
			(unsigned int)(std::abs(z)),
			(unsigned int)(std::abs(t))
		);
	}

	/// assignment operator
	Point4D& operator= (const Point4D<T>& rhs /**<[in] copy source*/) {
		if(&rhs == this)
			return *this;
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->t = rhs.t;
		return *this;
	}

	/// compound addition operator
	Point4D<T>& operator+= (const Point4D<T>& rhs /**<[in] second summand*/) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->t += rhs.t;
		return *this;
	}

	///compound subtraction operator
	Point4D<T>& operator-= (const Point4D<T>&rhs /**<[in] subtrahend*/) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->t -= rhs.t;
		return *this;
	}

	/// addition operator
	Point4D<T>& operator+ (const Point4D<T> &rhs /**<[in] second summand*/) const {
		return Point4D(*this) += rhs;
	}

	/// subtraction operator
	Point4D<T>& operator- (const Point4D<T> &rhs /**<[in] subtrahend*/) const {
		return Point4D(*this) -= rhs;
	}

	/// comparison operator
	bool operator < (const Point4D<T> &rhs /**<[in] values to compare with*/) const {
		if (this->t < rhs.t) {return true;} else if (this->t > rhs.t) {return false;}
		if (this->z < rhs.z) {return true;} else if (this->z > rhs.z) {return false;}
		if (this->y < rhs.y) {return true;} else if (this->y > rhs.y) {return false;}
		if (this->x < rhs.x) {return true;} else if (this->x > rhs.x) {return false;}
		// if it got this far, either x=y=z=t in which case it has to be
		// false or something fishy is going on, in which case it should also
		// be false, so not much of a control here
		return false;
	}

	/// comparison operator
	bool operator > (const Point4D<T>& rhs /**<[in] values to compare with*/) const {
		return rhs < *this;
	}

	/// comparison operator
	bool operator== (const Point4D<T>& rhs /**<[in] values to compare with*/) const {
		return ((this->x == rhs.x) && (this->y == rhs.y)
			&& (this->z == rhs.z) && (this->t == rhs.t));
	}

	/// comparison operator
	bool operator <= (const Point4D<T>& rhs /**<[in] values to compare with*/) const {
		return !(*this > rhs);
	}

	/// comparison operator
	bool operator >= (const Point4D<T>& rhs /**<[in] values to compare with*/) const {
		return !(*this < rhs);
	}

	/// Calculate the volume relative to the origin.
	T volume() {
		return x * y * z * t;
	}

	///default destructor
	~Point4D() {}
};

/// print Point4D to stream
/** \param out      output stream to print to
 *  \param rhs      point to print
 *  \return         reference to the used output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const Point4D<T>& rhs) {
	out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.t << ")";
	return out;
}

#endif //_POINT4D_H_
