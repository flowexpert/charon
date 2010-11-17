#include <charon-core/ParameteredObject.hxx>
#include <charon/CDFitFunction.h>

CDFitFunction::CDFitFunction(
		const std::string& cName,
		const std::string& iName,
		const std::string& doc) :
			ParameteredObject(cName, iName, doc)
{
	self() = this;
	probability() = 0;
	energy() = 0;

	_addOutputSlot(self, "self",
			"self-pointer", "CDFitFunction*");
	_addOutputSlot(probability, "probability",
			"probability function", "Function*");
	_addOutputSlot(energy, "energy",
			"energy function", "Function*");
}

CDFitFunction::~CDFitFunction() {
}

void CDFitFunction::normalize() {
}

double CDFitFunction::rangeHint(std::vector<double>::size_type) const {
	return 1.;
}

void CDFitFunction::printResults() {
	sout << "CDFitFunction \"" << getName() << "\" fit results:\n\t";
	std::vector<double>::size_type ii;
	for(ii = 0; ii < fitParameters.size(); ii++)
		sout << fitParameters[ii] << ";";
	sout << std::endl;
}
