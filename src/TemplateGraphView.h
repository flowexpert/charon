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
/**	@file	TemplateGraphView.h
 *	@brief	Declaration of class TemplateGraphView
 *
 *	@date	21.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef TEMPLATEGRAPHVIEW_H_
#define TEMPLATEGRAPHVIEW_H_

#include "GraphView.h"

/// Graph view with drag handling specialized for display of
/// class templates.
class TemplateGraphView : public GraphView {
    Q_OBJECT

public:
    /// Default constructor.
    /// @param parent		parent widget
    /// @param classesFile  metadata to use, use file manager if empty
    TemplateGraphView(QWidget* parent = 0, QString classesFile = "");

    /// default destructor
    virtual ~TemplateGraphView();

signals:
    /// add new node of given kind to flow graph
    /// @param className	name of the class to create a new node
    void addNode(const QString& className) const;

    /// show documentation page
    /// @param fileName		fileName of doc page
    void showDocPage(const QString& fileName);

    /// show documentation page
    /// @param className	class name to generate the documentation for
    void showClassDoc(const QString& className);

protected:
    /// Render graph and update display
    ///	@param graph	Graph to render
    virtual void _renderGraph(graph_t* graph);

    /// @name Input device handling.
    //	@{

    /// Mouse press event handler.
    /// Clicking on a dummy object shows class documentation.
    /// @param event		mouse event
    virtual void mousePressEvent(QMouseEvent* event);

    /// Mouse move event handler.
    /// When the mouse is moved with pressed left button and the distance
    /// is beyond the system defined start drag distance, a drag is emitted
    /// with the current slot (if there is any).
    /// @param event		mouse event
    virtual void mouseMoveEvent(QMouseEvent* event);

    /// Mouse press event handler.
    /// Graph nodes can be renamed by double clicking on it.
    /// @param event		mouse event
    virtual void mouseDoubleClickEvent(QMouseEvent* event);

    //	@} /* input device handling */
};

#endif /*TEMPLATEGRAPHVIEW_H_*/
