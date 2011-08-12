#include <charon-core/ParameteredObject.hxx>
#include <charon-utils/CImg.h>
#include <charon/McmcSampler.h>
#include <charon/Functions/Function.h>

McmcSampler::McmcSampler(const std::string& name) :
		Sampler("mcmcsampler", name,
				"monte carlo sampler (metropolis algorithm)"),
		seed("0.")
{
	_addParameter(sigma, "sigma",
			"sigma of mcmc noise", 0.05);
	_addParameter(steps, "steps",
			"number of steps between two samples", 1u);
	_addParameter(skip, "skip",
			"skip first samples to avoid seed influence", 1u);
	_addParameter(seed, "seed",
			"seed and current value");
}

McmcSampler::~McmcSampler() {
}

const std::vector<double>& McmcSampler::sample() {
	static std::vector<double>& cur = seed();
	const Function& p = *(pdf());
	std::vector<double>::size_type dim = p.dims(), kk;
	if(cur.size() != dim)
		throw std::invalid_argument(
				"McmcSampler: seed has wrong dimension!");

	static std::vector<double> cand(cur);
	if(cand.size() != cur.size())
		cand = cur;

	double accept, ratio;
	const double s=sigma(), st=steps();

	for(unsigned int ii = 0; ii < st; ii++) {
		// draw new x candidate value from a gaussian centered
		// around the current value
		for(kk = 0; kk < dim; kk++)
			cand[kk] = cur[kk] + cimg_library::cimg::grand() * s;
		// draw an acceptance level uniformly out of [0,1]
		accept  = cimg_library::cimg::rand();
		// calculate probability ratio of candidate and current value
		ratio = p(cand) / p(cur);
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
/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" mcmcsampler_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
