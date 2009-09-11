#ifndef __TESTPLUGIN2_HXX__
#define __TESTPLUGIN2_HXX__

#include "testplugin2.h"

template<typename T>
TestPlugin2<T>::TestPlugin2(const std::string & name) :
	TemplatedParameteredObject<T> ("testplugin2", name, "Test Plugin 2") {
}

template<typename T>
void TestPlugin2<T>::execute() {
	ParameteredObject::execute();
	std::cout << t1.testString << std::endl;
}

#endif /* __TESTPLUGIN2_HXX__ */
