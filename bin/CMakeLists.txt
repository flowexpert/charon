# this file has to be in the bin directory because the test executables
# are located in the ${PROJECT_BINARY_DIR}/bin

IF(ENABLE_TEST)
	ADD_TEST(ImgManipTest imgmaniptest)
	ADD_TEST(ImgToolTest imgtooltest)
ENDIF(ENABLE_TEST)