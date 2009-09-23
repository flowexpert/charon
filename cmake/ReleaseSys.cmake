# enable release system
SET(CPACK_PACKAGE_VENDOR
    "Heidelberg Collaboratory for Image Processing"
)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Configuration Graph Tools"
)

# package version setup
SET(CPACK_PACKAGE_VERSION_MAJOR         0)
SET(CPACK_PACKAGE_VERSION_MINOR         1)
SET(CPACK_PACKAGE_VERSION_PATCH         3)
SET(CPACK_PACKAGE_INSTALL_DIRECTORY     "${PROJECT_NAME}")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY  "${PROJECT_NAME}")
SET(CPACK_RESOURCE_FILE_LICENSE         "${PROJECT_SOURCE_DIR}/COPYING.txt")
SET(CPACK_RESOURCE_FILE_README          "${PROJECT_SOURCE_DIR}/README.txt")

SET(CPACK_PACKAGE_EXECUTABLES
    paramedit "Configuration Graph Editor"
    paraminspector "ParameterFile Editor"
)
SET(CPACK_STRIP_FILES
    paramedit
    paraminspector
    editparam
    editgraph
)
SET(CPACK_SOURCE_IGNORE_FILES
    .svn
    ./*/.svn
	bin
	build
    ".#"
    "#.*~"
)
SET(CPACK_PACKAGE_CONTACT
    "Jens-Malte Gottfried <jmgottfried@web.de>"
)
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "charon-core (>=0.1.3), libgraphviz-dev (>=2.16), qt4-dev-tools (>=4.4.0)")

# install components
SET(CPACK_COMPONENTS_ALL applications libraries headers dlls)

# Display name properties
SET(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Applications")
SET(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME    "Static Libraries")
SET(CPACK_COMPONENT_HEADERS_DISPLAY_NAME      "C++ Headers")
SET(CPACK_COMPONENT_DLLS_DISPLAY_NAME         "Needed DLLs")

# Descriptions
SET(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "paramedit and paraminspector executables")
SET(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
    "libraries to link external projects against ${PROJECT_NAME}")
SET(CPACK_COMPONENT_HEADERS_DESCRIPTION
    "header files for library usage")
SET(CPACK_COMPONENT_DLLS_DESCRIPTION
    "Qt dlls, C++ runtime; needed if you have no build environment installed")

# headers only useful if libraries are installed
SET(CPACK_COMPONENT_HEADERS_DEPENDS		libraries)

# grouping
SET(CPACK_COMPONENT_APPLICATIONS_GROUP  Runtime)
SET(CPACK_COMPONENT_DLLS_GROUP          Runtime)
SET(CPACK_COMPONENT_LIBRARIES_GROUP     Development)
SET(CPACK_COMPONENT_HEADERS_GROUP       Development)

SET(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
    "applications and shared libraries")
SET(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
    "Tools you need to develop software that uses ${PROJECT_NAME}")

# preselections
SET(CPACK_COMPONENT_APPLICATIONS_REQUIRED   On)
SET(CPACK_COMPONENT_LIBRARIES_DISABLED      On)
SET(CPACK_COMPONENT_HEADERS_DISABLED        On)

# add optional documentation if avaiable
IF(ENABLE_DOC)
    LIST(APPEND CPACK_COMPONENTS_ALL htmldoc)
    SET(CPACK_COMPONENT_HTMLDOC_DISPLAY_NAME "HTML Documentation")
    SET(CPACK_COMPONENT_HTMLDOC_DESCRIPTION
        "HTML Documentation (including API doc)")
    SET(CPACK_COMPONENT_HTMLDOC_GROUP "Documentation")
    SET(CPACK_COMPONENT_GROUP_DOCUMENTATION_DESCRIPTION
        "doxygen generated manual and api documentation")

    # separate pdf documentation
    IF(LATEX_COMPILER)
        LIST(APPEND CPACK_COMPONENTS_ALL pdfdoc)
        SET(CPACK_COMPONENT_PDFDOC_DISPLAY_NAME "PDF Documentation")
        SET(CPACK_COMPONENT_PDFDOC_DESCRIPTION
            "PDF Documentation (without API doc)")
        SET(CPACK_COMPONENT_PDFDOC_GROUP "Documentation")
    ENDIF(LATEX_COMPILER)
ENDIF(ENABLE_DOC)

INCLUDE (CPack)
