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
/** @file IterativeSolver.hxx
 *  Implementation of class IteratorHelper.
 *  @author <a href="mailto:techfreaq@web.de">
 *      Nina Hernitschek</a>
 *  @date 26.02.2010
 */

#ifndef _ITERATORHELPER_HXX_
#define _ITERATORHELPER_HXX_

#include "IteratorHelper.h"

template <typename T>
void IteratorHelper<T>::_init() {
	//this->_addInputSlot(imgListIn,"imgListIn",
	//	"CImgList from file","CImgList<T>");
	this->_addInputSlot(imgListFileIn,"imgListFileIn",
		"CImgList from IterativeSolver","CImgList<T>");
	this->_addParameter(iterations,"iterations","number of iterations",5,"int");
	this->_addOutputSlot(imgListOut,"imgListOut",
		"CImgList containing the CImgList used in iteration","CImgList<T>");
	this->_addOutputSlot(out,"this","Pointer to itself","IteratorHelper<T>*");

	out = this;

	i = 0;
	flag = 1;
}

template <typename T>
IteratorHelper<T>::IteratorHelper(
		const std::string& classname,
		const std::string& name,
		const std::string& doc) :
		TemplatedParameteredObject<T>(classname,name,
			doc + "<br><br>derived by class IteratorHelper")
{
	_init();
}

template <typename T>
IteratorHelper<T>::IteratorHelper(
		const std::string& name) :
		TemplatedParameteredObject<T>("IteratorHelper",name,
			"Helper for iterative solvers")
{
	_init();
}

template <typename T>
void IteratorHelper<T>::update(cimg_library::CImgList<T> imgList)
{
	imgListIn = imgList;
}

template <typename T>
void IteratorHelper<T>::execute() {
	ParameteredObject::execute();

	if (i<=iterations)
	{
		if(flag==1)
		{
			imgListOut=imgListFileIn;
			flag=0;
		}
		else
		{
			imgListOut=imgListIn;
		}
	i++;
	}


}

#endif // _ITERATORHELPER_HXX_
