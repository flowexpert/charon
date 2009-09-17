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

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef blockmatchingliacs_EXPORTS
///Visual C++ specific code
#define blockmatchingliacs_DECLDIR __declspec(dllexport)
#else
#define blockmatchingliacs_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define blockmatchingliacs_DECLDIR
#endif

#include "../BlockMatching.h"
#include "../PixelSelection.h"
#include <ListedPixelSelection.h>
#include "../Incrementor.h"
#include <IncrementorCountUp.h>
#include "../ObjectiveFunction.h"
#include <ObjectiveFunctionComparing.h>


template<typename T>
class blockmatchingliacs_DECLDIR BlockMatchingLIACS: public BlockMatching<T>
{
private:
	std::vector<Parameter<T>* > paramListForApply;
public:
	/// standard constructor
	BlockMatchingLIACS(const std::string& name);
	virtual void /*cimg_library::CImgList<T>&*/ findFlow();

};

#endif /* BLOCKMATCHINGLIACS_H_ */
