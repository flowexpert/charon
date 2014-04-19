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
			a, "a",
			"parameter a",
			0.5);
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
	return _lambda*std::pow(std::pow(x[0u],2)+_ep2,a());
}

double PsiFunction::diff(
		std::vector<double> x, std::vector<double>::size_type /*i*/) const {
	assert(x.size() == 1u);
	return _lambda*std::pow(std::pow(x[0u],2)+_ep2,a()-1)*a()*2*x[0u];
}

double PsiFunction::diff2(
		std::vector<double> x,
		std::vector<double>::size_type /*i*/,
		std::vector<double>::size_type /*j*/) const {
	assert(x.size() == 1u);
	return 2*a()*_lambda*std::pow(std::pow(x[0u],2)+_ep2,a()-2)
		*(std::pow(x[0u],2)+_ep2+2*std::pow(x[0u],2)*(a-1));
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
		const std::string& name, ParameteredObject::template_type /*t*/) {
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
