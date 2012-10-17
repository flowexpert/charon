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

#include "../include/charon-core/ParameteredGroupObject.h"
#include "../include/charon-core/ParameteredObject.hxx"
#include "../include/charon-core/InputSlotBundle.h"
#include "../include/charon-core/OutputSlotBundle.h"
#include "../include/charon-core/ExceptionHandler.h"

/** @file   testParameterGropupeObject.cpp
 *  @brief  Unit tests ParameteredBroupObject class.
 *  @date   15.10.2012
 *  @author <a href="mailto:gerald.mwangi@gmx.de">Gerald Mwangi</a>
 */

int numins=10;
int numouts=20;
void createWorkFlowWithBundle()
{
	PluginManager man(CHARON_PLUGINS);


	ParameteredObject* in=man.createInstance("InputSlotBundle");

	ParameteredObject* out=man.createInstance("OutputSlotBundle");

	in->setParameter("num_slots",numins);
	//ParameterFile pf;

	//in->saveParameters(pf);
	//in->prepareDynamicInterface(pf);

	out->setParameter("num_slots",numouts);

	//out->saveParameters(pf);
	//out->prepareDynamicInterface(pf);


	man.saveParameterFile("TestParameteredGroupObject-child.wrp");

	man.reset();

	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("GroupObject","TestGroup"));

	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestParameteredGroupObject-child.wrp";



	group->workFlowFile=childworkflow.str();
	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	group->pluginPaths=path;

	group->initialize();
	man.saveParameterFile("TestParameteredGroupObject-parent.wrp");



}

int testLoadInputSlots()
{
	PluginManager man(CHARON_PLUGINS);


	man.loadParameterFile("TestParameteredGroupObject-parent.wrp");
	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.getInstance("TestGroup"));
	if(!group)
	{
		std::cerr<<"Could not find TestGroup in TestParameteredGroupObject-parent.wrp"<<std::endl;
		return EXIT_FAILURE;
	}
	std::map<std::string, Slot*> inputs=group->getInputSlots();
	if(inputs.size()!=numins)
	{
		std::cerr<<"Incorrect number of inputs in TestGroup"<<std::endl;
		return EXIT_FAILURE;
	}
	for(int i=0;i<numins;i++)
	{
		std::stringstream name;
		name<<"VirtualSlot-in"<<i;

		std::map<std::string, Slot*>::iterator it=inputs.find(name.str());
		if(it==inputs.end())
		{
			std::cerr<<"Could not find Slot "<<name.str()<<std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			Slot* sl=it->second;
			if(sl->getDisplayName()!=name.str())
			{
				std::cerr<<"Wrong displayname "<<sl->getDisplayName()<<" of slot in TestGroup"<<std::endl;
				std::cerr<<"Should be "<<name.str()<<std::endl;
				return EXIT_FAILURE;
			}
			std::string isType, refType;
			if(i==0)
				refType="int";
			else
				refType="virtual";

			if(sl->getType()!=refType)
			{
				std::cerr<<"Wrong type "<<sl->getType()<<" of slot in TestGroup"<<std::endl;
				std::cerr<<"Should be "<<refType<<std::endl;
				return EXIT_FAILURE;
			}



		}



	}
}


int testLoadOutputSlots()
{
	PluginManager man(CHARON_PLUGINS);


	man.loadParameterFile("TestParameteredGroupObject-parent.wrp");
	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.getInstance("TestGroup"));
	if(!group)
	{
		std::cerr<<"Could not find TestGroup in TestParameteredGroupObject-parent.wrp"<<std::endl;
		return EXIT_FAILURE;
	}
	std::map<std::string, Slot*> outputs=group->getOutputSlots();
	if(outputs.size()!=numouts)
	{
		std::cerr<<"Incorrect number of inputs in TestGroup"<<std::endl;
		return EXIT_FAILURE;
	}



	for(int i=0;i<numouts;i++)
	{
		std::stringstream name;
		name<<"VirtualSlot-out"<<i;

		std::map<std::string, Slot*>::iterator it=outputs.find(name.str());
		if(it==outputs.end())
		{
			std::cerr<<"Could not find Slot "<<name.str()<<std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			Slot* sl=it->second;
			if(sl->getDisplayName()!=name.str())
			{
				std::cerr<<"Wrong displayname "<<sl->getDisplayName()<<" of slot in TestGroup"<<std::endl;
				std::cerr<<"Should be "<<name.str()<<std::endl;
				return EXIT_FAILURE;
			}
			std::string isType, refType;
			if(i==0)
				refType="int";
			else
				refType="virtual";

			if(sl->getType()!=refType)
			{
				std::cerr<<"Wrong type "<<sl->getType()<<" of slot in TestGroup"<<std::endl;
				std::cerr<<"Should be "<<refType<<std::endl;
				return EXIT_FAILURE;
			}

		}



	}
}

void createWorkFlowWithBundleAndDummyModule()
{
	PluginManager man(MODULE_DIR,CHARON_PLUGINS);


	ParameteredObject* in=man.createInstance("InputSlotBundle","inbundle");

	ParameteredObject* out=man.createInstance("OutputSlotBundle","outbundle");


	man.createInstance("CasedDummy","dummy");

	in->setParameter("num_slots",numins);
	ParameterFile pf;

	in->saveParameters(pf);
	in->prepareDynamicInterface(pf);

	out->setParameter("num_slots",numouts);

	out->saveParameters(pf);
	out->prepareDynamicInterface(pf);


	man.connect("inbundle.VirtualSlot-out0","dummy.iN");
	man.connect("outbundle.VirtualSlot-in0","dummy.oUt");
	man.saveParameterFile("TestParameteredGroupObject-child.wrp");

	man.reset();

	ParameteredGroupObject* group=dynamic_cast<ParameteredGroupObject*>(man.createInstance("GroupObject","TestGroup"));

	std::stringstream childworkflow;


	childworkflow<<FileTool::getCurrentDir()<<FileTool::slash<<"TestParameteredGroupObject-child.wrp";



	group->workFlowFile=childworkflow.str();
	std::vector<std::string> path;
	path.push_back(CHARON_PLUGINS);
	path.push_back(MODULE_DIR);
	group->pluginPaths=path;

	group->initialize();
	man.saveParameterFile("TestParameteredGroupObject-parent.wrp");



}




int main()
{
	sout.assign(std::cout);
	int ret = EXIT_SUCCESS;



	ret |= ExceptionHandler::run(createWorkFlowWithBundleAndDummyModule);
	ret |= ExceptionHandler::run(testLoadInputSlots);
	ret |= ExceptionHandler::run(testLoadOutputSlots);

	return ret;


}
