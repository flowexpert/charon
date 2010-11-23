/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

    This file is part of Charon.

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
/** \file flowIterated4_SpatialPW.cpp
 *  Unit tests for class DataBC
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 02.02.2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#error  GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error  LOCAL_PLUGIN_DIR not defined!
#endif
#ifndef TESTDIR
/// local test source directory
#define TESTDIR ""
#error  TESTDIR not defined!
#endif

#include <iomanip>

#include <charon-core/PluginManager.h>
#include <charon-core/ParameteredObject.h>
#include <charon-core/FileTool.h>
#include <charon-core/ExceptionHandler.h>

#include <charon/FlowComparator.h>
#include <charon/RelaxingIterator.h>
#include <charon/IteratorHelper.h>
#include <charon/Stencils/SpatialPW.h>

/// print header to inform about csv columns
void printHeader(
		std::ostream& strm                /** [out] output stream*/);

/// print current information to given stream
void printInfos(
		std::ostream& strm                /** [out] output stream*/,
		const std::string& curDir         /** [in]  data output directory*/,
		double cur                        /** [in]  relaxation status*/,
		IteratorHelper<double>* h1        /** [in]  inner iterator helper*/,
		IteratorHelper<double>* h2        /** [in]  outer iterator helper*/,
		SpatialPW<double>* stencil        /** [in]  stencil to watch*/,
		FlowComparator<double>* analyzer  /** [in]  result analyzer*/);

/// unit tests
int test() {
	std::ofstream log("SpatialPW.log", std::ios::trunc);
	std::ofstream csv("iteratorTest4_energy.csv", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/flowIterated4.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	SimpleIterator<double>* iterator =
			dynamic_cast<SimpleIterator<double>*>(
					man.getInstance("iterator"));
	assert(iterator);

	RelaxingIterator<double>* relaxator =
			dynamic_cast<RelaxingIterator<double>*>(
					man.getInstance("relaxator"));
	assert(relaxator);

	IteratorHelper<double>* helper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper"));
	assert(helper);

	IteratorHelper<double>* relaxinghelper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("relaxinghelper"));
	assert(helper);

	SpatialPW<double>* spatialPW = dynamic_cast<SpatialPW<double>*>(
			man.getInstance("spatialterm_learned"));
	assert(spatialPW);

	FlowComparator<double>* analyzer = dynamic_cast<FlowComparator<double>*>(
			man.getInstance("analyzer"));
	assert(analyzer);

	std::string curDir = FileTool::getCurrentDir();
	FileTool::changeDir(TESTDIR);

	// make sure that ground truth is available
	ParameteredObject* gen = man.getInstance("seqgen");
	assert(gen);
	gen->execute();

	// connect analyzer input to current flow
	analyzer->result.disconnect();
	analyzer->result.connect(helper->flow);

	// write ground truth
	analyzer->groundtruth().get_append('c').save_cimg(
			(curDir+"_gt.cimg").c_str(),true);

	relaxator->initialize();
	printHeader(csv);

	std::cout << "Iteration: " << std::flush;
	bool contOut, contIn;
	do {
		relaxator->prepareStep();
		double cur = relaxator->getCur();
		printInfos(csv,curDir,cur,helper,relaxinghelper,spatialPW,analyzer);
		iterator->initialize();
		do {
			contIn = iterator->singleStep();
			printInfos(csv,curDir,cur,helper,relaxinghelper,spatialPW,analyzer);
			std::cout << helper->countAll() << " " << std::flush;
		} while (contIn);
		iterator->finalize();
		contOut = relaxator->finishStep();
	} while (contOut);
	relaxator->finalize();

	std::cout << "done" << std::endl;

	double meanEndpointError = analyzer->getMeanEndpointError();

	man.reset();

	sout.assign(std::cout, log);
	sout << "\tmean endpoint error: " << meanEndpointError << std::endl;
	sout.assign();
	csv.close();
	log.close();

	// check results
	if (meanEndpointError >= 0.1) {
		std::cout << "Mean endpoint error too bad!" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void printHeader(std::ostream& strm) {
	strm << "# SpatialPW iteration monitor\n";
	strm << "# globaliterations (g); outer count (o); inner count (i);\n";
	strm << "#  g   o   i       cur        "
			<< "   mean(u)        mean(v)      mean EPE\n";
	strm << "# --- --- ---   ----------    "
			<< "------------   ------------  ------------";
	strm << std::endl;
}

void printInfos(
		std::ostream& strm,
		const std::string& curDir,
		double cur,
		IteratorHelper<double>* helper1,
		IteratorHelper<double>* helper2,
		SpatialPW<double>* stencil,
		FlowComparator<double>* analyzer) {
	// only print informations once for each counter value
	static int count = -1;
	if(count == int(helper1->countAll()))
		return;

	stencil->execute();

	// print out information
	strm << std::setw(5) << (count = helper1->countAll())
			<< std::setw(4) << helper2->count()
			<< std::setw(4) << helper1->count();
	if (count > 0)
		strm << std::setw(13) << std::scientific << std::setprecision(4)
			<< cur;
	else
		// suppress output of first data point where no calculation
		// took place
		strm << "       na    ";


	assert(helper1->flow().size() == 2u);
	cimglist_for(helper1->flow(),kk)
		strm << std::setw(15) << std::scientific << std::setprecision(6)
				<< helper1->flow()[kk].mean();

	strm << " " << std::setw(13) << std::scientific
			<< std::setprecision(6) << analyzer->getMeanEndpointError();
	strm << std::endl;

	// write out stencil values
	if(count > 0) {
		static cimg_library::CImgList<double> lambdas;
		lambdas.push_back(stencil->apply(helper1->flow(), 0u));
		lambdas.save_cimg(
				(curDir + "_lambdas.cimg").c_str(),true);
	}
}

/// run unit tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
