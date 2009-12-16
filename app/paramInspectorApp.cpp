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
/**	@file paramInspectorApp.cpp
 *	GUI Application for ParameterFile manipulation.
 *	@date 08.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QApplication>
#include <QErrorMessage>

#include "ParamInspectorWindow.h"
#include "FileManager.h"

/**	Start of test application.
 * @param argc	Counter of command line arguments.
 * @param argv	Content of command line arguments.
 * @return		0 in case of sucessful execution.
 */
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	ParamInspectorWindow window;
	FileManager::dialogParent = &window;
	window.resize(320, 480);
	window.show();
	QErrorMessage* handler = QErrorMessage::qtHandler();
	//handler->setParent(FileManager::dialogParent);
	handler->setModal(true);
	return app.exec();
}
