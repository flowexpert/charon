#ifndef _stencil_HXX_
#define _stencil_HXX_

#include "CImg.h"
#include "ParameteredObject.h"
#include "Parameter.h"
#include <vector>
#include <string>
#include <map>

///abstract base class for stencils
template <class T>
class Stencil : public ParameteredObject
{
	private:
		std::vector<std::string> unknowns;	//vector of the names of unknowns
		Parameter<T> lambda;
		std::map<std::string, cimg_library::CImg<T> > stencil;
		OutputSlot<Stencil*> out;

	protected:
		Stencil(const std::string& classname, const std::string& name = "") : 
			ParameteredObject(classname,name,
			"discretizes partial differential equation terms or defines derivatives filters for images")) {
				_addOutputSlot(out,"this","Pointer to itself","Stencil*");
				_addParameter(lambda,"lambda","weight of the pde term",1)
		}
	
	public:
		virtual T get(	const unsigned int x,
						const unsigned int y,
						const unsigned int z=0,
						const unsigned int t=0,
						const unsigned int v=0) const;
						
		virtual std::map<std::string, cimg_library::CImg<T> >& get(
							const unsigned int x,
							const unsigned int y,
							const unsigned int z=0,
							const unsigned int t=0,
							const unsigned int v=0
							std::map<std::string, T>& rhs) const =0;
							
				
		cimg_library::CImgList<T>& apply(	const cimg_library::CImgList<T>& seq,
											const unsigned int frame) const;
											
		const std::map<std::string, cimg_library::CImg<T> >& getStencil() const {return stencil;};

		//Getter for the names of unknowns, overload this in your implementation
		//if you have unknowns
		virtual std::vector<std::string>& getUnknowns() const {return unknowns;};
				
		virtual ~Stencil();
		
}

#endif //_stencil_HXX_