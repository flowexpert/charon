IF(ENABLE_TEST)
	IF(COMPILE_BLOCKMATCHING)
	
		ADD_TEST(listedpixelselectiontest listedpixelselectiontest)
	
	ENDIF(COMPILE_BLOCKMATCHING)
	
	IF(COMPILE_SOLVER)
		
		ADD_TEST(petscsolvertest petscsolvertest)
		
	ENDIF(COMPILE_SOLVER)
ENDIF(ENABLE_TEST)
