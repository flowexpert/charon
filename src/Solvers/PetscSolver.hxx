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
/** @file PetscSolver.hxx
 *  Implementation of class PetscSolver.
 *  This is the PETSc implementation of a solver. It has been designed to run on
 *  multiple instances simultaneously.
 *  @see Solver.hxx
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *
 *  @date 8.09.2009
 */

#ifndef _PETSCSOLVER_HXX_
#define _PETSCSOLVER_HXX_

#include "solver.hxx"
#include "petscksp.h"

///@todo incorporate std::runtime_error and #include <stdexept> instead of throw "string"

template <class T>
class PetscSolver : public Solver
{
	private:
		PetscInt		*columns;
		PetscScalar		*values;
		
		class PetscMetaStencil : public Solver<T>::MetaStencil
		{			
			public:
				PetscMetaStencil(const std::string unknown,const std::vector<stencil<T>*>& stencils) :
				Solver<T>::MetaStencil(unknown,stencils) {
					std::vector< Substencil<T>* >::iterator ssIt; //substencil Iterator
					for (ssIt = substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
						//Calculating offsets
						int xo = center.x - ssIt->second->center.x;
						int yo = center.y - ssIt->second->center.y;
						int zo = center.z - ssIt->second->center.z;
						int to = center.t - ssIt->second->center.t;
						//saving the offset as Point4D for later convennience
						Point4D offset(xo,yo,zo,to);
						
						//Iterate through all pixels of the substencil...
						for (int tc=0 ; tc < ssIt->second->patterm.dimv() ; tc++) {
							for (int zc=0 ; zc < ssIt->second->pattern.dimz() ; zc++) {
								for (int yc=0 ; yc < ssIt->second->pattern.dimy() ; yc++) {
									for (int xc=0 ; xc < ssIt->second->pattern.dimx() ; xc++) {
										//...and set the pattern into the
										//metastencil (with offset).
										if (ssIt->second->pattern(xc,yc,zc,tc)) {
											Point4D p(xc,yc,zc,tc);
											pattern.insert(p+offset);
										}
									}
								}
							}
						}
					}
				}
				
				/**
				 * transfer the data from the substencils into columns and values array.
				 * @param[in] gi global index.
				 */
				void update(const unsigned int gi) {
					
				}
		};
		
		/**
		 * Converts a coordinate in an ROI into a relative vector index.
		 * @param[in] p Point to convert.
		 * @param[in] dim Dimensions of the ROI that p is in.
		 * @see getVectorIndex()
		 * @see getCoordinate()
		 * @return Relative vector index.
		 */
		unsigned int getIndex(const Point4D p, const Point4D dim) const {
			unsigned int res=0;
			
			res += p.t * (dim.x * dim.y * dim.z);
			res += p.z * (dim.x * dim.y);
			res += p.y * (dim.x);
			res += p.x;
			
			return res;
		}
		
		/**
		 * Converts a relative vector index to a global vector index.
		 * @param[in] i Relative vector index.
		 * @param[in] unknown The unknown in which the relative vector index is.
		 * @param[in] unknownSizes Map of ROIs associated to their unknown.
		 * @see getIndex()
		 * @see getCoordinate()
		 * @return Global vector index.
		 */
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
		/**
		 * Convert a global vector index to 4-dimensional coordinates and the according unknown.
		 * @param[in] vi Global vector index.
		 * @param[in] unknownSizes Map of ROIs associated to their unknown.
		 * @param[out] unknown Unknown of the ROI in which the point is.
		 * @param[out] p Coordinates of the point.
		 * @see getIndex()
		 * @see getVectorIndex()
		 */
		void getCoordinate(	const unsigned int vi,
							const std::map<std::string, roi<int> >& unknownSizes,
							std::string& unknown, Point4D& p) {
			unsigned int i=vi;
			std::map<std::string,roi<int> >::iterator usIt=unknownSizes.begin();
			while (i - (usIt->second().getVolume()) > 0) {
				i -= usIt->second().getVolume();
				usIt++;
			}
			unknown = usIt.first();
			
			roi<int>& dim = usIt.second();
			p.x =  i % (dim.getWidth());
			i -= p.x;
			p.y =  (i/(dim.getWidth()))%dim.getHeight();
			i -= (p.y*dim.getWidth());
			p.z =  (i/(dim.getWidth*dim.getHeight()))%dim.getDepth();
			i -= (p.z*dim.getWidth()*dim.getHeight());
			p.t =  (i/(dim.getWidth()*dim.getHeight()*dim.getDepth()))%dim.getDuration();
		}
		
		/**
		 * Find the case for the boundary condition.
		 * Point4D is slightly abused here to store the caseID.
		 * @param[in] p Ghost point whose case should be determined.
		 * @return A Point4D containing the caseID reaching from (0,0,0,0)
		 *         to (2,2,2,2) where (1,1,1,1) is forbidden because this could
		 *         not be a ghost point.
		 */
		Point4D& findCase(Point4D &p) {
			Point4D caseID;
			if (p.x <= roi().left) {caseID.x = 0;}
			if (p.x > roi().left && p.x < roi().right) {caseID.x = 1;}
			if (p.x >= roi().right) {caseID.x = 2;}
			if (p.y <= roi().top) {caseID.y = 0;}
			if (p.y > roi().top && p.y < roi().bottom) {caseID.y = 1;}
			if (p.y >= roi().bottom) {caseID.y = 2;}
			if (p.z <= roi().front) {caseID.z = 0;}
			if (p.z > roi().front && p.z < roi().back) {caseID.z = 1;}
			if (p.z >= roi().back) {caseID.z = 2;}
			if (p.t <= roi().before) {caseID.t = 0;}
			if (p.t > roi().before && p.t < roi().after) {caseID.t = 1;}
			if (p.t >= roi().after) {caseID.t = 2;}
			
			return caseID;
		}
		
		Point4D& resolveCase(Point4D &caseID) {
			switch(caseID.t) {
				case 0:
					switch(caseID.z) {
						case 0:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 1:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 2:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
					} //end of switch(caseID.z)
				case 1:
					switch(caseID.z) {
						case 0:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 1:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 2:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
					} //end of switch(caseID.z)
				case 2:
					switch(caseID.z) {
						case 0:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 1:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
						case 2:
							switch(caseID.y) {
								case 0:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 1:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
								case 2:
									switch(caseID.x) {
										case 0:
										
										case 1:
										
										case 2:
										
									} //end of switch(caseID.x)
							} //end of switch(caseID.y)
					} //end of switch(caseID.z)
			} //end of switch(caseID.t)
		} //end of resolveCase
		

	public:	
		PetscSolver(const std::string& name = "") : 
		Solver("PetscSolver","Solver based on PETSc","solves the linear system with PETSc") {
			columns=NULL;
			values=NULL;
		}
		
		void update() {
			ParameteredObject::update();
						
			//    *=======================*
			//    | P R E P A R A T I O N |
			//    *=======================*
			
			//For better acess, the first thing we will do is to reorder the
			//Substencils. Until now, all the substencils of a method are
			//grouped together in a Stencil<T> object. What we want to end with
			//in this preparation phase is a map of metastencils, which group
			//the substencils by unknown. This is a 2-step process, in the first
			//step, we go through all stencils and add a pointer to the stencil
			//into a map, where it is associated with its respective unknown.
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
					uIt!=(*sIt)->getUnknowns().end();
					uIt++) {
					substencils[*uIt].push_back( (stencil<T>*)**sIt );	//not sure about the 2nd * of sIt
					//a cast from AbstractSlot<T>* to Stencil<T>*
				}
			}
			
			//In the second step, we go through the just created map of
			//substencils and create a PetscMetastencil object for each unknown.
			//We organize these PetscMetastencils in a map - keyed to their
			//unknown.
			std::map<std::string,PetscMetaStencil> MetaStencils;
			std::map<std::string, std::vector<stencil<T>*> >::iterator ssIt; //substencil Iterator
			for (ssIt=substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
				PetscMetaStencil pms(ssIt->first(),ssIt->second());
				MetaStencils[ssIt->first()] = pms;
			}
			
			//now we can determine the size of the expanded ROIs which we will
			//organize in a map<string, roi<int> > to know how big the individual
			//vector parts and thus the whole vector and the matrix will be
			//and we can determine the maximum number of entries for the PetscInt
			//and PetscScalar arrays, which will be later used to set values in
			//the matrix
			unsigned int max_ne=0;	//maximum number of entries;
			std::map<std::string,roi<int> > unknownSizes;
			std::map<std::string,PetscMetaStencil>::iterator msIt; //PetscMetaStencil iterator
			for(msIt=MetaStencils.begin() ; msIt != MetaStencils.end() ; msIt++) {
				unknownSizes[msIt->first()]=msIt->second().expand(this->roi);
				//find the maximum number of entries for the MatSetValues call
				//so that the size of columns and values is optimal
				if ( (unsigned int)msIt->getPattern().size() > max_ne) {
					max_ne = (unsigned int)msIt->getPattern().size();
				}
			}
			
			//delete both arrays if the already exist.
			if (columns != NULL) {delete[] columns; columns = NULL;}
			if (values  != NULL) {delete[] values;  values  = NULL;}
			//set the sizes of PetscScalar* values and PetscInt* columns
			//to the just calculated necessary size.
			columns = new PetscInt(max_ne);
			values = new PetscScalar(max_ne);
			
			//Calculate the size of the problem (number of rows/columns of
			//the matrix, number of elements in the vectors).
			PetscInt n=0;	//Better be safe than sorry
			std::map<std::string,roi<int> >::iterator usIt;	//unknown sizes iterator
			//iterate through the unknown Sizes and add their volume up
			for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
				n += usIt->getVolume();
			}

			
			//now we have the individual lenghts of each unknowns block in the
			//matrix and thus the size of the whole matrix and the lenght of the
			//solution vector
			
			//     *===========*
			//     | P E T S C |
			//     *===========*
			//PetscInitialize and MPI Initialization are done in the main
			
			Vec				x, b;	//x: approx. solution, b: right hand side
			Mat				A;		//A: Linear System Matrix
			KSP				ksp;	//KSP context
			PC				pc;		//PC context
			PetscErrorCode	ierr;	//PETSc Error code for error-traceback
			PetscInt		i,j;	//i: column index j: row index
			PetscInt		Istart, Iend;	//Ownership range
			///@todo create PetscInt and PetscScalar arrays with new and make sure, that no trash is left over
			
			
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
			
			for(j = Istart ; j < Iend ; j++) { //iterate through the owned rows
				//Convert the row index into point-coordinates and the unknown
				//of the block in which the point is.
				
				std::string unknown;
				Point4D p;
				
				getCoordinate( (unsigned int)j, unknownSizes, unknown, p);
				//now p contains the coordinate of the current point and
				//unknown contains the name of the current unknown.
				
				//First, determine whether the point that corresponds to this
				//row is a ghost node or 'real point'
				if (this->roi()->isInside(p.x, p.y, p.z, p.t)) { //Real point:
					//Update all stencils of the unknown to contain current data
					int nos = (int)substencils[unknown].size();	//number of stencils
					for (int index = 0 ; index < nos ; index++) {
						substencils[unknown][index].updateStencil(p.x, p.y, p.z, p.t);
					}
					//now call the Metastencil of this unknown to gather all the
					//data of its substencils (which have just been updated) and
					//put the values and their indices into the respective arrays
					//for MatSetValues
					
					
				} else { //Ghost node:
					//The handling of the boundary condition is split into two
					//steps: Finding out, which case it is and resolving it.
					//Picture the following
					Point4D caseID;
					Point4D associated;
					unsigned int point;
					caseID = findCase(p);
					//'associated' contains the coordinates of the associated pixel
					//the conversion into the column index has to take place in
					//this scope.
					associated = resolveCase(caseID);
					//MatSetValues
				}				
			}
		}
		
		~PetscSolver() {
			//make sure, PetscInt and PetscScalar get cleared
			delete[] columns;
			delete[] values;
		}
};

#endif // _PETSCSOLVER_HXX_