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
/** @file Point4D.hxx
 *  Implementation of class Point4D.
 *  This object represents a point in four-dimensional space.
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _POINT4D_H_
#define _POINT4D_H_

template <class T>
class Point4D
{
	public:
		/**
		 * x, y, z and t coordinates of the Point4D.
		 */
		T x, y, z, t;
		
		///default constructor.
		Point4D() {
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->t = 0;
		}		
		
		///constructor with coordinates.
		Point4D(T x, T y, T z, T t) : x(x), y(y), z(z), t(t) {}
		
		///copy constructor.
		Point4D(const Point4D<T> &rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->t = rhs.t;
		}
		
		///cast to Point4D<int>.
		operator Point4D<int>() {
			Point4D<int> res;
			res.x = (int)this->x;
			res.y = (int)this->y;
			res.z = (int)this->z;
			res.t = (int)this->t;
			return res;
		}
		
		///cast to Point4D<unsigned int>.
		operator Point4D<unsigned int>() {
			Point4D<unsigned int> res;
			res.x = (unsigned int)abs(this->x);
			res.y = (unsigned int)abs(this->y);
			res.z = (unsigned int)abs(this->z);
			res.t = (unsigned int)abs(this->t);
			return res;
		}
				
		///assignment operator
		Point4D& operator= (const Point4D<T> &rhs) {
			if(&rhs == this) {return *this;}
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->t = rhs.t;
			return *this;
		}
		
		///compound addition operator
		Point4D<T>& operator+= (const Point4D<T> &rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->t += rhs.t;
			return *this;
		}
		
		///compound subtraction operator
		Point4D<T>& operator-= (const Point4D<T> &rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->t -= rhs.t;
			return *this;
		}
		
		///addition operator
		Point4D<T>& operator+ (const Point4D<T> &rhs) const {
			return Point4D(*this) += rhs;
		}
		
		///subtraction operator
		Point4D<T>& operator- (const Point4D<T> &rhs) const {
			return Point4D(*this) -= rhs;
		}
		
		///comparison operator
		bool operator< (const Point4D<T> &rhs) const {
			if (this->t < rhs.t) {return true;} else if (this->t > rhs.t) {return false;}
			if (this->z < rhs.z) {return true;} else if (this->z > rhs.z) {return false;}
			if (this->y < rhs.y) {return true;} else if (this->y > rhs.y) {return false;}
			if (this->x < rhs.x) {return true;} else if (this->x > rhs.x) {return false;}
			//if it got this far, either x=y=z=t in which case it has to be
			//false or something fishy is going on, in which case it should also
			//be false, so not much of a control here
			return false;
		}
		
		///comparison operator
		bool operator> (const Point4D<T> &rhs) const {
			if (this->t > rhs.t) {return true;} else if (this->t < rhs.t) {return false;}
			if (this->z > rhs.z) {return true;} else if (this->z < rhs.z) {return false;}
			if (this->y > rhs.y) {return true;} else if (this->y < rhs.y) {return false;}
			if (this->x > rhs.x) {return true;} else if (this->x < rhs.x) {return false;}
			//if it got this far, either x=y=z=t in which case it has to be
			//false or something fishy is going on, in which case it should also
			//be false, so not much of a control here
			return false;
		}
		
		///comparison operator
		bool operator== (const Point4D<T> &rhs) const {
			return ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z) && (this->t == rhs.t));
		}
		
		///comparison operator
		bool operator<= (const Point4D<T> &rhs) const {
			return (*this < rhs || *this == rhs);
		}
		
		///comparison operator
		bool operator>= (const Point4D<T> &rhs) const {
			return (*this > rhs || *this == rhs);
		}
		
		//*
		 * Calculate the volume relative to the origin.
		 */
		T volume() {
			return x * y * z * t;
		}
		
		///default destructor.
		~Point4D() {}
};

#endif //_POINT4D_H_