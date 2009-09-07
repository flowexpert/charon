#ifndef _L2NORM_HXX_
#define _L2NORM_HXX_

#include "Stencil.h"

template <class T>
class L2Norm : public stencil
{
	private:
		Parameter<unsigned int> dimensions;
		ParameterList<std::string> pUnknowns;	//parameter list of unknowns
	
	public:
		L2Norm(const std::string& name = "") : 
		Stencil("L2Norm","L2Norm",
		"discretizes partial differential equation terms or defines derivatives filters for images") {
			_addParameter(unknowns, "unknowns", "List of unknowns");
			_addParameter(dimensions, "dimensions", "Number of dimensions",2);
			if (dimensions > 4) {throw "invalid dimensions";}
		}
		
		void update() {
			ParameteredObject::update();
			//create masks in appropriate dimensions
			switch (dimensions) {
				case 1:
					cimg_library::CImg(3,1,1,1,0)<T> dataMask;
					cimg_library::CImg(3,1,1,1,0)<T> patternMast;
					break;
				case 2:
					cimg_library::CImg(3,3,1,1,0)<T> dataMask;
					cimg_library::Cimg(3,3,1,1,0)<T> patternMask;
					break;
				case 3:
					cimg_library::CImg(3,3,3,1,0)<T> dataMask;
					cimg_library::CImg(3,3,3,1,0)<T> patternMask;
					break;
				case 4:
					cimg_library::CImg(3,3,3,3,0)<T> dataMask;
					cimg_library::CImg(3,3,3,3,0)<T> patternMask;
					break;
			}
			
			Point4D center;
			
			//filling mask with values and setting the center
			if (dimensions == 1) {
				dataMask(0) += T(-1 * lambda);
				dataMask(1) += T( 2 * lambda);
				dataMask(2) += T(-1 * lambda);
				
				patternMask(0) = 1;
				patternMask(1) = 1;
				patternMask(2) = 1;
				
				center.x=1;
				center.y=0;
				center.z=0;
				center.t=0;
			} else if (dimensions == 2) {
				dataMask(1,0) += T(-1 * lambda);
				dataMask(0,1) += T(-1 * lambda);
				dataMask(1,1) += T( 4 * lambda);
				dataMask(2,1) += T(-1 * lambda);
				dataMask(1,2) += T(-1 * lambda);
				
				patternMask(1,0) = 1;
				patternMask(0,1) = 1;
				patternMask(1,1) = 1;
				patternMask(2,1) = 1;
				patternMask(1,2) = 1;
				
				center.x=1;
				center.y=1;
				center.z=0;
				center.t=0;
			} else if (dimensions == 3) {
				dataMask(1,1,0) += T(-1 * lambda);
				dataMask(1,0,1) += T(-1 * lambda);
				dataMask(0,1,1) += T(-1 * lambda);
				dataMask(1,1,1) += T( 6 * lambda);
				dataMask(2,1,1) += T(-1 * lambda);
				dataMask(1,2,1) += T(-1 * lambda);
				dataMask(1,1,2) += T(-1 * lambda);
				
				patternMask(1,1,0) = 1;
				patternMask(1,0,1) = 1;
				patternMask(0,1,1) = 1;
				patternMask(1,1,1) = 1;
				patternMask(2,1,1) = 1;
				patternMask(1,2,1) = 1;
				patternMask(1,1,2) = 1;
				
				center.x=1;
				center.y=1;
				center.z=1;
				center.t=0;
			} else if (dimensions == 4) {
				dataMask(1,1,1,0) += T(-1 * lambda);
				dataMask(1,1,0,1) += T(-1 * lambda);
				dataMask(1,0,1,1) += T(-1 * lambda);
				dataMask(0,1,1,1) += T(-1 * lambda);
				dataMask(1,1,1,1) += T( 8 * lambda);
				dataMask(2,1,1,1) += T(-1 * lambda);
				dataMask(1,2,1,1) += T(-1 * lambda);
				dataMask(1,1,2,1) += T(-1 * lambda);
				dataMask(1,1,1,2) += T(-1 * lambda);
				
				patternMask(1,1,1,0) = 1;
				patternMask(1,1,0,1) = 1;
				patternMask(1,0,1,1) = 1;
				patternMask(0,1,1,1) = 1;
				patternMask(1,1,1,1) = 1;
				patternMask(2,1,1,1) = 1;
				patternMask(1,2,1,1) = 1;
				patternMask(1,1,2,1) = 1;
				patternMask(1,1,1,2) = 1;
				
				center.x=1;
				center.y=1;
				center.z=1;
				center.t=1;
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
				switch(dimensions) {
					case 1:
						substencil entry(3,1,1,1,center);
						break;
					case 2:
						substencil entry(3,3,1,1,center);
						break;
					case 3:
						substencil entry(3,3,3,1,center);
						break;
					case 4:
						substencil entry(3,3,3,3,center);
						break;
				}
				entry.data = dataMask;
				entry.pattern = patternMask;
				substencils[*uIt] = entry;
			}
		}
		
		void updateStencil( const unsigned int x,
							const unsigned int y,
							const unsigned int z=0,
							const unsigned int t=0,
							const unsigned int v=0) {
			//Nothing to do since all the data is static.
		}
		
		~L2Norm() {}
		
};

#endif // _L2NORM_HXX_