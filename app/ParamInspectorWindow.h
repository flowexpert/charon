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
/**	@file ParamInspectorWindow.h
 *	Test displaying a ParameterFileModel.
 *	@date 08.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef PARAMINSPECTORWINDOW_H_
#define PARAMINSPECTORWINDOW_H_

#include <QMainWindow>
#include <QString>

class QMenu;
class ObjectInspector;

/// Main Window to display a HierarchyModel using a QTreeView.
class ParamInspectorWindow : public QMainWindow
{
	Q_OBJECT

public:
	ParamInspectorWindow();
	
public slots:
	/// open file and update window title
	void openFile();

private:
	QMenu*	fileMenu;				///< File menu
	ObjectInspector* _inspector;	///< Inspector widget
};

#endif /* PARAMINSPECTORWINDOW_H_ */
