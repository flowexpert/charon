/*  Copyright (C) 2012 Gerald Mwangi

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
/** @file   groupVirtualSlot.cpp
 *  @brief  Unit tests VirtualSlot and ParameteredGroupObject class.
 *  @date   06.02.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */

#include "../include/charon-core/IfGroup.h"
#include "../include/charon-core/ParameteredGroupObject.h"
#include "../include/charon-core/DataManagerParameterFile.hxx"
#include "../include/charon-core/ExceptionHandler.h"

void createGroup(PluginManager& man,std::string name,std::string workflowfile)
{
	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("GroupObject",name));

	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	path.push_back(MODULE_DIR);
	group->workFlowFile=workflowfile;
	group->pluginPaths=path;
	group->initialize();

}

void createWorkflows()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);

	//Create child workflow
	ParameteredObject* in=man.createInstance("InputSlotBundle","inputs");
	in->setParameter("num_slots",2);
	ParameterFile pf;

	in->saveParameters(pf);
	in->prepareDynamicInterface(pf);

	ParameteredObject* intreader=man.createInstance("VirtualSlotTestReader","reader");
	man.connect("inputs.VirtualSlot-out0","reader.integer");
	man.connect("inputs.VirtualSlot-out1","reader.image");
	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-childreader.wrp";
	man.saveParameterFile(childworkflow.str());

	man.reset();

	//Create child workflow
	ParameteredObject* out=man.createInstance("OutputSlotBundle","outputs");
	out->setParameter("num_slots",2);
	ParameterFile pf2;

	out->saveParameters(pf2);
	out->prepareDynamicInterface(pf2);

	man.createInstance("VirtualSlotTestWriter","writer");
	man.connect("outputs.VirtualSlot-in0","writer.integer");
	man.connect("outputs.VirtualSlot-in1","writer.image");
	std::stringstream childworkflow2;


	childworkflow2<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-childwriter.wrp";
	man.saveParameterFile(childworkflow2.str());

	man.reset();

	//Create datapassing workflow
	ParameteredObject* in2=man.createInstance("InputSlotBundle","inputs");
	in2->setParameter("num_slots",2);
	ParameterFile pf3;

	in2->saveParameters(pf3);
	in2->prepareDynamicInterface(pf3);

	ParameteredObject* out2=man.createInstance("OutputSlotBundle","outputs");
	out2->setParameter("num_slots",2);
	ParameterFile pf4;

	out2->saveParameters(pf4);
	out2->prepareDynamicInterface(pf4);

	man.connect("inputs.VirtualSlot-out0","outputs.VirtualSlot-in0");
	man.connect("inputs.VirtualSlot-out1","outputs.VirtualSlot-in1");

	std::stringstream childworkflow3;


	childworkflow3<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-childdatapassing.wrp";
	man.saveParameterFile(childworkflow3.str());

}

void testDataToGroup()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);
	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-childreader.wrp";


	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("GroupObject","TestGroup"));

	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	path.push_back(MODULE_DIR);
	group->workFlowFile=childworkflow.str();
	group->pluginPaths=path;
	group->initialize();
	std::stringstream pworkflow;


	pworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-parent.wrp";

	man.createInstance("VirtualSlotTestWriter","writer");
	man.connect("writer.integer","TestGroup.VirtualSlot-in0");
	man.connect("writer.image","TestGroup.VirtualSlot-in1");
	man.saveParameterFile(pworkflow.str());
	man.runWorkflow();


}

void testDataFromGroup()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);
	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-childwriter.wrp";


	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("GroupObject","TestGroup"));

	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	path.push_back(MODULE_DIR);
	group->workFlowFile=childworkflow.str();
	group->pluginPaths=path;
	group->initialize();
	std::stringstream pworkflow;


	pworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-parent2.wrp";

	man.createInstance("VirtualSlotTestReader","reader");
	man.connect("reader.integer","TestGroup.VirtualSlot-out0");
	man.connect("reader.image","TestGroup.VirtualSlot-out1");
	man.saveParameterFile(pworkflow.str());
	man.runWorkflow();

}

void testDataPassing()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);
	std::string dir=FileTool::getCurrentDir()+FileTool::slash;
	createGroup(man,"writer",dir+"TestVirtualSlot-childwriter.wrp");
	createGroup(man,"datapassing",dir+"TestVirtualSlot-childdatapassing.wrp");
	createGroup(man,"reader",dir+"TestVirtualSlot-childreader.wrp");

	man.connect("writer.VirtualSlot-out0","datapassing.VirtualSlot-in0");
	man.connect("writer.VirtualSlot-out1","datapassing.VirtualSlot-in1");

	man.connect("datapassing.VirtualSlot-out0","reader.VirtualSlot-in0");
	man.connect("datapassing.VirtualSlot-out1","reader.VirtualSlot-in1");
	std::stringstream pworkflow;


	pworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestVirtualSlot-parent3.wrp";
	man.saveParameterFile(pworkflow.str());
	man.runWorkflow();
}

int main()
{
    sout.assign(std::cout);
    int ret = EXIT_SUCCESS;
	ret |= ExceptionHandler::run(createWorkflows);
    ret |= ExceptionHandler::run(testDataToGroup);
    ret |= ExceptionHandler::run(testDataFromGroup);
	ret |= ExceptionHandler::run(testDataPassing);

    return ret;


}
