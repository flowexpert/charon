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
/** \file McmcSampler.h
 *  Declaration of parameter class McmcSampler
 *  \author <a href="mailto:Jens-Malte.Gottfried@iwr.uni-heidelberg.de">
 *      Jens-Malte Gottfried</a>
 *  \date 24.9.2009
 */

#ifndef MCMC_SAMPLER_H
#define MCMC_SAMPLER_H

#include "Sampler.h"

#ifdef HANDLE_DLL
#ifdef mcmcsampler_EXPORTS
#define mcmcsampler_DECLDIR __declspec(dllexport)
#else
#define mcmcsampler_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define mcmcsampler_DECLDIR
#endif

/// Markov chain monte carlo sampler
///
/// This sampler implements the metropolis algorithm
/// and changes the current value using
/// gaussian noise with given sigma.
class mcmcsampler_DECLDIR McmcSampler : public Sampler {
public:
	/// mcmc noise sigma
	Parameter<double> sigma;
	/// step size between two samples
	Parameter<unsigned int> steps;
	/// skip first n steps
	Parameter<unsigned int> skip;
	/// seed, current value
	Parameter<double> seed;
	
	/// create instance with given name
	/// \name	instance name
	McmcSampler(const std::string& name = "");

	virtual ~McmcSampler();

	virtual double sample();
};

#endif // MCMC_SAMPLER_H
