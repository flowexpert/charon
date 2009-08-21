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
/**	@file	messageHandler.cpp
 *	@brief	Implementation of message handler.
 *	@date	15.08.2008
 *	@author	jmg
 */

#include "messageHandler.h"
#include <QMessageBox>
#include <iostream>
#include "defines.h"

void messageHandler(QtMsgType type, const char *msg) {
	switch (type) {
	case QtDebugMsg:
		#ifndef MH_NO_DEBUG_OUTPUT
		//QMessageBox::warning(0, "Debug Message", msg);
		std::cout << msg << std::endl;
		#endif // MH_NO_DEBUG_OUTPUT
		break;
	case QtWarningMsg:
		#ifndef MH_NO_WARNING_OUTPUT
		QMessageBox::warning(0, "Warning Message", msg);
		#endif // MH_NO_WARNING_OUTPUT
		break;
	case QtCriticalMsg:
		QMessageBox::critical(0, "Critical Warning Message", msg);
		break;
	case QtFatalMsg:
		QMessageBox::critical(0, "Fatal Warning Message", msg);
		abort();
	}
}
