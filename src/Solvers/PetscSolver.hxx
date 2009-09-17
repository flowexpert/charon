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

#include "Petscsolver.h"

///@todo incorporate std::runtime_error and #include <stdexept> instead of throw "string"

template <class T>
PetscSolver<T>::PetscMetaStencil::PetscMetaStencil(const std::string unknown,
                                                   const std::vector<Stencil<T>* >& stencils) :
		Solver<T>::MetaStencil(unknown,stencils) {
	for (int i = 0 ; i < this->substencils.size() ; i++) {
		//Calculating offsets
		int xo = this->center.x - this->substencils[i]->center.x;
		int yo = this->center.y - this->substencils[i]->center.y;
		int zo = this->center.z - this->substencils[i]->center.z;
		int to = this->center.t - this->substencils[i]->center.t;
		//saving the offset as Point4D for later convennience
		Point4D<unsigned int> offset(xo,yo,zo,to);
		
		//Iterate through all pixels of the SubStencil...
		for (int tc=0 ; tc < this->substencils[i]->patterm.dimv() ; tc++) {
			for (int zc=0 ; zc < this->substencils[i]->pattern.dimz() ; zc++) {
				for (int yc=0 ; yc < this->substencils[i]->pattern.dimy() ; yc++) {
					for (int xc=0 ; xc < this->substencils[i]->pattern.dimx() ; xc++) {
						//...and set the pattern into the
						//MetaStencil (with offset).
						if (this->substencils[i]->pattern(xc,yc,zc,tc)) {
							Point4D<unsigned int> p(xc,yc,zc,tc);
							this->pattern.insert(p+offset);
						}
					}
				}
			}
		}
	}
}

template <class T>
unsigned int PetscSolver<T>::PetscMetaStencil::update(const std::string unknown,
                                         const Point4D<unsigned int>& p,
                                         const std::map<std::string,Roi<int> >& unknownSizes,
                                         PetscInt* &columns, PetscScalar* &values) {
	//first, copy all data from the SubStencils into
	//the CImg data object of the MetaStencil
	for (int i = 0 ; i < this->substencils.size() ; i++) {
		//Calculating offsets
		int xo = this->center.x - this->substencils[i]->center.x;
		int yo = this->center.y - this->substencils[i]->center.y;
		int zo = this->center.z - this->substencils[i]->center.z;
		int to = this->center.t - this->substencils[i]->center.t;
		//saving the offset as Point4D for later convennience
		Point4D<unsigned int> offset(xo,yo,zo,to);
		
		//Iterate through all pixels of the SubStencil...
		for (int tc=0 ; tc < this->substencils[i]->data.dimv() ; tc++) {
			for (int zc=0 ; zc < this->substencils[i]->data.dimz() ; zc++) {
				for (int yc=0 ; yc < this->substencils[i]->data.dimy() ; yc++) {
					for (int xc=0 ; xc < this->substencils[i]->data.dimx() ; xc++) {
						//...and copy them into the
						//MetaStencil (with offset).
						this->data(xc+xo,yc+yo,zc+zo,tc+to)
							+=this->substencils[i]->data(xc,yc,zc,tc);
					}
				}
			}
		}
	}
	//now, the data from all the SubStencils has been merged
	//into this->data. For all the Point4Ds, which are in
	//this->pattern, we need to add the index to PetscInt *columns
	//and its value to PetscScalar *values
	for(int i = 0 ; i < this->pattern.size() ; i++) { //for all Point4Ds in this->pattern
		columns[i] = pointToGlobalIndex(this->pattern[i]+p,
		                                unknown,unknownSizes);
		values[i] = this->data(this->pattern[i].x, this->pattern[i].y,
		                       this->pattern[i].z, this->pattern[i].t);
	}
	return this->pattern.size();
}
		
template <class T>
unsigned int PetscSolver<T>::pointToRelativeIndex(const Point4D<unsigned int> p,
                                                  const Roi<int> &dim) const {
	unsigned int res=0;
	
	res += p.t * (dim.getWidth() * dim.getHeight() * dim.getDepth());
	res += p.z * (dim.getWidth() * dim.getHeight());
	res += p.y * (dim.getWidth());
	res += p.x;
	
	return res;
}

template <class T>
unsigned int PetscSolver<T>::relativeIndexToGlobalIndex(
                              const unsigned int i,
                              const std::string& unknown,
                              const std::map<std::string,Roi<int> >& unknownSizes) {
	unsigned int res=0;
	
	std::map<std::string,Roi<int> >::iterator usIt; //unknown Sizes iterator
	for(usIt=unknownSizes.begin() ; usIt != unknownSizes.find(unknown) ; usIt++) {
		res += usIt->second().getVolume();
	}
	
	res += i;
	
	return res;
}

template <class T>
void PetscSolver<T>::globalIndexToPoint(const unsigned int vi,
                         const std::map<std::string, Roi<int> >& unknownSizes,
                         std::string& unknown, Point4D<unsigned int>& p) {
	unsigned int i=vi;
	std::map<std::string,Roi<int> >::iterator usIt=unknownSizes.begin();
	while (i - (usIt->second().getVolume()) > 0) {
		i -= usIt->second().getVolume();
		usIt++;
	}
	unknown = usIt->first;
	
	Roi<int>& dim = usIt->second;
	p.x =  i % (dim.getWidth());
	i -= p.x;
	p.y =  (i/(dim.getWidth()))%dim.getHeight();
	i -= (p.y*dim.getWidth());
	p.z =  (i/(dim.getWidth()*dim.getHeight()))%dim.getDepth();
	i -= (p.z*dim.getWidth()*dim.getHeight());
	p.t =  (i/(dim.getWidth()*dim.getHeight()*dim.getDepth()))%dim.getDuration();
	
	Point4D<unsigned int> offset(-dim.left, -dim.top, -dim.front, -dim.back);
	p -= offset;
}

template <class T>
unsigned int PetscSolver<T>::pointToGlobalIndex(const Point4D<unsigned int> &p,
                                                const std::string unknown,
                                                const std::map<std::string, Roi<int> >& unknownSizes) {
	unsigned int result;
	result = pointToRelativeIndex(p,unknownSizes[unknown]);
	result = relativeIndexToGlobalIndex(result, unknown, unknownSizes);
	return result;
}

template <class T>
Point4D<unsigned int>& PetscSolver<T>::getBoundary(Point4D<unsigned int>& p) {
	//First, identify the case
	//Point4D caseID does not contain coordinates in this scope.
	//It contains an identification for the different cases of boundary
	//conditions for easy and efficient handling.
	Point4D<unsigned int> caseID;
	if (p.x <= this->roi().left) {caseID.x = 0;}
	if (p.x > this->roi().left && p.x < this->roi().right) {caseID.x = 1;}
	if (p.x >= this->roi().right) {caseID.x = 2;}
	if (p.y <= this->roi().top) {caseID.y = 0;}
	if (p.y > this->roi().top && p.y < this->roi().bottom) {caseID.y = 1;}
	if (p.y >= this->roi().bottom) {caseID.y = 2;}
	if (p.z <= this->roi().front) {caseID.z = 0;}
	if (p.z > this->roi().front && p.z < this->roi().back) {caseID.z = 1;}
	if (p.z >= this->roi().back) {caseID.z = 2;}
	if (p.t <= this->roi().before) {caseID.t = 0;}
	if (p.t > this->roi().before && p.t < this->roi().after) {caseID.t = 1;}
	if (p.t >= this->roi().after) {caseID.t = 2;}
				
	Point4D<unsigned int> result;
	
	//resolve each dimension of the case
	switch(caseID.x) {
		case 0: result.x = this->roi()->left(); break;
		case 1: result.x = p.x; break;
		case 2: result.x = this->roi()->right(); break;
	}
	
	switch(caseID.y) {
		case 0: result.y = this->roi()->top(); break;
		case 1: result.y = p.y; break;
		case 2: result.y = this->roi()->bottom(); break;
	}
	
	switch(caseID.z) {
		case 0: result.z = this->roi()->front(); break;
		case 1: result.z = p.z; break;
		case 2: result.z = this->roi()->back(); break;
	}
	
	switch(caseID.t) {
		case 0: result.t = this->roi()->before(); break;
		case 1: result.t = p.t; break;
		case 2: result.t = this->roi()->after(); break;
	}
	
	return result;
}

template <class T>
PetscSolver<T>::PetscSolver(const std::string& name) : 
		Solver<T>("PetscSolver") {
	this->columns=NULL;
	this->values=NULL;
}

template <class T>
bool PetscSolver<T>::isRankZero() {
	int initialized = 0;
	MPI_Initialized(&initialized);
	if(!initialized) //just output everything if MPI is not yet initialized
		return true;
	
	PetscMPIInt rank;
	MPI_Comm_rank(PETSC_COMM_WORLD,&rank); //WARNING: this line crashes, if petsc was not initialized!
	return !rank;
}

template <class T>
void PetscSolver<T>::execute() {
	ParameteredObject::execute();
				
	//    *=======================*
	//    | P R E P A R A T I O N |
	//    *=======================*
	
	//For better acess, the first thing we will do is to reorder the
	//SubStencils. Until now, all the SubStencils of a method are
	//grouped together in a Stencil<T> object. What we want to end with
	//in this preparation phase is a map of MetaStencils, which group
	//the SubStencils by unknown. This is a 2-step process, in the first
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
	
	std::map<std::string, std::vector<Stencil<T>*> > substencils;
	typename std::set<AbstractSlot<Stencil<T>* >* >::const_iterator sIt; //stencil iterator
	std::set<std::string>::iterator uIt;			//unknowns iterator
	for(sIt=this->stencils.begin() ; sIt!=this->stencils.end() ; sIt++) {	//iterate through stencils
	InputSlot<Stencil<T>* >& is = *(InputSlot<Stencil<T>* >*)*sIt;
		for(uIt=is()->getUnknowns().begin();						//iterate through its unknowns
			uIt!=is()->getUnknowns().end();
			uIt++) {
			substencils[*uIt].push_back(is() );	//not sure about the 2nd * of sIt
			//a cast from AbstractSlot<T>* to Stencil<T>*
		}
	}
	
	//In the second step, we go through the just created map of
	//SubStencils and create a PetscMetaStencil object for each unknown.
	//We organize these PetscMetaStencils in a map - keyed to their
	//unknown.
	std::map<std::string,PetscMetaStencil> MetaStencils;
	typename std::map<std::string, std::vector<Stencil<T>*> >::iterator ssIt; //SubStencil Iterator
	for (ssIt=substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
		typename PetscSolver<T>::PetscMetaStencil pms(ssIt->first,ssIt->second);
		MetaStencils[ssIt->first] = pms;
	}
	
	//now we can determine the size of the expanded ROIs which we will
	//organize in a map<string, Roi<int> > to know how big the individual
	//vector parts and thus the whole vector and the matrix will be
	//and we can determine the maximum number of entries for the PetscInt
	//and PetscScalar arrays, which will be later used to set values in
	//the matrix
	unsigned int max_ne=0;	//maximum number of entries;
	std::map<std::string,Roi<int> > unknownSizes;
	typename std::map<std::string,PetscMetaStencil>::iterator msIt; //PetscMetaStencil iterator
	for(msIt=MetaStencils.begin() ; msIt != MetaStencils.end() ; msIt++) {
		unknownSizes[msIt->first]=msIt->second.expand(this->roi);
		//find the maximum number of entries for the MatSetValues call
		//so that the size of columns and values is optimal
		if ( (unsigned int)msIt->getPattern().size() > max_ne) {
			max_ne = (unsigned int)msIt->getPattern().size();
		}
	}
	
	//The map containing the unknownSizes is - like all maps - alphanumerically
	//sorted by key. This will get important with the write-back of the results.
	
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
	std::map<std::string,Roi<int> >::iterator usIt;	//unknown sizes iterator
	//iterate through the unknown Sizes and add their volume up
	for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
		n += PetscInt(usIt->second.getVolume());
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
	PetscInt		j;		//j: row index
	PetscInt		Istart, Iend;	//Ownership range
	
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
		
		std::string unknown;	//current unknown
		Point4D<unsigned int> p;	//current point
		
		globalIndexToPoint( (unsigned int)j, unknownSizes, unknown, p);
		//now p contains the coordinate of the current point and
		//unknown contains the name of the current unknown.
		
		//First, determine whether the point that corresponds to this
		//row is a ghost node or 'real point'
		if (this->roi()->isInside(p.x, p.y, p.z, p.t)) { //Real point:
			//Update all stencils of the unknown to contain current data
			int nos = (int)substencils[unknown].size();	//number of stencils
			PetscScalar rhs;	//right hand side
			for (int index = 0 ; index < nos ; index++) {
				substencils[unknown][index]->updateStencil(p.x, p.y, p.z, p.t);
				rhs += (PetscScalar)substencils[unknown][index]->getRhs()[unknown];
			}
			//now call the MetaStencil of this unknown to gather all the
			//data of its SubStencils (which have just been updated) and
			//put the values and their indices into the respective arrays
			//for MatSetValues					
			unsigned int ne; //number of entries
			//transfer data form the SubStencils into the arrays and get
			//the number of entries back
			ne = MetaStencils[unknown]->update(unknown,p,unknownSizes,columns,values);
			
			//write values into matrix
			ierr = MatSetValues(A,1,&j,ne,columns,values,INSERT_VALUES);CHKERRQ(ierr);
			//and right hand side
			ierr = VecSetValues(b,1,&j,&rhs,INSERT_VALUES);CHKERRQ(ierr);
			
			
		} else { //Ghost node:
			//To handle Ghost nodes, we have to resolve the boundary
			//conditions. In this case, the boundary condition is hard
			//coded and dictates, that all ghost pixels have the same
			//value as their closest real pixel.
			//Find the closest boundary pixel
			Point4D<unsigned int> boundary = getBoundary(p);
			
			values[0] = 1;
			values[1] = -1;
			columns[0] = j;
			columns[1] = pointToGlobalIndex(boundary, unknown, unknownSizes);
			ierr = MatSetValues(A,1,&j,2,columns,values,INSERT_VALUES);CHKERRQ(ierr);
		}				
	}
	
	ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = VecAssemblyBegin(b);CHKERRQ(ierr);
	ierr = VecAssemblyEnd(b);CHKERRQ(ierr);
	
	ierr = KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN);CHKERRQ(ierr);
	ierr = KSPSolve(ksp,b,x);CHKERRQ(ierr);
	
	//only the #0 Machine is supposed to write results back
	if (this->isRankZero()) {
		//Prepare vector and context first
		Vec				result;	//Vector to store the result in
		VecScatter		scatter;//context for scattering the result
		
		//create lookup map to convert unknown to index in the CImgList
		std::map<std::string, unsigned int> lookup;
		//an Iterator for the unknownSizes already exists in this scope: usIt
		unsigned int tempIndex=0;
		//unknownSizes is inherently sorted by key in alphanumerical order
		for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
			lookup[usIt->first]=tempIndex;
			tempIndex++;
		}
		//now, lookup contains all unknowns and their index in the CImgList
		//the indices are easily accessibly with lookup.find(unknown)->second
		std::map<std::string, unsigned int>::iterator lIt; //Lookup iterator
		unsigned int globalIndex;	//to store the current global index
		
		//Create local vector to store the result in
		ierr = VecCreate(PETSC_COMM_WORLD,&result);CHKERRQ(ierr);
		ierr = VecSetSizes(result,n,n);CHKERRQ(ierr); //local vector
		ierr = VecSetFromOptions(result);CHKERRQ(ierr);
		ierr = VecScatterCreate(x,PETSC_NULL,result,PETSC_NULL,&scatter);CHKERRQ(ierr);
		ierr = VecScatterBegin(scatter,x,result,INSERT_VALUES,SCATTER_FORWARD);CHKERRQ(ierr);
		ierr = VecScatterEnd(scatter,x,result,INSERT_VALUES,SCATTER_FORWARD);CHKERRQ(ierr);
		//now 'result' contains - well - the result. The difference is, that x is a
		//parallel vector (scattered across all machines), but 'result' is a local
		//vector which exists only in the #0 machine
		PetscScalar* res;
		//now make the result-vector available as an array of PetscScalar
		ierr = VecGetArray(result,&res);CHKERRQ(ierr);
		
		//epxand the CImgList to the needed size
		this->out() = new cimg_library::CImgList<T>(lookup.size(),
		                                            this->roi().getWidth(),
		                                            this->roi().getHeight(),
		                                            this->roi().getDepth(),
		                                            this->roi().getDuration());

		//Due to data locallity, it is performance-wise better to iterate
		//through the unknowns first and fill the CImgList element by element
		//instead of pixel by pixel. Same with the solution vector.
		for(lIt=lookup.begin() ; lIt!=lookup.end();lIt++) { //for all unknowns
			forRoiXYZT(this->roi(),x,y,z,t) { //go through the image
				//get the current global index
				globalIndex = pointToGlobalIndex(Point4D<unsigned int>(x,y,z,t),lIt->first, unknownSizes);
				//and write the results into the output slot 'out'
				this->out()(lIt->second,x,y,z,t) = res[globalIndex];
			}
		}
		
		ierr = VecRestoreArray(result,&res);CHKERRQ(ierr);
	}
	
	
	//clean up
	ierr = VecDestroy(x);
	ierr = VecDestroy(b);
	ierr = MatDestroy(A);
	ierr = KSPDestroy(ksp);
	ierr = PetscFinalize();
}

template <class T>
PetscSolver<T>::~PetscSolver() {
	//make sure, PetscInt and PetscScalar get cleared
	delete[] columns;
	delete[] values;
	
}

#endif // _PETSCSOLVER_HXX_