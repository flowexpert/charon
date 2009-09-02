#define TYPE BrightnessModels::MovingIllumination

#include "MovingIllumination.hxx"

#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR ""
#endif

typedef unsigned short int template_type;

extern "C" DECLDIR ParameteredObject * create(const std::string & name ) {
	return new TYPE<int>(name);
}

extern "C" DECLDIR void destroy(ParameteredObject * b) {
	delete b;
}
