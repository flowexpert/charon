/**
 *  @file Incrementor.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef INCREMENTOR_HXX_
#define INCREMENTOR_HXX_

#include "Incrementor.h"
#include "ParameteredObject.hxx"

template<typename T>
Incrementor<T>::Incrementor(const std::string& name) :
	TemplatedParameteredObject<T> ("incrementor", name,
			" Increments the Parameters, witch are needed to compute changes "
				"inside the Image "), paramList(false, true), actualPos(this)
{
	this->_addInputSlot(image, "Image", "Image where to Increment", "CImg");
	this->_addInputSlot(paramList, "paramList",
			"list of Parameters for Incrementor", "Incrementor Parameter");
	this->_addOutputSlot(actualPos, "actualPos",
			"Position in Image after Incrementation", "Incrementor<T>*");
}

template<typename T>
void Incrementor<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
std::vector<T>& Incrementor<T>::getListOfParams()
{
	return this->listOfParams;
}

#endif
