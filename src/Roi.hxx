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
/** @file Roi.hxx
 *  Implementation template class Roi.
 *
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *
 *  @date 21.04.2009
 */

#ifndef _Roi_HXX_
#define _Roi_HXX_

#include "Roi.h"

template<typename T>
void Roi<T>::_init() {
	this->_addParameter (top,    "top"   , "upper bound" , T(0));
	this->_addParameter (left,   "left"  , "left bound"  , T(0));
	this->_addParameter (bottom, "bottom", "lower bound" , T(0));
	this->_addParameter (right,  "right" , "right bound" , T(0));
	this->_addParameter (front,  "front" , "front bound" , T(0));
	this->_addParameter (back,   "back"  , "back bound"  , T(1));
	this->_addParameter (before, "before", "before bound", T(0));
	this->_addParameter (after,  "after" , "after bound" , T(1));


	_addOutputSlot(out, "out", "pointer to this roi", "Roi<T>");
	out = this;
}

template<typename T>
Roi<T>::Roi(std::string name) :
	TemplatedParameteredObject<T> ("roi", name, "Region of interest") {
	_init();
}

template<typename T>
Roi<T>::Roi(const ParameterFile& pf, std::string name) :
	TemplatedParameteredObject<T> ("roi", name, "Region of interest") {
	_init();
	this->load(pf, name);
}

template<typename T>
Roi<T>::Roi(const T& t, const T& l, const T& bo, const T& r,
            const T& f, const T& ba, const T& be, const T& af, std::string name) :
	TemplatedParameteredObject<T> ("roi", name, "Region of interest") {
	_init();
	top = t;
	left = l;
	bottom = bo;
	right = r;
	front = f;
	back = ba;
	before = be;
	after = af;
}

template<typename T>
Roi<T>::~Roi(void) {
}

template<typename T>
T Roi<T>::getWidth() const {
	return right() - left();
}

template<typename T>
T Roi<T>::getHeight() const {
	return bottom() - top();
}

template<typename T>
T Roi<T>::getDepth() const {
	return back() - front();
}

template <typename T>
T Roi<T>::getDuration() const {
	return before() - after();
}

template<typename T>
void Roi<T>::setWidth(T w) {
	right = left() + w;
}

template<typename T>
void Roi<T>::setHeight(T h) {
	bottom = top() + h;
}

template<typename T>
void Roi<T>::setDepth(T d) {
	back() = front() + d;
}

template <typename T>
void Roi<T>::setDuration(T d) {
	after() = before() + d;
}

template <typename T>
T Roi<T>::getVolume() const {
    return getDuration() ?
            getWidth()*getHeight()*getDepth()*getDuration() :
    	    getDepth() ?
                getWidth()*getHeight()*getDepth() :
                getWidth()*getHeight();
}

template <typename T>
void Roi<T>::assign(T t, T l, T bo, T r, T f, T ba, T be, T af) {
    top    = t;
    left   = l;
    bottom = bo;
    right  = r;
    front  = f;
    back   = ba;
    before = be;
    after  = af;
}

template <typename T>
void Roi<T>::operator= (const Roi<T>& rhs) {
    top    = rhs.top,
    left   = rhs.left,
    bottom = rhs.bottom,
    right  = rhs.right,
    front  = rhs.front,
    back   = rhs.back,
    before = rhs.before,
    after  = rhs.after;
}

template <typename T>
void Roi<T>::intersectionWith(const Roi<T>& rhs) {
    top    = rhs.top()    > top()    ? rhs.top()    : top();
    left   = rhs.left()   > left()   ? rhs.left()   : left();
    bottom = rhs.bottom() < bottom() ? rhs.bottom() : bottom();
    right  = rhs.right()  < right()  ? rhs.right()  : right();
    front  = rhs.front()  > front()  ? rhs.front()  : front();
    back   = rhs.back()   < back()   ? rhs.back()   : back();
    before = rhs.before() > before() ? rhs.before() : before();
    after  = rhs.after()  < after()  ? rhs.after()  : after();
}

template <typename T>
void Roi<T>::unionWith(const Roi<T>& rhs) {
    top    = rhs.top()    < top()    ? rhs.top()    : top();
    left   = rhs.left()   < left()   ? rhs.left()   : left();
    bottom = rhs.bottom() > bottom() ? rhs.bottom() : bottom();
    right  = rhs.right()  > right()  ? rhs.right()  : right();
    front  = rhs.front()  < front()  ? rhs.front()  : front();
    back   = rhs.back()   > back()   ? rhs.back()   : back();
    before = rhs.before() < before() ? rhs.before() : before();
    after  = rhs.after()  > after()  ? rhs.after()  : after();
}

template <typename T>
bool Roi<T>::isInside(T x, T y, T z, T t) const {
    return x >= left() && x < right() && y >= top() && y < bottom()
	    && z >= front() && z < back() && t >= before() && t < after();
}

template<typename T>
void Roi<T>::loadParameters(const ParameterFile& pf) {
	ParameteredObject::loadParameters(pf);

	//correct order of parameters
	if (top() > bottom()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching top and bottom appropriately." << std::endl;
		T tmp = bottom();
		bottom = top();
		top = tmp;
	}

	if (left() > right()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching left and right appropriately." << std::endl;
		T tmp = right();
		right = left();
		left = tmp;
	}

	if (front() > back()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching back and front appropriately." << std::endl;
		T tmp = back();
		back = front();
		front = tmp;
	}
	
    if (before() > after()) {
        sout << "Roi '" << ParameteredObject::getName();
        sout << "': Switching before and after appropriately." << std::endl;
        T tmp = after();
        after  = before();
        before = tmp;
    }
	
}

template<typename T>
inline std::ostream& operator<<(std::ostream& strm, const Roi<T>& roi) {
	strm << "l=x0=" << roi.left() << ",r=x1=" << roi.right();
	strm << ",t=y0=" << roi.top() << ",b=y1=" << roi.bottom();
	strm << ",f=z0=" << roi.front() << ",b=z1=" << roi.back();
    strm << ",b=t0=" << roi.before() << ",a=t1=" << roi.after();
	return strm;
}

#endif // _Roi_HXX_
