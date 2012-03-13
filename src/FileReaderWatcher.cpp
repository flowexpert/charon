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

#include <charon-utils/FileReaderWatcher.h>

#include <QString>
#include <iostream>

FileReaderWatcher::FileReaderWatcher( ParameteredObject *fileReader )
	: QFileSystemWatcher()
{
	_fileReader = fileReader;
	connect( this, SIGNAL( fileChanged( const QString & ) ),
	         this, SLOT( resetFileReader() ) );
}

void FileReaderWatcher::setFilename( const std::string &fn )
{
//	(!!) watched files have to be removed on filename change (!!)
//	this->removePaths( this->files() );  //  (!!) This does not work!
	Parameter< bool > &watchable = dynamic_cast<Parameter< bool >& >(_fileReader->getParameter("watchable"));
	if (watchable()) {
		this->removePath( QString::fromStdString( fn ) );
		this->addPath( QString::fromStdString( fn ) );
	}
}

void FileReaderWatcher::resetFileReader()
{
	_fileReader->resetExecuted();
	sout << "(DD) FileReaderWatcher::resetFileReader() called !" << std::endl;
//	(!!) timeout handling necessary,
//      (!!) since files are being written continously
}

