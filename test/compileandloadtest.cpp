#include "PluginManager.h"
#include <stdio.h>
#ifdef MSVC
#include <windows.h>
#endif

#ifndef   PLUGIN_DIR
#warning "PLUGIN_DIR not defined"
#endif
#ifndef   TESTPLUGIN_SOURCE_DIR
#warning "TESTPLUGIN_SOURCE_DIR not defined"
#endif

int main() {
//Compile and load fails if compiled as debug with Visual Studio. Plug-ins are always compiled as
//Release and a Debug compiled main application cannot load Release compiled Plug-ins.
#if !defined(_DEBUG) || !defined(MSVC)
#ifdef MSVC
	std::cout << "Copying " << (FileTool::getCurrentDir() +	"\\..\\src\\Release\\charon-core.lib").c_str() 
		<< " to " << (FileTool::getCurrentDir() + "\\charon-core.lib").c_str() << std::endl;

	if(!CopyFile((FileTool::getCurrentDir() + 	"\\..\\src\\Release\\charon-core.lib").c_str(),
		(FileTool::getCurrentDir() + "\\charon-core.lib").c_str(), false)) {
		std::cout << "Error on copying library file, error #" << GetLastError() << std::endl;
		return -1;
	}
#endif /* MSVC */
	try{
	PluginManager man(PLUGIN_DIR);

	man.compileAndLoadPlugin(TESTPLUGIN_SOURCE_DIR "/testplugin1.cpp");

	std::vector<std::string> v;
	v.push_back("testplugin1");
	man.compileAndLoadPlugin(TESTPLUGIN_SOURCE_DIR "/testplugin2.cpp", v);

	man.createInstance("testplugin2")->execute();
	} catch (AbstractPluginLoader::PluginException e) {
		std::cout << e.what() << std::endl;
		return -1;
	} catch (std::string s) {
		std::cout << s << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Some error occurred." << std::endl;
		return -1;
	}

	remove(PLUGIN_DIR "/libtestplugin1.dylib");
	remove(PLUGIN_DIR "/libtestplugin1.wrp");
	remove(PLUGIN_DIR "/libtestplugin2.dylib");
	remove(PLUGIN_DIR "/libtestplugin2.wrp");
	
#else
	std::cout << "Compile and load does not with Visual C++ when compiled as Debug." << std::endl;
#endif /* !defined(_DEBUG) || !defined(MSVC) */

	return 0;
}
