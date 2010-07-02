# Use information from FindDoxygen and set up documentation creation

IF (DOXYGEN_FOUND)
    # Possibility to enable/disable documentation creation
    OPTION(WITH_LATEX "activate pdfdoc generation" ON)
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
    SET(DOXY_HTML_STYLESHEET    "${PROJECT_SOURCE_DIR}/doc/style.css")
    SET(DOXY_TEMPLATE           "${CMAKE_MODULE_PATH}/Doxyfile.in")
    SET(DOXY_CONFIG             "${PROJECT_BINARY_DIR}/Doxyfile")
    SET(DOXY_CONFIG_PDF         "${PROJECT_BINARY_DIR}/DoxyPdf")
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
    IF(NOT WITH_LATEX)
        SET(DOXY_SKIP_PDFDOC    YES)
    ENDIF(NOT WITH_LATEX)

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
            MESSAGE(STATUS "latex not found - disabling pdf output.")
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

    SET(DOXY_DOC_PATTERN        "*.cpp *.h *.hxx *.hpp *_doc.txt")
    SET(DOXY_DOC_PATHS          "doc src include/charon-core")
    SET(DOXY_GENERATE_HTML      YES)
    SET(DOXY_GENERATE_LATEX     NO )
    SET(DOXY_TAGFILE_INPUT      )
    SET(DOXY_TAGFILE_OUTPUT     "${PROJECT_BINARY_DIR}/doc/${PROJECT_NAME}.tag")
    CONFIGURE_FILE(${DOXY_TEMPLATE} ${DOXY_CONFIG}     @ONLY)

    ADD_CUSTOM_TARGET(${PROJECT_NAME}_doc_html
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating ${PROJECT_NAME} html documentation"
    )
    ADD_DEPENDENCIES(doc ${PROJECT_NAME}_doc_html)
    SET_TARGET_PROPERTIES(${PROJECT_NAME}_doc_html PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

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

    IF(LATEX_COMPILER AND NOT DOXY_SKIP_PDFDOC)
        SET(DOXY_DOC_RECURSIVE      NO)
        SET(DOXY_DOC_PATTERN        *_doc.txt)
        SET(DOXY_DOC_PATHS          doc)
        SET(DOXY_GENERATE_HTML      NO)
        SET(DOXY_GENERATE_LATEX     YES)
        SET(DOXY_TAGFILE_INPUT)
        SET(DOXY_TAGFILE_OUTPUT)
        CONFIGURE_FILE(${DOXY_TEMPLATE} ${DOXY_CONFIG_PDF} @ONLY)

        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
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
        ADD_CUSTOM_TARGET(${PROJECT_NAME}_doc_pdf
            DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
            COMMENT "Generating ${PROJECT_NAME} pdf manual"
        )

        ADD_DEPENDENCIES(doc ${PROJECT_NAME}_doc_pdf)
        SET_TARGET_PROPERTIES(${PROJECT_NAME}_doc_pdf PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

        # install pdf documentation
        INSTALL(
            FILES           ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
            RENAME          ${PROJECT_NAME}-manual.pdf
            DESTINATION     ${${PROJECT_NAME}_INSTALL_DOC}
            OPTIONAL
            COMPONENT       pdfdoc
        )
    ENDIF(LATEX_COMPILER AND NOT DOXY_SKIP_PDFDOC)
ENDIF (DOXYGEN_FOUND)
