#ifndef SAMPLE_GENERATOR_HXX
#define SAMPLE_GENERATOR_HXX

#include <charon-core/ParameteredObject.hxx>
#include "SampleGenerator.h"
#include <cassert>

template<typename T>
SampleGenerator<T>::SampleGenerator(const std::string& name) :
		TemplatedParameteredObject<T>(
				"samplegenerator", name,
				"apply given sampler to generate test images")
{
	ParameteredObject::_addInputSlot(sampler, "sampler",
			"sampler to use", "Sampler*");
	ParameteredObject::_addInputSlot(size, "size",
			"size of output image", "Roi<int>");
	ParameteredObject::_addOutputSlot(out, "out",
			"image output", "CImgList<T>");
}

template<typename T>
void SampleGenerator<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	cimg_library::CImgList<T>& res = out();
	try {
		res.clear();
		unsigned int
			dx = size()->getWidth(),
			dy = size()->getHeight(),
			dz = size()->getDepth(),
			dt = size()->getDuration();
		assert(dx > 0);
		assert(dy > 0);
		assert(dz > 0);
		assert(dt > 0);
		res.push_back(cimg_library::CImg<T>(dx,dy,dz,dt,T(0.)));
		cimg_for(res[0], ptr, T)
				*ptr = T(sampler()->sample());
	}
	catch (std::bad_alloc) {
		res.clear();
	}
}

#endif // SAMPLE_GENERATOR_HXX

