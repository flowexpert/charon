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
 *  @file BlockMatchingLIACS.hxx
 *  @brief Implementation of class BlockMatchingLIACS
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 03.09.2009
 */

#ifndef BLOCKMATCHINGLIACS_HXX_
#define BLOCKMATCHINGLIACS_HXX_

#include "BlockMatchingLIACS.h"
#include "../BlockMatching.hxx"

template<typename T>
BlockMatchingLIACS<T>::BlockMatchingLIACS(const std::string& name) :
	BlockMatching<T>::BlockMatching(name, "BlockMatchingLIACS")
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
	if (this->sequence().size())
	{
		// adjust size of surface
		this->flow().assign(4, this->sequence()[0].width(),
				this->sequence()[0].height(), this->sequence()[0].depth(),
				this->sequence()[0].spectrum());
		std::vector<T> pixelProperties;
		std::vector<std::vector<IncrementorParameter<T>*> > allParameters;
		// run over the sequence
		cimg_forXYZC(this->sequence()[0], x, y, z, t)
					{
						// loop over i until the doStep function from 
						// Incrementor returns true 
						for (int i = 0; !this->newParams()->doStep(); i++)
						{
							allParameters.push_back(
									this->newParams()->getListOfParams());
							// compute change in ObjectiveFunction
							pixelProperties.push_back(this->changes()->compare(
									this->pixelList()->getListOfPixel(),
									this->newParams()->getListOfParams()));
						}
						// find parameters with min change from the computed in 
						// SurfaceAnalysis
						std::vector<IncrementorParameter<T>*> tempParams =
								this->bestParam()->findMinChange(
										pixelProperties, allParameters);
						//typename std::vector<IncrementorParameter<T>*>::iterator
						//		it;
						T nextX, nextY, nextZ, nextT;
						for (unsigned int j = 0; j != tempParams.size(); j++)
						{
							if (tempParams[j]->name == "x")
							{
								nextX = tempParams[j]->current;
							}
							if (tempParams[j]->name == "y")
							{
								nextY = tempParams[j]->current;
							}
							if (tempParams[j]->name == "z")
							{
								nextZ = tempParams[j]->current;
							}
							if (tempParams[j]->name == "t")
							{
								nextT = tempParams[j]->current;
							}
						}
						// insert computed parameters in flow
						this->flow()(0, x, y, z, t) = nextX;
						this->flow()(1, x, y, z, t) = nextY;
						this->flow()(2, x, y, z, t) = nextZ;
						this->flow()(3, x, y, z, t) = nextT;
					}
	}
}

#endif /* BLOCKMATCHINGLIACS_HXX_ */

