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
/**	@file	GraphView.h
 *	@brief	Declaration of class GraphView
 *
 *	@date	30.10.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef GRAPHVIEW_H_
#define GRAPHVIEW_H_

#include <QGraphicsView>
#include <QList>
#include <QColor>
#include <graphviz/types.h>

class GraphModel;
class GraphBuilder;

/// Graphvics view with drag and drop handling on graph visualisation.
class GraphView : public QGraphicsView {
    Q_OBJECT

public:
    /// Default constructor.
    /// @param parent		parent widget
    /// @param classesFile  metadata to use, use file manager if empty
    GraphView(QWidget* parent = 0, QString classesFile = "");

    /// default destructor
    virtual ~GraphView();

    /// get the connected model
    GraphModel* model();

    /// const version
    const GraphModel* model() const;

public slots:
    ///	Load ParameterFile and display content.
    ///	The file to load can be specified in an openFile
    ///	dialog.
    void load();

    /// connect with the given GraphModel
    /// @param model		model to connect with
    void setModel(GraphModel* model);

    /// Save view content to graphics file.
    void save() const;

    /// Update display.
    /// This calls renderGraph with the graph given by the
    /// used graph builder.
    virtual void updateDisplay();

signals:
    /// Inform about changed model
    ///	@param model	new model
    void modelChanged(GraphModel* model);

    /// Send status message.
    /// @param msg			message
    /// @param timeout		time to show the message
    void statusMessage(const QString& msg, int timeout) const;

protected:
    /// Mouse press event handler.
    /// This manages item selection and saves the event position to be able
    /// to recognize dragging.
    /// @param event		mouse event
    virtual void mousePressEvent(QMouseEvent* event);

    /// Render graph and update display
    ///	@param graph	Graph to render
    virtual void _renderGraph(graph_t* graph) = 0;

    /// graphviz context (for graph rendering)
    GVC_t* _gvc;

    /// Drag start position.
    QPoint _dragStartPosition;

    /// Connected model
    GraphModel* _model;

    /// Handle graph creation
    GraphBuilder* _builder;

    /// Class has Ownership of _model
    bool _ownModel;
};

#endif /*GRAPHVIEW_H_*/
