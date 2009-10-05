# this file has to be in the bin directory because the test executables
# are located in the ${PROJECT_BINARY_DIR}/bin

IF(ENABLE_TEST)
	ADD_TEST(ImgManipTest imgManipTest)
	ADD_TEST(ImgToolTest imgToolTest)
	ADD_TEST(PluginTest pluginTest)
ENDIF(ENABLE_TEST)
