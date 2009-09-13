# this file has to be in the bin directory because the test executables
# are located in the ${PROJECT_BINARY_DIR}/bin

IF(ENABLE_TEST)
	ADD_TEST(StreamTest streamtest)
	ADD_TEST(ParamTest paramtest)
	ADD_TEST(ParameterTest parametertest)
	ADD_TEST(CompileAndLoadTest compileandloadtest)
ENDIF(ENABLE_TEST)