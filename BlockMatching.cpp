#define TYPE BlockMatching

#include "BlockMatching.hxx"

#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR ""
#endif

typedef unsigned short int template_type;

extern "C" DECLDIR ParameteredObject * create(const std::string & name ) {
	switch(t) {
	case ParameteredObject::TYPE_DOUBLE:
		return new TYPE<double>(name, t);
		break;
	case ParameteredObject::TYPE_FLOAT:
		return new TYPE<float>(name, t);
		break;
	case ParameteredObject::TYPE_INT:
		return new TYPE<int>(name, t);
		break;
	default:
		return new TYPE<int>(name, ParameteredObject::TYPE_INT);
		break;
	}
}

extern "C" DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
