# Use information from FindDoxygen and set up documentation creation

SET(DOC_QUIET FALSE)

IF (DOXYGEN_FOUND)
    # Possibility to enable/disable documentation creation
    OPTION(ENABLE_DOC "Automatically create documentation" ON)
	OPTION(ENABLE_DOC_VERBOSE "Verbose documentation creation" ON)
    SET(CMAKE_INSTALL_DOC doc/${PROJECT_NAME}
        CACHE PATH "documentation install prefix")
    MARK_AS_ADVANCED(CMAKE_INSTALL_DOC ENABLE_DOC_VERBOSE)

    SET(DOXY_WARN_FORMAT        "$file:$line: $text")
    SET(DOXY_OUTPUT_PATH        ${PROJECT_BINARY_DIR}/doc)
    SET(DOXY_IMAGE_PATH         ${PROJECT_SOURCE_DIR}/doc/img)
    SET(DOXY_EXAMPLE_PATH       ${PROJECT_SOURCE_DIR}/doc/example)
    SET(DOXY_HTML_STYLESHEET	${PROJECT_SOURCE_DIR}/doc/style.css)
    SET(DOXY_TEMPLATE           ${CMAKE_MODULE_PATH}/Doxyfile.in)
    SET(DOXY_CONFIG             ${PROJECT_BINARY_DIR}/Doxyfile)
    SET(DOXY_CONFIG_PDF         ${PROJECT_BINARY_DIR}/DoxyPdf)
    SET(DOXY_PROJECTNAME        ${PROJECT_NAME})
    SET(DOXY_DOC_EXCLUDE        )
    SET(DOXY_DOC_RECURSIVE      NO)
    SET(DOXY_LATEX_BATCHMODE    YES)
    SET(DOXY_COMPACT_LATEX      YES)
    IF(ENABLE_DOC_VERBOSE)
        SET(DOXY_QUIET          NO)
    ELSE(ENABLE_DOC_VERBOSE)
        SET(DOXY_QUIET          YES)
    ENDIF(ENABLE_DOC_VERBOSE)
        
    # LaTeX needed to create formula
    FIND_PACKAGE(LATEX)
	IF(LATEX_COMPILER)
		OPTION(WITH_LATEX "PDF documentation creation" ON)
	ENDIF(LATEX_COMPILER)
        
    # graphviz needed to generate graphs
    IF(DOXYGEN_DOT_EXECUTABLE)
        SET(DOXY_DOT_ENABLE "YES")
    ELSE(DOXYGEN_DOT_EXECUTABLE)
        SET(DOXY_DOT_ENABLE "NO")
    ENDIF(DOXYGEN_DOT_EXECUTABLE)
        
    IF(NOT DOC_QUIET)
        IF (NOT LATEX_COMPILER)
            MESSAGE(STATUS "latex not found - disabling pdf output.")
            MESSAGE(STATUS "You will probably get warnings generating formulae.")
        ENDIF (NOT LATEX_COMPILER)
    
        IF (NOT DOXYGEN_DOT_EXECUTABLE)
            MESSAGE(STATUS "dot not found, graphs will not be generated")
        ENDIF (NOT DOXYGEN_DOT_EXECUTABLE)
    ENDIF(NOT DOC_QUIET)


    # set up documentation target
    SET(DOXY_DOC_PATTERN        *.cpp *.h *.hxx *_doc.txt)
    FILE(GLOB HTMLDOCS
        ${PROJECT_SOURCE_DIR}/doc/*_doc.txt
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.hxx
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/test/*.h
        ${PROJECT_SOURCE_DIR}/test/*.hxx
        ${PROJECT_SOURCE_DIR}/test/*.cpp
    )
    #MESSAGE(STATUS "Htmldoc deps: ${HTMLDOCS}")

    SET(DOXY_DOC_PATTERN        "*.cpp *.h *.hxx *_doc.txt")
    SET(DOXY_DOC_PATHS          "app src doc")
    SET(DOXY_GENERATE_HTML      YES)
    SET(DOXY_GENERATE_LATEX     NO )
    SET(DOXY_TAGFILE_INPUT      ${CHARON_UTILS_TAG_IMPORT})
    SET(DOXY_TAGFILE_OUTPUT     "${PROJECT_BINARY_DIR}/doc/html/${PROJECT_NAME}.tag")
    CONFIGURE_FILE(${DOXY_TEMPLATE} ${DOXY_CONFIG}     @ONLY)

    ADD_CUSTOM_TARGET(htmldoc
        DEPENDS ${PROJECT_BINARY_DIR}/doc/html/index.html
        COMMENT "Generating html documentation"
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT  ${PROJECT_BINARY_DIR}/doc/html/index.html
        DEPENDS ${HTMLDOCS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    IF(ENABLE_DOC)
        ADD_CUSTOM_TARGET(doc ALL)
        ADD_DEPENDENCIES(doc htmldoc)

        # install html documentation
        INSTALL(
            DIRECTORY       ${PROJECT_BINARY_DIR}/doc/html
            DESTINATION     ${CMAKE_INSTALL_DOC}
            COMPONENT       htmldoc
        )
    ENDIF(ENABLE_DOC)

    IF(LATEX_COMPILER AND WITH_LATEX)
        SET(DOXY_DOC_RECURSIVE      NO)
        SET(DOXY_DOC_PATTERN		*_doc.txt)
        SET(DOXY_DOC_PATHS          doc)
        SET(DOXY_GENERATE_HTML      NO)
        SET(DOXY_GENERATE_LATEX     YES)
        SET(DOXY_TAGFILE_INPUT)
        SET(DOXY_TAGFILE_OUTPUT)
        CONFIGURE_FILE(${DOXY_TEMPLATE} ${DOXY_CONFIG_PDF} @ONLY)

        FILE(GLOB TEXDOCS
            ${PROJECT_SOURCE_DIR}/doc/*_doc.txt
        )
        #MESSAGE(STATUS "PDFdoc deps: ${TEXDOCS}")
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
            DEPENDS ${TEXDOCS}
            COMMAND ${DOXYGEN_EXECUTABLE} "${DOXY_CONFIG_PDF}"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )

        # pdf generation
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
            DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
            COMMAND ${CMAKE_COMMAND} -E remove *.aux *.toc *.idx *.ind *.ilg *.log *.out
            COMMAND ${PDFLATEX_COMPILER}  refman.tex
            COMMAND ${MAKEINDEX_COMPILER} refman.idx
            COMMAND ${PDFLATEX_COMPILER}  refman.tex
            COMMAND ${PDFLATEX_COMPILER}  refman.tex
            COMMAND ${PDFLATEX_COMPILER}  refman.tex
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/latex
        )
        ADD_CUSTOM_TARGET(pdfgen
            DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
        )

        IF(ENABLE_DOC)
            ADD_DEPENDENCIES(doc pdfgen)

            # install pdf documentation
            INSTALL(
                FILES           ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
                RENAME          ${PROJECT_NAME}-manual.pdf
                DESTINATION     ${CMAKE_INSTALL_DOC}
                COMPONENT       pdfdoc
            )
        ENDIF(ENABLE_DOC)
    ENDIF(LATEX_COMPILER AND WITH_LATEX)
ENDIF (DOXYGEN_FOUND)
