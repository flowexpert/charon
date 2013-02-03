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
/** \file   CommunicationHandler.h
 *  \brief  Declaration of class CommunicationHandler
 *  \date   13.11.2011
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef COMM_HANDLER_H
#define COMM_HANDLER_H

#include <QThread>
#include <QStringList>

/// handle command line arguments and process communication
class CommunicationHandler : public QThread
{
	Q_OBJECT

public:
	/// constructor with cmdline arguments
	/** \param args    command line arguments
	 *  \param parent  parent object
	 */
	explicit CommunicationHandler(
		const QStringList& args, QObject* parent = 0);

signals:
	/// update module metadata
	void updatePlugins();
	/// run given workflow file
	/** \param fName   workflow filename */
	void runWorkflow(QString fName);
	/// update dynamic plugins of given workflow file
	/** \param fName   workflow filename */
	void updateDynamics(QString fName);

protected:
	/// thread execution code
	virtual void run();

private:
	/// enter interactive loop
	bool _interactive;
	/// avoid info output
	bool _quiet;
	/// cmd line argument cache
	QStringList _args;
};

#endif // COMM_HANDLER_H
