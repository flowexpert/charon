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
 *  @file Blockmatching.hxx
 *  @brief implementation of the abstract base class blockmatching
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef _BLOCKMATCHING_HXX_
#define _BLOCKMATCHING_HXX_

#include "BlockMatching.h"
#include <vector>

template<typename T>
BlockMatching<T>::BlockMatching(const std::string& name) :
	TemplatedParameteredObject<T> ("blockmatching", name,
			"BlockMatching try to fing a flow")
{
	this->_addInputSlot(sequence, "sequence", "Sequence of Images",
			"cimg_library::CImgList<T>& ");
	this->_addInputSlot(range, "range", "range of interest", "Roi<int>*");
	this->_addInputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T>>*");
	this->_addInputSlot(newParams, "newParams",
			"Next Parameters after Incrementation", "Incrementor<T>*");
	this->_addInputSlot(changes, "changes", "Models to be changed",
			"ObjectiveFuntction<T>*");
	this->_addInputSlot(bestParam, "bestParam",
			"Computes best fitting Parameters, for the flow",
			"SurfaceAnalysis<T>*");
	this->_addOutputSlot(out, "out", "Find Flow in Sequence",
			"BlockMatching<T> *");
}

template<typename T>
void BlockMatching<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
cimg_library::CImgList<T>& BlockMatching<T>::getFlow()
{
	return this->surface;
}

#endif
