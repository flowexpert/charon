#ifndef _L2NORM_HXX_
#define _L2NORM_HXX_

#include "Stencil.h"

template <class T>
class L2Norm : public stencil
{
	private:
		Parameter<unsigned int> dimensions;
		ParameterList<std::string> unknowns;	//List of unknowns
	
	public:
		L2Norm(const std::string& name = "") : 
		Stencil("L2Norm","L2Norm",
		"discretizes partial differential equation terms or defines derivatives filters for images")) {
			_addParameter(unknowns, "unknowns", "List of unknowns");
			_addParameter(dimensions, "dimensions", "Number of dimensions",2);
			if (dimensions > 4) {throw "invalid dimensions";}
		}
		
		void update() {
			ParameteredObject::update();
			//create mask in appropriate dimensions
			switch (dimensions) {
				case 1:
					cimg_library::CImg(3,0,0,0,0)<T> mask;
					break;
				case 2:
					cimg_library::CImg(3,3,0,0,0)<T> mask;
					break;
				case 3:
					cimg_library::CImg(3,3,3,0,0)<T> mask;
					break;
				case 4:
					cimg_library::CImg(3,3,3,3,0)<T> mask;
					break;
			}
			//filling mask with values
			if (dimensions == 1) {
				mask(0) += T(-1 * lambda);
				mask(1) += T( 2 * lambda);
				mask(2) += T(-1 * lambda);
			} else if (dimensions == 2) {
				mask(1,0) += T(-1 * lambda);
				mask(0,1) += T(-1 * lambda);
				mask(1,1) += T( 4 * lambda);
				mask(2,1) += T(-1 * lambda);
				mask(1,2) += T(-1 * lambda);
			} else if (dimensions == 3) {
				mask(1,1,0) += T(-1 * lambda);
				mask(1,0,1) += T(-1 * lambda);
				mask(0,1,1) += T(-1 * lambda);
				mask(1,1,1) += T( 6 * lambda);
				mask(2,1,1) += T(-1 * lambda);
				mask(1,2,1) += T(-1 * lambda);
				mask(1,1,2) += T(-1 * lambda);			
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
			}
			//filling stencil with masks
			std::vector<std:string>::iterator uIt;
			for(uIt=unknowns().begin() ; uIt!=unknowns().end() ; uIt++) {
				stencil[*uIt] = mask;
			}
		}
			
		std::map<std::string, cimg_library::CImg<T> >& get(
					const unsigned int x, const unsigned int y, const unsigned int z=0,
					const unsigned int t=0, const unsigned int v=0,
					std::map<std::string, T>& rhsMap) const {
			return stencil;
		}
}

#endif // _L2NORM_HXX_