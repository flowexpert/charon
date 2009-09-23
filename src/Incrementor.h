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
/**
 *  @file Incrementor.h
 *  @brief declaration of abstract class Incrementor
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef INCREMENTOR_H_
#define INCREMENTOR_H_

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef incrementor_EXPORTS
///Visual C++ specific code
#define incrementor_DECLDIR __declspec(dllexport)
#else
#define incrementor_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define incrementor_DECLDIR
#endif

#include <ParameteredObject.h>
#include <IncrementorParameter.h>
#include <vector>

/// The incrementor class compute the parameters for the blockmatching algorithm
template<typename T>
class incrementor_DECLDIR Incrementor: public TemplatedParameteredObject<T>
{
protected:
	/// @param listOfParams list of computed parameters
	std::vector<IncrementorParameter<T>*> listOfParams;

public:
	/// standard constructor
	Incrementor(const std::string& name, const std::string& pluginName);
	/// set of inputslots of type IncrementorParameter
	InputSlot<IncrementorParameter<T>*> paramList;
	/// this outputslot
	OutputSlot<Incrementor<T>*> out;
	virtual void execute() =0;

	/** computes the next Step
	 *  the return is 0 if all max not reached and 1 if all max are reached
	 */
	virtual bool doStep() =0;
	///returns a list of the computed Parameters
	const std::vector<IncrementorParameter<T>*>& getListOfParams();
};

#endif /* INCREMENTOR_H_ */
