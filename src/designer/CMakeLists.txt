QT4_WRAP_CPP(DESIG_MOC_SRC
	QFilterWidget.h
)

SET(DESIG_CPP_SRC
	QFilterWidget.h
	QFilterWidget.cpp
)

ADD_LIBRARY(designer-widgets ${DESIG_CPP_SRC} ${DESIG_MOC_SRC})
SET_TARGET_PROPERTIES(designer-widgets PROPERTIES FOLDER "Tuchulcha")

# add install target for library
INSTALL(TARGETS designer-widgets
	EXPORT tuchulcha
	DESTINATION lib${LIB_SUFFIX}
	COMPONENT   libraries
)
INSTALL(DIRECTORY .
	DESTINATION include/tuchulcha/designer
	COMPONENT   libraries
	FILES_MATCHING PATTERN "*.h"
)
