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
			ParameteredObject("blockmatching", name,
					"BlockMatching try to fing a flow")
{
	_addInputSlot(sequence, "sequence", "Sequence of Images", "imagesequence");
	_addInputSlot(range, "range", "range of interest", "range");
	_addInputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T>>*");
	_addInputSlot(newpos, "newpos",
			"Next Position in Image after Incrementation", "Incrementor<T>*");
	_addInputSlot(changes, "changes", "Models to be changed",
			"std::vector<std::string>*");
	BlockMatching<T>::setChanges();

}

template<typename T>
void BlockMatching<T>::setChanges()
{
	this->constant = false;
	this->diffusion = false;
	this->exponential = false;
	this->movingIllumination = false;
	this->localAffines = false;
	this->localConstant = false;
	this->localPlanarity = false;
	this->localRotation = false;
	this->localStretch = false;
	std::vector<std::string>::iterator it;
	for (it = this->changes.begin(); it <= this->changes.end(); it++)
	{
		if (this->changes[it] == "Constant")
		{
			this->constant = true;
		}
		if (this->changes[it] == "Diffusion")
		{
			this->diffusion = true;
		}
		if (this->changes[it] == "Exponential")
		{
			this->exponential = true;
		}
		if (this->changes[it] == "MovingIllumination")
		{
			this->movingIllumination = true;
		}
		if (this->changes[it] == "LocalAffines")
		{
			this->localAffines = true;
		}
		if (this->changes[it] == "LocalConstant")
		{
			this->localConstant = true;
		}
		if (this->changes[it] == "LocalPlanarity")
		{
			this->localPlanarity = true;
		}
		if (this->changes[it] == "LocalRotation")
		{
			this->localRotation = true;
		}
		if (this->changes[it] == "LocalStretch")
		{
			this->localStretch = true;
		}
	}
}

#endif
