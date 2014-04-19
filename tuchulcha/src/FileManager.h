/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file   FileManager.h
 *  \brief  Declaration of class FileManager
 *  \date   02.12.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <QString>
#include <QDir>
#include <QObject>

class QWidget;

/// Common config file information and handling.
/** This class handles creation of a config directory if not avaiable.
 *  If no config directory exists, a new one is created and populated
 *  with a dummy sampleclass file.
 */
class FileManager : public QObject {
	Q_OBJECT

public:
	/// get a reference to the file Manager instance
	static const FileManager& instance();

	/// parent widget for operation result dialogs
	/** (e.g. when upating plugins etc.) */
	static QWidget* dialogParent;

	/// get the path of the classes file
	QString classesFile() const;

	/// get config directory
	QDir configDir() const;

private:
	FileManager();
	/// forbid copying
	Q_DISABLE_COPY(FileManager)
	virtual ~FileManager();

	/// pointer to FileManager instance
	static FileManager* _inst;
};

#endif /*FILEMANAGER_H_*/
