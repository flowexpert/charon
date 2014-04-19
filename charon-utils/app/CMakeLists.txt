IF(USE_QT)
	ADD_EXECUTABLE(charon-xml-helper xmlModuleDetector.cpp)
	TARGET_LINK_LIBRARIES(charon-xml-helper ${QT_LIBRARIES})
	SET_TARGET_PROPERTIES(charon-xml-helper PROPERTIES
		FOLDER "charon-utils-helpers")
	INSTALL(
		TARGETS      charon-xml-helper
		EXPORT       ${EXPORT_NAME}
		RUNTIME      DESTINATION bin${BIN_SUFFIX}
		LIBRARY      DESTINATION lib${LIB_SUFFIX}
		ARCHIVE      DESTINATION lib${LIB_SUFFIX}
		COMPONENT    applications
	)
ENDIF(USE_QT)
