#ifndef __TESTPLUGIN2_H__
#define __TESTPLUGIN2_H__

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef testplugin2_EXPORTS
///Visual C++ specific code
#define testplugin2_DECLDIR __declspec(dllexport)
#else
#define testplugin2_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define testplugin2_DECLDIR
#endif

#include <iostream>
#include "ParameteredObject.hxx"
#include "testplugin1.h"

template<typename T>
class testplugin2_DECLDIR TestPlugin2: public TemplatedParameteredObject<T> {
public:
	TestPlugin1 t1;
	TestPlugin2(const std::string & name = "");

	void execute();
};

#endif /* __TESTPLUGIN2_H__ */
