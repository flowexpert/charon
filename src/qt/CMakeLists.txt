INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET(charonwidgets_HEADERS
	charonwidgets.h
)
SET(charonwidgets_QT_HEADERS
	MainWindow.hpp
	ViewStack.hpp
	FrameSelectWidget.hpp
	StatisticsDisplayWidget.hpp
)
SET(charonwidgets_SOURCES
	MainWindow.cpp
	ViewStack.cpp
	FrameSelectWidget.cpp
	StatisticsDisplayWidget.cpp
)
QT4_WRAP_UI(charonwidgets_UIS
	FrameSelectWidget.ui
)
QT4_WRAP_CPP(charonwidgets_MOCS ${charonwidgets_QT_HEADERS})
SOURCE_GROUP("moc files" FILES ${charonwidgets_MOCS})
SOURCE_GROUP("ui headers" FILES ${charonwidgets_UIS})

###### Set up VigraQt ##################

FIND_PACKAGE(OpenGL)
SET(vigraqt_HEADERS
	vigraqt/vigraqt_export.hxx
	vigraqt/colormap.hxx
	vigraqt/createqimage.hxx
	vigraqt/linear_colormap.hxx
	vigraqt/qglimageviewer.hxx
	vigraqt/qrgbvalue.hxx
	vigraqt/rgbavalue.hxx
	vigraqt/vigraqimage.hxx
)
SET(vigraqt_QT_HEADERS
	vigraqt/cmeditor.hxx
	vigraqt/cmgradient.hxx
	vigraqt/fimageviewer.hxx
	vigraqt/imagecaption.hxx
	vigraqt/overlayviewer.hxx
	vigraqt/qimageviewer.hxx
)

SET(vigraqt_SOURCES
	vigraqt/cmgradient.cxx
	vigraqt/colormap.cxx
	vigraqt/fimageviewer.cxx
	vigraqt/imagecaption.cxx
	vigraqt/linear_colormap.cxx
	vigraqt/overlayviewer.cxx
	vigraqt/qglimageviewer.cxx
	vigraqt/qimageviewer.cxx
	vigraqt/test.cxx
	vigraqt/cmeditor.cxx
)

QT4_WRAP_CPP(vigraqt_MOCS ${vigraqt_QT_HEADERS})

SOURCE_GROUP("vigraqt moc files" FILES ${vigraqt_MOCS})
SOURCE_GROUP("vigraqt headers" FILES
	${vigraqt_HEADERS} ${vigraqt_QT_HEADERS})
SOURCE_GROUP("vigraqt sources" FILES ${vigraqt_SOURCES})
	
############################################################

ADD_LIBRARY(charonwidgets SHARED 
	${charonwidgets_HEADERS}
	${charonwidgets_QT_HEADERS}
	${charonwidgets_SOURCES}
	${charonwidgets_MOCS}
	${charonwidgets_UIS}
	${vigraqt_HEADERS}
	${vigraqt_QT_HEADERS}
	${vigraqt_SOURCES}
	${vigraqt_MOCS}
)

TARGET_LINK_LIBRARIES(charonwidgets
	charon-core
	${QT_LIBRARIES}
	${VIGRA_LIBRARIES}
	${QT_OPENGL_LIBRARIES}
	${OPENGL_LIBRARIES}
)

SET_TARGET_PROPERTIES(charonwidgets PROPERTIES
	FOLDER "charon-utils Modules"
)

INSTALL(
	TARGETS          charonwidgets
	EXPORT           ${EXPORT_NAME}
	RUNTIME          DESTINATION bin${BIN_SUFFIX}
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT        libraries
)

