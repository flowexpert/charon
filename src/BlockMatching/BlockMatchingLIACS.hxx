/** @file BlockmatchingLIACS.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 03.09.2009
 */

#ifndef BLOCKMATCHINGLIACS_HXX_
#define BLOCKMATCHINGLIACS_HXX_

#include "BlockMatchingLIACS.h"
#include "BlockMatching.hxx"
#include "ListedPixelSelection.hxx"
#include "IncrementorCountUp.hxx"
#include "ObjectiveFunctionComparing.hxx"

template<typename T>
BlockMatchingLIACS<T>::BlockMatchingLIACS(const std::string& name) :
	BlockMatching<T>::BlockMatching(name)
{

}

template<typename T>
void BlockMatchingLIACS<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
void BlockMatchingLIACS<T>::findFlow()
{
	for (typename std::vector<Pixel<T> >::iterator iter =
			this->pixelList()->begin(); iter != this->pixelList()->end(); iter++)
	{
			//	IncrementorCountUp;
	}

}

#endif /* BLOCKMATCHINGLIACS_HXX_ */

