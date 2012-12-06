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
#ifndef PARAMETEREDGROUPOBJECT_H
#define PARAMETEREDGROUPOBJECT_H

#include "ParameteredObject.h"
#include "PluginManager.h"
#include "SlotBundleInterfaces.h"







///// The function getSlotVector exposes _outputSlots to the parent group.
//class OutputSlotBundle: public SlotBundle
//{
//public:
//	OutputSlotBundle(const std::string& className,
//			const std::string& name = "", const std::string& doc = "");
//	std::vector<VirtualOutputSlot*>&  getSlotVector();
//	std::vector<VirtualInputSlot*>& getInternalSlotVector();
//protected:
//	void _addAllSlots();
//	void _removeAllSlots();
//};

class  charon_core_DLL_PUBLIC ParameteredGroupObject : public ParameteredObject
{
public:
	/// Default constructor.
	/** Init class name with given string.
	 *  Also generates a unique instance name if necessary.
	 *  \param className    Initialization value for className property.
	 *  \param name         Instance name (auto generation if empty)
	 *  \param doc  Class docstring (for metadata)
	 */
	ParameteredGroupObject(const std::string& className="ParameteredGroupObject",
		const std::string& name = "", const std::string& doc = "");

	/// plugin search paths which are used when executing this loop
	ParameterList<std::string> pluginPaths;
	
	/// work flow file which will be executed in this loop
	Parameter<std::string> workFlowFile;

	/// Default deconstructor
	virtual ~ParameteredGroupObject();

	/// Prepare the interface.
	/** Loads the child workflow file given by the parent workflow file,
	  * and searches for the input and output slot bundles. These, if  found, are added to the interface
	  */
	virtual void prepareDynamicInterface(const ParameterFile &file);

	/// Initialization
	/** Calls initialize Group */
	void initialize();

	/// Group Initialization
	/** Does nothing. Can be used in derived classes. */
	virtual void initializeGroup();

	/// Execution
	/** Calls executeGroup() */
	void execute();

	/// Execution
	/** Executes the group by calling executeWorkflow() in PluginManager */
	virtual void executeGroup();

	/// Finalization
	/** Calls finalizeGroup */
	void finalize();

	/// Finalization
	/** Does nothing. Can be used in derived classes. */
	virtual void finalizeGroup();




	/// LoopOutToInput
	/** Loop the given output to an input.
	 *  The loop connection is only valid after one iteration of the group
	 */
	void loopInputToOutput(int input,int output);

	/// breakLoop
	/** Break a loop connection.
	  */
	void breakLoop(int input);

	/// enableLoopConnections
	/** enable all looped connections
	  */
	void enableLoopConnections();

	/// disableLoopConnections
	void disableLoopConnections();

	/// Custom save
	/** saves the internal workflow */
	virtual void onSave(ParameterFile &pf) const;

	/// Custom load
	/** Load the loop connections*/
	virtual void onLoad(const ParameterFile &pf, const PluginManagerInterface *man);

protected:
	PluginManager* _pluginMan;
	InputSlotBundleIntf* _inputs;
	OutputSlotBundleIntf* _outputs;
	std::map<int,VirtualOutputSlot*> _loopedSlots;
	std::vector<Parameter<int>* > _loopOutputNumber; //For ever
};

#endif // PARAMETEREDGROUPOBJECT_H
