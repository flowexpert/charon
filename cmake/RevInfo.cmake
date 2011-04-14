OPTION(USE_VCS "Try to determine VCS revision info" ON)
IF(USE_VCS)
	IF(NOT tuchulcha_VCS_DIR)
		SET(tuchulcha_VCS_DIR "${PROJECT_SOURCE_DIR}")
	ENDIF()
	SET(tuchulcha_VCS_DIR "${tuchulcha_VCS_DIR}"
		CACHE PATH "Directory to query VCS information from" FORCE)

	# determine project svn information if possible
	IF(SVNCOMMAND)
		EXECUTE_PROCESS(
			COMMAND ${SVNCOMMAND} info ${tuchulcha_VCS_DIR}
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
				"^.*/tuchulcha/(.*)$" "\\1"
				SVN_BRANCH "${SVN_WC_URL}"
			)
			SET(tuchulcha_VCS_INFO
				"svn r${SVN_WC_REVISION} (${SVN_BRANCH})"
			)
		ENDIF()
	ENDIF(SVNCOMMAND)

	# determine project bzr information if possible
	IF(BZRCOMMAND)
		EXECUTE_PROCESS(
			COMMAND ${BZRCOMMAND} revno "${tuchulcha_VCS_DIR}"
			OUTPUT_VARIABLE BZR_REV
			ERROR_VARIABLE BZR_REV_ERROR
			RESULT_VARIABLE BZR_REV_RESULT
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		IF(${BZR_REV_RESULT} EQUAL 0)
			SET(tuchulcha_VCS_INFO
				"bzr r${BZR_REV}"
			)
		ENDIF()
	ENDIF(BZRCOMMAND)

	# Info from VCS
	SET(tuchulcha_VCS_INFO "${tuchulcha_VCS_INFO}"
		CACHE STRING "VCS information (set automatically)" FORCE)
ENDIF(USE_VCS)

# Option to add custom information
SET(tuchulcha_BUILD_INFO "${tuchulcha_BUILD_INFO}"
	CACHE STRING "Build information (optional)" FORCE)

