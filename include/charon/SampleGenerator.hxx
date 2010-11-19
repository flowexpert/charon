#ifndef SAMPLE_GENERATOR_HXX
#define SAMPLE_GENERATOR_HXX

#include <charon-core/ParameteredObject.hxx>
#include <charon/Functions/Function.h>
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
		std::vector<double>::size_type kk,
			dx = size()->getWidth(),
			dy = size()->getHeight(),
			dz = size()->getDepth(),
			dt = size()->getDuration(),
			n  = sampler()->pdf()->dims();
		assert(dx > 0);
		assert(dy > 0);
		assert(dz > 0);
		assert(dt > 0);
		assert(n  > 0);
		res.assign(n,dx,dy,dz,dt,T(0.));
		cimg_forXYZC(res[0],x,y,z,t) {
			const std::vector<double>& spl = sampler()->sample();
			assert(spl.size() == n);
			for (kk = 0; kk < n; kk++)
				res(kk,x,y,z,t) = spl[kk];
		}
	}
	catch (std::bad_alloc) {
		res.clear();
	}
}

#endif // SAMPLE_GENERATOR_HXX

