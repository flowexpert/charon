
SET(charonwidgets_HEADERS
	MainWindow.hpp
	ViewStack.hpp
	charonwidgets.h
)

SET(charonwidgets_SOURCES
	MainWindow.cpp
	ViewStack.cpp
)	

IF(USE_VIGRA)
	SET(USE_VIGRAQT True)
ENDIF(USE_VIGRA)

IF(USE_VIGRAQT)

	FIND_PACKAGE(OpenGL)
	SET(QT_USE_QTOPENGL 1)
	INCLUDE(${QT_USE_FILE})
	
	SET(vigraqt_HEADERS
		vigraqt/vigraqt_export.hxx
		vigraqt/cmeditor.hxx
		vigraqt/cmgradient.hxx
		vigraqt/colormap.hxx
		vigraqt/createqimage.hxx
		vigraqt/fimageviewer.hxx
		vigraqt/imagecaption.hxx
		vigraqt/linear_colormap.hxx
		vigraqt/overlayviewer.hxx
		vigraqt/qglimageviewer.hxx
		vigraqt/qimageviewer.hxx
		vigraqt/qrgbvalue.hxx
		vigraqt/rgbavalue.hxx
		vigraqt/vigraqimage.hxx
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
	
	QT4_WRAP_CPP(vigraqt_MOCS ${vigraqt_HEADERS})
	
	SOURCE_GROUP("vigraqt moc files" FILES ${vigraqt_MOCS})
	SOURCE_GROUP("vigraqt headers" FILES ${vigraqt_HEADERS})
	SOURCE_GROUP("vigraqt sources" FILES ${vigraqt_SOURCES})
	
ENDIF(USE_VIGRAQT)

	
QT4_WRAP_CPP(charonwidgets_MOCS ${charonwidgets_HEADERS})

SOURCE_GROUP("moc files" FILES ${charonwidgets_MOCS})

ADD_LIBRARY(charonwidgets SHARED 
	${charonwidgets_HEADERS}
	${charonwidgets_SOURCES}
	${charonwidgets_MOCS}
	${vigraqt_HEADERS}
	${vigraqt_SOURCES}
	${vigraqt_MOCS}
)

TARGET_LINK_LIBRARIES(charonwidgets ${QT_LIBRARIES})
IF(USE_VIGRAQT)
	TARGET_LINK_LIBRARIES(charonwidgets ${VIGRA_LIBRARIES} ${QT_OPENGL_LIBRARIES} ${OPENGL_LIBRARIES})
ENDIF(USE_VIGRAQT)

SET_TARGET_PROPERTIES(charonwidgets
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)

IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()


INSTALL(
	TARGETS          charonwidgets
	EXPORT			 ${EXPORT_NAME}
	RUNTIME          DESTINATION bin
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT        libraries
)

