#include "PluginManager.h"
#include <stdexcept>
#ifdef MSVC
#include <windows.h>
#endif

#ifndef   PLUGIN_DIR
/// charon-utils plugin path (in bild directory)
#define   PLUGIN_DIR
#warning  PLUGIN_DIR not defined
#endif
#ifndef   TESTPLUGIN_SOURCE_DIR
/// path where test files are located
#define   TESTPLUGIN_SOURCE_DIR
#warning  TESTPLUGIN_SOURCE_DIR not defined
#endif

int main() {
// Compile and load fails if compiled as debug with Visual Studio.
// Plug-ins are always compiled as Release and a Debug compiled main
// application cannot load Release compiled Plug-ins.
#if !defined(_DEBUG) || !defined(MSVC)
#ifdef MSVC
	std::cout << "Copying " << (FileTool::getCurrentDir()
			+ "\\..\\src\\Release\\charon-core.lib").c_str()
			<< " to " << (FileTool::getCurrentDir() + "\\charon-core.lib")
			.c_str() << std::endl;

	if(!CopyFile((FileTool::getCurrentDir() +
			"\\..\\src\\Release\\charon-core.lib").c_str(),
			(FileTool::getCurrentDir()+"\\charon-core.lib").c_str(), false)) {
		std::cout << "Error on copying library file, error #"
				<< GetLastError() << std::endl;
		return -1;
	}
#endif /* MSVC */
	try{
		std::cout << "Using plugin directory " << PLUGIN_DIR << std::endl;
		PluginManager man(PLUGIN_DIR);

		std::string testfile = TESTPLUGIN_SOURCE_DIR "/testplugin1.cpp";
		std::cout << "Trying to compile and load " << testfile << std::endl;
		man.compileAndLoadPlugin(testfile);

		std::vector<std::string> v;
		v.push_back("testplugin1");
		testfile = TESTPLUGIN_SOURCE_DIR "/testplugin2.cpp";
		std::cout << "Trying to compile and load " << testfile << std::endl;
		man.compileAndLoadPlugin(testfile, v);
		ParameteredObject* instance = man.createInstance("testplugin2");
		std::cout << "Executing testinstance" << std::endl;
		instance->execute();
		std::cout << "Number of instances: "
				<< man.getInstancesCount() << std::endl;
		std::cout << "Destroying testinstance" << std::endl;
		man.destroyInstance(instance);
		std::cout << "Number of instances instances: "
				<< man.getInstancesCount() << std::endl;
		assert(man.getInstancesCount() == 0);
	}
	catch (const std::exception& e) {
		std::cout << "Got exception of type " << typeid(e).name() << ":\n";
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::string& s) {
		std::cout << "Got exception of type std::string:\n";
		std::cout << s << std::endl;
		return EXIT_FAILURE;
	}
	catch (...) {
		std::cout << "Some error occurred." << std::endl;
		return EXIT_FAILURE;
	}

	try {
		remove(PLUGIN_DIR "/libtestplugin1.dylib");
		remove(PLUGIN_DIR "/libtestplugin1.wrp");
		remove(PLUGIN_DIR "/libtestplugin2.dylib");
		remove(PLUGIN_DIR "/libtestplugin2.wrp");
		TypeDetector::destroy();
	}
	catch (...) {
		std::cout << "Error during cleanup." << std::endl;
		return EXIT_FAILURE;
	}
#else
	std::cout << "Compile and load does not work with Visual C++ when "
			<< "compiled as Debug." << std::endl;
#endif /* !defined(_DEBUG) || !defined(MSVC) */

	return EXIT_SUCCESS;
}
