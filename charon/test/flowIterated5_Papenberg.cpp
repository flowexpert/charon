/*  Copyright (C) 2011 Jens-Malte Gottfried

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file flowIterated5_Papenberg.cpp
 *  Unit tests for class DataBC and SpatialPW as well as Psi functions
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 13.01.2011
 *  \test
 *  using data and spatial term from Sun's Learning Flow algorithm
 *  with Charbonnier psi-functions on Yosemite sequence
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

#include <charon/RelaxingIterator.h>
#include <charon/IteratorHelper.h>
#include <charon/FlowComparator.h>
#include <charon/Stencils/SpatialPW.h>
#include <charon/Stencils/DataConstant.h>

/// prefix for output files
#define PREFIX "flowIterated5"

/// print header to inform about csv columns
void printHeader(
		std::ostream& strm                /** [out] output stream*/);

/// print current information to given stream
void printInfos(
		std::ostream& strm                /** [out] output stream*/,
		const std::string& oDir           /** [in]  output dir*/,
		double cur                        /** [in]  relaxation status*/,
		IteratorHelper<double>* h1        /** [in]  inner iterator helper*/,
		IteratorHelper<double>* h2        /** [in]  outer iterator helper*/,
		SpatialPW<double>* spatial        /** [in]  stencil1 to watch*/,
		DataConstant<double>* data        /** [in]  stencil2 to watch*/,
		FlowComparator<double>* analyzer  /** [in]  result analyzer*/);

/// unit tests
int test() {
	std::ofstream log("flowIterated5_Papenberg.log", std::ios::trunc);
	std::ofstream csv("flowIterated5_energy.csv", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	// start tests
	std::string testfile(TESTDIR "/flowIterated5.wrp");
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
			man.getInstance("spatial_pb"));
	assert(spatialPW);

	DataConstant<double>* dataBC = dynamic_cast<DataConstant<double>*>(
			man.getInstance("data_pb"));
	assert(dataBC);

	FlowComparator<double>* analyzer = dynamic_cast<FlowComparator<double>*>(
			man.getInstance("analyzer"));
	assert(analyzer);

	std::string curDir = FileTool::getCurrentDir();
	FileTool::changeDir(TESTDIR);

	// make sure that ground truth is available
	ParameteredObject* gen = man.getInstance("read_flow");
	assert(gen);
	gen->run();

	// connect analyzer input to current flow
	analyzer->result.disconnect();
	analyzer->result.connect(helper->flow);

	// write ground truth
	analyzer->groundtruth().get_append('c').save_cimg(
			(curDir+"/" PREFIX "_gt.cimg").c_str(),true);

	relaxator->prepareIterations();
	printHeader(csv);

	std::cout << "Iteration: " << std::flush;
	bool contOut, contIn;
	do {
		relaxator->prepareStep();
		double cur = relaxator->getCur();
		printInfos(
				csv,curDir,cur,helper,relaxinghelper,
				spatialPW,dataBC,analyzer);
		iterator->prepareIterations();
		do {
			contIn = iterator->singleStep();
			printInfos(
					csv,curDir,cur,helper,relaxinghelper,
					spatialPW,dataBC,analyzer);
			std::cout << helper->countAll() << " " << std::flush;
		} while (contIn);
		iterator->finishIterations();
		contOut = relaxator->finishStep();
	} while (contOut);
	relaxator->finishIterations();

	std::cout << "done" << std::endl;

	double meanEndpointError = analyzer->getMeanEndpointError();

	man.reset();

	sout.assign(std::cout, log);
	sout << "\tmean endpoint error: " << meanEndpointError << std::endl;
	sout.assign();
	csv.close();
	log.close();

	// check results
	// should be masked due to false values at black "cloud-area"
	if (meanEndpointError >= 0.25) {
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
		const std::string& oDir,
		double cur,
		IteratorHelper<double>* helper1,
		IteratorHelper<double>* helper2,
		SpatialPW<double>* spatial,
		DataConstant<double>* data,
		FlowComparator<double>* analyzer) {
	// only print informations once for each counter value
	static int count = -1;
	if(count == int(helper1->countAll()))
		return;

	spatial->run();

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

	const cimg_library::CImgList<double>& curFlow = helper1->flow();
	assert(curFlow.size() == 2u);
	assert(curFlow[0].spectrum() == 1u);
	assert(curFlow[1].spectrum() == 1u);
	cimglist_for(curFlow,kk)
		strm << std::setw(15) << std::scientific << std::setprecision(6)
				<< curFlow[kk].mean();

	strm << " " << std::setw(13) << std::scientific
			<< std::setprecision(6) << analyzer->getMeanEndpointError();
	strm << std::endl;

	// write out stencil values and flow
	if(count > 0) {
		static cimg_library::CImgList<double> flow(curFlow);
		cimglist_for(flow,cc)
			flow[cc].append(curFlow[cc],'c');
		std::string flowFName = oDir + "/" PREFIX "_flows.cimg";
		flow.save_cimg(flowFName.c_str(),true);

		static cimg_library::CImgList<double> lambdas;
		lambdas.push_back(spatial->apply(curFlow, 0u));
		lambdas.save_cimg(
				(oDir + "/" PREFIX "_lambdas.cimg").c_str(),true);

		// update weight map
		static cimg_library::CImgList<double> weightMap, dummy;
		const std::string weightsFName =
				(oDir + "/" PREFIX "_weightMaps.cimg");

		// arguments of apply are ignored and may be set arbitrarily
		helper1->resetExecuted();
		data->run();
		weightMap.push_back(data->apply(dummy,0u));
		weightMap.save_cimg(weightsFName.c_str(),true);
	}
}

/// run unit tests with exception handling
int main() {
	return ExceptionHandler::run(test);
}
