#ifndef VIGRAQT_EXPORT_HXX
#define VIGRAQT_EXPORT_HXX

#include "../charonwidgets.h"

#define VIGRAQT_EXPORT charonwidgets_DECLDIR

//replace original vigraqt Import/Export defines with charon-specific defines

// borrowed from kdemacros.h:
/*
#ifdef HAVE_GCC_VISIBILITY
# define _VIGRAQT_NO_EXPORT __attribute__ ((visibility("hidden")))
# define _VIGRAQT_EXPORT __attribute__ ((visibility("default")))
# define _VIGRAQT_IMPORT __attribute__ ((visibility("default")))
#elif defined(_WIN32) || defined(_WIN64)
# define _VIGRAQT_NO_EXPORT
# define _VIGRAQT_EXPORT charonwidgets_DECLDIR
# define _VIGRAQT_IMPORT charonwidgets_DECLDIR
#else
# define _VIGRAQT_NO_EXPORT
# define _VIGRAQT_EXPORT
# define _VIGRAQT_IMPORT
#endif

#ifndef VIGRAQT_EXPORT
# if defined(MAKE_VIGRAQT_LIB)
   // We are building this library
#  define VIGRAQT_EXPORT _VIGRAQT_EXPORT
# else
   // We are using this library
#  define VIGRAQT_EXPORT _VIGRAQT_IMPORT
# endif
#endif
*/


#include <cmath>
#include <math.h>

namespace vigraqt {
	template <typename T>
	inline int round(T val) {
		return std::floor(val + T(0.5));
	}
}

#endif // VIGRAQT_EXPORT_HXX
