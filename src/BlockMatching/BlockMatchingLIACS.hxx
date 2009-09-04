/** @file BlockmatchingLIACS.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 03.09.2009
 */

#ifndef BLOCKMATCHINGLIACS_HXX_
#define BLOCKMATCHINGLIACS_HXX_

#include "BlockMatchingLIACS.h"
#include "BlockMatching.hxx"

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
	///@TODO implement me
}

#endif /* BLOCKMATCHINGLIACS_HXX_ */

