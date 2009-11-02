INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

QT4_WRAP_UI(UI_SRC mainwindow.ui)

SET(APP_SRC
	main.cpp
	mainwindow.cpp
	mainwindow.h
	${UI_SRC}
)

QT4_AUTOMOC(${APP_SRC})

ADD_EXECUTABLE(templategenerator ${APP_SRC})
TARGET_LINK_LIBRARIES(templategenerator
	mylib
	${QT_LIBRARIES}
)
