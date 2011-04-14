/*	Copyright (C) 2011 Jonathan Wuest

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
/** @file   NodeView.h
 *  @brief  Declaration of class NodeView
 *  @date   15.03.2011
 *  @author <a href="mailto:wuest.jonathan@gmail.com">Jonathan Wuest</a>
 */

#ifndef NODEVIEW_H
#define	NODEVIEW_H

#include <QGraphicsView>
#include "NodeHandler.h"

class GraphModel;
/// QGraphicsView that uses NodeHandler as QGraphicsScene
class NodeView : public QGraphicsView {
	Q_OBJECT

public:
	/// default constructor
	/// @param parent         parent QWidget
	/// @param classesFile    filename of the classesFile
	NodeView(QWidget* parent, QString classesFile="");

	/// @returns the set GraphModel
	GraphModel* model();

public slots:
	/// Load ParameterFile and display content.
	/// The file to load can be specified in an openFile
	/// dialog.
	/// \param fileName     file to open (file dialog if empty)
	bool load(const QString& fileName = "");

	/// does nothing
	void save();

	/// sets the GraphModel
	/// @param model    GraphModel to set
	void setModel(GraphModel* model);

	/// updates the display
	virtual void updateDisplay();
private:
	/// NodeHandler scene
	NodeHandler* _nodehandler;

	/// the current set GraphModel
	GraphModel* _model;
};

#endif	/* NODEVIEW_H */

