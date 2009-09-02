#ifndef _L2NORM_HXX_
#define _L2NORM_HXX_

#include "Stencil.h"

template <class T>
class L2Norm : public stencil
{
	private:
		bool isCurrent;		//flag to signal if the substencils contain current data or not
		Parameter<unsigned int> dimensions;
		ParameterList<std::string> pUnknowns;	//parameter list of unknowns
	
	public:
		L2Norm(const std::string& name = "") : 
		Stencil("L2Norm","L2Norm",
		"discretizes partial differential equation terms or defines derivatives filters for images"),
		isCurrent=0 {
			_addParameter(unknowns, "unknowns", "List of unknowns");
			_addParameter(dimensions, "dimensions", "Number of dimensions",2);
			if (dimensions > 4) {throw "invalid dimensions";}
		}
		
		///@todo update, updateStencil and getStructure are colliding in substencils
		void update() {
			ParameteredObject::update();
			updateStencil(0,0,0,0,0);
		}
		
		void updateStencil( const unsigned int x,
							const unsigned int y,
							const unsigned int z=0,
							const unsigned int t=0,
							const unsigned int v=0) {
			if(!isCurrent) {	//only update values if the data is not current
				//create mask in appropriate dimensions
				switch (dimensions) {
					case 1:
						cimg_library::CImg(3,1,1,1,0)<T> mask;
						break;
					case 2:
						cimg_library::CImg(3,3,1,1,0)<T> mask;
						break;
					case 3:
						cimg_library::CImg(3,3,3,1,0)<T> mask;
						break;
					case 4:
						cimg_library::CImg(3,3,3,3,0)<T> mask;
						break;
				}
				//filling mask with values and setting the center
				if (dimensions == 1) {
					mask(0) += T(-1 * lambda);
					mask(1) += T( 2 * lambda);
					mask(2) += T(-1 * lambda);
					
					center[0]=1;
					center[1]=0;
					center[2]=0;
					center[3]=0;
				} else if (dimensions == 2) {
					mask(1,0) += T(-1 * lambda);
					mask(0,1) += T(-1 * lambda);
					mask(1,1) += T( 4 * lambda);
					mask(2,1) += T(-1 * lambda);
					mask(1,2) += T(-1 * lambda);
					
					center[0]=1;
					center[1]=1;
					center[2]=0;
					center[3]=0;
				} else if (dimensions == 3) {
					mask(1,1,0) += T(-1 * lambda);
					mask(1,0,1) += T(-1 * lambda);
					mask(0,1,1) += T(-1 * lambda);
					mask(1,1,1) += T( 6 * lambda);
					mask(2,1,1) += T(-1 * lambda);
					mask(1,2,1) += T(-1 * lambda);
					mask(1,1,2) += T(-1 * lambda);
					
					center[0]=1;
					center[1]=1;
					center[2]=1;
					center[3]=0;
				} else if (dimensions == 4) {
					mask(1,1,1,0) += T(-1 * lambda);
					mask(1,1,0,1) += T(-1 * lambda);
					mask(1,0,1,1) += T(-1 * lambda);
					mask(0,1,1,1) += T(-1 * lambda);
					mask(1,1,1,1) += T( 8 * lambda);
					mask(2,1,1,1) += T(-1 * lambda);
					mask(1,2,1,1) += T(-1 * lambda);
					mask(1,1,2,1) += T(-1 * lambda);
					mask(1,1,1,2) += T(-1 * lambda);
					
					center[0]=1;
					center[1]=1;
					center[2]=1;
					center[3]=1;
				}
				//Copy the unknowns form the Parameter list into the set, which was
				//inherited from the Stencil class
				std::vector<std::string>::iterator puIt; //parameter list of unknowns iterator
				for(puIt=pUnknowns.begin() ; puIt!=pUnknowns.end() ; puIt++) {
					unknowns.insert(*puIt);
				}
				
				//filling stencil with masks
				std::set<std::string>::iterator uIt;	//unknowns iterator
				for(uIt=unknowns().begin() ; uIt!=unknowns().end() ; uIt++) {
					substencils[*uIt] = mask;
				}
			isCurrent = 1;	//flag the stencil to signal, that it contains current data
			}
		}
		
		void getStructure() {
			//create mask in appropriate dimensions
			switch (dimensions) {
				case 1:
					cimg_library::CImg(3,1,1,1,0)<T> mask;
					break;
				case 2:
					cimg_library::CImg(3,3,1,1,0)<T> mask;
					break;
				case 3:
					cimg_library::CImg(3,3,3,1,0)<T> mask;
					break;
				case 4:
					cimg_library::CImg(3,3,3,3,0)<T> mask;
					break;
			}
			//filling mask with 1s and setting the center
			if (dimensions == 1) {
				mask(0) += 1;
				mask(1) += 1;
				mask(2) += 1;
				
				center[0]=1;
				center[1]=0;
				center[2]=0;
				center[3]=0;
			} else if (dimensions == 2) {
				mask(1,0) += 1;
				mask(0,1) += 1;
				mask(1,1) += 1;
				mask(2,1) += 1;
				mask(1,2) += 1;
				
				center[0]=1;
				center[1]=1;
				center[2]=0;
				center[3]=0;
			} else if (dimensions == 3) {
				mask(1,1,0) += 1;
				mask(1,0,1) += 1;
				mask(0,1,1) += 1;
				mask(1,1,1) += 1;
				mask(2,1,1) += 1;
				mask(1,2,1) += 1;
				mask(1,1,2) += 1;
				
				center[0]=1;
				center[1]=1;
				center[2]=1;
				center[3]=0;
			} else if (dimensions == 4) {
				mask(1,1,1,0) += 1;
				mask(1,1,0,1) += 1;
				mask(1,0,1,1) += 1;
				mask(0,1,1,1) += 1;
				mask(1,1,1,1) += 1;
				mask(2,1,1,1) += 1;
				mask(1,2,1,1) += 1;
				mask(1,1,2,1) += 1;
				mask(1,1,1,2) += 1;
				
				center[0]=1;
				center[1]=1;
				center[2]=1;
				center[3]=1;
			}
			//filling stencil with masks
			std::set<std:string>::iterator uIt;	//unknowns iterator
			for(uIt=unknowns().begin() ; uIt!=unknowns().end() ; uIt++) {
				substencils[*uIt] = mask;
			}
			isCurrent = 0;	//flag stencil to signal, that it does not contain current data
		}
}

#endif // _L2NORM_HXX_