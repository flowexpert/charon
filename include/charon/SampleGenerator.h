/// \file SampleGenerator.h
/// Declaration of the class SampleGenerator
/// \author Jens-Malte Gottfried <jmgottfried@web.de>
/// \date 24.09.2009

#ifndef SAMPLE_GENERATOR_H
#define SAMPLE_GENERATOR_H

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>
#include <charon-utils/Roi.h>
#include "Sampler.h"

#ifdef HANDLE_DLL
#ifdef samplegenerator_EXPORTS
#define samplegenerator_DECLDIR __declspec(dllexport)
#else
#define samplegenerator_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define samplegenerator_DECLDIR
#endif

/// apply given sampler to generate test images
/**
 *  \ingroup charon-modules
 *  \ingroup charon-flow
 *  \ingroup charon-learning
 */
template <typename T>
class samplegenerator_DECLDIR SampleGenerator :
		public TemplatedParameteredObject<T> {
public:
	/// create instance
	/// \param name		instance name
	SampleGenerator(const std::string& name = "");

	/// sampler to use
	InputSlot<Sampler*>	sampler;
	/// sample size
	InputSlot<Roi<int>*> size;
	/// image outupt
	OutputSlot<cimg_library::CImgList<T> > out;

protected:
	/// perform sampling
	/// \implements ParameteredObject::execute()
	virtual void execute();
};

#endif // SAMPLE_GENERATOR_H

