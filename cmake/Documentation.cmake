# Use information from FindDoxygen and set up documentation creation

IF (DOXYGEN_FOUND)
	OPTION(ENABLE_DOC_VERBOSE "Verbose documentation creation" ON)
	SET(${PROJECT_NAME}_INSTALL_DOC doc/${PROJECT_NAME}
		CACHE PATH "${PROJECT_NAME} documentation install prefix")
	MARK_AS_ADVANCED(${PROJECT_NAME}_INSTALL_DOC ENABLE_DOC_VERBOSE)

	IF(MSVC)
		SET(DOXY_WARN_FORMAT    "$file($line) : $text")
	ELSE(MSVC)
		SET(DOXY_WARN_FORMAT    "$file:$line: $text")
	ENDIF(MSVC)
	SET(DOXY_OUTPUT_PATH        "${PROJECT_BINARY_DIR}/doc")
	SET(DOXY_STRIP_INC_PATH     "${PROJECT_SOURCE_DIR}/include")
	SET(DOXY_IMAGE_PATH         "${PROJECT_SOURCE_DIR}/doc/img")
	SET(DOXY_EXAMPLE_PATH       "${PROJECT_SOURCE_DIR}/doc/example")
	SET(DOXY_TEMPLATE           "${CMAKE_MODULE_PATH}/Doxyfile.in")
	SET(DOXY_CONFIG             "${PROJECT_BINARY_DIR}/Doxyfile")
	SET(DOXY_PROJECTNAME        "${PROJECT_NAME}")
	SET(DOXY_PROJECT_VERSION    "${${PROJECT_NAME}_VERSION}")
	SET(DOXY_DOC_EXCLUDE        "")
	SET(DOXY_DOC_RECURSIVE      YES)
	IF(ENABLE_DOC_VERBOSE)
		SET(DOXY_QUIET          NO)
	ELSE(ENABLE_DOC_VERBOSE)
		SET(DOXY_QUIET          YES)
	ENDIF(ENABLE_DOC_VERBOSE)

	# LaTeX needed to generate formula
	FIND_PACKAGE(LATEX QUIET)

	# graphviz needed to generate graphs
	IF(DOXYGEN_DOT_EXECUTABLE)
		SET(DOXY_DOT_ENABLE "YES")
	ELSE(DOXYGEN_DOT_EXECUTABLE)
		SET(DOXY_DOT_ENABLE "NO")
	ENDIF(DOXYGEN_DOT_EXECUTABLE)

	IF(NOT DOC_QUIET)
		IF (NOT LATEX_COMPILER)
			MESSAGE(STATUS "latex not found")
			MESSAGE(STATUS "You will probably get warnings generating formulae.")
		ENDIF (NOT LATEX_COMPILER)

		IF (NOT DOXYGEN_DOT_EXECUTABLE)
			MESSAGE(STATUS "dot not found, graphs will not be generated")
		ENDIF (NOT DOXYGEN_DOT_EXECUTABLE)
	ENDIF(NOT DOC_QUIET)

	# dummy target for documentation creation
	IF(NOT TARGET doc)
		ADD_CUSTOM_TARGET(doc)
	ENDIF()

	SET(DOXY_DOC_PATTERN        "*.h *_doc.txt *.cpp")
	SET(DOXY_DOC_PATHS          "doc include/${PROJECT_NAME} test")
	SET(DOXY_GENERATE_HTML      YES)
	SET(DOXY_GENERATE_LATEX     NO )
	SET(DOXY_TAGFILE_INPUT      "${charon-core_TAG_IMPORT} ${charon-utils_TAG_IMPORT} \"${PROJECT_SOURCE_DIR}/doc/CImg.tag=http://cimg.sourceforge.net/reference\"")
	SET(DOXY_TAGFILE_OUTPUT     "${PROJECT_BINARY_DIR}/doc/${PROJECT_NAME}.tag")
	CONFIGURE_FILE(${DOXY_TEMPLATE} ${DOXY_CONFIG}     @ONLY)

	ADD_CUSTOM_TARGET(${PROJECT_NAME}_doc_html
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Generating ${PROJECT_NAME} html documentation"
	)
	ADD_DEPENDENCIES(doc ${PROJECT_NAME}_doc_html)
	SET_TARGET_PROPERTIES(${PROJECT_NAME}_doc_html PROPERTIES FOLDER "DocGen")

	# install html documentation
	INSTALL(
		DIRECTORY       ${PROJECT_BINARY_DIR}/doc/html
		DESTINATION     ${${PROJECT_NAME}_INSTALL_DOC}
		OPTIONAL
		COMPONENT       htmldoc
	)

	# install doxygen tagfile
	INSTALL(
		FILES           ${PROJECT_BINARY_DIR}/doc/${PROJECT_NAME}.tag
		DESTINATION     ${${PROJECT_NAME}_INSTALL_DOC}
		OPTIONAL
		COMPONENT       htmldoc
	)
ENDIF (DOXYGEN_FOUND)
