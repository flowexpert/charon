/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file CharonRun.h
 *  \brief Declaration of class CharonRun
 *
 *  \date 13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef CHARONRUN_H
#define CHARONRUN_H

#include <QObject>
class PluginManager;

/// workflow execution and plugin update management
class CharonRun : public QObject
{
	Q_OBJECT
public:
	/// constructor
	/** \param parent parent object */
	explicit CharonRun(QObject* parent = 0);
	~CharonRun();

public slots:
	/// update plugin information
	void updatePlugins();
	/// run given workflow file
	/** \param fName  workflow filename */
	void runWorkflow(QString fName);
	/// update dynamic plugins of given workflow file
	/** \param fName  workflow filename */
	void updateDynamics(QString fName);

	/// unlock exiting, quit application if all tasks are completed
	void exitWhenFinished();

signals:
	/// signal to avoid new command prompt
	void busy();
	/// show that running tasks are complete
	void ready();

private:
	/// plugin manager
	PluginManager* _man;
	/// logger
	std::ofstream* _log;
	/// setup plugin manager
	void _setupMan(QString logFileName);
	/// release plugin manager
	void _freeMan();

private slots:
	/// increase task counter
	void _taskStart();
	/// decrease task counter
	void _taskFinished();
};

#endif // CHARONRUN_H
