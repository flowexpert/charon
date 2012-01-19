#include <charon/PsiFunction.h>
#include <cmath>
#include <limits>

PsiFunction::PsiFunction(const std::string& name) :
		ParameteredObject(
				"PsiFunction", name,
				"function implementing psi energy term "
				"psi(x)=sqrt(x^2+eps^2)"
		)
{
	ParameteredObject::_addParameter(
			epsilon, "epsilon",
			"parameter epsilon",
			0.02);
	ParameteredObject::_addParameter(
			lambda, "lambda", "rescaling", 1.);
	ParameteredObject::_addOutputSlot(
			self, "self", "self pointer", "Function*");
}

void PsiFunction::execute() {
	self() = this;
	_ep2    = std::pow(epsilon(), 2);
	_lambda = lambda();
}

double PsiFunction::calculate(std::vector<double> x) const {
	assert(executed());
	assert(x.size()==1u);
	return _lambda*std::sqrt(std::pow(x[0u],2)+_ep2);
}

double PsiFunction::diff(
		std::vector<double> x, std::vector<double>::size_type /*i*/) const {
	assert(x.size() == 1u);
	return x[0u]/std::sqrt(std::pow(x[0u],2)+_ep2)*_lambda;
}

double PsiFunction::diff2(
		std::vector<double> x,
		std::vector<double>::size_type /*i*/,
		std::vector<double>::size_type /*j*/) const {
	assert(x.size() == 1u);
	const double e0 = std::sqrt(std::pow(x[0],2)+_ep2);
	return (1-std::pow(x[0]/e0,2))/e0*_lambda;
}

double PsiFunction::diff2Linearized(
		std::vector<double> x,
		std::vector<double>::size_type i,
		std::vector<double>::size_type j) const {
	// divide-by-zero protection
	if(std::abs(x[j]) <= std::numeric_limits<double>::min())
		return diff2(x,i,j);
	return Diff2LinFunction::diff2Linearized(x,i,j);
}

extern "C" psifunction_DECLDIR ParameteredObject* create(
		const std::string& name, template_type /*t*/) {
	return new PsiFunction(name);
}

extern "C" psifunction_DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}

/// Report build configuration to prevent linking of incompatibel runtime libs
extern "C" psifunction_DECLDIR ParameteredObject::build_type getBuildType() {
#ifdef _DEBUG
	return ParameteredObject::DEBUG_BUILD ;
#else
	return ParameteredObject::RELEASE_BUILD ;
#endif
}
