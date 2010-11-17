#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon/McmcSampler.h>
#include <charon/Functions/Function.h>

McmcSampler::McmcSampler(const std::string& name) :
		Sampler("mcmcsampler", name,
				"monte carlo sampler (metropolis algorithm)")
{
	_addParameter(sigma, "sigma",
			"sigma of mcmc noise", 0.05);
	_addParameter(steps, "steps",
			"number of steps between two samples", 1u);
	_addParameter(skip, "skip",
			"skip first samples to avoid seed influence", 1u);
	_addParameter(seed, "seed",
			"seed and current value", 0.0);
}

McmcSampler::~McmcSampler() {
}

double McmcSampler::sample() {
	double& cur = seed();
	const Function& p = *(pdf());
	double cand, accept, ratio;
	const double s=sigma(), st=steps();
	std::vector<double> candV, curV;
	candV.resize(1u,cur);
	curV.resize(1u,cur);

	for(unsigned int ii = 0; ii < st; ii++) {
		// draw new x candidate value from a gaussian centered
		// around the current value
		cand = cur + cimg_library::cimg::grand() * s;
		// draw an acceptance level uniformly out of [0,1]
		accept  = cimg_library::cimg::rand();
		// calculate probability ratio of candidate and current value
		candV[0]=cand; curV[0]=cur;
		ratio = p(candV) / p(curV);
		// reject if ratio is too small
		if ( ratio > accept )
			cur = cand;
	}
	return cur;
}

extern "C" mcmcsampler_DECLDIR ParameteredObject* create(
		const std::string& name, template_type /*t*/) {
	return new McmcSampler(name);
}

extern "C" mcmcsampler_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
