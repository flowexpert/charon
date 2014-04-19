/*  This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
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
#include <algorithm>

template <typename T>
Roi<T> Roi<T>::createCopy(const Roi<T>& rhs) const {
	return Roi<T>(rhs);
}

template <typename T>
void Roi<T>::_init() {
	ParameteredObject::_setTags("charon-utils") ;
	
	// register parameters and slots
	ParameteredObject::_addParameter<T>(xBegin, "xBegin",
		"upper bound", T(0), "T");
	ParameteredObject::_addParameter<T>(xEnd, "xEnd" ,
		"right bound" , T(1), "T");
	ParameteredObject::_addParameter<T>(yBegin, "yBegin",
		"left bound", T(0), "T");
	ParameteredObject::_addParameter<T>(yEnd, "yEnd",
		"lower bound" , T(1), "T");
	ParameteredObject::_addParameter<T>(zBegin, "zBegin",
		"front bound", T(0), "T");
	ParameteredObject::_addParameter<T>(zEnd, "zEnd",
		"back bound"  , T(1), "T");
	ParameteredObject::_addParameter<T>(tBegin, "tBegin",
		"lower time bound", T(0), "T");
	ParameteredObject::_addParameter<T>(tEnd, "tEnd",
		"upper time bound" , T(1), "T");
	ParameteredObject::_addParameter<T>(vBegin, "vBegin",
		"lower channel bound", T(0), "T");
	ParameteredObject::_addParameter<T>(vEnd, "vEnd",
		"upper channel bound", T(1), "T");

	ParameteredObject::_addOutputSlot(out, "out",
		"pointer to this roi", "Roi<T>*");

	// add export symbols
	_addFunction(Roi<T>::getWidth);
	_addFunction(Roi<T>::getHeight);
	_addFunction(Roi<T>::getDepth);
	_addFunction(Roi<T>::getDuration);
	_addFunction(Roi<T>::getChannels);
	_addFunction(Roi<T>::setWidth);
	_addFunction(Roi<T>::setHeight);
	_addFunction(Roi<T>::setDepth);
	_addFunction(Roi<T>::setDuration);
	_addFunction(Roi<T>::setChannels);
	_addFunction(Roi<T>::getVolume);
	_addFunction(Roi<T>::isInside);
	_addFunction(Roi<T>::assign);
	_addFunction(Roi<T>::operator =);
	_addFunction(Roi<T>::print);
	_addFunction(Roi<T>::createCopy);
}

template <typename T>
Roi<T>::Roi(std::string name) :
TemplatedParameteredObject<T> ("Roi", name, "Region of interest") {
	_init();
}

template <typename T>
Roi<T>::Roi(const ParameterFile& pf, std::string name) :
TemplatedParameteredObject<T> ("Roi", name, "Region of interest") {
	_init();
	this->load(pf, name);
}

template <typename T>
void Roi<T>::execute() {
	out() = this;
}

template <typename T>
Roi<T>::Roi(
		const T& xB, const T& xE, const T& yB, const T& yE,
		const T& zB, const T& zE, const T& tB, const T& tE,
		const T& vB, const T& vE, std::string name) :
	TemplatedParameteredObject<T> ("roi", name, "Region of interest")
{
	_init();
	assign(xB,xE,yB,yE,zB,zE,tB,tE,vB,vE);
}

template <typename T>
Roi<T>::Roi(const Roi<T>& rhs) :
TemplatedParameteredObject<T> ("roi", "", "Region of interest") {
	_init();
	*this = rhs;
}

template <typename T>
Roi<T>::~Roi(void) {
}

template <typename T>
void Roi<T>::assign(
		const T& xB, const T& xE, const T& yB, const T& yE,
		const T& zB, const T& zE, const T& tB, const T& tE,
		const T& vB, const T& vE)
{
	xBegin() = xB;
	yBegin() = yB;
	zBegin() = zB;
	tBegin() = tB;
	vBegin() = vB;
	xEnd() = xE;
	yEnd() = yE;
	zEnd() = zE;
	tEnd() = tE;
	vEnd() = vE;
}

template <typename T>
T Roi<T>::getWidth() const {
	return xEnd() - xBegin();
}

template <typename T>
T Roi<T>::getHeight() const {
	return yEnd() - yBegin();
}

template <typename T>
T Roi<T>::getDepth() const {
	return zEnd() - zBegin();
}

template <typename T>
T Roi<T>::getDuration() const {
	return tEnd() - tBegin();
}

template <typename T>
T Roi<T>::getChannels() const {
	return vEnd() - vBegin();
}

template <typename T>
void Roi<T>::setWidth(T w) {
	xEnd() = xBegin() + w;
}

template <typename T>
void Roi<T>::setHeight(T h) {
	yEnd() = yBegin() + h;
}

template <typename T>
void Roi<T>::setDepth(T d) {
	zEnd() = zBegin() + d;
}

template <typename T>
void Roi<T>::setDuration(T d) {
	tEnd() = tBegin() + d;
}

template <typename T>
void Roi<T>::setChannels(T v) {
	vEnd() = vBegin() + v;
}

template <typename T>
T Roi<T>::getVolume() const {
	T volume = getWidth();
	if (getHeight())
		volume *= getHeight();
	if (getDepth())
		volume *= getDepth();
	if (getDuration())
		volume *= getDuration();
	if (getChannels())
		volume *= getChannels();
	return volume;
}

template <typename T>
bool Roi<T>::isInside(T x, T y, T z, T t, T v) const {
	bool res = true;
	res = res && (x >= xBegin() && x < xEnd());
	res = res && (y >= yBegin() && y < yEnd());
	res = res && (z >= zBegin() && z < zEnd());
	res = res && (t >= tBegin() && t < tEnd());
	res = res && (v >= vBegin() && v < vEnd());
	return res;
}

template <typename T>
Roi<T>& Roi<T>::operator= (const Roi<T>& rhs) {
	assign(rhs.xBegin(),rhs.xEnd(),
		rhs.yBegin(), rhs.yEnd(),
		rhs.zBegin(), rhs.zEnd(),
		rhs.tBegin(), rhs.tEnd(),
		rhs.vBegin(), rhs.vEnd());
	return *this;
}

template <typename T>
void Roi<T>::intersectionWith(const Roi<T>& rhs) {
	if(rhs.xBegin() > xBegin()) xBegin() = rhs.xBegin();
	if(rhs.yBegin() > yBegin()) yBegin() = rhs.yBegin();
	if(rhs.zBegin() > zBegin()) zBegin() = rhs.zBegin();
	if(rhs.tBegin() > tBegin()) tBegin() = rhs.tBegin();
	if(rhs.vBegin() > vBegin()) vBegin() = rhs.vBegin();

	if(rhs.xEnd() < xEnd()) xEnd() = rhs.xEnd();
	if(rhs.yEnd() < yEnd()) yEnd() = rhs.yEnd();
	if(rhs.zEnd() < zEnd()) zEnd() = rhs.zEnd();
	if(rhs.tEnd() < tEnd()) tEnd() = rhs.tEnd();
	if(rhs.vEnd() < vEnd()) vEnd() = rhs.vEnd();
}

template <typename T>
void Roi<T>::unionWith(const Roi<T>& rhs) {
	if(rhs.xBegin() < xBegin()) xBegin() = rhs.xBegin();
	if(rhs.yBegin() < yBegin()) yBegin() = rhs.yBegin();
	if(rhs.zBegin() < zBegin()) zBegin() = rhs.zBegin();
	if(rhs.tBegin() < tBegin()) tBegin() = rhs.tBegin();
	if(rhs.vBegin() < vBegin()) vBegin() = rhs.vBegin();

	if(rhs.xEnd() > xEnd()) xEnd() = rhs.xEnd();
	if(rhs.yEnd() > yEnd()) yEnd() = rhs.yEnd();
	if(rhs.zEnd() > zEnd()) zEnd() = rhs.zEnd();
	if(rhs.tEnd() > tEnd()) tEnd() = rhs.tEnd();
	if(rhs.vEnd() > vEnd()) vEnd() = rhs.vEnd();
}

template <typename T>
void Roi<T>::loadParameters(const ParameterFile& pf) {
	ParameteredObject::loadParameters(pf);

	// correct order of parameters
	if (xBegin() > xEnd()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching xBegin and xEnd." << std::endl;
		std::swap(xBegin(), xEnd());
	}

	if (yBegin() > yEnd()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching yBegin and yEnd." << std::endl;
		std::swap(yBegin(), yEnd());
	}

	if (zBegin() > zEnd()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching zBegin and zEnd." << std::endl;
		std::swap(zBegin(), zEnd());
	}

	if (tBegin() > tEnd()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching tBegin and tEnd." << std::endl;
		std::swap(tBegin(), tEnd());
	}

	if (vBegin() > vEnd()) {
		sout << "Roi '" << ParameteredObject::getName();
		sout << "': Switching vBegin and vEnd." << std::endl;
		std::swap(vBegin(), vEnd());
	}
}

template <typename T>
void Roi<T>::print(std::ostream& strm) const {
	strm << *this;
}

template <typename T>
roi_DECLDIR std::ostream& operator<<(std::ostream& strm, const Roi<T>& roi) {
	strm << "x:[" << roi.xBegin() << "," << roi.xEnd() << "), ";
	strm << "y:[" << roi.yBegin() << "," << roi.yEnd() << "), ";
	strm << "z:[" << roi.zBegin() << "," << roi.zEnd() << "), ";
	strm << "t:[" << roi.tBegin() << "," << roi.tEnd() << "), ";
	strm << "v:[" << roi.vBegin() << "," << roi.vEnd() << ")";
	return strm;
}

#endif // _Roi_HXX_
