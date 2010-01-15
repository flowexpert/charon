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
 *  @author <a href="mailto:stengele@stud.uni-heidelberg.de">
 *      Oliver Stengele</a>
 *  @date 8.09.2009
 */

#ifndef _PETSCSOLVER_HXX_
#define _PETSCSOLVER_HXX_

#include "../Solver.hxx"
#include "PetscSolver.h"
#include <charon-utils/ImgTool.hxx>
#include <sstream>

template <typename T>
PetscSolver<T>::PetscMetaStencil::PetscMetaStencil(
	const std::string& unknown,
	const std::vector<Stencil<T>*>& stencils) :
		Solver<T>::MetaStencil(unknown,stencils)
{
}

template <typename T>
PetscSolver<T>::PetscMetaStencil::PetscMetaStencil(
	const PetscMetaStencil& rhs) :
		Solver<T>::MetaStencil(rhs) {
}

template <typename T>
PetscSolver<T>::PetscMetaStencil::PetscMetaStencil() :
		Solver<T>::MetaStencil() {
}

template <typename T>
unsigned int PetscSolver<T>::PetscMetaStencil::update(
		const std::string unknown,
		const Point4D<unsigned int>& p,
		const std::map<std::string,const Roi<int>*>& unknownSizes,
		PetscInt*& columns,
		PetscScalar*& values)
{
	// first, initialize content to zero
	this->data.fill(T(0));

	// copy all data from the SubStencils into
	// the CImg data object of the MetaStencil
	for (unsigned int i = 0 ; i < this->substencils.size() ; i++) {
		// Calculating offsets
		int xo = int(this->center.x) - int(this->substencils[i]->center.x);
		int yo = int(this->center.y) - int(this->substencils[i]->center.y);
		int zo = int(this->center.z) - int(this->substencils[i]->center.z);
		int to = int(this->center.t) - int(this->substencils[i]->center.t);

		// Iterate through all pixels of the SubStencil...
		cimg_forXYZC(this->substencils[i]->data,xc,yc,zc,tc) {
			// ...and copy them into the
			// MetaStencil (with offset).
			this->data(xc+xo,yc+yo,zc+zo,tc+to)
				+= this->substencils[i]->data(xc,yc,zc,tc);
		}
	}
	// now, the data from all the SubStencils has been merged
	// into this->data. For all the Point4Ds, which are in
	// this->pattern, we need to add the index to PetscInt* columns
	// and its value to PetscScalar* values

	// pattern Iterator
	std::set<Point4D<unsigned int> >::const_iterator pIt = this->pattern.begin();
	// for all Point4Ds in this->pattern
	for(unsigned int i=0 ; i < this->pattern.size() ; i++,pIt++) {
		const Point4D<unsigned int>& curP = *pIt;
		Point4D<int> curArg = Point4D<int>(curP)+Point4D<int>(p)-this->center;
		PetscInt curCol =
			PetscSolver<T>::_pointToGlobalIndex(curArg,unknown,unknownSizes);
		columns[i] = curCol;
		values[i] = this->data(pIt->x, pIt->y, pIt->z, pIt->t);
	}
	return this->pattern.size();
}

template <typename T>
unsigned int PetscSolver<T>::_pointToRelativeIndex(
		const Point4D<int>& p,
		const Roi<int>& dim)
{
	unsigned int res = 0;
	Point4D<int> offset(dim.xBegin,dim.yBegin,dim.zBegin,dim.tBegin);
	Point4D<int> cur = p - offset;

	res += cur.t * (dim.getWidth() * dim.getHeight() * dim.getDepth());
	res += cur.z * (dim.getWidth() * dim.getHeight());
	res += cur.y * (dim.getWidth());
	res += cur.x;

	return res;
}

template <typename T>
unsigned int PetscSolver<T>::_relativeIndexToGlobalIndex(
		const unsigned int i,
		const std::string& unknown,
		const std::map<std::string,const Roi<int>*>& unknownSizes)
{
	unsigned int res = 0;

	// unknown Sizes iterator: usIt
	std::map<std::string,const Roi<int>* >::const_iterator usIt;
	for(usIt=unknownSizes.begin() ; usIt != unknownSizes.find(unknown) ; usIt++) {
		res += usIt->second->getVolume();
	}

	res += i;

	return res;
}

template <typename T>
void PetscSolver<T>::_globalIndexToPoint(
		const unsigned int vi,
		const std::map<std::string, const Roi<int>*>& unknownSizes,
		std::string& unknown,
		Point4D<int>& p)
{
	// detect block
	unsigned int blockBegin = 0;
	unsigned int blockEnd;
	std::map<std::string, const Roi<int>* >::const_iterator usIt;
	for(usIt=unknownSizes.begin(); usIt !=unknownSizes.end(); usIt++) {
		blockEnd = blockBegin + usIt->second->getVolume();
		if(vi >= blockBegin && vi < blockEnd)
			break;
		blockBegin = blockEnd;
	}
	assert(usIt != unknownSizes.end());
	unknown = usIt->first;

	// calculate coordinates relative to block
	unsigned int diff = vi - blockBegin;
	const Roi<int>& dim = *(usIt->second);
	p.x =  diff % (dim.getWidth());
	assert((int)diff >= p.x);
	diff -= p.x;
	p.y =  (diff/(dim.getWidth()))%dim.getHeight();
	assert((int)diff >= p.y*dim.getWidth());
	diff -= (p.y*dim.getWidth());
	p.z =  (diff/(dim.getWidth()*dim.getHeight()))%dim.getDepth();
	assert((int)diff >= p.z*dim.getWidth()*dim.getHeight());
	diff -= (p.z*dim.getWidth()*dim.getHeight());
	p.t =  (diff/(dim.getWidth()*dim.getHeight()*dim.getDepth()))%dim.getDuration();

	// correct global block coodinates as offset
	Point4D<int> offset(dim.xBegin, dim.yBegin, dim.zBegin, dim.tBegin);
	p += offset;
}

template <typename T>
unsigned int PetscSolver<T>::_pointToGlobalIndex(
		const Point4D<int>& p,
		const std::string& unknown,
		const std::map<std::string,const Roi<int>*>& unknownSizes)
{
	unsigned int result;
	result = _pointToRelativeIndex(p,*(unknownSizes.find(unknown)->second));
	result = _relativeIndexToGlobalIndex(result, unknown, unknownSizes);
	return result;
}

template <typename T>
Point4D<int> PetscSolver<T>::_getBoundary(Point4D<int>& p) const {
	// we just need read-only acces to the global roi
	const Roi<int>& globalRoi = *(this->roi());
	// First, identify the case
	//
	// Point4D caseID does not contain coordinates in this scope.
	// It contains an identification for the different cases of boundary
	// conditions for easy and efficient handling.
	Point4D<int> caseID;
	if (p.x <= globalRoi.xBegin) {caseID.x = 0;}
	if (p.x > globalRoi.xBegin && p.x < globalRoi.xEnd) {caseID.x = 1;}
	if (p.x >= globalRoi.xEnd) {caseID.x = 2;}
	if (p.y <= globalRoi.yBegin) {caseID.y = 0;}
	if (p.y > globalRoi.yBegin && p.y < globalRoi.yEnd) {caseID.y = 1;}
	if (p.y >= globalRoi.yEnd) {caseID.y = 2;}
	if (p.z <= globalRoi.zBegin) {caseID.z = 0;}
	if (p.z > globalRoi.zBegin && p.z < globalRoi.zEnd) {caseID.z = 1;}
	if (p.z >= globalRoi.zEnd) {caseID.z = 2;}
	if (p.t <= globalRoi.tBegin) {caseID.t = 0;}
	if (p.t > globalRoi.tBegin && p.t < globalRoi.tEnd) {caseID.t = 1;}
	if (p.t >= globalRoi.tEnd) {caseID.t = 2;}

	Point4D<int> result;

	//resolve each dimension of the case
	switch(caseID.x) {
		case 0: result.x = globalRoi.xBegin; break;
		case 1: result.x = p.x; break;
		case 2: result.x = globalRoi.xEnd-1; break;
	}

	switch(caseID.y) {
		case 0: result.y = globalRoi.yBegin; break;
		case 1: result.y = p.y; break;
		case 2: result.y = globalRoi.yEnd-1; break;
	}

	switch(caseID.z) {
		case 0: result.z = globalRoi.zBegin; break;
		case 1: result.z = p.z; break;
		case 2: result.z = globalRoi.zEnd-1; break;
	}

	switch(caseID.t) {
		case 0: result.t = globalRoi.tBegin; break;
		case 1: result.t = p.t; break;
		case 2: result.t = globalRoi.tEnd-1; break;
	}

	return result;
}

template <typename T>
bool PetscSolver<T>::_initialized = false;

template <typename T>
PetscSolver<T>::PetscSolver(const std::string& name) : 
		Solver<T>("PetscSolver", name)
{
	// add petsc command line
	ParameteredObject::_addParameter(commandLine, "commandLine",
		"petsc command line");
}

template <typename T>
PetscSolver<T>::~PetscSolver() {
	if (_initialized) {
		for (int i=0; i < _argc; i++)
			delete[] _argv[i];
		delete[] _argv;
		_argc = 0;
		_argv = 0;

		// finalize petsc
		sout << "\tfinalizing Petsc" << std::endl;
		PetscErrorCode ierr = PetscFinalize();
		if (ierr)
			sout << "Got petsc error code during destructor:\n"
				<< PetscError(__LINE__,__FUNCT__,__FILE__,__SDIR__,
					ierr,0," ")
				<< std::endl;

		int initialized = 0;
		MPI_Initialized(&initialized);
		if (initialized) {
			ierr = MPI_Finalize();
			if (ierr)
				sout << "Got petsc error code during destructor:\n"
					<< PetscError(__LINE__,__FUNCT__,__FILE__,__SDIR__,ierr,
						0," ")
					<< std::endl;
		}
		_initialized = false;
	}
}

template <typename T>
bool PetscSolver<T>::isRankZero() {
	int initialized = 0;
	MPI_Initialized(&initialized);
	// just output everything if MPI is not yet initialized
	if(!initialized)
		return true;

	int rank;
	MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	return !rank;
}


template <typename T>
int PetscSolver<T>::petscExecute() {
	/*
	 *	*=======================*
	 *	| P R E P A R A T I O N |
	 *	*=======================*
	 *
	 *	For better acess, the first thing we will do is to reorder the
	 *	SubStencils. Until now, all the SubStencils of a method are
	 *	grouped together in a Stencil<T> object. What we want to end with
	 *	in this preparation phase is a map of MetaStencils, which group
	 *	the SubStencils by unknown. This is a 2-step process, in the first
	 *	step, we go through all stencils and add a pointer to the stencil
	 *	into a map, where it is associated with its respective unknown.
	 *
	 *	Data structure:
	 *	map<string,         vector<Stencil*> >
	 *	    name of         vector of pointers
	 *	    unknown         that use the unknown
	 *
	 *	example:
	 *	    unknown         Stencil*
	 *	    a1              &stencil1, &stencil2, &stencil3
	 *	    b1              &stencil1, &stencil4
	 *	    c1              &stencil2, &stencil3
	 *
	 *	This way, we can iterate through unknowns and access the
	 *	necessary stencils by pointers instead of searching through all
	 *	stencils to find those that use the unknown in question
	 */

	// map mentionend above
	std::map<std::string, std::vector<Stencil<T>*> > substencils;
	// stencil iterator
	typename std::set<AbstractSlot<Stencil<T>*>*>::const_iterator sIt;
	// unknowns iterator
	std::set<std::string>::const_iterator uIt;
	// just need read-only access to global roi
	const Roi<int>& globalRoi = *(this->roi());
	sout << "\tglobal Roi:\n\t\t" << globalRoi << std::endl;

	// iterate through stencils
	for(sIt=this->stencils.begin() ; sIt!=this->stencils.end() ; sIt++) {
		// *sIt (dereferencing of sIt) gives us an AbstractSlot<Stencil<T>*>*
		// (a pointer).
		// Then we cast it to an InputSlot<Stencil<T>*>* (still a pointer)
		// Then we dereference that and get the InputSlot<Stencil<T>*>
		// (reference, not a pointer).
		// And we call that 'is' (short for InputStencil).
		// The operator() returns the Stencil<T>* within.
		Stencil<T>* is = (*((InputSlot<Stencil<T>*>*)*sIt))();

		//iterate through its unknowns
		sout << "\tgot Stencil: " << is->getName() << std::endl;
		if(is->getUnknowns().begin() == is->getUnknowns().end())
			sout << "\t\twarning: no unknowns found!" << std::endl;
		for(uIt=is->getUnknowns().begin();
				uIt!=is->getUnknowns().end();
					uIt++)
		{
			substencils[*uIt].push_back(is);
#ifndef NDEBUG
			// print debug information
			sout << "\t\tfound unknown \"" << *uIt
				<< "\" with the following content:" << std::endl;
			is->updateStencil(*uIt);
			const cimg_library::CImg<T>& dat =
				is->get().find(*uIt)->second.data;
			ImgTool::printInfo(sout, dat, "\t\t\t");
			sout << "\t\tand the following pattern:" << std::endl;
			const cimg_library::CImg<char>& pat =
				is->get().find(*uIt)->second.pattern;
			ImgTool::printInfo(sout, pat, "\t\t\t");
#endif
		}
	}

	/*
	 *	In the second step, we go through the just created map of
	 *	SubStencils and create a PetscMetaStencil object for each unknown.
	 *	We organize these PetscMetaStencils in a map - keyed to their
	 *	unknown.
	 */
	std::map<std::string,PetscMetaStencil> MetaStencils;
	// SubStencil Iterator
	typename std::map<std::string, std::vector<Stencil<T>*> >::iterator ssIt;
	for (ssIt=substencils.begin() ; ssIt != substencils.end() ; ssIt++) {
		typename PetscSolver<T>::PetscMetaStencil pms(ssIt->first,ssIt->second);
		MetaStencils[ssIt->first] = pms;
#ifndef NDEBUG
		sout << "\t\tcreating MetaStencil for \"" << ssIt->first
			<< "\"" << std::endl;
		sout << "\t\t\tcenter: " << MetaStencils[ssIt->first].getCenter()
			<< std::endl;
#endif
	}
#ifndef NDEBUG
	sout << "\tcreated " << MetaStencils.size()
		<< " MetaStencils." << std::endl;
	sout << "\tExpanding Roi for unknowns" << std::endl;
	sout << "\t\tglobal value:\n\t\t\t" << globalRoi << std::endl;
#endif

	/*
	 *	Now we can determine the size of the expanded ROIs which we will
	 *	organize in a map<string, const Roi<int>*> to know how big the
	 *	individual vector parts and thus the whole vector and the matrix
	 *	will be.
	 *	Also, we can determine the maximum number of entries for the PetscInt
	 *	and PetscScalar arrays, which will be later used to set values in
	 *	the matrix.
	 */

	// maximum number of entries;
	unsigned int max_ne = 0;
	std::map<std::string, const Roi<int>* > unknownSizes;
	// PetscMetaStencil iterator
	typename std::map<std::string,PetscMetaStencil>::iterator msIt;
	for(msIt=MetaStencils.begin() ; msIt != MetaStencils.end() ; msIt++) {
		Roi<int>* current = new Roi<int>(globalRoi);
		msIt->second.expand(*current);
		unknownSizes[msIt->first]=current;
#ifndef NDEBUG
		sout << "\t\t\"" << msIt->first << "\": " << std::endl;
		sout << "\t\t\t" << *current << std::endl;
#endif
		// find the maximum number of entries for the MatSetValues call
		// so that the size of columns and values is optimal
		if ( (unsigned int)msIt->second.getPattern().size() > max_ne) {
			max_ne = (unsigned int)msIt->second.getPattern().size();
		}
	}
#ifndef NDEBUG
	sout << "\tdetermined maximal number of entries per unknown: " << std::endl;
	sout << "\t\tmax_ne = " << max_ne << std::endl;
#endif

	// The map containing the unknownSizes is - like all maps -
	// alphanumerically sorted by key. This will get important with the
	// write-back of the results.

	// set the sizes of PetscScalar* values and PetscInt* columns
	// to the just calculated necessary size.
	// petsc values for columns and values
	PetscInt*       columns = new PetscInt[max_ne];   assert(columns);
	PetscScalar*    values = new PetscScalar[max_ne]; assert(values);

	// Calculate the size of the problem (number of rows/columns of
	// the matrix, number of elements in the vectors).

	// Better be safe than sorry
	PetscInt n=0;
	// unknown sizes iterator
	std::map<std::string, const Roi<int>* >::iterator usIt;
	// iterate through the unknown Sizes and add their volume up
	for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
		n += PetscInt(usIt->second->getVolume());
	}
#ifndef NDEBUG
	sout << "\tcalculated matrix/vector size: " << std::endl;
	sout << "\t\tn = " << n << std::endl;
#endif


	// now we have the individual lenghts of each unknowns block in the
	// matrix and thus the size of the whole matrix and the lenght of the
	// solution vector

	/*
	 *	*===========*
	 *	| P E T S C |
	 *	*===========*
	 */

	// Petsc initialization
	PetscErrorCode	ierr;			// PETSc Error code for error-traceback
	if(!_initialized) {
		// convert parameters into argument vector
		std::vector<std::string> args;
		std::string modLine = "petscSolver " + commandLine();
		StringTool::explode(modLine, ' ', args);

		_argc = args.size();
		_argv = new char* [_argc];
		for (int i=0; i < _argc; i++) {
			const unsigned int s = args[i].length()+1;
			_argv[i] = new char[s+1];
			memset(_argv[i], '\0', s);
			args[i].copy(_argv[i], s);
		}

		// call petsc init
		sout << "\tinitializing Petsc" << std::endl;
		sout << "\t\tusing command line \"" << commandLine() << "\"" << std::endl;
		ierr = MPI_Init(&_argc,&_argv);
		CHKERRQ(ierr);
		ierr = PetscInitialize(&_argc,&_argv,PETSC_NULL,PETSC_NULL);
		CHKERRQ(ierr);
		_initialized = true;
	}

	Vec             x,              // approx. solution
	                b;              // right hand side
	Mat             A;              // Linear System Matrix
	KSP             ksp(0);         // KSP context
	PetscInt        j;              // row index
	PetscInt        Istart, Iend;   // Ownership range

	// Create Vector
	ierr = VecCreate(PETSC_COMM_WORLD,&x); CHKERRQ(ierr);
	// Set the size of the vector
	ierr = VecSetSizes(x,PETSC_DECIDE,n); CHKERRQ(ierr);
	// Set other options from the database
	ierr = VecSetFromOptions(x); CHKERRQ(ierr);
	// Duplicate the vector x and save the duplicate in b
	ierr = VecDuplicate(x,&b); CHKERRQ(ierr);
	// Create the Matrix A
	ierr = MatCreate(PETSC_COMM_WORLD,&A); CHKERRQ(ierr);
	// Set the Size of A to be n*n - let PETSc decide how big the local
	// chunks should be
	ierr = MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,n,n); CHKERRQ(ierr);
	// Set other options from the database
	ierr = MatSetFromOptions(A); CHKERRQ(ierr);

	ierr = MatGetOwnershipRange(A,&Istart,&Iend); CHKERRQ(ierr);

	// iterate through the owned rows
	for(j = Istart ; j < Iend ; j++) {
		// Convert the row index into point-coordinates and the unknown
		// of the block in which the point is.

		std::string unknown;    //current unknown
		Point4D<int> p;         //current point

		_globalIndexToPoint((unsigned int)j, unknownSizes, unknown, p);
		// now p contains the coordinate of the current point and
		// unknown contains the name of the current unknown.

		// First, determine whether the point that corresponds to this
		// row is a ghost node or 'real point'
		if (globalRoi.isInside(p.x, p.y, p.z, p.t)) {
			/*
			 *	Real point:
			 *		if it is a real point, we have positive coordinates and thus
			 *		can cast without messing things up.
			 */
			p = Point4D<unsigned int>(p);
			// Update all stencils of the unknown to contain current data
			int nos = (int)substencils[unknown].size(); // number of stencils
			PetscScalar rhs = 0;                        // right hand side
			for (int index = 0 ; index < nos ; index++) {
				substencils.find(unknown)->second[index]->
					updateStencil(unknown, p.x, p.y, p.z, p.t);
				std::map<std::string, T>& myrhs = *(const_cast<std::map<std::string, T>*>
					(&(substencils.find(unknown)->second[index]->getRhs())));
				rhs += PetscScalar(myrhs[unknown]);
			}
			// now call the MetaStencil of this unknown to gather all the
			// data of its SubStencils (which have just been updated) and
			// put the values and their indices into the respective arrays
			// for MatSetValues
			unsigned int ne; //number of entries

			// transfer data form the SubStencils into the arrays and get
			// the number of entries back
			ne = MetaStencils[unknown].update(
				unknown,p,unknownSizes,columns,values);

			// write values into matrix
			ierr = MatSetValues(A,1,&j,ne,columns,values,INSERT_VALUES);
				CHKERRQ(ierr);

			// add missing entries (cross terms)
			ne = _addCrossTerms(unknown, p, unknownSizes, columns, values);
			ierr = MatSetValues(A,1,&j,ne,columns,values,INSERT_VALUES);
				CHKERRQ(ierr);

			// and right hand side
			ierr = VecSetValues(b,1,&j,&rhs,INSERT_VALUES); CHKERRQ(ierr);
		}
		else {
			/*
			 *	Ghost node:
			 *		To handle Ghost nodes, we have to resolve the boundary
			 *		conditions. In this case, the boundary condition is hard
			 *		coded and dictates, that all ghost pixels have the same
			 *		value as their closest real pixel.
			 */

			// Find the closest boundary pixel
			Point4D<int> boundary = _getBoundary(p);

			values[0] = 1;
			values[1] = -1;
			columns[0] = j;
			columns[1] = _pointToGlobalIndex(boundary, unknown, unknownSizes);
			ierr = MatSetValues(A,1,&j,2,columns,values,INSERT_VALUES);
				CHKERRQ(ierr);
		}				
	}

	ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
	ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
	ierr = VecAssemblyBegin(b); CHKERRQ(ierr);
	ierr = VecAssemblyEnd(b); CHKERRQ(ierr);

	ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
	ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
	ierr = KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN); CHKERRQ(ierr);
	ierr = KSPMonitorSet(ksp,KSPMonitorDefault,PETSC_NULL,PETSC_NULL);
		CHKERRQ(ierr);
	ierr = KSPSolve(ksp,b,x); CHKERRQ(ierr);
	ierr = KSPView(ksp, PETSC_VIEWER_STDOUT_SELF); CHKERRQ(ierr);
	// only the #0 Machine is supposed to write results back
	if (this->isRankZero()) {
		// create lookup map to convert unknown to index in the CImgList
		std::map<std::string, unsigned int> lookup;
		// an Iterator for the unknownSizes already exists in this scope: usIt
		unsigned int tempIndex=0;
		// unknownSizes is inherently sorted by key in alphanumerical order
		for(usIt=unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
			lookup[usIt->first]=tempIndex;
			tempIndex++;
		}

		// now, lookup contains all unknowns and their index in the CImgList
		// the indices are easily accessibly with lookup.find(unknown)->second

		// Prepare vector and context first
		Vec             result;  // Vector to store the result in
		VecScatter      scatter; // context for scattering the result

		// Lookup iterator
		std::map<std::string, unsigned int>::iterator lIt;
		// to store the current global index
		unsigned int globalIndex;

		// Create local vector to store the result in
		ierr = VecCreate(PETSC_COMM_WORLD,&result);CHKERRQ(ierr);
		ierr = VecSetSizes(result,n,n);CHKERRQ(ierr); //local vector
		ierr = VecSetFromOptions(result);CHKERRQ(ierr);
		ierr = VecScatterCreate(x,PETSC_NULL,result,PETSC_NULL,&scatter);
			CHKERRQ(ierr);
		ierr = VecScatterBegin(scatter,x,result,INSERT_VALUES,SCATTER_FORWARD);
			CHKERRQ(ierr);
		ierr = VecScatterEnd(scatter,x,result,INSERT_VALUES,SCATTER_FORWARD);
			CHKERRQ(ierr);

		// now 'result' contains - well - the result. The difference is,
		// that x is a parallel vector (scattered across all machines), but
		// 'result' is a local vector which exists only in the #0 machine
		PetscScalar* res;
		// now make the result-vector available as an array of PetscScalar
		ierr = VecGetArray(result,&res); CHKERRQ(ierr);

		// epxand the CImgList to the needed size
		this->out().assign(lookup.size(),
			globalRoi.getWidth(), globalRoi.getHeight(),
			globalRoi.getDepth(), globalRoi.getDuration(),
			T(0));

		// Due to data locallity, it is performance-wise better to iterate
		// through the unknowns first and fill the CImgList element by element
		// instead of pixel by pixel. Same with the solution vector.

		// for all unknowns
		for(lIt=lookup.begin() ; lIt!=lookup.end();lIt++) {
			// go through the image
			forRoiXYZT(globalRoi,x,y,z,t) {
				// get the current global index
				globalIndex = _pointToGlobalIndex(Point4D<int>(x,y,z,t),
					lIt->first, unknownSizes);
				// and write the results into the output slot 'out'
				this->out()(lIt->second,
					x-globalRoi.xBegin,
					y-globalRoi.yBegin,
					z-globalRoi.zBegin,
					t-globalRoi.tBegin) = T(res[globalIndex]);
			}
		}

		ierr = VecRestoreArray(result,&res); CHKERRQ(ierr);
	}

	// clean up
	for(usIt = unknownSizes.begin() ; usIt != unknownSizes.end() ; usIt++) {
		delete usIt->second;
	}

	ierr = VecDestroy(x); CHKERRQ(ierr);
	ierr = VecDestroy(b); CHKERRQ(ierr);
	ierr = MatDestroy(A); CHKERRQ(ierr);
	ierr = KSPDestroy(ksp); CHKERRQ(ierr);

	if (columns)
		delete[] columns;
	if (values)
		delete[] values;

	return 0;
}

template <typename T>
unsigned int PetscSolver<T>::_addCrossTerms(
		const std::string& unknown,
		const Point4D<int>& p,
		const std::map<std::string, const Roi<int>*>& unknownSizes,
		PetscInt*& columns,
		PetscScalar*& values) const
{
	typename std::set<AbstractSlot<Stencil<T>*>*>::const_iterator sIt;
	std::map<PetscInt, PetscScalar> matrixEntries;

	for(sIt=this->stencils.begin();sIt!=this->stencils.end();sIt++) {
		Stencil<T>* s = (*((InputSlot<Stencil<T>*>*)*sIt))();
		s->updateStencil(unknown,p.x,p.y,p.z,p.t);
		const std::set<std::string>& allUnk = s->getUnknowns();
		typename std::set<std::string>::const_iterator unk;
		for(unk = allUnk.begin(); unk != allUnk.end(); unk++) {
			// current unknown already handled by MetaStencil
			if (*unk == unknown)
				continue;
			// skip emtpy substencils
			const SubStencil<T>& entry = s->get().find(*unk)->second;
			if (entry.pattern.is_empty())
				continue;
			assert (entry.pattern.is_sameXYZC(entry.data));

			cimg_forXYZC(entry.pattern,cx,cy,cz,ct) {
				if (entry.pattern(cx,cy,cz,ct)) {
					Point4D<int> curArg(cx,cy,cz,ct);
					curArg += Point4D<int>(p) - entry.center;
					PetscInt curCol =
							PetscSolver<T>::_pointToGlobalIndex(
									curArg,
									*unk,
									unknownSizes);
					// int() should initialize to zero in std::map,
					// if errors occur here, add further checks
					matrixEntries[curCol] += entry.data(cx,cy,cz,ct);
				}
			}
		}
	}
	unsigned int cur=0;
	typename std::map<PetscInt, PetscScalar>::const_iterator entIt;
	for (entIt = matrixEntries.begin(); entIt != matrixEntries.end(); entIt++) {
		columns[cur] = entIt->first;
		values[cur] = entIt->second;
		cur++;
	}
	return matrixEntries.size();
}

template <typename T>
void PetscSolver<T>::execute() {
	ParameteredObject::execute();
	int errorCode;
	errorCode = petscExecute();
	if (errorCode) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << std::endl;
		msg << "\tPETSc error occured" << std::endl;
		msg << "\tError code:\n\t\t" << errorCode;
		throw std::runtime_error(msg.str().c_str());
	}
}

#endif // _PETSCSOLVER_HXX_
