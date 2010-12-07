FIND_PACKAGE(OpenGL)
SET(QT_USE_QTOPENGL 1)
INCLUDE(${QT_USE_FILE})

SET(VIGRAQT_HEADERS
	vigraqt_export.hxx
	cmeditor.hxx
	cmgradient.hxx
	colormap.hxx
	createqimage.hxx
	fimageviewer.hxx
	imagecaption.hxx
	linear_colormap.hxx
	overlayviewer.hxx
	qglimageviewer.hxx
	qimageviewer.hxx
	qrgbvalue.hxx
	rgbavalue.hxx
	vigraqimage.hxx
)

SET(VIGRAQT_SOURCES
	cmgradient.cxx
	colormap.cxx	
	fimageviewer.cxx
	imagecaption.cxx
	linear_colormap.cxx
	overlayviewer.cxx
	qglimageviewer.cxx
	qimageviewer.cxx
	test.cxx
	cmeditor.cxx
)

QT4_WRAP_CPP(VIGRAQT_MOCS ${VIGRAQT_HEADERS})

SOURCE_GROUP("moc Files" FILES ${VIGRAQT_MOCS})

IF(MSVC)
	ADD_DEFINITIONS(/DMAKE_VIGRAQT_LIB)
ENDIF(MSVC)

ADD_LIBRARY(vigraqt SHARED ${VIGRAQT_HEADERS} ${VIGRAQT_SOURCES} ${VIGRAQT_MOCS})
TARGET_LINK_LIBRARIES(vigraqt ${QT_LIBRARIES} ${QT_OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} ${VIGRA_LIBRARIES})

SET_TARGET_PROPERTIES(vigraqt
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)


IF(NOT EXPORT_NAME)
	SET(EXPORT_NAME ${PROJECT_NAME}-export)
ENDIF()


INSTALL(
	TARGETS          vigraqt
	EXPORT			 ${EXPORT_NAME}
	RUNTIME          DESTINATION bin
	LIBRARY          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	ARCHIVE          DESTINATION lib${LIB_SUFFIX}/charon-plugins
	COMPONENT        libraries
)
