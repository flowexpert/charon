/*  This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
/**	@file	FileTool.cpp
 *	@brief	Implementation of class FileTool.
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @author <a href="mailto:bc002@ix.urz.uni-heidelberg.de">Cornelius Ratsch</a>
 *	@date	11.08.2008
 *	@remark	Based on FileTool.hxx by Daniel Kondermann.
 */

#include "FileTool.h"

#if defined(sun)         || defined(__sun)       || defined(linux) \
 || defined(__linux)     || defined(__linux__)   || defined(__CYGWIN__) \
 || defined(BSD)         || defined(__FreeBSD__) || defined(__OPENBSD__) \
 || defined(__MACOSX__)  || defined(__APPLE__)   || defined(sgi) \
 || defined(__sgi)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define MODE ,0711
#define CHARON_LINUX
#elif defined(_WIN32) || defined(__WIN32__)
#ifdef _MSC_VER
#include <crtdbg.h>
#endif /* _MSC_VER */
#include <direct.h>
#include <io.h>
#include <windows.h>

#define MODE
#define mkdir _mkdir
#define unlink _unlink
#define chdir _chdir
#define getcwd _getcwd
#define CHARON_WINDOWS
#else
#error "Operating system not recognized!"
#endif

#include "StringTool.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cassert>

#include <iostream>

#ifdef CHARON_LINUX
const char FileTool::slash = '/';
#else
const char FileTool::slash = '\\';
#endif

FileTool::FileTool() {
}

FileTool::~FileTool() {
}

int FileTool::makePath(std::string& path) {
	slashConvert(path);
	std::vector<std::string> dirs;
	StringTool::explode(path, slash, dirs);

	std::string cdir = "";
	for (unsigned int i = 0; i < dirs.size() - 1; ++i) {
		if (i)
			cdir += slash + dirs[i];
		else
			cdir += dirs[i];
		makeDir(cdir);
	}
	return makeDir(cdir + slash + dirs[dirs.size() - 1]);
}

int FileTool::makeDir(const std::string& dir) {
	return mkdir(dir.c_str() MODE);
}

int FileTool::changeDir(const std::string& dir) {
	return chdir(dir.c_str());
}

std::string FileTool::getCurrentDir() {
	char cwd[2048];
	char* res;
	res = getcwd(cwd, 2048);
	// res should now be the same as cwd.
	// On error, res is NULL, cwd undefined
	assert(res);
	return std::string(cwd);
}

std::vector<std::string> FileTool::getFilesWithSuffix(std::string suffix) {
	std::vector<std::string> v;
#ifdef CHARON_LINUX
	DIR *hdir;
	struct dirent *entry;

	hdir = opendir(".");
	do {
		entry = readdir(hdir);
		if (entry) {
			std::string s = entry->d_name;
			if (s.find_last_of('.') != s.npos && s.substr(s.find_last_of('.'),
					s.length() - s.find_last_of('.')) == suffix) {
				v.push_back(s);
			}
		}
	} while (entry);
	closedir(hdir);
#elif defined CHARON_WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind;
	//DWORD dwError=0;

	hFind = FindFirstFile(".\\*", &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		throw (std::string("Error in opening the folder."));
	}

	do {
		std::string s = ffd.cFileName;
		if (s.find_last_of('.') != s.npos && s.substr(s.find_last_of('.'),
				s.length()) == suffix) {
			v.push_back(s);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

#endif
	return v;
}

void FileTool::slashConvert(std::string& src) {
#ifdef CHARON_LINUX
	char wrongSlash = '\\';
#else
	char wrongSlash = '/';
#endif
	replace(src.begin(), src.end(), wrongSlash, slash);
}

bool FileTool::exists(const std::string& file) {
	std::ifstream test(file.c_str());
	return !test.fail();
}

int FileTool::remove(const std::string& file) {
	return unlink(file.c_str());
}

int FileTool::rename(const std::string& oldFile, const std::string& newFile) {
	return ::rename(oldFile.c_str(), newFile.c_str());
}

