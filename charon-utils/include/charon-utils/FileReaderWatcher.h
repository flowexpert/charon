/*  Copyright (C) 2012 The regents of the University of Heidelberg

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  Declaration of the FileReader's watcher for file changes.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.03.2011
 */

#ifndef _FILEREADER_WATCHER_H
#define _FILEREADER_WATCHER_H

#include <QtCore/QFileSystemWatcher>
class ParameteredObject;

#if defined(MSVC) && defined(HANDLE_DLL)
#ifdef filereader_EXPORTS
///Visual C++ specific code
#define filereader_DECLDIR __declspec(dllexport)
#else
#define filereader_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
///Not needed with GCC
#define filereader_DECLDIR
#endif

/// watcher to notify file changes
class filereader_DECLDIR FileReaderWatcher
	: public QFileSystemWatcher
{
	Q_OBJECT
public:
	/// set up watcher and connect to given FileReader
	FileReaderWatcher( ParameteredObject *fileReader );
	/// filename to watch
	void setFilename( QString fn );
public slots:
	/// handle file change event
	void resetFileReader();
private:
	/// connection to parent FileReader
	ParameteredObject *_fileReader;
};

#endif

