OPTION(USE_VCS "Try to determine VCS revision info" ON)
IF(USE_VCS)
	IF(NOT ${PROJECT_NAME}_VCS_DIR)
		SET(${PROJECT_NAME}_VCS_DIR "${PROJECT_SOURCE_DIR}")
	ENDIF()
	SET(${PROJECT_NAME}_VCS_DIR "${${PROJECT_NAME}_VCS_DIR}"
		CACHE PATH "Directory to query VCS information from" FORCE)

	# determine project svn information if possible
	IF(SVNCOMMAND)
		EXECUTE_PROCESS(
			COMMAND ${SVNCOMMAND} info ${${PROJECT_NAME}_VCS_DIR}
			OUTPUT_VARIABLE SVN_INFO
			ERROR_VARIABLE SVN_INFO_ERROR
			RESULT_VARIABLE SVN_INFO_RESULT
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		IF(${SVN_INFO_RESULT} EQUAL 0)
			STRING(REGEX REPLACE "^(.*\n)?URL: ([^\n]+).*"
				"\\2" SVN_WC_URL "${SVN_INFO}")
			STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
				"\\2" SVN_WC_REVISION "${SVN_INFO}")
			STRING(REGEX REPLACE
				"^.*/${PROJECT_NAME}/(.*)$" "\\1"
				SVN_BRANCH "${SVN_WC_URL}"
			)
			SET(${PROJECT_NAME}_VCS_INFO
				"svn r${SVN_WC_REVISION} (${SVN_BRANCH})"
			)
		ENDIF()
		UNSET(SVN_INFO)
		UNSET(SVN_INFO_ERROR)
		UNSET(SVN_INFO_RESULT)
		UNSET(SVN_WC_URL)
		UNSET(SVN_WC_REVISION)
		UNSET(SVN_BRANCH)
	ENDIF(SVNCOMMAND)

	# determine project bzr information if possible
	IF(BZRCOMMAND)
		EXECUTE_PROCESS(
			COMMAND ${BZRCOMMAND} revno "${${PROJECT_NAME}_VCS_DIR}"
			OUTPUT_VARIABLE BZR_REV
			ERROR_VARIABLE BZR_REV_ERROR
			RESULT_VARIABLE BZR_REV_RESULT
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		IF(${BZR_REV_RESULT} EQUAL 0)
			SET(${PROJECT_NAME}_VCS_INFO
				"bzr r${BZR_REV}"
			)
		ENDIF()
		UNSET(BZR_REV)
		UNSET(BZR_REV_ERROR)
		UNSET(BZR_REV_RESULT)
	ENDIF(BZRCOMMAND)

	# Info from VCS
	SET(${PROJECT_NAME}_VCS_INFO "${${PROJECT_NAME}_VCS_INFO}"
		CACHE STRING "VCS information (set automatically)" FORCE)
ENDIF(USE_VCS)

# Option to add custom information
SET(${PROJECT_NAME}_BUILD_INFO "${${PROJECT_NAME}_BUILD_INFO}"
	CACHE STRING "Build information (optional)" FORCE)
