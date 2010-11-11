INCLUDE(FeatureSummary)
SET_FEATURE_INFO(Qt4
	"Qt Framework by Nokia"
	"http://qt.nokia.com"
	"Version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}"
)
SET_FEATURE_INFO(Subversion
	"SVN Version control system"
	"http://subversion.apache.org"
)
SET_FEATURE_INFO(charon-core
	"charon plugin framework"
	"http://charon-suite.sourceforge.net"
	"Version ${charon-core_VERSION}"
)
SET_FEATURE_INFO(Graphviz
	"Graph Visualization Software"
	"http://www.graphviz.org"
	"Version ${Graphviz_VERSION}"
)
SET_FEATURE_INFO(Doxygen
	"Documentation system"
	"http://doxygen.org"
)
PRINT_ENABLED_FEATURES()
PRINT_DISABLED_FEATURES()

