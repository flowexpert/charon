#ifndef _GBCCE_HXX_
#define _GBCCE_HXX_

#include "Stencil.hxx"

templace <class T>
class Gbcce : public Stencil
{
	Private:
		InpurSlot<BrightnessModel*> brightnessIn
		InputSlot<MotionModel*> motionIn;

	Public:
		Gbcce(const std::string& name = "") : 
		Stencil("GBCCE","General brightness change constraint equation",
		"discretizes partial differential equation terms or defines derivatives filters for images")) {
			_addInputSlot(brightnessIn,"Brightness Model","Brightness Model","BrightnessModel*");
			_addInputSlot(motionIn,"Motion Model","Motion Model","MotionModel*");
		}

		void update() {
			ParametedObject::update();
			unknowns.erase();	//erase the old vector of unknowns
			std::vector<std::string> bmUnknowns = brightIn()->getUnknowns();
			std::vector<std::string> mmUnknonws = motionIn()->getUnknowns();
			std::vector<std::string>::iterator bmIt = bmUnknowns.begin();
			std::vector<std::string>::iterator mmIt = mmUnknowns.begin();
			
			//Collision detection
			while (bmIt != bmUnknowns.end()) { //go through the BrightnessModel-Vector of unknowns
				while (mmIt != mmUnknowns.end()) { //go through the MotionModel-Vector of unknowns
					if (*bmIt.compare(*mmIt) == 0) { //and compare them
						throw "name collision in unknowns";
					}
					mmIt++;
				} 				
				mmIt = mmUnknowns.begin(); //reset the inner loop iterator
				bmIt++;
			}
			//if no collision is detected, merge both input vectors into the
			//unkowns vector which was inherited from the Stencil class
			unknowns.insert(unknowns.end(),bmUnknowns.begin(),bmUnknowns.end());
			unknowns.insert(unknowns.end(),mmUnknowns.begin(),bmUnknowns.end());
			
			//expanding stencil-map to appropriate size and filling with dummy
			//values
			std::vector<std::string>::iterator uIt;
			for(uIt=unknowns.begin();uIt!=unknowns.end();uIt++) {
				this->stencil[*uIt](0,0) = 0;
			}
		}
						
		std::map<std::string, cimg_library::CImg<T> >& get(
					const unsigned int x, const unsigned int y, const unsigned int z=0,
					const unsigned int t=0, const unsigned int v=0,
					std::map<std::string, T>& rhsMap) const {
			std::map<std::string, T> term;
			T rhs=0;
			brightIn->compute(x, y, z, t, v, term, rhs);
			motionIn->compute(x, y, z, t, v, term, rhs);
			std::map<std::string, T>::iterator termIt;
			for(termIt=term.begin();termIt!=term.end();termIt++) {
				this->stencil[termIt->first](0,0) = termIt->second * lambda;
			}
			
			for(termIt=term.begin();termIt!=term.end();termIt++) {
				rhsMap[termIt->first] = rhs * termIt->second * lambda;
			}
			return stencil;
		}

		std::vector<std::string>& getUnknowns() const {return unknowns;}
}

#endif //_GBCCE_HXX_