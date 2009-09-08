/** @file Blockmatching.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef _BLOCKMATCHING_HXX_
#define _BLOCKMATCHING_HXX_

#include "BlockMatching.h"
#include <vector>

template<typename T>
BlockMatching<T>::BlockMatching(const std::string& name) :
	TemplatedParameteredObject<T>("blockmatching", name,
			"BlockMatching try to fing a flow"), newpos(false, true)
{
	this->_addInputSlot(sequence, "sequence", "Sequence of Images", "CImg_library");
	this->_addInputSlot(range, "range", "range of interest", "Roi<int>*");
	this->_addInputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T>>*");
	this->_addInputSlot(newpos, "newpos",
			"Next Position in Image after Incrementation", "Incrementor<T>*");
	this->_addInputSlot(changes, "changes", "Models to be changed",
			"std::vector<std::string>*");
}

#endif
