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
/** @file GBCCE.hxx
 *  Implementation of class Gbcce.
 *  This is the General Brightness Change Constraint Equation stencil which is
 *  derived from the stencil class.
 *  @see Stencil.hxx
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _GBCCE_HXX_
#define _GBCCE_HXX_

#include "Stencil.hxx"

templace <class T>
class Gbcce : public Stencil
{
	Private:
		InpurSlot<BrightnessModel*> brightnessIn;
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
			unknowns.erase();	//erase the old set of unknowns
			std::set<std::string>& bmUnknowns = brightIn()->getUnknowns();
			std::set<std::string>& mmUnknonws = motionIn()->getUnknowns();
			std::set<std::string>::iterator bmIt = bmUnknowns.begin();
			std::set<std::string>::iterator mmIt = mmUnknowns.begin();
			
			//Collision detection
			while (bmIt != bmUnknowns.end()) { //go through the BrightnessModel-Vector of unknowns
				while (mmIt != mmUnknowns.end()) { //go through the MotionModel-Vector of unknowns
					if (*bmIt == *mmIt) { //and compare them
						throw "name collision in unknowns";
					}
					mmIt++;
				} 				
				mmIt = mmUnknowns.begin(); //reset the inner loop iterator
				bmIt++;
			}
			//if no collision is detected, merge both input vectors into the
			//unkowns set which was inherited from the Stencil class
			unknowns.insert(bmUnknowns.begin(),bmUnknowns.end());
			unknowns.insert(mmUnknowns.begin(),bmUnknowns.end());
			
			//expanding stencil-map to appropriate size and filling with dummy
			//values
			std::set<std::string>::iterator uIt;
			for(uIt=unknowns.begin();uIt!=unknowns.end();uIt++) {
				Point4D center(0,0,0,0);
				substencil entry(1,1,1,1,center)
				entry.pattern(0,0) = 1;
				this->substencils[*uIt] = entry;
			}
		}
						
		void updateStencil(
				const unsigned int x,
				const unsigned int y,
				const unsigned int z=0,
				const unsigned int t=0,
				const unsigned int v=0) {
			std::map<std::string, T> term;
			T rhsV=0;
			brightIn->compute(x, y, z, t, v, term, rhsV);
			motionIn->compute(x, y, z, t, v, term, rhsV);
			std::map<std::string, T>::iterator termIt;	//term Iterator
			for(termIt=term.begin();termIt!=term.end();termIt++) {
				this->substencils[termIt->first].data(0,0) = termIt->second * lambda;
				this->rhs[termIt->first] = rhsV * termIt->second * lambda;
			}
		}

		std::set<std::string>& getUnknowns() const {return unknowns;}
		
		~GBCCE() {}
};

#endif //_GBCCE_HXX_