/*	Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.
    
    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/**	@file FlowWidget.h
 *	@brief Declaration of class FlowWidget
 *
 *	@date	27.08.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef FLOWWIDGET_H_
#define FLOWWIDGET_H_

#include <QMdiSubWindow>

class QGraphicsView;
class NodeHandler;
class GraphModel;

///	Widget to display data flow diagram.
///	Contains a graphics scene and viewer with flowchart content.
class FlowWidget : public QMdiSubWindow {
	Q_OBJECT

public:
	/// Default constructor.
	/** \param model         model to be used within this flow widget
	 *  \param parent        pointer to parent widget
	 */
	FlowWidget(GraphModel* model, QWidget* parent = 0);

	/// Get used model
	GraphModel* model();

	/// const version
	const GraphModel* model() const;

public slots:
	/// Save flowchart diagram to graphics file.
	void saveFlowChart() const;

	/// zoom in
	void zoomIn();

	/// zoom out
	void zoomOut();

	/// fit in view
	void zoomFit();

	/// fit in view after widget resize
	void zoomFit(Qt::WindowStates oldState, Qt::WindowStates newState);

	/// update title and main window file information
	void updateFileName(const QString& fileName /** new filename*/);

	/// set modified flag
	void modify(bool val = true /** [in] new value*/);

signals:
	/// Inform about changed model
	/// @param model        new model
	void modelChanged(GraphModel* model);

	/// Send status message.
	/// @param msg          message
	void statusMessage(QString msg) const;

	/// Send type of currently selected node
	/// @param type         type
	void nodeTypeSelected(QString type) const;

protected:
	/// viewer
	QGraphicsView* _viewer;
	/// graphics scene to display
	NodeHandler* _nodehandler;

	/// check modification status and ask to save before closing
	virtual void closeEvent(QCloseEvent* closeEvent);

	/// zooms in and out on mousewheel event
	virtual void wheelEvent(QWheelEvent* event);
};

#endif /* FLOWWIDGET_H_ */
