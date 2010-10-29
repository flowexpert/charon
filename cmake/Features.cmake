INCLUDE(FeatureSummary)
SET_FEATURE_INFO(charon-core
	"charon plugin framework"
	"http://charon-suite.sourceforge.net"
	"Version ${charon-core_VERSION}"
)
SET_FEATURE_INFO(charon-utils
	"charon image processing modules"
	"http://charon-suite.sourceforge.net"
	"Version ${charon-core_VERSION}"
)
SET_FEATURE_INFO(Threads
	"system thread library"
)
SET_FEATURE_INFO(BLAS
	"linear algebra interface"
	"http://www.netlib.org/blas/"
)
SET_FEATURE_INFO(LAPACK
	"linear algebra package"
	"http://www.netlib.org/lapack/"
)
SET_FEATURE_INFO(X11
	"X Window System"
	"www.x.org"
)
SET_FEATURE_INFO(MPI
	"Message Passing Interface"
	"http://www.mcs.anl.gov/research/projects/mpi/"
)
SET_FEATURE_INFO(petsc
	"Portable, Extensible Toolkit for Scientific Computation"
	"http://www.mcs.anl.gov/petsc/petsc-as/"
)
SET_FEATURE_INFO(Doxygen
	"Documentation system"
	"http://doxygen.org"
)
_PRINT_FEATURES(ENABLED_FEATURES "Found packages:")
_PRINT_FEATURES(DISABLED_FEATURES "Not found (disabled):")
