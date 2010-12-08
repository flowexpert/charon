#include <cmath>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charon-core/ParameteredObject.hxx>
#include <charon/MixtureOfGaussians.h>

#ifndef  M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551 // 2/sqrt(PI)
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440  // 1/sqrt(2)
#endif

MixtureOfGaussians::ProbabilityFunction::ProbabilityFunction(
		const std::vector<double>& weights,
		const std::vector<double>& sigmas) :
		_weights(weights),
		_sigmas(sigmas)
{
}

inline double MixtureOfGaussians::ProbabilityFunction::normPdf(
		double x, double mu, double s) {
	return std::exp(-(x-mu)*(x-mu)/(2.*s*s))/s*M_2_SQRTPI*M_SQRT1_2/2.;
}

double MixtureOfGaussians::ProbabilityFunction::calculate(
		std::vector<double> x) const {
	assert(x.size() == 1u);
	double res = 0;
	size_t NN = _weights.size();
	assert (NN == _sigmas.size());
	for(unsigned int ii=0; ii<NN; ii++)
		res += _weights[ii]*normPdf(x[0],0.,_sigmas[ii]);
	return res;
}

double MixtureOfGaussians::ProbabilityFunction::diff(
		std::vector<double> x, std::vector<double>::size_type) const {
	assert(x.size() == 1u);
	double res = 0;
	size_t NN = _weights.size();
	assert (NN == _sigmas.size());
	for(unsigned int ii=0; ii<NN; ii++)
		res += _weights[ii]/pow(_sigmas[ii],2)
			* normPdf(x[0],0.,_sigmas[ii]);
	return (-x[0] * res);
}

double MixtureOfGaussians::ProbabilityFunction::diff2(
		std::vector<double> x, std::vector<double>::size_type,
		std::vector<double>::size_type) const {
	assert(x.size() == 1u);
	double res = 0;
	size_t NN = _weights.size();
	assert (NN == _sigmas.size());
	for(unsigned int ii=0; ii<NN; ii++)
		res += _weights[ii]*(x[0]*x[0]/pow(_sigmas[ii],4)
				- 1/pow(_sigmas[ii],2)) * normPdf(x[0],0.,_sigmas[ii]);
	return res;
}

MixtureOfGaussians::EnergyFunction::EnergyFunction(
		const Diff2Function& p, const std::vector<double>& sigmas) :
		_prob(p),
		_sigmas(sigmas)
{
}

double MixtureOfGaussians::EnergyFunction::calculate(
		std::vector<double> x) const {
	return -log(_prob.calculate(x));
}

double MixtureOfGaussians::EnergyFunction::diff(
		std::vector<double> x, std::vector<double>::size_type) const {
	return -_prob.diff(x)/_prob.calculate(x);
}

double MixtureOfGaussians::EnergyFunction::diff2(
		std::vector<double> x, std::vector<double>::size_type,
		std::vector<double>::size_type) const {
	return std::pow(_prob.diff(x)/_prob.calculate(x),2)
			- _prob.diff2(x)/_prob.calculate(x);
}
double MixtureOfGaussians::EnergyFunction::diff2Linearized(
		std::vector<double> x,
		std::vector<double>::size_type i,
		std::vector<double>::size_type j) const {
	// avoid division by zero
	if(std::abs(x[j]) <= std::numeric_limits<double>::min())
		return diff2(x,i,j);

	// assume largest sigma to be the last one
	const double maxSigma = _sigmas[_sigmas.size()-1];
	if (std::abs(x[0]) < 3*maxSigma)
		return Diff2LinFunction::diff2Linearized(x,i,j);
	return std::pow(maxSigma,-2);
}

MixtureOfGaussians::MixtureOfGaussians(const std::string& name) :
		CDFitFunction("mixtureofgaussians", name, "mixture of gaussians"),
		_weights(CDFitFunction::fitParameters),
		_probability(CDFitFunction::fitParameters, _sigmas),
		_energy(_probability, _sigmas)
{
	ParameteredObject::_addParameter(weights, "weights",
			"Gaussian function weights");
	ParameteredObject::_addParameter(sigmas, "sigmas",
			"Gaussian function widths");
	probability = &_probability;
	energy = &_energy;
}

MixtureOfGaussians::~MixtureOfGaussians() {
}

double MixtureOfGaussians::diffLog(
		std::vector<double> x, std::vector<double>::size_type ii) const {
	return ProbabilityFunction::normPdf(x[0],0,_sigmas[ii])/_probability(x);
}

void MixtureOfGaussians::normalize() {
	double sum = 0;
	for(std::vector<double>::size_type ii=0; ii<fitParameters.size(); ii++)
		sum += fitParameters[ii];
	if(sum < std::numeric_limits<double>::min()) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__
				<< "\n\tsum below double::min\n\tsum value: " << sum;
		throw std::out_of_range(msg.str().c_str());
	}
	for(std::vector<double>::size_type ii=0; ii<fitParameters.size(); ii++)
		fitParameters[ii] /= sum;
}

double MixtureOfGaussians::rangeHint(std::vector<double>::size_type) const {
	// only one-dimensional, assume largest sigma at the last position
	double maxWidth = _sigmas[_sigmas.size()-1];
	return 3*maxWidth;
}

void MixtureOfGaussians::execute() {
	std::vector<double>::size_type ii, siz = sigmas.size();
	if(siz != weights.size()) {
		throw std::runtime_error(
				"MixtureOfGaussians: sigmas and weights have to contain "
				"the same number of elements!");
	}
	if(siz <= 0) {
		throw std::runtime_error(
				"MixtureOfGaussians: sigmas and weights have to contain "
				"at least one element!");
	}

	// copy sigmas and weights
	_sigmas.clear();
	_sigmas.resize(siz, sigmas[0]);
	_weights.clear();
	_weights.resize(siz, weights[0]);
	for(ii = 1; ii < siz; ii++) {
		_sigmas[ii] = sigmas[ii];
		_weights[ii] = weights[ii];
		// check if sigmas are in ascending order which is assumed later on
		if(_sigmas[ii] < _sigmas[ii-1])
			throw std::runtime_error(
					"MixtureOfGaussians: sigmas have to be specified in "
					"ascending order!");
	}
}

void MixtureOfGaussians::printParameters() {
	sout << "MixtureOfGaussian \"" << getName() << "\" parameters:";
	std::vector<double>::size_type ii;
	sout << "\n\tweights (to-fit): ";
	for(ii = 0; ii < _weights.size(); ii++)
		sout << _weights[ii] << ";";
	sout << "\n\tsigmas  (fixed) : ";
	for(ii = 0; ii < _sigmas.size(); ii++)
		sout << _sigmas[ii] << ";";
	sout << std::endl;
}


extern "C" mixtureofgaussians_DECLDIR ParameteredObject* create(
		const std::string& name, template_type /*t*/) {
	return new MixtureOfGaussians(name);
}

extern "C" mixtureofgaussians_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}
