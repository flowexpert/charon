/// @brief flow estimation by BlockMatching
/**
 *  BlockMatching by:
 *      Listed pixel selection and Increment parameters with the Apply function.
 *      Compareing with the objective function to find a flow with the Surface
 *      analysis.
 *  short: LIACS
 *
 *  @file BlockmatchingLIACS.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 03.09.2009
 */

#ifndef BLOCKMATCHINGLIACS_H_
#define BLOCKMATCHINGLIACS_H_

#include "BlockMatching.h"
#include "PixelSelection.h"
#include "ListedPixelSelection.h"
#include "IncrementorCountUp.h"
#include "ObjectiveFunctionComparing.h"


template<typename T>
class BlockMatchingLIACS: public BlockMatching<T>
{
public:
	/// standard constructor
	BlockMatchingLIACS(const std::string& name);
	/// standard execute from ParameteredObject
	void execute();
	void findFlow();

};

#endif /* BLOCKMATCHINGLIACS_H_ */
