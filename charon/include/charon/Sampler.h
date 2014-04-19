/*  This file is part of Charon.

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
/** \file Sampler.h
 *  Declaration of parameter class Sampler
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 24.9.2009
 */

#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#ifdef HANDLE_DLL
#ifdef sampler_EXPORTS
#define sampler_DECLDIR __declspec(dllexport)
#else
#define sampler_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define sampler_DECLDIR
#endif

#include <charon-core/ParameteredObject.h>
class Function;

/// Abstract sampling class
/**
 *  \ingroup charon-learning
 */
class sampler_DECLDIR Sampler : public ParameteredObject {
public:
	/// sampling probability distribution
	InputSlot<Function*> pdf;
	/// self-pointer
	OutputSlot<Sampler*> out;

	/// draw a sample from the given distribution (may be multi-dimensional)
	virtual const std::vector<double>& sample() = 0;
	virtual ~Sampler();

protected:
	/// \implements ParameteredObject::execute()
	virtual void execute();

	/// standard constructor
	/// \param className	className
	/// \param name			instance name
	/// \param doc			docstring
	Sampler(
			const std::string& className,
			const std::string& name="",
			const std::string& doc="");
};

#endif // _SAMPLER_H_
