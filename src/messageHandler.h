/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file	messageHandler.h
 *	@brief	Provides message handler for error messages.
 *	To be used when initializing a Qt application.
 *
 *	Usage:
 *	@code
 *	#include <QApplication>
 *	#include "messageHandler.h"
 *	int main(int argc, char* argv[])
 *	{
 *		qInstallMsgHandler(messageHandler);
 *		QApplication app(argc, argv);
 *		...
 *		return app.exec();
 *	}
 *	@endcode
 *	@date	15.08.2008
 *	@author	jmg
 */

#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

#include <QtGlobal>

/**	handler to display debug or error messages within
 *	a Qt Application. See documentation of messageHandler.h
 *	for an usage example.
 *	@param type		Message type
 *	@param msg		Message content
 */
void messageHandler(QtMsgType type, const char* msg);

#endif /* MESSAGEHANDLER_H_ */
