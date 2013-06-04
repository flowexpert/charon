QT4_WRAP_CPP(DESIG_MOC_SRC
	filterwidget_p.h
)

SET(DESIG_CPP_SRC
	shared_global_p.h
	iconloader_p.h
	iconloader.cpp
	filterwidget_p.h
	filterwidget.cpp
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
