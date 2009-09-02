#ifndef _PETSCSOLVER_HXX_
#define _PETSCSOLVER_HXX_

#include "solver.hxx"
#include "petscksp.h"

template <class T>
class PetscSolver : public Solver
{
	private:
		class PetscMetaStencil : public Solver<T>::MetaStencil
		{
			private:
				PetscInt* indices;
			
			public:
				PetscMetaStencil(const std::string unknown,const std::vector<stencil<T>*>& stencils) :
				Solver<T>::MetaStencil(unknown,stencils) {
					std::vector<std::pair<cimg_library::CImg<T>*,Point4D> >::iterator ssIt; //substencil Iterator
					for (ssIt = substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
						ssIt->first->getStructure();
						int xo=center[0] - ssIt->second->x;
						int yo=center[1] - ssIt->sedonc->y;
						int zo=center[2] - ssIt->sedonc->z;
						int to=center[3] - ssIt->sedonc->t;
					}
				}
				
				void update(const unsigned int x,
							const unsigned int y,
							const unsigned int z,
							const unsigned int t) {
				}
		};
		
		//convert from coordinate in the ROI of an unknown to the vector index
		//of the unknown
		unsigned int getIndex(const Point4D p, const Point4D dim) const {
			unsigned int res=0;
			
			res += p.t * (dim.x * dim.y * dim.z);
			res += p.z * (dim.x * dim.y);
			res += p.y * (dim.x);
			res += p.x;
			
			return res;
		}
		
		//convert from the relative vector index (inside an unkown) to the
		//global vector index
		unsigned int getVectorIndex(const unsigned int i,
									const std::string& unknown,
									const std::map<std::string,roi<int> >& unknownSizes) {
			unsigned int res=0;
			
			std::map<std::string,roi<int> >::iterator usIt; //unknown Sizes iterator
			for(usIt=unknownSizes.begin() ; usIt != unknownSizes.find(unknown) ; usIt++) {
				res += usIt->second().getVolume();
			}
			
			res += i;
			
			return res;
		}
		
		//convert form the global vector index to the coordinate in an ROI of
		//an unknown
		void getCoordinate(	const unsigned int vi,
							const std::map<std::string, roi<int> >& unknownSizes,
							std::string& unknown,
							int& x, int& y, int& z, int& t) {
			unsigned int i=vi;
			std::map<std::string,roi<int> >::iterator usIt=unknownSizes.begin();
			while (i - (usIt->second().getVolume()) > 0) {
				i -= usIt->second().getVolume;
				usIt++;
			}
			unknown = usIt.first();
			
			roi<int> dim = usIt.second();
			x =  i % (dim.getWidth());
			i -= x;
			y =  (i/(dim.getWidth()))%dim.getHeight();
			i -= (y*dim.getWidth());
			z =  (i/(dim.getWidth*dim.getHeight()))%dim.getDepth();
			i -= (z*dim.getWidth()*dim.getHeight());
			t =  (i/(dim.getWidth()*dim.getHeight()*dim.getDepth()))%dim.getDuration();
		}

	public:
		PetscSolver(const std::string& name = "") : 
		Solver("PetscSolver","Solver based on PETSc","solves the linear system with PETSc") {
			
		}
		
		void update() {
			ParameteredObject::update();
			
			int nu		//number of unknowns (former: nc)
			
			//     =====================
			//     P R E P A R A T I O N
			//     =====================
			
			//because iterating through stencils is cumbersome, we reorder
			//the whole structure to iterate through unknowns and get pointer
			//to the stencils that work with this unknown.
			//
			//Data structure:
			//map<string,     vector<stencil*> >
			//    name of     vector of pointers
			//    unknown     that use the unknown
			//
			//example:
			//unknown          stencil*
			//a1               &stencil1, &stencil2, &stencil3
			//b1               &stencil1, &stencil4
			//c1               &stencil2, &stencil3
			//
			//This way, we can iterate through unknowns and access the
			//necessary stencils by pointers instead of searching through all
			//stencils to find those that use the unknown in question
			
			std::map<std::string, std::vector<stencil<T>*> > substencils;
			std::set<AbstractSlot<T>*>::const_iterator sIt; //stencil iterator
			std::set<std::string>::iterator uIt;			//unknowns iterator
			for(sIt=stencils.begin() ; sIt!=stencils.end() ; sIt++) {	//iterate through stencils
				for(uIt=sIt->getUnknowns().begin();						//iterate through its unknowns
					uIt!=sIt->getUnknowns().end();
					uIt++) {
					substencils[*uIt].push_back( (stencil<T>*)*sIt );		//add entry to the map
					//a cast from AbstractSlot<T>* to Stencil<T>*
				}
			}
			
			//From this map we can now create the individual MetaStencils
			std::map<std::string,PetscMetaStencil> MetaStencils;
			std::map<std::string, std::vector<stencil<T>*> >::iterator ssIt; //substencil Iterator
			for (ssIt=substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
				PetscMetaStencil pms(ssIt->first(),ssIt->second());
				MetaStencils[ssIt->first()] = pms;
			}
			
			//now we can determine the size of the expanded ROIs which we will
			//organize in a map<string, roi<int> > to know how big the individual
			//vector parts and thus the whole vector and the matrix will be
			std::map<std::string,roi<int> > unknownSizes;
			std::map<std::string,PetscMetaStencil>::iterator msIt; //PetscMetaStencil iterator
			for(msIt=MetaStencils.begin() ; msIt != MetaStencils.end() ; msIt++) {
				unknownSizes[msIt->first()]=msIt->second().expand(this->roi);
			}
			
			//now we have the individual lenghts of each unknowns block in the
			//matrix and thus the size of the whole matrix and the lenght of the
			//solution vector
			
			//     =========
			//     P E T S C
			//     =========
			//PetscInitialize and MPI Initialization are done in the main
			
			Vec				x, b;	//x: approx. solution, b: right hand side
			Mat				A;		//A: Linear System Matrix
			KSP				ksp;	//KSP context
			PC				pc;		//PC context
			PetscErrorCode	ierr;	//PETSc Error code for error-traceback
			PetscInt		n,i,j;	//n: size of matrix, i: column index j: row index
			PetscInt		Istart, Iend;	//Ownership range
			
			//Calculate the size of the problem (number of rows/columns of
			//the matrix, number of elements in the vectors).
			n=0;	//Better be safe than sorry
			std::map<std::string,roi<int> >::iterator usIt;	//unknown sizes iterator
			//iterate through the unknown Sizes and add their volume up
			for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
				n += usIt->getVolume();
			}
			
			//Create Vector
			ierr = VecCreate(PETSC_COMM_WORLD,&x);CHKERRQ(ierr);
			//Set the size of the vector
			ierr = VecSetSizes(x,PETSC_DECIDE,n);CHKERRQ(ierr);
			//Set other options from the database
			ierr = VecSetFromOptions(x);CHKERRQ(ierr);
			//Duplicate the vector x and save the duplicate in b
			ierr = VecDuplicate(x,&b);CHKERRQ(ierr);
			//Create the Matrix A
			ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);
			//Set the Size of A to be n*n - let PETSc decide how big the local
			//chunks should be
			ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
			//Set other options from the database
			ierr = MatSetFromOptions(A);CHKERRQ(ierr);
			
			ierr = MatGetOwnershipRange(A,&Istart,&Iend);CHKERRQ(ierr);
			
			for(i = Istart ; i < Iend ; i++) { //iterate through the owned rows
				//and set values
				
			}
		}
}

#endif // _PETSCSOLVER_HXX_