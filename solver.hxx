#ifndef _SOLVER_HXX_
#define _SOLVER_HXX_

#include "CImg.h"
#include "petscksp.h"
#include "ParameteredObject.h"
#include "Roi.h"
#include "stencil.hxx"

template <class T>
class Solver : public ParameteredObject
{
	private:
		InpurSlot< Stencil<T>* > stencils;
		InputSlot< Roi<int>* > roi;
		OutputSlot<cimg_library::CImgList<T>& > out;
		
		/// @todo Make class Point4D global for use in other classes like stencil?
		/// nested class to specify a point in 4-dimensional space
		class Point4D
		{
			public:
				unsigned int x, y, z, t;
				
				///default constructor
				Point4D(int x, int y, int z, int t) : x(x), y(y), z(z), t(t) {}
				
				unsigned int volume() {
					unsigned int v= x * y * z * t;
					return v;
				}
		}

		
		//meta stencil class to combine multiple substencils by unknown
		class MetaStencil
		{
			private:
				//vector of pointer to the substencils
				//where actual the data will be presented
				///@todo substencils changed to inlcude pair - modify other code to comply
				std::vector<std::pair<cimg_library::CImg<T>*,Point4D> > substencils;
				
				//The Metastencil
				cimg_library::CImg<T> metastencil;
				
				//expansions in all 8 directions
				int left=0     , right=0;	//x (left is negative, right is positive)
				int up=0       , down=0;	//y (up is negative, down is positive)
				int backward=0 , forward=0;	//z (backward is negative, forward is positive)
				int before=0   , after=0;	//t (before is negative, after is positive)
				
				int center[4];				//coordinates of the center of the meta stencil
			public:
				///default constructor
				MetaStencil(const std::string unknown,const std::vector<stencil<T>*>& stencils) {
					//Iterate through the stencils and in each one, find the substencils
					//to the given unknown. Save the maximum of expansion in each dimension
					//and reposition the center of the meta stencil accordingly
					std::vector<stencil<T>*>::iterator sIt;		//stencil iterator
					for (sIt=stencils.begin() ; sIt != stencils.end() ; sIt++) {
						//setting work-variable for better reading
						int centerx = sIt->getCenterX();
						int centery = sIt->getCenterY();
						int centerz = sIt->getCenterZ();
						int centert = sIt->getCenterT();
						
						//Measuring the substencil that is currently being added
						//to the metastencil
						int width   = sIt->get().find(unknown).second().dimx();
						int height  = sIt->get().find(unknown).second().dimy();
						int depth   = sIt->get().find(unknown).second().dimz();
						int length  = sIt->get().find(unknown).second().dimv();
						//remember the re-declaration of the v-dimension of CImg
						//to be the time axis? Yeah, right ;-)
						
						//positioning of the center of the meta stencil
						//it is important to understand, that the center can
						//onle "wander" in positive directions - no more fancy
						//checks and calculations are needed.
						if (centerx > center[0]) {center[0] = centerx;}
						if (centery > center[1]) {center[1] = centery;}
						if (centerz > center[2]) {center[2] = centerz;}
						if (centert > center[3]) {center[3] = centert;}
						
						//setting the expansions
						//Here's the same principle as with the center:
						//the individual expansions can only grow, not shrink
						//these - admittedly messy - lines just find the maximum
						//of expansions in all 8 directions
						//over all added metastencils
						if (centerx > this->left) {this->left = centerx;}
						if (width-centerx-1 > this->right) {this->right = width-centerx-1;}
						if (centery > this->up) {this->up = centery;}
						if (height-centery-1 > this->down) {this->down = height-centery-1;}
						if (centerz > this->backward) {this->backward = centerz;}
						if (depth-centerz-1 > this->forward) {this->forward = depth-centerz-1;}
						if (centert > this->before) {this->before = centert;}
						if (length-centert-1 > this->after) {this->after = length-centert-1;}
						
						//and after having measured the substencil, we finally
						//add it and its center to the metastencil
						Point4D center(centerx, centery, centerz, centert);
						std::pair<cimg_library::CImg<T>*,Point4D> entry;
						entry.first = &(sIt->get().find(unknown).second();
						entry.second = center;
						substencils.push_back(entry);
					}
					
					//expand the metastencil CImg to the appropriate size
					int dimx=left+1+right;
					int dimy=up+1+down;
					int dimz=backward+1+forward;
					int dimt=before+1+after;
					metastencil.assign(dimx, dimy, dimz, dimt, 0);
				}
				//Your implementation of the MetaStencil will need a function
				//to gather the data from the different Stencils and present
				//it in any way you want it to in order to work with your
				//solver.
				//It would be appreciated if you'd name this function 'update'.

				Roi<int> expand(const Roi<int>& inRoi) {
					int t  = -up;
					int l  = -left;
					int bo = inRoi.getHeight() + down;
					int r  = inRoi.getWidth() + right;
					int f  = -backward;
					int ba = inRoi.getDepth() + forward;
					int be = -before;
					int af = inRoi.getDuration() + after;
					
					Roi<int> roi(t, l, bo, r, f, ba, be, af, "");
					return roi;
				}
		};
	public:
		Solver(const std::string& classname, const std::string& name = "") : 
			ParameteredObject(classname,name,"solves the linear system"),
			stencils(false,true)	//make stencil input slot mandatory and multi
		{
			_addInputSlot(stencils,"stencil","Multi Input slot for stencils","stencil<T>*");
			_addInputSlot(roi,"roi","region of interest to work on","Roi<int>*");
			_addOutputSlot(out,"out","CImgList containing the solution","CImgList");
		}
	
		virtual void update() =0;
		//this is the one, only and primary function of the solver
		//once every object is connected, update() will bi called recursively
		//so all the calculations will take place in this function, thus in
		//the end, the correct data will be presented in the outputSlot
		//of the solver
}

#endif // _SOLVER_HXX_