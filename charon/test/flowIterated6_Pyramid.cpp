/*  Copyright (C) 2009 Jens-Malte Gottfried <jmgottfried@web.de>

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
/** \file flowIterated6_Pyramid.cpp
 *  \author <a href="Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried </a>
 *  \date 02.02.2010
 *  \test Iterated Horn&Schunck algo with PyramidRescale
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

/// save multiple png images for debugging
#define SAVE_IMAGES

// needed CPP definitions
#ifndef GLOBAL_PLUGIN_DIR
/// set this to the global plugin path
#define GLOBAL_PLUGIN_DIR ""
#error GLOBAL_PLUGIN_DIR not defined!
#endif
#ifndef LOCAL_PLUGIN_DIR
/// set this to the plugin build path (local plugin path)
#define LOCAL_PLUGIN_DIR ""
#error LOCAL_PLUGIN_DIR not defined!
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
#include <charon/SimpleIterator.h>
#include <charon/IteratorHelper.h>
#include <charon/FlowComparator.h>
#include <charon/PyramidRescale.h>
#include <charon-utils/FileReader.h>
#include <charon-utils/Flow2HSV.h>
#include <charon-utils/FlowQuiver.h>
#include <charon-utils/WarpSymmetric.h>

/// dummy class providing CImg output slot
class Dummy : public ParameteredObject {
public:
	/// dummy output slot
	OutputSlot<cimg_library::CImgList<double> > out;
	/// standard constructor
	Dummy(const std::string& name = "" /** [in] instance name*/) :
			ParameteredObject("dummy",name, "dummy class") {
		_addOutputSlot(out, "out", "out", "CImgList<double>");
	}
};

/// simple pyramid test
int testPyramid() {
	std::cout << "Executing simple workflow..." << std::endl;
	std::ofstream log("flowIterated6_PyramidSimple.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);

	Dummy dummy1, dummy2;
	IteratorHelper<double> helper;
	SimpleIterator<double> iter;
	PyramidRescale<double> pyramid;

	// set up connections
	dummy1.out.connect(helper.in);
	dummy2.out.connect(helper.initFlow);
	helper.sequence.connect(pyramid.seqIn);
	helper.flow.connect(pyramid.flowIn);
	helper.self.connect(iter.helper);
	helper.count.connect(pyramid.level);
	pyramid.flowOut.connect(iter.flow);
	pyramid.flowOut.connect(iter.flowInit);

	// set iterations
	const unsigned int levels = 4;
	pyramid.levels() = iter.maxRuns() = levels;
	iter.accumulate() = false;

	// set up images
	cimg_library::CImgList<double>& seq = dummy1.out();
	cimg_library::CImgList<double>& flow = dummy2.out();
	const cimg_library::CImgList<double>& pyFlow = pyramid.flowOut();
	const unsigned int& cur = helper.count();
	const double fx = 256., fy = -128.;
	const double tolerance = 1.e-4;

	seq.assign(1,128,64,1,2,0.);
	flow.assign(
			cimg_library::CImg<double>(128,64,1,1,fx),
			cimg_library::CImg<double>(128,64,1,1,fy));

	// emulate iter.run()
	iter.prepareIterations();
	bool cont;
	do {
		// test in single step mode
		cont = iter.singleStep();

		// check actual flow values
		double diff = pyFlow[0].mean()-fx*std::pow(2.,-(double)levels+cur);
		assert(std::abs(diff) < tolerance);
		diff = pyFlow[1].mean()-fy*std::pow(2.,-(double)levels+cur);
		assert(std::abs(diff) < tolerance);
		assert(pyFlow[0].variance() < tolerance);
		assert(pyFlow[1].variance() < tolerance);
	} while (cont);
	iter.finishIterations();

	assert(std::abs(pyFlow[0].mean()-fx) < tolerance);
	assert(std::abs(pyFlow[1].mean()-fy) < tolerance);

	sout.assign();
	std::cout << "Workflow execution finished.\n" << std::endl;

	return EXIT_SUCCESS;
}

/// test using workflow "flowIterated6.wrp"
int testWorkflow() {
	std::ofstream log("flowIterated6_Pyramid.log", std::ios::trunc);
	assert(log.good());
	sout.assign(log);
	std::ofstream epeLog("flowIterated6_Pyramid.csv", std::ios::trunc);
	assert(epeLog.good());

	// load plugin manager
	PluginManager man(GLOBAL_PLUGIN_DIR, LOCAL_PLUGIN_DIR);

	std::string wDir = FileTool::getCurrentDir();
	FileTool::changeDir(TESTDIR);

	// start tests
	std::string testfile("flowIterated6.wrp");
	std::cout << "Loading parameter file \"" << testfile;
	std::cout << "\"" << std::endl;
	man.loadParameterFile(testfile);

	// get test instances
	FlowComparator<double>* comparator = dynamic_cast<FlowComparator<double>*>(
			man.getInstance("analyzer"));
	assert(comparator);
	SimpleIterator<double>* iterator =
			dynamic_cast<SimpleIterator<double>*>(
					man.getInstance("iterator"));
	assert(iterator);
	IteratorHelper<double>* helper =
			dynamic_cast<IteratorHelper<double>*>(
					man.getInstance("helper"));
	assert(helper);
	PyramidRescale<double>* pyramid =
			dynamic_cast<PyramidRescale<double>*>(
					man.getInstance("pyramid"));
	assert(pyramid);
	PyramidRescale<double>* pyramidGT =
			dynamic_cast<PyramidRescale<double>*>(
					man.getInstance("pyramidgt"));
	assert(pyramidGT);
#ifdef SAVE_IMAGES
	FileReader<double>* readgt =
			dynamic_cast<FileReader<double>*>(
					man.getInstance("readgt"));
	assert(readgt);
	Flow2HSV<double>* hsv =
			dynamic_cast<Flow2HSV<double>*>(
					man.getInstance("tohsv"));
	assert(hsv);
	FlowQuiver<double>* quiver =
			dynamic_cast<FlowQuiver<double>*>(
					man.getInstance("quiv"));
	assert(quiver);
	Flow2HSV<double>* hsv2 =
			dynamic_cast<Flow2HSV<double>*>(
					man.getInstance("hsv2"));
	assert(hsv2);
	FlowQuiver<double>* quiver2 =
			dynamic_cast<FlowQuiver<double>*>(
					man.getInstance("quiv2"));
	assert(quiver2);
	WarpSymmetric<double>* warper =
			dynamic_cast<WarpSymmetric<double>*>(
					man.getInstance("warper"));
	assert(warper);
#endif

	comparator->result.disconnect();
	comparator->result.connect(helper->flow);

	std::cout << "Executing workflow..." << std::endl;
	iterator->prepareIterations();
	bool cont;
	do {
		pyramidGT->run();
		cont = iterator->singleStep();
		epeLog << std::setw(4) << helper->count() << " "
				<< std::setw(13) << std::scientific << std::setprecision(6)
				<< comparator->getMeanEndpointError() << std::endl;
#ifdef SAVE_IMAGES
		std::ostringstream fName1;
		fName1<<wDir<<"/flowIterated6_iteration"<<helper->count()<<".png";
		pyramid->seqOut()[0].get_channel(0).get_append(
				pyramid->seqOut()[0].get_channel(1)).save(
						fName1.str().c_str());
		std::ostringstream fName2;
		fName2<<wDir<<"/flowIterated6_warped"<<helper->count()<<".png";
		warper->out()[0].get_channel(0).get_append(
				warper->out()[0].get_channel(1)).save(
						fName2.str().c_str());
		std::ostringstream fName3;
		fName3<<wDir<<"/flowIterated6_flow"<<helper->count()<<".png";
		quiver->run();
		quiver->out().save(fName3.str().c_str());
		std::ostringstream fName4;
		fName4<<wDir<<"/flowIterated6_initFlow"<<helper->count()<<".png";
		quiver2->run();
		quiver2->out().save(fName4.str().c_str());
#endif
	} while (cont);
	iterator->finishIterations();

#ifdef SAVE_IMAGES
	// save flow result
	hsv->resetExecuted();
	hsv->flow.disconnect() ;
	hsv->flow.connect(iterator->result);
	quiver->flow.disconnect() ;
	quiver->flow.connect(iterator->result);
	hsv2->resetExecuted();
	hsv2->flow.disconnect() ;
	hsv2->flow.connect(readgt->out);
	quiver2->flow.disconnect() ;
	quiver2->flow.connect(readgt->out);
	quiver2->run();
	std::ostringstream fName;
	fName<<wDir<<"/flowIterated6_result.png";
	quiver->run();
	quiver->out()[0].get_append(quiver2->out()[0]).save(fName.str().c_str());
#endif

	std::cout << "Workflow execution finished.\n" << std::endl;

	std::cout << "mean endpoint error: "
			<< comparator->getMeanEndpointError() << std::endl;
	assert(comparator->getMeanEndpointError() <= 0.5);
	epeLog.close();

	man.reset();
	sout.assign();
	log.close();

	return EXIT_SUCCESS;
}

/// start tests with exception handling
int main() {
	int ret = ExceptionHandler::run(testPyramid);
	if (ret != EXIT_SUCCESS)
		return ret;

	ret = ExceptionHandler::run(testWorkflow);
	return ret;
}
