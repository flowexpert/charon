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
 *  @file BlockmatchingLIACS.hxx
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
#include "SurfaceAnalysisMinChange.hxx"

template<typename T>
BlockMatchingLIACS<T>::BlockMatchingLIACS(const std::string& name) :
	BlockMatching<T>::BlockMatching(name)
{

}

template<typename T>
cimg_library::CImgList<T>& BlockMatchingLIACS<T>::findFlow()
{
	std::vector<T> pixelProperties;
	std::vector<std::vector<IncrementorParameter<T>*> > allParameters;
	cimg_forXYZV(this->sequence()[0], x, y, z, t)
				{
					for (int i = 0; this->newParams()->doStep(); i++)
					{
						allParameters.push_back(
								this->newParams()->getListOfParams());
						pixelProperties.push_back(this->changes()->compare(
								*(this->pixelList()),
								this->newParams()->getListOfParams()));
					}
					std::vector<IncrementorParameter<T>*> tempParams =
							this->bestParam()->findMinChange(pixelProperties,
									allParameters);
					typename std::vector<IncrementorParameter<T>*>::iterator it;
					T x, y, z, t;
					unsigned int j;
					for (it = tempParams.begin(); it != tempParams.end(); it++)
					{
						if (tempParams[j]->getName() == "x")
						{
							x = tempParams[j]->getCurrent();
						}
						if (tempParams[j]->getName() == "y")
						{
							y = tempParams[j]->getCurrent();
						}
						if (tempParams[j]->getName() == "z")
						{
							z = tempParams[j]->getCurrent();
						}
						if (tempParams[j]->getName() == "t")
						{
							t = tempParams[j]->getCurrent();
						}
						j++;
					}
					this->surface(x, y, z, t);
				}
	return this->surface;
}

#endif /* BLOCKMATCHINGLIACS_HXX_ */

