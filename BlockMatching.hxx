/*
 * Blockmatching.hxx
 *
 *  Created on: 12.08.2009
 *      Author: andreas
 */

#ifndef _BLOCKMATCHING_HXX_
#define _BLOCKMATCHING_HXX_

#include "BlockMatching.h"

template <typename T>
BlockMatching<T>::BlockMatching(const std::string& name) :
	ParameteredObject("blockmatching", name,
            " todo ")  //TODO Beschreibung für BlockMatching einfügen
{
    _addInputSlot(sequence, "sequence", "Sequence of Images", "imagesequence" );
    _addInputSlot(range, "range", "range of interest", "range");
}

#endif
