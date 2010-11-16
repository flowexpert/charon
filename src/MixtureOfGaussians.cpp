#include <cmath>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charon-core/ParameteredObject.hxx>
#include <charon/MixtureOfGaussians.h>

MixtureOfGaussians::ProbabilityFunction::ProbabilityFunction(
		const std::vector<double>& weights,
		const std::vector<double>& sigmas) :
		_weights(weights),
		_sigmas(sigmas)
{
}

inline double MixtureOfGaussians::ProbabilityFunction::_normPdf(
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
		res += _weights[ii]*_normPdf(x[0],0.,_sigmas[ii]);
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
			* _normPdf(x[0],0.,_sigmas[ii]);
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
				- 1/pow(_sigmas[ii],2)) * _normPdf(x[0],0.,_sigmas[ii]);
	return res;
}

MixtureOfGaussians::EnergyFunction::EnergyFunction(const Diff2Function& p) :
		_prob(p)
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

MixtureOfGaussians::MixtureOfGaussians(const std::string& name) :
		CDFitFunction("mixtureofgaussians", name, "mixture of gaussians"),
		_weights(CDFitFunction::fitParameters),
		_probability(CDFitFunction::fitParameters, _sigmas),
		_energy(_probability)
{
	ParameteredObject::_addParameter(weights, "weights",
			"Gaussian function weights");
	ParameteredObject::_addParameter(sigmas, "sigmas",
			"Gaussian function widths");
}

MixtureOfGaussians::~MixtureOfGaussians() {
}

double MixtureOfGaussians::diffLog(
		std::vector<double> x, std::vector<double>::size_type ii) const {
	if (_weights[ii] <= std::numeric_limits<double>::min()) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Parameter weight[" << ii << "] too small.\n\t";
		msg << "weight[" << ii << "] = " << _weights[ii];
		throw std::out_of_range(msg.str());
	}
	double d = 0;
	unsigned int NN = _weights.size();
	assert (NN == _sigmas.size());
	for(unsigned int jj=0; jj<NN; jj++) {
		if (ii==jj)
			continue;
		d += (_weights[jj]/_weights[ii])
			 * ProbabilityFunction::_normPdf(x[0],0,_sigmas[jj]);
	}
	return 1./(1.+d);
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
	// only one-dimensional
	double maxWidth = 0;
	for(std::vector<double>::size_type ii=0; ii<sigmas().size(); ii++)
		if(sigmas()[ii] > maxWidth)
			maxWidth = sigmas()[ii];
	return 3*maxWidth;
}

extern "C" mixtureofgaussians_DECLDIR ParameteredObject* create(
		const std::string& name, template_type /*t*/) {
	return new MixtureOfGaussians(name);
}

extern "C" mixtureofgaussians_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}
