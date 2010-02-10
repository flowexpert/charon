# configure CPack package generation system
SET(CPACK_PACKAGE_VENDOR "Heidelberg Collaboratory for Image Processing")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Charon plugin template generator")
SET(CPACK_PACKAGE_CONTACT "Robert Weidel <r.weidel@web.de>")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY     "${PROJECT_NAME}")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY  "${PROJECT_NAME}")
#SET(CPACK_RESOURCE_FILE_LICENSE         "${PROJECT_SOURCE_DIR}/COPYING.txt")
#SET(CPACK_RESOURCE_FILE_README          "${PROJECT_SOURCE_DIR}/README.txt")
SET(CPACK_PACKAGE_EXECUTABLES templategenerator "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")

# package version setup
STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+" "\\1" CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION}")

# configure dependencies
SET(CPACK_STRIP_FILES TRUE)
SET(CPACK_SOURCE_IGNORE_FILES ${PROJECT_SOURCE_DIR}/.svn ./*/.svn build ".#" "#.*~")
SET(QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "qt4-dev-tools (>=${QT_VERSION})")
INCLUDE (CPack)
