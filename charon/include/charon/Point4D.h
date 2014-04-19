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
private:
	/// coordinates of the Point4D.
	T _c[4];

public:
	T& x; ///< x coordinate (horizontal)
	T& y; ///< y coordinate (vertical)
	T& z; ///< z coordinate (3rd D)
	T& t; ///< t coordinate (time)

	/// default constructor
	Point4D() : x(_c[0]),y(_c[1]),z(_c[2]),t(_c[3]) {
		for (unsigned int i=0; i<4u; i++)
			_c[i] = T(0);
	}

	/// constructor with coordinates
	/** \param[in] rx,ry,rz,rt	initial coordinates */
	Point4D(T rx, T ry, T rz, T rt) : x(_c[0]),y(_c[1]),z(_c[2]),t(_c[3]) {
		_c[0] = rx;
		_c[1] = ry;
		_c[2] = rz;
		_c[3] = rt;
	}

	/// copy constructor
	Point4D(const Point4D<T>& rhs /**< [in] copy source */)
			: x(_c[0]),y(_c[1]),z(_c[2]),t(_c[3]) {
		*this = rhs;
	}

	/// assignment operator
	/** \param[in] rhs copy source
	 *  \returns resulting object
	 */
	Point4D& operator= (const Point4D<T>& rhs) {
		if(&rhs == this)
			return *this;
		for (unsigned int i=0; i<4u; i++)
			_c[i] = rhs._c[i];
		return *this;
	}

	/// compound addition operator
	/** \param[in] rhs second summand
	 *  \returns sum
	 */
	Point4D<T>& operator+= (const Point4D<T>& rhs) {
		for (unsigned int i=0; i<4u; i++)
			_c[i] += rhs._c[i];
		return *this;
	}

	/// compound subtraction operator
	/** \param[in] rhs subtrahend
	 *  \returns difference
	 */
	Point4D<T>& operator-= (const Point4D<T>&rhs) {
		for (unsigned int i=0; i<4u; i++)
			_c[i] -= rhs._c[i];
		return *this;
	}

	/// addition operator
	/** \param[in] rhs second summand
	 *  \returns sum
	 */
	Point4D<T>& operator+ (const Point4D<T> &rhs) const {
		return Point4D(*this) += rhs;
	}

	/// subtraction operator
	/** \param[in] rhs subtrahend
	 *  \returns difference
	 */
	Point4D<T>& operator- (const Point4D<T> &rhs) const {
		return Point4D(*this) -= rhs;
	}

	/// comparison operator
	/** \param[in] rhs values to compare with
	 *  \returns true if one of the coordinates is less than the
	 *      corresponding of rhs, the first different coordinate counts.
	 *      If *this==rhs, false is returned.
	 */
	bool operator < (const Point4D<T> &rhs) const {
		for (int i=3u; i>=0; i--) {
			if (_c[i] < rhs._c[i])
				return true;
			if (_c[i] > rhs._c[i])
				return false;
		}
		// all coordinates equal
		return false;
	}

	/// comparison operator
	/** \param[in] rhs values to compare with
	 *  \returns true if one of the coordinates is greater than the
	 *      corresponding of rhs, the first different coordinate counts.
	 *      If *this==rhs, false is returned.
	 */
	bool operator > (const Point4D<T>& rhs) const {
		return rhs < *this;
	}

	/// comparison operator
	/** \param[in] rhs values to compare with
	 *  \return true if all coordinates are equal
	 */
	bool operator== (const Point4D<T>& rhs) const {
		bool ret = true;
		for (unsigned int i=0; i<4u; i++)
			ret = ret && (_c[i]==rhs._c[i]);
		return ret;
	}

	/// comparison operator
	/** \param[in] rhs values to compare with
	 *  \returns true if one of the coordinates is less than the
	 *      corresponding of rhs, the first different coordinate counts.
	 *      If *this==rhs, true is returned.
	 */
	bool operator <= (const Point4D<T>& rhs) const {
		return !(*this > rhs);
	}

	/// comparison operator
	/** \param[in] rhs values to compare with
	 *  \returns true if one of the coordinates is greater than the
	 *      corresponding of rhs, the first different coordinate counts.
	 *      If *this==rhs, true is returned.
	 */
	bool operator >= (const Point4D<T>& rhs) const {
		return !(*this < rhs);
	}

	/// Calculate the volume relative to the origin.
	T volume() {
		T vol = 1;
		for (unsigned int i=0; i<4u; i++)
			vol *= _c[i];
		return vol;
	}

	/// Check if all coordinates are positive
	bool isPositive() const {
		bool ret = true;
		for (unsigned int i=0; i<4u; i++)
			ret = ret && (_c[i] >= T(0));
		return ret;
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
	out << "(" << rhs.x << "," << rhs.y << ","
			<< rhs.z << "," << rhs.t << ")";
	return out;
}

#endif //_POINT4D_H_
