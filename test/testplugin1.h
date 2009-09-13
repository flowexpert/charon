#ifndef __TESTPLUGIN1_H__
#define __TESTPLUGIN1_H__

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef testplugin1_EXPORTS
///Visual C++ specific code
#define testplugin1_DECLDIR __declspec(dllexport)
#else
#define testplugin1_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define testplugin1_DECLDIR
#endif

#include <iostream>
#include "ParameteredObject.hxx"

class testplugin1_DECLDIR TestPlugin1: public ParameteredObject {
public:
	std::string testString;
	TestPlugin1(const std::string & name = "");
};

#endif /* __TESTPLUGIN1_H__ */
