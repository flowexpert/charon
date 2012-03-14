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
/** \file FileReaderWatcher.cpp
 *  Definition of the FileReaders's watcher for file changes.
 *  \author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">
 *      Michael Baron</a>
 *  \date 13.03.2011
 */

#include <charon-core/ParameteredObject.h>
#include <charon-utils/FileReaderWatcher.h>
#include <charon-core/SplitStream.h>

FileReaderWatcher::FileReaderWatcher( ParameteredObject* fileReader )
	: QFileSystemWatcher(), _fileReader(fileReader)
{
	Q_ASSERT(fileReader);
	connect(this, SIGNAL(fileChanged(QString)), SLOT(resetFileReader()));
}

void FileReaderWatcher::setFilename( QString fn ) {
	Parameter<bool>& watchable = dynamic_cast< Parameter<bool>& >(
			_fileReader->getParameter("watchable"));
	if (watchable()) {
		// watched files have to be removed on filename change
		// to avoid signals on changes of the old file
		this->removePath(fn);
		this->addPath(fn);
	}
}

void FileReaderWatcher::resetFileReader()
{
	_fileReader->resetExecuted();
	sout << "(DD) FileReaderWatcher::resetFileReader() called !" << std::endl;
}

