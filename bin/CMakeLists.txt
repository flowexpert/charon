# Tests for Tuchulcha are located in the bin directory
IF(ENABLE_TEST)
    ADD_TEST(GraphTest graphtest)
    ADD_TEST(ModelTest modeltest)
	ADD_TEST(GraphTempTest graphtemptest)
ENDIF(ENABLE_TEST)
