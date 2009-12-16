/*  Copyright (C) 2009 Jens-Malte Gottfried

 This file is part of Tuchulcha.

 Tuchulcha is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Tuchulcha is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file   FileManager.h
 *  @brief  Declaration of class FileManager
 *  @date   02.12.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <QString>
#include <QDir>
#include <AbstractPluginLoader.h>
#include <QObject>

class QWidget;

/// this class handles creation of a config directory if not avaiable.
/// If no config directory exists, a new one is created and populated
/// with a dummy sampleclass file.
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

	/// get a filename for a temporary file
	QString tempFileName() const;

	/// get config directory
	QDir configDir() const;

	/// Reloads all plugins and creates metadata information
	void loadPluginInformation() const;

	/// Puts existing metadata information together
	void updateMetadata() const;

	/// Configures plugin paths
	/// @param parent Parent Qwidget for dialog boxes
	/// @param force  Configure even if a configuration file exists
	void configure(QWidget * parent = NULL, bool force = false) const;

	/// Compiles a plugin and creates metadata information
	/// Asks for referenced plugins
	/// @param parent Parent QWidget for dialog boxes
	/// @return False, if an error occurred
	bool compileAndLoad(QWidget * parent = NULL) const
			throw (AbstractPluginLoader::PluginException);

	/**
	 * Returns the global plugin path
	 * @return Global plugin path
	 */
	std::string getGlobalPluginPath() const;

	/**
	 * Returns the private plugin path
	 * An empty string is returned if no path is specified
	 * @return Private plugin path
	 */
	std::string getPrivatePluginPath() const;

private:
	FileManager();

	virtual ~FileManager();

	/// @name forbid copying
	//	@{
	FileManager(const FileManager&);
	FileManager& operator=(const FileManager&);
	//	@}

	/**
	 * Returns the path where the Paths.config file should be
	 * @return Path to the file Paths.config
	 */
	inline std::string _paramFile() const;

	/**
	 * Returns the path to the metadata files.
	 * @return Path to the metadata files.
	 */
	inline std::string _metaPath() const;

	/**
	 * @return True, if a private plugin path is specified
	 */
	bool _isPrivatePluginPathSet() const;

	/**
	 * @return Path to the charon-utils install
	 */
	std::string _charonUtilsInstall() const;

	/**
	 * @return Path to the charon-core install
	 */
	std::string _charonCoreInstall() const;

	/// pointer to FileManager instance
	static FileManager* _inst;
};

#endif /*FILEMANAGER_H_*/
