/*	Copyright (C) 2012 Gerald Mwangi

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

#include "../include/charon-core/WhileGroup.h"
#include "../include/charon-core/ExceptionHandler.h"
/** @file   testWhileGroup.cpp
 *  @brief  Unit tests WhileGroup and WhileGroupStatement class.
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */

void test()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);


	//Create TestWhileGroup-child.wrp
	ParameteredObject* in=man.createInstance("InputSlotBundle","inbundle");

	ParameteredObject* out=man.createInstance("OutputSlotBundle","outbundle");

	ParameteredObject* internReader=man.createInstance("WhileGroupTestReader","InternReader");

	ParameteredObject* internWriter=man.createInstance("WhileGroupTestWriter","InternWriter");

	man.createInstance("WhileGroupTestStatement","statement");

	in->setParameter("num_slots",2);
	ParameterFile pf;

	in->saveParameters(pf);
	in->prepareDynamicInterface(pf);

	out->setParameter("num_slots",3);

	out->saveParameters(pf);
	out->prepareDynamicInterface(pf);

	internWriter->setParameter("inside_group",true);
	internWriter->saveParameters(pf);
	internWriter->prepareDynamicInterface(pf);

	man.connect("inbundle.VirtualSlot-out0","InternReader.in1");
	man.connect("inbundle.VirtualSlot-out1","InternReader.in2");
	man.connect("outbundle.VirtualSlot-in0","InternWriter.out1");
	man.connect("outbundle.VirtualSlot-in1","InternWriter.out2");
	man.connect("outbundle.VirtualSlot-in2","statement.iteration");
	man.saveParameterFile("TestWhileGroup-child.wrp");
	man.reset();


	//Create TestWhileGroup-childloop.wrp
	ParameteredObject* inloop=man.createInstance("InputSlotBundle","inbundle");

	ParameteredObject* outloop=man.createInstance("OutputSlotBundle","outbundle");

	ParameteredObject* internReaderloop=man.createInstance("WhileGroupTestReader","InternReader");

	ParameteredObject* internWriterloop=man.createInstance("WhileGroupTestWriter","InternWriter");

	man.createInstance("WhileGroupTestStatement","statement");

	inloop->setParameter("num_slots",2);
	ParameterFile pfloop;

	inloop->saveParameters(pfloop);
	inloop->prepareDynamicInterface(pfloop);

	outloop->setParameter("num_slots",3);

	outloop->saveParameters(pf);
	outloop->prepareDynamicInterface(pf);

	//InternReader tests loop
	internReaderloop->setParameter("loopmode",true);
	internReaderloop->saveParameters(pf);
	internReaderloop->prepareDynamicInterface(pf);

	internWriterloop->setParameter("inside_group",true);
	internWriterloop->saveParameters(pf);
	internWriterloop->prepareDynamicInterface(pf);
	man.connect("inbundle.VirtualSlot-out0","InternReader.in1");
	man.connect("inbundle.VirtualSlot-out1","InternReader.in2");
	man.connect("outbundle.VirtualSlot-in0","InternWriter.out1");
	man.connect("outbundle.VirtualSlot-in1","InternWriter.out2");
	man.connect("outbundle.VirtualSlot-in2","statement.iteration");
	man.saveParameterFile("TestWhileGroup-childloop.wrp");
	man.reset();


	//Create TestWhileGroup-parent.wrp
	//Create TestGroup. TestGroup just iterates over TestWhileGroup-child.wrp 10 times. No Loops.
	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("WhileGroup","TestGroup"));

	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestWhileGroup-child.wrp";



	group->workFlowFile=childworkflow.str();
	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	path.push_back(MODULE_DIR);
	group->pluginPaths=path;

	group->initialize();

	ParameteredObject* externReader=man.createInstance("WhileGroupTestReader","ExternReader");

	ParameteredObject* externWriter=man.createInstance("WhileGroupTestWriter","ExternWriter");

	man.connect("TestGroup.VirtualSlot-out0","ExternReader.in1");
	man.connect("TestGroup.VirtualSlot-out1","ExternReader.in2");
	man.connect("TestGroup.VirtualSlot-in0","ExternWriter.out1");
	man.connect("TestGroup.VirtualSlot-in1","ExternWriter.out2");




	//Create TestGroupLoop
	//TestGroupLoop iterates over TestWhileGroup-childloop.wrp and loops its output 0 to input 0!!
	ParameteredGroupObject* grouploop=dynamic_cast<ParameteredGroupObject*>(man.createInstance("WhileGroup","TestGroupLoop"));

	std::stringstream childworkflowloop;


	childworkflowloop<<FileTool::getCurrentDir()<<FileTool::slash<<"TestWhileGroup-childloop.wrp";



	grouploop->workFlowFile=childworkflowloop.str();

	grouploop->pluginPaths=path;

	grouploop->initialize();


	grouploop->setParameter("loop_input_0_to_output",0);
	grouploop->saveParameters(pf);
	grouploop->onLoad(pf,&man);

	ParameteredObject* externReaderloop=man.createInstance("WhileGroupTestReader","ExternReaderLoop");

	ParameteredObject* externWriterloop=man.createInstance("WhileGroupTestWriter","ExternWriterLoop");

	man.connect("TestGroupLoop.VirtualSlot-out0","ExternReaderLoop.in1");
	man.connect("TestGroupLoop.VirtualSlot-out1","ExternReaderLoop.in2");
	man.connect("TestGroupLoop.VirtualSlot-in0","ExternWriterLoop.out1");
	man.connect("TestGroupLoop.VirtualSlot-in1","ExternWriterLoop.out2");

	man.saveParameterFile("/home/gmwangi/Test.wrp");

	std::stringstream parentwrp;


	parentwrp<<FileTool::getCurrentDir()<<FileTool::slash<<"TestWhileGroup-parent.wrp";

	man.saveParameterFile(parentwrp.str());

	//man.reset();

	//man.loadParameterFile(parentwrp.str());
	man.runWorkflow();




}

int main()
{
    sout.assign(std::cout);
    int ret = EXIT_SUCCESS;

	ret |= ExceptionHandler::run(test);
	//ret |= ExceptionHandler::run(testWhileIterationLoop);

    return ret;

}


