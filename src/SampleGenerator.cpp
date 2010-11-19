#include <charon-core/ParameteredObject.hxx>
#include <charon/SampleGenerator.hxx>

#define TYPE SampleGenerator

extern "C" samplegenerator_DECLDIR ParameteredObject* create(
		const std::string& name, template_type t) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new TYPE<double>(name);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new TYPE<float>(name);
		break;
	case ParameteredObject::TYPE_INT:
		return new TYPE<int>(name);
		break;
	default:
		return new TYPE<int>(name);
		break;
	}
}

extern "C" samplegenerator_DECLDIR void destroy(ParameteredObject* b) {
	delete b;
}
