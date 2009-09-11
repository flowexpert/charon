#ifndef __TESTPLUGIN1_HXX__
#define __TESTPLUGIN1_HXX__

#include "testplugin1.h"

TestPlugin1::TestPlugin1(const std::string & name) :
	ParameteredObject("testplugin1", name, "Test Plugin 1"), testString(
			"test string") {
}

#endif /* __TESTPLUGIN1_HXX__ */
